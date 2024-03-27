#include <sys/shm.h>
#include <QApplication>
#include <stdio.h>
#include "all_head.h"

using namespace std;
using namespace Eigen;
using namespace qpOASES;
Robot robot;
MPC_Control mpc;
mobile_pose mobile;
Slave_Copmputer F407;
float *p = NULL;
uint8_t follow_target = 0; //0跟随移动目标 1跟随GET_MOVE中输入的目标

int main(int argc, char *argv[])
{
    int shmid;
    int key;
    key = ftok("/home/ni/share_memory.txt",'a');
    shmid = shmget(key,64,IPC_CREAT | 0777);
    printf("creat share memory sucess shmid=%d\n",shmid);
    p = (float *)shmat(shmid,NULL,0);//映射到用户空间
    robot.Robot_Set_Work_position();
//    mobile.refresh_object();
//    mobile.reflash_target_f();
//    mobile.reflash_target_s(robot);
    F407.joint_rotate_direction(robot);
    QApplication a(argc, argv);
    Widget w;
    w.show();

    return a.exec();
}
