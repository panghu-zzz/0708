#ifndef _PATH_H_
#define _PATH_H_
#include "common.h"
#include "include.h"
#include "control.h"
#define IMG_BUF imgbuff   //�궨��ͼ��������
#define IMG_H 58
#define IMG_W 80
#define FILL_LEN 70 //���ڲ�ȫ��ʧ����          //70
#define H_U_AND_D IMG_H //�ϱߵ�y�������±�y����Ĳ�ֵ 
#define X_RENEW(x,y) (x-DOWN_EDGE/2)*DOWN_EDGE*H_U_AND_D/(UP_EDGE*H_U_AND_D+(DOWN_EDGE-UP_EDGE)*y)+DOWN_EDGE/2

#define FIND_X_MAX_COUNT 10		 //�ұ���ʱ����Ѱ�ҵĸ���
#define xiao_wan 10

#define zhi_dao 3

//#define get_img_point(h,w) IMG_BUF[(IMG_W*(h)+(w))>>3]&(1 << (7 - ((w) & 0x07) ))
enum PATH_TYPE
{
    Long_zhidao,//��ֱ��
    Short_zhidao,//��ֱ��
    lean_zhidao,//б��ֱ��
    Shi_zi,//ʮ��   
    Zhang_ai,//�ϰ�    
    Xiao_S,//СS��    
    Zhong_S,//��S��  
    Da_S,//��S��    
    Xiao_wan,//С��    
    Zhong_wan,//����    
    Da_wan,//����   
    T_Da_wan,//�ش���
    Da_turn,//��ת��
    Loop_road,//��·
};

extern uint8 guai_dian_count;
extern uint8 gl_zhidao_count;
extern uint8 donw_guan_dian;
extern uint8 S_offset_flag;
extern uint8 gl_path_type;
extern uint8 gl_shang_po_flag;
extern uint8 imgbuff[CAMERA_SIZE]; 
extern uint8 ring_road_w;
extern int center_num;
extern int ramp_flag;
extern int get_path();
extern int get_path_type(Point * path_array,int num);
uint8 If_straight(uint8 start,uint8 end,uint8 val);
int get_img_point(uint16 h,uint16 w);
void get_even_diff_s();
void with_left_to_center();
void with_right_to_center();
void bDistance(void);//������ȷ��˲� ȥ����Ч��
void xielv_lvbo(void);//�����ֵ���Ʒ��˲� ȥ����Ч��
int test_draw_point(uint16 line,uint16 x,uint16 color);
void If_LStraight();
int beleved_shizi_loop();
extern int shizi_wait;
extern int temp[60];
extern int zhidao_loop_flag;

extern int line_weight[60];

#endif
