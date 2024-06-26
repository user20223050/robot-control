#include <QString>
#include <QSerialPortInfo>
#include<QMessageBox>
#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    QStringList serialNamePort;
    F407.serialPort = new QSerialPort(this);
    connect(F407.serialPort,SIGNAL(readyRead()),this,SLOT(serialPortReadyRead_Slot()));
    foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts()) {
        serialNamePort<<info.portName();
        ui->serialCb->addItems(serialNamePort);
    }
    ui->Get_MoveEdit->clear();
    QString Robot_Init = QString::number(270) + "," + QString::number(0) + "," + QString::number(275);
    ui->Get_MoveEdit->append(Robot_Init);
    ui->Joint1Slider->setValue(0);
    ui->Joint2Slider->setValue(-90);
    ui->Joint3Slider->setValue(-90);
    ui->Joint4Slider->setValue(0);
    ui->Joint5Slider->setValue(0);
    ui->Joint6Slider->setValue(0);
    ui->Joint1Edit->setText(QString::number(ui->Joint1Slider->value()));
    ui->Joint2Edit->setText(QString::number(ui->Joint2Slider->value()));
    ui->Joint3Edit->setText(QString::number(ui->Joint3Slider->value()));
    ui->Joint4Edit->setText(QString::number(ui->Joint4Slider->value()));
    ui->Joint5Edit->setText(QString::number(ui->Joint5Slider->value()));
    ui->Joint6Edit->setText(QString::number(ui->Joint6Slider->value()));
}

Widget::~Widget()
{
    F407.serialPort->write(F407.STOP,4);
    F407.stop_sign = 1;
//    mpc.Calculate_Out_X(0,robot,F407,mobile);
//    mpc.Calculate_Out_Y(0,robot,F407,mobile);
//    mpc.Calculate_Out_Z(0,robot,F407,mobile);
    delete ui;
}
/*50ms进入一次该程序*/
void Widget::serialPortReadyRead_Slot()
{
    static uint follow_num = 0;
    /*状态1机械臂进入工作状态或机械臂进入初始状态*/
    if(F407.state_sign == 1) {
        F407.Work_position_set(robot);//线性规划函数
        if(F407.Init_sign == 0) {
            QString init_sign = {"WORKST_INITING"};
            ui->MoveObjEdit->appendPlainText(init_sign);
            ui->recvEdit->appendPlainText(init_sign);
            ui->ErrorEdit->appendPlainText(init_sign);
        }
        if(F407.Init_sign == 1){
            QString init_sign = {"INITST_INITING"};
            ui->MoveObjEdit->appendPlainText(init_sign);
            ui->recvEdit->appendPlainText(init_sign);
            ui->ErrorEdit->appendPlainText(init_sign);
        }
        ui->Joint1Slider->setValue(robot.Joint_1_angle_R*180/(M_PI));
        ui->Joint2Slider->setValue(robot.Joint_2_angle_R*180/(M_PI));
        ui->Joint3Slider->setValue(robot.Joint_3_angle_R*180/(M_PI));
        ui->Joint4Slider->setValue(robot.Joint_4_angle_R*180/(M_PI));
        ui->Joint5Slider->setValue(robot.Joint_5_angle_R*180/(M_PI));
        ui->Joint6Slider->setValue(robot.Joint_6_angle_R*180/(M_PI));
        ui->Joint1Edit->setText(QString::number(ui->Joint1Slider->value()));
        ui->Joint2Edit->setText(QString::number(ui->Joint2Slider->value()));
        ui->Joint3Edit->setText(QString::number(ui->Joint3Slider->value()));
        ui->Joint4Edit->setText(QString::number(ui->Joint4Slider->value()));
        ui->Joint5Edit->setText(QString::number(ui->Joint5Slider->value()));
        ui->Joint6Edit->setText(QString::number(ui->Joint6Slider->value()));
    }
    /*状态2达到工作状态或达到初始状态*/
    if(F407.state_sign == 2) {
        ui->recvEdit->clear();
        ui->MoveObjEdit->clear();
        ui->ErrorEdit->clear();
        if(F407.Init_sign == 0) {
            QString init_sign = {"WORKST OK"};
            ui->MoveObjEdit->appendPlainText(init_sign);
            ui->recvEdit->appendPlainText(init_sign);
            ui->ErrorEdit->appendPlainText(init_sign);
//            F407.state_sign = 3;
            F407.Work_sign = 0;
            follow_num = 0;
            F407.serialPort->write(F407.STOP,4);
            F407.stop_sign = 1;
            mpc.Calculate_Out_X(0,robot,F407,mobile);
            mpc.Calculate_Out_Y(0,robot,F407,mobile);
            mpc.Calculate_Out_Z(0,robot,F407,mobile);
//            mpc.Calculate_Out_J4(0,robot,mobile);
//            mpc.Calculate_Out_J5(0,robot,mobile);
//            mpc.Calculate_Out_J6(0,robot,mobile);
        }
        if(F407.Init_sign == 1) {
            QString init_sign = {"INIT OK"};
            ui->MoveObjEdit->appendPlainText(init_sign);
            ui->recvEdit->appendPlainText(init_sign);
            ui->ErrorEdit->appendPlainText(init_sign);
            F407.state_sign = 1;
            F407.Init_sign = 0;
            follow_num = 0;
            F407.serialPort->write(F407.STOP,4);
            F407.stop_sign = 1;
            mpc.Calculate_Out_X(0,robot,F407,mobile);
            mpc.Calculate_Out_Y(0,robot,F407,mobile);
            mpc.Calculate_Out_Z(0,robot,F407,mobile);
//            mpc.Calculate_Out_J4(0,robot,mobile);
//            mpc.Calculate_Out_J5(0,robot,mobile);
//            mpc.Calculate_Out_J6(0,robot,mobile);
        }
    }
    /*状态3跟随物体状态*/
    if(F407.state_sign == 3) {
        QString Get_Move = ui->Get_MoveEdit->toPlainText();
        QStringList strList = Get_Move.split(",");
        F407.Follow_Mobj(mpc,robot,mobile);
        QString MoveObj = QString::number(follow_num) + ":" + "X" + ":" + QString::number(round(mobile.X_point_OBJ_Real*1)/1) + "," + "Y" + ":" + QString::number(round(mobile.Y_point_OBJ_Real*1)/1) + "," + "Z" + ":" + QString::number(round(mobile.Z_point_OBJ_Real*1)/1);
        ui->MoveObjEdit->appendPlainText(MoveObj);
        QString robot_tip = QString::number(follow_num) + ":" + "X" + ":" + QString::number(round(robot.X_tool_point_R*1)/1) + "," + "Y" + ":" + QString::number(round(robot.Y_tool_point_R*1)/1) + "," + "Z" + ":" + QString::number(round(robot.Z_tool_point_R*1)/1);
        ui->recvEdit->appendPlainText(robot_tip);
//        ui->ErrorEdit->appendPlainText(QString::number(follow_num) + ":" + "Xerror" + ":" + QString::number(mpc.X_Error(0,0)) + "," + "Yerror" + ":" + QString::number(mpc.Y_Error(0,0)) + "," + "Zerror" + ":" + QString::number(mpc.Z_Error(0,0))+ "," + "XAerror" + ":" + QString::number(mpc.J4_Error(0,0)) + "," + "YAerror" + ":" + QString::number(mpc.J5_Error(0,0)) + "," + "ZAerror" + ":" + QString::number(mpc.J6_Error(0,0)));
        ui->ErrorEdit->appendPlainText(QString::number(follow_num) + ":" + "Xerror" + ":" + QString::number(round(mobile.X_point_OBJ_Real*1)/1 - round(robot.X_tool_point_R*1)/1) + "," + "Yerror" + ":" + QString::number(round(mobile.Y_point_OBJ_Real*1)/1 - round(robot.Y_tool_point_R*1)/1) + "," + "Zerror" + ":" + QString::number(round(mobile.Z_point_OBJ_Real*1)/1 - round(robot.Z_tool_point_R*1)/1));
        ui->Joint1Slider->setValue(robot.Joint_1_angle_R*180/(M_PI));
        ui->Joint2Slider->setValue(robot.Joint_2_angle_R*180/(M_PI));
        ui->Joint3Slider->setValue(robot.Joint_3_angle_R*180/(M_PI));
        ui->Joint4Slider->setValue(robot.Joint_4_angle_R*180/(M_PI));
        ui->Joint5Slider->setValue(robot.Joint_5_angle_R*180/(M_PI));
        ui->Joint6Slider->setValue(robot.Joint_6_angle_R*180/(M_PI));
        ui->Joint1Edit->setText(QString::number(ui->Joint1Slider->value()));
        ui->Joint2Edit->setText(QString::number(ui->Joint2Slider->value()));
        ui->Joint3Edit->setText(QString::number(ui->Joint3Slider->value()));
        ui->Joint4Edit->setText(QString::number(ui->Joint4Slider->value()));
        ui->Joint5Edit->setText(QString::number(ui->Joint5Slider->value()));
        ui->Joint6Edit->setText(QString::number(ui->Joint6Slider->value()));
        follow_num++;
    }
}

void Widget::on_LinkBt_clicked()
{
   F407.serialPort->setPortName(ui->serialCb->currentText());
   F407.serialPort->setBaudRate(QSerialPort::Baud460800);
   F407.serialPort->setDataBits(QSerialPort::Data8);
   F407.serialPort->setStopBits(QSerialPort::OneStop);
   F407.serialPort->setParity(QSerialPort::NoParity);
   if(F407.serialPort->open(QIODevice::ReadWrite) == true)
      QMessageBox::information(this,"hint","OK");
   else
      QMessageBox::critical(this,"hint","NO");
}

void Widget::on_closeBt_clicked()
{
    F407.serialPort->write(F407.STOP,4);
    F407.stop_sign = 1;
    mpc.Calculate_Out_X(0,robot,F407,mobile);
    mpc.Calculate_Out_Y(0,robot,F407,mobile);
    mpc.Calculate_Out_Z(0,robot,F407,mobile);
    F407.serialPort->close();
}

void Widget::on_Begin_followBt_clicked()
{
        F407.state_sign = 3;
        F407.serialPort->write(F407.Start_T,4);
        F407.stop_sign = 0;
        mpc.limit = 0;
}

void Widget::on_Stop_TrackBT_clicked()
{
    F407.serialPort->write(F407.STOP,4);
}

void Widget::on_Work_StateBt_clicked()
{
    static uint8_t sign = 0;
    F407.state_sign = 1;
    if(sign == 1) {
        robot.Linear_Speed_Plan(0,robot.Joint_1_angle_R,robot.joint1_buf);
        robot.Linear_Speed_Plan(-M_PI/2,robot.Joint_2_angle_R,robot.joint2_buf);
        robot.Linear_Speed_Plan(0,robot.Joint_3_angle_R,robot.joint3_buf);
        robot.Linear_Speed_Plan(0,robot.Joint_4_angle_R,robot.joint4_buf);
        robot.Linear_Speed_Plan(0,robot.Joint_5_angle_R,robot.joint5_buf);
        robot.Linear_Speed_Plan(0,robot.Joint_6_angle_R,robot.joint6_buf);
    }
    sign = 1;
    F407.serialPort->write(F407.Start_T,4);
    mpc.limit = 0;
    robot.errror_position = 0;
}

void Widget::on_Init_StateBt_clicked()
{
    F407.state_sign = 1;
    F407.Init_sign = 1;
    robot.Linear_Speed_Plan(0,robot.Joint_1_angle_R,robot.joint1_buf);
    robot.Linear_Speed_Plan(-M_PI/2,robot.Joint_2_angle_R,robot.joint2_buf);
    robot.Linear_Speed_Plan(-M_PI/2,robot.Joint_3_angle_R,robot.joint3_buf);
    robot.Linear_Speed_Plan(0,robot.Joint_4_angle_R,robot.joint4_buf);
    robot.Linear_Speed_Plan(0,robot.Joint_5_angle_R,robot.joint5_buf);
    robot.Linear_Speed_Plan(0,robot.Joint_6_angle_R,robot.joint6_buf);
    F407.serialPort->write(F407.Start_T,4);
}
