#ifndef LOG_H
#define LOG_H

#include <QString>
#include <QDateTime>
#include <QFile>
#include <QTextStream>
#include <QDebug>

class Log
{
public:
    enum class Level {
        Info,
        Warning,
        Error
    };

    // Ghi log (console + file)
    static void Write(Level level, const QString& message);

    // Shortcut
    static void Info(const QString& message);
    static void Warning(const QString& message);
    static void Error(const QString& message);

private:
    static QString LevelToString(Level level);
    static QString logFileName;
};

#endif // LOG_H
