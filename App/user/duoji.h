#ifndef _DIRECTION_H_
#define _DIRECTION_H_
#include "common.h"
#include "include.h"
#include "my_cfg.h"

extern int jiaodu_cur;	//�Ƕ�ֵ
extern int last_turn;	//��һ��ת��ֵ
extern int dj_pid_num;	//����Ƕ�ֵ

extern void duoji_init();
extern void change_angle(uint32 duty);
extern void duoji_PD(int err);//���PD
extern void duoji_control();//ƫת�Ƕ�

#endif
