#include <QtTest>

// add necessary includes here

class Main : public QObject
{
    Q_OBJECT

public:
    Main();
    ~Main();

private slots:
    void test_case1();

};

Main::Main()
{

}

Main::~Main()
{

}

void Main::test_case1()
{

}

QTEST_APPLESS_MAIN(Main)

#include "tst_main.moc"
