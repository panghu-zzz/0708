#include "ctrl.h"
#include "my_cfg.h"


uint8 img_deal_flag=0;//��ʼ�ɼ�����ͼ��
int line_lose_flag=0;

////////////////////////////////ȫ�ֱ�������////////////////////////////////////

uint16 Bline_left[CAMERA_H];	 //����ߴ������
uint16 Bline_right[CAMERA_H];	 //�ұ��ߴ������
uint16 Pick_table[CAMERA_H];	 //�����ߴ������

uint16 left_pos[CAMERA_H];	 //����ߴ������
uint16 right_pos[CAMERA_H];	 //�ұ��ߴ������
uint16 center_pos[CAMERA_H];	 //�����ߴ������

uint8  Pick_flag[CAMERA_H];//�����Ƿ��ҵ����߱�־����
uint8  Deal_flag[CAMERA_H];//���������Ƿ���Ч��־����


//����������λ�õ��㷨1�����м������߲�ѯ�ڵ�,������ֱ�������ߣ�XM
void XM_Get_edge_1(uint8 *img)
{
    uint8 i,j,k; 
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

void control_duoji(void)
{
   chazhi[10]=80-(b[10][0]+b[10][1])/2;//��ƫ�ң������0
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