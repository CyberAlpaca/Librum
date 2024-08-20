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

class ButtonInteraction : public QObject
{
    Q_OBJECT

private:
    QQuickWindow *window;
    QPointer<QApplication> app;
    QPointer<QQmlApplicationEngine> engine;
    void steps_test_one();
    void steps_typeLogin();

public:
    ButtonInteraction();
    ~ButtonInteraction();

private slots:
    void test_one();
    void test_typeLogin();
};

ButtonInteraction::ButtonInteraction()
{
}

ButtonInteraction::~ButtonInteraction()
{
}

void ButtonInteraction::steps_test_one() {
    Automator automator(engine);
    QQuickItem *forgotPasswordLink = qobject_cast<QQuickItem*>(automator.findObject("forgotPasswordLabel"));
    automator.click(forgotPasswordLink);
    engine->deleteLater();
    app->quit();
    app->deleteLater();
}

void ButtonInteraction::test_one()
{
    auto [app, engine] = initializeApplication(0, nullptr);
    this->app = app;
    this->engine = engine;

    if (!app || !engine) {
        exit(1);
    }
    QTimer::singleShot(3000, this, &ButtonInteraction::steps_test_one);
    app->exec();
}

void ButtonInteraction::test_typeLogin()
{
    auto [app, engine] = initializeApplication(0, nullptr);
    this->app = app;
    this->engine = engine;

    if (!app || !engine) {
        exit(1);
    }
    QTimer::singleShot(3000, this, &ButtonInteraction::steps_typeLogin);
    app->exec();
}

void ButtonInteraction::steps_typeLogin()
{
    Automator automator(engine);

    QQuickItem *myInputField = qobject_cast<QQuickItem*>(automator.findObject("myInputField"));

    QVERIFY2(myInputField, "myInputField should be found");
    QVERIFY2(myInputField->isVisible(), "myInputField should be visible");
    automator.typeText(myInputField, "alpaca@cyberalpaca.com");

    QTest::qSleep(2000);
    QVariant textProperty = myInputField->property("text");
    QString txt = textProperty.toString();

    QCOMPARE(txt, "alpaca@cyberalpaca.com");

    QTest::qSleep(2000);
}

#include "tst_guitests.moc"

int main(int argc, char *argv[])
{
    ButtonInteraction tests = ButtonInteraction();
    QTest::qExec(&tests, argc, argv);
}
