#include "common.h"
#include "include.h"

void XM_Get_edge_1(uint8 *img);
void XM_SerialPrint(void);


uint8 imgbuff[CAMERA_SIZE];
//uint8 imgbuff[CAMERA_SIZE]={0xff,0xff,0xf8,0x00,0x00,  0x00,0x00,0x00,0x00,0xff, 0xff,0xff,0xff,0xff,0xff, 0xff,0xff,0xff,0xff,0xff};             
//����洢����ͼ�������,һ���ֽ�8�����ص㣬20�У�120��
uint8 img[CAMERA_W*CAMERA_H];

uint8 b[120][2]={0};//��Ե������

int8 chazhi[120];//��ֵ����
uint8 quanzhi[120];//Ȩֵ����
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
     
    Site_t site     = {0, 0};                           //��ʾͼ�����Ͻ�λ��
    Size_t imgsize  = {CAMERA_W, CAMERA_H};             //ͼ���СW=160,H=120
    Size_t size;                   //��ʾ����ͼ���С
    
    LCD_init();
    //LCD_str(site,"Cam init ing",FCOLOUR,BCOLOUR);
    
    size.H = LCD_H;//128
    size.W = LCD_W;//128
    
    camera_init(imgbuff);
    //LCD_str(site,"Cam init OK!",FCOLOUR,BCOLOUR);
    
    uart_init(UART4,115200);//���ڹ۲�����ͷ�Ĵ��� 
    
    PWM_dianji_init(5000,870);
    PWM_duoji_init(200,300);
      
//    �����жϸ�λ����
    set_vector_handler(PORTA_VECTORn , PORTA_IRQHandler);   //����LPTMR���жϸ�λ����Ϊ PORTA_IRQHandler
    set_vector_handler(DMA0_VECTORn , DMA0_IRQHandler);     //����LPTMR���жϸ�λ����Ϊ PORTA_IRQHandler

    while(1)
    {  
        camera_get_img();         //����ͷ��ȡͼ�� ����ʱ8.4ms ��ȥ��LCD��ʾʱ���Դﵽ150֡�ʣ���ʱ6.7ms         
        XM_Get_edge_1(imgbuff);
        control_duoji();
        LCD_Img_Binary_Z(site, size, imgbuff, imgsize);//LCD��ʾͼ��,��ʱ11.6m       
        vcan_sendimg(imgbuff,CAMERA_SIZE);//���ڹ۲�����ͷ�Ĵ���
        //LCD_Img_Binary(site,size,imgbuff);//ͼƬ��С����ʾ�����С��Ҫ��Ӧ
        
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
//��ͬ����λ������ͬ������
//���ʹ��������λ��������Ҫ�޸Ĵ���

void vcan_sendimg(uint8 *imgaddr, uint32 imgsize)
{
    #define CMD_IMG  1

    uint8 cmdf[2] = {CMD_IMG, ~CMD_IMG};    //ɽ����λ�� ʹ�õ�����
    uint8 cmdr[2] = {~CMD_IMG, CMD_IMG};    //ɽ����λ�� ʹ�õ�����

    uart_putbuff(VCAN_PORT, cmdf, sizeof(cmdf));    //�ȷ�������

    uart_putbuff(VCAN_PORT, imgaddr, imgsize); //�ٷ���ͼ��

    uart_putbuff(VCAN_PORT, cmdr, sizeof(cmdr));    //�ȷ�������
}


//ѹ����ֵ��ͼ���ѹ���ռ� �� ʱ�� ��ѹ��
//srclen �Ƕ�ֵ��ͼ���ռ�ÿռ��С
void img_extract(uint8 *dst, uint8 *src, uint32 srclen)
{
    uint8 colour[2] = {255, 0}; //0 �� 1 �ֱ��Ӧ����ɫ
    //ע��ɽ�������ͷ 0 ��ʾ ��ɫ��1��ʾ ��ɫ
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

//���������ʼ����������ţ�PTA7,�Ƽ�Ƶ��:1K--20K��ռ�ձ�Ϊ:(duty/1000)*100%,���ߣ�XM
void PWM_dianji_init(uint32 frequency,uint32 duty)
{
  FTM_PWM_init(FTM0, FTM_CH4,frequency,duty);
}

//���������ʼ����������ţ�PTA10,�Ƽ�Ƶ��:100--300��ռ�ձ�Ϊ:(duty/1000)*100%,
//�ߵ�ƽ0.5ms--2.5ms֮�䣬����:0.5<1000*duty/frequency<2.5,���ߣ�XM
void PWM_duoji_init(uint32 frequency,uint32 duty)
{ 
  FTM_PWM_init(FTM2, FTM_CH0,frequency,duty);
}

//�ı�������ռ�ձȣ�ռ�ձ�Ϊ:(duty/1000)*100%,����Ҫ��0<duty<1000,���ߣ�XM
void PWM_dianji_duty(uint32 duty)
{
 FTM_PWM_Duty(FTM0, FTM_CH4,duty);
}

//�ı�������ռ�ձȣ�ռ�ձ�Ϊ:(duty/1000)*100%,����Ҫ��0.5<1000*duty/frequency<2.5,���ߣ�XM
void PWM_duoji_duty(uint32 duty)
{
 //FTM_PWM_Duty(FTM2,;[++++ FTM_CH0,duty);
}

void control_duoji(void)
{
 uint16 PWM=300;
 
 chazhi[10]=80-(b[10][0]+b[10][1])/2;//��ƫ�ң������0
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

//����������λ�õ��㷨1�����м������߲�ѯ�ڵ�,������ֱ�������ߣ�XM
void XM_Get_edge_1(uint8 *img)
{uint8 i,j,k; 
 uint8 const hang=120; 
  //���������
  for(i=hang;i>0;i--)//��120�У������������е����������У�����Һ����ʾ��ͼ��˵��
  {
    for(j=0;j<10;j++)//j<10,ֻ���ڵ�ǰ��
    {
      if(img[20*i-11-j]>0)//����ֽڵ����ݴ���0��˵����ɫ���ڴ��ֽ���
      {
       for(k=0;;k++)
       {
        if(img[20*i-11-j]&(1<<k))//���ɫλ�����ڴ��ֽ��е���һλ
        {
         b[i-1][0]=80-8*j-k;//λ�ø�ֵ��b����
         break;
        }
       }
       break;
      }
      else
      {
       b[i-1][0]=0x00;//��ǰ�������޺�ɫ��
      }
    }
  }
  //�����ұ���
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
       b[i-1][1]=0xA0;//��ǰ���ұ��޺�ɫ�㡣
      }
    }
  }
}