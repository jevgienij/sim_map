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
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QScrollArea>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include "mapwidget.h"
#include "vlc-qt/WidgetVideo.h"

QT_BEGIN_NAMESPACE

class Ui_SimMapWindow
{
public:
    QAction *actionOpenUrl;
    QWidget *centralQWidget;
    QHBoxLayout *horizontalLayout;
    QScrollArea *scrollArea;
    MapWidget *mapWidget;
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
        centralQWidget = new QWidget(SimMapWindow);
        centralQWidget->setObjectName(QStringLiteral("centralQWidget"));
        centralQWidget->setEnabled(true);
        horizontalLayout = new QHBoxLayout(centralQWidget);
        horizontalLayout->setSpacing(6);
        horizontalLayout->setContentsMargins(11, 11, 11, 11);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        scrollArea = new QScrollArea(centralQWidget);
        scrollArea->setObjectName(QStringLiteral("scrollArea"));
        scrollArea->setLineWidth(1);
        scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        scrollArea->setWidgetResizable(false);
        scrollArea->setAlignment(Qt::AlignCenter);
        mapWidget = new MapWidget();
        mapWidget->setObjectName(QStringLiteral("mapWidget"));
        mapWidget->setEnabled(true);
        mapWidget->setGeometry(QRect(0, 0, 900, 900));
        scrollArea->setWidget(mapWidget);

        horizontalLayout->addWidget(scrollArea);

        SimMapWindow->setCentralWidget(centralQWidget);
        statusBar = new QStatusBar(SimMapWindow);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        statusBar->setEnabled(false);
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
        SimMapWindow->setWindowTitle(QApplication::translate("SimMapWindow", "Sim Map", 0));
        actionOpenUrl->setText(QApplication::translate("SimMapWindow", "Open URL", 0));
        setStreamUrl->setText(QApplication::translate("SimMapWindow", "Set stream URL", 0));
    } // retranslateUi

};

namespace Ui {
    class SimMapWindow: public Ui_SimMapWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
