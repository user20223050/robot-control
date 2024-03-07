#include "robot.h"

Slave_Copmputer::Slave_Copmputer()//帧格式[帧头 脉冲频率*2 旋转方向 CRC校验 帧尾]
{   //开始跟踪数据帧
    Start_T[0] = 0xbb;
    Start_T[1] = 0xbb;
    Start_T[2] = 0xbb;
    Start_T[3] = 0xbb;
    //紧急停止数据帧
    STOP[0] = 0x1f;
    STOP[1] = 0x1f;
    STOP[2] = 0x1f;
    STOP[3] = 0x1f;
    //数据冲突数据帧
    ERROR[0] = 0xee;
    ERROR[1] = 0xee;
    ERROR[2] = 0xee;
    ERROR[3] = 0xee;
    //轨迹跟踪段帧头
    buffer1[0] = 0x66;
    buffer2[0] = 0x66;
    buffer3[0] = 0x66;
    buffer4[0] = 0x66;
    buffer5[0] = 0x66;
    buffer6[0] = 0x66;
    //轨迹跟踪段帧尾
    buffer1[5] = 0x88;
    buffer2[5] = 0x88;
    buffer3[5] = 0x88;
    buffer4[5] = 0x88;
    buffer5[5] = 0x88;
    buffer6[5] = 0x88;
    //CRC段先全为00
    buffer1[4] = 0x11;
    buffer2[4] = 0x11;
    buffer3[4] = 0x11;
    buffer4[4] = 0x11;
    buffer5[4] = 0x11;
    buffer6[4] = 0x11;

    pulse1 = 65535;
    pulse2 = 65535;
    pulse3 = 65535;
    pulse4 = 65535;
    pulse5 = 65535;
    pulse6 = 65535;

    state_sign = 0;
    Init_sign = 0;
    Work_sign = 0;
    stop_sign = 0;
}

void Slave_Copmputer::calculate_pulse(Robot &robot)
{
    pulse1 = 42000000*(2*M_PI)/(abs(robot.Joint_1_speed_R)*6000*robot.Z1);//计算脉冲频率 50MS计算一次
    pulse2 = 42000000*(2*M_PI)/(abs(robot.Joint_2_speed_R)*6000*robot.Z2);
    pulse3 = 42000000*(2*M_PI)/(abs(robot.Joint_3_speed_R)*6000*robot.Z3);
    pulse4 = 42000000*(2*M_PI)/(abs(robot.Joint_4_speed_R)*6000*robot.Z4);
    pulse5 = 42000000*(2*M_PI)/(abs(robot.Joint_5_speed_R)*6000*robot.Z5);
    pulse6 = 42000000*(2*M_PI)/(abs(robot.Joint_6_speed_R)*6000*robot.Z6);

    if(pulse1 == 0 || pulse1 > 65535)
    pulse1 = 65535;
    if(pulse2 == 0 || pulse2 > 65535)
    pulse2 = 65535;
    if(pulse3 == 0 || pulse3 > 65535)
    pulse3 = 65535;
    if(pulse4 == 0 || pulse4 > 65535)
    pulse4 = 65535;
    if(pulse5 == 0 || pulse5 > 65535)
    pulse5 = 65535;
    if(pulse6 == 0 || pulse6 > 65535)
    pulse6 = 65535;
}

void Slave_Copmputer::joint_rotate_direction(Robot &robot)
{
    static int first_in = 0;

    static float joint_1_angle_old;
    static float joint_2_angle_old;
    static float joint_3_angle_old;
    static float joint_4_angle_old;
    static float joint_5_angle_old;
    static float joint_6_angle_old;

    if(first_in >= 1)
    {
        if((round(robot.Joint_1_angle_R * 10000) / 10000) - (round(joint_1_angle_old * 10000) / 10000) > 0)
            Joint1_rotate_direction = 1;
        if((round(robot.Joint_1_angle_R * 10000) / 10000) - (round(joint_1_angle_old * 10000) / 10000) < 0)
            Joint1_rotate_direction = 0;
        if((round(robot.Joint_2_angle_R * 10000) / 10000) - (round(joint_2_angle_old * 10000) / 10000) > 0)
            Joint2_rotate_direction = 0;
        if((round(robot.Joint_2_angle_R * 10000) / 10000) - (round(joint_2_angle_old * 10000) / 10000) < 0)
            Joint2_rotate_direction = 1;
        if((round(robot.Joint_3_angle_R * 10000) / 10000) - (round(joint_3_angle_old * 10000) / 10000) > 0)
            Joint3_rotate_direction = 1;
        if((round(robot.Joint_3_angle_R * 10000) / 10000) - (round(joint_3_angle_old * 10000) / 10000) < 0)
            Joint3_rotate_direction = 0;
        if((round(robot.Joint_4_angle_R * 10000) / 10000) - (round(joint_4_angle_old * 10000) / 10000) > 0)
            Joint4_rotate_direction = 1;
        if((round(robot.Joint_4_angle_R * 10000) / 10000) - (round(joint_4_angle_old * 10000) / 10000) < 0)
            Joint4_rotate_direction = 0;
        if((round(robot.Joint_5_angle_R * 10000) / 10000) - (round(joint_5_angle_old * 10000) / 10000) > 0)
            Joint5_rotate_direction = 0;
        if((round(robot.Joint_5_angle_R * 10000) / 10000) - (round(joint_5_angle_old * 10000) / 10000) < 0)
            Joint5_rotate_direction = 1;
        first_in = 2;//防止溢出
    }
    joint_1_angle_old = robot.Joint_1_angle_R;
    joint_2_angle_old = robot.Joint_2_angle_R;
    joint_3_angle_old = robot.Joint_3_angle_R;
    joint_4_angle_old = robot.Joint_4_angle_R;
    joint_5_angle_old = robot.Joint_5_angle_R;
    joint_6_angle_old = robot.Joint_6_angle_R;
    first_in++;
}

void Slave_Copmputer::Serial_message(Robot &robot)
{
    joint_rotate_direction(robot);
    uchar High_Byte;
    uchar Low_Byte;

    Low_Byte = pulse1&0xff;
    High_Byte = (pulse1>>8)&0xff;
    buffer1[1] = (static_cast<int>(Low_Byte));
    buffer1[2] = (static_cast<int>(High_Byte));
    if(Joint1_rotate_direction == 1)
    buffer1[3] = 0x00;
    else
    buffer1[3] = 0x01;

    Low_Byte = pulse2&0xff;
    High_Byte = (pulse2>>8)&0xff;
    buffer2[1] = (static_cast<int>(Low_Byte));
    buffer2[2] = (static_cast<int>(High_Byte));
    if(Joint2_rotate_direction == 1)
    buffer2[3] = 0x01;
    else
    buffer2[3] = 0x00;

    Low_Byte = pulse3&0xff;
    High_Byte = (pulse3>>8)&0xff;
    buffer3[1] = (static_cast<int>(Low_Byte));
    buffer3[2] = (static_cast<int>(High_Byte));
    if(Joint3_rotate_direction == 1)
    buffer3[3] = 0x01;
    else
    buffer3[3] = 0x00;

    Low_Byte = pulse4&0xff;
    High_Byte = (pulse4>>8)&0xff;
    buffer4[1] = (static_cast<int>(Low_Byte));
    buffer4[2] = (static_cast<int>(High_Byte));
    if(Joint4_rotate_direction == 1)
    buffer4[3] = 0x01;
    else
    buffer4[3] = 0x00;

    Low_Byte = pulse5&0xff;
    High_Byte = (pulse5>>8)&0xff;
    buffer5[1] = (static_cast<int>(Low_Byte));
    buffer5[2] = (static_cast<int>(High_Byte));
    if(Joint5_rotate_direction == 1)
    buffer5[3] = 0x00;
    else
    buffer5[3] = 0x01;
}

void Slave_Copmputer::Follow_Mobj(MPC_Control& mpc,Robot &robot,mobile_pose &mobile)
{
    static uint8_t sign = 0;
    mobile.refresh_object();
    mobile.reflash_target();
    float u_x = mpc.Prediction(mpc.X_Error.col(0), mpc.E,mpc.H, mpc.N);
    float u_y = mpc.Prediction(mpc.Y_Error.col(0), mpc.E,mpc.H, mpc.N);
    float u_z = mpc.Prediction(mpc.Z_Error.col(0), mpc.E,mpc.H, mpc.N);

    mpc.Calculate_Out_X(u_x,robot,*this);
    mpc.Calculate_Out_Y(u_y,robot,*this);
    mpc.Calculate_Out_Z(u_z,robot,*this);
    robot.Three_Ik();
    robot.get_joint_speed_first();
    robot.Get_Joint_Angle_first();
    if(robot.errror_position == 1) {
        serialPort->write(STOP);
    }
    if((abs(mpc.X_Error(0,0)) < 10.0 && abs(mpc.Y_Error(0,0)) < 10.0 && abs(mpc.Z_Error(0,0)) < 10.0) || (sign == 1)) {
        float u_xa = mpc.Prediction(mpc.XA_Error.col(0), mpc.E,mpc.H, mpc.N);
        float u_ya = mpc.Prediction(mpc.YA_Error.col(0), mpc.E,mpc.H, mpc.N);
        float u_za = mpc.Prediction(mpc.ZA_Error.col(0), mpc.E,mpc.H, mpc.N);
        mpc.Calculate_Out_ZA(u_za,robot);
        mpc.Calculate_Out_XA(u_xa,robot);
        mpc.Calculate_Out_YA(u_ya,robot);
        robot.zyzEuler_Ik();
        robot.get_joint_speed_second();
        robot.Get_Joint_Angle_second();
        sign = 1;
    }
//    if(Work_sign == 0){
//        if(robot.X_tool_point_R>300 || robot.X_tool_point_R<-5 || robot.Y_tool_point_R<-250 || robot.Y_tool_point_R>250 || robot.Z_tool_point_R>300 || robot.Z_tool_point_R<-100){
//            serialPort->write(STOP);
//            stop_sign = 1;
//            mpc.Calculate_Out_X(0,robot,*this);
//            mpc.Calculate_Out_Y(0,robot,*this);
//            mpc.Calculate_Out_Z(0,robot,*this);
//        }
//    }
    robot.FK();
    mpc.Refresh_Error(robot,mobile);

    calculate_pulse(robot);
    Serial_message(robot);
    serialPort->write(buffer1);
    serialPort->write(buffer2);
    serialPort->write(buffer3);
    serialPort->write(buffer4);
    serialPort->write(buffer5);
    serialPort->write(buffer6);
//    std::cout<<"pulse1 = "<<pulse1<<std::endl;
//    std::cout<<"pulse2 = "<<pulse2<<std::endl;
//    std::cout<<"pulse3 = "<<pulse3<<std::endl;
//    std::cout<<"pulse5 = "<<pulse5<<std::endl;
}

void Slave_Copmputer::Work_position_set(Robot &robot)
{
    uchar High_Byte;
    uchar Low_Byte;
    static uint8_t i = 1;
    float Joint1_speed;
    float Joint2_speed;
    float Joint3_speed;
    float Joint4_speed;
    float Joint5_speed;
    float Joint6_speed;
    Joint1_speed = (robot.joint1_buf[i] - robot.joint1_buf[i-1])/0.05;
    robot.Joint_1_angle_R = robot.joint1_buf[i];
    uint Pulse1 = 42000000*(2*M_PI)/(abs(Joint1_speed)*6000*robot.Z1);
    Low_Byte = Pulse1&0xff;
    High_Byte = (Pulse1>>8)&0xff;
    buffer1[1] = (static_cast<int>(Low_Byte));
    buffer1[2] = (static_cast<int>(High_Byte));
    if((robot.joint1_buf[i] - robot.joint1_buf[i-1]) > 0)
        buffer1[3] = 0x01;
    else
        buffer1[3] = 0x00;
    Joint2_speed = (robot.joint2_buf[i] - robot.joint2_buf[i-1])/0.05;
    robot.Joint_2_angle_R = robot.joint2_buf[i];
    uint Pulse2 = 42000000*(2*M_PI)/(abs(Joint2_speed)*6000*robot.Z2);
    Low_Byte = Pulse2&0xff;
    High_Byte = (Pulse2>>8)&0xff;
    buffer2[1] = (static_cast<int>(Low_Byte));
    buffer2[2] = (static_cast<int>(High_Byte));
    if((robot.joint2_buf[i] - robot.joint2_buf[i-1]) > 0)
        buffer2[3] = 0x01;
    else
        buffer2[3] = 0x00;
    Joint3_speed = (robot.joint3_buf[i] - robot.joint3_buf[i-1])/0.05;
    robot.Joint_3_angle_R = robot.joint3_buf[i];
    uint Pulse3 = 42000000*(2*M_PI)/(abs(Joint3_speed)*6000*robot.Z3);
    Low_Byte = Pulse3&0xff;
    High_Byte = (Pulse3>>8)&0xff;
    buffer3[1] = (static_cast<int>(Low_Byte));
    buffer3[2] = (static_cast<int>(High_Byte));
    if((robot.joint3_buf[i] - robot.joint3_buf[i-1]) > 0)
        buffer3[3] = 0x01;
    else
        buffer3[3] = 0x00;
    Joint4_speed = (robot.joint4_buf[i] - robot.joint4_buf[i-1])/0.05;
    robot.Joint_4_angle_R = robot.joint4_buf[i];
    uint Pulse4 = 42000000*(2*M_PI)/(abs(Joint4_speed)*6000*robot.Z4);
    Low_Byte = Pulse4&0xff;
    High_Byte = (Pulse4>>8)&0xff;
    buffer4[1] = (static_cast<int>(Low_Byte));
    buffer4[2] = (static_cast<int>(High_Byte));
    if((robot.joint4_buf[i] - robot.joint4_buf[i-1]) > 0)
        buffer4[3] = 0x01;
    else
        buffer4[3] = 0x00;
    Joint5_speed = (robot.joint5_buf[i] - robot.joint5_buf[i-1])/0.05;
    robot.Joint_5_angle_R = robot.joint5_buf[i];
    uint Pulse5 = 42000000*(2*M_PI)/(abs(Joint5_speed)*6000*robot.Z5);
    Low_Byte = Pulse5&0xff;
    High_Byte = (Pulse5>>8)&0xff;
    buffer5[1] = (static_cast<int>(Low_Byte));
    buffer5[2] = (static_cast<int>(High_Byte));
    if((robot.joint5_buf[i] - robot.joint5_buf[i-1]) > 0)
        buffer5[3] = 0x01;
    else
        buffer5[3] = 0x00;
    Joint6_speed = (robot.joint6_buf[i] - robot.joint6_buf[i-1])/0.05;
    robot.Joint_6_angle_R = robot.joint6_buf[i];
    uint Pulse6 = 42000000*(2*M_PI)/(abs(Joint6_speed)*6000*robot.Z6);
    Low_Byte = Pulse6&0xff;
    High_Byte = (Pulse6>>8)&0xff;
    buffer6[1] = (static_cast<int>(Low_Byte));
    buffer6[2] = (static_cast<int>(High_Byte));
    if((robot.joint6_buf[i] - robot.joint6_buf[i-1]) > 0)
        buffer6[3] = 0x01;
    else
        buffer6[3] = 0x00;
    serialPort->write(buffer1);
    serialPort->write(buffer2);
    serialPort->write(buffer3);
    serialPort->write(buffer4);
    serialPort->write(buffer5);
    serialPort->write(buffer6);
    i++;
    if(i == 100) {
        i = 0;
        state_sign = 2;
    }
     std::cout<<"i = "<<static_cast<int>(i)<<std::endl;
}

