
= Go言語でのテストとベンチマーク

Goは標準でテストコードとベンチマークで計測するためのtestingパッケージが用意されています。しかし、そのパッケージを理解した上で、良いテストを書くことは簡単なことではありません。本章ではテストコードやベンチマークの一般的な書き方をサンプルコードとともに解説していきます。

== テーブル駆動型テスト - Table-Driven Testing

Goでは、標準パッケージも含めて、多くの状況でテーブル駆動型によるテストで記述されていることがあります。テーブルと呼ばれる、テストに必要な入力と期待する結果を含んだテストケースを準備し、テスト実行の出力にフォーカスした重複コードの無いテストを記述することができます。

テーブル駆動型テストにすることによって、テストコードの可読性や保守性を向上させることができます。Goの標準パッケージに点在するテストコードも参照するとほとんどがテストケースとしてのテーブルを準備しており、それに対してテストを繰り返し実行することで、コードの検証を行っています。

//emlist[][go]{
package main

import (
    "strings"
    "testing"
)

func Test_ToUpper(t *testing.T) {
    tests := []struct {
        input, expected string
    }{
        {"foo", "FOO"}, {"bar", "BAR"},
    }

    for _, tt := range tests {
        result := strings.ToUpper(tt.input)
        if tt.expected != result {
            t.Errorf("expected %v, but %v", tt.expected, result)
        }
    }
}
//}

テーブルは、入力と期待する結果を構造体の配列として準備し、そのテーブルのテストアイテムについて繰り返しテストを実行します。このテーブル駆動型テストは、次に解説するサブテストと組み合わせることによって、効率的なテストを記述することができます。

== サブテストによるテストの並行処理

Go1.7からサブテストというテストの機能が追加になりました。このサブテストは、テストをテーブル駆動形テストとは別の方法で効率的かつ簡潔に記述することができます。テストコードを記述するときに問題になる、テストコードの煩雑化を軽減することができます。また、サブテストを並行で実行することもサポートしているため、テスト実行についても時間に対して効率的に実行することができます。

//emlist[][go]{
func Test_ToUpper(t *testing.T) {
    tests := []struct {
        input, expected string
    }{
        {"foo", "FOO"}, {"bar", "BAR"},
    }

    for _, tt := range tests {
        tt := tt
        // サブテストでの実行
        t.Run(tt.input, func(t *testing.T) {
            result := strings.ToUpper(tt.input)
            if tt.expected != result {
                t.Errorf("expected %v, but %v", tt.expected, result)
            }
        })
    }
}
//}

テーブルとサブテストを掛け合わせることにより、ここまで簡潔にテストを記述することができますし、サブテストによって、複数のテストケースの集合を扱えるようになるため、テストスイートという考えてテストを記述することが可能です。

サンプルコード中に出てくる「@<tt>{tt := tt}」はループ内で並行処理を行う際に、変数をキャプチャしておかないと変数への代入で意図しない結果となってしまうために、ループ内で改めて変数を定義しています。

=== @<tt>{setup}と@<tt>{teardown}

サブテストは、テスト実行されている関数内で、階層的にテストを記述することが可能でした。その階層化されたテストで@<tt>{setup}や@<tt>{teardown}と呼ばれる、テスト時の前処理と後処理を行うことも可能です。

//emlist[][go]{
func Test_Foo(t *testing.T) {
    // setup code
    t.Run("Sub", func(t *testing.T) {
        // setup code
        t.Run("Sub/1", func(t *testing.T) {
            // ...
        })
        t.Run("Sub/2", func(t *testing.T) {
            // ...
        })
        // teardown code
    })
    // teardown code
}
//}

この前処理と後処理をシリアルで適切に実行可能なことにより、例えば、永続データの後始末をした上で、再テストを実行することが可能になります。

ちょっとしたTipsになりますが、テストコードの前処理と後処理はほとんどの場合はセットで定義されるため、まとめてひとつの関数に記述しておくことによって、処理のセットとして、コードの理解へとつながります。

//emlist[][go]{
func setupWithTeardown() func() {
    // setup code
    return func() {
        // teardown code
    }
}

// ケース1
func Test_Foo(t *testing.T) {
    // do setup
    teardown := setupWithTeardown()
    t.Run("Foo", func(t *testing.T) {
        // ...
    })
    // do teardown
    teardown()
}

// ケース2
func Test_Bar(t *testing.T) {
    // do setup and then stacking teardown func
    defer setupWithTeardown()()
    t.Run("Bar", func(t *testing.T) {
        // ...
    })
}
//}

ケース1のパターンは、丁寧に処理の前後に関数を呼び出しています。ケース2のパターンは、@<tt>{setup}を実行し、その後、コールスタックに@<tt>{teardown}関数をスタックさせています。

=== サブテストの並行処理

サブテストを実行している@<tt>{Run}関数内で、@<tt>{Parallel}関数をコールすることにより、テストを並行で処理させることができます。そして、すべてのサブテストの処理が終了し次第、親のテストを終了することができます。これは、@<tt>{teardown}のコードがすべてのサブテストが終了してから実行されることが保証されることを意味しています。

//emlist[][go]{
func Test_ToUpper(t *testing.T) {
    tests := []struct {
        input, expected string
    }{
        {"foo", "FOO"}, {"bar", "BAR"},
    }

    for _, tt := range tests {
        tt := tt
        // サブテストでの実行
        t.Run(tt.input, func(t *testing.T) {
            t.Parallel() // 並行処理実行
            result := strings.ToUpper(tt.input)
            if tt.expected != result {
                t.Errorf("expected %v, but %v", tt.expected, result)
            }
        })
    }
}
//}

上記のサンプルを実行すると、全てのサブテストが並行で処理されます。@<tt>{teardown}を実行させる場合、下記のようにサブテストで囲む必要があります。

//emlist[][go]{
func setupWithTeardown() func() {
    // setup code
    return func() {
        // teardown code
    }
}

func Test_ToUpper(t *testing.T) {
    tests := []struct {
        input, expected string
    }{
        {"foo", "FOO"}, {"bar", "BAR"},
    }

    // setup
    teardown := setupWithTeardown()
    // teardown 効かせるために Run で囲む
    t.Run("parent group", func(t *testing.T) {
        for _, tt := range tests {
            tt := tt
            // サブテストでの実行
            t.Run(tt.input, func(t *testing.T) {
                t.Parallel() // 並行処理実行
                result := strings.ToUpper(tt.input)
                if tt.expected != result {
                    t.Errorf("expected %v, but %v", tt.expected, result)
                }
            })
        }
    })
    // teardown
    teardown()
}
//}


ここまでがサブテストと並行処理の話でした。サブテストと同じような記述方法でサブベンチマークも記述することができます。

サブテストを使用することにより、何かが大きく変わるわけではないです。ただ、テストコードの可読性と保守性が高ければ高いほど、メンテナンスが容易なテストコードになるため、メンテナンスのしやすいテストコードとするためにも積極的に使用することをおすすめします。

== ベンチマークでのパフォーマンス測定

Goのtestingパッケージでは、Goコードをベンチマークするための仕組みも標準パッケージが提供しています。ベンチマークでパフォーマンスを測定するための実装はテストコードと同じくらい容易ですし、実行も@<tt>{go test}コマンドで使用して行います。

さて、ベンチマークでパフォーマンス計測をするための題材に、簡単な数値解析のニュートン法@<fn>{fn_newton_method}にて、平方根を求める関数を対象としてパフォーマンス測定をします。
//footnote[fn_newton_method][Tour of Goを最後までやり通した方ならご存知のはずです。]

//emlist[][go]{
func Sqrt(x float64) float64 {
    z := 1.0
    for i := 0; i < 10; i++ {
        z = z - (z*z-x)/(2*z)
    }
    return z
}
//}

このニュートン法の数値計算によって求められる@<tt>{Sqrt}関数と、mathパッケージが容易している@<tt>{math.Sqrt}関数の性能を比較します。

さて、実際にベンチマークのコードを書きますが、テストコードと基本の書き方は変わりません。テストコードの場合、テストコードと認識するために、@<tt>{TestXXX(t *testing.T)}のような関数名と引数である必要がありますが、ベンチマークでは@<tt>{BenchmarkXXX(b *testing.B)}のような関数名と引数が必要になります。

さて、今回のベンチマークは「2の平方根を求める」ベンチマークを行います。

//emlist[][go]{
// ユーザ定義のSqrt関数
func Benchmark_Sqrt(b *testing.B) {
    for n := 0; n < b.N; n++ {
        Sqrt(2)
    }
}

// mathパッケージのSqrt関数
func Benchmark_MathSqrt(b *testing.B) {
    for n := 0; n < b.N; n++ {
        math.Sqrt(2)
    }
}
//}

コード中で急に出てくる@<tt>{b.N}ですが、これは対象となるベンチマークが信頼に足る回数分のループをベンチマークの方で判断して測定を行ってくれます。

このベンチマークを実行すると、下記のように出力されます。

//emlist[][go]{
PASS
Benchmark_Sqrt           30000000                51.8 ns/op
Benchmark_MathSqrt       2000000000               0.36 ns/op
ok      github.com/eure/go-benchmark/math       2.373s
//}

@<tt>{Benchmark_Sqrt}の結果は、「30,000,000回」のループを実施し、一回のループに平均「51.8ns」の実行時間が掛かっています。他方で、@<tt>{BenchmarkMathSqrt}の結果は、「2,000,000,000回」のループを実施し、一回のループに平均「0.36ns」の実行時間しか掛かっておらず、圧倒的に@<tt>{BenchmarkMathSqrt}の方が実行速度が速いことがわかります。

ループの回数は@<tt>{b.N}が決定しているため、基本的には、1回のループの平均時間でパフォーマンスを比較します。今回だとmathパッケージのSqrt実装の方がかなり高速で処理を完了することができていることが判断できます。

=== ベンチマークのメモリ測定

ベンチマークではメモリの測定も行うことができますが、本書では深く触れません。コマンド実行時に各種オプションが存在していますので、そちらをご確認ください。

@<tt>{-benchmem}オプションでベンチマークを実行すると、メモリアロケーションに関する情報も出力してくれます。速度以外のパフォーマンス・チューニングを行う場合、必ず付けてベンチマークを実行してメモリ状態も併せて確認した方がよいです。

@<tt>{-benchtime}オプションでベンチマークを実行すると、実行する時間をこちらで指定することが可能です。デフォルトの実行時間は1秒なので、10秒に変更したい場合は @<tt>{-benchtime 10s}をコマンド実行時に付与することで実現可能です。
