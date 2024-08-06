#include <QPair>
#include <QGuiApplication>
#include "application_init.h"


int main(int argc, char* argv[])
{
    auto [app, engine] = initializeApplication(argc, argv);

    if (!app || !engine) {
        return -1;
    }

    // return app->exec();

    return QGuiApplication::exec();
}
