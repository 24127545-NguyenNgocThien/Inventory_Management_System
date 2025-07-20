#ifndef APP_H
#define APP_H

#include <QMainWindow>
#include <QObject>
#include "data.h"
#include "mainwindow.h"

class App : public QObject
{
    Q_OBJECT

private:
    Database* db;
    MainWindow* w;

public:
    explicit App(QObject *parent = nullptr);
    ~App();

    void run();
};

#endif // APP_H
