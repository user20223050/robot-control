/********************************************************************************
** Form generated from reading UI file 'widget.ui'
**
** Created by: Qt User Interface Compiler version 5.8.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_WIDGET_H
#define UI_WIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPlainTextEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSlider>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Widget
{
public:
    QGridLayout *gridLayout_2;
    QGridLayout *gridLayout;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout;
    QPlainTextEdit *recvEdit;
    QPlainTextEdit *MoveObjEdit;
    QPlainTextEdit *ErrorEdit;
    QTextEdit *Get_MoveEdit;
    QVBoxLayout *verticalLayout_3;
    QPushButton *LinkBt;
    QSpacerItem *verticalSpacer_2;
    QPushButton *closeBt;
    QSpacerItem *verticalSpacer_3;
    QPushButton *Begin_followBt;
    QSpacerItem *verticalSpacer_4;
    QPushButton *Stop_TrackBT;
    QSpacerItem *verticalSpacer_5;
    QPushButton *Work_StateBt;
    QSpacerItem *verticalSpacer_6;
    QPushButton *Init_StateBt;
    QSpacerItem *verticalSpacer;
    QHBoxLayout *horizontalLayout_8;
    QComboBox *serialCb;
    QLabel *label_19;
    QVBoxLayout *verticalLayout_2;
    QHBoxLayout *horizontalLayout_2;
    QLabel *label;
    QSlider *Joint1Slider;
    QLabel *label_2;
    QLineEdit *Joint1Edit;
    QSpacerItem *verticalSpacer_7;
    QHBoxLayout *horizontalLayout_3;
    QLabel *label_3;
    QSlider *Joint2Slider;
    QLabel *label_4;
    QLineEdit *Joint2Edit;
    QSpacerItem *verticalSpacer_8;
    QHBoxLayout *horizontalLayout_4;
    QLabel *label_6;
    QSlider *Joint3Slider;
    QLabel *label_5;
    QLineEdit *Joint3Edit;
    QSpacerItem *verticalSpacer_11;
    QHBoxLayout *horizontalLayout_5;
    QLabel *label_9;
    QSlider *Joint4Slider;
    QLabel *label_12;
    QLineEdit *Joint4Edit;
    QSpacerItem *verticalSpacer_9;
    QHBoxLayout *horizontalLayout_6;
    QLabel *label_7;
    QSlider *Joint5Slider;
    QLabel *label_8;
    QLineEdit *Joint5Edit;
    QSpacerItem *verticalSpacer_10;
    QHBoxLayout *horizontalLayout_7;
    QLabel *label_11;
    QSlider *Joint6Slider;
    QLabel *label_10;
    QLineEdit *Joint6Edit;

    void setupUi(QWidget *Widget)
    {
        if (Widget->objectName().isEmpty())
            Widget->setObjectName(QStringLiteral("Widget"));
        Widget->resize(898, 649);
        gridLayout_2 = new QGridLayout(Widget);
        gridLayout_2->setSpacing(6);
        gridLayout_2->setContentsMargins(11, 11, 11, 11);
        gridLayout_2->setObjectName(QStringLiteral("gridLayout_2"));
        gridLayout = new QGridLayout();
        gridLayout->setSpacing(6);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        verticalLayout = new QVBoxLayout();
        verticalLayout->setSpacing(6);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        recvEdit = new QPlainTextEdit(Widget);
        recvEdit->setObjectName(QStringLiteral("recvEdit"));
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(recvEdit->sizePolicy().hasHeightForWidth());
        recvEdit->setSizePolicy(sizePolicy);
        recvEdit->setReadOnly(true);

        horizontalLayout->addWidget(recvEdit);

        MoveObjEdit = new QPlainTextEdit(Widget);
        MoveObjEdit->setObjectName(QStringLiteral("MoveObjEdit"));
        sizePolicy.setHeightForWidth(MoveObjEdit->sizePolicy().hasHeightForWidth());
        MoveObjEdit->setSizePolicy(sizePolicy);
        MoveObjEdit->setReadOnly(true);

        horizontalLayout->addWidget(MoveObjEdit);

        ErrorEdit = new QPlainTextEdit(Widget);
        ErrorEdit->setObjectName(QStringLiteral("ErrorEdit"));
        ErrorEdit->setReadOnly(true);

        horizontalLayout->addWidget(ErrorEdit);


        verticalLayout->addLayout(horizontalLayout);

        Get_MoveEdit = new QTextEdit(Widget);
        Get_MoveEdit->setObjectName(QStringLiteral("Get_MoveEdit"));
        sizePolicy.setHeightForWidth(Get_MoveEdit->sizePolicy().hasHeightForWidth());
        Get_MoveEdit->setSizePolicy(sizePolicy);
        Get_MoveEdit->setMaximumSize(QSize(1000000, 45));

        verticalLayout->addWidget(Get_MoveEdit);


        gridLayout->addLayout(verticalLayout, 0, 0, 1, 1);

        verticalLayout_3 = new QVBoxLayout();
        verticalLayout_3->setSpacing(6);
        verticalLayout_3->setObjectName(QStringLiteral("verticalLayout_3"));
        LinkBt = new QPushButton(Widget);
        LinkBt->setObjectName(QStringLiteral("LinkBt"));
        sizePolicy.setHeightForWidth(LinkBt->sizePolicy().hasHeightForWidth());
        LinkBt->setSizePolicy(sizePolicy);
        LinkBt->setMaximumSize(QSize(140, 16777215));

        verticalLayout_3->addWidget(LinkBt);

        verticalSpacer_2 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_3->addItem(verticalSpacer_2);

        closeBt = new QPushButton(Widget);
        closeBt->setObjectName(QStringLiteral("closeBt"));
        sizePolicy.setHeightForWidth(closeBt->sizePolicy().hasHeightForWidth());
        closeBt->setSizePolicy(sizePolicy);
        closeBt->setMaximumSize(QSize(140, 16777215));

        verticalLayout_3->addWidget(closeBt);

        verticalSpacer_3 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_3->addItem(verticalSpacer_3);

        Begin_followBt = new QPushButton(Widget);
        Begin_followBt->setObjectName(QStringLiteral("Begin_followBt"));
        sizePolicy.setHeightForWidth(Begin_followBt->sizePolicy().hasHeightForWidth());
        Begin_followBt->setSizePolicy(sizePolicy);
        Begin_followBt->setMaximumSize(QSize(140, 16777215));

        verticalLayout_3->addWidget(Begin_followBt);

        verticalSpacer_4 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_3->addItem(verticalSpacer_4);

        Stop_TrackBT = new QPushButton(Widget);
        Stop_TrackBT->setObjectName(QStringLiteral("Stop_TrackBT"));
        sizePolicy.setHeightForWidth(Stop_TrackBT->sizePolicy().hasHeightForWidth());
        Stop_TrackBT->setSizePolicy(sizePolicy);
        Stop_TrackBT->setMaximumSize(QSize(140, 16777215));

        verticalLayout_3->addWidget(Stop_TrackBT);

        verticalSpacer_5 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_3->addItem(verticalSpacer_5);

        Work_StateBt = new QPushButton(Widget);
        Work_StateBt->setObjectName(QStringLiteral("Work_StateBt"));
        sizePolicy.setHeightForWidth(Work_StateBt->sizePolicy().hasHeightForWidth());
        Work_StateBt->setSizePolicy(sizePolicy);
        Work_StateBt->setMaximumSize(QSize(140, 16777215));

        verticalLayout_3->addWidget(Work_StateBt);

        verticalSpacer_6 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_3->addItem(verticalSpacer_6);

        Init_StateBt = new QPushButton(Widget);
        Init_StateBt->setObjectName(QStringLiteral("Init_StateBt"));
        QSizePolicy sizePolicy1(QSizePolicy::Minimum, QSizePolicy::Expanding);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(Init_StateBt->sizePolicy().hasHeightForWidth());
        Init_StateBt->setSizePolicy(sizePolicy1);
        Init_StateBt->setMaximumSize(QSize(140, 16777215));

        verticalLayout_3->addWidget(Init_StateBt);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_3->addItem(verticalSpacer);

        horizontalLayout_8 = new QHBoxLayout();
        horizontalLayout_8->setSpacing(6);
        horizontalLayout_8->setObjectName(QStringLiteral("horizontalLayout_8"));
        serialCb = new QComboBox(Widget);
        serialCb->setObjectName(QStringLiteral("serialCb"));
        QSizePolicy sizePolicy2(QSizePolicy::Expanding, QSizePolicy::Fixed);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(serialCb->sizePolicy().hasHeightForWidth());
        serialCb->setSizePolicy(sizePolicy2);
        serialCb->setMaximumSize(QSize(70, 62));

        horizontalLayout_8->addWidget(serialCb);

        label_19 = new QLabel(Widget);
        label_19->setObjectName(QStringLiteral("label_19"));
        sizePolicy2.setHeightForWidth(label_19->sizePolicy().hasHeightForWidth());
        label_19->setSizePolicy(sizePolicy2);
        label_19->setMaximumSize(QSize(62, 62));
        QFont font;
        font.setPointSize(10);
        label_19->setFont(font);

        horizontalLayout_8->addWidget(label_19);


        verticalLayout_3->addLayout(horizontalLayout_8);


        gridLayout->addLayout(verticalLayout_3, 0, 1, 2, 1);

        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setSpacing(6);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        label = new QLabel(Widget);
        label->setObjectName(QStringLiteral("label"));
        QFont font1;
        font1.setPointSize(12);
        label->setFont(font1);

        horizontalLayout_2->addWidget(label);

        Joint1Slider = new QSlider(Widget);
        Joint1Slider->setObjectName(QStringLiteral("Joint1Slider"));
        QSizePolicy sizePolicy3(QSizePolicy::Expanding, QSizePolicy::Maximum);
        sizePolicy3.setHorizontalStretch(0);
        sizePolicy3.setVerticalStretch(0);
        sizePolicy3.setHeightForWidth(Joint1Slider->sizePolicy().hasHeightForWidth());
        Joint1Slider->setSizePolicy(sizePolicy3);
        Joint1Slider->setMaximumSize(QSize(16777215, 40));
        Joint1Slider->setMinimum(-180);
        Joint1Slider->setMaximum(180);
        Joint1Slider->setOrientation(Qt::Horizontal);

        horizontalLayout_2->addWidget(Joint1Slider);

        label_2 = new QLabel(Widget);
        label_2->setObjectName(QStringLiteral("label_2"));
        QSizePolicy sizePolicy4(QSizePolicy::Preferred, QSizePolicy::Fixed);
        sizePolicy4.setHorizontalStretch(0);
        sizePolicy4.setVerticalStretch(0);
        sizePolicy4.setHeightForWidth(label_2->sizePolicy().hasHeightForWidth());
        label_2->setSizePolicy(sizePolicy4);
        label_2->setFont(font1);

        horizontalLayout_2->addWidget(label_2);

        Joint1Edit = new QLineEdit(Widget);
        Joint1Edit->setObjectName(QStringLiteral("Joint1Edit"));
        QSizePolicy sizePolicy5(QSizePolicy::Maximum, QSizePolicy::Maximum);
        sizePolicy5.setHorizontalStretch(0);
        sizePolicy5.setVerticalStretch(0);
        sizePolicy5.setHeightForWidth(Joint1Edit->sizePolicy().hasHeightForWidth());
        Joint1Edit->setSizePolicy(sizePolicy5);
        Joint1Edit->setMaximumSize(QSize(40, 40));

        horizontalLayout_2->addWidget(Joint1Edit);


        verticalLayout_2->addLayout(horizontalLayout_2);

        verticalSpacer_7 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Fixed);

        verticalLayout_2->addItem(verticalSpacer_7);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setSpacing(6);
        horizontalLayout_3->setObjectName(QStringLiteral("horizontalLayout_3"));
        label_3 = new QLabel(Widget);
        label_3->setObjectName(QStringLiteral("label_3"));
        label_3->setFont(font1);

        horizontalLayout_3->addWidget(label_3);

        Joint2Slider = new QSlider(Widget);
        Joint2Slider->setObjectName(QStringLiteral("Joint2Slider"));
        sizePolicy3.setHeightForWidth(Joint2Slider->sizePolicy().hasHeightForWidth());
        Joint2Slider->setSizePolicy(sizePolicy3);
        Joint2Slider->setMaximumSize(QSize(16777215, 40));
        Joint2Slider->setLayoutDirection(Qt::LeftToRight);
        Joint2Slider->setMinimum(-180);
        Joint2Slider->setMaximum(180);
        Joint2Slider->setValue(0);
        Joint2Slider->setOrientation(Qt::Horizontal);
        Joint2Slider->setInvertedAppearance(false);

        horizontalLayout_3->addWidget(Joint2Slider);

        label_4 = new QLabel(Widget);
        label_4->setObjectName(QStringLiteral("label_4"));
        label_4->setFont(font1);

        horizontalLayout_3->addWidget(label_4);

        Joint2Edit = new QLineEdit(Widget);
        Joint2Edit->setObjectName(QStringLiteral("Joint2Edit"));
        sizePolicy5.setHeightForWidth(Joint2Edit->sizePolicy().hasHeightForWidth());
        Joint2Edit->setSizePolicy(sizePolicy5);
        Joint2Edit->setMaximumSize(QSize(40, 40));

        horizontalLayout_3->addWidget(Joint2Edit);


        verticalLayout_2->addLayout(horizontalLayout_3);

        verticalSpacer_8 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Fixed);

        verticalLayout_2->addItem(verticalSpacer_8);

        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setSpacing(6);
        horizontalLayout_4->setObjectName(QStringLiteral("horizontalLayout_4"));
        label_6 = new QLabel(Widget);
        label_6->setObjectName(QStringLiteral("label_6"));
        label_6->setFont(font1);

        horizontalLayout_4->addWidget(label_6);

        Joint3Slider = new QSlider(Widget);
        Joint3Slider->setObjectName(QStringLiteral("Joint3Slider"));
        sizePolicy3.setHeightForWidth(Joint3Slider->sizePolicy().hasHeightForWidth());
        Joint3Slider->setSizePolicy(sizePolicy3);
        Joint3Slider->setMaximumSize(QSize(16777215, 40));
        Joint3Slider->setMinimum(-180);
        Joint3Slider->setMaximum(180);
        Joint3Slider->setOrientation(Qt::Horizontal);

        horizontalLayout_4->addWidget(Joint3Slider);

        label_5 = new QLabel(Widget);
        label_5->setObjectName(QStringLiteral("label_5"));
        label_5->setFont(font1);

        horizontalLayout_4->addWidget(label_5);

        Joint3Edit = new QLineEdit(Widget);
        Joint3Edit->setObjectName(QStringLiteral("Joint3Edit"));
        sizePolicy5.setHeightForWidth(Joint3Edit->sizePolicy().hasHeightForWidth());
        Joint3Edit->setSizePolicy(sizePolicy5);
        Joint3Edit->setMaximumSize(QSize(40, 40));

        horizontalLayout_4->addWidget(Joint3Edit);


        verticalLayout_2->addLayout(horizontalLayout_4);

        verticalSpacer_11 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Fixed);

        verticalLayout_2->addItem(verticalSpacer_11);

        horizontalLayout_5 = new QHBoxLayout();
        horizontalLayout_5->setSpacing(6);
        horizontalLayout_5->setObjectName(QStringLiteral("horizontalLayout_5"));
        label_9 = new QLabel(Widget);
        label_9->setObjectName(QStringLiteral("label_9"));
        label_9->setFont(font1);

        horizontalLayout_5->addWidget(label_9);

        Joint4Slider = new QSlider(Widget);
        Joint4Slider->setObjectName(QStringLiteral("Joint4Slider"));
        sizePolicy3.setHeightForWidth(Joint4Slider->sizePolicy().hasHeightForWidth());
        Joint4Slider->setSizePolicy(sizePolicy3);
        Joint4Slider->setMaximumSize(QSize(16777215, 40));
        Joint4Slider->setMinimum(-360);
        Joint4Slider->setMaximum(360);
        Joint4Slider->setOrientation(Qt::Horizontal);

        horizontalLayout_5->addWidget(Joint4Slider);

        label_12 = new QLabel(Widget);
        label_12->setObjectName(QStringLiteral("label_12"));
        sizePolicy4.setHeightForWidth(label_12->sizePolicy().hasHeightForWidth());
        label_12->setSizePolicy(sizePolicy4);
        label_12->setFont(font1);

        horizontalLayout_5->addWidget(label_12);

        Joint4Edit = new QLineEdit(Widget);
        Joint4Edit->setObjectName(QStringLiteral("Joint4Edit"));
        sizePolicy5.setHeightForWidth(Joint4Edit->sizePolicy().hasHeightForWidth());
        Joint4Edit->setSizePolicy(sizePolicy5);
        Joint4Edit->setMaximumSize(QSize(40, 40));

        horizontalLayout_5->addWidget(Joint4Edit);


        verticalLayout_2->addLayout(horizontalLayout_5);

        verticalSpacer_9 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Fixed);

        verticalLayout_2->addItem(verticalSpacer_9);

        horizontalLayout_6 = new QHBoxLayout();
        horizontalLayout_6->setSpacing(6);
        horizontalLayout_6->setObjectName(QStringLiteral("horizontalLayout_6"));
        label_7 = new QLabel(Widget);
        label_7->setObjectName(QStringLiteral("label_7"));
        label_7->setFont(font1);

        horizontalLayout_6->addWidget(label_7);

        Joint5Slider = new QSlider(Widget);
        Joint5Slider->setObjectName(QStringLiteral("Joint5Slider"));
        sizePolicy3.setHeightForWidth(Joint5Slider->sizePolicy().hasHeightForWidth());
        Joint5Slider->setSizePolicy(sizePolicy3);
        Joint5Slider->setMaximumSize(QSize(16777215, 40));
        Joint5Slider->setLayoutDirection(Qt::LeftToRight);
        Joint5Slider->setMinimum(-360);
        Joint5Slider->setMaximum(360);
        Joint5Slider->setValue(0);
        Joint5Slider->setOrientation(Qt::Horizontal);
        Joint5Slider->setInvertedAppearance(false);

        horizontalLayout_6->addWidget(Joint5Slider);

        label_8 = new QLabel(Widget);
        label_8->setObjectName(QStringLiteral("label_8"));
        label_8->setFont(font1);

        horizontalLayout_6->addWidget(label_8);

        Joint5Edit = new QLineEdit(Widget);
        Joint5Edit->setObjectName(QStringLiteral("Joint5Edit"));
        sizePolicy5.setHeightForWidth(Joint5Edit->sizePolicy().hasHeightForWidth());
        Joint5Edit->setSizePolicy(sizePolicy5);
        Joint5Edit->setMaximumSize(QSize(40, 40));

        horizontalLayout_6->addWidget(Joint5Edit);


        verticalLayout_2->addLayout(horizontalLayout_6);

        verticalSpacer_10 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Fixed);

        verticalLayout_2->addItem(verticalSpacer_10);

        horizontalLayout_7 = new QHBoxLayout();
        horizontalLayout_7->setSpacing(6);
        horizontalLayout_7->setObjectName(QStringLiteral("horizontalLayout_7"));
        label_11 = new QLabel(Widget);
        label_11->setObjectName(QStringLiteral("label_11"));
        label_11->setFont(font1);

        horizontalLayout_7->addWidget(label_11);

        Joint6Slider = new QSlider(Widget);
        Joint6Slider->setObjectName(QStringLiteral("Joint6Slider"));
        sizePolicy3.setHeightForWidth(Joint6Slider->sizePolicy().hasHeightForWidth());
        Joint6Slider->setSizePolicy(sizePolicy3);
        Joint6Slider->setMaximumSize(QSize(16777215, 40));
        Joint6Slider->setMinimum(-360);
        Joint6Slider->setMaximum(360);
        Joint6Slider->setOrientation(Qt::Horizontal);

        horizontalLayout_7->addWidget(Joint6Slider);

        label_10 = new QLabel(Widget);
        label_10->setObjectName(QStringLiteral("label_10"));
        label_10->setFont(font1);

        horizontalLayout_7->addWidget(label_10);

        Joint6Edit = new QLineEdit(Widget);
        Joint6Edit->setObjectName(QStringLiteral("Joint6Edit"));
        sizePolicy5.setHeightForWidth(Joint6Edit->sizePolicy().hasHeightForWidth());
        Joint6Edit->setSizePolicy(sizePolicy5);
        Joint6Edit->setMaximumSize(QSize(40, 40));

        horizontalLayout_7->addWidget(Joint6Edit);


        verticalLayout_2->addLayout(horizontalLayout_7);


        gridLayout->addLayout(verticalLayout_2, 1, 0, 1, 1);


        gridLayout_2->addLayout(gridLayout, 0, 0, 1, 1);


        retranslateUi(Widget);

        QMetaObject::connectSlotsByName(Widget);
    } // setupUi

    void retranslateUi(QWidget *Widget)
    {
        Widget->setWindowTitle(QApplication::translate("Widget", "Widget", Q_NULLPTR));
        LinkBt->setText(QApplication::translate("Widget", "\344\270\262\345\217\243\350\277\236\346\216\245", Q_NULLPTR));
        closeBt->setText(QApplication::translate("Widget", "\344\270\262\345\217\243\346\226\255\345\274\200", Q_NULLPTR));
        Begin_followBt->setText(QApplication::translate("Widget", "\345\274\200\345\247\213\350\267\237\350\270\252", Q_NULLPTR));
        Stop_TrackBT->setText(QApplication::translate("Widget", "\345\201\234\346\255\242\350\267\237\350\270\252", Q_NULLPTR));
        Work_StateBt->setText(QApplication::translate("Widget", "\345\267\245\344\275\234\347\212\266\346\200\201", Q_NULLPTR));
        Init_StateBt->setText(QApplication::translate("Widget", "\345\210\235\345\247\213\347\212\266\346\200\201", Q_NULLPTR));
        label_19->setText(QApplication::translate("Widget", "\344\270\262\345\217\243\345\217\267", Q_NULLPTR));
        label->setText(QApplication::translate("Widget", "-180\302\260", Q_NULLPTR));
        label_2->setText(QApplication::translate("Widget", "180\302\260", Q_NULLPTR));
        label_3->setText(QApplication::translate("Widget", "-180\302\260", Q_NULLPTR));
        label_4->setText(QApplication::translate("Widget", "180\302\260", Q_NULLPTR));
        label_6->setText(QApplication::translate("Widget", "-180\302\260", Q_NULLPTR));
        label_5->setText(QApplication::translate("Widget", "180\302\260", Q_NULLPTR));
        label_9->setText(QApplication::translate("Widget", "-180\302\260", Q_NULLPTR));
        label_12->setText(QApplication::translate("Widget", "180\302\260", Q_NULLPTR));
        label_7->setText(QApplication::translate("Widget", "-180\302\260", Q_NULLPTR));
        label_8->setText(QApplication::translate("Widget", "180\302\260", Q_NULLPTR));
        label_11->setText(QApplication::translate("Widget", "-180\302\260", Q_NULLPTR));
        label_10->setText(QApplication::translate("Widget", "180\302\260", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class Widget: public Ui_Widget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_WIDGET_H
