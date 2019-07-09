#include "my_cfg.h"

uint8 direct_Kd = 45;  // 45
uint8 direct_Kp = 10;

float speed_P=80;
float speed_I=5.2;
float speed_D=10;

float diff_speed=53;//差速因子53  57
uint8 diff_valid_line=45;//45

uint8 b[120][2]={0};//边缘线数组
int8 chazhi[120];//差值数组
uint8 quanzhi[120];//权值数组







