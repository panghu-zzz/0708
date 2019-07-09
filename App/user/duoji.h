#ifndef _DIRECTION_H_
#define _DIRECTION_H_
#include "common.h"
#include "include.h"
#include "my_cfg.h"

extern int jiaodu_cur;	//角度值
extern int last_turn;	//上一次转向值
extern int dj_pid_num;	//舵机角度值

extern void duoji_init();
extern void change_angle(uint32 duty);
extern void duoji_PD(int err);//舵机PD
extern void duoji_control();//偏转角度

#endif
