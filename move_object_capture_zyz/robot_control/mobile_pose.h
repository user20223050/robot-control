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
    float X_point_OBJ_Real;
    float Y_point_OBJ_Real;
    float Z_point_OBJ_Real;
    //物体在摄像头中的位置
    float X_point_OBJ_Camera;
    float Y_point_OBJ_Camera;
    float Z_point_OBJ_Camera;
    //目标位姿矩阵
    Eigen::MatrixXd object;
    //摄像头旋转的角度
    Eigen::MatrixXd Tc_c;
    //摄像头坐标与世界坐标的关系
    Eigen::MatrixXd Tc_0;
    //需要跟随的目标位置
    float X_point_target;
    float Y_point_target;
    float Z_point_target;
    //需要跟随的目标速度
    float X_speed_target;
    float Y_speed_target;
    float Z_speed_target;
    //需要跟随的目标加速度
    float X_dspeed_target;
    float Y_dspeed_target;
    float Z_dspeed_target;
    //需要跟随的目标加加速度
    float X_ddspeed_target;
    float Y_ddspeed_target;
    float Z_ddspeed_target;

    //需要跟随的目标角度
    float Joint_4_angle_T;
    float Joint_5_angle_T;
    float Joint_6_angle_T;
    //需要跟随的目标角速度
    float Joint_4_dangle_T;
    float Joint_5_dangle_T;
    float Joint_6_dangle_T;
    //需要跟随的目标角加速度
    float Joint_4_ddangle_T;
    float Joint_5_ddangle_T;
    float Joint_6_ddangle_T;
    //目标角加加速度
    float Joint_4_dddangle_T;
    float Joint_5_dddangle_T;
    float Joint_6_dddangle_T;

public:
    mobile_pose();
    void refresh_object(void);
    void conversion_coord_f(void);
    void conversion_coord_s(Robot &robot);
    void reflash_target_f(void);
    void reflash_target_s(Robot &robot);
    void kalman_forecast(void);

};

#endif // MOBILE_POSE_H
