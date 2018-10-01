
= CLIツールを開発する

Goでコマンドラインツールを開発したことがある人は多くいると思います。




== まずは開発



== CLIのテストを記述



== 適切なSTDIN/OUT

// 記事

Go言語でコマンドラインツールを作るときに入力を受け取るインターフェースでオプションや標準入力で受け付けることはあると思いますが、パイプで渡すことも考慮されているとクールなツールになるなと思っています。


=== 標準入力の受け取り


それぞれの実装方法は簡単です。


==== インタラクティブ

//emlist[][go]{
var stdin string
fmt.Scan(&stdin)
fmt.Println(stdin)
//}


インタラクティブに標準入力からデータを受け取るには @<tt>{fmt.Scan} で入力待ちをします。このとき入力した値が渡した変数に格納されます。


==== パイプ

//emlist[][go]{
body, err := ioutil.ReadAll(os.Stdin)
fmt.Println(string(body))
//}


パイプで渡ってきたものは @<tt>{os.Stdin} というファイルディスクリプタにデータが入っているので、ここから取得します。


=== インタラクティブかパイプを判定する


パイプでファイルディスクリプタが渡ってきた場合はそのままそのファイルディスクリプタからデータを取得すれば良いので、インタラクティブな入力待ちは必要ありません。
そんなときは @<tt>{syscall} パッケージを利用します。


==== syscallパッケージ

//emlist[][go]{
const ioctlReadTermios = 0x5401  // syscall.TCGETS

// IsTerminal returns true if the given file descriptor is a terminal.
func IsTerminal() bool {
    fd := syscall.Stdin
    var termios syscall.Termios
    _, _, err := syscall.Syscall6(syscall.SYS_IOCTL, uintptr(fd), ioctlReadTermios, uintptr(unsafe.Pointer(&termios)), 0, 0, 0)
    return err == 0
}
//}


上記の @<tt>{IsTerminal} 関数が @<tt>{true} で返ってくる場合、インタラクティブにデータを受け取る場合になります。処理としては、@<tt>{/dev/stdin} のファイルディスクリプタ値である @<tt>{syscall.Stdin} が読み込みをしているかを判定しています。



ただ、この実装は Linux に依存しており、他のPlatformで利用するには @<tt>{ioctlReadTermios} の値を適宜変更しなければいけません。
これを自分で実装するのはちょい面倒なので、既に実装がされているものを利用します。


==== terminalパッケージ


golang.orgに @<tt>{golang.org/x/crypto/ssh/terminal} というパッケージが存在していて、ここにある@<href>{https://github.com/golang/crypto/blob/master/ssh/terminal/util.go#L30-L35,IsTerminal}という関数が先ほどの各Platformの要件を満たしています。


//emlist[][shell]{
go get -u golang.org/x/crypto/ssh/terminal
//}

//emlist[][go]{
import "golang.org/x/crypto/ssh/terminal"

func main() {
    // ...
    if terminal.IsTerminal(syscall.Stdin) {
        // Do something ...
    }
    // ...
}
//}

=== 全体の実装

//emlist[][go]{
package main

import (
    "fmt"
    "io/ioutil"
    "os"
    "syscall"

    "golang.org/x/crypto/ssh/terminal"
)

func main() {
    if terminal.IsTerminal(syscall.Stdin) {
        // Execute: go run main.go
        fmt.Print("Type something then press the enter key: ")
        var stdin string
        fmt.Scan(&stdin)
        fmt.Printf("Result: %s\n", stdin)
        return
    }

    // Execute: echo "foo" | go run main.go
    body, err := ioutil.ReadAll(os.Stdin)
    if err != nil {
        panic(err)
    }
    fmt.Printf("Result: %s\n", string(body))
}
//}

// ここまで記事

== ひとつのことをうまくやる

