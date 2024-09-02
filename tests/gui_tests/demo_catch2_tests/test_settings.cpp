#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_all.hpp>
#include <suri/automator.h>
#include <QQuickItem>
#include "TestFixture.h"
#include "macros.h"

TEST_CASE_METHOD(TestFixture, "Change Librum settings", "[example]") {

    automator.wait(500);

    QQuickItem *emailInput = qobject_cast<QQuickItem*>(automator.findObject("emailInput_textInput"));
    QQuickItem *passwordInput = qobject_cast<QQuickItem*>(automator.findObject("passwordInput_textInput"));
    REQUIRE_MSG(emailInput, "emailInput should be found");
    REQUIRE_MSG(emailInput->isVisible(), "emailInput should be visible");
    REQUIRE_MSG(passwordInput, "passwordInput should be found");
    REQUIRE_MSG(passwordInput->isVisible(), "passwordInput should be visible");

    INFO("Type contact@cyberalpaca.com to emailInput");
    automator.typeText(emailInput, "contact@cyberalpaca.com");
    automator.wait(500);

    INFO("Type correct password to passwordInput");
    automator.click(passwordInput);
    automator.wait(500);
    automator.typeText(passwordInput, "*****");
    automator.wait(500);

    INFO("Click login button");
    //QObjectList allButtons = automator.findObjectsText("MButton");
    automator.click("loginButton");

    automator.wait(2000);

    QQuickItem *settingsItem = qobject_cast<QQuickItem*>(automator.findObject("settingsItem"));
    REQUIRE_MSG(settingsItem, "settingsItem should be found");
    INFO("Click settings item");
    automator.click("settingsItem");
    automator.click("appearanceItem");
    automator.wait(500);

    QQuickItem *languageComboBox = qobject_cast<QQuickItem*>(automator.findObject("languageComboBox"));
    REQUIRE_THAT(languageComboBox->metaObject()->className(), Catch::Matchers::StartsWith("MComboBox"));

    INFO("Change language to Italian");
    automator.click("languageComboBox");
    automator.wait(500);
    QObjectList italianoObjects = automator.findObjectsByText("Italiano");
    REQUIRE_FALSE_MSG(italianoObjects.isEmpty(), "Italiano lang should be found");
    QQuickItem *italianoObject = qobject_cast<QQuickItem*>(italianoObjects.first());
    automator.click(italianoObject);

    INFO("Change language back to English");
    automator.click("languageComboBox");
    automator.wait(500);
    QObjectList englishObjects = automator.findObjectsByText("English");
    REQUIRE_FALSE_MSG(englishObjects.isEmpty(), "English lang should be found");
    QQuickItem *englishObject = qobject_cast<QQuickItem*>(englishObjects.first());
    automator.click(englishObject);

    automator.wait(5000);
}
