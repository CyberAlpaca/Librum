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
    QQuickItem* findItemById(const QString &aName);

public:
    GuiTests(QPointer<QApplication>, QPointer<QQmlApplicationEngine>);
    ~GuiTests();

private slots:
    void initTestCase();
    void cleanupTestCase();
    void test_case1();
};

GuiTests::GuiTests(QPointer<QApplication> app, QPointer<QQmlApplicationEngine> engine)
{
    this->app = app;
    this->engine = engine;
}

GuiTests::~GuiTests()
{
}

QQuickItem* findItemByName(QQuickItem* parent, const QString& objectName) {
    QQuickItem* foundItem = parent->findChild<QQuickItem*>(objectName);
    if (foundItem) return foundItem;
    return nullptr;
}

QQuickItem* findItemByName(QQmlApplicationEngine* engine, const QString& objectName) {
    for (auto rootObject : engine->rootObjects()) {
        QQuickItem* item = qobject_cast<QQuickItem*>(rootObject);
        if (item) {
            QQuickItem* foundItem = findItemByName(item, objectName);
            if (foundItem) return foundItem;
        }
    }
    return nullptr;
}

void GuiTests::initTestCase()
{
    window = qobject_cast<QQuickWindow *>(engine->rootObjects().first());
    QVERIFY(window);
    // window->show();
    window->showMaximized();
    app->processEvents();
    QtConcurrent::run(app->exec);
    QTest::qSleep(2000);
}

void GuiTests::cleanupTestCase()
{
    window->close();
}

void GuiTests::test_case1()
{

    Automator automator(engine);

    QObject *loginButtonPure = window->findChild<QObject*>("loginButton");
    QQuickItem *buttonItem1Directly = window->findChild<QQuickItem*>("loginButton");
    QQuickItem *buttonItemCast = qobject_cast<QQuickItem*>(loginButtonPure);

    QQuickItem *link = window->findChild<QQuickItem*>("forgotPasswordLabel");
    automator.click(link);

    QTest::qSleep(20000);
}

// QTEST_MAIN(GuiTests)

#include "tst_guitests.moc"

int main(int argc, char *argv[])
{
    auto [app, engine] = initializeApplication(0, nullptr);

    GuiTests tests = GuiTests(app, engine);

    return QTest::qExec(&tests, argc, argv);
}
