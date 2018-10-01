
= 標準パッケージから学ぶGo言語

Go界隈では、「標準パッケージを参考にすべき」と語る人は、筆者も含めて少なくないと思います。GitHubにも多くのライブラリが公開されていますが、特定のパッケージを参考にした方がよいというのはほとんど聞いた試しがありません。ときどき、そのような話し耳にした経験はありますが、そこまで流行っていないライブラリであることが多いと感じます。

筆者は、標準パッケージでなければGoogleが開発をしたgo-githubやgo-cloudのような設計を参考にするなど、なるべく、「Go言語らしさ」を理解している人がいるコミュニティのパッケージだけを参考にするようにしています。

筆者がツールを作成するときに活用する場合、コードレビューをした上で、Goらしからぬ実装で使いにくいAPIが提供されていれば、ソースコードのライセンスを踏まえた上で、ソースコードをそのままプロジェクトに導入することも稀にあります。

使い勝手が悪いパッケージの場合、コード上で

== os package



golang の定数宣言内での iota 識別子
<script async class="speakerdeck-embed" data-slide="12" data-id="de8b739c3ccd4c9db202641cb33f3ad2" data-ratio="1.77777777777778" src="//speakerdeck.com/assets/embed.js"></script>



前に登壇した内容に iota について書いていたので、少しまとめておきました。


//emlist{
XXX: BLOCK_HTML: YOU SHOULD REWRITE IT
<!-- more -->
//}

== iota とは


iota とは、ギリシャ文字の9番目の『 Ι , ι 』にあたり、発音は ʌɪˈəʊtə （ｧｲｵｩﾀ）となります。
日本の多くの人はイオタと発音する人が多い（気がする）ので、自分の場合はイオタと発音しています。大学で数学を勉強していたときも ι はイオタと発音していたので、イオタの方が慣れてしまっており…


== golang における iota 識別子


const の宣言グループ内で、事前に宣言されている iota 識別子によって、 @<strong>{型なしの連続する整数定数} を表します。const グループ毎に iota の値は 0 から開始されるので、関連する定数のセットを作成するときに非常に便利です。


//emlist[][go]{
const (
    foo1 = iota // foo1 == 0
    bar1 = iota // bar1 == 1
    baz1 = iota // baz1 == 2
)

// iota は 0 から開始
// iota を省略しても評価されて連続する整数が付与される
const (
    foo2 = iota // foo2 == 0
    bar2        // bar2 == 1
    baz2        // baz2 == 2
)
//}

=== iota カウントアップ


iota 識別子のカウントアップは ConstSpec 毎に行われるので、識別子と式のそれぞれのリスト（IdentifierList, ExpressionList）が同一の ConstSpec に存在するときは同一の整数値となります。


//emlist[][go]{
const (
    foo1, foo2 = iota, 2 * iota // foo1 == 0, foo2 == 0 (iota == 0)
    bar1, bar2                  // bar1 == 1, bar2 == 2 (iota == 1)
    baz1, baz2                  // baz1 == 2, baz2 == 4 (iota == 2)
)
//}

=== iota とブランク識別子


@<tt>{_} （アンダースコア）を使用して表現するブランク識別子は、 const 内でも識別されますが、宣言において IdentiferList, ExpressionList へのバインドは行われません。
ConstSpec へはアサインされるので、ConstSpec 毎のカウントアップは実行されます。


//emlist[][go]{
const (
    foo1, foo2 = iota, 2 * iota // foo1 == 0, foo2 == 0 (iota == 0)
    _, _                        //                      (iota == 1)
    baz1, baz2                  // baz1 == 2, baz2 == 4 (iota == 2)
)
//}

==== ConstSpec, IdentifierList, ExpressionList?


定数に限らないですが、コンパイラが事前宣言における解釈をするためのリストになります。コンパイラの方で type や const の宣言を判断し、対応する TypeSpec や ConstSpec に適切にアサインされていきます。


//emlist[][go]{
ConstDecl      = "const" ( ConstSpec | "(" { ConstSpec ";" } ")" ) .
ConstSpec      = IdentifierList [ [ Type ] "=" ExpressionList ] .

IdentifierList = identifier { "," identifier } .
ExpressionList = Expression { "," Expression } .
//}


IdentifierList, ExpressionList はそれぞれ『識別子』と『式』のリストになっており、カンマ区切りのものをバインドします。



参考： [https://golang.org/ref/spec#Constant_declarations:title]


== iota の活用例


[https://github.com/golang/go/wiki/Iota:title] にある例ですが、とても良い例です。


//emlist[][go]{
type ByteSize float64

const (
    _           = iota // ignore first value by assigning to blank identifier
    KB ByteSize = 1 << (10 * iota)
    MB
    GB
    TB
    PB
    EB
    ZB
    YB
)
//}


最初の値はブランク識別子をつかってバインドしないようにしていますが、もし仮にブランク識別子を使用しないで実装すると


//emlist[][go]{
const (
    KB ByteSize = 1 << (10 * (iota + 1)) // 1 << 10
    MB                                   // 1 << 20
    GB                                   // 1 << 30
    // ...
)
//}


のようにする必要があります。常に iota に +1 しなければならなく、間違えて @<tt>{1 << (10 * iota + 1)} にしてしまったら期待する動作にはなりません。


//emlist[][go]{
// 間違い例
const (
    KB ByteSize = 1 << (10 * iota + 1) // 1 << 11
    MB                                 // 1 << 12
    GB                                 // 1 << 13
    // ...
)
//}


意外と、こういう所で不具合を出してしまったりしまうので、ブランク識別子を有効活用するのも覚えていて損はないです。


== おわりに


iota の事書いていたのですが、ConstSpec とかの話をもっと掘り下げたくなりました。ここいらの話を知っておくほうがより一層、解釈のされ方が理解できると思います。



== io package



TBD



== sort package


= sortパッケージから学ぶ汎用的なパッケージ作成


Goでソートを行うときに@<href>{https://golang.org/pkg/sort/,sortパッケージ}を活用すると思います。Goを使っている人はこのsortパッケージの実装を熟知してから、自分で新規パッケージを作成すると、汎用的に利用できるパッケージを作ることができるはずです。


//emlist{
XXX: BLOCK_HTML: YOU SHOULD REWRITE IT
<!-- more -->
//}

== モチベーション


コーディングインタビューの話ではないが自分が人のコードを読むとしたら「どういうレイヤー／パッケージ構成」にしているか、というのは設計力を測る一つの観点にしています。


//emlist{
XXX: BLOCK_HTML: YOU SHOULD REWRITE IT
<blockquote class="twitter-tweet" data-lang="en"><p lang="ja" dir="ltr">coding interviewは好きじゃないけど、やるならこういうの出してみるかな〜ってのを仮で作ってみた。 <a href="https://t.co/gP7MkBCkd0">https://t.co/gP7MkBCkd0</a> <br>純粋に言語の習熟度とアルゴリズムの実装経験を測れる具合にしている。</p>&mdash; kaneshin (@kaneshin0120) <a href="https://twitter.com/kaneshin0120/status/1040860420766982145?ref_src=twsrc%5Etfw">September 15, 2018</a></blockquote>
//}

//emlist{
XXX: BLOCK_HTML: YOU SHOULD REWRITE IT
<script async src="https://platform.twitter.com/widgets.js" charset="utf-8"></script>

//}

== shuffle package


sortパッケージを模倣するのに作成するのは要素のシャッフルを行うshuffleパッケージというのを作成して解説していこうと思います。


=== interfaceの提供


早速、サンプルとして下記のようなinterfaceをパッケージから提供し、Shuffle関数を用意することによってGoらしさが際立つと思います。


//emlist[][go]{
package shuffle

import (
    "math/rand"
    "time"
)

type Interface interface {
    Seed() int64
    Len() int
    Swap(int, int)
}

func Shuffle(data Interface) {
    rand.Seed(data.Seed())
    n := data.Len()
    for i := n - 1; i >= 0; i-- {
        j := rand.Intn(i + 1)
        data.Swap(i, j)
    }
}

// ... implemented more
//}


[https://github.com/kaneshin/playground/blob/master/shuffle/shuffle.go#L8-L12:embed:cite]



※ sortパッケージにある @<tt>{sort#Slice} のような関数は今回の話には含みません。


=== shuffle#Interface


一番重要なところですが、何かしらのアルゴリズムの情報操作を容易にするパッケージを作成したい場合、特に数値解析をしているときは下記のような型を用意しておくことによって、外部からでも柔軟に型が違ってもパッケージに見合う型でアルゴリズムを適用出来ることが可能です。


//emlist[][go]{
type Interface interface {
    Seed() int64
    Len() int
    Swap(int, int)
}
//}
//table[tbl1][]{
Function	Description
-----------------
@<tt>{Seed() int64}	シャッフルするときのシードを設定する
@<tt>{Len() int}	シャッフルするコレクションの要素数
@<tt>{Swap() (int, int)}	シャッフルする要素の入れ替え
//}


このようなinterfaceを提供しておけば、import元のパッケージでは下記のように使用することができます。


//emlist[][go]{
package main

import (
    "fmt"
    "os"

    "github.com/kaneshin/playground/shuffle"
)

type Dice []int

func (d Dice) Seed() int64   { return int64(os.Getpid()) }
func (d Dice) Len() int      { return len(d) }
func (d Dice) Swap(i, j int) { d[i], d[j] = d[j], d[i] }

var dice = Dice([]int{1, 2, 3, 4, 5, 6})

func main() {
    fmt.Printf("%v\n", dice)
    shuffle.Shuffle(dice)
    fmt.Printf("%v\n", dice)
}
//}


このような実装方法を理解しておけば、上記のような状況におけるジェネリクスは特段必要が無いということもわかりますね。



[https://github.com/kaneshin/playground/blob/master/shuffle/example/main.go:title]


=== shuffle#Shuffle


実際にアルゴリズムを適用するところですが、今回のサンプルはひとつのアルゴリズムに留まっていますが、ここで前処理や与えられている型からの情報からアルゴリズムの適用を変更することも可能です。


//emlist[][go]{
func Shuffle(data Interface) {
    rand.Seed(data.Seed())
    n := data.Len()
    for i := n - 1; i >= 0; i-- {
        j := rand.Intn(i + 1)
        data.Swap(i, j)
    }
}
//}


特に、sortパッケージを読み込むと、コレクションの長さによってアルゴリズムを変更したりしています。


//emlist[][go]{
func quickSort(data Interface, a, b, maxDepth int) {
    for b-a > 12 { // Use ShellSort for slices <= 12 elements
        if maxDepth == 0 {
            heapSort(data, a, b)
            return
        }
        maxDepth--
        mlo, mhi := doPivot(data, a, b)
        // Avoiding recursion on the larger subproblem guarantees
        // a stack depth of at most lg(b-a).
        if mlo-a < b-mhi {
            quickSort(data, a, mlo, maxDepth)
            a = mhi // i.e., quickSort(data, mhi, b)
        } else {
            quickSort(data, mhi, b, maxDepth)
            b = mlo // i.e., quickSort(data, a, mlo)
        }
    }
    if b-a > 1 {
        // Do ShellSort pass with gap 6
        // It could be written in this simplified form cause b-a <= 12
        for i := a + 6; i < b; i++ {
            if data.Less(i, i-6) {
                data.Swap(i, i-6)
            }
        }
        insertionSort(data, a, b)
    }
}

// Sort sorts data.
// It makes one call to data.Len to determine n, and O(n*log(n)) calls to
// data.Less and data.Swap. The sort is not guaranteed to be stable.
func Sort(data Interface) {
    n := data.Len()
    quickSort(data, 0, n, maxDepth(n))
}
//}


[https://golang.org/src/sort/sort.go:title]


== sortパッケージに関して


先に、汎用的なパッケージ作成を解説しましたが、題材にあげているsortパッケージも解説します。


=== サンプルコード


Goの公式ページからの引用ですが、サンプルコードから解説したいと思います。


//emlist[][go]{
package main

import (
    "fmt"
    "sort"
)

type Person struct {
    Name string
    Age  int
}

func (p Person) String() string {
    return fmt.Sprintf("%s: %d", p.Name, p.Age)
}

type ByAge []Person

func (a ByAge) Len() int           { return len(a) }
func (a ByAge) Swap(i, j int)      { a[i], a[j] = a[j], a[i] }
func (a ByAge) Less(i, j int) bool { return a[i].Age < a[j].Age }

func main() {
    people := []Person{
        {"Bob", 31},
        {"John", 42},
        {"Michael", 17},
        {"Jenny", 26},
    }

    fmt.Println(people)

    // ascending
    sort.Sort(ByAge(people))
    fmt.Println(people)

    // descending
    sort.Slice(people, func(i, j int) bool {
        return people[i].Age > people[j].Age
    })
    fmt.Println(people)

}
//}


[https://play.golang.org/p/LZgZi-s8IX-]



非常にシンプルな形ですが、これだけで@<tt>{Person.Age}によってソート処理を行うことができています。


=== sort#Interface


さて、どのようなinterfaceが定義されているのかですが、先ほど紹介したのと似たように@<tt>{Interface}という型が定義されており、それを満たした関数を定義したに過ぎません。



[https://golang.org/pkg/sort/#Interface]


//emlist[][go]{
// A type, typically a collection, that satisfies sort.Interface can be
// sorted by the routines in this package. The methods require that the
// elements of the collection be enumerated by an integer index.
type Interface interface {
    // Len is the number of elements in the collection.
    Len() int
    // Less reports whether the element with
    // index i should sort before the element with index j.
    Less(i, j int) bool
    // Swap swaps the elements with indexes i and j.
    Swap(i, j int)
}
//}
//table[tbl2][]{
Function	Description
-----------------
@<tt>{Len() int}	ソートするコレクションの要素数
@<tt>{Less(int, int) bool}	ソートにおける要素の比較
@<tt>{Swap() (int, int)}	ソートにおける要素の入れ替え
//}

=== sort#Slice


上の方でも解説しましたが、場合によってはソート処理を行う前処理を行ったりしています。パッケージ内のI/Fを変更せずに内部アルゴリズムを変更するのは当たり前なんですが、とてもキレイな実装でもあるので、自身でコードを書くときも意識できると良いですね。



[https://golang.org/pkg/sort/#Slice]


