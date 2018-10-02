
= 標準パッケージから学ぶGo言語

Go界隈では、「標準パッケージを参考にすべき」と語る人は、筆者も含めて少なくないと思います。Goも昔と比べれば、GitHubにも多くのライブラリが公開されています。しかし、GitHubにホスティングされている、「このリポジトリのパッケージ構成を参考にした方がよい」、というのはほとんど聞いた試しがありません。

筆者は、標準パッケージでなければGoogleが開発をしたgo-github@<fn>{fn_google_go_github}やgo-cloud@<fn>{fn_google_go_cloud}のような設計を参考にするなど、なるべく、「Go言語らしさ」を理解している人がいるコミュニティのパッケージを参考にするようにしています。
//footnote[fn_google_go_github][github.com/google/go-github: https://github.com/google/go-github]
//footnote[fn_google_go_cloud][github.com/google/go-cloud: https://github.com/google/go-cloud]

また、筆者が、CLIツールやパッケージを開発する場合、使用するライブラリのAPIをレビューをした上で、Goらしくない実装でなくAPIが提供されていれば、ソースコードのライセンスを踏まえた上で、ソースコードをそのままプロジェクトに含めてしまい、改修してしまうこともあります。Goではベンダリングの機能も拡充していきていますが、開発以外の、依存パッケージのバージョン管理や後方互換性の検証という、問題解決をすることとは違うことに時間を掛けてしまうのは、Goが開発された意図に反していると考えます。

== sortパッケージから学ぶ汎用例

Goでソート処理を実行する場合、sortパッケージの@<tt>{sort.Interface}を満たし、@<tt>{sort.Sort}を実行するか、@<tt>{sort.Slice}関数を実行してソートしているでしょう。このsortパッケージの実装を十分に理解することによって、振る舞いを内包した汎用的なパッケージを作成することができます。

sortパッケージについて、Goの公式ページからの引用ですが、サンプルコードから解説します。

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

シンプルに記述されていますが、これを実行することによって、@<tt>{people}のスライスを@<tt>{Person.Age}によって昇順と降順のソートを行っています。昇順では@<tt>{sort.Interface}を満たし、降順では@<tt>{sort.Slice}をそれぞれ利用して、ソート処理を実現しています。

=== sort.Interface、sort.Sort

@<tt>{sort.Interface}は、@<tt>{interface}に３つの関数を定義しており、それらの関数を定義した型を提供するだけで、@<tt>{sort.Sort}を実行することができます。

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

@<tt>{Len() int}、@<tt>{Less(int, int) bool}、@<tt>{Swap() (int, int)}の３つは、ソート処理を行うときに、「これらの情報さえ知っておけば、ソート処理は実現可能」ということを示しています。

 * @<tt>{Len() int}: ソートするコレクションの要素数
 * @<tt>{Less(int, int) bool}: ソートにおける要素の比較
 * @<tt>{Swap() (int, int)}: ソートにおける要素の入れ替え

これらを満たし、@<tt>{sort.Sort}の引数に変数を渡して呼び出すことにより、ソート処理を実行します。

//emlist[][go]{
type Person struct {
    Name string
    Age  int
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

    sort.Sort(ByAge(people))
    fmt.Println(people)
}
//}

簡略化しましたが、この例では、@<tt>{Person.Age}をもとにソート可能にするため、@<tt>{sort.Interface}を満たす@<tt>{ByAge}を定義しています。このように、@<tt>{ByAge}のように別で型を提供し、@<tt>{sort.Sort(ByAge(people))}のように実行することで、ソート処理の責務のみとなり、シンプルさが際立ちます。

=== sort.Slice

@<tt>{func Slice(slice interface\{\}, less func(i, j int) bool)}のシグネチャで用意されている@<tt>{sort.Slice}関数ですが、ソート対象のスライスと、ソートをする際の要素比較を関数として渡すことにより、@<tt>{sort.Interface}を満たす型を提供せずとも、ソート処理が実現可能です。
//emlist[][go]{
type Person struct {
    Name string
    Age  int
}

func main() {
    people := []Person{
        {"Bob", 31},
        {"John", 42},
        {"Michael", 17},
        {"Jenny", 26},
    }

    sort.Slice(people, func(i, j int) bool {
        return people[i].Age > people[j].Age
    })
    fmt.Println(people)
}
//}

@<tt>{sort.Interface}と比べれば、コード量が削減されていますし、@<tt>{ByAge}のような型も定義せずに、ソート処理が可能です。こちらの方が実用性が高いかもしれませんが、時には、ソート処理を満たした型を明示した方が可読性があがることももちろんあります。可読性、保守性の観点で、必要に応じて使い分けることが重要です。


=== sort.Sortの内部処理

パッケージのAPIとなるI/Fの設計ですが、@<tt>{sort.Sort}では@<tt>{sort.Interface}を引数に渡して呼び出すI/Fとなっています。しかし、処理の内部では、スライスの長さによって、アルゴリズムを変更するなど、振る舞いを変更しています。このような設計であれば、場合によって、処理実行における、前処理や後処理を行うことができます。このように、パッケージが提供しているAPIのI/Fを変更せず、内部アルゴリズムを変更することによって、使い手側は内部を知らずとも、最適なアルゴリズムで処理してくれることを担保できます。

//emlist[][go]{
func quickSort(data Interface, a, b, maxDepth int) {
    for b-a > 12 { // Use ShellSort for slices <= 12 elements
        if maxDepth == 0 {
            heapSort(data, a, b)
            return
        }
        maxDepth--
        mlo, mhi := doPivot(data, a, b)
        if mlo-a < b-mhi {
            quickSort(data, a, mlo, maxDepth)
            a = mhi // i.e., quickSort(data, mhi, b)
        } else {
            quickSort(data, mhi, b, maxDepth)
            b = mlo // i.e., quickSort(data, a, mlo)
        }
    }
    if b-a > 1 {
        for i := a + 6; i < b; i++ {
            if data.Less(i, i-6) {
                data.Swap(i, i-6)
            }
        }
        insertionSort(data, a, b)
    }
}

func Sort(data Interface) {
    n := data.Len()
    quickSort(data, 0, n, maxDepth(n))
}
//}

sortパッケージのsort.goでは、@<tt>{quickSort}の処理をスライスの長さによって@<tt>{heapSort}か@<tt>{insertionSort}、また、@<tt>{quickSort}を再帰的に呼び出す、ということをしており、実に興味深い実装になっています。

== sortパッケージを模倣する

sortパッケージを模倣し、スライスの要素をシャッフルするshuffleパッケージというのを作成します。やり方は、sortパッケージと似たように、@<tt>{shuffle.Interface}と@<tt>{shuffle.Shuffle}という型と関数を定義します。

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
//}

@<tt>{shuffle.Interface}は、シャッフルするのに必要な情報だけを取得できるよ、最低限の３つの関数を定義しています。

 * @<tt>{Seed() int64}: シャッフルするときのシードを設定する
 * @<tt>{Len() int}: シャッフルするコレクションの要素数
 * @<tt>{Swap() (int, int)}: シャッフルする要素の入れ替え

このような@<tt>{interface}を提供することで、使用先では下記のように使用することができます。@<fn>{fn_playground_shuffle}
//footnote[fn_playground_shuffle][サンプルコード: https://github.com/kaneshin/playground/blob/master/shuffle/example/main.go]

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
