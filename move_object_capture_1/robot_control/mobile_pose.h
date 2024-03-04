#ifndef MOBILE_POSE_H
#define MOBILE_POSE_H

#include "robot.h"

extern float *p;
extern uint8_t follow_target;

class mobile_pose
{
    friend class MPC_Control;
    friend class Slave_Copmputer;
public:
    //ץȡĿ��λ��
    float X_point_OBJ_Real;
    float Y_point_OBJ_Real;
    float Z_point_OBJ_Real;
    //����������ͷ�е�λ��
    float X_point_OBJ_Camera;
    float Y_point_OBJ_Camera;
    float Z_point_OBJ_Camera;
    //Ŀ��λ�˾���
    Eigen::MatrixXd object;
    //����ͷ��ת�ĽǶ�
    Eigen::MatrixXd Tc_c;
    //����ͷ��������������Ĺ�ϵ
    Eigen::MatrixXd Tc_0;
    //��Ҫ�����Ŀ��λ��
    float X_point_target;
    float Y_point_target;
    float Z_point_target;
    //��Ҫ�����Ŀ���ٶ�
    float X_speed_target;
    float Y_speed_target;
    float Z_speed_target;
    //��Ҫ�����Ŀ����ٶ�
    float X_dspeed_target;
    float Y_dspeed_target;
    float Z_dspeed_target;
    //��Ҫ�����Ŀ��Ƕ�
    float X_angle_target;
    float Y_angle_target;
    float Z_angle_target;
    //��Ҫ�����Ŀ����ٶ�
    float X_dangle_target;
    float Y_dangle_target;
    float Z_dangle_target;
    //��Ҫ�����Ŀ��Ǽ��ٶ�
    float X_ddangle_target;
    float Y_ddangle_target;
    float Z_ddangle_target;
public:
    mobile_pose();
    void refresh_object(void);
    void conversion_coord(void);
    void reflash_target(void);
    void kalman_forecast(void);

};

#endif // MOBILE_POSE_H
