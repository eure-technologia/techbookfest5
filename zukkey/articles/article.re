
= ARCoreをAndroidStudioではじめよう！
== はじめに
こんにちは！はじめまして！
普段はAndroidアプリの開発をメインでやっている、@<href>{https://twitter.com/zukkey59, @zukkey}です。
今回は、ARに興味を持ちましてAndroidでARCoreを導入して開発してみたいという初心者の方向けに導入手順の紹介とどういったことができるのかなどを解説していこうと思います。

* この本に書いてあること
** 初心者でARを始めてみたい！という方で導入とかよくわかんない人に導入から紹介します
** UnityじゃなくてAndroidStudioでやる方法を書いています


== 目次
この本で紹介することの一覧は下記になります

* ARCoreとは

* AndroidStudioで開発するための準備

* ARCoreでできること

** Sceneform

*** Widgetの表示

*** 図形の表示

*** 3Dモデルの表示

**** Blenderで3Dモデルを作る

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

// TODO: setupのことを書く。つまづきやすいところとか、勘違いしやすいとこを書いておく。

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
