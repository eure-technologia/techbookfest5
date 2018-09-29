
= Widgetとは
Widgetと聞いて最初に想像するのはどんなものでしょうか。パソコンのデスクトップに設置するアプリケーションのことや、BlogなどのWebサイトのサイドバーに表示されるアクセスカウンターを想像するでしょうか。
普段Androidを使っていたり開発している方であれば、Android端末のホーム画面に設置するWidgetのことを一番最初に想像するのではないでしょうか。
たとえば@<img>{widgets}では時計やGooglePlayMusicや天気予報アプリのWidgetをホーム画面に設置した例です。
//image[widgets][Widgetの例][scale=0.3]{
//}

アプリ一覧からアプリを起動することなく、そのアプリの機能を使ったりアプリが保有する情報を表示したりすることができるのがWidgetです。この章ではそんなWidgetにはどんな種類があり開発する場合に必要となってくるベースとなる知識についてご紹介していきます。

== Widgetの種類
Android Developrの公式ドキュメント（@<href>{https://developer.android.com/guide/topics/appwidgets/overview}）によるとWidgetの種類は大きく4つに分けることができます。

==== Information Widgets
1つめはInformation Widgetsです。
ユーザーにとって重要な特定の情報を時間経過と共に変化させながら表示させることに向いています。
たとえば天気Widgetや時計Widgetなどが良い例です。多くのInfomation Widgetsはタップすることで表示されている情報のより詳細な情報が見られる画面が起動されます。
#@# TODO: Infomation Widgetsの例となる端末のスクショを添付する

==== Collection Widgets
2つめはCollection Widgetsです。
Collectionという名前が示すようにListViewやGridViewを使ったWidgetでニュースアプリやTODO管理アプリのWidgetなどによく使われています。アプリ内での表示と同様に、Widget上でもスクロールさせることができるのも特徴です。またアプリ内のリスト画面と詳細画面の関係のように、タップすることで詳細画面が起動されます。
#@# TODO: Collection Widgetsの例となる端末のスクショを添付する

==== Control Widgets
3つめはControl Widgetsです。
アプリを開くことなくスイッチのON/OFFなどを行うことが目的のWidgetです。最近のOSではNotificationエリアに端末の設定を変更できるトグルボタンのショートカットが配置されるようになったので見なくなりましたが、ホーム画面にWifiのON/OFFやBluetoothのON/OFFのWidgetをおいている方も多かったのではないでしょうか。
#@# TODO: Control Widgetsの例となる端末のスクショを添付する

==== Hybrid Widgets
4つめはHybrid Widgetsです。
これまで紹介した3つのWidgetsの複合Widgetsのことを言います。たとえば音楽プレイヤーアプリのWidgetなどは、Widgetを通した再生・停止ができるControl Widgetsの機能と、現在再生している曲名を表示するInformation Widgetsの機能をあわせ持っているHybrid Widgetsといえます。
#@# TODO: Hybrid Widgetsの例となる端末のスクショを添付する

== Widgetで使えるViewの種類
ホーム画面に設置することでさまざまなショートカットとして使える便利なWidgetですが、実装する上ではいくつかの制約が存在します。
その中の1つが使えるViewの制約です。Widgetで使えるViewはRemoteVeiwsを使った操作が可能なViewのみです。

Widgetに使えるViewGroupは次の4つです。

 * FrameLayout
 * GridLayout
 * LinearLayout
 * RelativeLayout

残念ながらConstraintLayoutは使うことはできません。ホーム画面に設置するWidgetは1画面に複数のアプリのWidgetを設置することも珍しくなく、Widget自体に複雑なレイアウトを求めることも少ないためこの4つがあれば問題ないともいえます。

Widgetに使えるViewは次の12個です。

 * AdapterViewFlipper
 * AnalogClock
 * Button
 * Chronometer
 * GridView
 * ImageButton
 * ImageView
 * ListView
 * ProgressBar
 * StackView
 * TextView
 * ViewFlipper

著者が個人的に使ったことがないViewについてもう少し詳しく紹介したいと思います。

==== AdapterViewFlipper
AdapterViewFlipperに追加された2つまたはそれ以上のView間をアニメーションさせることができるViewです。1度に1つのViewしか表示されません。設定することで定期的にアニメーションを実行して表示されるViewを切り替えることが可能です。

==== AnalogClock


==== Chronometer

==== StackView

==== ViewFlipper

== Widget開発で登場するクラス

== Widgetのlifecycle

====
