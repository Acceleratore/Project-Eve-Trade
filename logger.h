#ifndef LOGGER_H
#define LOGGER_H

#include <QObject>
#include <QLoggingCategory>
#include <QFile>
#include <QDir>
#include <QScopedPointer>
#include <QTextStream>
#include <QApplication>
#include <QMessageBox>
#include <QDir>

Q_DECLARE_LOGGING_CATEGORY(logDebug)
Q_DECLARE_LOGGING_CATEGORY(logInfo)
Q_DECLARE_LOGGING_CATEGORY(logWarning)
Q_DECLARE_LOGGING_CATEGORY(logCritical)

#endif // LOGGER_H
