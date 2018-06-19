#include "mainwindow.h"
#include <QApplication>

QScopedPointer<QFile> LoggerFile;

void messageHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg);

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    //Проверка на существование папки логов
    if (!QDir("logs").exists())
        QDir().mkdir("logs");

    //Определяем файл для записи логов
    LoggerFile.reset(new QFile(QString("logs/LogFile.log")));

    //Определяем режим открытия
    LoggerFile.data()->open(QFile::Truncate | QFile::Text | QFile::ReadWrite);

    //Переопределяем функцию для вывода информации
    qInstallMessageHandler(messageHandler);


    MainWindow w;
    w.show();

    return a.exec();
}

//Переопределенная функция по выводу лога
void messageHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{

    QTextStream OutStreamLog(LoggerFile.data());

    // По типу определяем, к какому уровню относится сообщение

    switch (type)
    {
        case QtInfoMsg:     OutStreamLog << "INF "; break;
        case QtDebugMsg:    OutStreamLog << "DBG "; break;
        case QtWarningMsg:  OutStreamLog << "WRN "; break;
        case QtCriticalMsg: OutStreamLog << "CRT "; break;
        case QtFatalMsg:    OutStreamLog << "FTL "; break;
    }

    // Записываем в вывод категорию сообщения и само сообщение
    OutStreamLog << context.category << ": " << msg << endl;

    OutStreamLog.flush();    // Очищаем буферизированные данные

}
