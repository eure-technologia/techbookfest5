= まえがき

「Clojureで作るハイパフォーマンスSPA」を手にとって頂き、ありがとうございます。
本書は、著者が業務中に実践しているパフォーマンスに配慮したSPAをClojureで実現するまでの手法をまとめたものです。
応用的な内容のためある程度Clojureについての前提知識が必要かと思いますが、本書がClojureでWebフロントエンド開発をする際に少しでも役に立てれば幸いです。

//image[sample-spa-top][作成するサンプルアプリ][scale=0.5]{
//}

本書では、上の画像のようなGitHubクライアントアプリケーションの完成までの経緯を振り返りながら解説していきます。
完成後のものが https://sample-github-spa.now.sh にデプロイされているので、GitHubのアカウントがあれば是非一度お試しください。

また、本書の中で登場するサンプルコードは全てGitHub上のレポジトリsample-github-spa(https://github.com/boxp/sample-github-spa)に用意しております。
途中コードを用いて説明する際は、このレポジトリ内のソースコードのパスを明記しているので予め clone しておいて適宜参照いただければ幸いです。

最後に、本書を執筆するにあたりアドバイスをくれた同僚達と編集作業をしてくれたfutaboooさん、並びに表紙絵を書いてくださったkarinさんにこの場を借りて深く感謝します。今執筆している文がこうして製本されて頒布されているのは間違いなく皆さんのお陰です。 重ね重ね感謝致します。

それでは、次の章から本編へ移ります。

== 本書の内容について
タイトルにあるとおり本書はAndroidのWidget開発について書いた本です。
#@# TODO: 章の箇条書きと章ごとの簡単な説明を記載する

== エウレ・テクノロギアとは
//footnote[株式会社エウレカ][株式会社エウレカ https://eure.jp/]
エウ・レテクノロギアは株式会社エウレカ@<fn>{株式会社エウレカ}の有志によって結成された技術サークルです。

== お問い合わせ先
本書に関するお問い合わせ先：@<br>{}https://github.com/eureka-twelve/techbookfest5/issues

== 免責事項
本書に記載された内容は、情報の提供のみを目的としています。したがって、本書を用いた開発、制作、運用は必ずご自身の責任と判断によって行ってください。これらの情報による開発、制作、運用の結果について、著者はいかなる責任も負いません。