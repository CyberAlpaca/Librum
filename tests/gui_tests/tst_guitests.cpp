#include "../../src/application_init.h"
#include <QGuiApplication>
#include <QtTest>
#include <QQmlApplicationEngine>
#include <QQuickWindow>
#include <QQuickItem>
#include <QtConcurrent>
#include <QQmlExpression>
#include <QQmlContext>
#include <QQmlProperty>
#include <suri/automator.h>

class GuiTests : public QObject
{
    Q_OBJECT

private:
    QQuickWindow *window;
    QPointer<QApplication> app;
    QPointer<QQmlApplicationEngine> engine;
    QFuture<void> future;

public:
    GuiTests();
    ~GuiTests();

private slots:
    void initTestCase();
    void init();
    void cleanupTestCase();
    void cleanup();
    void test_forgotPasswordAndBack();
    void test_registerAndBack();
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

void GuiTests::init()
{
    auto [app, engine] = initializeApplication(0, nullptr);
    this->app = app;
    this->engine = engine;
    this->window = qobject_cast<QQuickWindow *>(this->engine->rootObjects().first());
    QVERIFY(window);
    window->showMaximized();
    this->future = QtConcurrent::run(app->exec);
    QTest::qSleep(500);
}

void GuiTests::cleanup()
{
    app->exit(0);
    QApplication::quit();
    QTest::qSleep(5000);
}

void GuiTests::test_forgotPasswordAndBack()
{
    Automator automator(engine);
    QQuickItem *forgotPasswordLink = qobject_cast<QQuickItem*>(automator.findObject("forgotPasswordLabel"));
    automator.click(forgotPasswordLink);
    QTest::qSleep(500);
    QQuickItem *backButton = qobject_cast<QQuickItem*>(automator.findObject("backFromForgotPassword"));
    automator.click(backButton);
    QTest::qSleep(2000);
}

void GuiTests::test_registerAndBack()
{
    Automator automator(engine);
    QQuickItem *forgotPasswordLink = qobject_cast<QQuickItem*>(automator.findObject("registerLinkLabel"));
    automator.click(forgotPasswordLink);
    QTest::qSleep(500);
    QQuickItem *backButton = qobject_cast<QQuickItem*>(automator.findObject("backFromRegister"));
    automator.click(backButton);
    QTest::qSleep(2000);
}

// QTEST_MAIN(GuiTests)

#include "tst_guitests.moc"

int main(int argc, char *argv[])
{
    GuiTests tests = GuiTests();
    return QTest::qExec(&tests, argc, argv);
}
