#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_all.hpp>
#include <suri/automator.h>
#include "../../../src/application_init.h"
#include <QTest>
#include <QQuickItem>

Automator automator;

class TestFixture {
public:

    TestFixture() {
        // This code will still run before each SECTION
        // Let's guard initialization with a static variable
        if (!initialized) {
            initTestCase();
            initialized = true;
        }
    }

    ~TestFixture() {
        // Cleanup code - optional
        automator.closeApp();
    }

    void initTestCase() {
        auto [app, engine] = initializeApplication(0, nullptr);

        if (!app || !engine) {
            exit(1);
        }

        QTest::qSleep(5000);
        automator.setEngine(engine);
    }

private:
    inline static bool initialized = false;
};

TEST_CASE_METHOD(TestFixture, "Example test", "[example]") {

    SECTION("Test forgot password link") {
        QQuickItem *forgotPasswordLink = qobject_cast<QQuickItem*>(automator.findObject("forgotPasswordLabel"));
        REQUIRE(forgotPasswordLink); // "forgotPasswordLink should be found"

        INFO("Click forgotPasswordLink");
        automator.click(forgotPasswordLink);

        QTest::qSleep(1000);

        QQuickItem *backFromForgotPassword = qobject_cast<QQuickItem*>(automator.findObject("backFromForgotPassword"));
        REQUIRE(backFromForgotPassword); //"backFromForgotPassword should be found"

        INFO("Click backFromForgotPassword");
        automator.click(backFromForgotPassword);
        QTest::qSleep(1000);

    }

    SECTION("Type Login") {
        QQuickItem *myInputField = qobject_cast<QQuickItem*>(automator.findObject("myInputField"));

        REQUIRE(myInputField); //"myInputField should be found"
        REQUIRE(myInputField->isVisible());// "myInputField should be visible"
        automator.typeText(myInputField, "alpaca@cyberalpaca.com");

        QTest::qSleep(1000);
        QVariant textProperty = myInputField->property("text");
        QString txt = textProperty.toString();

        REQUIRE_THAT(txt.toStdString(), Catch::Matchers::Equals("alpaca@cyberalpaca.com"));

        QTest::qSleep(1000);
        automator.click(myInputField);

        QTest::qSleep(1000);

    }

}

