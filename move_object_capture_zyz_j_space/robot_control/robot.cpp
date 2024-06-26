#include "robot.h"

Robot::Robot():T1_0(4,4),T2_1(4,4),T3_2(4,4),T4_3(4,4),P4_3(4,1),P4_0(4,1),T5_4(4,4),T6_5(4,4),Tt_6(4,4),Tt_0_C(4,4),Tt_0_R(4,4),T6_0_C(4,4),T6_0_R(4,4),R6_0_C(3,3),R6_0_R(3,3)
{
    Z1 = 192;
    Z2 = 150;
    Z3 = 62.5;
    Z4 = 192;
    Z5 = 95.4;
    Z6 = 50;
    cla_d = 79;
    Tt_6 <<   1, 0, 0,     0,
              0, 1, 0,     0,
              0, 0, 1, cla_d,
              0, 0, 0,     1;
    Tt_0_C << 1, 0, 0,     0,
              0, 1, 0,     0,
              0, 0, 1,     0,
              0, 0, 0,     1;
    Tt_0_R << 1, 0, 0,     0,
              0, 1, 0,     0,
              0, 0, 1,     0,
              0, 0, 0,     1;
    for (int i = 0; i < 100; ++i) {
        joint2_buf[i] = -M_PI/2;
    }
    for (int i = 0; i < 100; ++i) {
        joint3_buf[i] = -M_PI/2;
    }
    errror_position = 0;
}
/*从初始状态到工作状态*/
void Robot::Robot_Set_Work_position(void)
{
    /*INIT_STATE*/
    X_robot_point_C = 0;
    Y_robot_point_C = 0;
    Z_robot_point_C = 275 + 270;
    FA_X_tool_angle_C = 0;
    FA_Y_tool_angle_C = 0;
    FA_Z_tool_angle_C = -M_PI;
//    Tt_0_C << -1,  0,  0, X_tool_point_C,
//               0, -1,  0, Y_tool_point_C,
//               0,  0,  1, Z_tool_point_C,
//               0,  0,  0,              1;
    Three_Ik();
    zyzEuler_Ik();
    float init_angle3 = Joint_3_angle_C;
    /*WORK_STATE*/
    X_robot_point_C = 270;
    Y_robot_point_C = 0;
    Z_robot_point_C = 275;
    FA_X_tool_angle_C = M_PI;
    FA_Y_tool_angle_C = -M_PI/2;
    FA_Z_tool_angle_C = 0;
//    Tt_0_C << 0,  0,  1, X_tool_point_C,
//              0, -1,  0, Y_tool_point_C,
//              1,  0,  0, Z_tool_point_C,
//              0,  0,  0,              1;
    Three_Ik();
    zyzEuler_Ik();
    float finish_angle3 = Joint_3_angle_C;
    Linear_Speed_Plan(finish_angle3,init_angle3,joint3_buf);
    X_robot_point_R = X_robot_point_C;
    Y_robot_point_R = Y_robot_point_C;
    Z_robot_point_R = Z_robot_point_C;
    FA_X_tool_angle_R = FA_X_tool_angle_C;
    FA_Y_tool_angle_R = FA_Y_tool_angle_C;
    FA_Z_tool_angle_R = FA_Z_tool_angle_C;
    Joint_1_angle_R = Joint_1_angle_C;
    Joint_2_angle_R = Joint_2_angle_C;
    Joint_3_angle_R = Joint_3_angle_C;
    Joint_4_angle_R = Joint_4_angle_C = 0;
    Joint_5_angle_R = Joint_5_angle_C = 0;
    Joint_6_angle_R = Joint_6_angle_C = 0;
//    get_joint_speed_first();
//    get_joint_speed_second();
    FK();//由关节角度得到末端位置
}
/*三关节逆运动学求解*/
void Robot::Three_Ik(void)
{
//    Fixed_angle_calculate();
    Joint_3_angle_C = -asin((pow(X_robot_point_C, 2) + pow(Y_robot_point_C, 2) + pow(Z_robot_point_C, 2)) / 148500.0 - 5941.0 / 5940.0);
    Joint_3_angle_C = round(Joint_3_angle_C * 10000) / 10000;
    if(Joint_3_angle_C > 0){
        Joint_2_angle_C = real(-log(-(pow((exp(Joint_3_angle_C * complex<double>(0.0, 1.0)) * pow(Z_robot_point_C, 2) - 148525.0 * exp(Joint_3_angle_C * complex<double>(0.0, 1.0)) - exp(Joint_3_angle_C * complex<double>(0.0, 2.0)) * complex<double>(0.0, 74250.0) + complex<double>(0.0, 74250.0)),0.5) - Z_robot_point_C * exp((Joint_3_angle_C * complex<double>(0.0, 1.0)) / 2.0)) / (5.0 * (exp((Joint_3_angle_C * complex<double>(0.0, 1.0)) / 2.0) * complex<double>(0.0, 55.0) - 54.0 * exp((Joint_3_angle_C * complex<double>(0.0, 3.0)) / 2.0)))) * complex<double>(0.0, 1.0));
        Joint_2_angle_C = round(Joint_2_angle_C * 10000) / 10000;
    }
    else{
        Joint_2_angle_C = real(-log((pow((exp(Joint_3_angle_C * complex<double>(0.0, 1.0)) * pow(Z_robot_point_C, 2) - 148525.0 * exp(Joint_3_angle_C * complex<double>(0.0, 1.0)) - exp(Joint_3_angle_C * complex<double>(0.0, 2.0)) * complex<double>(0.0, 74250.0) + complex<double>(0.0, 74250.0)),0.5) + Z_robot_point_C * exp((Joint_3_angle_C * complex<double>(0.0, 1.0)) / 2.0)) / (5.0 * (exp((Joint_3_angle_C * complex<double>(0.0, 1.0)) / 2.0) * complex<double>(0.0, 55.0) - 54.0 * exp((Joint_3_angle_C * complex<double>(0.0, 3.0)) / 2.0)))) * complex<double>(0.0, 1.0));
        Joint_2_angle_C = round(Joint_2_angle_C * 10000) / 10000;
    }
    if(X_robot_point_C >= 0){
        Joint_1_angle_C = -asin(Y_robot_point_C / (270.0 * cos(Joint_2_angle_C) * sin(Joint_3_angle_C) - 275.0 * cos(Joint_2_angle_C) + 270.0 * cos(Joint_3_angle_C) * sin(Joint_2_angle_C)));
        Joint_1_angle_C = round(Joint_1_angle_C * 10000) / 10000;
    }
    else{
        Joint_1_angle_C = M_PI + asin(Y_robot_point_C / (270.0 * cos(Joint_2_angle_C) * sin(Joint_3_angle_C) - 275.0 * cos(Joint_2_angle_C) + 270.0 * cos(Joint_3_angle_C) * sin(Joint_2_angle_C)));
        Joint_1_angle_C = round(Joint_1_angle_C * 10000) / 10000;
    }
}
/*末端三关节逆运动学求解*/
void Robot::zyzEuler_Ik(void)
{
    Fixed_angle_calculate();
    Eigen::MatrixXd R1_0(3,3);
    Eigen::MatrixXd R2_1(3,3);
    Eigen::MatrixXd R3_2(3,3);
    Eigen::MatrixXd R4_X(3,3);
    Eigen::MatrixXd R3_0(3,3);
    Eigen::MatrixXd R6_3(3,3);
    R1_0 << cos(Joint_1_angle_R), -sin(Joint_1_angle_R), 0,
            sin(Joint_1_angle_R),  cos(Joint_1_angle_R), 0,
                               0,                     0, 1;
    R2_1 << cos(Joint_2_angle_R), -sin(Joint_2_angle_R), 0,
                               0,                     0, 1,
           -sin(Joint_2_angle_R), -cos(Joint_2_angle_R), 0;
    R3_2 << cos(Joint_3_angle_R), -sin(Joint_3_angle_R), 0,
            sin(Joint_3_angle_R),  cos(Joint_3_angle_R), 0,
                               0,                     0, 1;
    R4_X <<                    1,                     0, 0,
                               0,                     0, 1,
                               0,                    -1, 0;
    R3_0 = R1_0*R2_1*R3_2*R4_X;
    R6_3 = (R3_0.inverse())*R6_0_C;
    Joint_5_angle_C = round((atan2(sqrt(pow(R6_3(2,0),2) + pow(R6_3(2,1),2)),R6_3(2,2))) * 1000) / 1000;
    if(round(((Joint_5_angle_C)*180/M_PI)*1)/1 == 0){
        Joint_4_angle_C = round((0 - M_PI) * 1000) / 1000;
        Joint_6_angle_C = round((atan2(-R6_3(0,1),R6_3(0,0)) - M_PI) * 1000) / 1000;
    }
    if(round(((Joint_5_angle_C)*180/M_PI)*1)/1 ==180){
        Joint_4_angle_C = round((0 - M_PI) * 1000) / 1000;
        Joint_6_angle_C = round((atan2(R6_3(0,1),-R6_3(0,0)) - M_PI) * 1000) / 1000;
    }
    else {
        Joint_4_angle_C = round((atan2(R6_3(1,2)/sin(Joint_5_angle_C),R6_3(0,2)/sin(Joint_5_angle_C)) - M_PI) * 1000) / 1000;
        Joint_6_angle_C = round((atan2(R6_3(2,1)/sin(Joint_5_angle_C),-R6_3(2,0)/sin(Joint_5_angle_C)) - M_PI) * 1000) / 1000;
    }
}

void Robot::FK()
{
    static double X_robot_point_R_old = X_robot_point_R;
    static double Y_robot_point_R_old = Y_robot_point_R;
    static double Z_robot_point_R_old = Z_robot_point_R;
    static double X_robot_dpoint_R_old = X_robot_dpoint_R;
    static double Y_robot_dpoint_R_old = Y_robot_dpoint_R;
    static double Z_robot_dpoint_R_old = Z_robot_dpoint_R;
    static double Joint_1_angle_R_old = Joint_1_angle_R;
    static double Joint_2_angle_R_old = Joint_2_angle_R;
    static double Joint_3_angle_R_old = Joint_3_angle_R;
    static double Joint_4_angle_R_old = Joint_4_angle_R;
    static double Joint_5_angle_R_old = Joint_5_angle_R;
    static double Joint_6_angle_R_old = Joint_6_angle_R;
    static double Joint_1_speed_R_old = Joint_1_speed_R;
    static double Joint_2_speed_R_old = Joint_2_speed_R;
    static double Joint_3_speed_R_old = Joint_3_speed_R;
    static double Joint_4_speed_R_old = Joint_4_speed_R;
    static double Joint_5_speed_R_old = Joint_5_speed_R;
    static double Joint_6_speed_R_old = Joint_6_speed_R;
    T1_0 << cos(Joint_1_angle_R), -sin(Joint_1_angle_R), 0, 0,
            sin(Joint_1_angle_R),  cos(Joint_1_angle_R), 0, 0,
                               0,                     0, 1, 0,
                               0,                     0, 0, 1;
    T2_1 << cos(Joint_2_angle_R), -sin(Joint_2_angle_R), 0, 0,
                               0,                     0, 1, 0,
           -sin(Joint_2_angle_R), -cos(Joint_2_angle_R), 0, 0,
                               0,                     0, 0, 1;
    T3_2 << cos(Joint_3_angle_R), -sin(Joint_3_angle_R), 0,275,
            sin(Joint_3_angle_R),  cos(Joint_3_angle_R), 0,  0,
                               0,                     0, 1,  0,
                               0,                     0, 0,  1;
    T4_3 << cos(Joint_4_angle_R), -sin(Joint_4_angle_R), 0,  0,
                               0,                     0, 1,270,
           -sin(Joint_4_angle_R), -cos(Joint_4_angle_R), 0,  0,
                               0,                     0, 0,  1;
    T5_4 << cos(Joint_5_angle_R), -sin(Joint_5_angle_R), 0, 0,
                               0,                     0,-1, 0,
            sin(Joint_5_angle_R),  cos(Joint_5_angle_R), 0, 0,
                               0,                     0, 0, 1;
    T6_5 << cos(Joint_6_angle_R), -sin(Joint_6_angle_R), 0, 0,
                               0,                     0, 1, 0,
           -sin(Joint_6_angle_R), -cos(Joint_6_angle_R), 0, 0,
                               0,                     0, 0, 1;
    Tt_6 << 1,                   0,                  0,    0,
            0,                   1,                  0,    0,
            0,                   0,                  1,cla_d,
            0,                   0,                  0,    1;
    T6_0_R = T1_0*T2_1*T3_2*T4_3*T5_4*T6_5;
    Tt_0_R = T6_0_R*Tt_6;
//    Fixed_angle_invcalculate();
    X_tool_point_R = Tt_0_R(0,3);
    Y_tool_point_R = Tt_0_R(1,3);
    Z_tool_point_R = Tt_0_R(2,3);
    X_robot_point_R = T6_0_R(0,3);
    Y_robot_point_R = T6_0_R(1,3);
    Z_robot_point_R = T6_0_R(2,3);
    X_robot_dpoint_R = (X_robot_point_R - X_robot_point_R_old)/0.05;
    Y_robot_dpoint_R = (Y_robot_point_R - Y_robot_point_R_old)/0.05;
    Z_robot_dpoint_R = (Z_robot_point_R - Z_robot_point_R_old)/0.05;
    X_robot_ddpoint_R = (X_robot_dpoint_R-X_robot_dpoint_R_old)/0.05;
    Y_robot_ddpoint_R = (Y_robot_dpoint_R-Y_robot_dpoint_R_old)/0.05;
    Z_robot_ddpoint_R = (Z_robot_dpoint_R-Z_robot_dpoint_R_old)/0.05;

    Joint_1_speed_R = (Joint_1_angle_R - Joint_1_angle_R_old)/0.05;
    Joint_2_speed_R = (Joint_2_angle_R - Joint_2_angle_R_old)/0.05;
    Joint_3_speed_R = (Joint_3_angle_R - Joint_3_angle_R_old)/0.05;
    Joint_4_speed_R = (Joint_4_angle_R - Joint_4_angle_R_old)/0.05;
    Joint_5_speed_R = (Joint_5_angle_R - Joint_5_angle_R_old)/0.05;
    Joint_6_speed_R = (Joint_6_angle_R - Joint_6_angle_R_old)/0.05;

    Joint_1_dspeed_R = (Joint_1_speed_R - Joint_1_speed_R_old)/0.05;
    Joint_2_dspeed_R = (Joint_2_speed_R - Joint_2_speed_R_old)/0.05;
    Joint_3_dspeed_R = (Joint_3_speed_R - Joint_3_speed_R_old)/0.05;
    Joint_4_dspeed_R = (Joint_4_speed_R - Joint_4_speed_R_old)/0.05;
    Joint_5_dspeed_R = (Joint_5_speed_R - Joint_5_speed_R_old)/0.05;
    Joint_6_dspeed_R = (Joint_6_speed_R - Joint_6_speed_R_old)/0.05;
    X_robot_point_R_old = X_robot_point_R;
    Y_robot_point_R_old = Y_robot_point_R;
    Z_robot_point_R_old = Z_robot_point_R;
    X_robot_dpoint_R_old = X_robot_dpoint_R;
    Y_robot_dpoint_R_old = Y_robot_dpoint_R;
    Z_robot_dpoint_R_old = Z_robot_dpoint_R;

    Joint_1_angle_R_old = Joint_1_angle_R;
    Joint_2_angle_R_old = Joint_2_angle_R;
    Joint_3_angle_R_old = Joint_3_angle_R;
    Joint_4_angle_R_old = Joint_4_angle_R;
    Joint_5_angle_R_old = Joint_5_angle_R;
    Joint_6_angle_R_old = Joint_6_angle_R;

    Joint_1_speed_R_old = Joint_1_speed_R;
    Joint_2_speed_R_old = Joint_2_speed_R;
    Joint_3_speed_R_old = Joint_3_speed_R;
    Joint_4_speed_R_old = Joint_4_speed_R;
    Joint_5_speed_R_old = Joint_5_speed_R;
    Joint_6_speed_R_old = Joint_6_speed_R;
}

void Robot::Fixed_angle_calculate()
{
    Eigen::MatrixXd X(3,3);
    Eigen::MatrixXd Y(3,3);
    Eigen::MatrixXd Z(3,3);
    X << 1,                        0,                       0,
         0,   cos(FA_X_tool_angle_C), -sin(FA_X_tool_angle_C),
         0,   sin(FA_X_tool_angle_C),  cos(FA_X_tool_angle_C);
    Y << cos(FA_Y_tool_angle_C),   0,  sin(FA_Y_tool_angle_C),
                              0,   1,                       0,
        -sin(FA_Y_tool_angle_C),   0,  cos(FA_Y_tool_angle_C);
    Z << cos(FA_Z_tool_angle_C), -sin(FA_Z_tool_angle_C),   0,
         sin(FA_Z_tool_angle_C),  cos(FA_Z_tool_angle_C),   0,
                              0,                       0,   1;
    R6_0_C = Z*Y*X;
}

void Robot::Fixed_angle_invcalculate()
{
    FA_Y_tool_angle_R = atan2(-Tt_0_R(2,0),sqrt(pow(Tt_0_R(0,0),2) + pow(Tt_0_R(1,0),2)));
    if(round(FA_Y_tool_angle_R*180/M_PI*1)/1 == 90) {
        FA_Z_tool_angle_R = 0;
        FA_X_tool_angle_R = atan2(Tt_0_R(0,1),Tt_0_R(1,1));
    }
    if(round(FA_Y_tool_angle_R*180/M_PI*1)/1 == -90) {
        FA_Z_tool_angle_R = 0;
        FA_X_tool_angle_R = -atan2(Tt_0_R(0,1),Tt_0_R(1,1));
    }
    else {
        FA_Z_tool_angle_R = atan2(Tt_0_R(1,0)/cos(FA_Y_tool_angle_R),Tt_0_R(0,0)/cos(FA_Y_tool_angle_R));
        FA_X_tool_angle_R = atan2(Tt_0_R(2,1)/cos(FA_Y_tool_angle_R),Tt_0_R(2,2)/cos(FA_Y_tool_angle_R));
    }
    std::cout<<"XA = "<<FA_X_tool_angle_R<<std::endl;
    std::cout<<"YA = "<<FA_Y_tool_angle_R<<std::endl;
    std::cout<<"ZA = "<<FA_Z_tool_angle_R<<std::endl;
}

void Robot::get_joint_speed_first(void)
{
    Joint_1_speed_R = (Joint_1_angle_C-Joint_1_angle_R)/0.05;
    Joint_2_speed_R = (Joint_2_angle_C-Joint_2_angle_R)/0.05;
    Joint_3_speed_R = (Joint_3_angle_C-Joint_3_angle_R)/0.05;
    Joint_4_speed_R = (Joint_4_angle_C-Joint_4_angle_R)/0.05;
    Joint_5_speed_R = (Joint_5_angle_C-Joint_5_angle_R)/0.05;
    Joint_6_speed_R = (Joint_6_angle_C-Joint_6_angle_R)/0.05;
    //电机段最大5000rpm/min == 523rad/s == speed*z
    if(Joint_1_speed_R > 2.7)//Z1 == 192
        Joint_1_speed_R = 2.7;
    if(Joint_1_speed_R < -2.7)
        Joint_1_speed_R = -2.7;
    if(Joint_2_speed_R > 3.5)// Z2 == 150
        Joint_2_speed_R = 3.5;
    if(Joint_2_speed_R < -3.5)
        Joint_2_speed_R = -3.5;
    if(Joint_3_speed_R > 8.4)// Z3 == 62.5
        Joint_3_speed_R = 8.4;
    if(Joint_3_speed_R < -8.4)
        Joint_3_speed_R = -8.4;
    if(Joint_4_speed_R > 2.7)//Z4 == 192
        Joint_4_speed_R = 2.7;
    if(Joint_4_speed_R < -2.7)
        Joint_4_speed_R = -2.7;
    if(Joint_5_speed_R > 5.5)//Z5 == 95.4
        Joint_5_speed_R = 5.5;
    if(Joint_5_speed_R < -5.5)
        Joint_5_speed_R = -5.5;
    if(Joint_6_speed_R > 10.5)//Z6 == 50
        Joint_6_speed_R = 10.5;
    if(Joint_6_speed_R < -10.5)
        Joint_6_speed_R = -10.5;
    if(isnan(Joint_1_angle_C) || isnan(Joint_2_angle_C) || isnan(Joint_3_angle_C)) {
        Joint_1_speed_R = Joint_2_speed_R = Joint_3_speed_R = 0;
        errror_position = 1;
    }
    Joint_1_angle_R = Joint_1_angle_R + Joint_1_speed_R*0.05;
    Joint_2_angle_R = Joint_2_angle_R + Joint_2_speed_R*0.05;
    Joint_3_angle_R = Joint_3_angle_R + Joint_3_speed_R*0.05;
    Joint_4_angle_R = Joint_4_angle_R + Joint_4_speed_R*0.05;
    Joint_5_angle_R = Joint_5_angle_R + Joint_5_speed_R*0.05;
    Joint_6_angle_R = Joint_6_angle_R + Joint_6_speed_R*0.05;

}

void Robot::get_joint_speed_second(void)
{
    Joint_4_speed_R = (Joint_4_angle_C-Joint_4_angle_R)/0.05;
    Joint_5_speed_R = (Joint_5_angle_C-Joint_5_angle_R)/0.05;
    Joint_6_speed_R = (Joint_6_angle_C-Joint_6_angle_R)/0.05;
    //电机段最大3000rpm/min == 314rad/s == speed*z
    if(Joint_4_speed_R > 3)
        Joint_4_speed_R = 3;
    if(Joint_4_speed_R < -3)
        Joint_4_speed_R = -3;
    if(Joint_5_speed_R > 3.2)
        Joint_5_speed_R = 3.2;
    if(Joint_5_speed_R < -3.2)
        Joint_5_speed_R = -3.2;
    if(Joint_6_speed_R > 6)
        Joint_6_speed_R = 6;
    if(Joint_6_speed_R < -6)
        Joint_6_speed_R = -6;
    std::cout<<"Joint_4_angle_R = "<<Joint_4_angle_R<<std::endl;
    std::cout<<"Joint_5_angle_R = "<<Joint_5_angle_R<<std::endl;
    std::cout<<"Joint_6_angle_R = "<<Joint_6_angle_R<<std::endl;
    std::cout<<"\n "<<std::endl;
}

void Robot::Back_Init_Position()
{

}

void Robot::Linear_Speed_Plan(float finish_angle,float init_angle,float *joint_num)
{
    float v = (finish_angle-init_angle)/4;
    float dv1 = v;
    float dv2 = -v;
    float t = 0.05;
    for(uint i = 0;i<20;i++) {
        joint_num[i] = init_angle + 0.5*dv1*pow(t,2);
        t += 0.05;
    }
    for(uint i = 20;i<80;i++) {
        joint_num[i] = init_angle + v*(t-0.5);
        t += 0.05;
    }
    for(uint i = 80;i<100;i++) {
        joint_num[i] = init_angle +v*(t-0.5)+0.5*dv2*pow((t-4),2);
        t += 0.05;
    }
}

void Robot::Get_Joint_Angle_first(void)
{
    Joint_1_angle_R = Joint_1_angle_R + Joint_1_speed_R*0.05;
    Joint_2_angle_R = Joint_2_angle_R + Joint_2_speed_R*0.05;
    Joint_3_angle_R = Joint_3_angle_R + Joint_3_speed_R*0.05;
}

void Robot::Get_Joint_Angle_second(void)
{
    Joint_4_angle_R = Joint_4_angle_R + Joint_4_speed_R*0.05;
    Joint_5_angle_R = Joint_5_angle_R + Joint_5_speed_R*0.05;
    Joint_6_angle_R = Joint_6_angle_R + Joint_6_speed_R*0.05;
}
