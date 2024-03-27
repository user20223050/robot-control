#include "mpc_control.h"

MPC_Control::MPC_Control():X_Error(3,1),Y_Error(3,1),Z_Error(3,1),J1_Error(3,1),J2_Error(3,1),J3_Error(3,1),J4_Error(3,1),J5_Error(3,1),J6_Error(3,1)
{
    //�����𲽽׶μ��ٶ�
    limit = 0;
    //MPC����
    MatrixXd A(3, 3);
    A << 1, 0.05, 0.00125, 0, 1, 0.05, 0, 0, 1;
    MatrixXd B(3, 1);
    B << 2.083e-05, 0.00125, 0.05;
    MatrixXd Q(3, 3);
    Q << 100, 0, 0, 0, 50, 0, 0, 0, 1;
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
        // ���� C ����
        C.block(rows[0], 0, n, C.cols()) << tmp * B, C.block(rows[0] - n, 0, n, C.cols() - p);
        // ���� tmp
        tmp = A * tmp;
        // ���� M ����
        M.block(rows[0], 0, n, M.cols()) = tmp;
    }
    // ��Q_bar��ʼ��Ϊ����Q�ظ�N�κ�F��Ϊ���һ����Ŀ�ԽǾ���
    Eigen::MatrixXd Q_bar = Eigen::MatrixXd::Zero(N * Q.rows() + F.rows(), N * Q.cols() + F.cols());
    // ʹ�ÿ�Խǽṹ���Q_bar
    for (int i = 0; i < N; ++i) {
        Q_bar.block(i * Q.rows(), i * Q.cols(), Q.rows(), Q.cols()) = Q;
    }
    // �����һ��������ΪF
    Q_bar.block(N * Q.rows(), N * Q.cols(), F.rows(), F.cols()) = F;
    // ��ʼ��R_barΪ����R�ظ�N�ε�Kronecker��
    Eigen::MatrixXd R_bar = Eigen::MatrixXd::Zero(N, N);
    // ����Kronecker��
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
    QProblem example(15,0);
    Options options;
    example.setOptions( options );
    real_t Hh[15*15];
//    real_t Hh[15*15] = {  0.266914  , 0.147965 ,  0.129957   ,0.112949 ,  0.097001  ,0.0821637   ,0.068484  ,0.0560003  ,0.0447419 , 0.0347271  ,0.0259614 , 0.0184366  ,0.0121288 ,0.00699669 ,0.00298051
//                         ,0.147965 ,  0.233992  ,  0.11825   ,0.103284 , 0.0891549  ,0.0759202  ,0.0636324  ,0.0523378   ,0.042075 , 0.0328732 , 0.0247505 , 0.0177127  ,0.0117514 ,0.00684279 ,0.00294561
//                         ,0.129957 ,   0.11825 ,  0.207115  ,0.0940682 , 0.0816554  ,0.0699371  ,0.0589707  ,0.0488087  ,0.0394974 , 0.0310755 , 0.0235721 , 0.0170054  ,0.0113811 ,0.00669097 ,0.00291098
//                         ,0.112949 ,  0.103284 , 0.0940682  , 0.185303 , 0.0745025  ,0.0642145  ,0.0544988  ,0.0454128  ,0.0370091  ,0.0293341 , 0.0224262 , 0.0163147  ,0.0110178 ,0.00654123  ,0.0028766
//                         ,0.097001 , 0.0891549 , 0.0816554  ,0.0745025  , 0.167696  ,0.0587522  ,0.0502168  ,0.0421503  ,0.0346102  ,0.0276489  , 0.021313 , 0.0156408  ,0.0106616 ,0.00639357 ,0.00284249
//                        ,0.0821637 , 0.0759202 , 0.0699371  ,0.0642145 , 0.0587522   , 0.15355  ,0.0461246  ,0.0390212  ,0.0323006   , 0.02602 , 0.0202322 , 0.0149835  ,0.0103124 ,  0.006248 ,0.00280864
//                        , 0.068484 , 0.0636324 , 0.0589707  ,0.0544988 , 0.0502168  ,0.0461246  , 0.142222  ,0.0360253  ,0.0300802  ,0.0244474 , 0.0191841 , 0.0143429 ,0.00997018 ,0.00610451 ,0.00277504
//                        ,0.0560003 , 0.0523378 , 0.0488087  ,0.0454128 , 0.0421503  ,0.0390212  ,0.0360253  , 0.133163  ,0.0279493  , 0.022931 , 0.0181684 , 0.0137189 ,0.00963502 ,0.00596311 ,0.00274171
//                        ,0.0447419 ,  0.042075 , 0.0394974  ,0.0370091 , 0.0346102 , 0.0323006  ,0.0300802  ,0.0279493  , 0.125908  ,0.0214708 , 0.0171854 , 0.0131116  ,0.0093069 ,0.00582378 ,0.00270864
//                        ,0.0347271 , 0.0328732 , 0.0310755  ,0.0293341 , 0.0276489  ,  0.02602  ,0.0244474  , 0.022931  ,0.0214708  , 0.120067 , 0.0162349  , 0.012521 ,0.00898581 ,0.00568654 ,0.00267582
//                        ,0.0259614 , 0.0247505 , 0.0235721  ,0.0224262  , 0.021313  ,0.0202322  ,0.0191841  ,0.0181684  ,0.0171854  ,0.0162349   ,0.115317   ,0.011947 ,0.00867174 ,0.00555139 ,0.00264327
//                        ,0.0184366 , 0.0177127 , 0.0170054  ,0.0163147 , 0.0156408  ,0.0149835  ,0.0143429  ,0.0137189  ,0.0131116  , 0.012521  , 0.011947   , 0.11139 ,0.00836471 ,0.00541832 ,0.00261098
//                        ,0.0121288 , 0.0117514 , 0.0113811 , 0.0110178 , 0.0106616  ,0.0103124 ,0.00997018 ,0.00963502 , 0.0093069 ,0.00898581 ,0.00867174 ,0.00836471  , 0.108065 ,0.00528733 ,0.00257895
//                       ,0.00699669, 0.00684279 ,0.00669097, 0.00654123, 0.00639357  , 0.006248 ,0.00610451 ,0.00596311 ,0.00582378 ,0.00568654 ,0.00555139 ,0.00541832 ,0.00528733  , 0.105158 ,0.00254718
//                       ,0.00298051 ,0.00294561 ,0.00291098 , 0.0028766 ,0.00284249 ,0.00280864 ,0.00277504 ,0.00274171, 0.00270864 ,0.00267582 ,0.00264327 ,0.00261098 ,0.00257895 ,0.00254718,  0.102516};
    real_t g[15*1]; //= { 1059.37,849.588,669.838,517.687,390.72,286.588,202.921,137.416,87.7913,51.7965,27.2133,11.8539,3.56234,0.213432,-0.286513};
    for(int i = 0;i < 15; i++) {
        g[i] = mid(i,0);
    }
    for(int i = 0; i < 15; i++) {
        for(int j = 0; j < 15; j++) {
            Hh[i*15+j] = H(i,j);
        }
    }
    if(dasign == 2) {
        for(uint8_t i = 0; i < 15; i++) {
            lb[i] = -LIMIT_DDDANGLE;
            ub[i] = LIMIT_DDDANGLE;
        }
    }
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
    robot.Joint_4_dspeed_C += (mobile.Joint_1_dddangle_T-U)*0.05;
    robot.Joint_1_dspeed_C -= U*0.05;
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
//    if(abs(J1_Error[0]) < 2) {
//        robot.Joint_1_speed_C = 0;
//        robot.Joint_1_dspeed_C = 0;
//     }
    std::cout<<"J1U = "<<U<<std::endl;
    std::cout<<"robot.Joint_1_speed_C = "<<robot.Joint_1_speed_C<<std::endl;
//    std::cout<<"\n "<<std::endl;
}

void MPC_Control::Calculate_Out_Y(float U, Robot &robot,Slave_Copmputer& F407,mobile_pose &mobile)
{
    robot.Joint_4_dspeed_C += (mobile.Joint_2_dddangle_T-U)*0.05;
    robot.Joint_2_dspeed_C -= U*0.05;
    if(robot.Joint_2_dspeed_C > LIMIT_DDANGLE)
        robot.Joint_2_dspeed_C = LIMIT_DDANGLE;
    if(robot.Joint_2_dspeed_C < -LIMIT_DDANGLE)
        robot.Joint_2_dspeed_C = -LIMIT_DDANGLE;
    robot.Joint_2_speed_C += robot.Joint_2_dspeed_C*0.05;
    if(robot.Joint_2_speed_C > LIMIT_DANGLE)
        robot.Joint_2_speed_C = LIMIT_DANGLE;
    if(robot.Joint_2_speed_C < -LIMIT_DANGLE)
        robot.Joint_2_speed_C = -LIMIT_DANGLE;
//    if(abs(J2_Error[0]) < 2) {
//        robot.Joint_2_speed_C = 0;
//        robot.Joint_2_dspeed_C = 0;
//     }
    robot.Joint_2_angle_C = (robot.Joint_2_angle_R + robot.Joint_2_speed_C*0.05);

    std::cout<<"J2U = "<<U<<std::endl;
    std::cout<<"robot.Joint_2_speed_C = "<<robot.Joint_2_speed_C<<std::endl;
//    std::cout<<"\n "<<std::endl;
}

void MPC_Control::Calculate_Out_Z(float U, Robot &robot,Slave_Copmputer& F407,mobile_pose &mobile)
{
    robot.Joint_4_dspeed_C += (mobile.Joint_3_dddangle_T-U)*0.05;
    robot.Joint_3_dspeed_C -= U*0.05;
    if(robot.Joint_3_dspeed_C > LIMIT_DDANGLE)
        robot.Joint_3_dspeed_C = LIMIT_DDANGLE;
    if(robot.Joint_3_dspeed_C < -LIMIT_DDANGLE)
        robot.Joint_3_dspeed_C = -LIMIT_DDANGLE;
    robot.Joint_3_speed_C += robot.Joint_3_dspeed_C*0.05;
    if(robot.Joint_3_speed_C > LIMIT_DANGLE)
        robot.Joint_3_speed_C = LIMIT_DANGLE;
    if(robot.Joint_3_speed_C < -LIMIT_DANGLE)
        robot.Joint_3_speed_C = -LIMIT_DANGLE;
//    if(abs(J3_Error[0]) < 2) {
//        robot.Joint_3_speed_C = 0;
//        robot.Joint_3_dspeed_C = 0;
//     }
    robot.Joint_3_angle_C = (robot.Joint_3_angle_R + robot.Joint_3_speed_C*0.05);

    std::cout<<"J3U = "<<U<<std::endl;
    std::cout<<"robot.Joint_3_speed_C = "<<robot.Joint_3_speed_C<<std::endl;
//    std::cout<<"\n "<<std::endl;
}

void MPC_Control::Calculate_Out_J4(float U, Robot &robot,mobile_pose &mobile)
{
    robot.Joint_4_dspeed_C += (mobile.Joint_4_dddangle_T-U)*0.05;
    robot.Joint_4_dspeed_C -= U*0.05;
    if(robot.Joint_4_dspeed_C > LIMIT_DDANGLE)
        robot.Joint_4_dspeed_C = LIMIT_DDANGLE;
    if(robot.Joint_4_dspeed_C < -LIMIT_DDANGLE)
        robot.Joint_4_dspeed_C = -LIMIT_DDANGLE;
    robot.Joint_4_speed_C += robot.Joint_4_dspeed_C*0.05;
    if(robot.Joint_4_speed_C > LIMIT_DANGLE)
        robot.Joint_4_speed_C = LIMIT_DANGLE;
    if(robot.Joint_4_speed_C < -LIMIT_DANGLE)
        robot.Joint_4_speed_C = -LIMIT_DANGLE;
//    if(abs(J4_Error[0]) < 2) {
//        robot.Joint_4_speed_C = 0;
//        robot.Joint_4_dspeed_C = 0;
//    }
    robot.Joint_4_angle_C = (robot.Joint_4_angle_R + robot.Joint_4_speed_C*0.05);
//    robot.Joint_4_angle_C = fmod((robot.FA_X_tool_angle_R + robot.FA_X_tool_dangle_C*0.05),2*M_PI);
    std::cout<<"J4U = "<<U<<std::endl;
    std::cout<<"robot.Joint_4_speed_C = "<<robot.Joint_4_speed_C<<std::endl;
//    std::cout<<"\n "<<std::endl;
}

void MPC_Control::Calculate_Out_J5(float U, Robot &robot,mobile_pose &mobile)
{
    robot.Joint_5_dspeed_C += (mobile.Joint_5_dddangle_T-U)*0.05;
    robot.Joint_5_dspeed_C -= U*0.05;
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
//    if(abs(J5_Error[0]) < 2) {
//        robot.Joint_5_speed_C = 0;
//        robot.Joint_5_dspeed_C = 0;
//    }
    std::cout<<"J5U = "<<U<<std::endl;
    std::cout<<"robot.Joint_5_speed_C = "<<robot.Joint_5_speed_C<<std::endl;
//    std::cout<<"\n "<<std::endl;
}

void MPC_Control::Calculate_Out_J6(float U, Robot &robot,mobile_pose &mobile)
{
    robot.Joint_6_dspeed_C += (mobile.Joint_6_dddangle_T-U)*0.05;
    robot.Joint_6_dspeed_C -= U*0.05;
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
//    if(abs(J6_Error[0]) < 2) {
//        robot.Joint_6_speed_C = 0;
//        robot.Joint_6_dspeed_C = 0;
//    }
    std::cout<<"J6U = "<<U<<std::endl;
    std::cout<<"robot.Joint_6_speed_C = "<<robot.Joint_6_speed_C<<std::endl;
//    std::cout<<"\n "<<std::endl;
}


