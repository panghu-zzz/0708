#ifndef _CONTROI_H_
#define _CONTROI_H_
#include "common.h"
#include "include.h"

////////////////////////////////ȫ�ֱ���    ����////////////////////////////////////
extern uint16 Bline_left[CAMERA_H];	 //����ߴ������
extern uint16 Bline_right[CAMERA_H];	 //�ұ��ߴ������
extern uint16 Pick_table[CAMERA_H];	 //�����ߴ������

extern uint16 left_pos[CAMERA_H];	 //����ߴ������
extern uint16 right_pos[CAMERA_H];	 //�ұ��ߴ������
extern uint16 center_pos[CAMERA_H];	 //�����ߴ������

extern uint8  Pick_flag[CAMERA_H];//�����Ƿ��ҵ����߱�־����
extern uint8  Deal_flag[CAMERA_H];//���������Ƿ���Ч��־����
extern uint16 lost_already;
extern uint8 Pick_line;
extern uint8 Out_flag;		//�����־����Чͼ���־

extern void XM_Get_edge_1(uint8 *img);
extern void control_duoji(void);

#endif