#include "mobile_pose.h"

mobile_pose::mobile_pose():object(4,4),Tc_c(4,4),Tc_0(4,4)
{
    object << 1,     0,     0,  400,
              0,     1,     0,    0,
              0,     0,     1,    0,
              0,     0,     0,    1;
    Tc_0   << 0,     0,    -1, 1000,
              1,     0,     0,    0,
              0,    -1,     0,  500,
              0,     0,     0,    1;
    Tc_c   << 1,     0,     0,    0,
              0, 0.866,   0.5,    0,
              0,  -0.5, 0.866,    0,
              0,     0,     0,    1;
}

void mobile_pose::refresh_object()
{
    Eigen::MatrixXd Tt_0(4,1);
    Eigen::MatrixXd Tt_c(4,1);
    Tt_c << *(p)*1000, *(p+1)*1000, *(p+2)*1000, 1;
    Tt_0 = Tc_0*Tc_c*Tt_c;
    object(0,3) = X_point_OBJ_Real =  Tt_0(0,0);
    object(1,3) = Y_point_OBJ_Real =  Tt_0(1,0);
    object(2,3) = Z_point_OBJ_Real =  Tt_0(2,0);
}

void mobile_pose::conversion_coord()
{
    Eigen::MatrixXd Tt_6(4,4);
    Eigen::MatrixXd T6_0(4,4);
    Tt_6   << 1, 0, 0, 0,
              0,-1, 0, 0,
              0, 0,-1,79,
              0, 0, 0, 1;
    T6_0 = object * Tt_6.inverse();//由世界坐标系中物体的姿态Tt_0转换到世界坐标系中T6_0的姿态t为tool工具坐标系
    Y_angle_target = atan2(-T6_0(2,0),sqrt(pow(T6_0(0,0),2) + pow(T6_0(1,0),2)));
    if(round(Y_angle_target*180/M_PI*1)/1 == 90){
        Z_angle_target = 0;
        X_angle_target = atan2(T6_0(0,1),T6_0(1,1));
    }
    if(round(Y_angle_target*180/M_PI*1)/1 == -90){
        Z_angle_target = 0;
        X_angle_target = -atan2(T6_0(0,1),T6_0(1,1));
    }
    else {
        Z_angle_target = atan2(T6_0(1,0)/cos(Y_angle_target),T6_0(0,0)/cos(Y_angle_target));
        X_angle_target = atan2(T6_0(2,1)/cos(Y_angle_target),T6_0(2,2)/cos(Y_angle_target));
    }
    X_point_target = T6_0(0,3);
    Y_point_target = T6_0(1,3);
    Z_point_target = T6_0(2,3);
}

void mobile_pose::reflash_target()
{
    conversion_coord();
    static float X_point_target_old = X_point_target;
    static float Y_point_target_old = Y_point_target;
    static float Z_point_target_old = Z_point_target;
    static float X_angle_target_old = X_angle_target;
    static float Y_angle_target_old = Y_angle_target;
    static float Z_angle_target_old = Z_angle_target;
    static float X_speed_target_old = X_speed_target;
    static float Y_speed_target_old = Y_speed_target;
    static float Z_speed_target_old = Z_speed_target;
    static float X_dangle_target_old = X_dangle_target;
    static float Y_dangle_target_old = Y_dangle_target;
    static float Z_dangle_target_old = Z_dangle_target;
    X_speed_target = (X_point_target - X_point_target_old)/0.05;
    Y_speed_target = (Y_point_target - Y_point_target_old)/0.05;
    Z_speed_target = (Z_point_target - Z_point_target_old)/0.05;
    X_dangle_target = (X_angle_target - X_angle_target_old)/0.05;
    Y_dangle_target = (Y_angle_target - Y_angle_target_old)/0.05;
    Z_dangle_target = (Z_angle_target - Z_angle_target_old)/0.05;
    X_dspeed_target = (X_speed_target - X_speed_target_old)/0.05;
    Y_dspeed_target = (Y_speed_target - Y_speed_target_old)/0.05;
    Z_dspeed_target = (Z_speed_target - Z_speed_target_old)/0.05;
    X_ddangle_target = (X_dangle_target - X_dangle_target_old)/0.05;
    Y_ddangle_target = (Y_dangle_target - Y_dangle_target_old)/0.05;
    Z_ddangle_target = (Z_dangle_target - Z_dangle_target_old)/0.05;
    X_point_target_old = X_point_target;
    Y_point_target_old = Y_point_target;
    Z_point_target_old = Z_point_target;
    X_angle_target_old = X_angle_target;
    Y_angle_target_old = Y_angle_target;
    Z_angle_target_old = Z_angle_target;
    X_speed_target_old = X_speed_target;
    Y_speed_target_old = Y_speed_target;
    Z_speed_target_old = Z_speed_target;
    X_dangle_target_old = X_dangle_target;
    Y_dangle_target_old = Y_dangle_target;
    Z_dangle_target_old = Z_dangle_target;
}

void mobile_pose::kalman_forecast()
{

}
