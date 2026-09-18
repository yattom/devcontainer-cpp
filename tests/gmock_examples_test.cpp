#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <stdexcept>
#include <string>
#include <vector>

/**
 * GoogleMock の3種類のテストダブルの使い方の例。
 *
 * - Stub  — 決まった値を返すだけの偽物。テスト対象に、本来なら得られない
 *           入力を与えるために使う。
 * - Mock  — やりとりを EXPECT_CALL で宣言し、その通りに呼ばれたかを
 *           テスト終了時に検証する偽物。
 * - Spy   — 本物のオブジェクトに処理を委譲する偽物。明示的にスタブしない
 *           限り本物の動作が走るので、一部のメソッドだけ差し替えられる。
 *
 * C++ では実行時に型を差し替えられないので、モックできるのは仮想関数だけ。
 * そのため協働オブジェクトは、まずインターフェース（純粋仮想クラス）として
 * 切り出しておく必要がある。
 */

namespace {

using ::testing::_;
using ::testing::ElementsAre;
using ::testing::NiceMock;
using ::testing::Return;
using ::testing::Throw;

// ---- 協働オブジェクトのインターフェース --------------------------------

class StringList {
public:
    virtual ~StringList() = default;
    virtual void add(const std::string& value) = 0;
    virtual std::string get(int index) const = 0;
    virtual int size() const = 0;
};

class StringIterator {
public:
    virtual ~StringIterator() = default;
    virtual bool hasNext() = 0;
    virtual std::string next() = 0;
};

class StringConsumer {
public:
    virtual ~StringConsumer() = default;
    virtual void accept(const std::string& value) = 0;
};

class IntPredicate {
public:
    virtual ~IntPredicate() = default;
    virtual bool test(int value) = 0;
};

// ---- モッククラス -------------------------------------------------------

class MockStringList : public StringList {
public:
    MOCK_METHOD(void, add, (const std::string& value), (override));
    MOCK_METHOD(std::string, get, (int index), (const, override));
    MOCK_METHOD(int, size, (), (const, override));
};

class MockStringIterator : public StringIterator {
public:
    MOCK_METHOD(bool, hasNext, (), (override));
    MOCK_METHOD(std::string, next, (), (override));
};

class MockStringConsumer : public StringConsumer {
public:
    MOCK_METHOD(void, accept, (const std::string& value), (override));
};

class MockIntPredicate : public IntPredicate {
public:
    MOCK_METHOD(bool, test, (int value), (override));
};

// ---- テスト対象になる、協働オブジェクトを使う関数 -----------------------

void forEach(const std::vector<std::string>& values, StringConsumer& consumer) {
    for (const std::string& value : values) {
        consumer.accept(value);
    }
}

void removeIf(std::vector<int>& numbers, IntPredicate& predicate) {
    std::vector<int> kept;
    for (int number : numbers) {
        if (!predicate.test(number)) {
            kept.push_back(number);
        }
    }
    numbers = kept;
}

// ---- Stub: 決まった値を返す --------------------------------------------

TEST(GoogleMockExamples, Stub_ReturnsCannedValues) {
    // NiceMock にすると、スタブしていないメソッドが呼ばれても警告が出ない。
    // ON_CALL は「呼ばれたらこう返す」だけを決め、呼ばれたかどうかは検証しない。
    NiceMock<MockStringList> stubbedList;
    ON_CALL(stubbedList, get(0)).WillByDefault(Return("first"));
    ON_CALL(stubbedList, size()).WillByDefault(Return(3));

    EXPECT_EQ("first", stubbedList.get(0));
    EXPECT_EQ(3, stubbedList.size());
}

TEST(GoogleMockExamples, Stub_ReturnsDifferentValuesOnSuccessiveCallsThenThrows) {
    // 呼ばれるたびに違う値を返すには WillOnce を並べる。ここでは回数も
    // 決めたいので EXPECT_CALL を使う（GoogleMock ではスタブと検証を
    // 同じ EXPECT_CALL で書く点が Mockito と違う）。
    MockStringIterator stubbedIterator;
    EXPECT_CALL(stubbedIterator, hasNext())
        .WillOnce(Return(true))
        .WillOnce(Return(true))
        .WillOnce(Return(false));
    EXPECT_CALL(stubbedIterator, next())
        .WillOnce(Return("a"))
        .WillOnce(Return("b"))
        .WillRepeatedly(Throw(std::out_of_range("no more elements")));

    std::vector<std::string> collected;
    while (stubbedIterator.hasNext()) {
        collected.push_back(stubbedIterator.next());
    }

    EXPECT_THAT(collected, ElementsAre("a", "b"));
}

// ---- Mock: テスト対象が協働オブジェクトをどう使ったか検証する -----------

TEST(GoogleMockExamples, Mock_VerifiesEachElementWasPassedToTheCollaborator) {
    MockStringConsumer mockConsumer;
    EXPECT_CALL(mockConsumer, accept("Alice"));
    EXPECT_CALL(mockConsumer, accept("Bob"));
    EXPECT_CALL(mockConsumer, accept("Carol"));

    forEach({"Alice", "Bob", "Carol"}, mockConsumer);

    // EXPECT_CALL はテスト終了時（モックの破棄時）に自動で検証される。
    // Mockito の verify(...) のように後から書く必要はない。
}

TEST(GoogleMockExamples, Mock_VerifiesCollaboratorWasConsultedForEveryElement) {
    MockIntPredicate mockPredicate;
    // 引数が何であれ3回呼ばれ、常に false を返す、という宣言。
    // 複数の EXPECT_CALL の条件が重なる場合、後に書いたものから順に
    // 照合されるので、広いマッチャ（_）と個別のマッチャを混ぜるときは注意する。
    EXPECT_CALL(mockPredicate, test(_)).Times(3).WillRepeatedly(Return(false));

    std::vector<int> numbers = {1, 2, 3};
    removeIf(numbers, mockPredicate);

    EXPECT_EQ(3u, numbers.size());  // mock が常に false を返すので、何も削除されない
}

// ---- Spy: 本物のオブジェクトに委譲し、一部のメソッドだけ差し替える -------

class VectorStringList : public StringList {
public:
    void add(const std::string& value) override { values_.push_back(value); }
    std::string get(int index) const override { return values_[index]; }
    int size() const override { return static_cast<int>(values_.size()); }

private:
    std::vector<std::string> values_;
};

// 既定の動作を本物のオブジェクトへの委譲にしたモック。これが GoogleMock で
// spy を作るときの定石。
class SpiedStringList : public MockStringList {
public:
    SpiedStringList() {
        ON_CALL(*this, add(_)).WillByDefault([this](const std::string& value) {
            real_.add(value);
        });
        ON_CALL(*this, get(_)).WillByDefault([this](int index) {
            return real_.get(index);
        });
        ON_CALL(*this, size()).WillByDefault([this]() {
            return real_.size();
        });
    }

private:
    VectorStringList real_;
};

TEST(GoogleMockExamples, Spy_RealMethodsRunUnlessStubbed) {
    NiceMock<SpiedStringList> spiedList;
    // 動作を書かない EXPECT_CALL は、ON_CALL で決めた既定の動作
    //（＝本物への委譲）をそのまま使う。
    EXPECT_CALL(spiedList, add("real"));

    spiedList.add("real");

    EXPECT_EQ(1, spiedList.size());        // 本物の VectorStringList の動作
    EXPECT_EQ("real", spiedList.get(0));   // 本物の VectorStringList の動作
}

TEST(GoogleMockExamples, Spy_StubsOneMethodWhileOthersStayReal) {
    NiceMock<SpiedStringList> spiedList;
    spiedList.add("a");
    spiedList.add("b");

    EXPECT_CALL(spiedList, size()).WillRepeatedly(Return(100));

    EXPECT_EQ(100, spiedList.size());     // スタブした値
    EXPECT_EQ("a", spiedList.get(0));     // ここは本物のまま
}

}  // namespace
