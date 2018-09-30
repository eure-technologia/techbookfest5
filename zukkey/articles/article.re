
= ARCoreをAndroidStudioではじめよう！
== はじめに
こんにちは！はじめまして！
普段はAndroidアプリの開発をメインでやっている、@<href>{https://twitter.com/zukkey59, @zukkey}です。
今回は、ARに興味を持ちましてAndroidでARCoreを導入して開発してみたいという初心者の方向けに導入手順の紹介とどういったことができるのかなどを解説していこうと思います。

 * この本に書いてあること
 ** 初心者でARを始めてみたい！という方で、導入とかよくわかんない人に導入から紹介します
 ** UnityじゃなくてAndroidStudioでやる方法を書いています


== 目次
この本で紹介することの一覧は次の通りです。

 * ARCoreとは

 * AndroidStudioで開発するための準備

 * ARCoreでできること

 * Sceneform
 ** Widgetの表示
 ** 図形の表示
 ** 3Dモデルの表示

 * Argumented Images
 ** The arcoreimg toolを使って画像マーカーにできるかを知る
 ** 画像をマーカーにして3Dモデルを表示してみよう

== ARCoreとは
ARCoreとは、Googleが提供しているSDKです。AR機能に必須のモーショントラッキングや環境、
光の加減など全てサポートしてくれておりSDKを利用することで既存のアプリケーションいAR機能を搭載したり、新しいARアプリケーションを構築することができます。
公式の概要は下記リンクを参照してください。

@<href>{公式リンク: https://developers.google.com/ar/develop/, https://developers.google.com/ar/develop/}

== AndroidStudioで開発するための準備
AndroidStudioで開発を始めるにはいくつか準備が必要です。
すでに準備が整っている場合や公式のドキュメントを読んだことがある方は、スキップしていただいて結構です。
この説では、公式のドキュメントの意訳が中心になります。

=== 開発環境をセットアップする
まずは、開発環境を整えていきましょう。
AndroidStudioはversion3.1以上のものをインストールしてください。
AndroidStudio > About AndroidStudioから次の画像のようにバージョンの確認ができます。

//image[techbookfest5-1][AndroidStudioのバージョン確認画面]{
    techbookfest5-1
//}

エミュレータを利用する場合には、Android Emulator27.2.9以上が必要です。
また、エミュレータでOpenGL ES 3.0以上をサポートし、有効にする必要があります。

ARCoreを利用するために下記の3点を設定する必要があります。
1. AndroidManifestを設定する
2. ビルドの依存関係を追加する
3. ランタイムチェックを実行して、カメラのパーミッションを許可する

==== AndroidManifestを設定する
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


==== ビルドの依存関係を追加する
まずはじめに、プロジェクトのbuild.gradleの中が次のようになっているのを確認してください。
//emlist[][]{
  allprojects {
    repositories {
        ...
        google()
        ...
    }
}
//}
...は省略を意味しています。google()がrepositoriesの中にあれば大丈夫です。

次に、appのbuild.gradleの中にてARCoreのライブラリの最新バージョンを追加してください。
2018年10月現在の最新バージョンは1.4.0になります。
最新バージョンの確認は次のリンク先のgithubのreleasesから確認できます。
@<href>{https://github.com/google-ar/arcore-android-sdk/releases, https://github.com/google-ar/arcore-android-sdk/releases}

==== ランタイムチェックを実行して、カメラのパーミッションを許可する
AR機能をオプションとして設定した場合には、ランタイムチェックを実行する必要があります。AR機能を必須とした場合は必要ありません。
AR機能を利用する前に、AR機能を利用できる端末をチェックする必要があります。
次のようにしてランタイムチェックを行います。
//emlist[][]{
  ArCoreApk.Availability availability = ArCoreApk.getInstance().checkAvailability(this);
  if (availability.isSupported()) {
    // 許可されている場合の処理をここに記載する
  } else {
    // 許可されていない場合の処理をここに記載する
  }
//}
checkAvailabilityを呼ぶことでAR機能を利用できる端末かチェックでき、サポートされているかどうかをisSupportedで確認することができます。
サポートされている場合とそうではない場合に分けて、表示非表示などの書きたい処理を書いてください。

次に、ARCoreではカメラを利用するので、カメラのパーミッションを許可する必要があります。
前節のAndroidManifestを設定するにも記載してありますが、AndroidManifest内に次のようにカメラのパーミッションの記載があることを確認してください。
//emlist[][]{
  <manifest…/>
  …
  <uses-permission android:name="android.permission.CAMERA" />
  …
  </manifest>
//}
manifestタグの中に記載されていれば大丈夫です。

また、onResumeにARセッションを始める前に、カメラの許可が付与されているかを確認する処理を次のように記載しましょう。
//emlist[][]{
  @Override
  protected void onResume() {
    super.onResume();

    ...
    if (!CameraPermissionHelper.hasCameraPermission(this)) {
      CameraPermissionHelper.requestCameraPermission(this);
      return;
    }

    …
  }
//}

これで、カメラのパーミッションの許可については終わりです。
公式のドキュメントにも書かれていますが、ARCoreがインストールされているかをチェックする必要が場合によって出てきますがSceneformを利用している今回のような場合では、
ArFragmentが自動的に行ってくれているために言及しません。


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
今回は諸事情でこちらについては記載しておりません。

次の説から、具体的にどういったことができるのかなどを解説して行きます。

== Sceneform
Sceneformを用いることで、3Dモデルを表示したり、Androidの標準のWidget(TextView, ImageView, etc..)などを表示したり、図形の表示することが可能です。
この章では、それぞれについて導入から説明して行きます。

=== 3Dモデルの表示

3Dモデルを導入する前に、プラグインをAndroidStudioに入れる必要があります。
AndroidStudioのPreferencesのPlugins > Browse repositoriesにてGoogle Sceneform Toolsと検索してください。
検索した結果が次のようになると思います。

//image[plugin01][Google Sceneform Tools検索時]{
    plugin01
//}

installしていない場合は、installと表記が出るのでinstallをクリックしてpluginをインストールしましょう。
Google Sceneform Toolsは、任意の3DモデルのobjファイルやfbxファイルなどをAndroidStudioに簡単に入れることができたり、実際に動かす前にViewerで表示の確認をすることができたり、
build.gradleの依存関係を自動的に追加してくれるプラグインになります。

3Dモデルを実際に導入して行くところをやっていきましょう。

3Dモデルを用意するのが難しい場合は、Google公式で次のサイトから3Dモデルを利用することが可能です。
//image[poly01][Poly - 無料の3Dオブジェクトライブラリ]{
    poly01
//}


@<href>{https://poly.google.com/, Poly}
Polyは、GoogleがARまたはVR開発者向けに用意してくれている無料の3Dオブジェクトライブラリです。
Polyでは、3Dモデルの作成は難しいがARを表示するのに手軽に試したい技術者向けにobjファイルやfbxファイルなど、ARやVRに用いることができるサンプルをダウンロードすることができます。
サンプルには著作権がある場合があるので、目的によって表記を忘れずにつけるようにしてください。

また、他にも無料で公開され二次創作利用できる3Dモデルキャラクターがあるので、そちらでも可能です。
私は、今回説明するにあたってImagineVR Inc.さんよりImagin Girlsのアセットをお借りしてきました。

//image[ImagineGirls][ImagineGirls公式サイト]{
    ImagineGirls
//}

ImagineVR Inc.さんのサイトは下記リンクから行けます。
@<href>{http://imaginegirls.com/, ImagineVR Inc.}
また、Imagine Girlsのアセットは二次創作物の製作と二次創作物の無償および有償での頒布をすることはできますが、ライセンスの表記とリンクの表記が必須ですので創作の際に利用する際にはライセンスの表記を忘れないようにしてください。
Imagine Girlsのライセンスは下記リンクから行けます。
@<href>{http://imaginegirls.com/imaginegirls_user_license_jp/, http://imaginegirls.com/imaginegirls_user_license_jp/}


ImagineVR Inc.さんの公式サイトからダウンロードしてきたobjファイルを、実際にプロジェクトに追加して表示するところまでやっていきましょう。
まず、@<href>{http://imaginegirls.com/downloads/, ImagineVR Inc. Downloads}から好みのキャラクターのobjファイルをダウンロードしてきましょう。
圧縮ファイルを開くと、.blendファイルがあると思います。
blendファイルはblenderで開けるファイルですので、blenderが入っていない方は@<href>{https://www.blender.org/, https://www.blender.org/}よりBlenderをダウンロードしてきましょう。

Blenderが入っていれば、フォルダの中にある.blendファイルをダブルクリックすることで開けます。
//image[ImagineGirlsFile][ImagineGirlsフォルダ]{
    ImagineGirlsFile
//}

Blenderを開いたら、File > Export > Wavefront(.obj)を選択して、書き出したい場所にてExport OBJを選択することでobjファイルとmtlファイルを書き出すことができます。
//image[ExportOBJ][Export後のフォルダの中身]{
    ExportOBJ
//}

２つのファイルは、それぞれ異なる役割を持っています。
objファイルはモデルの形状を表現するデータで、mtlファイルはモデルのマテリアルを表現するデータになります。
objが形状を決め、mtlがどのテクスチャを選ぶかを担います。
テクスチャというのは、3Dモデルの表面に貼り付けられるキャラクターの肌の色であったり、服の色であったりする部分になります。
objファイル、mtlファイル、テクスチャが揃って初めて3Dモデルを導入することが可能です。

次に、AndroidStudioでappフォルダを右クリックして New > Sample Data Directoryを選択してください。
そうすると、sampledataというディレクトリが追加されます。
追加されたのを確認したら、先ほどダウンロードしてきたImagineGirlsフォルダの中にあるtexuturesフォルダの中から、3Dモデルの肌や服の色となるテクスチャのpngと、
objファイル及びmtlファイルをSample Data Directoryの中に移動させます。MacならFinderから直接移動させて大丈夫です。

sampledata directoryの中身が次のとおりになっていることを確認してください。
//image[SampleData][ファイル移動後のSampleDataフォルダ]{
    SampleData
//}
objファイル、mtlファイル、テクスチャの画像などが全て入っていることを確認してください。

その次にsampledataを右クリックして、New > Sceneform Assetを選択します。
選択した後、次の画像のようにImport Wizardが出てきます。
//image[Sceneform01][選択前のImport Wizard]{
    Sceneform01
//}

Import Wizardには、４つの項目がありますが、ここではSource Asset Pathと.sfb Output Pathを設定します。
Source Asset Pathでは、先ほどダウンロードしてきたobjファイルを選択し、.sfb Output Pathではassetsもしくはres配下にrawフォルダを追加してrawのPathを設定します。
res > rawを.sfb Output Pathに指定している場合は_がPathに入っていると、AndroidStudioに怒られるので_を排除した名前に.sfb Output Pathを変更してください。

今回は、res配下にてrawフォルダを追加してPathを設定していきます。
ここまで設定をすると、次の画像のようになります。

//image[Sceneform02][選択後のImport Wizard]{
    Sceneform02
//}

この状態でFinishを選択すると、次のようにエラーが吐かれることがあります。
ERROR: Multiple different usages defined for texture: FX
これは、mtlファイル内でFXというテクスチャが複数定義されているために起こるバグになります。
//image[mtlFileError][mtlファイルのError画面]{
    mtlFileError
//}

map_dとmap_Kdが同じテクスチャを指定しているのが原因なので、同名のテクスチャの画像をコピーして名前を変えてPath名をどちらか片方変更する必要があります。
また、テクスチャの画像をSample Data Directoryに変更しているため、mtlファイルにてmap_dとmap_KdのPathがデフォルトのままになっていますので変更する必要があります。
map_d hogehoge/FX.pngとなっているのを、map_d ./FX.pngと修正してください。

導入がうまくいくと、次のような画面に自動的に遷移します。
//image[importComplete][3Dモデル導入後の画面]{
    importComplete
//}


appのbuild.gradleに次のコードが追加され、res > raw配下に.sfbファイルが自動的に追加されます。
//emlist[][]{
  sceneform.asset('sampledata/R1N4_V2_FREE.obj',
          'default',
          'sampledata/R1N4_V2_FREE.sfa',
          'src/main/res/raw/rin')
//}

このコードを追加するのではなく、あくまでプラグイン経由で入れた場合に自動的に追加されます。

もし、何らかの事情で入れ直したい場合、すでに同じ名前のものが作られているいて、エラーが吐かれますのでその場合は名前を変更するか、
appのbuild.gradleにあるsceneform.assetのコードを一旦削除してから再度入れなおしてください。

また、このときに次のようにViewerが起動しますが、テクスチャがうまく機能していない場合に真っ白になることがあります。
その場合は、mtlファイルにおけるテクスチャの画像のPathが間違っているか、SampleData Directoryにテクスチャの画像が入ってないなどの原因が考えられますので、
mtlファイルのPathとテクスチャ画像が入っているかを確認してください。

これで、objファイルをAndroidStudioに入れることができました。


ここから、導入したobjファイルをARとして表示していきましょう。

最初に、ARとして表示するために、AnchorとNodeというクラスについて説明します。
Anchorは、現実世界の位置と向きを決めます。ARを表示するためのスペースを決めるFieldのような役割を担います。
Nodeは、ちょっと理解が難しい部分であり公式のドキュメントによると、ARを表示するシーンの階層の中で変換を表すのが定義です。データ構造としてはツリー状の構造になります。
私の理解としては、場所を決めたり3Dモデルなどレンダリング可能なオブジェクトを表示する際につなぐオブジェクトのような役割をもつクラスと考えています。
次のような公式のサンプル(https://developers.google.com/ar/develop/java/images/sceneform-plane.jpg)で示されている、
白いドットのスペース(Plane)にAnchorで現実世界の位置と向きを決め、AnchorNodeで構築しているシーンに位置を変換し、レンダリングした3DモデルのオブジェクトをNodeで変換し表示しているのだと考えます。

//image[PlaneRender][公式サンプル画像(Plane)]{
    PlaneRender
//}

次に、AndroidStudioに導入した3Dモデルを表示するまでを作成しながらやっていきましょう。

まず、レイアウトのxmlを用意します。
res > layoutにて、New > Layout Resource fileから新規にxmlファイルを作りましょう。
自分の場合は、activity_model.xmlとして次のように用意しました。
//emlist[][]{
  <?xml version="1.0" encoding="utf-8"?>
  <android.support.constraint.ConstraintLayout xmlns:android="http://schemas.android.com/apk/res/android"
    xmlns:app="http://schemas.android.com/apk/res-auto"
    xmlns:tools="http://schemas.android.com/tools"
    android:layout_width="match_parent"
    android:layout_height="match_parent"
    tools:context=".MainActivity">

    <fragment
      android:id="@+id/ux_fragment"
      android:name="com.google.ar.sceneform.ux.ArFragment"
      android:layout_width="match_parent"
      android:layout_height="match_parent"/>

  </android.support.constraint.ConstraintLayout>
//}

ArFragmentは公式で用意されているFragmentで、共通のAR機能のために必要なレイアウトと処理を持っています。
3Dモデルの表示等をする際に、ARFragmentを利用すると簡単に実装できるのでこちらを利用していきましょう。

次に、作成したレイアウトを表示するActivityの作成を行なっていきましょう。
app > javaにて右クリックをし、New > java classから自分で名前を決めてActivityを追加してください。
私の場合は、ModelActivityと定義し、次のリンクのように書いています。
@<href>{https://github.com/yutaro6547/ARCoreSampleForTechBookFest/blob/master/app/src/main/java/zukkey/whiskey/com/arcoresamplefortechbookfest/ModelActivity.java, ModelActivity}

Pluginで導入した3Dモデルをレンダリング可能なオブジェクトとして作成するために次のコードが必要です。
//emlist[][]{
  private ModelRenderable modelRenderable;

  @Override
  protected void onCreate(@Nullable Bundle savedInstanceState) {
  ...
   ModelRenderable.builder()
       .setSource(this, R.raw.rin)
       .build()
       .thenAccept(renderable -> modelRenderable = renderable)
       .exceptionally(
           throwable -> {
             Toast toast =
                 Toast.makeText(this, "Unable to load andy renderable", Toast.LENGTH_LONG);
             toast.setGravity(Gravity.CENTER, 0, 0);
             toast.show();
             return null;
           });
  ...
  }
//}

ModelRenderable.builder()は、Pluginで導入した3Dモデルの.sfbファイルをレンダリング可能なオブジェクトとして作成するためのクラスです。
これを、Planeと呼ばれる白いドット上に表示するために下記のコードを追加します。
//emlist[][]{
  private ArFragment arFragment;

  @Override
  protected void onCreate(@Nullable Bundle savedInstanceState) {
  ...
   arFragment = (ArFragment) getSupportFragmentManager().findFragmentById(R.id.ux_fragment);
   arFragment.setOnTapArPlaneListener(
       (HitResult hitResult, Plane plane, MotionEvent motionEvent) -> {
         if (modelRenderable == null) {
           return;
         }

         Anchor anchor = hitResult.createAnchor();
         AnchorNode anchorNode = new AnchorNode(anchor);
         anchorNode.setParent(arFragment.getArSceneView().getScene());

         TransformableNode model = new TransformableNode(arFragment.getTransformationSystem());
         model.setParent(anchorNode);
         model.setRenderable(modelRenderable);
         model.getRotationController();
         model.getScaleController();
         model.getTranslationController();
         model.select();
       });
  ...
  }
//}
AnchorNodeでは、Planeと呼ばれる白いドット状のフィールドにてタップすることで位置と向きを決めて変換します。
その上に、TransformableNodeでARFragmentが持つどのノードが選択されているか、ジェスチャの検出や座標を取得するarFragment.getTransformationSystem()を渡してインスタンス化します。
TransformableNodeに対して、setRenderableで先ほど作成したレンダリング可能なオブジェクトであるmodelRendarableをセットすることでARとして表示することができます。
TransformableNodeは、拡大縮小や移動、回転をするかを設定することができます。
拡大縮小を行いたい場合は、getScaleControllerを呼び出します。移動するようにしたい場合は、getTranslationControllerを、回転させるようにしたい場合には、getRotationControllerを呼び出すことで設定できます。

ここまでできたら、実際にビルドしてPlaneをタップして読み込んだ3Dモデルが表示されることを確認してみましょう。
筆者が、実行した結果は次の通りです。
//image[Result][実行結果]{
    Result
//}

これで、任意の3Dモデルの読み込みと表示までができたと思います。

=== Android標準のWidgetの表示
Androidで標準で用意されている、Widgetの表示をARとして表示させることができます。
また、Widgetとしての機能を使用することができます。例えば、ARとして表示したButtonをタップした際にクリック後の処理を行うことができ標準のWidgetと同様の機能を使用することが可能です。

まずはじめに、Widgetの表示を行うところから始めていきましょう。
AndroidのWidgetを表示する前に3Dモデルや図形の表示を行う際にも共通してGoogleのArFragmentを用いる必要があります。
layoutのxmlファイルをはじめに用意します。


=== 図形の表示



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
