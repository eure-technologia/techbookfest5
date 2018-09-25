
= ARCoreをAndroidStudioではじめよう！
== はじめに
こんにちは！はじめまして！
普段はAndroidアプリの開発をメインでやっている、@<href>{https://twitter.com/zukkey59, @zukkey}です。
今回は、ARに興味を持ちましてAndroidでARCoreを導入して開発してみたいという初心者の方向けに導入手順の紹介とどういったことができるのかなどを解説していこうと思います。

* この本に書いてあること
** 初心者でARを始めてみたい！という方で導入とかよくわかんない人に導入から紹介します
** UnityじゃなくてAndroidStudioでやる方法を書いています


== 目次
この本で紹介することの一覧は次の通りです。

* ARCoreとは

* AndroidStudioで開発するための準備

* ARCoreでできること

** Sceneform

*** Widgetの表示

*** 図形の表示

*** 3Dモデルの表示

** Argumented Images

*** The arcoreimg toolを使って画像マーカーにできるかを知る

*** 画像をマーカーにして3Dモデルを表示してみよう

** Cloud Anchors

*** GCPのセットアップ

*** サンプルを動かしてみる

== ARCoreとは
ARCoreとは、Googleが提供しているSDKです。AR機能に必須のモーショントラッキングや環境、
光の加減など全てサポートしてくれておりSDKを利用することで既存のアプリケーションいAR機能を搭載したり、新しいARアプリケーションを構築することができます。
公式の概要は下記リンクを参照してください。
> https://developers.google.com/ar/develop/

== AndroidStudioで開発するための準備
AndroidStudioで開発を始めるにはいくつか準備が必要です。

=== 開発環境をセットアップする
まずは、開発環境を整えていきましょう。
AndroidStudioはversion3.1以上のものをインストールしてください。
AndroidStudio > About AndroidStudioから次の画像のようにバージョンの確認ができます。

// techbookfest5-1の画像を載せる

エミュレータを利用する場合には、Android Emulator27.2.9以上が必要です。
また、エミュレータでOpenGL ES 3.0以上をサポートし、有効にする必要があります。

ARCoreを利用するために下記の３点を設定する必要があります。
1. AndroidManifestを設定する
2. ビルドの依存関係を追加する
3. カメラのパーミッションを許可する

=== AndroidManifestを設定する
AR機能を利用するために、まずはじめにAndroidManifestの設定を行う必要があります。
AR機能を必須にする場合と、必須にしない場合によって設定する項目が違います。

* AR機能を必須にする場合
AR機能必須アプリの場合には、minSdkVersionが24以上になります。AndroidManifestには、次のように追加してください。
android:valueをrequiredにすることで、必須とすることができます。

//emlist[][]{
<uses-sdk android:minSdkVersion="24" />

<uses-permission android:name="android.permission.CAMERA" />
<uses-feature android:name="android.hardware.camera" />

<application>
    …
    <meta-data android:name="com.google.ar.core" android:value="required" />
    …
</application>
//}

* AR機能を必須にしない場合
AR機能を必須にしないで、機能の１つとして追加したい場合では、minSdkVersionが14以上である必要があります。
またandroid:valueをrequiredにすることで、オプショナルとすることができます。
//emlist[][]{
  <uses-sdk android:minSdkVersion="14" />

  <uses-permission android:name="android.permission.CAMERA" />

  <application>
      …
      <meta-data android:name="com.google.ar.core" android:value="optional" />
      …
  </application>
//}


=== ビルドの依存関係を追加する






== ARCoreでできること
ARCoreでできることは、次の３つになります。
1. Sceneform
Sceneformでは、Androidで標準で用意されているWidgetをARとして表示することができたり、図形の表示や3Dモデルを表示することが可能です。
また、表示したオブジェクトを拡大、縮小したり、移動したりクリックイベントによって他の動作をさせることもできます。

2. Argumented Images
Argumented Imagesでは、自分で用意した画像をARを表示するためのマーカーにして用意した3Dモデルを表示することが可能です。
どんな画像でも表示することができるわけではないですが、The arcoreimg toolを使って使用できる画像かを調べて条件を満たせばその画像をマーカにすることができます。

3. Cloud Anchors
Cloud Anchorsでは、特定の場所で出現させたARを２つの端末で操作することが可能です。
GCPを経由してアンカーを作って、出現させたオブジェクトを操作することができます。

次の説から、具体的にどういったことができるのかなどを解説して行きます。

== Sceneform
Sceneformを用いることで、3Dモデルを表示したり、Androidの標準のWidget(TextView, ImageView, etc..)などを表示することが可能です。
3Dモデルを用意するのが難しい場合は、Google公式で次のサイトから3Dモデルを利用することが可能です。
// TODO: poly01画像を出す
@<href>{https://poly.google.com/, Poly}
Polyは、GoogleがARまたはVR開発者向けに用意してくれている無料の3Dオブジェクトライブラリです。
Polyでは、3Dモデルの作成は難しいがARを表示するのに手軽に試したい技術者向けにobjファイルやfbxファイルなど、ARやVRに用いることができるサンプルをダウンロードすることができます。
サンプルには著作権がある場合があるので、目的によって表記を忘れずにつけるようにしてください。

=== Android標準のWidgetの表示
Androidで標準で用意されている、Widgetの表示をARとして表示させることができます。
また、Widgetとしての機能を使用することができます。例えば、ARとして表示したButtonをタップした際にクリック後の処理を行うことができ標準のWidgetと同様の機能を使用することが可能です。

まずはじめに、Widgetの表示を行うところから始めていきましょう。
AndroidのWidgetを表示する前に3Dモデルや図形の表示を行う際にも共通してGoogleのArFragmentを用いる必要があります。



=== 図形の表示


=== 3Dモデルの表示


== Argumented Images
Argumented Imagesでは、特定の画像をマーカーにして3Dモデルを表示することができます。
全ての画像で利用できるわけではないですが、arcoreimgと言われる公式のツールで特徴点のスコアを基準以上で満たせば利用できます。

=== The arcoreimg toolを使って画像マーカーにできるかを知る
Argumented Imagesでは画像をマーカーにすることができるのですが、全ての画像をマーカーにできるわけではありません。
画像をマーカーにできるかを知るために、The arcoreimg toolというツールがGoogle公式で用意されているのでこちらを利用しましょう。

@<href>{https://developers.google.com/ar/develop/java/augmented-images/arcoreimg, The arcoreimg tool}とは、AndroidおよびAndroid NDK開発者向けのコマンドラインツールです。
画像マーカーとしての画質をチェックしたり、Argumented Imagesで利用するための画像データベースの作成を行うことができます。

==== The arcoreimg toolの導入
The arcoreimg toolを導入するために次のリンクのLatest releaseのarcore-android-sdkの最新版のzipをダウンロードしてください。

@<href>{https://github.com/google-ar/arcore-android-sdk/releases, https://github.com/google-ar/arcore-android-sdk/releases}

ダウンロードした圧縮フォルダを解凍後に、tools > arcoreimgと進むとlinux、macos、windowsのOSごとに実行ファイルが用意されているのでそちらをダブルクリックしてください。
クリック後に、次の画像のようにプロセスが完了しました、という表記が出れば導入完了です。

// TODO: arcoreimg01画像を出す
