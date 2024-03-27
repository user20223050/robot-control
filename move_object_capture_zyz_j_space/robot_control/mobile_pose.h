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
    double X_point_OBJ_Real;
    double Y_point_OBJ_Real;
    double Z_point_OBJ_Real;
    //����������ͷ�е�λ��
    double X_point_OBJ_Camera;
    double Y_point_OBJ_Camera;
    double Z_point_OBJ_Camera;
    //Ŀ��λ�˾���
    Eigen::MatrixXd object;
    //����ͷ��ת�ĽǶ�
    Eigen::MatrixXd Tc_c;
    //����ͷ��������������Ĺ�ϵ
    Eigen::MatrixXd Tc_0;
    //��Ҫ�����Ŀ��λ��
    double X_point_target;
    double Y_point_target;
    double Z_point_target;
    //��Ҫ�����Ŀ���ٶ�
    double X_speed_target;
    double Y_speed_target;
    double Z_speed_target;
    //��Ҫ�����Ŀ����ٶ�
    double X_dspeed_target;
    double Y_dspeed_target;
    double Z_dspeed_target;
    //��Ҫ�����Ŀ��Ӽ��ٶ�
    double X_ddspeed_target;
    double Y_ddspeed_target;
    double Z_ddspeed_target;
    //��Ҫ�����Ŀ��Ƕ�
    double Joint_1_angle_T;
    double Joint_2_angle_T;
    double Joint_3_angle_T;
    double Joint_4_angle_T;
    double Joint_5_angle_T;
    double Joint_6_angle_T;
    //��Ҫ�����Ŀ����ٶ�
    double Joint_1_dangle_T;
    double Joint_2_dangle_T;
    double Joint_3_dangle_T;
    double Joint_4_dangle_T;
    double Joint_5_dangle_T;
    double Joint_6_dangle_T;
    //��Ҫ�����Ŀ��Ǽ��ٶ�
    double Joint_1_ddangle_T;
    double Joint_2_ddangle_T;
    double Joint_3_ddangle_T;
    double Joint_4_ddangle_T;
    double Joint_5_ddangle_T;
    double Joint_6_ddangle_T;
    //Ŀ��ǼӼ��ٶ�
    double Joint_1_dddangle_T;
    double Joint_2_dddangle_T;
    double Joint_3_dddangle_T;
    double Joint_4_dddangle_T;
    double Joint_5_dddangle_T;
    double Joint_6_dddangle_T;

public:
    mobile_pose();
    void refresh_object(void);
    void conversion_coord_f(void);
    void conversion_coord_s(void);
    void reflash_target_f(void);
    void reflash_target_s(void);
    void kalman_forecast(void);

};

#endif // MOBILE_POSE_H
