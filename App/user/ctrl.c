#include "ctrl.h"
#include "my_cfg.h"


uint8 img_deal_flag=0;//开始采集处理图像
int line_lose_flag=0;

////////////////////////////////全局变量定义////////////////////////////////////

uint16 Bline_left[CAMERA_H];	 //左边线存放数组
uint16 Bline_right[CAMERA_H];	 //右边线存放数组
uint16 Pick_table[CAMERA_H];	 //中心线存放数组

uint16 left_pos[CAMERA_H];	 //左边线存放数组
uint16 right_pos[CAMERA_H];	 //右边线存放数组
uint16 center_pos[CAMERA_H];	 //中心线存放数组

uint8  Pick_flag[CAMERA_H];//该行是否找到黑线标志数组
uint8  Deal_flag[CAMERA_H];//处理数据是否有效标志数组


//求赛道边沿位置的算法1，从中间向两边查询黑点,适用于直道。作者：XM
void XM_Get_edge_1(uint8 *img)
{
    uint8 i,j,k; 
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

void control_duoji(void)
{
   chazhi[10]=80-(b[10][0]+b[10][1])/2;//车偏右，则大于0
   chazhi[35]=80-(b[35][0]+b[35][1])/2;
   chazhi[60]=80-(b[60][0]+b[60][1])/2;
   chazhi[85]=80-(b[85][0]+b[85][1])/2;
   chazhi[110]=80-(b[110][0]+b[110][1])/2;
   
   int16 angle=chazhi[10]*quanzhi[10]+chazhi[35]*quanzhi[35]+chazhi[60]*quanzhi[60]+
     chazhi[85]*quanzhi[85]+chazhi[110]*quanzhi[110];
   
   uint32 PWM=300-angle/5;
    
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
   
   change_angle(PWM); 
}