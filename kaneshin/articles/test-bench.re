
= Go言語でのテストとベンチマーク

= テスト

== テーブル駆動テスト


== Golang におけるサブテストの並行処理実装について
Go1.7 が約1ヶ月前にリリースされましたが、つい先日 Go1.7.1 もリリースされ Golang の盛り上がりを感じますね！
さて、今回は Go1.7 から追加された機能の一つ「サブテスト」について紹介したいと思います。
サブテストとサブベンチマーク
Go1.7 から追加された、テスト（ベンチマーク）を効率的に記述＆実行するための機能で、煩雑になってしまうテストコードを簡潔に記述することができます。
また、サブテストを並行で実行することも可能なため、効率的にテストを実行することも可能です。
テストコード
テーブル駆動テスト - Table-Driven Testing
Go1.7 以前から Golang ではテーブル駆動テストでテストコードを記述することが可読性や保守性の面で良いとされています。
func TestFoo(t *testing.T) {
candidates := []struct {
		input    string
		expected string
	}{
		{"foo", "FOO"},
		{"bar", "BAR"},
	}
for _, c := range candidates {
		result := strings.ToUpper(c.input)
if c.expected != result {
			t.Errorf("expected %v, but %v", c.expected, result)
		}
	}
}
上記の例のように候補となるデータ構造の配列を用意しておき、その候補となるデータをイテレートしてテストをすることを「テーブル駆動テスト」といいます。
このテーブル駆動テストとサブテストを組み合わせることによって効率的にテストを記述することができます。
サブテスト - Subtests
サブテストをテストコードで使用することにより、テストを階層的に記述することができます。
func TestFoo(t *testing.T) {
// setup code
t.Run("A", func(t *testing.T) {
// setup code
t.Run("B", func(t *testing.T) {
			// ...
		})
t.Run("C", func(t *testing.T) {
			// ...
		})
// teardown code
	})
// teardown code
}
階層化されたテストにより setup や teardown が "気持ち" 見やすくなった気がします。
setup, teardown コード
ちょっとした Tips ですが、テストコードの事前処理と事後処理としてそれぞれ setup と teardown の関数を用意することが多いですが、ほとんどのケースで setup と teardown はセットとなっているため、一つの関数で定義しておくと見やすいです。
func setupWithTeardown() func() {
// setup code
return func() {
		// teardown code
	}
}
// ケース1
func TestFoo(t *testing.T) {
// do setup
	teardown := setupWithTeardown()
t.Run("A", func(t *testing.T) {
		// ...
	})
// do teardown
	teardown()
}
// ケース2
func TestBar(t *testing.T) {
// do setup and then stacking teardown func
	defer setupWithTeardown()()

	t.Run("A", func(t *testing.T) {
		// ...
	})
}
ケース1のパターンは丁寧に処理の前後に関数を呼び出していて、ケース2のパターンでは setup を実行してすぐにコールスタックに teardown 関数をスタックさせています。（defer 実行させています）
サブテストの並行処理
サブテストでは t.Run 関数内で t.Parallel をコールすることによりテストを並行で処理させることができ、全てのサブテストの処理が完了し次第、親の（サブ）テストを終了することができます。
func TestFoo(t *testing.T) {
	// ...
for _, c := range candidates {
		c := c // 変数のキャプチャ
t.Run(c.name, func(t *testing.T) {
			t.Parallel() // 並行処理実行
result := strings.ToUpper(c.input)
if c.expected != result {
				t.Errorf("expected %v, but %v", c.expected, result)
			}
		})
	}
}
上記の場合、全てのサブテストが並行で処理されますが、teardown を実行させる場合は下記のように t.Run で囲んでおく必要があります。
func setupWithTeardown() func() {
// setup code
return func() {
		// teardown code
	}
}
// teardown をうまく効かせるために t.Run で囲む
func TestFoo(t *testing.T) {
	// ...
// setup
	teardown := setupWithTeardown()
t.Run("parent group", func(t *testing.T) {
		for _, c := range candidates {
			c := c
t.Run(c.name, func(t *testing.T) {
				t.Parallel() // 並行処理実行
result := strings.ToUpper(c.input)
if c.expected != result {
					t.Errorf("expected %v, but %v", c.expected, result)
				}
			})
		}
	})
teardown()
}
// teardown がうまく効かない例
func TestFooBar(t *testing.T) {
	// ...
// setup
	teardown := setupWithTeardown()
for _, c := range candidates {
		c := c
t.Run(c.name, func(t *testing.T) {
			t.Parallel() // 並行処理実行
result := strings.ToUpper(c.input)
if c.expected != result {
				t.Errorf("expected %v, but %v", c.expected, result)
			}
		})
	}
teardown() // この teardown はテスト実行中に処理されてしまう
}
defer をしても同じなので、並行処理させるサブテストで teardown を実行する場合は気をつける必要があります。
おわりに
以上がサブテストになります。今回はサブテスト中心に紹介しましたが、同じような記述でサブベンチマークにも展開することができます。
サブテストを使用することにより何かが大きく変わるわけではないですが、テストコードの可読性と保守性は高いほどメンテナブルなテストコードになるので、メンテナンスのしやすいテストコードにするためにも積極的に使用していきたいなと感じます。
参考リンク
Package testing - Subtests and Sub-benchmarks

== golang の httptest パッケージを使う

この記事は [Go Advent Calendar 2016 - Qiita](http://qiita.com/advent-calendar/2016/go) の2日目の記事です。

Golang については書きたいことがたくさんあるので、Go Advent Calendar 2016 その４が出てきても良いのではと思っている次第です。（空いていればいつでも書きます）

さて、今回、この記事では Golang で書かれた Web アプリケーションのリクエストのユニットテストについて解説しようと思います。

 [https://github.com/kaneshin/playground/tree/master/go/net/http/httptest:embed:cite]

## 1. Testing HTTP Handler

検証のために、ただ単に "pong" を返却する pingHandler と、URLクエリから値を取得してそのまま返却する echoHandler の2つを定義します。

_ー pingHandler_
```go
// pingHandler returns just "pong" string.
func pingHandler() func(http.ResponseWriter, *http.Request) {
	return func(w http.ResponseWriter, r *http.Request) {
		w.Write([]byte("pong"))
	}
}
```

_ー echoHandler_
```go
// echoHandler returns a 'msg' query parameter string.
func echoHandler() func(http.ResponseWriter, *http.Request) {
	return func(w http.ResponseWriter, r *http.Request) {
		w.Write([]byte(r.URL.Query().Get("msg")))
	}
}
```

※ ヘッダーやステータスコードの設定は省略しています。

### Tests with httptest.Server

本テストコードは [こちら](https://github.com/kaneshin/playground/blob/master/go/net/http/httptest/handler_test.go#L13) です。

実装したHTTPハンドラのテストを実行するために、httptest ([net/http/httptest](https://golang.org/pkg/net/http/httptest)) パッケージを使用してリクエストをサービングします。


_ー pingHandler テスト_
```go
t.Run("pingHandler", func(t *testing.T) {
	t.Parallel()

	s := httptest.NewServer(http.HandlerFunc(pingHandler()))
	defer s.Close()

	res, err := http.Get(s.URL)
	assert.NoError(t, err)

	defer res.Body.Close()
	body, err := ioutil.ReadAll(res.Body)
	assert.NoError(t, err)
	assert.Equal(t, "pong", string(body))
})
```

_ー echoHandler テスト_
```go
t.Run("echoHandler", func(t *testing.T) {
	candidates := []struct {
		query    string
		expected string
	}{
		{"", ""},
		{"foo=bar", ""},
		{"msg=foo", "foo"},
	}
	for _, c := range candidates {
		c := c
		t.Run(c.query, func(t *testing.T) {
			t.Parallel()

			s := httptest.NewServer(http.HandlerFunc(echoHandler()))
			defer s.Close()
			res, err := http.Get(fmt.Sprintf("%v?%v", s.URL, c.query))
			assert.NoError(t, err)

			defer res.Body.Close()
			body, err := ioutil.ReadAll(res.Body)
			assert.NoError(t, err)
			assert.Equal(t, c.expected, string(body))
		})
	}
})
```

テストのアサートには [github.com/stretchr/testify/assert](https://github.com/stretchr/testify/tree/master/assert) を使用しており、テストの記述については go1.7 以上の仕様となりますので、 [Golang におけるサブテストの並行処理実装について | eureka tech blog](https://developers.eure.jp/tech/go1_7-subtests/) を是非一読してみてください。

### httptest.NewServer とは

上の例のように、既にHTTPハンドラが定義されている場合、 httptest.Server を利用してリクエストをサービングします。

```go
httptest.NewServer(http.Handler) *httptest.Server
```

httptest.NewServer を呼んで httptest.Server を初期化します。httptest.Server は httptest.NewServer で初期化を行うと、リクエストをサーブする状態の httptest.Server が返却されるため、使用後に Close を行う必要があります。

また、そもそも初期化時にサービングしている httptest.Server が欲しくない場合は

```go
httptest.NewUnstartedServer(http.Handler) *httptest.Server
```

にて初期化を行います。

### httptest.NewServer の使い方

```go
// http.HandleFuncを満たすハンドラの用意
handler := http.HandleFunc(func(w http.ResponseWriter, r *http.Request) {
	// ...
})

// 定義したハンドラをサーブする Server
s := httptest.NewServer(handler)

// s.URL にリクエストURLが設定されている
res, err := http.Get(s.URL)

// ....

// 適切に Close させて終了
s.Close()
```

httptest.NewServer には http.Handler を満たした変数を渡せば十分です。
http.Handler を渡すこすためには func(w http.ResponseWriter, r *http.Request) のハンドラ関数を http.HandleFunc 型にするだけで満たされます。

```go
handler := http.HandleFunc(func(w http.ResponseWriter, r *http.Request) { ... })
```

## 2. Recording Response while Testing
httptest.Server を使用するのは上記で十分ですが、わざわざ検証を行うために httptest.Server を使って検証を行いたくない場合や、何らかの理由で httptest.Server が使用できないが、リクエストをシミュレートしてのユニットテストを行いたい場合は httptest.ResponseRecorder を使用します。

### httptest.ResponseRecorder

Responseを記録するための構造体で、httptest ([net/http/httptest](https://golang.org/pkg/net/http/httptest)) にこちらも存在しています。

```go
type ResponseRecorder struct {
    Code      int           // the HTTP response code from WriteHeader
    HeaderMap http.Header   // the HTTP response headers
    Body      *bytes.Buffer // if non-nil, the bytes.Buffer to append written data to
    Flushed   bool
    // contains filtered or unexported fields
}
```

http.ResponseWriter を満たしているため、HTTPハンドラに渡すことによって結果を書き込むことができます。

### Tests with httptest.ResponseRecorder

本テストコードは [こちら](https://github.com/kaneshin/playground/blob/master/go/net/http/httptest/handler_test.go#L61) です。

HTTPハンドラに ResponseRecorder を渡して、その値を検証することになります。

```go
// ResponseRecorder の生成
res := httptest.NewRecorder()

// リクエストの生成
req, err := http.NewRequest(http.MethodGet, "http://example.com/?msg=foo", nil)
```

用意したリクエストとレスポンスに対してHTTPハンドルします。

```go
// ハンドラに ResponseRecorder と Request を渡して実行する
handler := echoHandler()
handler(res, req)
```

※ echoHandler は Server で使用したものと同一

```go
// ResponseRecorder に記録された値の検証
assert.Equal(t, "text/plain", res.HeaderMap.Get("Content-Type"))
assert.Equal(t, 200, res.Code)
// Body の検証
body, err := ioutil.ReadAll(res.Body)
assert.NoError(t, err)
assert.Equal(t, c.expected, string(body))
```

検証には testify/assert を使用しています。httptest.ResponseRecorder には Response 検証に必要な情報が全て格納されているので、それを利用して検証を行います。


## おわりに

net/http/httptest パッケージは軽量ですが、テスト時に強力なパッケージになりますので、是非活用してもらえればと思います。

##  リソース

- [https://golang.org/pkg/net/http/httptest/:title]
- [https://golang.org/pkg/testing/#hdr-Subtests_and_Sub_benchmarks:title]
- [https://github.com/stretchr/testify/tree/master/assert:title]
- [https://developers.eure.jp/tech/go1_7-subtests/:title]



== テーブル駆動テストでのテストスイート設計

== サブテストによるテストの並行処理

== ベンチマークの実装


