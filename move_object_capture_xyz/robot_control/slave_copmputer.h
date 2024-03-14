#ifndef SLAVE_COPMPUTER_H
#define SLAVE_COPMPUTER_H

#include <iostream>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QDebug>
#include "robot.h"

class Robot;
class Widget;
class MPC_Control;
class mobile_pose;

using namespace std;

class Slave_Copmputer
{
public:
    //定时器ARR的值
    uint pulse1;
    uint pulse2;
    uint pulse3;
    uint pulse4;
    uint pulse5;
    uint pulse6;
    //关节旋转方向
    bool Joint1_rotate_direction;
    bool Joint2_rotate_direction;
    bool Joint3_rotate_direction;
    bool Joint4_rotate_direction;
    bool Joint5_rotate_direction;
    bool Joint6_rotate_direction;
    //串口发送数据
    char *Start_T;
    char *buffer1;
    char *buffer2;
    char *buffer3;
    char *buffer4;
    char *buffer5;
    char *buffer6;
    char *STOP;
    char *ERROR;
    //串口通讯
    QSerialPort *serialPort;
    //状态标志位
    uint8_t state_sign;
    uint8_t Work_sign;
    uint8_t Init_sign;
    uint8_t stop_sign;

public:
    Slave_Copmputer();
    void calculate_pulse(Robot &robot);
    void joint_rotate_direction(Robot &robot);
    void Serial_message(Robot &robot);
    void Follow_Mobj(MPC_Control& mpc,Robot &robot,mobile_pose &mobile);
    void Work_position_set(Robot &robot);
    void CRC_cheak(char* buf);


};

#endif // SLAVE_COPMPUTER_H
