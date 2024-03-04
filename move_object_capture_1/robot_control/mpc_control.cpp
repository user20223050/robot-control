#include "mpc_control.h"

MPC_Control::MPC_Control():X_Error(3,1),Y_Error(3,1),Z_Error(3,1),XA_Error(3,1),YA_Error(3,1),ZA_Error(3,1)
{
    //限制起步阶段加速度
    limit = 0;
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

float MPC_Control::Prediction(const MatrixXd &x_k, const MatrixXd &E, const MatrixXd &H, int N)
{
//    VectorXd U_k = VectorXd::Zero(NP);
    MatrixXd mid = E*x_k;
    QProblem example(15,0);
    Options options;
    example.setOptions( options );
    real_t Hh[15*15] = {  0.266914  , 0.147965 ,  0.129957   ,0.112949 ,  0.097001  ,0.0821637   ,0.068484  ,0.0560003  ,0.0447419 , 0.0347271  ,0.0259614 , 0.0184366  ,0.0121288 ,0.00699669 ,0.00298051
                         ,0.147965 ,  0.233992  ,  0.11825   ,0.103284 , 0.0891549  ,0.0759202  ,0.0636324  ,0.0523378   ,0.042075 , 0.0328732 , 0.0247505 , 0.0177127  ,0.0117514 ,0.00684279 ,0.00294561
                         ,0.129957 ,   0.11825 ,  0.207115  ,0.0940682 , 0.0816554  ,0.0699371  ,0.0589707  ,0.0488087  ,0.0394974 , 0.0310755 , 0.0235721 , 0.0170054  ,0.0113811 ,0.00669097 ,0.00291098
                         ,0.112949 ,  0.103284 , 0.0940682  , 0.185303 , 0.0745025  ,0.0642145  ,0.0544988  ,0.0454128  ,0.0370091  ,0.0293341 , 0.0224262 , 0.0163147  ,0.0110178 ,0.00654123  ,0.0028766
                         ,0.097001 , 0.0891549 , 0.0816554  ,0.0745025  , 0.167696  ,0.0587522  ,0.0502168  ,0.0421503  ,0.0346102  ,0.0276489  , 0.021313 , 0.0156408  ,0.0106616 ,0.00639357 ,0.00284249
                        ,0.0821637 , 0.0759202 , 0.0699371  ,0.0642145 , 0.0587522   , 0.15355  ,0.0461246  ,0.0390212  ,0.0323006   , 0.02602 , 0.0202322 , 0.0149835  ,0.0103124 ,  0.006248 ,0.00280864
                        , 0.068484 , 0.0636324 , 0.0589707  ,0.0544988 , 0.0502168  ,0.0461246  , 0.142222  ,0.0360253  ,0.0300802  ,0.0244474 , 0.0191841 , 0.0143429 ,0.00997018 ,0.00610451 ,0.00277504
                        ,0.0560003 , 0.0523378 , 0.0488087  ,0.0454128 , 0.0421503  ,0.0390212  ,0.0360253  , 0.133163  ,0.0279493  , 0.022931 , 0.0181684 , 0.0137189 ,0.00963502 ,0.00596311 ,0.00274171
                        ,0.0447419 ,  0.042075 , 0.0394974  ,0.0370091 , 0.0346102 , 0.0323006  ,0.0300802  ,0.0279493  , 0.125908  ,0.0214708 , 0.0171854 , 0.0131116  ,0.0093069 ,0.00582378 ,0.00270864
                        ,0.0347271 , 0.0328732 , 0.0310755  ,0.0293341 , 0.0276489  ,  0.02602  ,0.0244474  , 0.022931  ,0.0214708  , 0.120067 , 0.0162349  , 0.012521 ,0.00898581 ,0.00568654 ,0.00267582
                        ,0.0259614 , 0.0247505 , 0.0235721  ,0.0224262  , 0.021313  ,0.0202322  ,0.0191841  ,0.0181684  ,0.0171854  ,0.0162349   ,0.115317   ,0.011947 ,0.00867174 ,0.00555139 ,0.00264327
                        ,0.0184366 , 0.0177127 , 0.0170054  ,0.0163147 , 0.0156408  ,0.0149835  ,0.0143429  ,0.0137189  ,0.0131116  , 0.012521  , 0.011947   , 0.11139 ,0.00836471 ,0.00541832 ,0.00261098
                        ,0.0121288 , 0.0117514 , 0.0113811 , 0.0110178 , 0.0106616  ,0.0103124 ,0.00997018 ,0.00963502 , 0.0093069 ,0.00898581 ,0.00867174 ,0.00836471  , 0.108065 ,0.00528733 ,0.00257895
                       ,0.00699669, 0.00684279 ,0.00669097, 0.00654123, 0.00639357  , 0.006248 ,0.00610451 ,0.00596311 ,0.00582378 ,0.00568654 ,0.00555139 ,0.00541832 ,0.00528733  , 0.105158 ,0.00254718
                       ,0.00298051 ,0.00294561 ,0.00291098 , 0.0028766 ,0.00284249 ,0.00280864 ,0.00277504 ,0.00274171, 0.00270864 ,0.00267582 ,0.00264327 ,0.00261098 ,0.00257895 ,0.00254718,  0.102516};
    real_t g[15*1]; //= { 1059.37,849.588,669.838,517.687,390.72,286.588,202.921,137.416,87.7913,51.7965,27.2133,11.8539,3.56234,0.213432,-0.286513};
    for(int i = 0;i < 15;i++) {
        g[i] = mid(i,0);
    }
    real_t lb[15] = {-5000,-5000,-5000,-5000,-5000,-5000,-5000,-5000,-5000,-5000,-5000,-5000,-5000,-5000,-5000};
    real_t ub[15] = {5000,5000,5000,5000,5000,5000,5000,5000,5000,5000,5000,5000,5000,5000,5000};
    /* Solve first QP. */
    int_t nWSR = 50;
    example.init( Hh,g,NULL,lb,ub,NULL,NULL, nWSR );
    //example.init( Hh,g,NULL,NULL,NULL,NULL,NULL, nWSR );
    real_t xOpt[15];
    example.getPrimalSolution( xOpt );
//    cout<<xOpt[0]<<endl;
    return xOpt[0];
}

void MPC_Control::Refresh_Error(Robot &robot, mobile_pose &mobile)
{

     X_Error(0,0) = round((mobile.X_point_target -  robot.X_robot_point_R)*1)/1;
     X_Error(1,0) = mobile.X_speed_target - robot.X_robot_dpoint_R;
     X_Error(2,0) = mobile.X_dspeed_target - robot.X_robot_ddpoint_R;//加速度是必须的
//     x_error(0,0) = mobile.X_point_OBJ_Real -  robot.X_point_R;
//     x_error(1,0) = round((mobile.X_speed_OBJ_Real - robot.X_Spoint_R)*1)/1;
//     x_error(2,0) = round((mobile.X_dspeed_OBJ_Real - robot.X_dSpoint_R)*1)/1;//加速度是必须的
     Y_Error(0,0) = round((mobile.Y_point_target -   robot.Y_robot_point_R)*1)/1;
     Y_Error(1,0) = mobile.Y_speed_target -  robot.Y_robot_dpoint_R;
     Y_Error(2,0) = mobile.Y_dspeed_target -  robot.Y_robot_ddpoint_R;
//     y_error(0,0) = mobile.Y_point_OBJ_Real -  robot.Y_point_R;
//     y_error(1,0) = round((mobile.Y_speed_OBJ_Real - robot.Y_Spoint_R)*1)/1;
//     y_error(2,0) = round((mobile.Y_dspeed_OBJ_Real - robot.Y_dSpoint_R)*1)/1;
     Z_Error(0,0) = round((mobile.Z_point_target -  robot.Z_robot_point_R)*1)/1;
     Z_Error(1,0) = mobile.Z_speed_target - robot.Z_robot_dpoint_R;
     Z_Error(2,0) = mobile.Z_dspeed_target - robot.Z_robot_ddpoint_R;
//     z_error(0,0) = mobile.Z_point_OBJ_Real -  robot.Z_point_R;
//     z_error(1,0) = round((mobile.Z_speed_OBJ_Real - robot.Z_Spoint_R)*1)/1;
//     z_error(2,0) = round((mobile.Z_dspeed_OBJ_Real - robot.Z_dSpoint_R)*1)/1;
     XA_Error(0,0) = round((mobile.X_angle_target - robot.FA_X_tool_angle_R)*180/M_PI*1)/1;
     XA_Error(1,0) = mobile.X_dangle_target - robot.FA_X_tool_dangle_R;
     XA_Error(2,0) = mobile.X_ddangle_target - robot.FA_X_tool_ddangle_R;
     YA_Error(0,0) = round((mobile.Y_angle_target - robot.FA_Y_tool_angle_R)*180/M_PI*1)/1;
     YA_Error(1,0) = mobile.Y_dangle_target - robot.FA_Y_tool_dangle_R;
     YA_Error(2,0) = mobile.Y_ddangle_target - robot.FA_Y_tool_ddangle_R;
//     ZA_Error(0,0) = round((mobile.Z_angle_target - robot.FA_Z_tool_angle_R)*180/M_PI*1)/1;
//     ZA_Error(1,0) = mobile.Z_dangle_target - robot.FA_Z_tool_dangle_R;
//     ZA_Error(2,0) = mobile.Z_ddangle_target - robot.FA_Z_tool_ddangle_R;
//     std::cout<<"x_error(0,0) = "<<x_error(0,0)<<std::endl;
//     std::cout<<"x_error(1,0) = "<<x_error(1,0)<<std::endl;
//     std::cout<<"x_error(2,0) = "<<x_error(2,0)<<std::endl;
     std::cout<<"YA_Error(0,0) = "<<YA_Error(0,0)<<std::endl;
     std::cout<<"YA_Error(1,0) = "<<YA_Error(1,0)<<std::endl;
     std::cout<<"YA_Error(2,0) = "<<YA_Error(2,0)<<std::endl;
//     std::cout<<"z_error(0,0) = "<<z_error(0,0)<<std::endl;
//     std::cout<<"z_error(1,0) = "<<z_error(1,0)<<std::endl;
//     std::cout<<"z_error(2,0) = "<<z_error(2,0)<<std::endl;
}

void MPC_Control::Calculate_Out_X(float U,Robot &robot,Slave_Copmputer& F407)
{

    robot.X_robot_ddpoint_C -= U*0.05;
    if(robot.X_robot_ddpoint_C > limit*LIMIT_DDPOINT)
        robot.X_robot_ddpoint_C = limit*LIMIT_DDPOINT;
    if(robot.X_robot_ddpoint_C < -(limit*LIMIT_DDPOINT))
        robot.X_robot_ddpoint_C = -(limit*LIMIT_DDPOINT);
    robot.X_robot_dpoint_C += robot.X_robot_ddpoint_C * 0.05;
    if(robot.X_robot_dpoint_C > limit*LIMIT_DPOINT)
        robot.X_robot_dpoint_C = limit*LIMIT_DPOINT;
    if(robot.X_robot_dpoint_C < -(limit*LIMIT_DPOINT))
        robot.X_robot_dpoint_C = -(limit*LIMIT_DPOINT);
    if(F407.stop_sign == 1)
        robot.X_robot_ddpoint_C = 0;
    if(F407.stop_sign == 1)
        robot.X_robot_dpoint_C = 0;
    if(X_Error[0]==0 && X_Error[1]==0 && X_Error[2]==0){
        robot.X_robot_dpoint_C = 0;
        robot.X_robot_ddpoint_C = 0;
        follow_succeed |= 0x04;
    }
    else
        follow_succeed = 0;
    robot.X_robot_point_C = robot.X_robot_point_R + robot.X_robot_dpoint_C*0.05;
    limit = limit + 0.5;
    if(limit > 10)
        limit =10;
//    std::cout<<robot.X_point_R<<std::endl;
}

void MPC_Control::Calculate_Out_Y(float U, Robot &robot,Slave_Copmputer& F407)
{

    robot.Y_robot_ddpoint_C -= U*0.05;
    if(robot.Y_robot_ddpoint_C > limit*LIMIT_DDPOINT)
        robot.Y_robot_ddpoint_C = limit*LIMIT_DDPOINT;
    if(robot.Y_robot_ddpoint_C < -(limit*LIMIT_DDPOINT))
        robot.Y_robot_ddpoint_C = -(limit*LIMIT_DDPOINT);
    robot.Y_robot_dpoint_C += robot.Y_robot_ddpoint_C*0.05;
    if(robot.Y_robot_dpoint_C > limit*LIMIT_DPOINT)
        robot.Y_robot_dpoint_C = limit*LIMIT_DPOINT;
    if(robot.Y_robot_dpoint_C < -(limit*LIMIT_DPOINT))
        robot.Y_robot_dpoint_C = -(limit*LIMIT_DPOINT);
    if(F407.stop_sign == 1)
        robot.Y_robot_ddpoint_C = 0;
    if(F407.stop_sign == 1)
        robot.Y_robot_dpoint_C = 0;
    if(Y_Error[0]==0 && Y_Error[1]==0 && Y_Error[2]==0){
        robot.Y_robot_dpoint_C = 0;
        robot.Y_robot_ddpoint_C = 0;
        follow_succeed |= 0x02;
    }
    else
       follow_succeed = 0;
//    robot.Y_point_C = robot.Y_point_R + v*0.05 + 0.00125*a - (2.083e-05)*U;
    robot.Y_robot_point_C = robot.Y_robot_point_R+ robot.Y_robot_dpoint_C*0.05;
//    robot.Y_point_C = round(robot.Y_point_C*10000)/10000;
//    std::cout<<robot.Y_point_R<<std::endl;
}

void MPC_Control::Calculate_Out_Z(float U, Robot &robot,Slave_Copmputer& F407)
{

    robot.Z_robot_ddpoint_C -= U*0.05;
    if(robot.Z_robot_ddpoint_C > limit*LIMIT_DDPOINT)
        robot.Z_robot_ddpoint_C = limit*LIMIT_DDPOINT;
    if(robot.Z_robot_ddpoint_C < -(limit*LIMIT_DDPOINT))
        robot.Z_robot_ddpoint_C = -(limit*LIMIT_DDPOINT);
    robot.Z_robot_dpoint_C += robot.Z_robot_ddpoint_C*0.05;
    if(robot.Z_robot_dpoint_C > limit*LIMIT_DPOINT)
        robot.Z_robot_dpoint_C = limit*LIMIT_DPOINT;
    if(robot.Z_robot_dpoint_C < -(limit*LIMIT_DPOINT))
        robot.Z_robot_dpoint_C = -(limit*LIMIT_DPOINT);
    if(F407.stop_sign == 1)
        robot.Z_robot_ddpoint_C = 0;
    if(F407.stop_sign == 1)
        robot.Z_robot_dpoint_C = 0;
    if(Z_Error[0]==0 && Z_Error[1]==0 && Z_Error[2]==0){
        robot.Z_robot_dpoint_C = 0;
        robot.Z_robot_ddpoint_C = 0;
        follow_succeed |= 0x01;
    }
    else
        follow_succeed = 0;
//    robot.Z_point_C = robot.Z_point_R + v*0.05 + 0.00125*a - (2.083e-05)*U;
    robot.Z_robot_point_C = robot.Z_robot_point_R + robot.Z_robot_dpoint_C*0.05;
//    robot.Z_point_C = round(robot.Z_point_C*10000)/10000;
//    std::cout<<robot.Z_point_R<<std::endl;
}

void MPC_Control::Calculate_Out_XA(float U, Robot &robot)
{
    robot.FA_X_tool_ddangle_C -= U*0.05;
    if(robot.FA_X_tool_ddangle_C > LIMIT_DDANGLE)
        robot.FA_X_tool_ddangle_C = LIMIT_DDANGLE;
    if(robot.FA_X_tool_ddangle_C < -LIMIT_DDANGLE)
        robot.FA_X_tool_ddangle_C = -LIMIT_DDANGLE;
    robot.FA_X_tool_dangle_C += robot.FA_X_tool_ddangle_C*0.05;
    if(robot.FA_X_tool_dangle_C > LIMIT_DANGLE)
        robot.FA_X_tool_dangle_C = LIMIT_DANGLE;
    if(robot.FA_X_tool_dangle_C < -LIMIT_DANGLE)
        robot.FA_X_tool_dangle_C = -LIMIT_DANGLE;
    robot.FA_X_tool_angle_C = fmod((robot.FA_X_tool_angle_R + robot.FA_X_tool_dangle_C*0.05),2*M_PI);
    std::cout<<"FA_X_tool_angle_C ="<<robot.FA_X_tool_angle_C<<std::endl;
}

void MPC_Control::Calculate_Out_YA(float U, Robot &robot)
{
    robot.FA_Y_tool_ddangle_C -= U*0.05;
    if(robot.FA_Y_tool_ddangle_C > LIMIT_DDANGLE)
        robot.FA_Y_tool_ddangle_C = LIMIT_DDANGLE;
    if(robot.FA_Y_tool_ddangle_C < -LIMIT_DDANGLE)
        robot.FA_Y_tool_ddangle_C = -LIMIT_DDANGLE;
    robot.FA_Y_tool_dangle_C += robot.FA_Y_tool_ddangle_C*0.05;
    if(robot.FA_Y_tool_dangle_C > LIMIT_DANGLE)
        robot.FA_Y_tool_dangle_C = LIMIT_DANGLE;
    if(robot.FA_Y_tool_dangle_C < -LIMIT_DANGLE)
        robot.FA_Y_tool_dangle_C = -LIMIT_DANGLE;
    if(YA_Error[0] ==0)
        robot.FA_Y_tool_dangle_C = 0;
    robot.FA_Y_tool_angle_C = fmod((robot.FA_Y_tool_angle_R + robot.FA_Y_tool_dangle_C*0.05),2*M_PI);
    std::cout<<"FA_Y_tool_angle_C = "<<robot.FA_Y_tool_angle_C<<std::endl;
}

void MPC_Control::Calculate_Out_ZA(float U, Robot &robot)
{
    robot.FA_Z_tool_ddangle_C -= U*0.05;
    if(robot.FA_Z_tool_ddangle_C > LIMIT_DDANGLE)
        robot.FA_Z_tool_ddangle_C = LIMIT_DDANGLE;
    if(robot.FA_Z_tool_ddangle_C < -LIMIT_DDANGLE)
        robot.FA_Z_tool_ddangle_C = -LIMIT_DDANGLE;
    robot.FA_Z_tool_dangle_C += robot.FA_Z_tool_ddangle_C*0.05;
    if(robot.FA_Z_tool_dangle_C > LIMIT_DANGLE)
        robot.FA_Z_tool_dangle_C = LIMIT_DANGLE;
    if(robot.FA_Z_tool_dangle_C < -LIMIT_DANGLE)
        robot.FA_Z_tool_dangle_C = -LIMIT_DANGLE;
    robot.FA_Z_tool_angle_C = fmod((robot.FA_Z_tool_angle_R + robot.FA_Z_tool_dangle_C*0.05),2*M_PI);
    std::cout<<"FA_Z_tool_angle_C = "<<robot.FA_Z_tool_angle_C<<std::endl;
}


