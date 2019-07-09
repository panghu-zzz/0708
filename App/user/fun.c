#include "includes.h"
#include "MK60_FTM.h"
#include "fun.h"
#include "control.h"
#include "path.h"
#include "speed_new.h"
#include "direction.h"
#include "roundadout.h"
//extern ParValue myPar_num;
uint16 last_left_line=(V/2);
uint16 last_right_line=(V/2);
uint8 stop_car_line=0;
/*******************************************************************************
�������ƣ�regression���ع飩
��������: ��С���˷���б��
������
*******************************************************************************/
int regression(uint16 Pick_table[],int startline,int endline)//����б�ʺ���
{
    int num=0,i;
    int sumX=0,sumY=0,avrX=0,avrY=0;
    int B_up1=0,B_up2=0;
	int B_up=0,B_down=0;
	int slope=0;
    for(i=startline;i<=endline;i++)
    {
         if(Pick_table[i]) 
         {
            num++;
            sumX+=i;
            sumY+=Pick_table[i];
         }
    }
    avrX=sumX/num;
    avrY=sumY/num;
    for(i=startline;i<=endline;i++)
    {
	         if(Pick_table[i]) 
	         { 
	            B_up1=(int)Pick_table[i]-(int)avrY;
	            B_up2=i-avrX;
	            B_up+=(int)B_up1*(int)B_up2;
	            //B_up=B_up/100*100;
	            B_down+=(int)(i-avrX)*(int)(i-avrX);
	         }
   }
   if(B_down==0) slope=0;
   else slope=B_up*10/B_down;
   return slope;
}
int vl_ratio()//��Ч�е�б��
{
    int k=0;
     k=regression(Pick_table,START_Y,32);
     return k;
}
/*******************************************************************************
�������ƣ�PickCenter_near
��������: Ѱ�ҽ��������ߺ���
������
*******************************************************************************/
int32 PickCenter_near()
{
   int32 pixel,flag=0;//,temp1,temp2;
  uint32 Black_count_left=0,Black_count_right=0;
  
  uint16 pick_line=0,temp_line;
  uint16 temp_lost_line=0;
  int32 start_pixel;
  //uint16 left_lost_flag=0,right_lost_flag=0;
  //const int Cmp_D=25;
  //const int  MAX=240;
  uint32 i;
  for(temp_line=START_Y;temp_line<=2+START_Y;temp_line++)//ͳ��ǰ5���м�������50�кڵ���Ŀ 
  {																			  //���ڿ��Գ��Ա��ط�
      for(pixel=V/2-25;pixel<(V/2);pixel++)
      {
          if(get_img_point(temp_line,pixel))
          {
              Black_count_right++;
          }
      }
      for(pixel=V/2;pixel<(V/2+25);pixel++)
      {
          if(get_img_point(temp_line,pixel))
          {
              Black_count_left++;
          }
      }
  }
  if(Black_count_right>Black_count_left)//�м��ұߺڵ���Ŀ������� ����ƫ��
  {
		start_pixel=V/2+15;
  }
  else if(Black_count_right<Black_count_left)//�м���ߺڵ���Ŀ�����ұ� ����ƫ��
  {
		start_pixel=V/2-15;
  }
  else
  {
		start_pixel=V/2;
  }
  for(pick_line=START_Y;pick_line<=START_Y+2;pick_line++)
  {
      for(pixel=start_pixel;pixel<V;pixel+=2)
      {
          if(get_img_point(pick_line,pixel))
          {
              flag=0;
              if(pixel<(V-4))
              {
                  for(i=1;i<5;i++)
                  {
                      if(get_img_point(pick_line,pixel+i))
                         flag++;
                  }
                  if(flag>0)
                  {
                      Bline_left[pick_line]=pixel;
                      break;
                  }
              }
              else
              {
                    i=pixel;
                    while(i<(V-1))
                    {
                        if(get_img_point(pick_line,i+1))
                                flag++;
                        i++;
                     }
                    if(flag>0)
                    {
                        Bline_left[pick_line]=pixel;
                        break;
                    }
              }
           }
      }
      if(pixel>=V)
      {
        Bline_left[pick_line]=V-1;
        Pick_flag[pick_line]|=LEFT_LOST_W;
      }
      for(pixel=start_pixel;pixel>0;pixel-=2)
      {
          if(get_img_point(pick_line,pixel))
          {
              flag=0;
              if(pixel>3)
              {
                  for(i=1;i<5;i++)
                  {
                      if(get_img_point(pick_line,pixel))
                          flag++;
                  }
                  if(flag>0)
                  {
                       Bline_right[pick_line]=pixel;
                       break;
                  }
              }
              else
              {
                  i=pixel;
                  while(i>0)
                  {
                          if(get_img_point(pick_line,pixel))
                                  flag++;
                          i--;
                   }
                  if(flag>0)
                  {
                           Bline_right[pick_line]=pixel;
                           break;
                  }
              }
          }
      }
      if(pixel<=0)
      {
        Bline_right[pick_line]=0;
        Pick_flag[pick_line]|=RIGHT_LOST_W;
      }
      if(Bline_left[pick_line]==(V-1)&&Bline_right[pick_line]==0)//���ұ߽綼δ�ҵ�
      {
          temp_lost_line++;
          Pick_flag[pick_line]|=ALL_LOST_W;
          lost_w_count++;
      }
      else if(Bline_left[pick_line]<(V-100)&&Bline_right[pick_line]>100)
      {
          if(last_zhidao_flag==1)
            if((Bline_left[pick_line]-Bline_right[pick_line])<100)
            {
              //start_end_flag=1;
            }
      }
      //else
      {
        Bline_diff=abs_sub(Bline_left[pick_line],Bline_right[pick_line]);
      }
  }
 
  //Ϊ���������㷨��������ֵ
  last_left_line=Bline_left[START_Y+2];
  last_right_line=Bline_right[START_Y+2];
  if(temp_lost_line>0)
	return temp_lost_line;
  else
  	return 0;
}
/*******************************************************************************
�������ƣ�PickCenter_diff
��������: ����Ѱ��
������line Ѱ����
*******************************************************************************/
void PickCenter_diff(uint16 line)
{
  uint16 pixel;
  uint16 left_start;//
  uint16 right_start;//
  uint16 left_end;//�����Ѱ�߽����߽�
  uint16 right_end;//�ұ���Ѱ�߽����߽�
  uint16 i;
  uint8 flag=0;//�����жϱ�־
  volatile uint16 left_lost_flag=0,right_lost_flag=0;
  //const int Cmp_D=25;
  //const int  MAX=240;
  //////////////////////////////Ѱ���㷨����/////////////////////////////////////
{
      if(last_left_line>IMG_DIFF)
          left_start=last_left_line-IMG_DIFF;
      else
          left_start=1;
      if(Bline_left[line-1]<(V-IMG_DIFF))
          left_end=last_left_line+IMG_DIFF;//ȡ��һ�к���ƫ��IMG_DIFF������
      else
          left_end=V-1;
      
      if((get_img_point(line,left_start))&&(get_img_point(line,left_start+1)))
      {
              pixel=left_start;
      }
      else
      {
          for(pixel=left_start;pixel<left_end;pixel++)
          {
              if(!get_img_point(line,pixel-1)&&get_img_point(line,pixel))
              {
                 flag=0;
                 for(i=0;i<3;i++)
                 {
                    if(get_img_point(line,pixel+i))
                      flag++;
                 }
                 if(flag>1)
                 {
                    Bline_left[line]=pixel;//��line�еĵ�pixel������
                    last_left_line=pixel;
                    break;
                 }
              }
          }
      }
      left_pos[line]=Bline_left[line];//������ߵ�����
      if(pixel==left_start)
      {
          Pick_flag[line] |= LEFT_LOST_B;
          //Bline_left[line]=last_left_line;
          //if(abs_sub(Bline_left[line-1],Bline_left[line-2])>5)
                  Bline_left[line]=left_start;
          //else
              //Bline_left[line]=Bline_left[line-1];
          last_left_line=Bline_left[line];
          left_lost_flag=1;
          Lost_left_count++;
      }  
      else if(pixel==left_end)//δ�ҵ�����
      {
          Pick_flag[line] |= LEFT_LOST_W;
          Bline_left[line]=left_end;
          last_left_line=Bline_left[line];
          left_lost_flag=1;
          Lost_left_count++;
      }
     // left_pos[line]=Bline_left[line];//������ߵ�����
  ///////////////////////////////////���ң�///////////////////     ///////////////////////////
      if(last_right_line>IMG_DIFF)
              right_end=last_right_line-IMG_DIFF;
      else
          right_end=1;
      right_start=last_right_line+IMG_DIFF;
      if(right_start>(V-1))
      {
               right_start=V-1;
      }//ע���Ƿ��ȥ��
      if((get_img_point(line,right_start))&&(get_img_point(line,right_start-1)))
      {
              pixel=right_start;
      } 
      else
      {
          for(pixel=right_start;pixel>right_end;pixel--)
          {
              if(get_img_point(line,pixel))
              {
                 flag=0;
                 for(i=0;i<3;i++)
                 {
                     if(get_img_point(line,pixel-i))
                       flag++;
                 }
                if(flag>1)
                {
                    Bline_right[line]=pixel;
                    last_right_line=pixel;
                    break;
                }
              }
          }
      }
      right_pos[line]=Bline_right[line];//�����ұߵ�����
      if(pixel==right_start)
      {
          Pick_flag[line] |= RIGHT_LOST_B;
          //Bline_right[line]=last_right_line;
          //if(abs_sub(Bline_right[line-1],Bline_right[line-2])>5)
          Bline_right[line]=right_start;
          //else
             // Bline_right[line]=Bline_right[line-1];
          last_right_line=Bline_right[line];
          right_lost_flag=1;
          Lost_right_count++;
      }
      else if(pixel==right_end)//δ�ҵ��ұߺ���
      {
          Pick_flag[line] |= RIGHT_LOST_W;
          Bline_right[line]=right_end;
          last_right_line=Bline_right[line];
          right_lost_flag=1;
          Lost_right_count++;
      }
  }

      
  if((right_lost_flag==1)&&(left_lost_flag==1))
  {
	Lost_Line_count++;
	if((Pick_flag[line]&LEFT_LOST_W)&&(Pick_flag[line]&RIGHT_LOST_W))
        {
            Pick_flag[line] |= ALL_LOST_W;
            lost_w_count++;
        }
  }
  //else
  {
	Bline_diff=abs_sub(Bline_left[line],Bline_right[line]);
  }
}


void ti_jiaozheng(uint8 start,uint8 end)
{
  uint16 line;
  int temp_left,temp_right;
  int up_x=UP_EDGE/2;
  int down_x=DOWN_EDGE/2;
  float k=(float)H_U_AND_D/(up_x-down_x);
  float b=-(k*down_x);
  
  for(line=start;line<=end;line++)
  {
	   //////////////////////////////////����߽���////////////////////////////////
        if(Bline_left[line]>=(V-2))
        {
              Deal_flag[line] |= DEAL_LEFT_LOST;
        }
		temp_left=(int)((Bline_left[line]-40)*(float)down_x/((line-b)/k)+98);  
		if(temp_left<0){
			Bline_left[line]=0;
			Deal_flag[line] |= DEAL_LEFT_LOST;
		}
		else if(temp_left>(196-1))
		{
			Bline_left[line]=(196-1);
			Deal_flag[line] |= DEAL_LEFT_LOST;
		}
		else
		{
			Bline_left[line]=temp_left;
		}
        //////////////////////////////////�ұ��߽���////////////////////////////////
        if(Bline_right[line]<=1)
            Deal_flag[line] |= DEAL_RIGHT_LOST;
		
		temp_right=(int)((Bline_right[line]-40)*(float)down_x/((line-b)/k)+98);
		if(temp_right<0)
		{
			Bline_right[line]=0;
			Deal_flag[line] |= DEAL_RIGHT_LOST;
		}
		else if(temp_right>(196-1))
		{
			Bline_right[line]=(196-1);
			Deal_flag[line] |= DEAL_RIGHT_LOST;
		}
		else
		{
		    Bline_right[line]=temp_right;
		}
        if(my_debug_flag==1)
        {
            test_draw_point(LCD_H-line,LCD_W-Bline_left[line]+50,0X001F);
            test_draw_point(LCD_H-line,LCD_W-Bline_right[line]+50,0X001F);
        }
  }
  
}

//�������߽���
//���ڻ��������߲�������
void single_line_jiaozheng(int line_ary[],int start ,int end)
{
  uint16 line;
  int temp_left,temp_right;
  int up_x=UP_EDGE/2;
  int down_x=DOWN_EDGE/2;
  float k=(float)H_U_AND_D/(up_x-down_x);
  float b=-(k*down_x);
  
  for(line=start;line<=end;line++)
  {
     //////////////////////////////////�����߽���////////////////////////////////
     //////////////////////////////////�ұ��߽���////////////////////////////////
//        if(Bline_right[line]<=1)
//            Deal_flag[line] |= DEAL_RIGHT_LOST;
		
          temp_right=(int)((line_ary[line]-40)*(float)down_x/((line-b)/k)+98);
          if(temp_right<0)
          {
                  //Bline_right[line]=0;
                  line_ary[line]=0;
                  Deal_flag[line] |= DEAL_RIGHT_LOST;
          }
          else if(temp_right>(196-1))
          {
                  //Bline_right[line]=(196-1);
                  line_ary[line]=(196-1);
                  Deal_flag[line] |= DEAL_RIGHT_LOST;
          }
          else
          {
                //Bline_right[line]=temp_right;
                  line_ary[line]=temp_right;
          }
          
          Pick_table[line]=line_ary[line]+FILL_LEN/2-10;
          
       
        if(my_debug_flag==1)
        {
//            test_draw_point(LCD_H-line,LCD_W-Bline_left[line]+50,0X001F);
//            test_draw_point(LCD_H-line,LCD_W-Bline_right[line]+50,0X001F);
            //test_draw_point(LCD_H-line,LCD_W-Bline_left[line]+50,0X001F);
            test_draw_point(LCD_H-line,LCD_W-line_ary[line]+50,BLUE);//BLUE  0X001F
            test_draw_point(LCD_H-line,LCD_W-Pick_table[line]+50,RED);//BLUE  0X001F
        }
  }


}


/*******************************************************************************
�������ƣ�duoji_control
��������: �������
������
*******************************************************************************/
void duoji_control(uint16 jiaodu) 
{
	jiaodu=dj_center-jiaodu;//����ͨ���ı����ʹ�������
	change_angle(jiaodu);
}

/*******************************************************************************
�������ƣ�abs_sub
��������: �����޷���������ľ���ֵ
������
*******************************************************************************/
uint32 abs_sub(uint32 diff1,uint32 diff2)
{
	uint16 temp;
	if(diff1>diff2)
	{
		temp=diff1-diff2;
	}
	else
	{
		temp=diff2-diff1;
	}
	return temp;
}
/*******************************************************************************
�������ƣ�lvbo
��������: �˲�
������
*******************************************************************************/
void lvbo(uint8 num)
{
	uint8 line;
	for(line=START_Y+4;line<=valid_line;line++)
	{
		if((Bline_left[line]>Bline_left[line-1])&&(Bline_left[line]>Bline_left[line+1]))
		{
			if(((Bline_left[line]-Bline_left[line-1])>num)
			&&((Bline_left[line]-Bline_left[line+1])>num))
			{
				Bline_left[line]=(Bline_left[line-1]+Bline_left[line+1])/2;
			}
		}
		else if((Bline_left[line]<Bline_left[line-1])&&(Bline_left[line]<Bline_left[line+1]))
		{
			if(((Bline_left[line-1]-Bline_left[line])>num)
			&&((Bline_left[line+1]-Bline_left[line])>num))
			{
				Bline_left[line]=(Bline_left[line-1]+Bline_left[line+1])/2;
			}
		}

		if((Bline_right[line]>Bline_right[line-1])&&(Bline_right[line]>Bline_right[line+1]))
		{
			if(((Bline_right[line]-Bline_right[line-1])>num)
			&&((Bline_right[line]-Bline_right[line+1])>num))
			{
				Bline_right[line]=(Bline_right[line-1]+Bline_right[line+1])/2;
			}
		}
		else if((Bline_right[line]<Bline_right[line-1])&&(Bline_right[line]<Bline_right[line+1]))
		{
			if(((Bline_right[line-1]-Bline_right[line])>num)
			&&((Bline_right[line+1]-Bline_right[line])>num))
			{
				Bline_right[line]=(Bline_right[line-1]+Bline_right[line+1])/2;
			}
		}
	}
}
void my_shizi_bx()
{
    int16 is=0;
    uint16 ZX_StopFlag=0,YX_StopFlag=0,ZX_StopLine=0,YX_StopLine=0,lost_line_num=0;
    if(ZX_StopFlag!=1&&valid_line<58)
    {
     /**************�ڶ���������¶˹յ㣬��������¹յ����**************/
       for(is=5;is<50;is++)//
       {
         //���¶����������ߴ����Ҵ���0���յ�֮���ɽ��˵��յ�ʵ������ƣ��ұȽ϶�����֮�ϳʵݼ����ƣ��ұȽϻ���.
          if(ZX_StopFlag==0&&(Bline_right[is]>2&&Bline_right[is-2]>1&&Bline_right[is]-Bline_right[is-2]>0&&Bline_right[is]-Bline_right[is-3]>0&&
             Bline_right[is]-Bline_right[is+2]>=0&&Bline_right[is]-Bline_right[is+3]>0&&(Pick_flag[is]&LEFT_LOST_W)))
          {
            ZX_StopLine=is;//�ɽ���Զ Ѱ�����¶˹յ�
            ZX_StopFlag=1;  //�ұ��йյ㣬��ѯ������Ƿ�ʧ
          }
          //���¶����������ߴ�����С��CAMERA_W���յ�֮���ɽ��˵��յ�ʵݼ����ƣ��ұȽ϶�����֮�ϳʵ������ƣ��ұȽϻ���.
          if(YX_StopFlag==0&&Bline_left[is]<V-1&&Bline_left[is-2]<V-1&&Bline_left[is]-Bline_left[is-2]<=0&&Bline_left[is]-Bline_left[is-3]<0&&
             Bline_left[is]-Bline_left[is+2]<0&&Bline_left[is]-Bline_left[is+3]<0&&((Pick_flag[is]&RIGHT_LOST_W)))
          {
            YX_StopLine=is;//Ѱ�����¶˹յ�
            YX_StopFlag=1;  //����йյ㣬��ѯ�ұ����Ƿ�ʧ
          }
          if(ZX_StopFlag||YX_StopFlag)//�ҵ�����������һ���йյ�
          {
              break;
          }
       }
       if(ZX_StopFlag)
       {
          for(is=0;is<40;is+=2)
          {
              if(Pick_flag[is]&LEFT_LOST_W)            
              {
                  lost_line_num++;
              }
              if(lost_line_num>=10)
              {
                  valid_line=ZX_StopLine;
              }
          }
       }
       else if(YX_StopFlag)
       {
          for(is=10;is<40;is++)
          {
              if(Pick_flag[is]&RIGHT_LOST_W)            
              {
                  lost_line_num++;  
              }
              if(lost_line_num>=25)
              {
                  valid_line=YX_StopLine;
              }
          }
       }
    }
}
/*******************************************************************************
�������ƣ�center_buxian
��������: �˲�
������
*******************************************************************************/
void center_buxian()
{
  uint8 line;
  for(line=START_Y+1;line<valid_line;line++)
  {
	if((Pick_table[line]>Pick_table[line-1])&&(Pick_table[line]>Pick_table[line+1]))
	{
		Pick_table[line]=(Pick_table[line-1]+Pick_table[line+1])/2;
	}
	else if((Pick_table[line]<Pick_table[line-1])&&(Pick_table[line]<Pick_table[line+1]))
	{
		Pick_table[line]=(Pick_table[line-1]+Pick_table[line+1])/2; 
	}
        if(my_debug_flag==1)
        {
            test_draw_point(line,Pick_table[line],0XFFE0);
        }
  }
}
/*******************************************************************************
�������ƣ�bu_xian
��������: �˲�
������
*******************************************************************************/
void bu_xian()
{
  uint8 line;
  uint16 temp1,temp2;
  for(line=4;line<50;line++)
  {
	if((Bline_left[line]>Bline_left[line-1])&&(Bline_left[line]>Bline_left[line+1]))
	{
		temp1=(Bline_left[line-1]+Bline_left[line-2])/2;
	  	temp2=(Bline_left[line+1]+Bline_left[line+2])/2;
	  	Bline_left[line]=(temp1+temp2)/2;
	}
	else if((Bline_left[line]<Bline_left[line-1])&&(Bline_left[line]<Bline_left[line+1]))
	{
		temp1=(Bline_left[line-1]+Bline_left[line-2])/2;
  		temp2=(Bline_left[line+1]+Bline_left[line+2])/2;
  		Bline_left[line]=(temp1+temp2)/2;
	}
	
	if((Bline_right[line]>Bline_right[line-1])&&(Bline_right[line]>Bline_right[line+1]))
	{
		temp1=(Bline_right[line-1]+Bline_right[line-2])/2;
	  	temp2=(Bline_right[line+1]+Bline_right[line+2])/2;
	  	Bline_right[line]=(temp1+temp2)/2;
	}
	else if((Bline_right[line]<Bline_right[line-1])&&(Bline_right[line]<Bline_right[line+1]))
    {
		temp1=(Bline_right[line-1]+Bline_right[line-2])/2;
  		temp2=(Bline_right[line+1]+Bline_right[line+2])/2;
  		Bline_right[line]=(temp1+temp2)/2;
	}
  }
}

/**********************************************************
�����߼��
***********************************************************/
int mark_stop()
{
    int line_num=10,line=valid_line+1,x=0;
    int b_point_num=0,last_color=0,color=0;
    if(valid_line<30)
    {
        for(line=valid_line+1;line<valid_line+line_num;line++)
        {
            for(x=10;x<IMG_W-10;x++)
            {
                color=get_img_point(line,x);
                if(color!=last_color)//����
                    b_point_num++;
                last_color=color;
            }
        }
    }
    else
    {
        for(line=0;line<line_num;line++)
        {
            for(x=10;x<IMG_W-10;x++)
            {
                color=get_img_point(line,x);
                if(color!=last_color)//����
                    b_point_num++;
                last_color=color;
            }
        }
    }
    if(b_point_num>40)//�ڰ�������� 90  66
    {
      stop_car_line=1;
      zhidao_count_flag=2;//��⵽������ ˵����ֱ���� 
      return 1;    
    }
    else
    {
      stop_car_line=0;
      return 0;
    }
}
/**********************************************************
�����߼��
***********************************************************/
//�����߼���  ͣ��
int stop_car_flag=0;
void check_and_stop(int num,int flag)
{
	int count=50;
//	if(num>count&&flag&&valid_line<10)//ͣ����������
        if(num>count&&flag)//ͣ����������          
	{
            pidl.setPoint=0;
            pidr.setPoint=0;
            stop_car_flag=1;
  	}
        if(flag)//�����߳ɳ��������������������У�
        {
            start_line_ring_flag=1;
        
        }
}


















