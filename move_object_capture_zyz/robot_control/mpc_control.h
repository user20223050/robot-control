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

#define LIMIT_DDDAP    50000
#define LIMIT_DDPOINT  5000
#define LIMIT_DPOINT   150//需要*limit == 10 即需要扩大十倍即LIMIT_DPOINT 1500MM/S
#define LIMIT_DDDANGLE 5000
#define LIMIT_DDANGLE  3
#define LIMIT_DANGLE   0.3


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
    int N;//MPC预测的步数
    uint8_t follow_succeed;
    //起步阶段限制加速度
    float limit;
    /*位置和角度最大加加速度标志*/
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
