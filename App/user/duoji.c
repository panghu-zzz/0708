#include "duoji.h"

#define LOCATION_MAX DJ_DIFF
#define LOCATION_MIN -DJ_DIFF

int jiaodu_cur=dj_center;	//角度值
int last_turn=dj_center;	//上一次转向值
int dj_pid_num=dj_center;	//舵机角度值

//定义PID变量结构体
struct _pid{
    float err;                //定义偏差值
    float err_last;            //定义上一个偏差值
    float Kp,Ki,Kd;            //定义比例、积分、微分系数
}duoji_pid;

//舵机初始化
void duoji_init()
{
    FTM_PWM_init(FTM2, FTM_CH0,dj_frequency,dj_center);//200hz
}
//舵机角度变化
void change_angle(uint32 duty)
{
   FTM_PWM_Duty(FTM2,FTM_CH0,duty);
}

//舵机PD
void duoji_PD(int err)
{
    duoji_pid.Kp=direct_Kp;
    duoji_pid.Kd=direct_Kd;
    duoji_pid.err=err;//误差赋值
    
    jiaodu_cur=(int)(duoji_pid.Kp*duoji_pid.err+direct_Kd*(duoji_pid.err-duoji_pid.err_last));//舵机PD公式
    
    duoji_pid.err_last=duoji_pid.err;//记录误差
    if(jiaodu_cur > LOCATION_MAX){jiaodu_cur=LOCATION_MAX;}
    if(jiaodu_cur < LOCATION_MIN){jiaodu_cur=LOCATION_MIN;}
}
//舵机控制
void duoji_control()//偏转角度
{
    uint16 jiaodu=dj_center-jiaodu_cur;//可以通过改变符号使舵机反向
    change_angle(jiaodu);
}
