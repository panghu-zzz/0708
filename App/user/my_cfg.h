#ifndef MY_CFG
#define MY_CFG
#include "common.h"

extern uint8 b[120][2];//边缘线数组
extern int8 chazhi[120];//差值数组
extern uint8 quanzhi[120];//权值数组

/*舵机参数 */
#define dj_center 2900 //舵机中心值   5422  5400  5166
#define dj_frequency 200 //200hz        //舵机中值越大越往右偏
#define DJ_DIFF 600   //最大打角与中心值的差值 
#define dj_left_max  dj_center+DJ_DIFF   //舵机左向最大值
#define dj_right_max dj_center-DJ_DIFF //舵机右向最大值

extern uint8 direct_Kd;//转向D的值
extern uint8 direct_Kp;//转向P的值


/*电机参数*/
#define MOTOR_FTM   FTM0//定时器0控制
#define MOTOR4_PWM  FTM_CH4//通道4

extern uint8 zhidao_speed;

extern float speed_P;
extern float speed_D;
extern float speed_I;

/*编码器对应引脚，用的正交解码，可以在PORT_cfg.h中改*/
//
////      模块通道    端口          可选范围              建议
//#define FTM1_CH0    PTA12       //PTA8、PTA12、PTB0
//#define FTM1_CH1    PTA13       //PTA9、PTA13、PTB1

#endif


