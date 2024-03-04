#ifndef ROBOT_H
#define ROBOT_H

#include <iostream>
#include <complex>
#include <cmath>
#include <QMatrix4x4>
#include <Dense>
#include "mobile_pose.h"
#include "mpc_control.h"
#include "slave_copmputer.h"
#include "widget.h"

using namespace std;
using namespace Eigen;

class Robot
{
public:
    //计算关节角度
    double Joint_1_angle_C;
    double Joint_2_angle_C;
    double Joint_3_angle_C;
    double Joint_4_angle_C;
    double Joint_5_angle_C;
    double Joint_6_angle_C;
    //实际关节角度
    double Joint_1_angle_R;
    double Joint_2_angle_R;
    double Joint_3_angle_R;
    double Joint_4_angle_R;
    double Joint_5_angle_R;
    double Joint_6_angle_R;
    //实际关节角速度
    float Joint_1_speed_R;
    float Joint_2_speed_R;
    float Joint_3_speed_R;
    float Joint_4_speed_R;
    float Joint_5_speed_R;
    float Joint_6_speed_R;
    //实际关节角加加速度
    float Joint_1_dspeed_R;
    float Joint_2_dspeed_R;
    float Joint_3_dspeed_R;
    float Joint_4_dspeed_R;
    float Joint_5_dspeed_R;
    float Joint_6_dspeed_R;
    //计算夹具位置
    double X_tool_point_C;
    double Y_tool_point_C;
    double Z_tool_point_C;
    //实际夹具位置
    double X_tool_point_R;
    double Y_tool_point_R;
    double Z_tool_point_R;
    //计算夹具速度
    double X_tool_dpoint_C;
    double Y_tool_dpoint_C;
    double Z_tool_dpoint_C;
    //实际夹具速度
    double X_tool_dpoint_R;
    double Y_tool_dpoint_R;
    double Z_tool_dpoint_R;
    //计算夹具加速度
    double X_tool_ddpoint_C;
    double Y_tool_ddpoint_C;
    double Z_tool_ddpoint_C;
    //实际夹具加速度
    double X_tool_ddpoint_R;
    double Y_tool_ddpoint_R;
    double Z_tool_ddpoint_R;
    //计算三轴机械臂末端位置
    double X_robot_point_C;
    double Y_robot_point_C;
    double Z_robot_point_C;
    //实际三轴机械臂末端位置
    double X_robot_point_R;
    double Y_robot_point_R;
    double Z_robot_point_R;
    //计算三轴机械臂末端速度
    double X_robot_dpoint_C;
    double Y_robot_dpoint_C;
    double Z_robot_dpoint_C;
    //实际三轴机械臂末端速度
    double X_robot_dpoint_R;
    double Y_robot_dpoint_R;
    double Z_robot_dpoint_R;
    //计算三轴机械臂末端加速度
    double X_robot_ddpoint_C;
    double Y_robot_ddpoint_C;
    double Z_robot_ddpoint_C;
    //实际三轴机械臂末端加速度
    double X_robot_ddpoint_R;
    double Y_robot_ddpoint_R;
    double Z_robot_ddpoint_R;
    //计算Fixed Angle XYZ的转动角度
    double FA_X_tool_angle_C;
    double FA_Y_tool_angle_C;
    double FA_Z_tool_angle_C;
    //实际Fixed Angle XYZ的转动角度
    double FA_X_tool_angle_R;
    double FA_Y_tool_angle_R;
    double FA_Z_tool_angle_R;
    //计算Fixed Angle XYZ的转动角速度
    double FA_X_tool_dangle_C;
    double FA_Y_tool_dangle_C;
    double FA_Z_tool_dangle_C;
    //实际Fixed Angle XYZ的转动角速度
    double FA_X_tool_dangle_R;
    double FA_Y_tool_dangle_R;
    double FA_Z_tool_dangle_R;
    //计算Fixed Angle XYZ的转动角加速度
    double FA_X_tool_ddangle_C;
    double FA_Y_tool_ddangle_C;
    double FA_Z_tool_ddangle_C;
    //实际Fixed Angle XYZ的转动角加速度
    double FA_X_tool_ddangle_R;
    double FA_Y_tool_ddangle_R;
    double FA_Z_tool_ddangle_R;
    //关节减速比
    float Z1;
    float Z2;
    float Z3;
    float Z4;
    float Z5;
    float Z6;
    //夹具离5关节旋转中心的距离
    double cla_d;
    //计算夹具到世间坐标系的变换矩阵
    Eigen::MatrixXd Tt_0_C;
    //实际夹具到世间坐标系的变换矩阵
    Eigen::MatrixXd Tt_0_R;
    //计算机械臂末端到到世界坐标变换矩阵
    Eigen::MatrixXd T6_0_C;
    //实际机械臂末端到到世界坐标变换矩阵
    Eigen::MatrixXd T6_0_R;
    //计算机械臂末端到到世界坐标旋转矩阵
    Eigen::MatrixXd R6_0_C;
    //实际机械臂末端到到世界坐标旋转矩阵
    Eigen::MatrixXd R6_0_R;
    //中间变换矩阵
    Eigen::MatrixXd T1_0,T2_1,T3_2,T4_3,P4_3,P4_0,T5_4,T6_5,Tt_6;
    //从初始状态到工作状态的变换
    float init_motion[100];
public:
   Robot();
   void Robot_Set_Work_position(void);
   void Three_Ik(void);
   void zyzEuler_Ik(void);
   void FK(void);
   void Fixed_angle_calculate(void);
   void Fixed_angle_invcalculate(void);
   void get_joint_speed(void);
   void Back_Init_Position(void);
   void Linear_Speed_Plan(float finish_angle,float init_angle);
   void Get_Joint_Angle(void);
};

#endif // ROBOT_H
