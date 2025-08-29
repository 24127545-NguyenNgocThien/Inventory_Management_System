#include "log.h"

QString Log::logFileName = "app.log";

void Log::Write(Level level, const QString& message)
{
    QString logMessage = QString("[%1] [%2] %3")
    .arg(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"))
        .arg(LevelToString(level))
        .arg(message);

    // Ghi ra console
    qDebug().noquote() << logMessage;

    // Ghi ra file
    QFile file(logFileName);
    if (file.open(QIODevice::Append | QIODevice::Text))
    {
        QTextStream out(&file);
        out << logMessage << "\n";
        file.close();
    }
}

void Log::Info(const QString& message)
{
    Write(Level::Info, message);
}

void Log::Warning(const QString& message)
{
    Write(Level::Warning, message);
}

void Log::Error(const QString& message)
{
    Write(Level::Error, message);
}

QString Log::LevelToString(Level level)
{
    switch (level) {
    case Level::Info:    return "INFO";
    case Level::Warning: return "WARNING";
    case Level::Error:   return "ERROR";
    }
    return "UNKNOWN";
}
