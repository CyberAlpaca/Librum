#include "../../src/application_init.h"
#include <QGuiApplication>
#include <QTest>
#include <QQmlApplicationEngine>
#include <QQuickItem>
#include <QtConcurrent>
#include <QQmlExpression>
#include <QQmlContext>
#include <QQmlProperty>
#include <suri/automator.h>

class BasicUIInteraction : public QObject
{
    Q_OBJECT

private:
    Automator automator;
    void test_steps();
    void step_forgotPassword();
    void step_typeLogin();
    void closeApp();

public:
    BasicUIInteraction();
    ~BasicUIInteraction();

private slots:
    void initTestCase();
    void test_main();

};

BasicUIInteraction::BasicUIInteraction()
{
}

BasicUIInteraction::~BasicUIInteraction()
{
}

void BasicUIInteraction::initTestCase()
{
    auto [app, engine] = initializeApplication(0, nullptr);

    if (!app || !engine) {
        exit(1);
    }

    automator.setEngine(engine);

    QTest::qSleep(500);
}

void BasicUIInteraction::test_main()
{
    QTimer::singleShot(3000, this, &BasicUIInteraction::test_steps);
    automator.startApp();
}

void BasicUIInteraction::test_steps(){
    step_typeLogin();
    step_forgotPassword();
    automator.closeApp();
}

void BasicUIInteraction::step_typeLogin()
{
    QQuickItem *myInputField = qobject_cast<QQuickItem*>(automator.findObject("myInputField"));

    QVERIFY2(myInputField, "myInputField should be found");
    QVERIFY2(myInputField->isVisible(), "myInputField should be visible");
    automator.typeText(myInputField, "alpaca@cyberalpaca.com");

    QTest::qSleep(1000);
    QVariant textProperty = myInputField->property("text");
    QString txt = textProperty.toString();

    QCOMPARE(txt, "alpaca@cyberalpaca.com");

    QTest::qSleep(1000);
    automator.click(myInputField);

    QTest::qSleep(1000);

}

void BasicUIInteraction::step_forgotPassword() {
    QQuickItem *forgotPasswordLink = qobject_cast<QQuickItem*>(automator.findObject("forgotPasswordLabel"));
    QVERIFY2(forgotPasswordLink, "forgotPasswordLink should be found");

    automator.click(forgotPasswordLink);

    QTest::qSleep(1000);

    QQuickItem *backFromForgotPassword = qobject_cast<QQuickItem*>(automator.findObject("backFromForgotPassword"));
    QVERIFY2(backFromForgotPassword, "backFromForgotPassword should be found");

    automator.click(backFromForgotPassword);
    QTest::qSleep(1000);

}

#include "tst_guitests.moc"

int main(int argc, char *argv[])
{
    BasicUIInteraction tests = BasicUIInteraction();
    QTest::qExec(&tests, argc, argv);
}
