
#ifndef MY_CFG
#define MY_CFG
#include "common.h"
/**************************************************/
/*
    ����˵�����ѽ������õĲ����������ŵ�my_cfg.c��my_cfg.h�У�
              �Ȱ���������ļ���ע�Ϳ�һ��
    ɽ��С����ʹ��˵������������������ѡ��ͼӼ������������Ƿ��أ�
                        ������ȷ�����κ�״̬�°��м䶼�ǿ�ʼ��
    ��һ���ȵ�ͼ������ν������ı��±ߵ�DOWN_EDGE��UP_EDGE
    �ڶ��������ڶ����ֵ���Լ���ֵ����ֵ�Ĳ�ֵ����ֵ������350��500֮�䣬
            ����ͨ������ FTM3_PRECISON ���ı��ֵ����MK60_FTM.h��
    �����������Զ�����Ѷ���̶�����ֵ�ϣ������Ƴ�����һ�γ�����ֱ������
            ���Զ�����ҵĲ�ֵ�Ƿ����������ô�����
    ���Ĳ����򿪺� OPEN_CIRCLE_CONTROL ���п������ԣ�������OPEN_CIRCLE_SPEED��ֵ
            �����ٶ��ܵ����٣������ȿ������ԣ��ı�ת��P���ı�·��
    ���岽���ص��� OPEN_CIRCLE_CONTROL ���бջ����ԣ�����PID���Լ�����ٶȺ���С�ٶ�
*/

//#define OPEN_CIRCLE_CONTROL //��������
#define OPEN_CIRCLE_SPEED 4300

#define WAY_P 0  //ѡ�� �ֶ�p���ǹ̶�p(���)  
                 //  1���ֶ�P   0:�̶�P
#define DUOJI_P 15  //ȫ�̵�P�Ĵ�С��ֱ������,ֱ��ԽСԽ�ã� 15

//****�±����������ν����Ĺ�ʽ�������ĺ�
//***�ѳ�����ֱ���ϣ��������±ߵİ�ɫ������ DOWN_EDGE ��ֵ
//**�ı� UP_EDGE ��ֵ���ǲ�������ʾ�Ļ�ԭ���߽ӽ�ƽ�У�˵�������ɹ�
#define DOWN_EDGE 60 //63  //ͼ���±��ص����ص���� 64 70 63
#define UP_EDGE   12//9   //����ͼ���±�������ʾ�ĵȳ��ľ�����ͼ���ϱ�����ռ�����ص����  16
//63  12
/*ͼ���ʹ�þ��룬��������õ�*/
extern uint8 MIX_distance;//����ľ���  15 10 5  10 12
extern uint8 diff_valid_line;//ǰհ������ͨ��С��������

extern int gl_var_1;//���õı����������ڲ���set img�е���
extern int gl_var_2;//���õı����������ڲ���set img�е���
extern int gl_var_3;//���õı����������ڲ���set img�е���
extern int gl_var_4;//���õı����������ڲ���set img�е���
/*�������  ����--PTE5*/
#define DUO_JI FTM2 //����õĶ�ʱ��
#define DUO_JI_CH FTM_CH0//�����ʱ����ͨ��
#define dj_center 5188 //�������ֵ   5422  5400  5166

//�����ֵԽ��Խ����ƫ
#define DJ_DIFF 1250   //�����������ֵ�Ĳ�ֵ  600 350  1200  
extern int direct_kp_array[];//���ת���Pֵ����my_cfg.c�ļ��и�
extern uint8 direct_Kd;//ת��D��ֵ����my_cfg.c�ļ��и�
extern float kp_val;//���е�p������������
extern uint8 max_distance_array[];
extern uint8 max_speed_distance_array[];
extern uint8 Specia_type_speed[];
/*������� ���ſ���PORT_CFG.h�и���*/
#define MOTOR_FTM   FTM0//��ʱ��0����
#define MOTOR1_PWM  FTM_CH5//ͨ��5 PTD5
#define MOTOR2_PWM  FTM_CH6//ͨ��6 PTD6
#define MOTOR3_PWM  FTM_CH2//ͨ��4 PTA7
#define MOTOR4_PWM  FTM_CH4//ͨ��3 PTA5
#define MOTOR7_PWM  FTM_CH7//ͨ��7 PTD7
extern uint8 zhidao_speed;//ֱ���ٶȣ�����С��������
extern uint8 L_zhidao_speed;
extern uint8 CD_speed;	  //ȫ���ٶȣ�����С��������
extern float speed_P,speed_D;//�����PID��������my_cfg.c�ļ��и�
extern float speed_I;

extern uint8 speed_table[][2];//С��������ʱ���Ĭ�������ٶȣ���my_cfg.c�ļ��и�
extern float diff_speed;//��������



/*��������Ӧ���ţ��õ��������룬������PORT_cfg.h�и�*/

//      ģ��ͨ��    �˿�          ��ѡ��Χ              ����
//#define FTM1_CH0_PIN    PTA8       //PTA8��PTA12��PTB0
//#define FTM1_CH1_PIN    PTA9       //PTA9��PTA13��PTB1
//#define FTM2_CH0_PIN    PTA10       //PTA10��PTB18
//#define FTM2_CH1_PIN    PTA11       //PTA11��PTB19

#endif


