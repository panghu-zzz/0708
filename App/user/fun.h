#ifndef __FUN_H__
#define __FUN_H__

#include "includes.h"
//#include "core_cm4.h"

#define TEST_KEY(x,y) do{x=(GPIO_PDIR_REG(PORTB)&0x0000FFFF);y=(uint16)x;}while(0)
extern uint8 gl_zhangai_x;
extern uint8 gl_zhangai_y;
extern uint8 gl_zhangai_flag;
extern uint8 stop_car_line;
extern float zhnagai_rate ;
void check_and_stop(int num,int flag);
void duoji_control(uint16 jiaodu);//������ƺ�����ע���������޸ĺ��ֱ��Ӱ��
void ti_jiaozheng(uint8 start,uint8 end);//��������ʧ��
void PickCenter_diff(uint16 line);
int32 PickCenter_near();//Ѱ�ҽ���������
void PickCenter_new();
uint32 abs_sub(uint32 diff1,uint32 diff2);//����������ľ���ֵ
void lvbo(uint8 );//�˲�����
void bu_xian();//���ߺ���
void find_deal_zhangai();//���ϰ���Ͳ���
void center_buxian();//�����߲���
extern int mark_stop();
int check_and_stop1();
extern int stop_car_flag;
int regression(uint16 Pick_table[],int startline,int endline);//����б�ʺ���
int vl_ratio();//��Ч�е�б��

extern void single_line_jiaozheng(int line_ary[],int start ,int end);

#endif 
		 //gpio_reverse(PORTB,10);