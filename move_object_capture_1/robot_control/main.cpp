#include <sys/shm.h>
#include <QApplication>
#include <stdio.h>
#include "robot.h"

using namespace std;
using namespace Eigen;
using namespace qpOASES;

MPC_Control mpc;
Robot robot;
mobile_pose mobile;
Slave_Copmputer F407;
float *p = NULL;
uint8_t follow_target = 0; //0�����ƶ�Ŀ�� 1����GET_MOVE�������Ŀ��

int main(int argc, char *argv[])
{
    int shmid;
    int key;
    key = ftok("/home/ni/share_memory.txt",'a');
    shmid = shmget(key,64,IPC_CREAT | 0777);
    printf("creat share memory sucess shmid=%d\n",shmid);
    p = (float *)shmat(shmid,NULL,0);//ӳ�䵽�û��ռ�
//    printf("=%p\n",p);
    robot.Robot_Set_Work_position();
    F407.joint_rotate_direction(robot);
    mpc.Refresh_Error(robot,mobile);
    mobile.reflash_target();
    mpc.Calculate_Out_X(0,robot,F407);
    mpc.Calculate_Out_Y(0,robot,F407);
    mpc.Calculate_Out_Z(0,robot,F407);
    mpc.Calculate_Out_XA(0,robot);
    mpc.Calculate_Out_YA(0,robot);
    mpc.Calculate_Out_ZA(0,robot);
    QApplication a(argc, argv);
    Widget w;
    w.show();

    return a.exec();
}
