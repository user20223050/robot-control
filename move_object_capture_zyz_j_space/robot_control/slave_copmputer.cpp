#include "slave_copmputer.h"

const uint16_t t[256] ={0x0000,0xC0C1,0xC181,0x0140,0xC301,0x03C0,0x0280,0xC241,0xC601,0x06C0,0x0780,0xC741,0x0500,0xC5C1,0xC481,0x0440,0xCC01,0x0CC0,0x0D80,0xCD41,0x0F00,0xCFC1,0xCE81,0x0E40,0x0A00,0xCAC1,0xCB81,0x0B40,0xC901,0x09C0,0x0880,0xC841,0xD801,0x18C0,0x1980,0xD941,0x1B00,0xDBC1,0xDA81,0x1A40,0x1E00,0xDEC1,0xDF81,0x1F40,0xDD01,0x1DC0,0x1C80,0xDC41,0x1400,0xD4C1,0xD581,0x1540,0xD701,0x17C0,0x1680,0xD641,0xD201,0x12C0,0x1380,0xD341,0x1100,0xD1C1,0xD081,0x1040,0xF001,0x30C0,0x3180,0xF141,0x3300,0xF3C1,0xF281,0x3240,0x3600,0xF6C1,0xF781,0x3740,0xF501,0x35C0,0x3480,0xF441,0x3C00,0xFCC1,0xFD81,0x3D40,0xFF01,0x3FC0,0x3E80,0xFE41,0xFA01,0x3AC0,0x3B80,0xFB41,0x3900,0xF9C1,0xF881,0x3840,0x2800,0xE8C1,0xE981,0x2940,0xEB01,0x2BC0,0x2A80,0xEA41,0xEE01,0x2EC0,0x2F80,0xEF41,0x2D00,0xEDC1,0xEC81,0x2C40,0xE401,0x24C0,0x2580,0xE541,0x2700,0xE7C1,0xE681,0x2640,0x2200,0xE2C1,0xE381,0x2340,0xE101,0x21C0,0x2080,0xE041,0xA001,0x60C0,0x6180,0xA141,0x6300,0xA3C1,0xA281,0x6240,0x6600,0xA6C1,0xA781,0x6740,0xA501,0x65C0,0x6480,0xA441,0x6C00,0xACC1,0xAD81,0x6D40,0xAF01,0x6FC0,0x6E80,0xAE41,0xAA01,0x6AC0,0x6B80,0xAB41,0x6900,0xA9C1,0xA881,0x6840,0x7800,0xB8C1,0xB981,0x7940,0xBB01,0x7BC0,0x7A80,0xBA41,0xBE01,0x7EC0,0x7F80,0xBF41,0x7D00,0xBDC1,0xBC81,0x7C40,0xB401,0x74C0,0x7580,0xB541,0x7700,0xB7C1,0xB681,0x7640,0x7200,0xB2C1,0xB381,0x7340,0xB101,0x71C0,0x7080,0xB041,0x5000,0x90C1,0x9181,0x5140,0x9301,0x53C0,0x5280,0x9241,0x9601,0x56C0,0x5780,0x9741,0x5500,0x95C1,0x9481,0x5440,0x9C01,0x5CC0,0x5D80,0x9D41,0x5F00,0x9FC1,0x9E81,0x5E40,0x5A00,0x9AC1,0x9B81,0x5B40,0x9901,0x59C0,0x5880,0x9841,0x8801,0x48C0,0x4980,0x8941,0x4B00,0x8BC1,0x8A81,0x4A40,0x4E00,0x8EC1,0x8F81,0x4F40,0x8D01,0x4DC0,0x4C80,0x8C41,0x4400,0x84C1,0x8581,0x4540,0x8701,0x47C0,0x4680,0x8641,0x8201,0x42C0,0x4380,0x8341,0x4100,0x81C1,0x8081,0x4040};

Slave_Copmputer::Slave_Copmputer()//帧格式[帧头 脉冲频率*2 旋转方向 CRC校验 帧尾]
{   //开始跟踪数据帧
    Start_T = new char[4];
    STOP =    new char[4];
    ERROR =   new char[4];
    buffer1 = new char[6];
    buffer2 = new char[6];
    buffer3 = new char[6];
    buffer4 = new char[6];
    buffer5 = new char[6];
    buffer6 = new char[6];

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
    //CRC校验低字节
    buffer1[4] = 0xff;
    buffer2[4] = 0xff;
    buffer3[4] = 0xff;
    buffer4[4] = 0xff;
    buffer5[4] = 0xff;
    buffer6[4] = 0xff;
    std::cout<<"buffer6[4] "<<buffer6[0]<<std::endl;
    //CRC校验高字节
    buffer1[5] = 0xff;
    buffer2[5] = 0xff;
    buffer3[5] = 0xff;
    buffer4[5] = 0xff;
    buffer5[5] = 0xff;
    buffer6[5] = 0xff;

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
    if(robot.Joint_1_speed_R == 0)
        pulse1 = 0xffff;
    else
    pulse1 = 42000000*(2*M_PI)/(abs(robot.Joint_1_speed_R)*6000*robot.Z1);//计算脉冲频率 50MS计算一次
    if(robot.Joint_2_speed_R == 0)
        pulse2 = 0xffff;
    else
    pulse2 = 42000000*(2*M_PI)/(abs(robot.Joint_2_speed_R)*6000*robot.Z2);
    if(robot.Joint_3_speed_R == 0)
        pulse3 = 0xffff;
    else
    pulse3 = 42000000*(2*M_PI)/(abs(robot.Joint_3_speed_R)*6000*robot.Z3);
    if(robot.Joint_4_speed_R == 0)
        pulse4 = 0xffff;
    else
    pulse4 = 42000000*(2*M_PI)/(abs(robot.Joint_4_speed_R)*6000*robot.Z4);
    if(robot.Joint_5_speed_R == 0)
        pulse5 = 0xffff;
    else
    pulse5 = 42000000*(2*M_PI)/(abs(robot.Joint_5_speed_R)*6000*robot.Z5);
    if(robot.Joint_6_speed_R == 0)
        pulse6 = 0xffff;
    else
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
    buffer1[1] = ((char)Low_Byte);
    buffer1[2] = ((char)High_Byte);
    if(Joint1_rotate_direction == 1)
    buffer1[3] = 0x00;
    else
    buffer1[3] = 0x01;
    CRC_cheak(buffer1);
    Low_Byte = pulse2&0xff;
    High_Byte = (pulse2>>8)&0xff;
    buffer2[1] = ((char)Low_Byte);
    buffer2[2] = ((char)High_Byte);
    if(Joint2_rotate_direction == 1)
    buffer2[3] = 0x01;
    else
    buffer2[3] = 0x00;
    CRC_cheak(buffer2);
    Low_Byte = pulse3&0xff;
    High_Byte = (pulse3>>8)&0xff;
    buffer3[1] = ((char)Low_Byte);
    buffer3[2] = ((char)High_Byte);
    if(Joint3_rotate_direction == 1)
    buffer3[3] = 0x01;
    else
    buffer3[3] = 0x00;
    CRC_cheak(buffer3);
    Low_Byte = pulse4&0xff;
    High_Byte = (pulse4>>8)&0xff;
    buffer4[1] = ((char)Low_Byte);
    buffer4[2] = ((char)High_Byte);
    if(Joint4_rotate_direction == 1)
    buffer4[3] = 0x01;
    else
    buffer4[3] = 0x00;
    CRC_cheak(buffer4);
    Low_Byte = pulse5&0xff;
    High_Byte = (pulse5>>8)&0xff;
    buffer5[1] = ((char)Low_Byte);
    buffer5[2] = ((char)High_Byte);
    if(Joint5_rotate_direction == 1)
    buffer5[3] = 0x00;
    else
    buffer5[3] = 0x01;
    CRC_cheak(buffer5);
}

void Slave_Copmputer::CRC_cheak(char* data)
{
    uint16_t crc = 0xffff;
    uint8_t zj;
    for (uint8_t i= 0; i < 4 ; i++)
    {
        zj = (crc & 0xFF) ^ data[i];
        crc >>= 8;
        crc^=t[zj];
    }
    data[4] = (char)(crc & 0xff);
    data[5] = (char)(crc >> 8);
}

void Slave_Copmputer::Follow_Mobj(MPC_Control& mpc,Robot &robot,mobile_pose &mobile)
{
    mobile.refresh_object();
    mobile.reflash_target_s();
    mpc.dasign = 2;
    if(abs(mpc.J1_Error(0)) < 0.18)
    {
        uint8_t run_state_old = mpc.run_state;
        mpc.run_state = 2;
        if(run_state_old != mpc.run_state) {
            mpc.change_weight();
        }
    }
    else{
        uint8_t run_state_old = mpc.run_state;
        mpc.run_state = 1;
        if(run_state_old != mpc.run_state) {
            mpc.change_weight();
        }
    }
    std::cout<<"mpc.run_state = "<<mpc.run_state<<std::endl;
    double u_J1 = mpc.Prediction(mpc.J1_Error.col(0), mpc.E,mpc.H, mpc.N);
    if(abs(mpc.J2_Error(0)) < 0.18)
    {
        uint8_t run_state_old = mpc.run_state;
        mpc.run_state = 2;
        if(run_state_old != mpc.run_state) {
            mpc.change_weight();
        }
    }
    else{
        uint8_t run_state_old = mpc.run_state;
        mpc.run_state = 1;
        if(run_state_old != mpc.run_state) {
            mpc.change_weight();
        }
    }
    std::cout<<"mpc.run_state = "<<mpc.run_state<<std::endl;
    double u_J2 = mpc.Prediction(mpc.J2_Error.col(0), mpc.E,mpc.H, mpc.N);
    if(abs(mpc.J3_Error(0)) < 0.18)
    {
        uint8_t run_state_old = mpc.run_state;
        mpc.run_state = 2;
        if(run_state_old != mpc.run_state) {
            mpc.change_weight();
        }
    }
    else{
        uint8_t run_state_old = mpc.run_state;
        mpc.run_state = 1;
        if(run_state_old != mpc.run_state) {
            mpc.change_weight();
        }
    }
    std::cout<<"mpc.run_state = "<<mpc.run_state<<std::endl;
    double u_J3 = mpc.Prediction(mpc.J3_Error.col(0), mpc.E,mpc.H, mpc.N);
    if(abs(mpc.J4_Error(0)) < 0.18)
    {
        uint8_t run_state_old = mpc.run_state;
        mpc.run_state = 2;
        if(run_state_old != mpc.run_state) {
            mpc.change_weight();
        }
    }
    else{
        uint8_t run_state_old = mpc.run_state;
        mpc.run_state = 1;
        if(run_state_old != mpc.run_state) {
            mpc.change_weight();
        }
    }
    std::cout<<"mpc.run_state = "<<mpc.run_state<<std::endl;
    double u_J4 = mpc.Prediction(mpc.J4_Error.col(0), mpc.E,mpc.H, mpc.N);
    if(abs(mpc.J5_Error(0)) < 0.18)
    {
        uint8_t run_state_old = mpc.run_state;
        mpc.run_state = 2;
        if(run_state_old != mpc.run_state) {
            mpc.change_weight();
        }
    }
    else{
        uint8_t run_state_old = mpc.run_state;
        mpc.run_state = 1;
        if(run_state_old != mpc.run_state) {
            mpc.change_weight();
        }
    }
    std::cout<<"mpc.run_state = "<<mpc.run_state<<std::endl;
    double u_J5 = mpc.Prediction(mpc.J5_Error.col(0), mpc.E,mpc.H, mpc.N);
    if(abs(mpc.J6_Error(0)) < 0.18)
    {
        uint8_t run_state_old = mpc.run_state;
        mpc.run_state = 2;
        if(run_state_old != mpc.run_state) {
            mpc.change_weight();
        }
    }
    else{
        uint8_t run_state_old = mpc.run_state;
        mpc.run_state = 1;
        if(run_state_old != mpc.run_state) {
            mpc.change_weight();
        }
    }
    std::cout<<"mpc.run_state = "<<mpc.run_state<<std::endl;
    double u_J6 = mpc.Prediction(mpc.J6_Error.col(0), mpc.E,mpc.H, mpc.N);
    mpc.Calculate_Out_X(u_J1,robot,*this,mobile);
    mpc.Calculate_Out_Y(u_J2,robot,*this,mobile);
    mpc.Calculate_Out_Z(u_J3,robot,*this,mobile);
    mpc.Calculate_Out_J4(u_J4,robot,mobile);
    mpc.Calculate_Out_J5(u_J5,robot,mobile);
    mpc.Calculate_Out_J6(u_J6,robot,mobile);
    robot.get_joint_speed_first();
//    robot.Get_Joint_Angle_first();
//    robot.get_joint_speed_second();
//    robot.Get_Joint_Angle_second();
    if(robot.errror_position == 1) {
        serialPort->write(reinterpret_cast< const char *>(STOP));
    }
    robot.FK();
    mpc.Refresh_Error(robot,mobile);
    calculate_pulse(robot);
    Serial_message(robot);
    serialPort->write(buffer1,6);
    serialPort->write(buffer2,6);
    serialPort->write(buffer3,6);
    serialPort->write(buffer4,6);
    serialPort->write(buffer5,6);
    serialPort->write(buffer6,6);
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
    buffer1[1] = ((char)Low_Byte);
    buffer1[2] = ((char)High_Byte);
    if((robot.joint1_buf[i] - robot.joint1_buf[i-1]) > 0)
        buffer1[3] = 0x01;
    else
        buffer1[3] = 0x00;
    CRC_cheak(buffer1);
    Joint2_speed = (robot.joint2_buf[i] - robot.joint2_buf[i-1])/0.05;
    robot.Joint_2_angle_R = robot.joint2_buf[i];
    uint Pulse2 = 42000000*(2*M_PI)/(abs(Joint2_speed)*6000*robot.Z2);
    Low_Byte = Pulse2&0xff;
    High_Byte = (Pulse2>>8)&0xff;
    buffer2[1] = ((char)Low_Byte);
    buffer2[2] = ((char)High_Byte);
    if((robot.joint2_buf[i] - robot.joint2_buf[i-1]) > 0)
        buffer2[3] = 0x01;
    else
        buffer2[3] = 0x00;
    CRC_cheak(buffer2);
    Joint3_speed = (robot.joint3_buf[i] - robot.joint3_buf[i-1])/0.05;
    robot.Joint_3_angle_R = robot.joint3_buf[i];
    uint Pulse3 = 42000000*(2*M_PI)/(abs(Joint3_speed)*6000*robot.Z3);
    Low_Byte = Pulse3&0xff;
    High_Byte = (Pulse3>>8)&0xff;
    buffer3[1] = ((char)Low_Byte);
    buffer3[2] = ((char)High_Byte);
    if((robot.joint3_buf[i] - robot.joint3_buf[i-1]) > 0)
        buffer3[3] = 0x01;
    else
        buffer3[3] = 0x00;
    CRC_cheak(buffer3);
    Joint4_speed = (robot.joint4_buf[i] - robot.joint4_buf[i-1])/0.05;
    robot.Joint_4_angle_R = robot.joint4_buf[i];
    uint Pulse4 = 42000000*(2*M_PI)/(abs(Joint4_speed)*6000*robot.Z4);
    Low_Byte = Pulse4&0xff;
    High_Byte = (Pulse4>>8)&0xff;
    buffer4[1] = ((char)Low_Byte);
    buffer4[2] = ((char)High_Byte);
    if((robot.joint4_buf[i] - robot.joint4_buf[i-1]) > 0)
        buffer4[3] = 0x01;
    else
        buffer4[3] = 0x00;
    CRC_cheak(buffer4);
    Joint5_speed = (robot.joint5_buf[i] - robot.joint5_buf[i-1])/0.05;
    robot.Joint_5_angle_R = robot.joint5_buf[i];
    uint Pulse5 = 42000000*(2*M_PI)/(abs(Joint5_speed)*6000*robot.Z5);
    Low_Byte = Pulse5&0xff;
    High_Byte = (Pulse5>>8)&0xff;
    buffer5[1] = ((char)Low_Byte);
    buffer5[2] = ((char)High_Byte);
    if((robot.joint5_buf[i] - robot.joint5_buf[i-1]) > 0)
        buffer5[3] = 0x01;
    else
        buffer5[3] = 0x00;
    CRC_cheak(buffer5);
    Joint6_speed = (robot.joint6_buf[i] - robot.joint6_buf[i-1])/0.05;
    robot.Joint_6_angle_R = robot.joint6_buf[i];
    uint Pulse6 = 42000000*(2*M_PI)/(abs(Joint6_speed)*6000*robot.Z6);
    Low_Byte = Pulse6&0xff;
    High_Byte = (Pulse6>>8)&0xff;
    buffer6[1] = ((char)Low_Byte);
    buffer6[2] = ((char)High_Byte);
    if((robot.joint6_buf[i] - robot.joint6_buf[i-1]) > 0)
        buffer6[3] = 0x01;
    else
        buffer6[3] = 0x00;
    CRC_cheak(buffer6);
    serialPort->write(buffer1,6);
    serialPort->write(buffer2,6);
    serialPort->write(buffer3,6);
    serialPort->write(buffer4,6);
    serialPort->write(buffer5,6);
    serialPort->write(buffer6,6);
    i++;
    if(i == 100) {
        i = 0;
        state_sign = 2;
    }
     std::cout<<"i = "<<static_cast<int>(i)<<std::endl;
}

