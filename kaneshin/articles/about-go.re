
= Go言語の思想

GoはGoogleによって開発されたことは有名ですが、「なぜGoogleはGoが必要だったのか」という問いに答えられる人はあまりいません。Goが開発された背景についてはGo at Google@<fn>{fn_go_at_google}に記載されています。
//footnote[fn_go_at_google][Go at Google: https://talks.golang.org/2012/splash.article]

//quote{
The Go programming language was conceived in late 2007 as an answer to some of the problems we were seeing developing software infrastructure at Google. The computing landscape today is almost unrelated to the environment in which the languages being used, mostly C++, Java, and Python, had been created. The problems introduced by multicore processors, networked systems, massive computation clusters, and the web programming model were being worked around rather than addressed head-on. Moreover, the scale has changed: today's server programs comprise tens of millions of lines of code, are worked on by hundreds or even thousands of programmers, and are updated literally every day. To make matters worse, build times, even on large compilation clusters, have stretched to many minutes, even hours.

Go was designed and developed to make working in this environment more productive. Besides its better-known aspects such as built-in concurrency and garbage collection, Go's design considerations include rigorous dependency management, the adaptability of software architecture as systems grow, and robustness across the boundaries between components.
//}

Go at Googleには、Googleではソフトウェアの開発やさまざまな環境下で増え続ける問題への対処のために開発されたと書かれていたり、また、開発の利便性を捨てて、大規模ソフトウェアの問題に対処することにフォーカスをしています。

つまり、Goは開発のしやすさではなく、大規模ソフトウェア開発にまつわる問題を解決するために開発されたと言えるでしょう。そのため、プログラミング言語を深く探求する人には言語仕様上、物足りなさを感じると思います。Goは問題解決のために作られたプログラミング言語ということが思想としてあります。

== UNIX哲学

プログラミングをしている人ならば、UNIX哲学という単語は一度は聞いたことがあると思います。UNIX哲学では、プログラムやソフトウェア開発における、文化的な規範や哲学的なアプローチの集合であり、UNIXやソフトウェアの開発者たちの経験に基づいて創出されたものです。

Goを開発した方たちは、UNIXやC言語の開発者でもあるため、彼らが積み重ねてきた成功や失敗の経験を活かしています。それらを言語化されらが暗黙的にUNIX哲学にも結びついていると筆者は考えています。これから、ふたつのUNIX哲学をもとに、どのようにGoへ活かしていくかを解説します。

=== マキルロイのUNIX哲学

マルコム・ダグラス・マキルロイ (Malcolm Douglas McIlroy) は、UNIX創始者の一人であり、プロセス間通信を標準入出力を介して接続するパイプの発明者でもあります。マキルロイはUNIX哲学をつぎのように形にしています。

//quote{
これがUNIXの哲学である。
一つのことを行い、またそれをうまくやるプログラムを書け。
協調して動くプログラムを書け。
標準入出力（テキスト・ストリーム）を扱うプログラムを書け。標準入出力は普遍的インターフェースなのだ。
//}

"Do One Thing and Do It Well"、「ひとつのことを、うまくやれ」と要約されるこの哲学において、Goでツールを開発するとき、無駄な責務を与えないように開発をします。特に意識しているのはつぎの点です。

 1. ひとつの責務に集中させる
 2. 他のプログラムに連携可能にする

==== ひとつの責務に集中させる

プログラムをひとつの責務に集中させていれば複雑にならず、直感的にそのプログラムを使用することが可能です。例えば、パッケージやコマンドラインツールにおいて、クエリやオプション、引数の渡し方によってプログラムの振る舞いを変えるという実装を多々見てきましたが、それを一度許してしまうと、「またひとつ、またひとつ」のようにオプションが増えていき、プログラムが複雑化していきます。

プログラムの「責務」を最初に定義し、そこからはみ出してしまうこと、つまり、振る舞いを無理に変えてしまうことを避けて開発をすることでプログラムの複雑度を増加させないようにします。これによって、美しい、キレイなコードを保つこともできるでしょう。

==== 他のプログラムと連携可能にする

さて、ひとつの責務に集中させることに成功したら、別のプログラムと連携できるI/Fを提供します。ひとつの責務に集中させることは、別の責務については別のプログラムに責任を委譲していることになるため、その責務の受け渡しをスムーズに行う必要があります。

パッケージでは関数のシグネチャとして、入力となる引数、出力となる戻り値、そして、関数の名前を適切に設計して公開しなければなりません。アンチパターンとして、入力の引数に関数の振る舞いを変えるための@<tt>{boolean}を提供することは避けるべきです。

//emlist[][go]{
type Address struct {
    HomeTel string
    WorkTel string
}

func (a *Address) SetTel(tel string, isHome bool) {
    if isHome {
        a.HomeTel = tel
    } else {
        a.WorkTel = tel
    }
}
//}

@<tt>{SetTel}の第二引数に@<tt>{isHome}という@<tt>{boolean}値を渡す必要がありますが、呼び出し側では@<tt>{tel}を設定するだけなのに、なぜ、真偽値が必要になるかがコードを読まないと把握できません。このように、関数名から直感的に使えないI/Fを提供するのは避けるべきです。

コマンドラインツールについては、後述するツール作成の章にてお話します。

=== ガンカーズのUNIX哲学

マイク・ガンカーズ (Mike Gancarz) は、彼自身の経験を踏まえて、以下の9つをUNIX哲学として形にしています。

//quote{
1. 小さいものは美しい。
2. 各プログラムが一つのことをうまくやるようにせよ。
3. できる限り早く原型（プロトタイプ）を作れ。
4. 効率よりも移植しやすさを選べ。
5. 単純なテキストファイルにデータを格納せよ。
6. ソフトウェアを梃子(てこ)として利用せよ。
7. 効率と移植性を高めるためにシェルスクリプトを利用せよ。
8. 拘束的なユーザーインターフェースは作るな。
9. 全てのプログラムはフィルタとして振る舞うようにせよ。
//}

こちらは、「マキルロイのUNIX哲学」と似ているところがありますが、より具体的に書かれていることや、ガンカーズの経験が活かされた哲学になっています。

==== 動くものを作る

//quote{
3. できる限り早く原型（プロトタイプ）を作れ。
5. 単純なテキストファイルにデータを格納せよ。
//}

DRYの法則を知らないプログラマはいません（要出典）。しかし、DRYの法則を正確に理解している人は少ないです。

とあるプログラマはコードの重複を避けるために、あらゆる手段を用いてコードの重複を避けたが、逆に、複雑度が増加してしまい、メンテナンスコストが爆発的に増加してしまった。というおとぎ話のような話もあります。達人プログラマでDRYの法則について言及していたDave Thomasも「DRYのことを、コードを重複させてはいけない、という意味にとっている人が多いようですが、そうではありません。DRYの背景にある考えは、それよりもっと大きなものなのです。」と明言@<fn>{fn_dry_dave_thomas}しています。
//footnote[fn_dry_dave_thomas][DRY原則の利用: コードの重複と密結合の間: https://www.infoq.com/jp/news/2012/05/DRY-code-duplication-coupling]

==== すべてGoで書くな

//quote{
4. 効率よりも移植しやすさを選べ。
7. 効率と移植性を高めるためにシェルスクリプトを利用せよ。
//}

効率と移植を考えると、バイナリやコードのロジックの最適化、また、パッケージ設計の話に向かってしまう場面もありますが、そうではありません。

Goに慣れてくると、すべてをGoで実装してしまいたくなるのがエンジニアの性質ですが、既に世の中にあるツールを巧く組み合わせて、最速で問題を解決へ導くのが事業会社にいるエンジニアには必須なスキルです。例えば、ただ単にWeb APIを叩くだけのツールを作るのであれば@<tt>{curl}とシェルスクリプトで十分に実現が可能です。

「そこ、本当にGoで書く必要ある？」のように自問し、本来必要である責務にのみ集中させるプログラムを記述することで、複雑度を減少させることも重要です。

== 早すぎる最適化は諸悪の根源である

「動くものを作る」、「すべてGoで書くな」はドナルド・エルビン・クヌース (Donald Ervin Knuth) が述べている「早すぎる最適化は諸悪の根源である」にも関連しています。まずはプロトタイプとなる動くものを作ることにフォーカスし、動いてから無駄なものを省いていく作業に移行することも重要です。YAGNIの法則でもあるように、機能は必要となるときに追加することとし、それまではシンプルに保ち続けることが重要です。そして、それがまだ不確実性の高い不透明なプロダクトであるなら、最適化にも手を出さずに待ち続けることが大事です。

Goでは、@<tt>{testing}パッケージにてベンチマークも公式から提供されているので、最適化をする前に、まずは計測を行い、必要なところを対処していくようにします。ベンチマークの検証方法については、後述するテストの章にて解説します。

