#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_all.hpp>
#include <suri/automator.h>
#include <QQuickItem>
#include "TestFixture.h"
#include "macros.h"

TEST_CASE_METHOD(TestFixture, "Basic UI Interaction") {

    QQuickItem *forgotPasswordLink = qobject_cast<QQuickItem*>(automator.findObject("forgotPasswordLabel"));
    REQUIRE_MSG(forgotPasswordLink,
                    "forgotPasswordLink should be found");

    INFO("Click forgotPasswordLink");
    automator.click(forgotPasswordLink);
    automator.wait(1000);

    QQuickItem *backFromForgotPassword = qobject_cast<QQuickItem*>(automator.findObject("backFromForgotPassword"));
    REQUIRE_MSG(backFromForgotPassword,
                    "backFromForgotPassword should be found");

    INFO("Click backFromForgotPassword");
    automator.click(backFromForgotPassword);
    automator.wait(1000);

    QQuickItem *myInputField = qobject_cast<QQuickItem*>(automator.findObject("emailInput_textInput"));

    REQUIRE_MSG(myInputField, "myInputField should be found");
    REQUIRE_MSG(myInputField->isVisible(), "myInputField should be visible");
    automator.typeText(myInputField, "alpaca@cyberalpaca.com");

    automator.wait(1000);
    QVariant textProperty = myInputField->property("text");
    QString txt = textProperty.toString();

    REQUIRE_THAT(txt.toStdString(), Catch::Matchers::Equals("alpaca@cyberalpaca.com"));

    automator.wait(1000);
    automator.click(myInputField);

    automator.wait(1000);

}
