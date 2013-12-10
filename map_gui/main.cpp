#include "mainwindow.h"
#include <QtCore/QCoreApplication>
#include <QtCore/QTextCodec>
#include <QApplication>

int main(int argc, char *argv[])
{
    QCoreApplication::setApplicationName("Sim Map");
    QCoreApplication::setAttribute(Qt::AA_X11InitThreads);

#if QT_VERSION < 0x050000
    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));
#endif

    QApplication a(argc, argv);
    SimMapWindow window;
    window.showMaximized();

    return a.exec();
}
