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
    //抓取目标位置
    double X_point_OBJ_Real;
    double Y_point_OBJ_Real;
    double Z_point_OBJ_Real;
    //物体在摄像头中的位置
    double X_point_OBJ_Camera;
    double Y_point_OBJ_Camera;
    double Z_point_OBJ_Camera;
    //目标位姿矩阵
    Eigen::MatrixXd object;
    //摄像头旋转的角度
    Eigen::MatrixXd Tc_c;
    //摄像头坐标与世界坐标的关系
    Eigen::MatrixXd Tc_0;
    //需要跟随的目标位置
    double X_point_target;
    double Y_point_target;
    double Z_point_target;
    //需要跟随的目标速度
    double X_speed_target;
    double Y_speed_target;
    double Z_speed_target;
    //需要跟随的目标加速度
    double X_dspeed_target;
    double Y_dspeed_target;
    double Z_dspeed_target;
    //需要跟随的目标加加速度
    double X_ddspeed_target;
    double Y_ddspeed_target;
    double Z_ddspeed_target;
    //需要跟随的目标角度
    double Joint_1_angle_T;
    double Joint_2_angle_T;
    double Joint_3_angle_T;
    double Joint_4_angle_T;
    double Joint_5_angle_T;
    double Joint_6_angle_T;
    //需要跟随的目标角速度
    double Joint_1_dangle_T;
    double Joint_2_dangle_T;
    double Joint_3_dangle_T;
    double Joint_4_dangle_T;
    double Joint_5_dangle_T;
    double Joint_6_dangle_T;
    //需要跟随的目标角加速度
    double Joint_1_ddangle_T;
    double Joint_2_ddangle_T;
    double Joint_3_ddangle_T;
    double Joint_4_ddangle_T;
    double Joint_5_ddangle_T;
    double Joint_6_ddangle_T;
    //目标角加加速度
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
