//-------------------------------------------------------------------------*
//�ļ���: includes.h                                                       *
//˵  ��: ��ͷ�ļ�,���ļ�����:                                             *
//        ������(main)�ļ����õ���ͷ�ļ����ⲿ�����������йس�������       *
//-------------------------------------------------------------------------*
#ifndef INCLUDE_H_
#define INCLUDE_H_

//1 ͷ�ļ�
//1.1ͨ��ͷ�ļ�
//#include "MK60DZ10.h"
#include "common.h"            //ͨ�ú���ͷ�ļ�
#include <stdio.h>
#include <string.h>
#include "my_cfg.h"
#include "fun.h"                           //���ܺ���ͷ�ļ�
struct Bline
{
  uint16 line;//��
  int coord;  //������
};

typedef struct
{
  int    SET_FLAG;
  uint8 ZHIDAO_SPEED;
  uint8 CD_SPEED[8];
  uint8 DIS_IMG_FLAG;
  int BILI;
  float DJ_KP[6];
  
  float KP;
  float KD;
  float KI;
}ParValue;
#define byte uint8
#define word uint16
//2 �궨��
#define endlineROW 240     //OV7620��ÿ��240��
//#define  H 60   //�ɼ�����
#define  V 80  //ÿ�вɼ�����
//#define Cmp 160//����ͷ��ֵ����ֵ
#define left_way 0  //���������
#define right_way 2 //���������
#define center_way 1 //���������
#define dj_left_max  dj_center+DJ_DIFF   //����������ֵ
#define dj_right_max dj_center-DJ_DIFF //����������ֵ

#define IMG_DIFF 10  //ͼ�����Ѱ���㷨��ֵ  10
#define BW_DIFF 15  //������ֵ���ɫ�Ҷ�ֵ�� 15
#define JUDGE_DIFF 10	//�жϺ����Ƿ���Чб���жϲ�ֵ

#define KEY1 0x01
#define KEY2 0x02
#define KEY3 0x03
#define KEY4 0x04
#define KEY5 0x05

#define FAR_FIND_MIDDLE 1//ȫ������־
#define FAR_FIND_DIFF   0//����������־
#define SD_MODE 1

#define ROAD_SHIZI 3		//������־ ʮ�ֵ�
////////////////////////////��ȡ�ñ�־///////////////////////////////
#define LEFT_LOST_W 0x01u		//
#define LEFT_LOST_B 0x02u		//
#define RIGHT_LOST_W 0x04u		//
#define RIGHT_LOST_B 0x08u		//
#define ALL_LOST_W 0x10u		//
#define ALL_LOST_B 0x20u		//
#define ALL_LOST 0x40u			//
///////////////////////////�ж��ñ�־//////////////////////////////////
#define INVALID_LINE 0x01u			//
#define NONE_LOST 0 				//
#define DEAL_LEFT_LOST 0x2u		    //��ʼʱ����߶�ʧ��־
#define DEAL_RIGHT_LOST 0x4u		//��ʼʱ�ұ��߶�ʧ��־

//////////////////////����궨��///////////////////////////
#define SAVE_FLASH_DATA 0	//	����flash�����궨��
#define CHANG_TEST 0		//  ���ж���LED���궨��


#define TEST_FLASH_ADDR 202
#define TEST_SECTOR_SIZE 2048
#define TEST_SD_SECTOR_SIZE 512

#define LCD_HEIGHT 64
//3ȫ�ֱ�������

//
#define TEST_IO_H GPIOC_PDOR |= GPIO_PDOR_PDO(GPIO_PIN(15))
#define TEST_IO_L GPIOC_PDOR &=~GPIO_PDOR_PDO(GPIO_PIN(15))

#define LED_H GPIOA_PDOR |= GPIO_PDOR_PDO(GPIO_PIN(15))
#define LED_L GPIOA_PDOR &=~GPIO_PDOR_PDO(GPIO_PIN(15))
#define LED_TOGGLE GPIOA_PDOR ^=GPIO_PDOR_PDO(GPIO_PIN(15))

#define GPIO_PIN_MASK      0x1Fu    //0x1f=31,����λ��Ϊ0--31��Ч
#define GPIO_PIN(x)        (((1)<<(x & GPIO_PIN_MASK)))  //�ѵ�ǰλ��1



#endif
