#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_all.hpp>
#include <suri/automator.h>
#include <QQuickItem>
#include "TestFixture.h"

TEST_CASE_METHOD(TestFixture, "Login with invalid credentials", "[example]") {

    QQuickItem *emailInput = qobject_cast<QQuickItem*>(automator.findObject("emailInput_textInput"));
    QQuickItem *passwordInput = qobject_cast<QQuickItem*>(automator.findObject("passwordInput_textInput"));
    REQUIRE(emailInput); // emailInput should be found
    REQUIRE(emailInput->isVisible()); // emailInput should be visible
    REQUIRE(passwordInput); // passwordInput should be found
    REQUIRE(passwordInput->isVisible()); //passwordInput should be visible

    INFO("Type alpaca@cyberalpaca.com to emailInput");
    automator.typeText(emailInput, "alpaca@cyberalpaca.com");
    automator.wait(500);

    INFO("Type invalidPassword to passwordInput");
    automator.typeText(passwordInput, "invalidPassword");
    automator.wait(500);

    automator.click("loginButton");

    QString expectedErrorMsg = "Your email or password is wrong";
    REQUIRE(automator.waitUntil("passwordInput_errorLabel", "text", expectedErrorMsg, 10000));

}
