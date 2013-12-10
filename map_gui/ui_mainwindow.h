/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.1.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDockWidget>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include "mapwidget.h"
#include "vlc-qt/WidgetVideo.h"

QT_BEGIN_NAMESPACE

class Ui_SimMapWindow
{
public:
    QAction *actionOpenUrl;
    MapWidget *widget;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;
    QDockWidget *dockWidget;
    QWidget *dockWidgetContents;
    QVBoxLayout *verticalLayout_2;
    VlcWidgetVideo *video;
    QPushButton *setStreamUrl;

    void setupUi(QMainWindow *SimMapWindow)
    {
        if (SimMapWindow->objectName().isEmpty())
            SimMapWindow->setObjectName(QStringLiteral("SimMapWindow"));
        SimMapWindow->resize(1020, 535);
        actionOpenUrl = new QAction(SimMapWindow);
        actionOpenUrl->setObjectName(QStringLiteral("actionOpenUrl"));
        widget = new MapWidget(SimMapWindow);
        widget->setObjectName(QStringLiteral("widget"));
        SimMapWindow->setCentralWidget(widget);
        menuBar = new QMenuBar(SimMapWindow);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 1020, 21));
        SimMapWindow->setMenuBar(menuBar);
        mainToolBar = new QToolBar(SimMapWindow);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        SimMapWindow->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(SimMapWindow);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        SimMapWindow->setStatusBar(statusBar);
        dockWidget = new QDockWidget(SimMapWindow);
        dockWidget->setObjectName(QStringLiteral("dockWidget"));
        dockWidget->setMinimumSize(QSize(400, 400));
        dockWidgetContents = new QWidget();
        dockWidgetContents->setObjectName(QStringLiteral("dockWidgetContents"));
        verticalLayout_2 = new QVBoxLayout(dockWidgetContents);
        verticalLayout_2->setSpacing(0);
        verticalLayout_2->setContentsMargins(11, 11, 11, 11);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        verticalLayout_2->setContentsMargins(5, 0, 0, 0);
        video = new VlcWidgetVideo(dockWidgetContents);
        video->setObjectName(QStringLiteral("video"));
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(video->sizePolicy().hasHeightForWidth());
        video->setSizePolicy(sizePolicy);

        verticalLayout_2->addWidget(video);

        setStreamUrl = new QPushButton(dockWidgetContents);
        setStreamUrl->setObjectName(QStringLiteral("setStreamUrl"));
        QSizePolicy sizePolicy1(QSizePolicy::Minimum, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(setStreamUrl->sizePolicy().hasHeightForWidth());
        setStreamUrl->setSizePolicy(sizePolicy1);
        setStreamUrl->setMinimumSize(QSize(0, 10));
        setStreamUrl->setMaximumSize(QSize(16777215, 20));

        verticalLayout_2->addWidget(setStreamUrl);

        dockWidget->setWidget(dockWidgetContents);
        setStreamUrl->raise();
        video->raise();
        SimMapWindow->addDockWidget(static_cast<Qt::DockWidgetArea>(2), dockWidget);

        retranslateUi(SimMapWindow);

        QMetaObject::connectSlotsByName(SimMapWindow);
    } // setupUi

    void retranslateUi(QMainWindow *SimMapWindow)
    {
        SimMapWindow->setWindowTitle(QApplication::translate("SimMapWindow", "MainWindow", 0));
        actionOpenUrl->setText(QApplication::translate("SimMapWindow", "Open URL", 0));
        setStreamUrl->setText(QApplication::translate("SimMapWindow", "Set stream URL", 0));
    } // retranslateUi

};

namespace Ui {
    class SimMapWindow: public Ui_SimMapWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
