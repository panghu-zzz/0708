#ifndef _CONTROI_H_
#define _CONTROI_H_
#include "common.h"
#include "include.h"

////////////////////////////////全局变量    定义////////////////////////////////////
extern uint16 Bline_left[CAMERA_H];	 //左边线存放数组
extern uint16 Bline_right[CAMERA_H];	 //右边线存放数组
extern uint16 Pick_table[CAMERA_H];	 //中心线存放数组

extern uint16 left_pos[CAMERA_H];	 //左边线存放数组
extern uint16 right_pos[CAMERA_H];	 //右边线存放数组
extern uint16 center_pos[CAMERA_H];	 //中心线存放数组

extern uint8  Pick_flag[CAMERA_H];//该行是否找到黑线标志数组
extern uint8  Deal_flag[CAMERA_H];//处理数据是否有效标志数组
extern uint16 lost_already;
extern uint8 Pick_line;
extern uint8 Out_flag;		//出界标志，无效图像标志

extern void XM_Get_edge_1(uint8 *img);
extern void control_duoji(void);

#endif