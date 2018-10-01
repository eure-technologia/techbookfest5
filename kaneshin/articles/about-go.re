
= Go言語の思想

GoはGoogleによって開発されたことは有名ですが、「なぜGoogleはGoが必要だったのか」という問いに答えられる人はあまりいません。Goが開発された背景についてはGo at Google@<fn>{fn_go_at_google}に記載されています。
//footnote[fn_go_at_google][Go at Google: https://talks.golang.org/2012/splash.article]

//quote{
The Go programming language was conceived in late 2007 as an answer to some of the problems we were seeing developing software infrastructure at Google. The computing landscape today is almost unrelated to the environment in which the languages being used, mostly C++, Java, and Python, had been created. The problems introduced by multicore processors, networked systems, massive computation clusters, and the web programming model were being worked around rather than addressed head-on. Moreover, the scale has changed: today's server programs comprise tens of millions of lines of code, are worked on by hundreds or even thousands of programmers, and are updated literally every day. To make matters worse, build times, even on large compilation clusters, have stretched to many minutes, even hours.

Go was designed and developed to make working in this environment more productive. Besides its better-known aspects such as built-in concurrency and garbage collection, Go's design considerations include rigorous dependency management, the adaptability of software architecture as systems grow, and robustness across the boundaries between components.
//}

Go at Googleには、Googleではソフトウェアの開発や様々な環境下で増え続ける問題への対処のために開発されたと書かれていたり、また、開発の利便性を捨てて、大規模ソフトウェアの問題に対処することにフォーカスをしています。

つまり、Goは開発のしやすさではなく、大規模ソフトウェア開発にまつわる問題を解決するために開発されたと言えるでしょう。そのため、プログラミング言語を深く探求する人には言語仕様上、物足りなさを感じると思います。Goは問題解決のために作られたプログラミング言語ということが思想としてあります。

== UNIX哲学

プログラミングをしている人ならば、UNIX哲学という単語は一度は聞いたことがあると思います。UNIX哲学では、プログラムやソフトウェア開発における、文化的な規範や哲学的なアプローチの集合であり、UNIXやソフトウェアの開発者たちの経験に基づいて創出されたものです。

Goを開発した方たちは、UNIXやC言語の開発者でもあるため、彼らが積み重ねてきた成功や失敗の経験を活かしています。それらを言語化されらが暗黙的にUNIX哲学にも結びついていると筆者は考えています。これから、ふたつのUNIX哲学をもとに、どのようにGoへ活かしていくかを解説します。

=== マキルロイのUNIX哲学

マルコム・ダグラス・マキルロイ (Malcolm Douglas McIlroy) は、UNIX創始者の一人であり、プロセス間通信を標準入出力を介して接続するパイプの発明者でもあります。マキルロイはUNIX哲学を以下のように形にしています。

//quote{
これがUNIXの哲学である。
一つのことを行い、またそれをうまくやるプログラムを書け。
協調して動くプログラムを書け。
標準入出力（テキスト・ストリーム）を扱うプログラムを書け。標準入出力は普遍的インターフェースなのだ。
//}

"Do One Thing and Do It Well"、「ひとつのことを、うまくやれ」と要約されるこの哲学において、GoでAPIやツールを開発するとき、無駄な責務を与えないように開発をします。特に意識しているのは下記の点です。

1. ひとつの責務に集中させる
2. 他のプログラムに連携可能にする

==== ひとつの責務に集中させる

プログラムをひとつの責務に集中させていれば複雑にならず、直感的にそのプログラムを使用することが可能です。例えば、APIやコマンドラインツールにおいて、クエリやオプション、引数の渡し方によってプログラムの振る舞いを変えるという実装を多々見てきましたが、それを一度許してしまうと、「またひとつ、またひとつ」のようにオプションが増えていき、プログラムが複雑化していきます。

プログラムの「責務」を最初に定義し、そこからはみ出してしまうこと、つまり、振る舞いを無理に変えてしまうことを避けて開発をすることでプログラムの複雑度を増加させないようにします。これによって、美しい、キレイなコードを保つこともできるでしょう。

==== 他のプログラムと連携可能にする

さて、ひとつの責務に集中させることに成功したら、別のプログラムと連携できるI/Fを提供すること

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

== 早すぎる最適化は諸悪の根源である



== Go言語として、常に考え続けること

ひとつのことをうまくやる

