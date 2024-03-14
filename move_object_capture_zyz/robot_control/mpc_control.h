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

#define LIMIT_DDDAP   50000
#define LIMIT_DDANGLE   5
#define LIMIT_DANGLE    1
#define LIMIT_DDPOINT  5000
#define LIMIT_DPOINT    150//��Ҫ*limit == 10 ����Ҫ����ʮ����LIMIT_DPOINT 1500MM/S

class MPC_Control
{
    friend class Slave_Copmputer;
public:
     MatrixXd E,H;
     VectorXd X_Error;
     VectorXd Y_Error;
     VectorXd Z_Error;
     VectorXd J4_Error;
     VectorXd J5_Error;
     VectorXd J6_Error;
     int N;//MPCԤ��Ĳ���
     uint8_t follow_succeed;
     //�𲽽׶����Ƽ��ٶ�
     float limit;
public:
    MPC_Control();
    float Prediction (const MatrixXd& x_k, const MatrixXd& E, const MatrixXd& H, int N);
    void Calculate_Out_X(float U,Robot &robot,Slave_Copmputer& F407);
    void Calculate_Out_Y(float U,Robot &robot,Slave_Copmputer& F407);
    void Calculate_Out_Z(float U,Robot &robot,Slave_Copmputer& F407);
    void Calculate_Out_J4(float U,Robot &robot);
    void Calculate_Out_J5(float U,Robot &robot);
    void Calculate_Out_J6(float U,Robot &robot);
    void Refresh_Error(Robot &robot,mobile_pose &mobile);
};

#endif // MPC_CONTROL_H