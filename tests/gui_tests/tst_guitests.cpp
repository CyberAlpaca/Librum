#include <QCoreApplication>
#include <QtTest>

// add necessary includes here

class GuiTests : public QObject
{
    Q_OBJECT

public:
    GuiTests();
    ~GuiTests();

private slots:
    void initTestCase();
    void cleanupTestCase();
    void test_case1();
};

GuiTests::GuiTests()
{
}

GuiTests::~GuiTests()
{
}

void GuiTests::initTestCase()
{
}

void GuiTests::cleanupTestCase()
{
}

void GuiTests::test_case1()
{
}

QTEST_MAIN(GuiTests)

#include "tst_guitests.moc"
