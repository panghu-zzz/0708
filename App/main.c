#include "common.h"
#include "include.h"

void XM_Get_edge_1(uint8 *img);
void XM_SerialPrint(void);


uint8 imgbuff[CAMERA_SIZE];
//uint8 imgbuff[CAMERA_SIZE]={0xff,0xff,0xf8,0x00,0x00,  0x00,0x00,0x00,0x00,0xff, 0xff,0xff,0xff,0xff,0xff, 0xff,0xff,0xff,0xff,0xff};             
//定义存储接收图像的数组,一个字节8个像素点，20列，120行
uint8 img[CAMERA_W*CAMERA_H];

uint8 b[120][2]={0};//边缘线数组

int8 chazhi[120];//差值数组
uint8 quanzhi[120];//权值数组
int16 angle;

void PORTA_IRQHandler();
void DMA0_IRQHandler();

void  main(void)
{  
    quanzhi[10]=2;
    quanzhi[35]=2;
    quanzhi[60]=5;
    
    quanzhi[85]=10;
    quanzhi[110]=15;
     
    Site_t site     = {0, 0};                           //显示图像左上角位置
    Size_t imgsize  = {CAMERA_W, CAMERA_H};             //图像大小W=160,H=120
    Size_t size;                   //显示区域图像大小
    
    LCD_init();
    //LCD_str(site,"Cam init ing",FCOLOUR,BCOLOUR);
    
    size.H = LCD_H;//128
    size.W = LCD_W;//128
    
    camera_init(imgbuff);
    //LCD_str(site,"Cam init OK!",FCOLOUR,BCOLOUR);
    
    uart_init(UART4,115200);//用于观察摄像头的串口 
    
    PWM_dianji_init(5000,870);
    PWM_duoji_init(200,300);
      
//    配置中断复位函数
    set_vector_handler(PORTA_VECTORn , PORTA_IRQHandler);   //设置LPTMR的中断复位函数为 PORTA_IRQHandler
    set_vector_handler(DMA0_VECTORn , DMA0_IRQHandler);     //设置LPTMR的中断复位函数为 PORTA_IRQHandler

    while(1)
    {  
        camera_get_img();         //摄像头获取图像 ，耗时8.4ms ，去掉LCD显示时可以达到150帧率，耗时6.7ms         
        XM_Get_edge_1(imgbuff);
        control_duoji();
        LCD_Img_Binary_Z(site, size, imgbuff, imgsize);//LCD显示图像,耗时11.6m       
        vcan_sendimg(imgbuff,CAMERA_SIZE);//用于观察摄像头的串口
        //LCD_Img_Binary(site,size,imgbuff);//图片大小和显示区域大小需要对应
        
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
//不同的上位机，不同的命令
//如果使用其他上位机，则需要修改代码

void vcan_sendimg(uint8 *imgaddr, uint32 imgsize)
{
    #define CMD_IMG  1

    uint8 cmdf[2] = {CMD_IMG, ~CMD_IMG};    //山外上位机 使用的命令
    uint8 cmdr[2] = {~CMD_IMG, CMD_IMG};    //山外上位机 使用的命令

    uart_putbuff(VCAN_PORT, cmdf, sizeof(cmdf));    //先发送命令

    uart_putbuff(VCAN_PORT, imgaddr, imgsize); //再发送图像

    uart_putbuff(VCAN_PORT, cmdr, sizeof(cmdr));    //先发送命令
}


//压缩二值化图像解压（空间 换 时间 解压）
//srclen 是二值化图像的占用空间大小
void img_extract(uint8 *dst, uint8 *src, uint32 srclen)
{
    uint8 colour[2] = {255, 0}; //0 和 1 分别对应的颜色
    //注：山外的摄像头 0 表示 白色，1表示 黑色
    uint8 tmpsrc;
    while(srclen --)
    {
        tmpsrc = *src++;
        *dst++ = colour[ (tmpsrc >> 7 ) & 0x01 ];
        *dst++ = colour[ (tmpsrc >> 6 ) & 0x01 ];
        *dst++ = colour[ (tmpsrc >> 5 ) & 0x01 ];
        *dst++ = colour[ (tmpsrc >> 4 ) & 0x01 ];
        *dst++ = colour[ (tmpsrc >> 3 ) & 0x01 ];
        *dst++ = colour[ (tmpsrc >> 2 ) & 0x01 ];
        *dst++ = colour[ (tmpsrc >> 1 ) & 0x01 ];
        *dst++ = colour[ (tmpsrc >> 0 ) & 0x01 ];
    }
}

//电机驱动初始化，输出引脚：PTA7,推荐频率:1K--20K，占空比为:(duty/1000)*100%,作者：XM
void PWM_dianji_init(uint32 frequency,uint32 duty)
{
  FTM_PWM_init(FTM0, FTM_CH4,frequency,duty);
}

//舵机驱动初始化，输出引脚：PTA10,推荐频率:100--300，占空比为:(duty/1000)*100%,
//高电平0.5ms--2.5ms之间，所以:0.5<1000*duty/frequency<2.5,作者：XM
void PWM_duoji_init(uint32 frequency,uint32 duty)
{ 
  FTM_PWM_init(FTM2, FTM_CH0,frequency,duty);
}

//改变电机驱动占空比，占空比为:(duty/1000)*100%,输入要求：0<duty<1000,作者：XM
void PWM_dianji_duty(uint32 duty)
{
 FTM_PWM_Duty(FTM0, FTM_CH4,duty);
}

//改变舵机驱动占空比，占空比为:(duty/1000)*100%,输入要求：0.5<1000*duty/frequency<2.5,作者：XM
void PWM_duoji_duty(uint32 duty)
{
 //FTM_PWM_Duty(FTM2,;[++++ FTM_CH0,duty);
}

void control_duoji(void)
{
 uint16 PWM=300;
 
 chazhi[10]=80-(b[10][0]+b[10][1])/2;//车偏右，则大于0
 chazhi[35]=80-(b[35][0]+b[35][1])/2;
 chazhi[60]=80-(b[60][0]+b[60][1])/2;
 chazhi[85]=80-(b[85][0]+b[85][1])/2;
 chazhi[110]=80-(b[110][0]+b[110][1])/2;
 
 
 angle=chazhi[10]*quanzhi[10]+chazhi[35]*quanzhi[35]+chazhi[60]*quanzhi[60]+
   
   chazhi[85]*quanzhi[85]+chazhi[110]*quanzhi[110];
 
 PWM=300-angle/5;
  
 if(281<=PWM&&PWM<=329)
 {PWM_dianji_init(5000,750);
 }
 if(230<=PWM&&PWM<=280)
 {PWM_dianji_init(5000,800);
 }
  
 if(330<=PWM&&PWM<=380)
 {PWM_dianji_init(5000,800);
 }
 
 if(PWM<200)
 {PWM=200; PWM_dianji_init(5000,930);
 }

 if(PWM>400)
 {PWM=400; PWM_dianji_init(5000,930);
 }
 
 PWM_duoji_duty(PWM); 
}

//求赛道边沿位置的算法1，从中间向两边查询黑点,适用于直道。作者：XM
void XM_Get_edge_1(uint8 *img)
{uint8 i,j,k; 
 uint8 const hang=120; 
  //先求左边沿
  for(i=hang;i>0;i--)//共120行，从最下面那行到最上面那行（对于液晶显示的图来说）
  {
    for(j=0;j<10;j++)//j<10,只限于当前行
    {
      if(img[20*i-11-j]>0)//这个字节的数据大于0，说明黑色的在此字节中
      {
       for(k=0;;k++)
       {
        if(img[20*i-11-j]&(1<<k))//求黑色位具体在此字节中的哪一位
        {
         b[i-1][0]=80-8*j-k;//位置赋值给b数组
         break;
        }
       }
       break;
      }
      else
      {
       b[i-1][0]=0x00;//当前行左半边无黑色点
      }
    }
  }
  //后求右边沿
 for(i=hang;i>0;i--)
  {
    for(j=0;j<10;j++)
    {
      if(img[20*i-10+j]>0)
      {
       for(k=0;;k++)
       {
        if(img[20*i-10+j]&(0x80>>k))
        {
         b[i-1][1]=81+8*j+k;
         break;
        }
       }
       break;
      }
      else
      {
       b[i-1][1]=0xA0;//当前行右边无黑色点。
      }
    }
  }
}