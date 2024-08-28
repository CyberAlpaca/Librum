#include "../../src/application_init.h"
#include <QTest>
#include <QQuickItem>
#include <QtConcurrent>
#include <suri/automator.h>

class FailedLogin : public QObject
{
    Q_OBJECT

private:
    Automator automator;
    void steps_incorrect_credentials();
    void step_typeCredentials();
    void closeApp();

public:
    FailedLogin();
    ~FailedLogin();

private slots:
    void initTestCase();
    void test_incorrect_credentials();

};

FailedLogin::FailedLogin()
{
}

FailedLogin::~FailedLogin()
{
}

void FailedLogin::initTestCase()
{
    auto [app, engine] = initializeApplication(0, nullptr);
    QVERIFY2(app && engine, "App and engine should be initialized");
    automator.setEngine(engine);
}

void FailedLogin::test_incorrect_credentials()
{
    QTimer::singleShot(3000, this, &FailedLogin::steps_incorrect_credentials);
    automator.startApp();
}

void FailedLogin::steps_incorrect_credentials(){
    step_typeCredentials();
    automator.click("loginButton");
    QString expectedErrorMsg = "Your email or password is wrong";
    QString desc = "Login should fail with: " + expectedErrorMsg;

    if (!automator.waitUntil("passwordInput_errorLabel", "text", expectedErrorMsg, 10000)){
        automator.closeApp();
        QFAIL(desc.toUtf8());
    }
    automator.closeApp();
}

void FailedLogin::step_typeCredentials()
{
    QQuickItem *emailInput = qobject_cast<QQuickItem*>(automator.findObject("emailInput_textInput"));
    QQuickItem *passwordInput = qobject_cast<QQuickItem*>(automator.findObject("passwordInput_textInput"));
    QVERIFY2(emailInput, "emailInput should be found");
    QVERIFY2(emailInput->isVisible(), "emailInput should be visible");
    QVERIFY2(passwordInput, "passwordInput should be found");
    QVERIFY2(passwordInput->isVisible(), "passwordInput should be visible");

    automator.typeText(emailInput, "alpaca@cyberalpaca.com");
    automator.wait(500);
    automator.typeText(passwordInput, "invalidPassword");
    automator.wait(500);
}

#include "tst_failed_login.moc"

int main(int argc, char *argv[])
{
    FailedLogin tests = FailedLogin();
    QTest::qExec(&tests, argc, argv);
}
