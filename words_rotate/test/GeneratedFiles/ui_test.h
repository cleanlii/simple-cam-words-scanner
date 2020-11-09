/********************************************************************************
** Form generated from reading UI file 'test.ui'
**
** Created by: Qt User Interface Compiler version 5.12.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_TEST_H
#define UI_TEST_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QScrollArea>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_testClass
{
public:
    QWidget *centralWidget;
    QLabel *label;
    QPushButton *pushButton;
    QPushButton *pushButton_2;
    QScrollArea *scrollArea;
    QWidget *scrollAreaWidgetContents;
    QScrollArea *scrollArea_2;
    QWidget *scrollAreaWidgetContents_2;
    QLabel *label_2;
    QLabel *label_3;
    QLabel *label_4;
    QSpinBox *spinBox;
    QSpinBox *spinBox_2;
    QLabel *label_5;
    QLabel *label_6;
    QPushButton *pushButton_3;
    QLabel *label_7;
    QPushButton *pushButton_4;
    QPushButton *pushButton_5;
    QPushButton *pushButton_6;
    QPushButton *pushButton_7;
    QLabel *label_8;
    QSpinBox *spinBox_3;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *testClass)
    {
        if (testClass->objectName().isEmpty())
            testClass->setObjectName(QString::fromUtf8("testClass"));
        testClass->resize(1066, 694);
        centralWidget = new QWidget(testClass);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        label = new QLabel(centralWidget);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(340, 90, 71, 41));
        QFont font;
        font.setFamily(QString::fromUtf8("Arial"));
        font.setPointSize(18);
        font.setBold(true);
        font.setWeight(75);
        label->setFont(font);
        pushButton = new QPushButton(centralWidget);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));
        pushButton->setGeometry(QRect(50, 180, 81, 41));
        pushButton_2 = new QPushButton(centralWidget);
        pushButton_2->setObjectName(QString::fromUtf8("pushButton_2"));
        pushButton_2->setGeometry(QRect(50, 260, 81, 41));
        scrollArea = new QScrollArea(centralWidget);
        scrollArea->setObjectName(QString::fromUtf8("scrollArea"));
        scrollArea->setGeometry(QRect(200, 160, 371, 321));
        scrollArea->setWidgetResizable(true);
        scrollAreaWidgetContents = new QWidget();
        scrollAreaWidgetContents->setObjectName(QString::fromUtf8("scrollAreaWidgetContents"));
        scrollAreaWidgetContents->setGeometry(QRect(0, 0, 369, 319));
        scrollArea->setWidget(scrollAreaWidgetContents);
        scrollArea_2 = new QScrollArea(centralWidget);
        scrollArea_2->setObjectName(QString::fromUtf8("scrollArea_2"));
        scrollArea_2->setGeometry(QRect(630, 160, 371, 321));
        scrollArea_2->setWidgetResizable(true);
        scrollAreaWidgetContents_2 = new QWidget();
        scrollAreaWidgetContents_2->setObjectName(QString::fromUtf8("scrollAreaWidgetContents_2"));
        scrollAreaWidgetContents_2->setGeometry(QRect(0, 0, 369, 319));
        scrollArea_2->setWidget(scrollAreaWidgetContents_2);
        label_2 = new QLabel(centralWidget);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(780, 90, 81, 41));
        label_2->setFont(font);
        label_3 = new QLabel(centralWidget);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setGeometry(QRect(50, 530, 81, 21));
        label_4 = new QLabel(centralWidget);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        label_4->setGeometry(QRect(50, 570, 81, 21));
        QFont font1;
        font1.setFamily(QString::fromUtf8("Arial"));
        font1.setPointSize(10);
        font1.setBold(false);
        font1.setWeight(50);
        label_4->setFont(font1);
        spinBox = new QSpinBox(centralWidget);
        spinBox->setObjectName(QString::fromUtf8("spinBox"));
        spinBox->setGeometry(QRect(150, 530, 61, 22));
        spinBox->setMinimum(1);
        spinBox->setMaximum(500);
        spinBox->setSingleStep(1);
        spinBox->setValue(150);
        spinBox_2 = new QSpinBox(centralWidget);
        spinBox_2->setObjectName(QString::fromUtf8("spinBox_2"));
        spinBox_2->setGeometry(QRect(150, 570, 61, 22));
        spinBox_2->setMinimum(1);
        spinBox_2->setMaximum(500);
        spinBox_2->setValue(90);
        label_5 = new QLabel(centralWidget);
        label_5->setObjectName(QString::fromUtf8("label_5"));
        label_5->setGeometry(QRect(220, 532, 211, 20));
        label_6 = new QLabel(centralWidget);
        label_6->setObjectName(QString::fromUtf8("label_6"));
        label_6->setGeometry(QRect(220, 570, 201, 20));
        pushButton_3 = new QPushButton(centralWidget);
        pushButton_3->setObjectName(QString::fromUtf8("pushButton_3"));
        pushButton_3->setGeometry(QRect(900, 540, 101, 41));
        label_7 = new QLabel(centralWidget);
        label_7->setObjectName(QString::fromUtf8("label_7"));
        label_7->setGeometry(QRect(40, 30, 141, 31));
        QFont font2;
        font2.setFamily(QString::fromUtf8("\345\276\256\350\275\257\351\233\205\351\273\221"));
        font2.setPointSize(11);
        label_7->setFont(font2);
        pushButton_4 = new QPushButton(centralWidget);
        pushButton_4->setObjectName(QString::fromUtf8("pushButton_4"));
        pushButton_4->setGeometry(QRect(290, 20, 81, 41));
        pushButton_5 = new QPushButton(centralWidget);
        pushButton_5->setObjectName(QString::fromUtf8("pushButton_5"));
        pushButton_5->setGeometry(QRect(180, 20, 81, 41));
        pushButton_6 = new QPushButton(centralWidget);
        pushButton_6->setObjectName(QString::fromUtf8("pushButton_6"));
        pushButton_6->setGeometry(QRect(50, 340, 81, 41));
        pushButton_7 = new QPushButton(centralWidget);
        pushButton_7->setObjectName(QString::fromUtf8("pushButton_7"));
        pushButton_7->setGeometry(QRect(50, 420, 81, 41));
        label_8 = new QLabel(centralWidget);
        label_8->setObjectName(QString::fromUtf8("label_8"));
        label_8->setGeometry(QRect(450, 550, 51, 21));
        spinBox_3 = new QSpinBox(centralWidget);
        spinBox_3->setObjectName(QString::fromUtf8("spinBox_3"));
        spinBox_3->setGeometry(QRect(510, 550, 61, 22));
        spinBox_3->setMinimum(-100);
        spinBox_3->setMaximum(300);
        spinBox_3->setValue(5);
        testClass->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(testClass);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 1066, 23));
        testClass->setMenuBar(menuBar);
        mainToolBar = new QToolBar(testClass);
        mainToolBar->setObjectName(QString::fromUtf8("mainToolBar"));
        testClass->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(testClass);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        testClass->setStatusBar(statusBar);

        retranslateUi(testClass);
        QObject::connect(pushButton, SIGNAL(clicked()), testClass, SLOT(on_OpenFig_clicked()));
        QObject::connect(pushButton_2, SIGNAL(clicked()), testClass, SLOT(on_Progress_clicked()));
        QObject::connect(pushButton_3, SIGNAL(clicked()), testClass, SLOT(on_Save_clicked()));
        QObject::connect(pushButton_4, SIGNAL(clicked()), testClass, SLOT(on_SaveAs_clicked()));
        QObject::connect(pushButton_5, SIGNAL(clicked()), testClass, SLOT(on_Clear_clicked()));
        QObject::connect(pushButton_6, SIGNAL(clicked()), testClass, SLOT(on_OCR_clicked()));
        QObject::connect(pushButton_7, SIGNAL(clicked()), testClass, SLOT(on_Sharpen_clicked()));

        QMetaObject::connectSlotsByName(testClass);
    } // setupUi

    void retranslateUi(QMainWindow *testClass)
    {
        testClass->setWindowTitle(QApplication::translate("testClass", "\346\226\207\345\255\227\347\272\240\346\255\243", nullptr));
        label->setText(QApplication::translate("testClass", "Origin", nullptr));
        pushButton->setText(QApplication::translate("testClass", "\346\211\223\345\274\200", nullptr));
        pushButton_2->setText(QApplication::translate("testClass", "\347\272\240\346\255\243", nullptr));
        label_2->setText(QApplication::translate("testClass", "Result", nullptr));
        label_3->setText(QApplication::translate("testClass", "\344\272\214\345\200\274\345\214\226\351\230\210\345\200\274", nullptr));
        label_4->setText(QApplication::translate("testClass", "\351\234\215\345\244\253\346\243\200\346\265\213\351\230\210\345\200\274", nullptr));
        label_5->setText(QApplication::translate("testClass", "\357\274\210\350\266\212\345\244\247\357\274\214\350\277\207\346\273\244\346\225\243\347\202\271\350\266\212\345\274\272\357\274\211", nullptr));
        label_6->setText(QApplication::translate("testClass", "\357\274\210\350\266\212\345\244\247\357\274\214\346\243\200\346\265\213\345\210\260\347\232\204\347\233\264\347\272\277\350\266\212\345\244\232\357\274\211", nullptr));
        pushButton_3->setText(QApplication::translate("testClass", "\344\277\235\345\255\230\350\256\276\347\275\256", nullptr));
        label_7->setText(QApplication::translate("testClass", "MADE BY CLEAN", nullptr));
        pushButton_4->setText(QApplication::translate("testClass", "\345\217\246\345\255\230\344\270\272", nullptr));
        pushButton_5->setText(QApplication::translate("testClass", "\346\270\205\347\251\272", nullptr));
        pushButton_6->setText(QApplication::translate("testClass", "OCR", nullptr));
        pushButton_7->setText(QApplication::translate("testClass", "\351\224\220\345\214\226\345\242\236\345\274\272", nullptr));
        label_8->setText(QApplication::translate("testClass", "\351\224\220\345\214\226\345\274\272\345\272\246", nullptr));
    } // retranslateUi

};

namespace Ui {
    class testClass: public Ui_testClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_TEST_H
