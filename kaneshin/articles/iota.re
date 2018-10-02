

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



