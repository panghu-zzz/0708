#include "path.h"
#include "direction.h"
#include "fun.h"
#include <math.h>
#include <string.h>
#include  "MK60_FTM.h"
#include "system_MK60DZ10.h"
#include "control.h"
#include  "info_deal_save.h"
#include "ring.h"
#include "speed_new.h"
#include "roundadout.h"

uint8 guai_dian_count=0;
uint8 donw_guan_dian=0;
uint8 gl_zhidao_count=0;
uint8 gl_path_type;
int ramp_flag;
int line_weight[60]={0};

//���� �� �õ�ͼ����ָ���ĵ�
//���� �� h ��60  w ��80
//����ֵ �� �� 0   �� ��0
int get_img_point(uint16 h,uint16 w)
{
    char * pimg=(char *)IMG_BUF;
//    uint8  ch=pimg[((img_size.W>>3)*h)+w>>3];
    uint8  ch=pimg[(IMG_W*h+w)>>3];
    ch&=(1 << (7 - (w & 0x07) ));
    return ch!=0;
//    return pimg[((img_size.W>>3)*h)+w>>3]& (1 << (7 - (w & 0x07) ));
}               
//���� ����һ���ҵ��ı��ߣ����ڲ���
int test_draw_line(Line_struct line,uint16 color)
{
	int start=line.start_point;
	int end=line.end_point;
	Site_t site;
	for(;start<=end;start++)
	{
		site.x=line.point[start];
		site.y=IMG_H-start-1;
		LCD_point(site,color);
	}
	return 0;
}
int test_draw_point(uint16 line,uint16 x,uint16 color)
{
    Site_t site;   
    site.x=x;
    site.y=line;
    LCD_point(site,color);
    return 0;
}

/*******************************************************************************
�������ƣ�If_straight
��������: �ж�С���Ƿ���ֱ���Ϸ���
������
*******************************************************************************/
uint8 If_straight(uint8 start,uint8 end,uint8 val)
{
	uint8 line;
	uint8 count=0,line_w,guai_dian_num=0;
	uint8 guai_dian_y[10]={0};//
	int16 sum_1,sum_2;
	int last_line_x=Pick_table[start],last_index=0,guai_dian_start=Pick_table[start];
	for(line=start;line<=end;line++)
	{
		line_w=abs_sub(Pick_table[line],196/2);
		if(line_w<=val)
			count++;
		if(line<=end&&((Pick_table[line]-last_line_x)*last_index<0))
		{
		    guai_dian_y[guai_dian_num++]=line;  
		}
		if(Pick_table[line]-last_line_x!=0)
		    last_index=Pick_table[line]-last_line_x;
		last_line_x=Pick_table[line];
	}
	for(line=0;line<guai_dian_num;line++)
	{
		if(line<end-5)
		{
			if(line==guai_dian_num-1)
			{
				sum_1=Pick_table[guai_dian_y[line]]-guai_dian_start;
				sum_2=Pick_table[guai_dian_y[line]]-Pick_table[end];
				if((sum_1)*(sum_2)>0)
				{
					if(abs(sum_1)>3&&abs(sum_2)>3)
					{
						if(donw_guan_dian==0)
						donw_guan_dian=guai_dian_y[line];
						guai_dian_count++;
					}
				}
			}
			else
			{
			  sum_1=Pick_table[guai_dian_y[line]]-guai_dian_start;
			  sum_2=Pick_table[guai_dian_y[line]]-Pick_table[guai_dian_y[line+1]];
			  if((sum_1*sum_2)>0)
			  {
			      if(abs(sum_1)>=3&&abs(sum_2)>=3)
			      {
			          if(donw_guan_dian==0)
			              donw_guan_dian=guai_dian_y[line];
			          guai_dian_count++;
			      }
			  }
			}
		}
	  guai_dian_start=Pick_table[guai_dian_y[line]];
	}
	return count;
}

/*******************************************************************************
�������ƣ�getBlineCenter
��������: ��ȡ����������
��������
*******************************************************************************/
void getBlineCenter()
{
  uint8 line;
  Site_t site;

  uint16 x,fill_line=FILL_LEN/2;
  
  for(line=START_Y;line<=valid_line;line++)
  {
      if(((Deal_flag[line]&DEAL_LEFT_LOST)==0)&&((Deal_flag[line]&DEAL_RIGHT_LOST)!=0))//�ұ��߶�ʧ(��������߲�����)
      {
          if(Bline_left[line]>fill_line)
          {
                Pick_table[line]=Bline_left[line]-fill_line;
          }
          else
          {       
              Pick_table[line]=0;
              Deal_flag[line]|=INVALID_LINE;
          }
      }
      else if(((Deal_flag[line]&DEAL_LEFT_LOST)!=0)&&((Deal_flag[line]&DEAL_RIGHT_LOST)==0))//����߶�ʧ(�����ұ��߲�����)
      {
            if(Bline_right[line]>fill_line)
            {
                    Pick_table[line]=Bline_right[line]+fill_line;
            }
            else
            {
              Pick_table[line]=0;
              Deal_flag[line]|=INVALID_LINE;
            }
      }
      else
      {
        Pick_table[line]=(Bline_left[line]+Bline_right[line])/2;
        fill_line=(Bline_left[line]-Bline_right[line])/2;
      }            
  }   

  if(my_debug_flag==1)
  {
      if(my_debug_flag==1)
      {
          for(line=START_Y;line<=valid_line;line++)
            test_draw_point(LCD_H-line,LCD_W-Pick_table[line]+50,0XF800);
      }
      for(x=40;x<80;x++)
      {
          site.x=x+50;
          site.y=LCD_H-valid_line;
          LCD_point(site,0XFFE0);
      }
  }
}

/*******************************************************************************
���ڼ�⵽�����Ժ���
*/
void with_left_to_center()
{
  uint8 line;
  Site_t site;
  uint16 x,fill_line=FILL_LEN/2;

  for(line=START_Y;line<=valid_line;line++)
  {
    Pick_table[line]=Bline_left[line]-(fill_line)+25;//+20  15
  }

//   valid_line= pick_pos1(left_pos);//���ʵ�ʵ���Ч��
//  if(!valid_line)//�ײ�����
//  {
//    ring_maxjiao_flag=1;
//  }
//  else
//  {
//    ring_maxjiao_flag=0;//û���߾���������
//
//  }
  
  if(my_debug_flag==1)
  {
      if(my_debug_flag==1)
      {
          for(line=START_Y;line<=valid_line;line++)
            test_draw_point(LCD_H-line,LCD_W-Pick_table[line]+50,0XF800);
      }
      for(x=40;x<80;x++)
      {
          site.x=x+50;
          site.y=LCD_H-valid_line;
          LCD_point(site,0XFFE0);
      }
  }


}
/************************************************************************/
void with_right_to_center()
{
  uint8 line;
  Site_t site;
  uint16 x,fill_line=FILL_LEN/2;
  uint16 temp=0; 
  int val_ring=10;//��Բ���õ���Ч��
  if(ring_size>=48)//��Բ��
  {
        val_ring;
  }
  else
  {
        val_ring=valid_line;
  }
  for(line=START_Y;line<=val_ring;line++)
  {
    temp=Bline_right[line]+(fill_line)-25;//-20  -15
    if(temp<5)
    {
        temp=2;
    }
    Pick_table[line]=temp;//20
  }
//   valid_line= pick_pos(right_pos);//���ʵ�ʵ���Ч��
//  if(!valid_line)//�ײ�����
//  {
//    ring_maxjiao_flag=1;
//  }
//  else
//  {
//    ring_maxjiao_flag=0;//û���߾���������
//
//  }
      if(my_debug_flag==1)
      {
          if(my_debug_flag==1)
          {
              for(line=START_Y;line<=valid_line;line++)
                test_draw_point(LCD_H-line,LCD_W-Pick_table[line]+50,0XF800);
          }
          for(x=40;x<80;x++)
          {
              site.x=x+50;
              site.y=LCD_H-valid_line;
              LCD_point(site,0XFFE0);
          }
      }
  

}




/*******************************************************************************
�������ƣ�averageLvBo
��������: ��ֵ�˲�
��������
*******************************************************************************/
void averageLvBo()
{
  uint8 line;
  for(line=START_Y+2;line<=(valid_line-2);line++)
  {
	Bline_left[line]=(Bline_left[line-1]+Bline_left[line-2]+Bline_left[line]
					  +Bline_left[line+1]+Bline_left[line+2])/5;
        Bline_right[line]=(Bline_right[line-1]+Bline_right[line-2]+Bline_right[line]
					  +Bline_right[line+1]+Bline_right[line+2])/5;
  }
 
}
/*******************************************************************************
�������ƣ�averageLvBo
��������: ��ֵ�˲�
��������
*******************************************************************************/
void bianxian_averageLvBo()
{
  uint8 line;
  for(line=START_Y+2;line<=(valid_line-2);line++)
  {
	Pick_table[line]=(Pick_table[line-1]+Pick_table[line-2]+Pick_table[line]
					  +Pick_table[line+1]+Pick_table[line+2])/5;
  }
}
/*********************************************************************************
������������������������ݵĺ�
*/
int add_arrary(int a[],int num)
{ 
  int i=0,s=0;
  for(;i<num;i++)
  {
    s=s+a[i];
  }
  return s;
}
/*********************************************************************************
���� ���ص�ĺ�������
*/
int total=1;
void test()
{
  int x=1,y=1;
  while(total)
  {
    for(x=0;x<79;x++)
    {
        for(y=0;y<59;y++)
        {
              if(get_img_point(y,x))
              {
                printf("1");              
              }
              else
              {
                printf("0");   
              }
        
        }
        printf("\n");
  
      }
    total=0;

  }
}

/*******************************************************************************
�������ƣ�bDistance
��������: ������ȷ��˲� ȥ����Ч��
��������
*******************************************************************************/
void bDistance()
{
  uint8 line,shizi_count=0,shizi_start_line=0;
  uint16 distance=90,zhangai_max_len=46,zhang_ai_num=0;;
  uint16 line_w=0;
  uint16 line_fact_w=0;
//  int b_point_num_jin_loop=0;
  int total_line_w=0;
  int av_fact_w=0;
  
  ramp_flag=0;
  
  for(line=START_Y;line<=valid_line;line++)//START_Y
  {
      line_w=(Bline_left[line]-Bline_right[line]);
      
      line_fact_w=left_pos[line]-right_pos[line];
      
      line_weight[line]=line_w;//�ռ�������ȵ�����
      
      
      total_line_w=total_line_w+line_w;
      if(((line_w>distance)||(Bline_left[line]<Bline_right[line])))
      {
        if(Pick_flag[line]==0)
        {
            Deal_flag[line] |= INVALID_LINE;
        }
        else
        {
          if((Deal_flag[line]&DEAL_LEFT_LOST)&&Bline_right[line]<Bline_right[line-1])
          {
              Deal_flag[line] |= INVALID_LINE;
              shizi_count++;
              if(shizi_start_line==0)
              {
                  shizi_start_line=line;
              }
          }
          else if((Deal_flag[line]&DEAL_RIGHT_LOST)&&Bline_left[line]>Bline_left[line-1])
          {
              Deal_flag[line] |= INVALID_LINE;
              shizi_count++;
              if(shizi_start_line==0)
              {
                  shizi_start_line=line;
              }
          }
        }
      }
      else if((line_w<zhangai_max_len)&&(line>2)&&(Pick_flag[line]==0))
      {
          if(gl_zhangai_start==0)
          {
              gl_zhangai_start=line;
              gl_zhangai_end=line;
          }
          else
          {
              if(line-gl_zhangai_end<=2)
                gl_zhangai_end=line;
          }
          zhang_ai_num++;
      }
  }
  if((valid_line-START_Y>0)&&(valid_line>52))//��ĸ����Ϊ0���п��ܿ�����
  {
      av_fact_w=total_line_w/(valid_line-START_Y);// �µ� ���տ�ȼ��
      if(av_fact_w>74)//�µ���� ��� 76
      {
//           b_point_num_jin_loop=beleved_shizi_loop();
//           if(b_point_num_jin_loop>30)
//           {           
//               valid_line=20; 
//               LoopFlag=1;
//               return ;
//           }        
//           else
//           {
//               ramp_flag=1;
//           }            
      }
  }  
  if(zhang_ai_num>7)
  {
      gl_zhangai_flag=1;//�ϰ� ���
  }
  if(shizi_count>8)
  {
      Shi_zi_flag=shizi_start_line;
      valid_line=Shi_zi_flag-1;
  }
}
/*******************************************************************************
�������ƣ�xielv_lvbo
��������: б�ʷ��˲� ȥ����Ч��
��������
*******************************************************************************/
void xielv_lvbo()
{
  uint8 line;
  for(line=START_Y+1;line<=valid_line;line++)
  {
	if((abs_sub(Bline_left[line],Bline_left[line-1])>JUDGE_DIFF)
	   ||(abs_sub(Bline_right[line],Bline_right[line-1])>JUDGE_DIFF))
	{
	  Deal_flag[line] |= INVALID_LINE;
	}
        if(my_debug_flag==1)
        {
            test_draw_point(line,Bline_left[line],0xFFE0);
            test_draw_point(line,Bline_right[line],0X001F);
        }
  }
}
/*******************************************************************************
�������ƣ�Is_out
��������: �жϸó�ͼ���Ƿ���Ч
������
*******************************************************************************/
void Is_out()
{
  if(valid_line<6)
  {
	Out_flag=1;
  }
  else
  {
	Out_flag=0;
  }
  
  
}
float squre_error(uint16 arrary[],int average,int start,int valid_line)
{
	int line = 0;
	float total_error = 0;
	float error = 0;
	for (line = start; line < valid_line; line++)
	{
		//temp[line] = (float)(arrary[line] - average)*(arrary[line] - average);
		total_error = total_error +(float)(arrary[line] - average)*(arrary[line] - average);
		
	}
	error = total_error / valid_line;
	return error;
}
/*******************************************************************************
�������ƣ�If_LStraight
��������: �ж�С���Ƿ��ڳ�ֱ���Ϸ���
��������
*******************************************************************************/
 int temp[60]={0};
int zhidao_loop_flag=0;
void If_LStraight()
{

    
    gl_zhidao_count=If_straight(START_Y,valid_line,16);//����ƫ����Χ�ڵ�����  15  
    if(gl_zhidao_count<40)//45
    {
        zhidao_count_flag=0;
        zhidao_loop_flag=0;
    }
    else if(gl_zhidao_count<=45)//50
    {
        zhidao_count_flag=1;
    }
    else
    {
        zhidao_count_flag=2;  
    }     
    last_zhidao_flag=zhidao_count_flag;
}
void shizi_find_end_line(uint8 *max_x,uint8 *max_y)
{
    uint8 start_line=Shi_zi_flag;
    uint8 index=10;//10
    *max_y=0;
    if(start_line<START_Y+2)
    {
        start_line=START_Y+2;
    }
    for(;index<IMG_W-10;index+=1)//3 1
    {
        for(start_line=Shi_zi_flag;start_line<IMG_H-1;start_line++)
        {
            if(get_img_point(start_line,index)||get_img_point(start_line-2,index))
            {
                break;
            }
        }
        if(start_line-1>(*max_y))
        {
            *max_y=start_line-1;
            *max_x=index;
        }
        
    }
}

int shizi_wait=0;
int shizi_num_right=0;
void shizi_find_line()
{

    uint8 sz_end_line,sz_end_x;
    uint8 line,lift_x,right_x,i;
    uint8 lost_line_count=0;
    shizi_find_end_line(&sz_end_x,&sz_end_line);
    lift_x=right_x=sz_end_x;
   
    for(line=sz_end_line;line>valid_line;line--)
    {
        Deal_flag[line]=0;
        //�������
        if(get_img_point(line,lift_x))
        {
            for(i=1;i<IMG_DIFF;i++)
            {
                if(lift_x==0||lift_x-i==0)
                {
                    lift_x=0;                    
                    Deal_flag[line]|=DEAL_LEFT_LOST;
                    break;
                }
                if(!get_img_point(line,lift_x-i))
                {
                    lift_x=lift_x-i+1;
                    break;
                }
            }
            if(i==IMG_DIFF)
            {
                lift_x=lift_x-i+1;
                Deal_flag[line]|=DEAL_LEFT_LOST;
                lost_line_count++;
            }
        }
        else
        {
            for(i=1;i<IMG_DIFF;i++)
            {
                if(lift_x+i>=IMG_W-1)
                {
                    lift_x=IMG_W-1;
                    Deal_flag[line]|=DEAL_LEFT_LOST;
                    break;
                }
                if(get_img_point(line,lift_x+i))
                {
                    lift_x=lift_x+i-1;
                    break;
                }
            }
            if(i==IMG_DIFF)
            {
                Deal_flag[line]|=DEAL_LEFT_LOST;
                lift_x=lift_x+i-1;
                lost_line_count++;
            }
        }
        if(lift_x==0||lift_x==IMG_H-1)
          lost_line_count++;
        Bline_left[line]=lift_x;        
        left_pos[line]=Bline_left[line];//������ߵ�����
        //���ұ���
        if(get_img_point(line,right_x))
        {
            for(i=1;i<IMG_DIFF;i++)
            {
                if(right_x+i>=IMG_W-1)
                {
                    right_x=IMG_W-1;
                    Deal_flag[line]|=DEAL_RIGHT_LOST;
                    break;
                }
                if(!get_img_point(line,right_x+i))
                {
                    right_x=right_x+i-1;
                    break;
                }
            }
            if(i==IMG_DIFF)
            {
                right_x=right_x+i-1;
                Deal_flag[line]|=DEAL_RIGHT_LOST;
                lost_line_count++;
            }
        }
        else
        {
            for(i=1;i<IMG_DIFF;i++)
            {
                if(right_x==0||right_x-i==0)
                {
                    right_x=0;
                    Deal_flag[line]|=DEAL_RIGHT_LOST;
                    break;
                }
                if(get_img_point(line,right_x-i))
                {
                    right_x=right_x-i+1;
                    break;
                }
            }
            if(i==IMG_DIFF)
            {
                right_x=right_x-i+1;
                Deal_flag[line]|=DEAL_RIGHT_LOST;
                lost_line_count++;
            }
        }
        if(right_x==0||right_x==IMG_H-1)
          lost_line_count++;
        Bline_right[line]=right_x;
        //right_pos[line]=right_x;
        right_pos[line]=Bline_right[line];//�����ұߵ�����
        center_pos[line]=(left_pos[line]+right_pos[line])/2;//�ϳ���������
        
        if((Deal_flag[line]&DEAL_RIGHT_LOST)&&(Deal_flag[line]&DEAL_LEFT_LOST))
        {
            Deal_flag[line]|=INVALID_LINE;
        }
        else if(!(Deal_flag[line]&DEAL_RIGHT_LOST)&&!(Deal_flag[line]&DEAL_LEFT_LOST))
        {
            lost_line_count=0;
        }
        if(lost_line_count>=10)
        {
            line+=10;
            break;
        }
    }
    ti_jiaozheng(line,sz_end_line);//����ͼ�������ʧ��
    for(i=valid_line;i<=line;i++)
      Deal_flag[i]|=INVALID_LINE;
    valid_line=sz_end_line;
}

//void ring_find_line()//�ԳƲ���
//{
//    int i=0,j=0;
//    int left_total_num=0;
//    int bian_lie=0;
//    int average_left=0;
//    int av_flag=0;
//    int differ=0;
//    int valin_lie=0;
//    for(i=3;i<40;i++)
//    {
//      if(abs(Bline_left[i]-Bline_left[i+1])>4)
//      {
//          bian_lie=i;//��¼
//          average_left=left_total_num/(bian_lie-3);  
//          av_flag=1;
//          break;
//      }
//      else
//      {
//          left_total_num=left_total_num+Bline_left[i];//�ۼ�
//      }    
//    }
//    
//    if(av_flag==1)
//    {
//        for(j=bian_lie;;j++)
//        {
//            if(Bline_left[j]>Bline_left[j+1])
//            {
//                valin_lie=j;
//                break;
//            }      
//        }
//        for(j=3;j<valin_lie;j++)
//        {
//            if(Bline_left[j]>average_left)
//            {
//                differ=Bline_left[j]-average_left;
//                Bline_left[j]=average_left-differ-25;
//            }        
//        }
//        if(j<35)
//        {
//             for(;j<35;j++)
//            {
//                Bline_left[j]=Bline_left[j-1]-5;                    
//            }
//        }    
//    }
//}

/*******************************************************************************
�������ƣ�get_even_diff
��������: ��ȡ��Ч����������ƫ��ƽ��ֵ
������
*******************************************************************************/
/********************************************************************************************************/
uint8 get_max_distance()
{
  //�ٶ�100����ʱ��̬ǰհ ����ȣ���������ٶȳ���100ʱ������ģʽ����
  //��Ҫ�̶�ǰհ ����ΪƵ�� �л�ǰհ  ʹС������
    int speed_index=0;
    speed_index=Vspeed/10;
    if(speed_index>11)
    {
        speed_index=11;
    }
    else if(speed_index<6)
    {
        speed_index=6;    
    }
//    if(CD_speed>=100||zhidao_speed>110)//����ģʽ
//    {
//        return max_speed_distance_array[speed_index-6];
//    }
//    else//�е���
//    {
        return max_distance_array[speed_index-6];
//    }
    
}
/*******************************************************************************
�������ƣ�get_even_diff
��������: ��ȡ��Ч����������ƫ��ƽ��ֵ
������
*******************************************************************************/
void get_even_diff_s()//
{
  uint8 line;
  uint8 v_line1=0;//����
  float diff1=0;
  float valid_line2=diff_valid_line;//��¼�ö����м�������С��Ҫ����
  //diff_valid_line=45;//�궨��������������
  uint8 center_val=98;//����ֵ��98����Ϊ֮ǰӲ������ƫ�����-5������������ʵ�������(�Ҽ����)
  int16  w1,w2;//                99
  int valid_line_diff=30;//��Զ���ǰհ��ֵ
  int mix_tance=MIX_distance;  //���ǰհ
  if(gl_zhangai_flag==1&&Shi_zi_flag==0&&gl_zhangai_start>3)//�����ϰ�ʱ������жϣ��ı�����ֵ������ת��
  {
      w1=abs(Bline_left[gl_zhangai_start+4]-Bline_left[gl_zhangai_start-1]);
      w2=abs(Bline_right[gl_zhangai_start+4]-Bline_right[gl_zhangai_start-1]);
      if(w1>w2)//�ϰ������
      {
//          center_val+=15;
         center_val+=20;
          
      }
      else
      {  
//          center_val-=15;
        center_val-=20;
          
      }
    }
    else if(stop_car_line&&valid_line<15)//������ֹͣ�ߣ����Ҳ�������
    {
        even_diff=0;
        return ;
    }

      if(valid_line<valid_line2)//���ܳ�����Ч������
      {
          valid_line2=valid_line;
      }
      if(valid_line2<START_Y+valid_line_diff)
      {
          mix_tance=START_Y;
      }
      even_diff=0;//������

      if(valid_line2<=mix_tance)//��Ч����С�����������
      {
          mix_tance=START_Y;
      }
      
        //��ͨ������������
        for(line=mix_tance;line<valid_line2;line++)//�ӵ�5�е�����valid_line2�в�����㣬ʵ�ʶ�Ӧ���Ǽ��㳵ǰ������һ�ε�·��Ҫ�Լ�����
        {
          if((Deal_flag[line]&INVALID_LINE)==0)//û�ж��߾ͼӺ�
          {               
              v_line1++;//��¼���˶�����
              diff1=diff1+( (int)(Pick_table[line]-center_val));//���
          }
        }
   //   }

      even_diff=(int)((float)(diff1)/(float)(v_line1));//ȡƽ��ֵ
    
   if(even_diff>=33)//����
    {
    even_diff=33;
    }
    else if(even_diff<=-33)//����
    {
    even_diff=-33;
    }

}
/********************************************************************************************************/





