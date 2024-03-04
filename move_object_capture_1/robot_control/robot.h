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
    //����ؽڽǶ�
    double Joint_1_angle_C;
    double Joint_2_angle_C;
    double Joint_3_angle_C;
    double Joint_4_angle_C;
    double Joint_5_angle_C;
    double Joint_6_angle_C;
    //ʵ�ʹؽڽǶ�
    double Joint_1_angle_R;
    double Joint_2_angle_R;
    double Joint_3_angle_R;
    double Joint_4_angle_R;
    double Joint_5_angle_R;
    double Joint_6_angle_R;
    //ʵ�ʹؽڽ��ٶ�
    float Joint_1_speed_R;
    float Joint_2_speed_R;
    float Joint_3_speed_R;
    float Joint_4_speed_R;
    float Joint_5_speed_R;
    float Joint_6_speed_R;
    //ʵ�ʹؽڽǼӼ��ٶ�
    float Joint_1_dspeed_R;
    float Joint_2_dspeed_R;
    float Joint_3_dspeed_R;
    float Joint_4_dspeed_R;
    float Joint_5_dspeed_R;
    float Joint_6_dspeed_R;
    //����о�λ��
    double X_tool_point_C;
    double Y_tool_point_C;
    double Z_tool_point_C;
    //ʵ�ʼо�λ��
    double X_tool_point_R;
    double Y_tool_point_R;
    double Z_tool_point_R;
    //����о��ٶ�
    double X_tool_dpoint_C;
    double Y_tool_dpoint_C;
    double Z_tool_dpoint_C;
    //ʵ�ʼо��ٶ�
    double X_tool_dpoint_R;
    double Y_tool_dpoint_R;
    double Z_tool_dpoint_R;
    //����о߼��ٶ�
    double X_tool_ddpoint_C;
    double Y_tool_ddpoint_C;
    double Z_tool_ddpoint_C;
    //ʵ�ʼо߼��ٶ�
    double X_tool_ddpoint_R;
    double Y_tool_ddpoint_R;
    double Z_tool_ddpoint_R;
    //���������е��ĩ��λ��
    double X_robot_point_C;
    double Y_robot_point_C;
    double Z_robot_point_C;
    //ʵ�������е��ĩ��λ��
    double X_robot_point_R;
    double Y_robot_point_R;
    double Z_robot_point_R;
    //���������е��ĩ���ٶ�
    double X_robot_dpoint_C;
    double Y_robot_dpoint_C;
    double Z_robot_dpoint_C;
    //ʵ�������е��ĩ���ٶ�
    double X_robot_dpoint_R;
    double Y_robot_dpoint_R;
    double Z_robot_dpoint_R;
    //���������е��ĩ�˼��ٶ�
    double X_robot_ddpoint_C;
    double Y_robot_ddpoint_C;
    double Z_robot_ddpoint_C;
    //ʵ�������е��ĩ�˼��ٶ�
    double X_robot_ddpoint_R;
    double Y_robot_ddpoint_R;
    double Z_robot_ddpoint_R;
    //����Fixed Angle XYZ��ת���Ƕ�
    double FA_X_tool_angle_C;
    double FA_Y_tool_angle_C;
    double FA_Z_tool_angle_C;
    //ʵ��Fixed Angle XYZ��ת���Ƕ�
    double FA_X_tool_angle_R;
    double FA_Y_tool_angle_R;
    double FA_Z_tool_angle_R;
    //����Fixed Angle XYZ��ת�����ٶ�
    double FA_X_tool_dangle_C;
    double FA_Y_tool_dangle_C;
    double FA_Z_tool_dangle_C;
    //ʵ��Fixed Angle XYZ��ת�����ٶ�
    double FA_X_tool_dangle_R;
    double FA_Y_tool_dangle_R;
    double FA_Z_tool_dangle_R;
    //����Fixed Angle XYZ��ת���Ǽ��ٶ�
    double FA_X_tool_ddangle_C;
    double FA_Y_tool_ddangle_C;
    double FA_Z_tool_ddangle_C;
    //ʵ��Fixed Angle XYZ��ת���Ǽ��ٶ�
    double FA_X_tool_ddangle_R;
    double FA_Y_tool_ddangle_R;
    double FA_Z_tool_ddangle_R;
    //�ؽڼ��ٱ�
    float Z1;
    float Z2;
    float Z3;
    float Z4;
    float Z5;
    float Z6;
    //�о���5�ؽ���ת���ĵľ���
    double cla_d;
    //����оߵ���������ϵ�ı任����
    Eigen::MatrixXd Tt_0_C;
    //ʵ�ʼоߵ���������ϵ�ı任����
    Eigen::MatrixXd Tt_0_R;
    //�����е��ĩ�˵�����������任����
    Eigen::MatrixXd T6_0_C;
    //ʵ�ʻ�е��ĩ�˵�����������任����
    Eigen::MatrixXd T6_0_R;
    //�����е��ĩ�˵�������������ת����
    Eigen::MatrixXd R6_0_C;
    //ʵ�ʻ�е��ĩ�˵�������������ת����
    Eigen::MatrixXd R6_0_R;
    //�м�任����
    Eigen::MatrixXd T1_0,T2_1,T3_2,T4_3,P4_3,P4_0,T5_4,T6_5,Tt_6;
    //�ӳ�ʼ״̬������״̬�ı任
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
