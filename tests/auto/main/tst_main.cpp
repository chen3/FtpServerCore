#include <QtTest>

// add necessary includes here

class main : public QObject
{
    Q_OBJECT

public:
    main();
    ~main();

private slots:
    void test_case1();

};

main::main()
{

}

main::~main()
{

}

void main::test_case1()
{

}

QTEST_APPLESS_MAIN(main)

#include "tst_main.moc"
