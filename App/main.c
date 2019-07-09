#include "common.h"
#include "include.h"
#include  "duoji.h"       //舵机控制
#include  "my_cfg.h"       //变量自定义

uint8 imgbuff[CAMERA_SIZE];
uint8 img[CAMERA_W*CAMERA_H];

void PORTA_IRQHandler();
void DMA0_IRQHandler();
void vcan_sendimg(uint8 *imgaddr, uint32 imgsize);

void show_CamView()
{
    Site_t site     = {0, 0};                           //显示图像左上角位置
    Size_t imgsize  = {CAMERA_W, CAMERA_H};             //图像大小W=160,H=120
    Size_t size;                   //显示区域图像大小
    //LCD_str(site,"Init ing",FCOLOUR,BCOLOUR);
    size.H = 60;//LCD_H;//128
    size.W = 80;//LCD_W;//128
     LCD_Img_Binary_Z(site, size, imgbuff, imgsize);//LCD显示图像,耗时11.6m       
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
    //uart_init(UART4,115200);//用于观察摄像头的串口 
          
    // 配置中断复位函数
    set_vector_handler(PORTA_VECTORn , PORTA_IRQHandler);   //设置LPTMR的中断复位函数为 PORTA_IRQHandler
    set_vector_handler(DMA0_VECTORn , DMA0_IRQHandler);     //设置LPTMR的中断复位函数为 PORTA_IRQHandler

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
        //vcan_sendimg(imgbuff,CAMERA_SIZE);//用于观察摄像头的串口
    }
}


/*!
 *  @brief      PORTA中断服务函数
 *  @since      v5.0
 */
void PORTA_IRQHandler()
{
    uint8  n;    //引脚号
    uint32 flag;

    while(!PORTA_ISFR);
    flag = PORTA_ISFR;
    PORTA_ISFR  = ~0;                                   //清中断标志位

    n = 29;                                             //场中断
    if(flag & (1 << n))                                 //PTA29触发中断
    {
        camera_vsync();
    }
#if ( CAMERA_USE_HREF == 1 )                            //使用行中断
    n = 28;
    if(flag & (1 << n))                                 //PTA28触发中断
    {
        camera_href();
    }
#endif

}

/*!
 *  @brief      DMA0中断服务函数
 *  @since      v5.0
 */
void DMA0_IRQHandler()
{
    camera_dma();
}

//发送图像到上位机显示
void vcan_sendimg(uint8 *imgaddr, uint32 imgsize)
{
    #define CMD_IMG  1

    uint8 cmdf[2] = {CMD_IMG, ~CMD_IMG};    //山外上位机 使用的命令
    uint8 cmdr[2] = {~CMD_IMG, CMD_IMG};    //山外上位机 使用的命令

    uart_putbuff(VCAN_PORT, cmdf, sizeof(cmdf));    //先发送命令

    uart_putbuff(VCAN_PORT, imgaddr, imgsize); //再发送图像

    uart_putbuff(VCAN_PORT, cmdr, sizeof(cmdr));    //先发送命令
}

