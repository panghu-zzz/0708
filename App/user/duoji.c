#include "duoji.h"

#define LOCATION_MAX DJ_DIFF
#define LOCATION_MIN -DJ_DIFF

int jiaodu_cur=dj_center;	//�Ƕ�ֵ
int last_turn=dj_center;	//��һ��ת��ֵ
int dj_pid_num=dj_center;	//����Ƕ�ֵ

//����PID�����ṹ��
struct _pid{
    float err;                //����ƫ��ֵ
    float err_last;            //������һ��ƫ��ֵ
    float Kp,Ki,Kd;            //������������֡�΢��ϵ��
}duoji_pid;

//�����ʼ��
void duoji_init()
{
    FTM_PWM_init(FTM2, FTM_CH0,dj_frequency,dj_center);//200hz
}
//����Ƕȱ仯
void change_angle(uint32 duty)
{
   FTM_PWM_Duty(FTM2,FTM_CH0,duty);
}

//���PD
void duoji_PD(int err)
{
    duoji_pid.Kp=direct_Kp;
    duoji_pid.Kd=direct_Kd;
    duoji_pid.err=err;//��ֵ
    
    jiaodu_cur=(int)(duoji_pid.Kp*duoji_pid.err+direct_Kd*(duoji_pid.err-duoji_pid.err_last));//���PD��ʽ
    
    duoji_pid.err_last=duoji_pid.err;//��¼���
    if(jiaodu_cur > LOCATION_MAX){jiaodu_cur=LOCATION_MAX;}
    if(jiaodu_cur < LOCATION_MIN){jiaodu_cur=LOCATION_MIN;}
}
//�������
void duoji_control()//ƫת�Ƕ�
{
    uint16 jiaodu=dj_center-jiaodu_cur;//����ͨ���ı����ʹ�������
    change_angle(jiaodu);
}
