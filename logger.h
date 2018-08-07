#ifndef LOGGER_H
#define LOGGER_H

#include <QLoggingCategory>
#include <QScopedPointer>
#include <QMessageBox>

Q_DECLARE_LOGGING_CATEGORY(logDebug)
Q_DECLARE_LOGGING_CATEGORY(logInfo)
Q_DECLARE_LOGGING_CATEGORY(logWarning)
Q_DECLARE_LOGGING_CATEGORY(logCritical)

#endif // LOGGER_H
