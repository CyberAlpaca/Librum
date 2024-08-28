
#include "TestFixture.h"

#include "../../../src/application_init.h"

TestFixture::TestFixture() {
    // This code will still run before each SECTION
    // Let's guard initialization with a static variable
    if (!initialized) {
        initTestCase();
        initialized = true;
    }
}

TestFixture::~TestFixture() {
    // Cleanup code - optional (runs after each SECTION ends)
    // automator.closeApp();
}

void TestFixture::initTestCase() {
    auto [app, engine] = initializeApplication(0, nullptr);

    if (!app || !engine) {
        exit(1);
    }
    automator.wait(5000);
    automator.setEngine(engine);
}

