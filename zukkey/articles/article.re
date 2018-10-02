
= 序章
== はじめに
こんにちは！はじめまして！
普段はAndroidアプリの開発をメインでやっている、@<href>{https://twitter.com/zukkey59, @zukkey}です。
今回は、ARに興味を持った筆者がAndroidでARCoreを導入して開発してみたいという初心者の方向けに導入手順の紹介とどういったことができるのかなどを解説していこうと思います。

対象読者と大体の内容は次のとおりです。

 * 対象読者
 ** 初心者でARを始めてみたい！という方に導入から紹介します
 ** UnityじゃなくてAndroidStudioでやる方法を書いています

 * ARCoreとは
 ** AndroidStudioで開発するための諸準備
 ** ARCoreでできることの内容

 * Sceneform
 ** 3Dモデルの導入と表示のやり方
 ** AndroidのWidgetの表示とクリック処理などのやり方

 * Augmented Images
 ** The arcoreimg toolを使って画像マーカーにできるかを知り、データベースを作成する方法
 ** 画像をマーカーにして3Dモデルを表示するまでの手順と解説

= ARCoreをAndroidStudioではじめよう！
== ARCoreとは
ARCoreとは、Googleが提供しているSDKです。AR機能に必須のモーショントラッキングや環境、
光の加減など全てサポートしてくれており、SDKを利用することで既存のアプリケーションにAR機能を搭載したり、新しいARアプリケーションを構築することができます。
OpenGLなどの専門的な知識が無くても、比較的手軽に試すことができるのも特徴です。

公式の概要は下記リンクを参照してください。

@<href>{https://developers.google.com/ar/develop/, 公式リンク:https://developers.google.com/ar/develop/}

== AndroidStudioで開発するための準備
AndroidStudioで開発を始めるにはいくつか準備が必要です。

すでに準備が整っている場合や公式ドキュメントを読んだことがある方は、スキップしていただいて結構です。
この説では、公式のドキュメントの意訳が中心になります。

=== 開発環境をセットアップする
まずは、開発環境を整えていきましょう。

AndroidStudioはversion3.1以上のものをインストールしてください。

AndroidStudio > About AndroidStudioから次の画像のようにバージョンの確認ができます。

//image[techbookfest5-1][AndroidStudioのバージョン確認画面][scale=0.8]{
    techbookfest5-1
//}

エミュレータを利用する場合には、Android Emulator27.2.9以上が必要です。
また、エミュレータでOpenGL ES 3.0以上をサポートし、有効にする必要があります。

ARCoreを利用するために下記の3点を設定する必要があります。
1. AndroidManifestを設定する
2. ビルドの依存関係を追加する
3. ランタイムチェックを実行して、カメラのパーミッションを許可する

=== AndroidManifestを設定する
AR機能を利用するために、まずはじめにAndroidManifestの設定を行う必要があります。
AR機能を必須にする場合と、必須にしない場合によって設定する項目が違います。

 * AR機能を必須にする場合
AR機能必須アプリの場合には、minSdkVersionが24以上になります。
AndroidManifestには、次のように追加してください。
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
...は省略を意味しており、次回以降も同様です。google()がrepositoriesの中にあれば大丈夫です。

次に、appのbuild.gradleの中にてARCoreのライブラリの最新バージョンを追加してください。
2018年10月現在の最新バージョンは1.4.0になります。

最新バージョンの確認は次のリンク先のgithubのreleasesから確認できます。
@<href>{https://github.com/google-ar/arcore-android-sdk/releases, https://github.com/google-ar/arcore-android-sdk/releases}

=== ランタイムチェックを実行して、カメラのパーミッションを許可する
AR機能をオプションとして設定した場合には、ランタイムチェックを実行する必要があります。AR機能を必須とした場合は必要ありません。
AR機能を利用する前に、AR機能を利用できる端末をチェックする必要があります。
次のようにしてランタイムチェックを行います。
//emlist[][]{
  ArCoreApk.Availability availability =
                        ArCoreApk.getInstance().checkAvailability(this);
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
    if (ContextCompat.checkSelfPermission(this, CAMERA_PERMISSION)
                                     != PackageManager.PERMISSION_GRANTED) {
      ActivityCompat.requestPermissions(this,
                                        new String[] {CAMERA_PERMISSION},
                                        CAMERA_PERMISSION_CODE);
    }

    …
  }
//}
ContextCompat.checkSelfPermissionで、カメラのパーミッションについて確認し、ない場合にはパーミッションのリクエストを行うようにしています。
onResumeに記載しているのは、onCreateだと一度パーミッションの許可をしないようにすると設定からやり直さないとカメラが起動しないので、都度確認できるonResumeに記載しています。

これで、カメラのパーミッションの許可については終わりです。
公式ドキュメントにも書かれていますが、ARCoreがインストールされているかをチェックする必要が場合によって出てきますがSceneformを利用している今回のような場合では、
ArFragmentが自動的に行ってくれているために言及しません。


== ARCoreでできること
ARCoreでできることは、次の３つになります。

1. Sceneform
Sceneformでは、Androidで標準で用意されているWidgetをARとして表示することができたり、図形の表示や3Dモデルを表示することが可能です。
また、表示したオブジェクトを拡大、縮小したり、移動したりクリックイベントによって他の動作をさせることもできます。

2. Augmented Images
Augmented Imagesでは、自分で用意した画像をARを表示するためのマーカーにして用意した3Dモデルを表示することが可能です。
どんな画像でも表示することができるわけではないですが、The arcoreimg toolを使って使用できる画像かを調べて条件を満たせばその画像をマーカにすることができます。

3. Cloud Anchors
Cloud Anchorsでは、特定の場所で出現させたARを２つの端末で操作することが可能です。
GCPを経由してアンカーを作って、出現させたオブジェクトを操作することができます。
今回は諸事情でこちらについては記載しておりません。

次の章から、具体的にどういったことができるのかなどを解説して行きます。

= Sceneform
Sceneformを用いることで、3Dモデルを表示したり、Androidの標準のWidget(TextView, ImageView, etc..)などを表示したり、図形の表示することが可能です。
この章では、3Dモデルの表示とAndroid標準のWidgetをARとして表示するために導入から説明して行きます。

== 3Dモデルの表示

3Dモデルを導入する前に、プラグインをAndroidStudioに入れる必要があります。
AndroidStudioのPreferencesのPlugins > Browse repositoriesにてGoogle Sceneform Toolsと検索してください。
検索した結果が次のようになると思います。

//image[plugin01][Google Sceneform Tools検索時][scale=0.8]{
    plugin01
//}

installしていない場合は、installと表記が出るのでinstallをクリックしてpluginをインストールしましょう。
Google Sceneform Toolsは、任意の3DモデルのobjファイルやfbxファイルなどをAndroidStudioに簡単に入れることができたり、実際に動かす前にViewerで3Dモデルの表示の確認をすることができたり、
build.gradleの依存関係を自動的に追加してくれるプラグインになります。

それでは実際に、3Dモデルを導入していくところからやっていきましょう。

導入の前に、もし3Dモデルを用意するのが難しい場合は、Google公式で次のサイトから3Dモデルを利用することが可能です。
//image[poly01][Poly - 無料の3Dオブジェクトライブラリ][scale=0.8]{
    poly01
//}


@<href>{https://poly.google.com/, Poly}
Polyは、GoogleがARまたはVR開発者向けに用意してくれている無料の3Dオブジェクトライブラリです。
Polyでは、3Dモデルの作成は難しいがARを表示するのに手軽に試したい技術者向けにobjファイルやfbxファイルなど、ARやVRに用いることができるサンプルをダウンロードすることができます。
サンプルには著作権があるので、目的によって著作権の表記を忘れずにつけるようにしてください。

また、他にも無料で公開され二次創作利用できる3Dモデルキャラクターがあるので、そちらでも利用することが可能です。
私は、今回説明するにあたってImagineVR Inc.さんよりImagin Girlsのアセットをお借りしてきました。

//image[ImagineGirls][ImagineVR Inc.さんの公式サイト][scale=0.8]{
    ImagineGirls
//}

ImagineVR Inc.さんのサイトは下記リンクから行けます。
@<href>{http://imaginegirls.com/, ImagineVR Inc.}
また、Imagine Girlsのアセットは二次創作物の製作と二次創作物の無償および有償での頒布をすることはできますが、ライセンスの表記とリンクの表記が必須ですので創作の際に利用する際にはライセンスの表記を忘れないようにしてください。
Imagine Girlsのライセンスは下記リンクから参照可能です。
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

objファイルとmtlファイルは、3Dに関わる開発をしていない方にとっては見慣れないと思いますので、
それぞれの役割について把握しましょう。

書き出された２つのファイルは、それぞれ異なる役割を持っています。
objファイルはモデルの形状を表現するデータで、mtlファイルはモデルのマテリアルを表現するデータになります。
objが形状を決め、mtlがどのテクスチャを選ぶかを担います。
テクスチャというのは、3Dモデルの表面に貼り付けられるキャラクターの肌の色であったり、服の色であったりする部分になります。
objファイル、mtlファイル、テクスチャが揃って初めて3Dモデルを導入することが可能です。
mtlファイルの中のテクスチャのPathの指定が間違っていたり、テクスチャの画像がなかったりすると真っ白な3Dモデルの表示になりますので全てが揃っていることを確認してください。

次に、AndroidStudioでappフォルダを右クリックして New > Sample Data Directoryを選択してください。
そうすると、sampledataというディレクトリがapp配下に追加されます。
追加されたのを確認したら、先ほどダウンロードしてきたImagineGirlsフォルダの中にあるtexuturesフォルダの中から、3Dモデルの肌や服の色となるテクスチャのpng群と、
objファイル及びmtlファイルをSample Data Directoryの中に移動させます。MacならFinderから直接移動させて大丈夫です。

sampledata directoryの中身が次のとおりになっていることを確認してください。
//image[SampleData][ファイル移動後のSampleDataフォルダ][scale=0.5]{
    SampleData
//}
必ずobjファイル、mtlファイル、テクスチャの画像などが全て入っていることを確認してください。

その次にsampledataを右クリックして、New > Sceneform Assetを選択します。
選択した後、次の画像のようにImport Wizardが出てきます。
//image[Sceneform01][選択前のImport Wizard][scale=0.8]{
    Sceneform01
//}

Import Wizardには、４つの項目がありますが、ここではSource Asset Pathと.sfb Output Pathを設定します。
Source Asset Pathでは、先ほどダウンロードしてきたobjファイルを選択し、.sfb Output Pathではassetsもしくはres配下にrawフォルダを追加してrawのPathを設定します。
res > rawを.sfb Output Pathに指定している場合は_がPathに入っていると、AndroidStudioに怒られるので_を排除した名前に.sfb Output Pathを変更してください。

今回は、res配下にてrawフォルダを追加してPathを設定していきます。
ここまで設定をすると、次の画像のようになります。

//image[Sceneform02][選択後のImport Wizard][scale=0.8]{
    Sceneform02
//}

この状態でFinishを選択すると、次のようにエラーが吐かれることがあります。
ERROR: Multiple different usages defined for texture: FX
これは、mtlファイル内でFXというテクスチャが複数定義されているために起こるバグになります。
//image[mtlFileError][mtlファイルのError画面][scale=0.8]{
    mtlFileError
//}

map_dとmap_Kdが同じテクスチャを指定しているのが原因なので、同名のテクスチャの画像をコピーして名前を変えてPath名をどちらか片方変更する必要があります。
また、テクスチャの画像をSample Data Directoryに変更しているため、mtlファイルにてmap_dとmap_KdのPathがデフォルトのままになっていますので変更する必要があります。
map_d hogehoge/FX.pngとなっているのを、map_d ./FX.pngと修正してください。

導入がうまくいくと、次のような画面に自動的に遷移します。
//image[importComplete][3Dモデル導入後の画面][scale=0.8]{
    importComplete
//}


appのbuild.gradleに次のコードが追加され、res > raw配下に.sfbファイルが自動的に追加されます。
//emlist[][]{
  sceneform.asset('sampledata/R1N4_V2_FREE.obj',
          'default',
          'sampledata/R1N4_V2_FREE.sfa',
          'src/main/res/raw/rin')
//}

このコードは自分で追加するのではなく、あくまでプラグイン経由で入れた場合に自動的に追加されます。

もし、何らかの事情で入れ直したい場合、すでに同じ名前のものが作られているいて、エラーが吐かれますのでその場合は名前を変更するか、
appのbuild.gradleにあるsceneform.assetのコードをいったん削除してから再度入れなおしてください。

また、このときに次のようにViewerが起動しますが、テクスチャがうまく機能していない場合に真っ白になることがあります。
その場合は、mtlファイルにおけるテクスチャの画像のPathが間違っているか、SampleData Directoryにテクスチャの画像が入ってないなどの原因が考えられますので、
mtlファイルのPathとテクスチャ画像が入っているかをもう一度確認してください。

これで、objファイルをAndroidStudioに入れることができました。


ここから、導入したobjファイルをARとして表示していきましょう。

最初に、ARとして表示するために、AnchorとNodeというクラスについて説明します。
Anchorは、現実世界の位置と向きを決めます。ARを表示するためのスペースを決めるFieldのような役割を担います。

Nodeは、ちょっと理解が難しい部分であり公式のドキュメントによると、ARを表示するシーンの階層の中で変換を表すのが定義です。データ構造としてはツリー状の構造になります。

私の理解としては、場所を決めたり3Dモデルなどレンダリング可能なオブジェクトを表示する際につなぐオブジェクトのような役割をもつクラスと考えています。
次のような公式のサンプル(https://developers.google.com/ar/develop/java/images/sceneform-plane.jpg)で示されている、
白いドットのスペース(Plane)にAnchorで現実世界の位置と向きを決め、AnchorNodeで構築しているシーンに位置を変換し、
レンダリングした3DモデルのオブジェクトをNodeで変換し表示していると考えます。

//image[PlaneRender][公式サンプル画像(Plane)][scale=0.8]{
    PlaneRender
//}

次に、AndroidStudioに導入した3Dモデルを表示するまでを実際に作成しながらやっていきましょう。

まず、レイアウトのxmlを用意します。
res > layoutにて、New > Layout Resource fileから新規にxmlファイルを作りましょう。
自分の場合は、activity_model.xmlとして次のように用意しました。
//emlist[][]{
  <?xml version="1.0" encoding="utf-8"?>
  <android.support.constraint.ConstraintLayout
    xmlns:android="http://schemas.android.com/apk/res/android"
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

レイアウトの用意ができたら、作成したレイアウトを表示するActivityの作成を行なっていきましょう。
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
                 Toast.makeText(this, "Unable to load andy renderable",
                                        Toast.LENGTH_LONG);
             toast.setGravity(Gravity.CENTER, 0, 0);
             toast.show();
             return null;
           });
  ...
  }
//}

ModelRenderable.builder()は、Pluginで導入した3Dモデルの.sfbファイルをレンダリング可能なオブジェクトとして作成するためのクラスです。
これを、Planeと呼ばれる白いドット上に表示するために次のコードを追加します。
//emlist[][]{
  private ArFragment arFragment;

  @Override
  protected void onCreate(@Nullable Bundle savedInstanceState) {
  ...
   arFragment = (ArFragment) getSupportFragmentManager()
                                           .findFragmentById(R.id.ux_fragment);
   arFragment.setOnTapArPlaneListener(
       (HitResult hitResult, Plane plane, MotionEvent motionEvent) -> {
         if (modelRenderable == null) {
           return;
         }

         Anchor anchor = hitResult.createAnchor();
         AnchorNode anchorNode = new AnchorNode(anchor);
         anchorNode.setParent(arFragment.getArSceneView().getScene());

         TransformableNode model =
                   new TransformableNode(arFragment.getTransformationSystem());
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

その上に、TransformableNodeでARFragmentが持つどのノードが選択されているか、
ジェスチャの検出や座標を取得するarFragment.getTransformationSystem()を渡してインスタンス化します。
setParentにてTransformableNodeをAnchorNodeにつけて、
TransformableNodeに対して、setRenderableで先ほど作成したレンダリング可能なオブジェクトであるmodelRendarableをセットすることでARとして表示することができます。

TransformableNodeは、拡大縮小や移動、回転をするかを設定することができます。
拡大縮小を行いたい場合は、getScaleControllerを呼び出します。
移動するようにしたい場合は、getTranslationControllerを、回転させるようにしたい場合には、getRotationControllerを呼び出すことで設定できます。

また、移動させたり、拡大縮小や、回転などを行う必要がない場合は、TransformableNodeではなくNodeクラスを用いて表示だけ行うことも可能です。

ここまでできたら、実際にビルドしてPlaneをタップして読み込んだ3Dモデルが表示されることを確認してみましょう。
筆者が、実行した結果は次のとおりです。
//image[Result][実行結果][scale=0.3]{
    Result
//}

これで、任意の3Dモデルの読み込みと表示までができました。
次は、Android標準で用意されている、各種WidgetをARとして表示する方法について解説します。

== Android標準のWidgetの表示
ARCoreを利用することで、Androidで標準で用意されているWidgetをARとして表示させることができます。
Androidで標準で用意されているWidgetというのは、TextViewやButtonなど、Androidの開発者ならよく目にすると思います。
また、Widgetとしての機能を使用することができます。例えば、ARとして表示したButtonをタップした際に、クリック後の処理を行うことができ標準のWidgetと同様の機能を使用することが可能です。

まずはじめに、Widgetの表示を行うところから始めていきましょう。
Widgetの表示を行うには、２種類のレイアウトが必要です。
前説の、3Dモデルを表示する際に用意したactivity_model.xmlのようにARFragmentをもつxmlファイルと、表示するWidget単体のxmlファイルが必要です。
ArFragmentをもつxmlに関しては、前節と同じように用意してください。
今回は、新たに表示したいWidget用のxmlファイルを作成します。私は、ImageViewを表示したいので、ImageView用のファイルをitem_image.xmlとして用意しました。
//emlist[][]{
  <?xml version="1.0" encoding="utf-8"?>
  <ImageView android:id="@+id/image"
    android:layout_width="wrap_content"
    android:layout_height="wrap_content"
    android:src="@drawable/eure"
    xmlns:android="http://schemas.android.com/apk/res/android" />
//}

ImageView単体を表示したいので、上のように定義していますが、複数のViewをまとめて表示したい場合はRelativeLayoutなどでまとめて定義することは可能です。
ただし、ArFragmentをもつxmlファイルとは別々に分ける必要があります。

次に、新しくActivityを作成してWidgetをレンダリング可能なオブジェクトにします。私の場合は、ViewActivity.javaを作成しました。
次のようにコードを追加してください。activity_viewはArFragmentをもつxmlファイルになります。
//emlist[][]{
  private ViewRenderable imageViewRenderable;

  @Override
  protected void onCreate(@Nullable Bundle savedInstanceState) {
    super.onCreate(savedInstanceState);
    setContentView(R.layout.activity_view);

    ...
    ViewRenderable.builder()
        .setView(this, R.layout.item_image)
        .build()
        .thenAccept(renderable -> imageViewRenderable = renderable);
    ...
  }
//}

ViewRenderableクラスは、2DのAndroidのViewを3D空間にレンダリングしてくれるクラスです。デフォルトでは、250dpごとにビューのサイズが1mの大きさになります。
3Dモデルの時と同様にして、PlaneにAnchorNodeとViewのNodeを作成してアタッチします。
具体的には次のように実装します。

//emlist[][]{
  private ArFragment arFragment;

  @Override
  protected void onCreate(@Nullable Bundle savedInstanceState) {
    super.onCreate(savedInstanceState);
    setContentView(R.layout.activity_view);

    ...
    arFragment.setOnTapArPlaneListener(
        (HitResult hitResult, Plane plane, MotionEvent motionEvent) -> {
          if (imageViewRenderable == null) {
            return;
          }

          Anchor anchor = hitResult.createAnchor();
          AnchorNode anchorNode = new AnchorNode(anchor);
          anchorNode.setParent(arFragment.getArSceneView().getScene());
          TransformableNode image =
                   new TransformableNode(arFragment.getTransformationSystem());
          image.setParent(anchorNode);
          image.setRenderable(imageViewRenderable);
          image.select();
        });
    ...
  }
//}
AnchorNodeをPlaneにつけ、setParentで親のNodeであるAnchorNodeにTransformableNodeをつけ、setRendarableで作成したレンダリング可能なオブジェクトをセットします。
これで実行した結果が、次のようになります。
//image[Result2][実行結果2][scale=0.3]{
    Result2
//}

また、これだけではなく、Widgetをクリックした際に何かの処理を行うということもできます。
今回は、Buttonを次のようにitem_text.xmlを用意し、クリックした際にwebviewに飛ばしトーストを表示するまでをやっていきます。
//emlist[][]{
  <?xml version="1.0" encoding="utf-8"?>
  <Button xmlns:android="http://schemas.android.com/apk/res/android"
    android:layout_width="match_parent"
    android:layout_height="match_parent"
    android:textSize="18sp"
    android:textColor="@color/colorAccent"
    android:background="@drawable/ripple_text"
    android:text="BOOTHにて電子版販売中！\nタップして詳細を見れるよ！"/>
//}
通常の開発で使用するButtonと同じように、rippleをつけることも可能です。

先ほどと同じようにViewRendarableクラスで、レンダリング可能なオブジェクトを作成して、Nodeをそれぞれ用意しセットします。
違う点だけをコードで示すと次のとおりです。
//emlist[][]{
  private ArFragment arFragment;
  private ViewRenderable textViewRenderable;

  @Override
  protected void onCreate(@Nullable Bundle savedInstanceState) {
    ...
    arFragment.setOnTapArPlaneListener(
        (HitResult hitResult, Plane plane, MotionEvent motionEvent) -> {
          ...
          Button btn = (Button) textViewRenderable.getView();
          btn.setOnClickListener(view -> {
            Toast.makeText(this, "ボタンをタップしました！詳細へ遷移します",
                                                   Toast.LENGTH_SHORT).show();
              startActivity(WebActivity.createIntent(this));
              finish();
            });
          ...
        });
    ...
  }
//}
Buttonをレンダリング可能なオブジェクトにしたtextViewRenderableを、getViewでButtonを取得できます。
通常のButtonのクリック処理と同じようにして、setOnClickListenerの中でクリックした時の処理を書くことでARとして表示したButtonにクリック後の処理を持たせることができます。
サンプルを動かすことで挙動が確認できるので、下記リンクからソースコードをダウンロードしてビルドして試してみてください。
@<href>{https://github.com/yutaro6547/ARCoreSampleForTechBookFest, https://github.com/yutaro6547/ARCoreSampleForTechBookFest}


= Augmented Images
Augmented Imagesでは、特定の画像をマーカーにして3Dモデルを表示することができます。
全ての画像で利用できるわけではないですが、arcoreimgと言われる公式のツールで特徴点のスコアを基準以上で満たせば利用できます。

== The arcoreimg toolを使って画像マーカーにできるかを知る
Augmented Imagesでは画像をマーカーにすることができるのですが、必ずしも全ての画像をマーカーにできるわけではありません。
画像をマーカーにできるかを知るために、The arcoreimg toolというツールがGoogle公式で用意されているのでこちらを利用しましょう。

@<href>{https://developers.google.com/ar/develop/java/augmented-images/arcoreimg, The arcoreimg tool}とは、AndroidおよびAndroid NDK開発者向けのコマンドラインツールです。
画像マーカーとしての画質をチェックしたり、Augmented Imagesで利用するための画像データベースの作成を行うことができます。

=== The arcoreimg toolの導入
The arcoreimg toolを導入するために次のリンクのLatest releaseのarcore-android-sdkの最新版のzipをダウンロードしてください。

@<href>{https://github.com/google-ar/arcore-android-sdk/releases, https://github.com/google-ar/arcore-android-sdk/releases}

ダウンロードした圧縮フォルダを解凍後に、tools > arcoreimgと進むとlinux、macos、windowsのOSごとに実行ファイルが用意されているのでそちらをダブルクリックしてください。
クリック後に、次の画像のようにプロセスが完了しました、という表記が出れば導入完了です。

//image[arcoreimg01][プロセス完了後の画面][scale=0.5]{
    arcoreimg01
//}

=== The arcoreimg toolを使って画像をマーカーにできるか確認する
ターミナルを開き、ダウンロードしたarcore-android-sdkの中のtools > arcoreimgに移動後に、各プラットフォームのところまでディレクトリ移動してください。
macの場合は、macosまでディレクトリに移動した後に、./arcoreimg eval-img --input_image_path=hogehoge/fugafuga.jpgを入力することで、fugafuga.jpgの特徴点のスコアが観れます。
このスコアが75以上でないと、画像をマーカーにすることができません。
試しに、前回の技術書典4で販売したエウレ・テクノロギアを画像マーカーにできるか調べてみたところ次のような結果となりました。

//image[arcoreimg02][エウレ・テクノロギア評価後画像]{
    arcoreimg02
//}
諸事情により一部白色で隠させていただいておりますが、無事エウレ・テクノロギアはスコアが95あるので画像マーカーとして利用できることがわかりました。

このeval-imgを行う際に気をつけて欲しい点として、--input_image_pathの後の=の間にスペースを入れたりしないようにしてください。また、画像までのパスを入力するようにしてください。
エラーが吐かれている際には上記２点を疑ってみてください。筆者はこれでハマりました。

画像マーカーにしたい画像のスコアが75以上であることを確認できたら、実際に画像をマーカーにして3Dモデルを表示していきましょう。

== 画像をマーカーにして3Dモデルを表示してみよう
画像をマーカーにするために、まずはじめに画像のデータベースを作成する必要があります。
先ほど導入した、arcoreimg toolを利用してAndroidStudioに画像のデータベースを作成しましょう。

先ほどと同じように、ターミナルを開きダウンロードしたarcore-android-sdkの中のtools > arcoreimgに移動後に、各プラットフォームのところまでディレクトリを移動してください。
自分のOSのところまで移動した後に、次のように入力します。
//quote{
  ./arcoreimg build-db --input_images_directory=/path/to/images --output_db_path=/path/to/sample_database.imgdb
//}
input_images_directoryは先ほど確認した画像のディレクトリを入力してください。
hogehoge/fugafuga.jpgであればhugehugeまでを入力すれば大丈夫です。
output_db_pathはAndroidStudioのassetsのPathを指定するのが良いでしょう。~/Project名/app/src/main/assets/sample_database.imgdbと入力すれば大丈夫です。
.imgdbの前はどんな名前でも大丈夫なので、自分の好きな名前を入れてください。筆者は、sample_database.imgdbと命名しました。

ターミナルにて上記のPathを指定した後にエンターキーを押すと、assetsの配下にsample_database.imgdbとsample_database.imgdb-imglist.txtが作成されていれば、
データベースの作成が完了です。

気をつけて欲しいこととして、=の前後にスペースを入れないことと、公式のドキュメントに書かれているのですがoutput_db_pathの前に\を入れないようにしてください。
\が入っていると、うまく動作せずエラーになります。筆者はこれでだいぶハマりました。

画像のデータベースを作成できたら、例によってxmlファイルを用意しましょう。
3Dモデルのときと同じように、ArFragmentをもつxmlファイルを用意してください。
ArFragmentをもつxmlファイルを用意できたら、新しくActivityを作成しましょう。
私は、AugmentedImage用にAugmentedImageActivityを作成しました。
全体については、下記リンクより確認することができます。
@<href>{https://github.com/yutaro6547/ARCoreSampleForTechBookFest, https://github.com/yutaro6547/ARCoreSampleForTechBookFest}

まずはじめに、ArFragmentはデフォルトだとPlaneの検出が始まるので、Planeの検出をオフにする必要があります。
Planeの検出をオフにするためには、次のコードを作成したActivityに追加してください。
//emlist[][]{
  private ArFragment arFragment;

  @Override
  protected void onCreate(@Nullable Bundle savedInstanceState) {
    ...
    if (arFragment != null) {
      arFragment.getPlaneDiscoveryController().hide();
      arFragment.getPlaneDiscoveryController().setInstructionView(null);
    }
    ...
  }
//}
getPlaneDiscoveryControllerでは、Planeをスキャンする役割を持っており、不要な場合はhideで無効化できるので、hideを呼ぶようにしましょう。
setInstructionViewでは、シーンに表示するというViewをセットする役割を持っているため、PlaneのViewを非表示にするためにsetInstructionView(null)で無効化しましょう。

次に、画像データベースに登録した画像のトラッキングに関する処理を追加します。
画像のトラッキングに関する処理を追加するために、次のコードを作成したActivityに追加してください。
//emlist[][]{
  private ArFragment arFragment;

  @Override
  protected void onCreate(@Nullable Bundle savedInstanceState) {
    ...
    arFragment.getArSceneView().getScene().addOnUpdateListener(frameTime -> {
      Frame frame = arFragment.getArSceneView().getArFrame();
      Collection<AugmentedImage> updatedAugmentedImages =
                             frame.getUpdatedTrackables(AugmentedImage.class);

      for (AugmentedImage img : updatedAugmentedImages) {
        if (img.getTrackingState() == TrackingState.TRACKING) {
          if (img.getName().contains("eure") && !isAttachedModel) {
            setUp3DModel(img.createAnchor(img.getCenterPose()));
          }
        }
      }
    });
    ...
  }
//}
公式のドキュメントにも書かれておりますが、ARのセッションが行われている中、カメラの画像から、
事前に登録した画像のデータベース内の特徴点と照らし合わせて、シーンを更新します。

シーンを更新する直前フレームごとにaddOnUpdateListenerの中でリスナーが呼ばれ、
事前に登録済みのエウレ・テクノロギアの表紙画像と一致していたら、
Widgetを3Dとして表示しています。

//emlist[][]{
public class AugmentedImageActivity extends AppCompatActivity {
  ...
  private void setUp3DModel(Anchor anchor) {
    ViewRenderable.builder()
        .setView(this, R.layout.item_text)
        .build()
        .thenAccept(renderable -> textViewRenderable = renderable)
        .exceptionally(
            throwable -> {
              Toast toast =
                  Toast.makeText(this, "Unable to load andy renderable",
                                        Toast.LENGTH_LONG);
              toast.setGravity(Gravity.CENTER, 0, 0);
              toast.show();
              return null;
            });

    if (textViewRenderable == null) {
      return;
    }

    AnchorNode anchorNode = new AnchorNode(anchor);
    anchorNode.setParent(arFragment.getArSceneView().getScene());

    TransformableNode text =
                 new TransformableNode(arFragment.getTransformationSystem());
    text.setParent(anchorNode);
    text.setRenderable(textViewRenderable);
    text.getRotationController();
    text.getScaleController();
    text.select();
    text.setLocalRotation(
            Quaternion.lookRotation(Vector3.down(), Vector3.up()));
    Button btn = (Button) textViewRenderable.getView();
    btn.setOnClickListener(view -> {
      Toast.makeText(this, "ボタンをタップしました！詳細へ遷移します",
                                       Toast.LENGTH_SHORT).show();
      startActivity(WebActivity.createIntent(this));
      finish();
    });
    isAttachedModel = true;
  }
  ...
}
//}

setUp3DModelの中身は、これまでの章で紹介してきた内容と同じでありますが、text.setLocalRotation(Quaternion.lookRotation(Vector3.down() ,Vector3.up()));で
3Dモデルの表示を画像に対して90度回転させて位置の調節を行なっています。これを行わないと、画像に対して垂直に3Dモデルが表示されます。

また、事前に登録した画像データベースを、ARのシステム状態を管理しライフサイクルを担うSessionクラスに結びつける必要があります。
onResumeの中で、下記のコードを追加します。
//emlist[][]{
  @Override
  protected void onResume() {
    super.onResume();
    if (session == null) {
      try {
        session = new Session(this);
      } catch (
        UnavailableArcoreNotInstalledException
        | UnavailableApkTooOldException
        | UnavailableSdkTooOldException e
      ) {
        e.printStackTrace();
      }
      sessionConfigured = true;
    }

    if (sessionConfigured) {
      configureSession();
      sessionConfigured = false;
    }
  }

  ...
  private void configureSession() {
    Config config = new Config(session);
    InputStream inputStream;
    try {
      inputStream = getAssets().open("sample_database.imgdb");
      AugmentedImageDatabase imageDatabase =
               AugmentedImageDatabase.deserialize(session, inputStream);
      config.setAugmentedImageDatabase(imageDatabase);
      config.setUpdateMode(Config.UpdateMode.LATEST_CAMERA_IMAGE);
      session.configure(config);
    } catch (IOException e) {
      Toast.makeText(this, "Don't configure Session",
                                          Toast.LENGTH_SHORT).show();
      e.printStackTrace();
    }
    arSceneView.setupSession(session);
  }
  ...
//}
Cofigクラスは、Sesionの設定を保持するためのクラスです。
公式ドキュメントにも書かれてあるとおり、既存の画像データベースが存在する場合にInputStreamクラスを用いて作成済みの画像データベースを利用することができます。
画像データベースをSessionに設定し、ArFragmentに対してセットする必要があります。

ただし、この際にconfig.setUpUpdateModeをConfig.UpdateMode.LATEST_CAMERA_IMAGEにしないと下記エラーを吐きます。
//quote{
java.lang.RuntimeException: Invalid ARCore UpdateMode BLOCKING, Sceneform requires that the ARCore session is configured to the UpdateMode LATEST_CAMERA_IMAGE.
//}
SessionではUpdateModeをConfig.UpdateMode.LATEST_CAMERA_IMAGEに設定する必要があるようなので、忘れずに追加してください。

ここまでできたら、画像をマーカーにして3Dモデルを表示することが可能です。
全体の挙動について確認したい場合、Augumented Imagesのサンプルの全体のソースコードは下記リンクから参照できるので、こちらを参照してビルドして試してみてください。
@<href>{https://github.com/yutaro6547/ARCoreSampleForTechBookFest/blob/master/app/src/main/java/zukkey/whiskey/com/arcoresamplefortechbookfest/AugmentedImageActivity.java, AugmentedImageActivity.java: https://github.com/yutaro6547/ARCoreSampleForTechBookFest/}

サンプルは、画像マーカーであるエウレ・テクノロギアの表紙を認識すると、ButtonのARが表示されタップするとWebViewでBoothの販売ページに遷移するようになっています。
Boothにて、画像のマーカーであるエウレ・テクノロギアが掲載されているので下記リンクから参照してテストを行うことができます。
@<href>{https://booth.pm/ja/items/830454, https://booth.pm/ja/items/830454}

= おわりに
　本書を手に取り、最後まで見ていただきありがとうございます。
　今回は、ARに興味を持った筆者が調べたことをまとめ、たびたび遭遇したハマったポイントなどを解説しました。筆者と同じようにARに興味を持ち、門戸を開いた方の助けに少しでもお役に立てれば嬉しいです。
　技術書典は、技術書典4に続き２回目でしたが個人として出した処女作だったのでところどころ拙くわかりづらい部分等あるかと思いますが、ご容赦いただけると幸いです。

　また、今回のサンプルのソースコードは下記リンクからいけるのでご確認ください。
　@<href>{https://github.com/yutaro6547/ARCoreSampleForTechBookFest, https://github.com/yutaro6547/ARCoreSampleForTechBookFest}

また、文書中でも記載しておりますが、ImagineVR Inc.さんよりARのアセットをお借りしております。
ライセンスは下記リンクになります。
@<href>{http://imaginegirls.com/imaginegirls_user_license_jp/, http://imaginegirls.com/imaginegirls_user_license_jp/}

　最後に、表紙を担当して頂いたデザイナーのKarin Iwazakiさん、一緒に頑張ったエウレ・テクノロギアのメンバーの皆さん、そして、諸々のマネジメントをして頂いた@futabooさんに感謝を。
　それでは、また次の機会に。
