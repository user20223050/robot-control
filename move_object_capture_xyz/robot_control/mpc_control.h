#ifndef MPC_CONTROL_H
#define MPC_CONTROL_H

#include <Dense>
#include <iostream>
#include <qpOASES.hpp>
#include "robot.h"

using namespace std;
using namespace Eigen;
using namespace qpOASES;

class Robot;
class Slave_Copmputer;
class mobile_pose;

#define LIMIT_DDDAP   50000
#define LIMIT_DDANGLE   0.5
#define LIMIT_DANGLE    0.1
#define LIMIT_DDPOINT  5000
#define LIMIT_DPOINT    150//需要*limit == 10 即需要扩大十倍即LIMIT_DPOINT 1500MM/S

class MPC_Control
{
    friend class Slave_Copmputer;
public:
     MatrixXd E,H;
     VectorXd X_Error;
     VectorXd Y_Error;
     VectorXd Z_Error;
     VectorXd XA_Error;
     VectorXd YA_Error;
     VectorXd ZA_Error;
     int N;//MPC预测的步数
     uint8_t follow_succeed;
     //起步阶段限制加速度
     float limit;
public:
    MPC_Control();
    float Prediction (const MatrixXd& x_k, const MatrixXd& E, const MatrixXd& H, int N);
    void Calculate_Out_X(float U,Robot &robot,Slave_Copmputer& F407);
    void Calculate_Out_Y(float U,Robot &robot,Slave_Copmputer& F407);
    void Calculate_Out_Z(float U,Robot &robot,Slave_Copmputer& F407);
    void Calculate_Out_XA(float U,Robot &robot);
    void Calculate_Out_YA(float U,Robot &robot);
    void Calculate_Out_ZA(float U,Robot &robot);
    void Refresh_Error(Robot &robot,mobile_pose &mobile);
};

#endif // MPC_CONTROL_H
