#ifndef MY_CFG
#define MY_CFG
#include "common.h"

extern uint8 b[120][2];//��Ե������
extern int8 chazhi[120];//��ֵ����
extern uint8 quanzhi[120];//Ȩֵ����

/*������� */
#define dj_center 2900 //�������ֵ   5422  5400  5166
#define dj_frequency 200 //200hz        //�����ֵԽ��Խ����ƫ
#define DJ_DIFF 600   //�����������ֵ�Ĳ�ֵ 
#define dj_left_max  dj_center+DJ_DIFF   //����������ֵ
#define dj_right_max dj_center-DJ_DIFF //����������ֵ

extern uint8 direct_Kd;//ת��D��ֵ
extern uint8 direct_Kp;//ת��P��ֵ


/*�������*/
#define MOTOR_FTM   FTM0//��ʱ��0����
#define MOTOR4_PWM  FTM_CH4//ͨ��4

extern uint8 zhidao_speed;

extern float speed_P;
extern float speed_D;
extern float speed_I;

/*��������Ӧ���ţ��õ��������룬������PORT_cfg.h�и�*/
//
////      ģ��ͨ��    �˿�          ��ѡ��Χ              ����
//#define FTM1_CH0    PTA12       //PTA8��PTA12��PTB0
//#define FTM1_CH1    PTA13       //PTA9��PTA13��PTB1

#endif


