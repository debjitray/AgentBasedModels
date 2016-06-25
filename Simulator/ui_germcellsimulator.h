/********************************************************************************
** Form generated from reading UI file 'germcellsimulator.ui'
**
** Created: Sat Jul 7 15:43:58 2012
**      by: Qt User Interface Compiler version 4.6.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_GERMCELLSIMULATOR_H
#define UI_GERMCELLSIMULATOR_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QGraphicsView>
#include <QtGui/QGridLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLCDNumber>
#include <QtGui/QLabel>
#include <QtGui/QMainWindow>
#include <QtGui/QMenu>
#include <QtGui/QMenuBar>
#include <QtGui/QPushButton>
#include <QtGui/QSlider>
#include <QtGui/QSpacerItem>
#include <QtGui/QStatusBar>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_GermCellSimulator
{
public:
    QAction *action_New;
    QAction *actionExit;
    QAction *actionSave_CSV;
    QAction *actionSave_Simulator;
    QAction *actionSave_Position_Data;
    QAction *actionSave_Errors;
    QWidget *centralWidget;
    QGridLayout *gridLayout_6;
    QGridLayout *gridLayoutMain;
    QGridLayout *gridLayoutVisualSide;
    QSlider *verticalSliderZoom;
    QGridLayout *gridLayoutVisualPlusMinus;
    QLabel *labelPlus;
    QLabel *labelMinus;
    QGraphicsView *graphicsViewVisual;
    QGridLayout *gridLayoutVisualTop;
    QLCDNumber *lcdNumberHour;
    QLabel *labelHour;
    QLCDNumber *lcdNumberDay;
    QLabel *labelDay;
    QSpacerItem *horizontalSpacerVisualTopRight;
    QSpacerItem *horizontalSpacerVisualTopLeft;
    QLabel *labelZoom;
    QGridLayout *gridLayoutBottom;
    QSlider *horizontalSliderSpeed;
    QLabel *labelSpeed;
    QPushButton *pushButtonPause;
    QPushButton *pushButtonStart;
    QPushButton *pushButtonRecordStart;
    QPushButton *pushButtonRecordStop;
    QMenuBar *menuBar;
    QMenu *menuFile;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *GermCellSimulator)
    {
        if (GermCellSimulator->objectName().isEmpty())
            GermCellSimulator->setObjectName(QString::fromUtf8("GermCellSimulator"));
        GermCellSimulator->resize(862, 783);
        GermCellSimulator->setContextMenuPolicy(Qt::NoContextMenu);
        action_New = new QAction(GermCellSimulator);
        action_New->setObjectName(QString::fromUtf8("action_New"));
        actionExit = new QAction(GermCellSimulator);
        actionExit->setObjectName(QString::fromUtf8("actionExit"));
        actionSave_CSV = new QAction(GermCellSimulator);
        actionSave_CSV->setObjectName(QString::fromUtf8("actionSave_CSV"));
        actionSave_Simulator = new QAction(GermCellSimulator);
        actionSave_Simulator->setObjectName(QString::fromUtf8("actionSave_Simulator"));
        actionSave_Position_Data = new QAction(GermCellSimulator);
        actionSave_Position_Data->setObjectName(QString::fromUtf8("actionSave_Position_Data"));
        actionSave_Errors = new QAction(GermCellSimulator);
        actionSave_Errors->setObjectName(QString::fromUtf8("actionSave_Errors"));
        centralWidget = new QWidget(GermCellSimulator);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        centralWidget->setContextMenuPolicy(Qt::NoContextMenu);
        gridLayout_6 = new QGridLayout(centralWidget);
        gridLayout_6->setSpacing(6);
        gridLayout_6->setContentsMargins(11, 11, 11, 11);
        gridLayout_6->setObjectName(QString::fromUtf8("gridLayout_6"));
        gridLayoutMain = new QGridLayout();
        gridLayoutMain->setSpacing(6);
        gridLayoutMain->setObjectName(QString::fromUtf8("gridLayoutMain"));
        gridLayoutVisualSide = new QGridLayout();
        gridLayoutVisualSide->setSpacing(6);
        gridLayoutVisualSide->setObjectName(QString::fromUtf8("gridLayoutVisualSide"));
        verticalSliderZoom = new QSlider(centralWidget);
        verticalSliderZoom->setObjectName(QString::fromUtf8("verticalSliderZoom"));
        verticalSliderZoom->setEnabled(false);
        verticalSliderZoom->setContextMenuPolicy(Qt::NoContextMenu);
        verticalSliderZoom->setValue(50);
        verticalSliderZoom->setOrientation(Qt::Vertical);

        gridLayoutVisualSide->addWidget(verticalSliderZoom, 0, 1, 1, 1);

        gridLayoutVisualPlusMinus = new QGridLayout();
        gridLayoutVisualPlusMinus->setSpacing(6);
        gridLayoutVisualPlusMinus->setObjectName(QString::fromUtf8("gridLayoutVisualPlusMinus"));
        labelPlus = new QLabel(centralWidget);
        labelPlus->setObjectName(QString::fromUtf8("labelPlus"));
        QFont font;
        font.setFamily(QString::fromUtf8("Lucida Grande"));
        font.setPointSize(18);
        font.setBold(true);
        font.setItalic(false);
        font.setWeight(75);
        labelPlus->setFont(font);
        labelPlus->setContextMenuPolicy(Qt::NoContextMenu);
        labelPlus->setTextFormat(Qt::PlainText);
        labelPlus->setAlignment(Qt::AlignHCenter|Qt::AlignTop);

        gridLayoutVisualPlusMinus->addWidget(labelPlus, 1, 0, 1, 1);

        labelMinus = new QLabel(centralWidget);
        labelMinus->setObjectName(QString::fromUtf8("labelMinus"));
        labelMinus->setFont(font);
        labelMinus->setContextMenuPolicy(Qt::NoContextMenu);
        labelMinus->setTextFormat(Qt::PlainText);
        labelMinus->setAlignment(Qt::AlignBottom|Qt::AlignHCenter);

        gridLayoutVisualPlusMinus->addWidget(labelMinus, 2, 0, 1, 1);


        gridLayoutVisualSide->addLayout(gridLayoutVisualPlusMinus, 0, 2, 1, 1);

        graphicsViewVisual = new QGraphicsView(centralWidget);
        graphicsViewVisual->setObjectName(QString::fromUtf8("graphicsViewVisual"));
        graphicsViewVisual->setEnabled(true);
        graphicsViewVisual->viewport()->setProperty("cursor", QVariant(QCursor(Qt::ArrowCursor)));
        graphicsViewVisual->setFocusPolicy(Qt::StrongFocus);
        graphicsViewVisual->setContextMenuPolicy(Qt::NoContextMenu);
        graphicsViewVisual->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        graphicsViewVisual->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        graphicsViewVisual->setInteractive(true);
        graphicsViewVisual->setDragMode(QGraphicsView::ScrollHandDrag);
        graphicsViewVisual->setResizeAnchor(QGraphicsView::AnchorUnderMouse);

        gridLayoutVisualSide->addWidget(graphicsViewVisual, 0, 0, 1, 1);


        gridLayoutMain->addLayout(gridLayoutVisualSide, 1, 0, 1, 1);

        gridLayoutVisualTop = new QGridLayout();
        gridLayoutVisualTop->setSpacing(6);
        gridLayoutVisualTop->setObjectName(QString::fromUtf8("gridLayoutVisualTop"));
        lcdNumberHour = new QLCDNumber(centralWidget);
        lcdNumberHour->setObjectName(QString::fromUtf8("lcdNumberHour"));
        QFont font1;
        font1.setBold(false);
        font1.setWeight(50);
        lcdNumberHour->setFont(font1);
        lcdNumberHour->setContextMenuPolicy(Qt::NoContextMenu);
        lcdNumberHour->setFrameShadow(QFrame::Plain);
        lcdNumberHour->setSegmentStyle(QLCDNumber::Flat);

        gridLayoutVisualTop->addWidget(lcdNumberHour, 1, 4, 1, 1);

        labelHour = new QLabel(centralWidget);
        labelHour->setObjectName(QString::fromUtf8("labelHour"));
        QFont font2;
        font2.setPointSize(15);
        labelHour->setFont(font2);
        labelHour->setContextMenuPolicy(Qt::NoContextMenu);
        labelHour->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        labelHour->setMargin(3);
        labelHour->setIndent(10);

        gridLayoutVisualTop->addWidget(labelHour, 1, 3, 1, 1);

        lcdNumberDay = new QLCDNumber(centralWidget);
        lcdNumberDay->setObjectName(QString::fromUtf8("lcdNumberDay"));
        lcdNumberDay->setContextMenuPolicy(Qt::NoContextMenu);
        lcdNumberDay->setFrameShadow(QFrame::Plain);
        lcdNumberDay->setLineWidth(1);
        lcdNumberDay->setSegmentStyle(QLCDNumber::Flat);

        gridLayoutVisualTop->addWidget(lcdNumberDay, 1, 2, 1, 1);

        labelDay = new QLabel(centralWidget);
        labelDay->setObjectName(QString::fromUtf8("labelDay"));
        QFont font3;
        font3.setPointSize(15);
        font3.setBold(false);
        font3.setWeight(50);
        labelDay->setFont(font3);
        labelDay->setContextMenuPolicy(Qt::NoContextMenu);
        labelDay->setMargin(3);
        labelDay->setIndent(-1);

        gridLayoutVisualTop->addWidget(labelDay, 1, 1, 1, 1);

        horizontalSpacerVisualTopRight = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayoutVisualTop->addItem(horizontalSpacerVisualTopRight, 1, 5, 1, 1);

        horizontalSpacerVisualTopLeft = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayoutVisualTop->addItem(horizontalSpacerVisualTopLeft, 1, 0, 1, 1);

        labelZoom = new QLabel(centralWidget);
        labelZoom->setObjectName(QString::fromUtf8("labelZoom"));
        QFont font4;
        font4.setPointSize(13);
        labelZoom->setFont(font4);
        labelZoom->setContextMenuPolicy(Qt::NoContextMenu);
        labelZoom->setAlignment(Qt::AlignCenter);
        labelZoom->setMargin(6);

        gridLayoutVisualTop->addWidget(labelZoom, 1, 6, 1, 1);


        gridLayoutMain->addLayout(gridLayoutVisualTop, 0, 0, 1, 1);


        gridLayout_6->addLayout(gridLayoutMain, 0, 0, 1, 1);

        gridLayoutBottom = new QGridLayout();
        gridLayoutBottom->setSpacing(6);
        gridLayoutBottom->setObjectName(QString::fromUtf8("gridLayoutBottom"));
        horizontalSliderSpeed = new QSlider(centralWidget);
        horizontalSliderSpeed->setObjectName(QString::fromUtf8("horizontalSliderSpeed"));
        horizontalSliderSpeed->setEnabled(false);
        horizontalSliderSpeed->setContextMenuPolicy(Qt::NoContextMenu);
        horizontalSliderSpeed->setMinimum(1);
        horizontalSliderSpeed->setMaximum(1000);
        horizontalSliderSpeed->setSliderPosition(1000);
        horizontalSliderSpeed->setOrientation(Qt::Horizontal);
        horizontalSliderSpeed->setInvertedAppearance(true);
        horizontalSliderSpeed->setInvertedControls(false);

        gridLayoutBottom->addWidget(horizontalSliderSpeed, 0, 3, 1, 1);

        labelSpeed = new QLabel(centralWidget);
        labelSpeed->setObjectName(QString::fromUtf8("labelSpeed"));
        labelSpeed->setContextMenuPolicy(Qt::NoContextMenu);
        labelSpeed->setIndent(30);

        gridLayoutBottom->addWidget(labelSpeed, 0, 2, 1, 1);

        pushButtonPause = new QPushButton(centralWidget);
        pushButtonPause->setObjectName(QString::fromUtf8("pushButtonPause"));
        pushButtonPause->setEnabled(false);
        pushButtonPause->setContextMenuPolicy(Qt::NoContextMenu);

        gridLayoutBottom->addWidget(pushButtonPause, 0, 1, 1, 1);

        pushButtonStart = new QPushButton(centralWidget);
        pushButtonStart->setObjectName(QString::fromUtf8("pushButtonStart"));
        pushButtonStart->setEnabled(false);
        pushButtonStart->setContextMenuPolicy(Qt::NoContextMenu);

        gridLayoutBottom->addWidget(pushButtonStart, 0, 0, 1, 1);

        pushButtonRecordStart = new QPushButton(centralWidget);
        pushButtonRecordStart->setObjectName(QString::fromUtf8("pushButtonRecordStart"));
        pushButtonRecordStart->setEnabled(false);

        gridLayoutBottom->addWidget(pushButtonRecordStart, 0, 5, 1, 1);

        pushButtonRecordStop = new QPushButton(centralWidget);
        pushButtonRecordStop->setObjectName(QString::fromUtf8("pushButtonRecordStop"));
        pushButtonRecordStop->setEnabled(false);

        gridLayoutBottom->addWidget(pushButtonRecordStop, 0, 6, 1, 1);


        gridLayout_6->addLayout(gridLayoutBottom, 1, 0, 1, 1);

        GermCellSimulator->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(GermCellSimulator);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 862, 23));
        menuBar->setContextMenuPolicy(Qt::NoContextMenu);
        menuFile = new QMenu(menuBar);
        menuFile->setObjectName(QString::fromUtf8("menuFile"));
        menuFile->setContextMenuPolicy(Qt::NoContextMenu);
        GermCellSimulator->setMenuBar(menuBar);
        statusBar = new QStatusBar(GermCellSimulator);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        GermCellSimulator->setStatusBar(statusBar);
#ifndef QT_NO_SHORTCUT
        labelSpeed->setBuddy(horizontalSliderSpeed);
#endif // QT_NO_SHORTCUT

        menuBar->addAction(menuFile->menuAction());
        menuFile->addAction(action_New);
        menuFile->addSeparator();
        menuFile->addSeparator();
        menuFile->addAction(actionSave_CSV);
        menuFile->addAction(actionSave_Simulator);
        menuFile->addAction(actionSave_Position_Data);
        menuFile->addAction(actionSave_Errors);
        menuFile->addSeparator();
        menuFile->addAction(actionExit);

        retranslateUi(GermCellSimulator);
        QObject::connect(pushButtonStart, SIGNAL(clicked()), GermCellSimulator, SLOT(startSimulation()));
        QObject::connect(pushButtonPause, SIGNAL(clicked()), GermCellSimulator, SLOT(stopSimulation()));
        QObject::connect(horizontalSliderSpeed, SIGNAL(valueChanged(int)), GermCellSimulator, SLOT(changeSpeed(int)));
        QObject::connect(verticalSliderZoom, SIGNAL(sliderMoved(int)), GermCellSimulator, SLOT(changeZoom(int)));
        QObject::connect(actionExit, SIGNAL(triggered()), GermCellSimulator, SLOT(close()));
        QObject::connect(action_New, SIGNAL(triggered()), GermCellSimulator, SLOT(loadSimulation()));
        QObject::connect(actionSave_CSV, SIGNAL(triggered()), GermCellSimulator, SLOT(saveCSV()));
        QObject::connect(actionSave_Simulator, SIGNAL(triggered()), GermCellSimulator, SLOT(saveData()));
        QObject::connect(actionSave_Position_Data, SIGNAL(triggered()), GermCellSimulator, SLOT(savePosData()));
        QObject::connect(pushButtonRecordStart, SIGNAL(clicked()), GermCellSimulator, SLOT(startRecording()));
        QObject::connect(pushButtonRecordStop, SIGNAL(clicked()), GermCellSimulator, SLOT(stopRecording()));
        QObject::connect(actionSave_Errors, SIGNAL(triggered()), GermCellSimulator, SLOT(saveErrors()));

        QMetaObject::connectSlotsByName(GermCellSimulator);
    } // setupUi

    void retranslateUi(QMainWindow *GermCellSimulator)
    {
        GermCellSimulator->setWindowTitle(QApplication::translate("GermCellSimulator", "Germ Cell Simulator", 0, QApplication::UnicodeUTF8));
        action_New->setText(QApplication::translate("GermCellSimulator", "&New", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        action_New->setToolTip(QApplication::translate("GermCellSimulator", "Create a new simulation", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        action_New->setShortcut(QApplication::translate("GermCellSimulator", "Ctrl+N", 0, QApplication::UnicodeUTF8));
        actionExit->setText(QApplication::translate("GermCellSimulator", "E&xit", 0, QApplication::UnicodeUTF8));
        actionSave_CSV->setText(QApplication::translate("GermCellSimulator", "Save Population Stats", 0, QApplication::UnicodeUTF8));
        actionSave_Simulator->setText(QApplication::translate("GermCellSimulator", "Save Population Snapshot", 0, QApplication::UnicodeUTF8));
        actionSave_Position_Data->setText(QApplication::translate("GermCellSimulator", "Save Position Data", 0, QApplication::UnicodeUTF8));
        actionSave_Errors->setText(QApplication::translate("GermCellSimulator", "Save Errors", 0, QApplication::UnicodeUTF8));
        labelPlus->setText(QApplication::translate("GermCellSimulator", "+", 0, QApplication::UnicodeUTF8));
        labelMinus->setText(QApplication::translate("GermCellSimulator", "-", 0, QApplication::UnicodeUTF8));
        labelHour->setText(QApplication::translate("GermCellSimulator", "Hour", 0, QApplication::UnicodeUTF8));
        labelDay->setText(QApplication::translate("GermCellSimulator", "Day", 0, QApplication::UnicodeUTF8));
        labelZoom->setText(QApplication::translate("GermCellSimulator", "Zoom", 0, QApplication::UnicodeUTF8));
        labelSpeed->setText(QApplication::translate("GermCellSimulator", "Simulation Speed", 0, QApplication::UnicodeUTF8));
        pushButtonPause->setText(QApplication::translate("GermCellSimulator", "Pause", 0, QApplication::UnicodeUTF8));
        pushButtonStart->setText(QApplication::translate("GermCellSimulator", "Start", 0, QApplication::UnicodeUTF8));
        pushButtonRecordStart->setText(QApplication::translate("GermCellSimulator", "Start Recording", 0, QApplication::UnicodeUTF8));
        pushButtonRecordStop->setText(QApplication::translate("GermCellSimulator", "Stop Recording", 0, QApplication::UnicodeUTF8));
        menuFile->setTitle(QApplication::translate("GermCellSimulator", "File", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class GermCellSimulator: public Ui_GermCellSimulator {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_GERMCELLSIMULATOR_H
