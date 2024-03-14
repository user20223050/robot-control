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
    //需要跟随的目标角度
    float X_angle_target;
    float Y_angle_target;
    float Z_angle_target;
    //需要跟随的目标角速度
    float X_dangle_target;
    float Y_dangle_target;
    float Z_dangle_target;
    //需要跟随的目标角加速度
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
