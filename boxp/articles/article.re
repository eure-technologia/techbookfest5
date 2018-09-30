
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

SPA(Single Page Application)に関わらず、Web上にあるコンテンツのパフォーマンスを向上させる事はユーザー体験の向上を考える上でとても重要なことです。

例えば、AndroidアプリやiOSアプリなどのネイティブ版に併せてSP版（スマートフォンブラウザ向けのWeb版）も提供しているWebサービスなどでは、インストールに時間がかかりユーザーが離脱してしまうネイティブ版に変わってSP版がユーザーの玄関口としての役割を果たす事でしょう。

そうした状況ではページの表示速度や応答速度がユーザーの離脱率に直結し、パフォーマンスの改善による大きな効果が期待できます@<fn>{fn_why_performance_matters}。
//footnote[fn_why_performance_matters][パフォーマンス向上による恩恵についてより詳しく知りたければ、 WebFundamentals による"Why Performance Matters" - https://developers.google.com/web/fundamentals/performance/why-performance-matters を一読する事をおすすめします。]

この、ユーザーに長時間のインストール時間を負担させる事もなく、ネイティブアプリに近いユーザー体験をユーザーにもたらすニーズを実現するものとして、GoogleがPWA(Progressive Web App)@<fn>{fn_pwa}と呼ばれるコンセプトを提唱しています。
//footnote[fn_pwa][残念ですが、この本ではスペースや著者の時間的な理由からPWAについての詳しい説明は省いています。詳しく知りたい場合はGoogleの"はじめてのプログレッシブウェブアプリ" - https://developers.google.com/web/fundamentals/codelabs/your-first-pwapp を読むか、既に先人たちが残している大量の記事の中からめぼしいものを探して読む事を勧めます。]

次の章からは、このコンセプトに沿って効果的にパフォーマンスを向上させるための手段を解説していきます。

= Code Splitting + LazyLoadを実現する

SPAを初めて表示する時のロード時間を短縮する手段として、Code Splitting（コードの分割）とLazyLoad（コードの遅延ロード）が有用です。

基本的に、SPAにおけるページ遷移では最小限のリソースのロードとクライアントサイドでの描画だけが行われ、高速にページ遷移をする事ができます。

しかし、どのページからにしろユーザーが初めて表示するページだけは通常のWebページ同様表示に必要なコードをまとめてロードする必要があるため、コードのサイズが大きいと表示が完了するまで多くの待ち時間を作ってしまい、最悪ユーザーがページを開いた瞬間に直帰してしまう可能性もあります。

そこでコード分割とコードの遅延ロードを実装し、app-shell@<fn>{fn_appshell}と呼ばれる最小限表示に必要なコードを切り出して、ページ毎に必要なコードを遅延ロードさせるように実装して行きましょう。
//footnote[fn_appshell][https://developers.google.com/web/fundamentals/architecture/app-shell]

以下に必要な手順を書いていきます。

== Code Splitting

サンプルアプリでは以下の図のようにログイン画面・リポジトリ一覧画面・リポジトリ詳細画面・アクティビティ一覧画面・アクティビティ詳細画面の５つの画面に分かれています。

#@# ここに画面遷移図を置く

この内の一覧画面と詳細画面は頻繁にそれぞれへ遷移する画面なので、一つのモジュールにまとめました。最終的に全画面共通のモジュール(app-shell)とレポジトリ画面、アクティビティ画面、ログイン画面の4つに分割します。

最終的に、project.cljでのモジュール定義は以下の形になります。

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

@<code>{:repository}, @<code>{:auth}, @<code>{:activity}, @<code>{:cljs-base}, @<code>{:client}の5つのモジュールに分割しており、それぞれのモジュールの名前空間や依存関係、出力先ファイル名を定義しています。

本来はこの5つのうちの@<code>{:client}がapp-shellにあたるモジュールですが、コメントにも書かれている通り動的なモジュールロードに必要な@<code>{:cljs-base}が必ず分割されるのでこちらもapp-shellの一部になります。

これでCode Splittingに必要な設定は全てです。
続いて分割したコードを動的にロードするLazyLoadについて解説していきます。

== LazyLoad

ClojureでLazyLoadを実現するためには、ClojureScript（ClojureからJSへのトランスパイラ）にのみ存在する標準APIの@<code>{cljs.loader/load}と@<code>{cljs.loader/set-loaded!}を利用します。

まずは@<code>{cljs.loader/set-loaded!}をLazyLoadしたいコードの末尾で呼び出す必要があるため、以下のように書いています。

//emlist[sample-github-spa/src/cljs/sample_github_spa/repository/container.cljs][clojure]{
(ns sample-github-spa.repository.container
  (:require [reagent.core :as reagent]
            [sample-github-spa.component]
            [sample-github-spa.repository.component :as component]
            [sample-github-spa.repository.subs :as subs]
            [sample-github-spa.repository.events :as events]
            [sample-github-spa.util :as util]
            [re-frame.core :as re-frame]))
...
(util/universal-set-loaded! :repository)
//}

実際には@<code>{util/universal-set-loaded!}を呼び出していますが、これはあくまでも@<code>{cljs.loader/set-loaded!}と同じ動作をするWrapperです（なぜWrapperを利用しているかについては後のSSRについての章で解説します）。

@<code>{cljs.loader/set-loaded!}を呼び出した事で、@<code>{cljs.loader/load}によるコードのLazyLoadが可能になります。
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

例によって@<code>{util/universal-load} が@<code>{cljs.loader/load}を内部で呼び出すWrapperになってるのですが、@<code>{cljs.loader/load}として読み取ってください。

@<code>{cljs.loader/load}はロードするモジュール名とロード完了後のcallback関数を受け取る関数で、サンプルコードではページ遷移をハンドルする@<code>{(defroute ...)}がこれを呼び出す事で画面に対応したモジュールをLazyLoadしています。

そしてロードが完了した後で、このサンプルではReagent@<fn>{fn_reagent}とre-frame@<fn>{fn_re_frame}を利用して画面の描画を行っているため、@<code>{cljs.loader/load}のcallbackからロードが完了したコンポーネントをReagentに描画させています。
//footnote[fn_reagent][https://reagent-project.github.io/]
//footnote[fn_re_frame][https://github.com/Day8/re-frame]

これでCode SplittingとLazyLoadに必要な設定は全てです。

続いて、クライアントサイドでのキャッシュやオフライン対応を実現出来るServiceWorkerについて解説していきます。

= ServiceWorkerを利用してキャッシュやオフライン対応を実現する

= SSRを実現する

= 終わりに
