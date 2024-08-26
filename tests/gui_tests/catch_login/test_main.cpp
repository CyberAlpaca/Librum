#define CATCH_CONFIG_RUNNER
#include <catch2/catch_session.hpp>
#include <QtGui/QGuiApplication>
//#include "../../../src/application_init.h"

int main(int argc, char** argv)
{
    // my setup
    /*
    auto [app, engine] = initializeApplication(0, nullptr);

    if (!app || !engine) {
        exit(1);
    }
    */
    //automator.setEngine(engine);

    int result = Catch::Session().run(argc, argv);

    //my cleanup

    return result;

}
