
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
ViewPagerのような動きをWidgetでも実現したい時に使えそうなViewです。

==== AnalogClock
AnalogClockは名前のとおりアナログ時計を表示するためのViewです。アナログ時計とは、時針・分針・秒針の針の向きで時刻を表示する時計のことです。新しい端末を買ってきた時にホーム画面にアナログ時計のWidgetが配置済なこともあるので、よく見るWidgetに使われているViewです。
しかしこのViewはAPI level 23からDeprecatedになっています。

==== Chronometer
Chromometerはタイマーを表示するためのViewです。カップラーメンを作る3分を計りたい時などなどに便利なViewです。Interfaceもシンプルなので単純なタイマーを作りたい場合はこのViewを使うだけで実現が可能です。一方で複雑なことを実現するためには物足りない感じもするので、その場合はTextViewなどで表示のみ行い、ロジックなどは自前で実装することになります。

==== StackView
StackViewは重なって表示されるカードを表示するためのViewです。Google Play BooksのWidgetでも使われています。このViewを使ったWidgetは使える場面が多数ありそうなので、@<chapref>{create-widget}でも紹介します。

==== ViewFlipper
ViewFlipperはAdaperViewFlipperと同じViewを提供すると考えて差し支えありません。しかしこの2つには明確な違いも存在しています。ViewFlipperを使う場合は子Viewをいくつ用意するのかをViewFlipperを呼び出す前に決めておく必要があります。またViewの切り替えが発生した際にViewのリサイクルが実行されないのがViewFlipperです。AdaperViewFlipperの場合は、普段私達がListViewやGridViewを表示するのと同様にAdapterを通してデータの変更が可能です。またViewの切替時にViewのリサイクルが実行されます。多くの子Viewを表示したい場合はAdapterViewFlipperを使い、そうじゃないシンプルな用途の場合にはViewFlipperを使うようにするとよさそうです。

== Widget開発で登場するクラス
Widget開発をする際に登場するクラスは次のとおりです。

 * AppWidgetProvider
 * AppWidgetManager
 * AppWidgetProviderInfo
 * RemoteViews

==== AppWidgetProvider
BroadcastReceiverを継承しWidget向けに必要なメソッドだけにしたクラスです。Widget開発といったらこのクラスをメインでいじることになります。SystemやUserの操作によって発生するイベントを取得するのが仕事です。イベントを取得した後はこのあと紹介するAppWidgetMangerを通してViewの更新を行います。
AppWidgetProviderを使うことでWidgetのライフサイクルに合わせた更新処理だけに実装者は気を使えばいいようになっています。

==== AppWidgetManager
WidgetのViewを更新します。またインストールされているAppWidgetProviderInfoの値を取得したり、Widget関連の情報の取得も行います。

==== AppWidgetProviderInfo
インストールされているAppWidgetProviderのメタデータを扱うクラスです。Widgetを開発する際は@<list>{infomation widget provider}のように@<strong>{appwidget-provider}タグを使って記述し、@<strong>{res/xml}配下に保存して使用します。
//list[infomation widget provider][AppWidgetProviderInfoの扱うメタデータの例][xml]{
<?xml version="1.0" encoding="utf-8"?>
<appwidget-provider
    xmlns:android="http://schemas.android.com/apk/res/android"
    android:initialKeyguardLayout="@layout/infomation_widget"
    android:initialLayout="@layout/infomation_widget"
    android:minHeight="40dp"
    android:minWidth="40dp"
    android:previewImage="@drawable/example_appwidget_preview"
    android:resizeMode="horizontal|vertical"
    android:updatePeriodMillis="86400000"
    android:widgetCategory="home_screen"
    >
</appwidget-provider>
//}

==== RemoteViews
@<hd>{Widgetで使えるViewの種類}で紹介したViewたちを扱うためのクラスです。Widgetで使っているViewを取得する場合に@<list>{remoteviews}のようにして使用します。さらに取得したRemoteViewInstanceを使用して、@<list>{setTextViewText}のようにTextViewにテキストをセットするようなことも可能です。
//list[remoteviews][WidgetのViewを取得][kotlin]{
val views = RemoteViews(context.packageName, R.layout.infomation_widget)
//}

//list[setTextViewText][WidgetのTextViewにtextをセット][kotlin]{
/**
 * Equivalent to calling {@link TextView#setText(CharSequence)}
 *
 * @param viewId The id of the view whose text should change
 * @param text The new text for the view
 */
public void setTextViewText(int viewId, CharSequence text) {
    setCharSequence(viewId, "setText", text);
}
//}
他にもView周りに対してなにか変更をくわえる場合にはRemoteViewのお世話になります。一度は目を通しておいたほうがいいクラスです。

== Widgetのlifecycle
著者がWidgetのlifecycleをはじめて見た時にActivityやFragmentとの違いに戸惑いを感じたのでここでご紹介しておきます。
AppWidgetProviderクラスの実装をのぞいてみると@<list>{AppWidgetProvider}のようになっています。（ここでは便宜上引数や内部の処理を省略）
AOSPのコードを見ていただくと分かるのですが、onReceive()以外のメソッドについてはすべてonReceive()内で、引数で受け取ったIntentのactionをもとに分岐した先で呼び出しています。
//list[AppWidgetProvider][AppWidgetProvider][java]{
public class AppWidgetProvider extends BroadcastReceiver {
    public AppWidgetProvider() { }

    public void onReceive() { }

    public void onUpdate() { }

    public void onAppWidgetOptionsChanged() { }

    public void onDeleted() { }

    public void onEnabled() { }

    public void onDisabled() { }

    public void onRestored() { }
}
//}

たとえば、onUpdate()は@<list>{onUpdate}のような処理がonRecieve()内で行われた結果呼び出されています。
//list[onUpdate][onUpdate][java]{
if (AppWidgetManager.ACTION_APPWIDGET_UPDATE.equals(action)) {
  Bundle extras = intent.getExtras();
    if (extras != null) {
      int[] appWidgetIds =
          extras.getIntArray(AppWidgetManager.EXTRA_APPWIDGET_IDS);
      if (appWidgetIds != null && appWidgetIds.length > 0) {
        this.onUpdate(
            context, AppWidgetManager.getInstance(context),
            appWidgetIds);
      }
  }
}
//}

==== onReceive()
BroadcastReceiverのabstructメソッドであるonReciveの実装です。BroadCastに反応して呼び出されます。

==== onUpdate()
onReceiveが受け取ったIntentのactionが@<strong>{AppWidgetManager.ACTION_APPWIDGET_UPDATE}のときに呼び出されます。updatePeriodMillisの更新が走った時、Systemの起動時に発行されるactionです。また明示的にonUpdateを走らせたい場合に使うことでWidgetの更新時間を短くしたり、任意のタイミングで更新させたりすることができます。

==== onAppWidgetOptionsChanged()
onReceiveが受け取ったIntentのactionが@<strong>{AppWidgetManager.ACTION_APPWIDGET_OPTIONS_CHANGED}のときに呼び出されます。Widgetのサイズが変更された時に発行されるactionです。サイズによってWidget内の特定の要素の表示非表示を変更したいような場合に使うことができます。

==== onDeleted()
onReceiveが受け取ったIntentのactionが@<strong>{AppWidgetManager.ACTION_APPWIDGET_DELETED}のときに呼び出されます。1つかそれ以上のWidgetが削除された時に発行されるactionです。Widgetの更新処理を自前で実装し、AlermManagerなどを使っている場合はここで解除することなどができます。

==== onEnabled()
onReceiveが受け取ったIntentのactionが@<strong>{AppWidgetManager.ACTION_APPWIDGET_ENABLED}のときに呼び出されます。はじめてWidgetが追加された時に発行されるactionです。

==== onDisabled()
onReceiveが受け取ったIntentのactionが@<strong>{AppWidgetManager.ACTION_APPWIDGET_DISABLED}のときに呼び出されます。追加されているWidgetの最後のWidgetが削除された時に発行されるactionです。

==== onRestored()
onReceiveが受け取ったIntentのactionが@<strong>{AppWidgetManager.ACTION_APPWIDGET_RESTORED}のときに呼び出されます。API level 21から追加されました。@<list>{onRestored}のようにonRestoredが呼ばれた直後にonUpdateが呼ばれます。アプリのアップデートなどでWidgetのレイアウトを変更したあとなどに使用できます。
//list[onRestored][onRestored][java]{
Bundle extras = intent.getExtras();
  if (extras != null) {
    int[] oldIds = extras.getIntArray(
        AppWidgetManager.EXTRA_APPWIDGET_OLD_IDS);
    int[] newIds = extras.getIntArray(
        AppWidgetManager.EXTRA_APPWIDGET_IDS);
    if (oldIds != null && oldIds.length > 0) {
      this.onRestored(context, oldIds, newIds);
      this.onUpdate(context, AppWidgetManager.getInstance(context), newIds);
    }
  }
//}