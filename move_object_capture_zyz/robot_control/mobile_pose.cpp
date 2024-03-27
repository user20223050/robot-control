#include "mobile_pose.h"

mobile_pose::mobile_pose():object(4,4),Tc_c(4,4),Tc_0(4,4)
{
//    object << 1,     0,     0,  400,
//              0,     1,     0,    0,
//              0,     0,     1,    0,
//              0,     0,     0,    1;
    object << 0,     0,     1,  400,
             -1,     0,     0,    0,
              0,    -1,     0,    0,
              0,     0,     0,    1;
    Tc_0   << 0,     0,    -1,  1000,
              1,     0,     0,    0,
              0,    -1,     0,  200,
              0,     0,     0,    1;
//    Tc_c   << 1,     0,     0,    0,
//              0, 0.866,   0.5,    0,
//              0,  -0.5, 0.866,    0,
//              0,     0,     0,    1;
}

void mobile_pose::refresh_object()
{
    Eigen::MatrixXd Tt_0(4,1);
    Eigen::MatrixXd Tt_c(4,1);
    Tt_c << *(p)*1000, *(p+1)*1000, *(p+2)*1000, 1;
//    Tt_0 = Tc_0*Tc_c*Tt_c;
    Tt_0 = Tc_0*Tt_c;
    object(0,3) = X_point_OBJ_Real =  Tt_0(0,0);
    object(1,3) = Y_point_OBJ_Real =  Tt_0(1,0);
    object(2,3) = Z_point_OBJ_Real =  Tt_0(2,0);
}

void mobile_pose::conversion_coord_f(void)
{
    Eigen::MatrixXd Tt_6(4,4);
    Eigen::MatrixXd T6_0(4,4);
//    Tt_6   << 1, 0, 0, 0,
//              0,-1, 0, 0,
//              0, 0,-1,79,
//              0, 0, 0, 1;
    Tt_6   << 1, 0, 0,   0,
              0, 1, 0,   0,
              0, 0, 1,  90,
              0, 0, 0,   1;
    T6_0 = object * Tt_6.inverse();//由世界坐标系中物体的姿态Tt_0转换到世界坐标系中T6_0的姿态t为tool工具坐标系
    X_point_target = T6_0(0,3);
    Y_point_target = T6_0(1,3);
    Z_point_target = T6_0(2,3);
}
void mobile_pose::conversion_coord_s(Robot &robot)
{
    Eigen::MatrixXd Tt_6(4,4);
    Eigen::MatrixXd T6_0(4,4);
    Eigen::MatrixXd T1_0(4,4);
    Eigen::MatrixXd T2_1(4,4);
    Eigen::MatrixXd T3_2(4,4);
    Eigen::MatrixXd T4_X(4,4);
    Eigen::MatrixXd T3_0(4,4);
    Eigen::MatrixXd T6_3(4,4);
//    Tt_6   << 1, 0, 0, 0,
//              0,-1, 0, 0,
//              0, 0,-1,79,
//              0, 0, 0, 1;
    Tt_6   << 1, 0, 0,   0,
              0, 1, 0,   0,
              0, 0, 1,  90,
              0, 0, 0,   1;
    T6_0 = object * Tt_6.inverse();//由世界坐标系中物体的姿态Tt_0转换到世界坐标系中T6_0的姿态t为tool工具坐标系

//    T1_0 << cos(robot.Joint_1_angle_R), -sin(robot.Joint_1_angle_R), 0, 0,
//            sin(robot.Joint_1_angle_R),  cos(robot.Joint_1_angle_R), 0, 0,
//                                     0,                           0, 1, 0,
//                                     0,                           0, 0, 1;
//    T2_1 << cos(robot.Joint_2_angle_R), -sin(robot.Joint_2_angle_R), 0, 0,
//                                     0,                           0, 1, 0,
//           -sin(robot.Joint_2_angle_R), -cos(robot.Joint_2_angle_R), 0, 0,
//                                     0,                           0, 0, 1;
//    T3_2 << cos(robot.Joint_3_angle_R), -sin(robot.Joint_3_angle_R), 0, 0,
//            sin(robot.Joint_3_angle_R),  cos(robot.Joint_3_angle_R), 0, 0,
//                                     0,                           0, 1, 0,
//                                     0,                           0, 0, 1;
//    T4_X <<                          1,                           0, 0, 0,
//                                     0,                           0, 1, 0,
//                                     0,                          -1, 0, 0,
//                                     0,                           0, 0, 1;
    //    Fixed_angle_calculate();
        Joint_3_angle_T = -asin((pow(T6_0(0,3), 2) + pow(T6_0(1,3), 2) + pow(T6_0(2,3), 2)) / 148500.0 - 5941.0 / 5940.0);
        Joint_3_angle_T = round(Joint_3_angle_T * 10000) / 10000;
        if(Joint_3_angle_T > 0){
            Joint_2_angle_T = real(-log(-(pow((exp(Joint_3_angle_T * complex<double>(0.0, 1.0)) * pow(T6_0(2,3), 2) - 148525.0 * exp(Joint_3_angle_T * complex<double>(0.0, 1.0)) - exp(Joint_3_angle_T * complex<double>(0.0, 2.0)) * complex<double>(0.0, 74250.0) + complex<double>(0.0, 74250.0)),0.5) - T6_0(2,3) * exp((Joint_3_angle_T * complex<double>(0.0, 1.0)) / 2.0)) / (5.0 * (exp((Joint_3_angle_T * complex<double>(0.0, 1.0)) / 2.0) * complex<double>(0.0, 55.0) - 54.0 * exp((Joint_3_angle_T * complex<double>(0.0, 3.0)) / 2.0)))) * complex<double>(0.0, 1.0));
            Joint_2_angle_T = round(Joint_2_angle_T * 10000) / 10000;
        }
        else{
            Joint_2_angle_T = real(-log((pow((exp(Joint_3_angle_T * complex<double>(0.0, 1.0)) * pow(T6_0(2,3), 2) - 148525.0 * exp(Joint_3_angle_T * complex<double>(0.0, 1.0)) - exp(Joint_3_angle_T * complex<double>(0.0, 2.0)) * complex<double>(0.0, 74250.0) + complex<double>(0.0, 74250.0)),0.5) + T6_0(2,3) * exp((Joint_3_angle_T * complex<double>(0.0, 1.0)) / 2.0)) / (5.0 * (exp((Joint_3_angle_T * complex<double>(0.0, 1.0)) / 2.0) * complex<double>(0.0, 55.0) - 54.0 * exp((Joint_3_angle_T * complex<double>(0.0, 3.0)) / 2.0)))) * complex<double>(0.0, 1.0));
            Joint_2_angle_T = round(Joint_2_angle_T * 10000) / 10000;
        }
        if(T6_0(0,3) >= 0){
            Joint_1_angle_T = -asin(T6_0(1,3) / (270.0 * cos(Joint_2_angle_T) * sin(Joint_3_angle_T) - 275.0 * cos(Joint_2_angle_T) + 270.0 * cos(Joint_3_angle_T) * sin(Joint_2_angle_T)));
            Joint_1_angle_T = round(Joint_1_angle_T * 10000) / 10000;
        }
        else{
            Joint_1_angle_T = M_PI + asin(T6_0(1,3) / (270.0 * cos(Joint_2_angle_T) * sin(Joint_3_angle_T) - 275.0 * cos(Joint_2_angle_T) + 270.0 * cos(Joint_3_angle_T) * sin(Joint_2_angle_T)));
            Joint_1_angle_T = round(Joint_1_angle_T * 10000) / 10000;
        }
    T1_0 << cos(Joint_1_angle_T), -sin(Joint_1_angle_T), 0, 0,
            sin(Joint_1_angle_T),  cos(Joint_1_angle_T), 0, 0,
                                     0,                           0, 1, 0,
                                     0,                           0, 0, 1;
    T2_1 << cos(Joint_2_angle_T), -sin(Joint_2_angle_T), 0, 0,
                                     0,                           0, 1, 0,
           -sin(Joint_2_angle_T), -cos(Joint_2_angle_T), 0, 0,
                                     0,                           0, 0, 1;
    T3_2 << cos(Joint_3_angle_T), -sin(Joint_3_angle_T), 0, 0,
            sin(Joint_3_angle_T),  cos(Joint_3_angle_T), 0, 0,
                                     0,                           0, 1, 0,
                                     0,                           0, 0, 1;
    T4_X <<                          1,                           0, 0, 0,
                                     0,                           0, 1, 0,
                                     0,                          -1, 0, 0,
                                     0,                           0, 0, 1;
    T3_0 = T1_0*T2_1*T3_2*T4_X;
    T6_3 = (T3_0.inverse())*T6_0;
    Joint_5_angle_T = round((atan2(sqrt(pow(T6_3(2,0),2) + pow(T6_3(2,1),2)),T6_3(2,2))) * 1000) / 1000;
    if(round(((Joint_5_angle_T)*180/M_PI)*1)/1 == 0){
        Joint_4_angle_T = round((0 + M_PI) * 1000) / 1000;
        Joint_6_angle_T = round((atan2(-T6_3(0,1),T6_3(0,0)) + M_PI) * 1000) / 1000;
    }
    if(round(((Joint_5_angle_T)*180/M_PI)*1)/1 ==180){
        Joint_4_angle_T = round((0 + M_PI) * 1000) / 1000;
        Joint_6_angle_T = round((atan2(T6_3(0,1),-T6_3(0,0)) + M_PI) * 1000) / 1000;
    }
    if((round(((Joint_5_angle_T)*180/M_PI)*1)/1 != 0) && (round(((Joint_5_angle_T)*180/M_PI)*1)/1 !=180)) {
        Joint_4_angle_T = round((atan2(T6_3(1,2)/sin(Joint_5_angle_T),T6_3(0,2)/sin(Joint_5_angle_T)) + M_PI) * 1000) / 1000;
        Joint_6_angle_T = round((atan2(T6_3(2,1)/sin(Joint_5_angle_T),-T6_3(2,0)/sin(Joint_5_angle_T)) + M_PI) * 1000) / 1000;
    }
}

void mobile_pose::reflash_target_f()
{
    conversion_coord_f();
    static float X_point_target_old = X_point_target;
    static float Y_point_target_old = Y_point_target;
    static float Z_point_target_old = Z_point_target;
    static float X_speed_target_old = X_speed_target;
    static float Y_speed_target_old = Y_speed_target;
    static float Z_speed_target_old = Z_speed_target;
    static float X_dspeed_target_old = X_dspeed_target;
    static float Y_dspeed_target_old = Y_dspeed_target;
    static float Z_dspeed_target_old = Z_dspeed_target;

    X_speed_target = (X_point_target - X_point_target_old)/0.05;
    Y_speed_target = (Y_point_target - Y_point_target_old)/0.05;
    Z_speed_target = (Z_point_target - Z_point_target_old)/0.05;
    X_dspeed_target = (X_speed_target - X_speed_target_old)/0.05;
    Y_dspeed_target = (Y_speed_target - Y_speed_target_old)/0.05;
    Z_dspeed_target = (Z_speed_target - Z_speed_target_old)/0.05;
    X_ddspeed_target = (X_dspeed_target - X_dspeed_target_old)/0.05;
    Y_ddspeed_target = (Y_dspeed_target - Y_dspeed_target_old)/0.05;
    Z_ddspeed_target = (Z_dspeed_target - Z_dspeed_target_old)/0.05;
    if(X_ddspeed_target > 2000)
        X_ddspeed_target = 2000;
    if(X_ddspeed_target < -2000)
        X_ddspeed_target = -2000;
    if(Y_ddspeed_target > 2000)
        Y_ddspeed_target = 2000;
    if(Y_ddspeed_target < -2000)
        Y_ddspeed_target = -2000;
    if(Z_ddspeed_target > 2000)
        Z_ddspeed_target = 2000;
    if(Z_ddspeed_target < -2000)
        Z_ddspeed_target = -2000;
    X_point_target_old = X_point_target;
    Y_point_target_old = Y_point_target;
    Z_point_target_old = Z_point_target;
    X_speed_target_old = X_speed_target;
    Y_speed_target_old = Y_speed_target;
    Z_speed_target_old = Z_speed_target;
    X_dspeed_target_old = X_dspeed_target;
    Y_dspeed_target_old = Y_dspeed_target;
    Z_dspeed_target_old = Z_dspeed_target;
}
void mobile_pose::reflash_target_s(Robot &robot)
{
    conversion_coord_s(robot);
    static float Joint_4_angle_T_old = Joint_4_angle_T;
    static float Joint_5_angle_T_old = Joint_5_angle_T;
    static float Joint_6_angle_T_old = Joint_6_angle_T;
    static float Joint_4_dangle_T_old = Joint_4_dangle_T;
    static float Joint_5_dangle_T_old = Joint_5_dangle_T;
    static float Joint_6_dangle_T_old = Joint_6_dangle_T;
    static float Joint_4_ddangle_T_old = Joint_4_ddangle_T;
    static float Joint_5_ddangle_T_old = Joint_5_ddangle_T;
    static float Joint_6_ddangle_T_old = Joint_6_ddangle_T;

    Joint_4_dangle_T = (Joint_4_angle_T - Joint_4_angle_T_old)/0.05;
    Joint_5_dangle_T = (Joint_5_angle_T - Joint_5_angle_T_old)/0.05;
    Joint_6_dangle_T = (Joint_6_angle_T - Joint_6_angle_T_old)/0.05;
    Joint_4_ddangle_T = (Joint_4_dangle_T - Joint_4_dangle_T_old)/0.05;
    Joint_5_ddangle_T = (Joint_5_dangle_T - Joint_5_dangle_T_old)/0.05;
    Joint_6_ddangle_T = (Joint_6_dangle_T - Joint_6_dangle_T_old)/0.05;
    Joint_4_dddangle_T = (Joint_4_ddangle_T - Joint_4_ddangle_T_old)/0.05;
    Joint_5_dddangle_T = (Joint_5_ddangle_T - Joint_5_ddangle_T_old)/0.05;
    Joint_6_dddangle_T = (Joint_6_ddangle_T - Joint_6_ddangle_T_old)/0.05;
    if(isnan(Joint_4_dddangle_T)) {
        Joint_4_dddangle_T = 0;
    }
    if(Joint_4_dddangle_T > 2){
        Joint_4_dddangle_T = 2;
    }
    if(Joint_4_dddangle_T < -2){
        Joint_4_dddangle_T = -2;
    }
    if(isnan(Joint_5_dddangle_T)) {
        Joint_5_dddangle_T = 0;
    }
    if(Joint_5_dddangle_T > 2){
        Joint_5_dddangle_T = 2;
    }
    if(Joint_5_dddangle_T < -2){
        Joint_5_dddangle_T = -2;
    }
    if(isnan(Joint_6_dddangle_T)) {
        Joint_6_dddangle_T = 0;
    }
    if(Joint_6_dddangle_T > 2){
        Joint_6_dddangle_T = 2;
    }
    if(Joint_6_dddangle_T < -2){
        Joint_6_dddangle_T = -2;
    }
    std::cout<<"Joint_4_angle_T = "<<Joint_4_angle_T<<std::endl;
    std::cout<<"Joint_5_angle_T = "<<Joint_5_angle_T<<std::endl;
    std::cout<<"Joint_6_angle_T = "<<Joint_6_angle_T<<std::endl;
    std::cout<<"\n"<<std::endl;
    std::cout<<"Joint_4_dangle_T = "<<Joint_4_dangle_T<<std::endl;
    std::cout<<"Joint_5_dangle_T = "<<Joint_5_dangle_T<<std::endl;
    std::cout<<"Joint_6_dangle_T = "<<Joint_6_dangle_T<<std::endl;
    std::cout<<"\n"<<std::endl;
    std::cout<<"Joint_4_ddangle_T = "<<Joint_4_ddangle_T<<std::endl;
    std::cout<<"Joint_4_ddangle_T = "<<Joint_5_ddangle_T<<std::endl;
    std::cout<<"Joint_4_ddangle_T = "<<Joint_6_ddangle_T<<std::endl;
    std::cout<<"\n"<<std::endl;
    std::cout<<"Joint_4_dddangle_T = "<<Joint_4_dddangle_T<<std::endl;
    std::cout<<"Joint_5_dddangle_T = "<<Joint_5_dddangle_T<<std::endl;
    std::cout<<"Joint_6_dddangle_T = "<<Joint_6_dddangle_T<<std::endl;
    std::cout<<"\n"<<std::endl;
    Joint_4_angle_T_old = Joint_4_angle_T;
    Joint_5_angle_T_old = Joint_5_angle_T;
    Joint_6_angle_T_old = Joint_6_angle_T;
    Joint_4_dangle_T_old = Joint_4_dangle_T;
    Joint_5_dangle_T_old = Joint_5_dangle_T;
    Joint_6_dangle_T_old = Joint_6_dangle_T;
    Joint_4_ddangle_T_old = Joint_4_ddangle_T;
    Joint_5_ddangle_T_old = Joint_5_ddangle_T;
    Joint_6_ddangle_T_old = Joint_6_ddangle_T;
}
void mobile_pose::kalman_forecast()
{

}
