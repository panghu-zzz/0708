#include "common.h"
#include "include.h"
#include  "duoji.h"       //�������
#include  "my_cfg.h"       //�����Զ���

uint8 imgbuff[CAMERA_SIZE];
uint8 img[CAMERA_W*CAMERA_H];

void PORTA_IRQHandler();
void DMA0_IRQHandler();
void vcan_sendimg(uint8 *imgaddr, uint32 imgsize);

void show_CamView()
{
    Site_t site     = {0, 0};                           //��ʾͼ�����Ͻ�λ��
    Size_t imgsize  = {CAMERA_W, CAMERA_H};             //ͼ���СW=160,H=120
    Size_t size;                   //��ʾ����ͼ���С
    //LCD_str(site,"Init ing",FCOLOUR,BCOLOUR);
    size.H = 60;//LCD_H;//128
    size.W = 80;//LCD_W;//128
     LCD_Img_Binary_Z(site, size, imgbuff, imgsize);//LCD��ʾͼ��,��ʱ11.6m       
}

void initFun()
{
    quanzhi[10]=2;
    quanzhi[35]=2;
    quanzhi[60]=5;
    quanzhi[85]=10;
    quanzhi[110]=15;
    
    duoji_init();
    LCD_init();
    camera_init(imgbuff); 
    //uart_init(UART4,115200);//���ڹ۲�����ͷ�Ĵ��� 
          
    // �����жϸ�λ����
    set_vector_handler(PORTA_VECTORn , PORTA_IRQHandler);   //����LPTMR���жϸ�λ����Ϊ PORTA_IRQHandler
    set_vector_handler(DMA0_VECTORn , DMA0_IRQHandler);     //����LPTMR���жϸ�λ����Ϊ PORTA_IRQHandler

}

void  main(void)
{  
    initFun();
    while(1)
    {  
        camera_get_img();        
        XM_Get_edge_1(imgbuff);
        control_duoji();
        show_CamView();
        //vcan_sendimg(imgbuff,CAMERA_SIZE);//���ڹ۲�����ͷ�Ĵ���
    }
}


/*!
 *  @brief      PORTA�жϷ�����
 *  @since      v5.0
 */
void PORTA_IRQHandler()
{
    uint8  n;    //���ź�
    uint32 flag;

    while(!PORTA_ISFR);
    flag = PORTA_ISFR;
    PORTA_ISFR  = ~0;                                   //���жϱ�־λ

    n = 29;                                             //���ж�
    if(flag & (1 << n))                                 //PTA29�����ж�
    {
        camera_vsync();
    }
#if ( CAMERA_USE_HREF == 1 )                            //ʹ�����ж�
    n = 28;
    if(flag & (1 << n))                                 //PTA28�����ж�
    {
        camera_href();
    }
#endif

}

/*!
 *  @brief      DMA0�жϷ�����
 *  @since      v5.0
 */
void DMA0_IRQHandler()
{
    camera_dma();
}

//����ͼ����λ����ʾ
void vcan_sendimg(uint8 *imgaddr, uint32 imgsize)
{
    #define CMD_IMG  1

    uint8 cmdf[2] = {CMD_IMG, ~CMD_IMG};    //ɽ����λ�� ʹ�õ�����
    uint8 cmdr[2] = {~CMD_IMG, CMD_IMG};    //ɽ����λ�� ʹ�õ�����

    uart_putbuff(VCAN_PORT, cmdf, sizeof(cmdf));    //�ȷ�������

    uart_putbuff(VCAN_PORT, imgaddr, imgsize); //�ٷ���ͼ��

    uart_putbuff(VCAN_PORT, cmdr, sizeof(cmdr));    //�ȷ�������
}

