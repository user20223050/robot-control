#include "mpc_control.h"

MPC_Control::MPC_Control():X_Error(3,1),Y_Error(3,1),Z_Error(3,1),J1_Error(3,1),J2_Error(3,1),J3_Error(3,1),J4_Error(3,1),J5_Error(3,1),J6_Error(3,1)
{
    //MPC控制
    MatrixXd A(3, 3);
    A << 1, 0.05, 0.00125, 0, 1, 0.05, 0, 0, 1;
    MatrixXd B(3, 1);
    B << 2.083e-05, 0.00125, 0.05;
    MatrixXd Q(3, 3);
    Q << 100, 0, 0, 0, 10, 0, 0, 0, 1;
    MatrixXd F(3, 3);
    F << 100, 0, 0, 0, 10, 0, 0, 0, 1;
    double R = 0.1;
    N = 15;
    int n = A.rows();
    int p = B.cols();
    MatrixXd M((N + 1) * n, n);
    M.topRows(n) = MatrixXd::Identity(n, n);
    M.bottomRows(N * n).setZero();
    MatrixXd C((N + 1) * n, N * p);
    C.setZero();
    MatrixXd tmp = MatrixXd::Identity(n, n);
    for (int i = 1; i <= N; ++i) {
        VectorXi rows = VectorXi::LinSpaced(n, i * n, (i + 1) * n - 1);
        // 更新 C 矩阵
        C.block(rows[0], 0, n, C.cols()) << tmp * B, C.block(rows[0] - n, 0, n, C.cols() - p);
        // 更新 tmp
        tmp = A * tmp;
        // 更新 M 矩阵
        M.block(rows[0], 0, n, M.cols()) = tmp;
    }
    // 将Q_bar初始化为具有Q重复N次和F作为最后一个块的块对角矩阵
    Eigen::MatrixXd Q_bar = Eigen::MatrixXd::Zero(N * Q.rows() + F.rows(), N * Q.cols() + F.cols());
    // 使用块对角结构填充Q_bar
    for (int i = 0; i < N; ++i) {
        Q_bar.block(i * Q.rows(), i * Q.cols(), Q.rows(), Q.cols()) = Q;
    }
    // 将最后一个块设置为F
    Q_bar.block(N * Q.rows(), N * Q.cols(), F.rows(), F.cols()) = F;
    // 初始化R_bar为具有R重复N次的Kronecker积
    Eigen::MatrixXd R_bar = Eigen::MatrixXd::Zero(N, N);
    // 计算Kronecker积
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            R_bar(i, i) = R;
        }
    }
    E = C.transpose() * Q_bar * M;
    H = C.transpose() * Q_bar * C + R_bar;
}

void MPC_Control::change_weight(void)
{
    //MPC控制
    MatrixXd A(3, 3);
    A << 1, 0.05, 0.00125, 0, 1, 0.05, 0, 0, 1;
    MatrixXd B(3, 1);
    B << 2.083e-05, 0.00125, 0.05;
    MatrixXd Q(3, 3);
    if(run_state == 1)//位置跟随模式
    Q << 100, 0, 0, 0, 10, 0, 0, 0, 1;
    if(run_state == 2)//速度跟随模式
    Q << 50, 0, 0, 0, 100, 0, 0, 0, 1;
    MatrixXd F(3, 3);
    F << 100, 0, 0, 0, 10, 0, 0, 0, 1;
    double R = 0.1;
    N = 15;
    int n = A.rows();
    int p = B.cols();
    MatrixXd M((N + 1) * n, n);
    M.topRows(n) = MatrixXd::Identity(n, n);
    M.bottomRows(N * n).setZero();
    MatrixXd C((N + 1) * n, N * p);
    C.setZero();
    MatrixXd tmp = MatrixXd::Identity(n, n);
    for (int i = 1; i <= N; ++i) {
        VectorXi rows = VectorXi::LinSpaced(n, i * n, (i + 1) * n - 1);
        // 更新 C 矩阵
        C.block(rows[0], 0, n, C.cols()) << tmp * B, C.block(rows[0] - n, 0, n, C.cols() - p);
        // 更新 tmp
        tmp = A * tmp;
        // 更新 M 矩阵
        M.block(rows[0], 0, n, M.cols()) = tmp;
    }
    // 将Q_bar初始化为具有Q重复N次和F作为最后一个块的块对角矩阵
    Eigen::MatrixXd Q_bar = Eigen::MatrixXd::Zero(N * Q.rows() + F.rows(), N * Q.cols() + F.cols());
    // 使用块对角结构填充Q_bar
    for (int i = 0; i < N; ++i) {
        Q_bar.block(i * Q.rows(), i * Q.cols(), Q.rows(), Q.cols()) = Q;
    }
    // 将最后一个块设置为F
    Q_bar.block(N * Q.rows(), N * Q.cols(), F.rows(), F.cols()) = F;
    // 初始化R_bar为具有R重复N次的Kronecker积
    Eigen::MatrixXd R_bar = Eigen::MatrixXd::Zero(N, N);
    // 计算Kronecker积
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            R_bar(i, i) = R;
        }
    }
    E = C.transpose() * Q_bar * M;
    H = C.transpose() * Q_bar * C + R_bar;

}
float MPC_Control::Prediction(const MatrixXd &x_k, const MatrixXd &E, const MatrixXd &H, int N)
{
//    VectorXd U_k = VectorXd::Zero(NP);
    MatrixXd mid = E*x_k;
    real_t lb[15];
    real_t ub[15];
    real_t Hh[15*15];
    real_t g[15*1];
    QProblem example(15,0);
    Options options;
    example.setOptions( options );
    for(int i = 0;i < 15; i++) {
        g[i] = mid(i,0);
    }
    for(int i = 0; i < 15; i++) {
        for(int j = 0; j < 15; j++) {
            Hh[i*15+j] = H(i,j);
        }
    }
    for(uint8_t i = 0; i < 15; i++) {
        lb[i] = -LIMIT_DDDANGLE;
        ub[i] = LIMIT_DDDANGLE;
    }
    /* Solve first QP. */
    int_t nWSR = 50;
    example.init( Hh,g,NULL,lb,ub,NULL,NULL, nWSR );
    real_t xOpt[15];
    example.getPrimalSolution( xOpt );
    return xOpt[0];
}

void MPC_Control::Refresh_Error(Robot &robot, mobile_pose &mobile)
{
    J1_Error(0,0) = (mobile.Joint_1_angle_T - robot.Joint_1_angle_R);
    J1_Error(1,0) = (mobile.Joint_1_dangle_T - robot.Joint_1_speed_R);
    J1_Error(2,0) = (mobile.Joint_1_ddangle_T - robot.Joint_1_dspeed_R);
    J2_Error(0,0) = (mobile.Joint_2_angle_T - robot.Joint_2_angle_R);
    J2_Error(1,0) = (mobile.Joint_2_dangle_T - robot.Joint_2_speed_R);
    J2_Error(2,0) = (mobile.Joint_2_ddangle_T - robot.Joint_2_dspeed_R);
    J3_Error(0,0) = (mobile.Joint_3_angle_T - robot.Joint_3_angle_R);
    J3_Error(1,0) = (mobile.Joint_3_dangle_T - robot.Joint_3_speed_R);
    J3_Error(2,0) = (mobile.Joint_3_ddangle_T - robot.Joint_3_dspeed_R);
    J4_Error(0,0) = (mobile.Joint_4_angle_T - robot.Joint_4_angle_R);
    J4_Error(1,0) = (mobile.Joint_4_dangle_T - robot.Joint_4_speed_R);
    J4_Error(2,0) = (mobile.Joint_4_ddangle_T - robot.Joint_4_dspeed_R);
    J5_Error(0,0) = (mobile.Joint_5_angle_T - robot.Joint_5_angle_R);
    J5_Error(1,0) = (mobile.Joint_5_dangle_T - robot.Joint_5_speed_R);
    J5_Error(2,0) = (mobile.Joint_5_ddangle_T - robot.Joint_5_dspeed_R);
    J6_Error(0,0) = (mobile.Joint_6_angle_T - robot.Joint_6_angle_R);
    J6_Error(1,0) = (mobile.Joint_6_dangle_T - robot.Joint_6_speed_R);
    J6_Error(2,0) = (mobile.Joint_6_ddangle_T - robot.Joint_6_dspeed_R);
}

void MPC_Control::Calculate_Out_X(float U,Robot &robot,Slave_Copmputer& F407,mobile_pose &mobile)
{
    robot.Joint_1_dspeed_C += (mobile.Joint_1_dddangle_T-U)*0.05;
    if(robot.Joint_1_dspeed_C > LIMIT_DDANGLE)
        robot.Joint_1_dspeed_C = LIMIT_DDANGLE;
    if(robot.Joint_1_dspeed_C < -LIMIT_DDANGLE)
        robot.Joint_1_dspeed_C = -LIMIT_DDANGLE;
    robot.Joint_1_speed_C += robot.Joint_1_dspeed_C*0.05;
    if(robot.Joint_1_speed_C > LIMIT_DANGLE)
        robot.Joint_1_speed_C = LIMIT_DANGLE;
    if(robot.Joint_1_speed_C < -LIMIT_DANGLE)
        robot.Joint_1_speed_C = -LIMIT_DANGLE;
    robot.Joint_1_angle_C = (robot.Joint_1_angle_R + robot.Joint_1_speed_C*0.05);

    std::cout<<"J1U = "<<U<<std::endl;
    std::cout<<"robot.Joint_1_dspeed_C = "<<robot.Joint_1_dspeed_C<<std::endl;
    std::cout<<"robot.Joint_1_speed_C = "<<robot.Joint_1_speed_C<<std::endl;
    std::cout<<"robot.Joint_1_angle_C = "<<robot.Joint_1_angle_C<<std::endl;
//    std::cout<<"\n "<<std::endl;
}

void MPC_Control::Calculate_Out_Y(float U, Robot &robot,Slave_Copmputer& F407,mobile_pose &mobile)
{
    robot.Joint_2_dspeed_C += (mobile.Joint_2_dddangle_T-U)*0.05;
    if(robot.Joint_2_dspeed_C > LIMIT_DDANGLE)
        robot.Joint_2_dspeed_C = LIMIT_DDANGLE;
    if(robot.Joint_2_dspeed_C < -LIMIT_DDANGLE)
        robot.Joint_2_dspeed_C = -LIMIT_DDANGLE;
    robot.Joint_2_speed_C += robot.Joint_2_dspeed_C*0.05;
    if(robot.Joint_2_speed_C > LIMIT_DANGLE)
        robot.Joint_2_speed_C = LIMIT_DANGLE;
    if(robot.Joint_2_speed_C < -LIMIT_DANGLE)
        robot.Joint_2_speed_C = -LIMIT_DANGLE;

    robot.Joint_2_angle_C = (robot.Joint_2_angle_R + robot.Joint_2_speed_C*0.05);

    std::cout<<"J2U = "<<U<<std::endl;
    std::cout<<"robot.Joint_2_dspeed_C = "<<robot.Joint_2_dspeed_C<<std::endl;
    std::cout<<"robot.Joint_2_speed_C = "<<robot.Joint_2_speed_C<<std::endl;
    std::cout<<"robot.Joint_2_angle_C = "<<robot.Joint_2_angle_C<<std::endl;
//    std::cout<<"\n "<<std::endl;
}

void MPC_Control::Calculate_Out_Z(float U, Robot &robot,Slave_Copmputer& F407,mobile_pose &mobile)
{
    robot.Joint_3_dspeed_C += (mobile.Joint_3_dddangle_T-U)*0.05;
    if(robot.Joint_3_dspeed_C > LIMIT_DDANGLE)
        robot.Joint_3_dspeed_C = LIMIT_DDANGLE;
    if(robot.Joint_3_dspeed_C < -LIMIT_DDANGLE)
        robot.Joint_3_dspeed_C = -LIMIT_DDANGLE;
    robot.Joint_3_speed_C += robot.Joint_3_dspeed_C*0.05;
    if(robot.Joint_3_speed_C > LIMIT_DANGLE)
        robot.Joint_3_speed_C = LIMIT_DANGLE;
    if(robot.Joint_3_speed_C < -LIMIT_DANGLE)
        robot.Joint_3_speed_C = -LIMIT_DANGLE;

    robot.Joint_3_angle_C = (robot.Joint_3_angle_R + robot.Joint_3_speed_C*0.05);

    std::cout<<"J3U = "<<U<<std::endl;
    std::cout<<"robot.Joint_3_dspeed_C = "<<robot.Joint_3_dspeed_C<<std::endl;
    std::cout<<"robot.Joint_3_speed_C = "<<robot.Joint_3_speed_C<<std::endl;
    std::cout<<"robot.Joint_3_angle_C = "<<robot.Joint_3_angle_C<<std::endl;
//    std::cout<<"\n "<<std::endl;
}

void MPC_Control::Calculate_Out_J4(float U, Robot &robot,mobile_pose &mobile)
{
    robot.Joint_4_dspeed_C += (mobile.Joint_4_dddangle_T-U)*0.05;
    if(robot.Joint_4_dspeed_C > LIMIT_DDANGLE)
        robot.Joint_4_dspeed_C = LIMIT_DDANGLE;
    if(robot.Joint_4_dspeed_C < -LIMIT_DDANGLE)
        robot.Joint_4_dspeed_C = -LIMIT_DDANGLE;
    robot.Joint_4_speed_C += robot.Joint_4_dspeed_C*0.05;
    if(robot.Joint_4_speed_C > LIMIT_DANGLE)
        robot.Joint_4_speed_C = LIMIT_DANGLE;
    if(robot.Joint_4_speed_C < -LIMIT_DANGLE)
        robot.Joint_4_speed_C = -LIMIT_DANGLE;

    robot.Joint_4_angle_C = (robot.Joint_4_angle_R + robot.Joint_4_speed_C*0.05);
//    robot.Joint_4_angle_C = fmod((robot.FA_X_tool_angle_R + robot.FA_X_tool_dangle_C*0.05),2*M_PI);
    std::cout<<"J4U = "<<U<<std::endl;
    std::cout<<"robot.Joint_4_dspeed_C = "<<robot.Joint_4_dspeed_C<<std::endl;
    std::cout<<"robot.Joint_4_speed_C = "<<robot.Joint_4_speed_C<<std::endl;
    std::cout<<"robot.Joint_4_angle_C = "<<robot.Joint_4_angle_C<<std::endl;
//    std::cout<<"\n "<<std::endl;
}

void MPC_Control::Calculate_Out_J5(float U, Robot &robot,mobile_pose &mobile)
{
    robot.Joint_5_dspeed_C += (mobile.Joint_5_dddangle_T-U)*0.05;
    if(robot.Joint_5_dspeed_C > LIMIT_DDANGLE)
        robot.Joint_5_dspeed_C = LIMIT_DDANGLE;
    if(robot.Joint_5_dspeed_C < -LIMIT_DDANGLE)
        robot.Joint_5_dspeed_C = -LIMIT_DDANGLE;
    robot.Joint_5_speed_C += robot.Joint_5_dspeed_C*0.05;
    if(robot.Joint_5_speed_C > LIMIT_DANGLE)
        robot.Joint_5_speed_C = LIMIT_DANGLE;
    if(robot.Joint_5_speed_C < -LIMIT_DANGLE)
        robot.Joint_5_speed_C = -LIMIT_DANGLE;
    robot.Joint_5_angle_C = (robot.Joint_5_angle_R + robot.Joint_5_speed_C*0.05);

    std::cout<<"J5U = "<<U<<std::endl;
    std::cout<<"robot.Joint_5_dspeed_C = "<<robot.Joint_5_dspeed_C<<std::endl;
    std::cout<<"robot.Joint_5_speed_C = "<<robot.Joint_5_speed_C<<std::endl;
    std::cout<<"robot.Joint_5_angle_C = "<<robot.Joint_5_angle_C<<std::endl;
//    std::cout<<"\n "<<std::endl;
}

void MPC_Control::Calculate_Out_J6(float U, Robot &robot,mobile_pose &mobile)
{
    robot.Joint_6_dspeed_C += (mobile.Joint_6_dddangle_T-U)*0.05;
    if(robot.Joint_6_dspeed_C > LIMIT_DDANGLE)
        robot.Joint_6_dspeed_C = LIMIT_DDANGLE;
    if(robot.Joint_6_dspeed_C < -LIMIT_DDANGLE)
        robot.Joint_6_dspeed_C = -LIMIT_DDANGLE;
    robot.Joint_6_speed_C += robot.Joint_6_dspeed_C*0.05;
    if(robot.Joint_6_speed_C > LIMIT_DANGLE)
        robot.Joint_6_speed_C = LIMIT_DANGLE;
    if(robot.Joint_6_speed_C < -LIMIT_DANGLE)
        robot.Joint_6_speed_C = -LIMIT_DANGLE;
    robot.Joint_6_angle_C = (robot.Joint_6_angle_R + robot.Joint_6_speed_C*0.05);

    std::cout<<"J6U = "<<U<<std::endl;
    std::cout<<"robot.Joint_6_dspeed_C = "<<robot.Joint_6_dspeed_C<<std::endl;
    std::cout<<"robot.Joint_6_speed_C = "<<robot.Joint_6_speed_C<<std::endl;
    std::cout<<"robot.Joint_6_angle_C = "<<robot.Joint_6_angle_C<<std::endl;
//    std::cout<<"\n "<<std::endl;
}


