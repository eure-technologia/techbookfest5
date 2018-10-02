
= Go言語でのCLIツール

GoでCLIツールを開発したことがある人は多くいると思います。また、業務でCLIツールが必要になったため、それを目的にGoを書き始めた人も少なくないと思います。また、Goに限らず、他のスクリプト言語でCLIツールを作成した方は本書を手にとっている人なら必ず開発した経験があるはずです。

GoでCLIツールを作成しはじめた人は、まずはパッケージ構成に悩むと思います。しかし、CLI (Command-Line Interface) のI/F (Interface) が適切に設計されたCLIツールは少ないです。

ツールのI/Fこそ、UNIX哲学を意識し、「ひとつのことをうまくやる」を徹底し、他のプログラムと協働するように開発を心がけることで、ひとつ上のツール開発者になれるでしょう。

== CLIツールを開発する

コマンドの最適な開発は、標準パッケージを参考に理解を深めることができます。参考にすべきコマンドは、Goを書いたことがある方なら必ず使用したことがある、@<tt>{go}コマンドの設計を理解することで、Goらしさを表現することができます。しかし、@<tt>{go}コマンドを理解するには、他のGoの機能を知っておいた方がよいため、今回は題材にしません。

今回、作成途中の拙作ではございますが、QR codeを画像で生成可能なエンコード処理、また、画像から文字列を取得するためのデコード処理を実装したQR code codexツール@<fn>{fn_github_kaneshin_qrcode}にて解説をします。
//footnote[fn_github_kaneshin_qrcode][github.com/kaneshin/qrcode: https://github.com/kaneshin/qrcode]

=== パッケージ構成

Goのリポジトリで実行可能ファイルを作成する場合、cmdディレクトリの配下に、実行ファイル名のディレクトリを作成し、そのディレクトリにて実行可能ファイルを作成します。

//emlist[][go]{
qrcode/
├── cmd/
│   ├── qrcode/
│   │   └── main.go
│   └── qrfmt/
│       └── main.go
├── decode.go
├── encode.go
├── format.go
└── README.md
//}

今回のサンプルでは、cmdディレクトリの中に、@<tt>{qrcode}と@<tt>{qrfmt}のディレクトリを作成して実行ファイルを作成します。

また、作成するツールをパッケージとしてAPI提供する場合、適切にディレクトリを作成して公開しておきます。今回の例では、qrcodeディレクトリ配下 (@<tt>{"github.com/kaneshin/qrcode"}) をそのまま使用可能なパッケージとして提供できるように設計しています。

cmd配下で作成するファイルでは、ビジネスロジックとなる実装は極力避けています。今回のcmd/qrcode/main.goの例では、@<tt>{"github.com/kaneshin/qrcode"}にQR codeのCodex処理を実装しています。そのqrcodeパッケージをインポートし、実行可能ファイルでは、Encode/Decode処理を呼び出すだけの責任を負っています。

//emlist[][go]{
var decode = flag.Bool("d", false, "decode data")

func doEncode(dst io.Writer, src []byte) error {
    enc := qrcode.NewEncoder(dst)
    return enc.Encode(src)
}

func doDecode(dst io.Writer, src []byte) error {
    buf := bytes.NewBuffer(src)
    dec := qrcode.NewDecoder(buf)
    return dec.Decode(dst)
}

func do(r io.Reader) error {
    src, err := ioutil.ReadAll(r)
    if err != nil {
        return err
    }

    var buf bytes.Buffer
    if *decode {
        err = doDecode(&buf, src)
    } else {
        err = doEncode(&buf, src)
    }
    if err != nil {
        return err
    }

    os.Stdout.Write(buf.Bytes())
    return nil
}
//}

このように、読み出し先となる@<tt>{io.Reader}を@<tt>{do}関数に渡して呼び出すのがコマンドの責務であり、それ以降のEncode/Decode処理はqrcodeパッケージに責務を委譲させ、そちらで処理を行います。

== CLIツールの実行

今回のサンプルでは、@<tt>{qrcode}と@<tt>{qrfmt}の２種類をコマンドとして作成しています。それぞれのコマンドは、別の責務を委譲されており、適切に連携を行うような設計にしています。

* @<tt>{qrcode}: 標準入力、もしくは、ファイルのテキストをEncode/Decode処理し、標準出力にPNG画像ファイルを出力する
* @<tt>{qrfmt}: QR codeのフォーマットに従った文字列を標準出力に出力する

このように設計することにより、下記のように、それぞれが連携した独立したツールと成り立っています。

//emlist[][shell]{
qrfmt -ssid Foo -password Bar -type WPA2 | qrcode > WiFi-Foo.png
//}

まさに、UNIX哲学の章にて記載した「ひとつのことをうまくやる」を実現しているI/Fです。

=== 標準入力とファイル名

CLIツールにて、標準入力からテキストを入力されることにより、プロセス間でのパイプ処理が可能なツールを作成することができます。ここの設計次第で、使われるか使われないかのツールになるでしょう。また、場合によって、ファイルからテキストを入力することもあります。それら２つのケースに対応できるI/Fをコードで実現します。

//emlist[][go]{
func run() error {
    var name string
    if args := flag.Args(); len(args) > 0 {
        name = args[0]
    }

    var r io.Reader
    switch name {
    case "", "-":
        // ケース：標準入力
        // $ echo 'input' | command
        //  or
        // $ command -
        r = os.Stdin
    default:
        // ケース：ファイル
        // $ command FILENAME
        f, err := os.Open(name)
        if err != nil {
            panic(err)
        }
        defer f.Close()
        r = f
    }

    // ...
}
//}

標準入力での受け渡しは、@<tt>{os.ModeNamedPipe}で検証できますが、使用する必要はありません。あまりオススメしないパターンですが、コマンドの引数にファイルではない、実行用のテキストを渡しているツールもあるので、その場合は必要になるケースがあります。

//emlist[][go]{
func main() {
    fi, err := os.Stdin.Stat()
    if err != nil {
        panic(err)
    }

    var r io.Reader
    if fi.Mode()&os.ModeNamedPipe == os.ModeNamedPipe {
        r = os.Stdin
    } else {
        args := flag.Args()
        if len(args) == 0 {
            r = os.Stdin
        } else {
            f, err := os.Open(args[0])
            if err != nil {
                panic(err)
            }
            defer f.Close()
            r = f
        }
    }
}
//}

このように@<tt>{os.Stdin.Stat.Mode}から判別することも可能です。

Goの開発思想からすれば、Goを用いてツール開発することは重要なモチベーションでもあるため、これからもツール作成者は増えてくるでしょう。読者の方は、ぜひ、標準パッケージのコマンドの作られ方と、UNIX哲学を踏まえたI/Fの設計を意識して開発をしてください。


