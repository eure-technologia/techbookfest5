= Widgetを作る


== Sample WidgetをAndroidStudioで簡単に作る
Widgetを作るだけであればAndroidStudioの機能を使って簡単に作ることができます。
AndroidStudioのメニューから@<strong>{File＞New>New Project...}で新規プロジェクトを作成します。
//image[create-widget-1][create new module 1][scale=0.5]{
//}

Widgetだけをつくる場合はActivityは必要ないため、@<strong>{Add No Activity}を選択します。
//image[create-widget-2][create new module 2][scale=0.5]{
//}

新規プロジェクトが作成されたらルートフォルダを右クリックして@<strong>{New>Widget>App Widget}を選択します。
//image[create-widget-3][create new widget][scale=0.5]{
//}

新しいWidget Componentの追加画面が表示されるので、順番に設定をしていきます。まずは名前をつけます。
//image[create-widget-4][new android component][scale=0.5]{
//}

次にどこに表示するためのWidgetを作成するのかを@<strong>{placement}のプルダウンから選択します。
//image[create-widget-5][widget placement][scale=0.7]{
//}
選択肢はつぎの3つがあります。

 * Home-screen and Keyguard
 * Home-screen only
 * Keyguard only（API 17 +）

Home-screenは普段使用しているホーム画面への設置です。Keyguardはロックスクリーン画面へ設置したい場合に選択することができます。しかしロックスクリーン画面へのWidgetの設置が行えるのはAPI level 19までで、Android5.0以上のAPI level 21以上ではできません。最近のAndroidOS事情からすると実質ここで選択するのは@<strong>{Home-screen only}になります。

次にWidgetのResizeを可能にするかどうかを選択します。
//image[create-widget-6][widget resizable][scale=0.7]{
//}
選択肢はつぎの3つがあります。

 * Horizontally and vertically
 * Only horizontally
 * Only vertically
 * Not resizable

1つ目の@<strong>{Horizontally and vertically}を選択したWidgetが@<img>{widget-resize}です。縦横のどちらにもサイズを拡張できるようになっていることが分かります。
//image[widget-resize][widget resize][scale=0.3]{
//}

Resize可能にするかどうかを決めた後はWidgetのミニマムサイズを決めます。@<img>{create-widget-7}と@<img>{create-widget-8}のようにプルダウンで1~4を選択することができます。この1~4の数字はホーム画面に設置する際に使用するセルの数を表しています。@<fn>{AppWidgetDesignGuidelines}
//footnote[AppWidgetDesignGuidelines][https://developer.android.com/guide/practices/ui_guidelines/widget_design]

//image[create-widget-7][widget minimum width][scale=0.7]{
//}
//image[create-widget-8][widget minimum hight][scale=0.7]{
//}

Source Languageではjavaかkotlinを選択することができます。既存のプロジェクトへ追加する場合は状況によるかもしれませんが、新規でWidgetを作るようなプロジェクトの場合はKotlinを選んでおけば間違いないでしょう。
//image[create-widget-9][widget source language][scale=0.7]{
//}

Target Source Setの選択ではどのbuildTypeやproductFlavorのフォルダ配下にWidget関連のクラスを作成するか選びます。特に特別な理由がなければ@<strong>{main}を選択でよいはずです。
//image[create-widget-10][widget target source set][scale=0.7]{
//}
ここまでの設定をすべて完了すると、AndroidStudioがWidgetに必要なクラス郡を自動作成してくれます。

作成したWidgetを実際に端末上で動かして確認するにはもうひと手間が必要です。
Run/Debug Configurationsを開き、@<strong>{Launch Options}のLaunchのプルダウンから@<strong>{Nothing}を選択します。今回の手順で作成したばかりのWidgetプロジェクトではまだActivityが存在していないためこの処理が必要となります。
//image[create-widget-11][run configrations][scale=0.5]{
//}


== StackViewを使ったWidgetを作る
StackViewをつかったWidgetのサンプルの実装が公式でも公開されています。今回は公開されているサンプルのコードをもとに紹介していきたいと思います。StackViewのようにlist形式のデータを扱う場合のWidgetでは少し複雑な処理が必要となります。そこで登場するのが次の2つのクラスです。

* RemoteViewsService
* RemoteViewsService.RemoteViewsFactory

普段のAndroidアプリ開発においてリストのデータを扱う場合にはArrayAdapterやRecyclerView.Adapterクラスを使って

==== StackWidgetService

==== StackRemoteViewsFactory

==== StackWidgetのlayout.xml
//list[stackview][stackview][xml]{
<?xml version="1.0" encoding="utf-8"?>
<StackView xmlns:android="http://schemas.android.com/apk/res/android"
  android:id="@+id/stack_view"
  android:layout_width="match_parent"
  android:layout_height="match_parent"
  android:loopViews="true" />
//}

//list[emptyview][emptyview][xml]{
<?xml version="1.0" encoding="utf-8"?>
<FrameLayout xmlns:android="http://schemas.android.com/apk/res/android"
  android:layout_width="match_parent"
  android:layout_height="match_parent">
  <StackView xmlns:android="http://schemas.android.com/apk/res/android"
    android:id="@+id/stack_view"
    android:layout_width="match_parent"
    android:layout_height="match_parent"
    android:gravity="center"
    android:loopViews="true" />
  <TextView xmlns:android="http://schemas.android.com/apk/res/android"
    android:id="@+id/empty_view"
    android:layout_width="match_parent"
    android:layout_height="match_parent"
    android:gravity="center"
    android:background="@drawable/widget_item_background"
    android:textColor="#ffffff"
    android:textStyle="bold"
    android:text="@string/empty_view_text"
    android:textSize="20sp" />
</FrameLayout>
//}


== Githab APIを使ったWidgetを作る

== 既存のアプリにWidgetを追加する

=== アプリ内からWidgetを追加できるようにする