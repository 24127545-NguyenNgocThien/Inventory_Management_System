#include <QApplication>
#include "app.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    App myApp;
    myApp.run();

    return app.exec();
}
