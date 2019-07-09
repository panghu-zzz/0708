/*!*****************************************************************
 *  �ڹ�Ժ ��˼���� ���ڵ��γ�������
 *  ���� ��ͨ�������������ò�����֧���Զ�������ĵ��ι�����ʷ��¼
*          ���Զ��屣����ԺõĲ���������������ṹ�壩����֧��ɾ��
 *  ʹ�÷�ʽ ��ͨ��ָ����Ʋ�����ָ��������е�ָ����������
 *���� ������Э���˼�����Ŷ�
********************************************************************/
#ifndef _INFO_DEAL_SAVE_H_
#define _INFO_DEAL_SAVE_H_
#include "includes.h"
#include "MK60_flash.h"
extern uint8_t cmd_buf[30];//�������洮�ڽ��յ����ַ���
extern uint8_t rec_cmd_flag;//���ڽ��յ��ַ�������1
//**************��Ҫ�ĵĲ���**********************
#define Par_DEBUG_ADDR 150 //���Ե�ʱ���õĴ����ַ
#define MAX_Par_DEBUG_COUNT 10 //���ԵĲ�����ʷ��¼�������
#define Par_SAVE_ADDR  151 //�����ԺõĲ������浽�����ַ
#define MAX_Par_SAVE_COUNT  10 //�������ĵ��ԺõĲ��������
#define SECTOR_SIZE FLASH_SECTOR_SIZE//��ҳ��С FN15��(4*1024)��DZ10��(2*1024)
#define PAR_STRUCT_TYPE_NAME ParValue//�Լ�����Ľṹ�����͵���
#define PAR_STRUCT_NAME myPar_num//�Լ�����Ľṹ�����
#define FLASH_ALIGN FLASH_ALIGN_ADDR //flashһ��Ҫд���ٸ��ֽڣ�FN15��8����DZ10��4��
#define FLASH_WRITE_CMD  0x07     //FN15��0x07��DZ10��0x06  flash�����ָ��һ��д�����ֽ�
//******************�±߲���Ҫ�Ķ�****************************************
#define PAR_STRUCT_SIZE (sizeof(PAR_STRUCT_TYPE_NAME)+(sizeof(PAR_STRUCT_TYPE_NAME)%FLASH_ALIGN))//��֤д������ݴ�С�ǵ�ַ����������
#define Par_Debug_head_addr (FLASH_SECTOR_SIZE*Par_DEBUG_ADDR)//�����õĴ����׵�ַ
#define Par_Save_head_addr  (FLASH_SECTOR_SIZE*Par_SAVE_ADDR) //���ԺõĲ��������ַ

//�궨�� FCCOB Number
#if defined(MK60DZ10)
#define     FCMD        FTFL_FCCOB0         //FTFL ����

#define     FADDR2      FTFL_FCCOB1         //Flash address [23:16]
#define     FADDR1      FTFL_FCCOB2         //Flash address [15:8]
#define     FADDR0      FTFL_FCCOB3         //Flash address [7:0]

#define     FDATA0      FTFL_FCCOB4         //Data Byte 0       //ע��һ�㣬4�ֽ�����FDATA3�����λ��FDATA0�����λ
#define     FDATA1      FTFL_FCCOB5         //Data Byte 1
#define     FDATA2      FTFL_FCCOB6         //Data Byte 2
#define     FDATA3      FTFL_FCCOB7         //Data Byte 3
#define     FDATA4      FTFL_FCCOB8         //Data Byte 4
#define     FDATA5      FTFL_FCCOB9         //Data Byte 5
#define     FDATA6      FTFL_FCCOBA         //Data Byte 6
#define     FDATA7      FTFL_FCCOBB         //Data Byte 7

//FCMD ����
#define     RD1BLK    0x00   // ������Flash
#define     RD1SEC    0x01   // ����������
#define     PGMCHK    0x02   // д����
#define     RDRSRC    0x03   // ��Ŀ������(4�ֽ�)
#define     PGM4      0x06   // д�볤��(4�ֽ�)
#define     ERSBLK    0x08   // ��������Flash
#define     ERSSCR    0x09   // ����Flash����
#define     PGMSEC    0x0B   // д������
#define     RD1ALL    0x40   // �����еĿ�
#define     RDONCE    0x41   // ֻ��һ��
#define     PGMONCE   0x43   // ֻдһ��
#define     ERSALL    0x44   // �������п�
#define     VFYKEY    0x45   // ��֤���ŷ���Կ��
#define     PGMPART   0x80   // д�����
#define     SETRAM    0x81   // �趨FlexRAM����

#elif defined(MK60F15)
#define     FCMD        FTFE_FCCOB0         //FTFL ����

#define     FADDR2      FTFE_FCCOB1         //Flash address [23:16]
#define     FADDR1      FTFE_FCCOB2         //Flash address [15:8]
#define     FADDR0      FTFE_FCCOB3         //Flash address [7:0]
//Must be 64-bit aligned (Flash address [2:0] = 000)  8�ֽڶ���

#define     FDATA0      FTFE_FCCOB4         //Data Byte 0       //ע��һ�㣬4�ֽ�����FDATA3�����λ��FDATA0�����λ
#define     FDATA1      FTFE_FCCOB5         //Data Byte 1
#define     FDATA2      FTFE_FCCOB6         //Data Byte 2
#define     FDATA3      FTFE_FCCOB7         //Data Byte 3
#define     FDATA4      FTFE_FCCOB8         //Data Byte 4
#define     FDATA5      FTFE_FCCOB9         //Data Byte 5
#define     FDATA6      FTFE_FCCOBA         //Data Byte 6
#define     FDATA7      FTFE_FCCOBB         //Data Byte 7

//FCMD ����
#define     RD1BLK    0x00   // ������Flash
#define     RD1SEC    0x01   // ����������
#define     PGMCHK    0x02   // д����
#define     RDRSRC    0x03   // ��Ŀ������(8�ֽ�)
#define     PGM8      0x07   // д�볤��(8�ֽ�)
#define     ERSBLK    0x08   // ��������Flash
#define     ERSSCR    0x09   // ����Flash����
#define     PGMSEC    0x0B   // д������
#define     RD1ALL    0x40   // �����еĿ�
#define     RDONCE    0x41   // ֻ��һ��
#define     PGMONCE   0x43   // ֻдһ��
#define     ERSALL    0x44   // �������п�
#define     VFYKEY    0x45   // ��֤���ŷ���Կ��
#define     PGMPART   0x80   // д�����
#define     SETRAM    0x81   // �趨FlexRAM����

#endif
extern ParValue myPar_num;
void init_Par_from_FLASH();//�����������õĳ�ʼ��
void usart_Par_cmd(uint8 *cmd);//���ڽ������ַ�������ã�������յ��ַ���
extern int max_speed;
extern int min_speed;

//****�ڲ�����������ļ������ò���****
extern void init_myPar_num();
extern void get_debug_Par_from_FLASH(PAR_STRUCT_TYPE_NAME *pv,uint8 num);
extern void save_Save_par();
extern void get_savePar_to_debugPar(uint8 num);
extern float str2float(uint8 *cmd);
extern void get_debug_and_printf_Par(uint8 num);
extern void get_save_and_printf_Par(uint8 num);
extern void save_Bebug_par();
extern void del_save_par(uint8 num);
void cmd_deal();
void evaluate_all(ParValue pv);

#endif
