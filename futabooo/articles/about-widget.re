
= Widgetとは
Widgetと聞いて最初に想像するのはどんなものでしょうか。パソコンのデスクトップに設置するアプリケーションのことや、BlogなどのWebサイトのサイドバーに表示されるアクセスカウンターを想像するでしょうか。
普段Androidを使っていたり開発している方であれば、Android端末のホーム画面に設置するWidgetのことを一番最初に想像するのではないでしょうか。
たとえば@<img>{widgets}では時計やGooglePlayMusicや天気予報アプリのWidgetをホーム画面に設置した例です。
//image[widgets][Widgetの例][scale=0.3]{
//}

アプリ一覧からアプリを起動することなく、そのアプリの機能を使ったりアプリが保有する情報を表示したりすることができるのがWidgetです。この章ではそんなWidgetにはどんな種類があり開発する場合に必要となってくるベースとなる知識についてご紹介していきます。

== Widgetの種類
Android Developrの公式ドキュメント@<href>{https://developer.android.com/guide/topics/appwidgets/overview}によるとWidgetの種類は大きく4つに分けることができます。

=== Information Widgets
ひとつめはInformation widgetsです。
ユーザーにとって重要な特定の情報を時間経過と共に変化させながら表示させることに向いています。
たとえば天気Widgetや時計Widgetなどが良い例です。
#@# TODO: Infomation Widgetsの例となる端末のスクショを添付する

=== Collection Widgets

=== Control Widgets

=== Hybrid Widgets

== Widgetで使えるViewの種類

=== ViewGroup
Widgetに使えるViewGroupは次の4つです。

 * FrameLayout
 * LinearLayout
 * RelativeLayout
 * GridLayout

=== View

== Widget開発で登場するクラス

== Widgetのlifecycle