#ifndef MY_UI_H_
#define MY_UI_H_
#define STR_BUF_LEN 30
typedef enum
{
    MAIN_UI  =   0,         //��������ʱ��Ӧ��ƽ
    SET_SPEED_UI    =   1,         //��������ʱ��Ӧ��ƽ
    IMAGE_UI,               //��������(���ڶ�ʱ����ɨ��)
    SET_IMG_UI,
    OPEN_IMG_UI,
} UI_STATUS_e;

extern void renew_UI();
extern void show_run_info();
#endif

