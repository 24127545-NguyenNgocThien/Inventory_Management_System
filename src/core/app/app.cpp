#include "app.h"

App::App(QObject *parent)
    : db(&Database::GetInstance())
    , w(new MainWindow(*db))
{}

App::~App()
{
    if(w) delete w;
}

void App::run()
{
    w->show();
}
