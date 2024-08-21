#include "../../src/application_init.h"
#include <QGuiApplication>
#include <QTest>
#include <QQmlApplicationEngine>
#include <QQuickWindow>
#include <QQuickItem>
#include <QtConcurrent>
#include <QQmlExpression>
#include <QQmlContext>
#include <QQmlProperty>
#include <suri/automator.h>

class NavigationTest : public QObject
{
    Q_OBJECT

private:
    QPointer<QApplication> app;
    QPointer<QQmlApplicationEngine> engine;
    QFuture<void> future;
    void test_one_steps();
    void test_two_steps();

public:
    NavigationTest();
    ~NavigationTest();

private slots:
    void initTestCase();
    void cleanupTestCase();
    void test_forgotPasswordAndBack();
    void test_registerAndBack();
};

NavigationTest::NavigationTest()
{
}

NavigationTest::~NavigationTest()
{
}

void NavigationTest::initTestCase()
{
    auto [app, engine] = initializeApplication(0, nullptr);
    this->app = app;
    this->engine = engine;
    this->future = QtConcurrent::run(app->exec);
    QTest::qSleep(500);
}

void NavigationTest::cleanupTestCase()
{
    this->engine->deleteLater();
    this->app->exit(0);
}


void NavigationTest::test_forgotPasswordAndBack()
{
    Automator automator(engine, app);
    QQuickItem *forgotPasswordLink = qobject_cast<QQuickItem*>(automator.findObject("forgotPasswordLabel"));
    automator.click(forgotPasswordLink);
    QTest::qSleep(500);
    QQuickItem *backButton = qobject_cast<QQuickItem*>(automator.findObject("backFromForgotPassword"));
    automator.click(backButton);
    QTest::qSleep(2000);
}

void NavigationTest::test_registerAndBack()
{
    Automator automator(engine, app);
    QQuickItem *forgotPasswordLink = qobject_cast<QQuickItem*>(automator.findObject("registerLinkLabel"));
    automator.click(forgotPasswordLink);
    QTest::qSleep(500);
    QQuickItem *backButton = qobject_cast<QQuickItem*>(automator.findObject("backFromRegister"));
    automator.click(backButton);
    QTest::qSleep(2000);
}

#include "tst_navigation.moc"

int main(int argc, char *argv[])
{
    NavigationTest navTest = NavigationTest();
    QTest::qExec(&navTest, argc, argv);
}
