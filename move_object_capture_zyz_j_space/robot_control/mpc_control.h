#ifndef MPC_CONTROL_H
#define MPC_CONTROL_H

#include <Dense>
#include <iostream>
#include <qpOASES.hpp>
#include "robot.h"
#include "slave_copmputer.h"
#include "mobile_pose.h"

using namespace std;
using namespace Eigen;
using namespace qpOASES;

class Robot;
class Slave_Copmputer;
class mobile_pose;
/*limit*/
#define LIMIT_DDDANGLE 888
#define LIMIT_DDANGLE  888
#define LIMIT_DANGLE   5

class MPC_Control
{
    friend class Slave_Copmputer;
public:

    MatrixXd E,H;
    VectorXd X_Error;
    VectorXd Y_Error;
    VectorXd Z_Error;
    VectorXd J1_Error;
    VectorXd J2_Error;
    VectorXd J3_Error;
    VectorXd J4_Error;
    VectorXd J5_Error;
    VectorXd J6_Error;
    int N;//MPCԤ��Ĳ���
    uint8_t follow_succeed;
    //�𲽽׶����Ƽ��ٶ�
    float limit;
    /*λ�úͽǶ����Ӽ��ٶȱ�־*/
    unsigned char dasign;
public:
    MPC_Control();
    float Prediction (const MatrixXd& x_k, const MatrixXd& E, const MatrixXd& H, int N);
    void Calculate_Out_X(float U,Robot &robot,Slave_Copmputer& F407,mobile_pose &mobile);
    void Calculate_Out_Y(float U,Robot &robot,Slave_Copmputer& F407,mobile_pose &mobile);
    void Calculate_Out_Z(float U,Robot &robot,Slave_Copmputer& F407,mobile_pose &mobile);
    void Calculate_Out_J4(float U,Robot &robot,mobile_pose &mobile);
    void Calculate_Out_J5(float U,Robot &robot,mobile_pose &mobile);
    void Calculate_Out_J6(float U,Robot &robot,mobile_pose &mobile);
    void Refresh_Error(Robot &robot,mobile_pose &mobile);
};

#endif // MPC_CONTROL_H
