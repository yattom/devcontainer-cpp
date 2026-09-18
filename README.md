# C++ リファクタリング練習用 DevContainer

リファクタリング練習のための、意図的に整理されていない C++ コードベース。
DevContainer でビルド・実行・テストがすぐにできる。

同じ題材の Java 版がある（`devcontainer-java`）。設計とコードはそちらを
そのまま C++ に移してある。

## クイックスタート

### 1. コンテナで開く

VS Code で DevContainer を起動:

- 左下の `><` アイコン → `Reopen in Container`
- または Command Palette (`Cmd+Shift+P`) → `Dev Containers: Reopen in Container`

### 2. 環境確認

```bash
g++ --version     # GCC 13
cmake --version   # CMake 3.28
ninja --version   # Ninja 1.11
locale            # ja_JP.UTF-8
date              # Asia/Tokyo
```

### 3. ビルド・テスト・実行

```bash
cmake -S . -B build                          # 構成（初回は GoogleTest を取得する）
cmake --build build                          # ビルド
ctest --test-dir build --output-on-failure   # テスト実行
./build/trainroute_app                       # アプリ実行
```

`CMAKE_GENERATOR=Ninja` をコンテナの環境変数に設定してあるので、`-G Ninja` は不要。

環境そのものを検証したいときは次を実行する。

```bash
bash scripts/verify-env.sh
```

## リポジトリの構成

```text
src/bad/trainroute/domain/   練習の題材。路線検索のドメインコード
src/hello_world.{h,cpp}      動作確認用の最小コード
tests/hello_world_test.cpp   GoogleTest の最小テスト
tests/gmock_examples_test.cpp GoogleMock の stub / mock / spy の例
CMakeLists.txt               ビルド定義と GoogleTest の取得・ピン留め
.clang-format                整形規則。エディタと clang-format コマンドが共用する
.clang-tidy                  静的解析の検査規則。エディタと clang-tidy コマンドが共用する
```

### 練習の題材

`src/bad/trainroute/domain/` に、中央線と山手線だけを扱う路線検索のコードが
入っている。テストは付いていない。次のような点が手つかずのまま残してある。

- すべてが static メンバ関数で、依存を差し替えられない
- 同じ処理の関数が名前だけ変えて重複している
  （`searchRoute` / `findRoute`、`getTimetableForStation` / `showSchedule`、
  `isValidStation` / `stationExists` / `checkStation`）
- 路線名や駅名が文字列リテラルとしてあちこちに散らばっている
- `a` `b` `idx1` `x` `y` `z` のような、意味の分からない変数名
- `split` が2つのファイルにコピーされている
- 固定長の配列とカウンタで可変長のデータを扱っている
- 動作が間違っている関数が混じっている

C++ ではモックできるのは仮想関数だけなので、テストダブルを使うには
まず依存をインターフェースとして切り出す必要がある。「テストを書ける形に
変える」ところからが練習になる。

## 環境仕様

### 基本設定

| 項目 | 値 |
|------|-----|
| ベースイメージ | Ubuntu 24.04 LTS |
| コンパイラ | GCC 13 (g++, C++20) |
| ビルドシステム | CMake 3.28 + Ninja 1.11 |
| テストフレームワーク | GoogleTest 1.18.0 |
| モックフレームワーク | GoogleMock 1.18.0（GoogleTest に同梱） |
| ユーザー | vscode (非root) |
| ロケール | ja_JP.UTF-8 |
| タイムゾーン | Asia/Tokyo |

GCC・CMake・Ninja は Ubuntu 24.04 の apt パッケージをそのまま使う。
GoogleTest は CMake の `FetchContent` で取得し、`CMakeLists.txt` で
コミットハッシュ（`063de7e9578f82b369302001269680b4b1553359` = v1.18.0）に
固定している。タグは付け替えられる可能性があるため、再現性が必要な
ピン留めにはハッシュを使う。バージョンを変えるときは `CMakeLists.txt` と
この表、`scripts/verify-env.sh` の3箇所を揃えること。

### 環境変数（自動設定）

```bash
CMAKE_GENERATOR=Ninja
LANG=ja_JP.UTF-8
TZ=Asia/Tokyo
```

### インストール済みツール

- **バージョン管理**: git
- **エディタ**: vim, nano
- **ユーティリティ**: curl, wget, jq, tree, htop, less
- **ビルド・開発**: build-essential (g++, make), cmake, ninja-build, gdb,
  clang-format, clang-tidy

clang-format と clang-tidy の設定はリポジトリ直下の `.clang-format` /
`.clang-tidy` にある。エディタ（C/C++ 拡張）もコマンドラインもこの2つを
読むので、どちらで実行しても同じ結果になる。

```bash
clang-format -i src/bad/trainroute/domain/route_searcher.cpp   # 整形
clang-tidy -p build src/bad/trainroute/domain/route_searcher.cpp   # 静的解析
```

VS Code では保存時に自動で整形され、clang-tidy の指摘は「問題」パネルと
Error Lens でコード上に表示される。指摘が多すぎるときは `.clang-tidy` の
`Checks` からカテゴリごと外す。

### VS Code 拡張機能

`.devcontainer/devcontainer.json` でバージョンまで固定してある。

| 拡張機能 | ID | バージョン | 用途 |
|---|---|---|---|
| C/C++ | `ms-vscode.cpptools` | 1.34.4 | IntelliSense / デバッグ / 整形 / clang-tidy |
| CMake Tools | `ms-vscode.cmake-tools` | 1.24.42 | 構成・ビルド・CTest 実行 |
| Markdown All in One | `yzhang.markdown-all-in-one` | 3.6.3 | 目次・表の整形 |
| Markdown Lint | `DavidAnson.vscode-markdownlint` | 0.62.1 | Markdown の書式チェック |
| Code Spell Checker | `streetsidesoftware.code-spell-checker` | 4.9.3 | 綴りチェック |
| Error Lens | `usernamehw.errorlens` | 3.28.0 | 診断をコード行に直接表示 |
| GitLens | `eamodio.gitlens` | 19.2.0 | 行単位の履歴 / blame / コミットグラフ |
| Live Share | `MS-vsliveshare.vsliveshare` | 1.1.122 | ペアプロ / モブプロ |

固定した版が自動更新で上書きされないよう、コンテナ側で
`"extensions.autoUpdate": false` にしてある。

拡張パック (`ms-vscode.cpptools-extension-pack`) は使っていない。パックの
バージョンを固定しても中身の拡張のバージョンは固定されないため、中身を
個別に並べている。

CMake の構文ハイライトとテストエクスプローラ（CTest 連携）は CMake Tools が
持っているので、別途 `twxs.cmake` や TestMate C++ を入れる必要はない。

### Live Share を使う

コンテナ内から共同編集セッションを開始できる。

1. 初回のみサインインが必要
   Command Palette (`Cmd+Shift+P`) → `Live Share: Sign In` →
   GitHub または Microsoft アカウントでブラウザ認証
2. Command Palette → `Live Share: Start Collaboration Session`
3. クリップボードにコピーされた招待 URL を相手に共有

参加者の接続は都度承認を求める設定 (`liveshare.guestApprovalRequired`) にしてある。

## テストの書き方

GoogleTest と GoogleMock の使い方は `tests/gmock_examples_test.cpp` にまとめてある。

- **Stub** — `ON_CALL(...).WillByDefault(Return(...))`。決まった値を返すだけで、
  呼ばれたかどうかは検証しない。`NiceMock` と組み合わせる。
- **Mock** — `EXPECT_CALL(...)`。呼ばれ方を宣言し、モックの破棄時に自動で
  検証される。Mockito のように後から `verify(...)` を書く必要はない。
- **Spy** — 既定の動作を本物のオブジェクトへの委譲にしたモックを作る。
  一部のメソッドだけ `EXPECT_CALL` で差し替えられる。

テストを追加するときは `CMakeLists.txt` の `unit_tests` にソースを足す。

```cmake
add_executable(unit_tests
    tests/hello_world_test.cpp
    tests/gmock_examples_test.cpp
    tests/your_new_test.cpp
)
```

## カスタマイズ

### 拡張機能を追加

`.devcontainer/devcontainer.json` の `customizations.vscode.extensions` に
`publisher.name@1.2.3` の形で ID とバージョンを追加し、Command Palette から
`Dev Containers: Rebuild Container` を実行する。あわせて上記の一覧表も更新する。

バージョンは pre-release ではない安定版を選ぶこと。マーケットプレイスが
「最新」として出すものが pre-release のことがある（例: C/C++ 拡張は
奇数マイナー版が pre-release）。

### C++ の規格を変える

`CMakeLists.txt` の `CMAKE_CXX_STANDARD` を変更する。
`.vscode/settings.json` の `C_Cpp.default.cppStandard` も合わせること。

### GoogleTest のバージョンを変える

`CMakeLists.txt` の `GIT_TAG` を新しいコミットハッシュに変更し、
`build/` を消してから再構成する。

```bash
rm -rf build
cmake -S . -B build
```

## トラブルシューティング

### GoogleTest の取得に失敗する

`cmake -S . -B build` は初回に GitHub から GoogleTest を clone する。
ネットワークに接続できない環境では構成が失敗する。取得済みのものは
`build/_deps/` に残るので、`build/` を消さなければ再取得は発生しない。

### IntelliSense がヘッダを見つけられない

`compile_commands.json` はビルド時に `build/` に生成される。
一度ビルドしてから VS Code をリロードする。

```bash
cmake --build build
```

### ビルド結果をすべて捨ててやり直す

```bash
rm -rf build
cmake -S . -B build
cmake --build build
```

### コンテナを完全に再構築

```bash
# Command Palette (Cmd+Shift+P)
Dev Containers: Rebuild Container

# またはキャッシュなしで再構築
Dev Containers: Rebuild Without Cache
```

### Git 設定

初回セットアップ時:

```bash
git config --global user.name "Your Name"
git config --global user.email "your.email@example.com"
```

## ライセンス

MIT License - 詳細は [LICENSE](LICENSE) を参照
