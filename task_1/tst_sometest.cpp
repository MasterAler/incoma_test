#include <QtTest>

#include "SomeClass.h"

/*!
 * \brief The SomeTest class
 * это небольшой юнит-тест, демонстрирующий, что релизованные
 * конструкторы и операторы реализованы корректно. Не то чтобы это было
 * необходимо, просто взял на себя смелость добавить это к решению, чтобы
 * было поинтереснее. Ну и показать, что я слышал про тесты, да =)
 */
class SomeTest : public QObject
{
    Q_OBJECT

private slots:
    void test_initialization();
    void test_copy_construction();
    void test_move_construction();
    void test_assignment();

private:
    SomeClass foobar() { return SomeClass("FOOBAR"); }
    SomeClass test(SomeClass&& tt) { return std::forward<SomeClass>(tt); }
};

void SomeTest::test_initialization()
{
    SomeClass empty;
    SomeClass one("Hello, World!");
    SomeClass two{"I've said, hello, World!"};

    SomeClass three{std::string{"asfasfsafasfsafsafsafsaf"}};

    SomeClass four{"FUBAR"};
    QVERIFY(four == SomeClass{"FUBAR"});
}

void SomeTest::test_copy_construction()
{
    SomeClass one{"A quick brown fox jumped over a lazy frog"};
    SomeClass two(one);
    QVERIFY(one == two);

    SomeClass three{"FUBAR"};
    SomeClass four = three;
    QVERIFY(four == three);
}

void SomeTest::test_move_construction()
{
    SomeClass not_obvious_move{SomeClass{"ASDDSA"}};
    QVERIFY(not_obvious_move == SomeClass{"ASDDSA"});

    SomeClass one{std::move(SomeClass{"ASDDSA"})}; // excessive
    QVERIFY(one == SomeClass{"ASDDSA"});

    SomeClass two(foobar());
    QVERIFY(two == foobar());

    SomeClass three = foobar();
    QVERIFY(three == foobar());
}

void SomeTest::test_assignment()
{
    SomeClass one;
    SomeClass two{"TWO"};
    one = two;
    two = two;
    one = one;
    QVERIFY(one == two);

    SomeClass three{"12312412"};
    three = two = one;
    QVERIFY(one == three);
    QVERIFY(one == two);
    QVERIFY(two == three);

    // move

    one = foobar();
    two = foobar();
    QVERIFY(one == two);

    SomeClass a1, a2;
    a1 = std::move(one);
    a2 = std::move(two);
    QVERIFY(a1 == a2);

    SomeClass tt  = three;
    SomeClass tt2 = test(std::move(three));
    QVERIFY(tt == tt2);
}

QTEST_APPLESS_MAIN(SomeTest)

#include "tst_sometest.moc"
