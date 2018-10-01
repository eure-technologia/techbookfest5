
= まえがき

「Clojureで作るハイパフォーマンスSPA」を手にとって頂き、ありがとうございます。
本書は、著者が業務中に実践しているパフォーマンスに配慮したSPAをClojureで実現するまでの手法をまとめたものです。
応用的な内容のためある程度Clojureについての前提知識が必要かと思いますが、本書がClojureでWebフロントエンド開発をする際に少しでも役に立てれば幸いです。

//image[sample-spa-top][作成するサンプルアプリ][scale=0.5]{
//}

本書では、上の画像のようなGitHubクライアントアプリケーションの完成までの経緯を振り返りながら解説していきます。
完成後のものが https://sample-github-spa.now.sh にデプロイされているので、GitHubのアカウントがあれば是非一度お試しください。

また、本書の中で登場するサンプルコードは全てGitHub上のレポジトリsample-github-spa(https://github.com/boxp/sample-github-spa)に用意しております。
途中コードを用いて説明する際は、このレポジトリ内のソースコードのパスを明記しているので予めダウンロードしておいて適宜参照いただければ幸いです。

最後に、本書を執筆するにあたりアドバイスをくれた同僚達と編集作業をしてくれたfutaboooさん、並びに表紙絵を書いてくださったkarinさんにこの場を借りて深く感謝します。今執筆している文がこうして製本されて頒布されているのは間違いなく皆さんのお陰です。 重ね重ね感謝致します。　

それでは、次の章から本編へ移ります！　

= SPAに求められるパフォーマンス

SPA(Single Page Application)に関わらず、Webページのパフォーマンスを向上させる事はユーザー体験の向上を考える上でとても重要なことです。

例えば、AndroidアプリやiOSアプリなどのネイティブ版に併せてSP版（スマートフォンブラウザ向けのWeb版）も提供しているWebサービスなどでは、インストールに時間がかかりユーザーが離脱してしまうネイティブ版に変わってSP版がユーザーの玄関口としての役割を果たす事でしょう。

そうした状況ではページの表示速度や応答速度がユーザーの離脱率に直結し、パフォーマンスの改善による大きな効果が期待できます@<fn>{fn_why_performance_matters}。
//footnote[fn_why_performance_matters][パフォーマンス向上による恩恵についてより詳しく知りたければ、 Google によるこちらの記事をを一読する事を勧めます。 "Why Performance Matters" - https://developers.google.com/web/fundamentals/performance/why-performance-matters]

この、ユーザーに長時間のインストール時間を負担させる事もなく、ネイティブアプリに近いユーザー体験をユーザーにもたらすニーズを実現するものとして、GoogleがPWA(Progressive Web App)@<fn>{fn_pwa}と呼ばれるコンセプトを提唱しています。
//footnote[fn_pwa][残念ですが、この本ではスペースや著者の時間的な理由からPWAについての詳しい説明は省いています。詳しく知りたい場合は Google の "はじめてのプログレッシブウェブアプリ" - https://developers.google.com/web/fundamentals/codelabs/your-first-pwapp を読む事を勧めます。]

次の章からは、このコンセプトに沿って効果的にパフォーマンスを向上させるための手段を解説していきます。

= Code Splitting + LazyLoadを実現する

SPAを初めて表示する時のロード時間を短縮する手段として、Code Splitting（コードの分割）とLazyLoad（コードの遅延ロード）が有用です。

基本的に、SPAにおけるページ遷移では最小限のリソースのロードとクライアントサイドでの描画だけが行われ、高速にページ遷移をする事ができます。

しかし、どのページからアクセスするにしろユーザーが初めて表示するページだけは通常のWebページ同様表示に必要なコードをまとめてロードする必要があるため、コードのサイズが大きいと表示が完了するまで多くの待ち時間を作ってしまい、最悪ユーザーがページを開いた瞬間に直帰してしまう可能性もあります。

そこでコード分割とコードの遅延ロードを実装し、app-shell@<fn>{fn_appshell}と呼ばれる最小限表示に必要なコードを切り出して、ページ毎に必要なコードを遅延ロードさせるように実装して行きましょう。
//footnote[fn_appshell][https://developers.google.com/web/fundamentals/architecture/app-shell]

以下に必要な手順を書いていきます。

== Code Splitting

サンプルアプリでは以下の図のようにログイン画面・リポジトリ一覧画面・リポジトリ詳細画面・アクティビティ一覧画面・アクティビティ詳細画面の５つの画面に分かれています。

#@# ここに画面遷移図を置く

この内の一覧画面と詳細画面は頻繁にそれぞれへ遷移する画面なので一つのモジュールにまとめるとして、最終的に全画面共通のモジュール (app-shell) とレポジトリ画面、アクティビティ画面、ログイン画面の4つに分割します。

コードとして落とし込むと、 project.clj でのモジュール定義は以下の形になります。

//emlist[sample-github-spa/project.clj][clojure]{
(defn modules
  [output-dir]
  {:repository {:entries #{"sample-github-spa.repository.container"}
                :output-to (str output-dir "/repository.js")
                :depends-on #{:client}}
   :auth {:entries #{"sample-github-spa.auth.container"}
          :output-to (str output-dir "/auth.js")
          :depends-on #{:client}}
   :activity {:entries #{"sample-github-spa.activity.container"}
              :output-to (str output-dir "/activity.js")
              :depends-on #{:client}}
   ;; 分割されたモジュールをロードするために最低限必要なモジュール
   ;; モジュールの分割を行うと必ずこのモジュールが分割されるので出力先ファイル名だけ変更している
   :cljs-base {:output-to (str output-dir "/cljs_base.js")}
   :client {:entries #{"sample-github-spa.client"}
            :output-to (str output-dir "/app.js")
            :depends-on #{:cljs-base}}})
...
  :cljsbuild
  {:builds
   [{:id           "dev"
     :source-paths ["src/cljs" "src/cljs-client"]
     :figwheel     {:on-jsload "sample-github-spa.client/mount-root"}
     :compiler     {:main sample-github-spa.client
		    ...
                    :modules ~(modules "resources/public/js/compiled")}} ;; <- ここでモジュールを定義する
...
//}

このサンプルアプリではアプリケーション全体を@<code>{:repository}, @<code>{:auth}, @<code>{:activity}, @<code>{:cljs-base}, @<code>{:client}の5つのモジュールに分割しており、それぞれのモジュールの名前空間や依存関係、出力先ファイル名を定義しています。

本来はこの5つのうちの@<code>{:client}が app-shell にあたるモジュールですが、コメントにも書かれている通り動的なモジュールロードに必要な@<code>{:cljs-base}が必ず分割されるのでこちらも app-shell の一部になります。

これで Code Splitting に必要な設定は全てです。
続いて分割したコードを動的にロードする LazyLoad について解説していきます。

== LazyLoad

Clojure で LazyLoad を実現するためには、 ClojureScript（ Clojure から JS へのトランスパイラ）にのみ存在する標準 API の@<code>{cljs.loader/load}と@<code>{cljs.loader/set-loaded!}を利用します。

まずは@<code>{cljs.loader/set-loaded!}を LazyLoad したいコードの末尾で呼び出す必要があるため、以下のように書いています。

//emlist[sample-github-spa/src/cljs/sample_github_spa/repository/container.cljs][clojure]{
(ns sample-github-spa.repository.container
  (:require ...
            [sample-github-spa.util :as util]
            ...))
...
(util/universal-set-loaded! :repository)
//}

コードの中で登場するものは実際には@<code>{util/universal-set-loaded!}ですが、これはあくまでも@<code>{cljs.loader/set-loaded!}と同じ動作をするWrapperでファイルの末尾で@<code>{cljs.loader/set-loaded!}を実行する事となんら変わりありません（なぜ Wrapper を利用しているかについては後の SSR についての章で解説します）。

@<code>{cljs.loader/set-loaded!}を呼び出した事で、@<code>{cljs.loader/load}によるコードの LazyLoad が可能になります。
以下のコードを見てください。

//emlist[sample-github-spa/src/cljs/sample_github_spa/route.cljs][clojure]{
(ns sample-github-spa.route
  (:require
    [re-frame.core :as re-frame]
    [sample-github-spa.util :as util]
    [sample-github-spa.events :as events]
    [secretary.core :as secretary :refer-macros [defroute]]))
...
(def route-table
  {:login {:title "Login"
           :container #(resolve 'sample-github-spa.auth.container/box)
           :module-name :auth}
   :repository {:title "Repository"
                :container #(resolve 'sample-github-spa.repository.container/grid-box)
                :module-name :repository}
   :about-repository {:title "About Repository"
                      :container #(resolve 'sample-github-spa.repository.container/detail)
                      :module-name :repository}
   :activity {:title "Activity"
              :container #(resolve 'sample-github-spa.activity.container/timeline)
              :module-name :activity}
   :about-activity {:title "About Activity"
                    :container #(resolve 'sample-github-spa.activity.container/detail)
                    :module-name :activity}})

(defn- lazy-push
  [key params]
  (util/universal-load (-> route-table key :module-name) #(re-frame/dispatch-sync [::events/push key params])))

;; ルーティング定義
(defroute root-path "/" []
  (lazy-push :login {}))
...
//}

例によって@<code>{util/universal-load} が@<code>{cljs.loader/load}を内部で呼び出す Wrapper になってるのですが、@<code>{cljs.loader/load}として読み取ってください。

@<code>{cljs.loader/load}はロードするモジュール名とロード完了後の callback 関数を受け取る関数で、サンプルコードではページ遷移をハンドルする@<code>{(defroute ...)}がこれを呼び出す事で画面に対応したモジュールを LazyLoad しています。

そしてロードが完了した後で、このサンプルでは Reagent @<fn>{fn_reagent}と re-frame @<fn>{fn_re_frame}を利用して画面の描画を行っているため、@<code>{cljs.loader/load}の callback からロードが完了したコンポーネントを Reagent に描画させています。
//footnote[fn_reagent][https://reagent-project.github.io/]
//footnote[fn_re_frame][https://github.com/Day8/re-frame]

これで Code Splitting と LazyLoad に必要な設定は全てです。

続いて、クライアントサイドでのキャッシュやオフライン対応を実現出来る Service Worker について解説していきます。

= Service Workerを利用してキャッシュやオフライン対応を実現する

ここまでの章では、比較的初めてのユーザーが利用する際のパフォーマンスについての解説をしてきました。
しかし、実際にユーザーがサービスを使い始めた後ではアプリケーションの使い心地が重要であり、キャッシュによる表示の高速化とオフライン環境の対応やコードの先読み等が有効になります。

そこでこの章では、 Web 標準技術の Service Worker を利用してキャッシュの設定やオフライン対応をしていきましょう。

== Service Worker と Workbox

Service Worker はブラウザのバックグラウンドで動作する Worker で、あらゆるコンテンツのキャッシュ管理からオフラインの対応、果ては Push 通知の受信なども設定する事ができます。

また、この Service Worker を PWA に沿ったハイパフォーマンスな SPA の開発に使うにあたって便利なライブラリの Workbox @<fn>{fn_workbox} が Google によって公開されており、サンプルアプリでもこの Workbox を通して Service Worker を利用しています。
//footnote[fn_workbox][https://developers.google.com/web/tools/workbox/]

続いて Workbox を実際に導入していく手順について解説していきます。

== Workboxの導入

Workbox を利用するには一つの設定ファイルと、設定ファイルから Service Worker のソースコードを生成する準備が必要になります。

設定ファイルからソースコードを生成する方法として、 Workbox 側からコマンドラインツール・ Node.js アプリケーションから呼び出す前提のモジュール・ Webpack Plugin の三種類の方法が提供されています。
しかし Clojure で開発する場合は webpack を利用する事はまずないと思うのでコマンドラインツール一択で良いでしょう。

Workbox のコマンドラインツールは Node.js のモジュール として提供されているため、 npm を利用して導入します。

//cmd{
npm install --save workbox-cli
//}

これで Workbox を利用する準備は整いました。

== Workboxの設定ファイル

Workbox では、ソースコードを生成する際に予め同一ドメインのファイル一覧を一緒に生成しておく事で Service Worker の読み込み後にバックグラウンドで該当のファイルを先読みする事が出来る Precache と、動的にリクエストされるコンテンツに対するキャッシュの２つのキャッシュを設定する事ができます。

まずは Precache を実現するための設定について見ていきましょう。
以下の設定ファイルを見てください。

//emlist[sample-github-spa/workbox-config.js][javascript]{
module.exports = {
  "globDirectory": "resources/public/prod",
  "globPatterns": [
    "js/compiled/*.js"
  ],
  "modifyUrlPrefix": {
      "js/compiled": "/static/js/compiled",
  },
  "swDest": "resources/public/prod/sw.js",
  "runtimeCaching": [
	...
  ],
};
//}

この設定ファイル内で定義されているプロパティのうち、 @<code>{"globDirectory"}で Precache を行う対象が含まれるディレクトリを指定しており、@<code>{"globPatterns"}で Precache を行うファイルを絞り込んでいます。
また、 @<code>{"modifyUrlPrefix"}によって対象ファイルのパスを実際のリクエストで取得すべきパスに書き換えできます。
設定された内容から、 Workbox は対象となるファイルを全て探索して Service Worker に事前にキャッシュさせるようコードを生成します。

以下が Workbox によって生成された命令の一部です。

//emlist[sample-github-spa/resources/public/prod/sw.js][javascript]{
...
/**
 * The workboxSW.precacheAndRoute() method efficiently caches and responds to
 * requests for URLs in the manifest.
 * See https://goo.gl/S9QRab
 */
self.__precacheManifest = [
  {
    "url": "/static/js/compiled/activity.js",
    "revision": "6d91379322587fa0b689f24781917a3f"
  },
  {
    "url": "/static/js/compiled/app.js",
    "revision": "e516039b14f4e77cecaaaa41f7965605"
  },
  {
    "url": "/static/js/compiled/auth.js",
    "revision": "fb8de68352044824762828199217663d"
  },
  {
    "url": "/static/js/compiled/cljs_base.js",
    "revision": "8e1bb963be635987e76563d2855c0875"
  },
  {
    "url": "/static/js/compiled/repository.js",
    "revision": "5748d0e0c26829f6ba00f7be5e5b416c"
  }
].concat(self.__precacheManifest || []);
workbox.precaching.suppressWarnings();
workbox.precaching.precacheAndRoute(self.__precacheManifest, {});
...
//}

生成されたコードを見ると、先の章で分割したコードたちが全てハッシュ値と共に羅列されている事が見て取れます。
これにより Service Worker は新しいコードがデプロイされてくるまでの間キャッシュを持ち続け、さながらインストールされたアプリケーションファイルかのように高速にコンテンツをロード出来るようになります@<fn>{fn_warn_request_parameter}。
もちろん、端末がオフライン状態であってもこのキャッシュは有効なので、ユーザーがもしオフライン状態でページを開いてもしっかりと表示出来ることでしょう（この時点ではAPIコールなどは失敗しますが）。
//footnote[fn_warn_request_parameter][サンプルアプリでは一切行っていないため問題ないですが、 Precache されたコンテンツを取得する際にクエリパラメータ（@<code>{?v=1}など）が付与されていると Precache されたコンテンツが無視されて必ず再取得されるためご注意ください。 Precache されたコンテンツに関しては@<code>{"revision"}プロパティのハッシュ値比較で最新である事が担保されるのでパラメータなしのリクエストにするのが吉です。]

続いて、動的コンテンツに対するキャッシュ設定について見ていきます。

もう一度 Workbox の設定ファイルを見てみましょう。

//emlist[sample-github-spa/workbox-config.js][javascript]{
module.exports = {
	...
  "runtimeCaching": [
      {
          "urlPattern": /\//,
          "handler": "networkFirst",
          "options": {
              "cacheableResponse": {
                  "statuses": [0, 200],
                  "headers": {
                      "Content-type": "text/html; charset=utf-8",
                  },
              },
          },
      },
      {
          "urlPattern": /^https\:\/\/use\.fontawesome\.com\/releases\//,
          "handler": "cacheFirst",
          "options": {
              "cacheableResponse": {
                  "statuses": [0, 200],
              },
          },
      },
      {
          "urlPattern": /^https\:\/\/api\.github\.com\//,
          "handler": "networkFirst",
          "options": {
              "cacheableResponse": {
                  "statuses": [0, 200],
              },
          },
      },
      {
          "urlPattern": /^https\:\/\/avatars0\.githubusercontent\.com\//,
          "handler": "cacheFirst",
          "options": {
              "cacheableResponse": {
                  "statuses": [0, 200],
              },
          },
      },
  ],
};
//}

設定ファイルのうちの@<code>{"runtimeCacheing"}プロパティが動的コンテンツのキャッシュ設定となっており、@<code>{"urlPattern"}プロパティがキャッシュを適用する対象のURL、@<code>{"handler"}プロパティでキャッシュ戦略の指定、@<code>{"options"}プロパティに書かれている@<code>{"cacheableResponse"}プロパティがキャッシュすべきレスポンスの条件についての設定になっています。

@<code>{"handler"}で指定されるキャッシュ戦略は、対象の URL からのレスポンスに対するキャッシュを利用するタイミングをいくつかのパターンから選び取れるもので、以下のものから一つ選んで決定します。

//table[cache_strategy][キャッシュ戦略一覧]{
handler	内容
------
cacheFirst	1度 Cache されたらアップデートされません。長期間更新されないアセットに最適です
cacheOnly	Cache からのみレスポンスを返します。
networkFirst	ネットワークを優先して返します。オフラインや呼び出し先の都合で通信が失敗した場合は Cache を呼び出します
networkOnly	ネットワークからのみレスポンスを返します。
staleWhileRevalidate	Cache とネットワークの両方から並列に要求され、Cache されたバージョンで応答します。 その時に Cache は、ネットワークから返されるものに置き換えられます
//}

例えばこのサンプルアプリの場合、ファイル名がバージョン毎で必ずユニークになっている fontawesome@<fn>{fn_fontawesome} の css ファイルについては一度キャッシュした以降は必ずキャッシュを利用する@<code>{cacheFirst}を利用しており、逆に GitHub API のレスポンスについてはオフライン時にのみキャッシュを利用する@<code>{networkFirst}を指定しています。
//footnote[fn_fontawesome][サンプルアプリで利用している様々なアイコンが同梱された Web フォントです。 https://fontawesome.com/]

以上を踏まえて Workbox の設定ファイルの準備ができたなら、以下のようにアプリケーションのビルド後に Workbox のコマンドラインツールを呼び出してソースコードを生成すれば Workbox の役目は終わりです。

//emlist[sample-github-spa/package.json][javascript]{
{
...
  "scripts": {
    ...
    "workbox": "workbox generateSW",
    ...
    "build:prod:client": "lein cljsbuild once prod && npm run workbox",
    ...
  },
...
}
//}

最後に生成したソースコードを Service Worker として登録する必要がありますが、そこに関しては次の SSR についての章で解説します。

= SSR(Server Side Rendering)を実現する

最後に、SPAを初めて表示する際のパフォーマンスを更に向上させるために

= 終わりに
