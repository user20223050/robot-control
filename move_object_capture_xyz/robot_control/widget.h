#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QSerialPort>
#include <QString>
#include <iostream>
#include <qdebug.h>
#include "robot.h"

#define FOLLOW_TARGET 0 //0跟随移动目标 1跟随GET_MOVE中输入的目标

extern MPC_Control mpc;
extern Robot robot;
extern mobile_pose mobile;
extern Slave_Copmputer F407;
extern uint8_t follow_target;

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();

private slots:
    void on_LinkBt_clicked();

    void on_closeBt_clicked();

    void serialPortReadyRead_Slot();

    void on_Begin_followBt_clicked();

    void on_Stop_TrackBT_clicked();

    void on_Work_StateBt_clicked();

    void on_Init_StateBt_clicked();

private:
    Ui::Widget *ui;
};

#endif // WIDGET_H
