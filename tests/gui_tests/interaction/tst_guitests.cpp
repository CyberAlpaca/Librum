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

public:
    ButtonInteraction();
    ~ButtonInteraction();

private slots:
    void test_one();
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

#include "tst_guitests.moc"

int main(int argc, char *argv[])
{
    ButtonInteraction tests = ButtonInteraction();
    QTest::qExec(&tests, argc, argv);
}
