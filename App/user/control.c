#include "path.h"
#include "control.h"
#include "includes.h"
#include "speed_new.h"
#include "direction.h"
#include "roundadout.h"
#include "info_deal_save.h"  
#include "ring.h"


uint8 run_start_flag=0;//�Ƿ�ʼ��
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
uint16 lost_already=0;
uint8 Pick_line=0;
uint16 maxBline_diff;
//uint8 PickCenter_flag=0;	//��ȡ���߱�־
uint8 Lost_Line_count=0;
uint8 Lost_left_count=0;
uint8 Lost_right_count=0;
uint8 Near_lost=0;//������ʧ��־
uint8 Shi_zi_line=0;
uint8 Shi_zi_num=0;
uint8 Shi_zi_flag=0;
uint8 stop_num=0;
int Position_diff=0;
uint8 Out_flag;		//�����־����Чͼ���־


uint8 last_vline;
uint8 valid_line=0;//�����Ч��
uint8 judge_vl;//�����жϵ���Ч��
uint8 last_lost=55;	//��һ����ʧ��
uint16 Bline_diff=0;//�����߾���

/***************************************************************/
int judge_xielv[CAMERA_H-5];	//б���ж����� 
uint8 zhidao_count_flag=0;	//ֱ���жϱ�־
uint8 last_zhidao_flag=0;
uint8 lost_w_count=0;//��ɫ��ʧ�б���
uint8 lost_b_count=0;//��ɫ��ʧ�б���

int even_diff=0;	//������ƽ��ƫ��
int even_diff_near=0;
float D_differen=0.0;   //
int jiaodu_num=dj_center;	//�Ƕ�ֵ
int last_turn=dj_center;	//��һ��ת��ֵ
int dj_pid_num=dj_center;	//����Ƕ�ֵ

int dj_act_jiaodu=dj_center;//���ʵ�ʽǶ�

uint8 set_flag=1;//���ñ�־λ
uint8 gl_zhangai_flag=0;
uint8 gl_zhangai_start=0;
uint8 gl_zhangai_end=0;

int ring_num=0;
int no_ring_num=0;
int RING_FLAG=0;
int HIGHT_RING_FLAG=0;
int one_flag=0;

int fact_weight[60]={0};

void control_hander()
{
    if((PIT_TFLG(1)&PIT_TFLG_TIF_MASK)!=0)
    {
      if(run_start_flag)
      {
            control_speed();
      }
    }
    PIT_TFLG(1)|=PIT_TFLG_TIF_MASK;       //���־
}
void duoji_hander()
{
    if((PIT_TFLG(2)&PIT_TFLG_TIF_MASK)!=0)
    {
        img_deal_flag=1;
    }
    PIT_TFLG(2)|=PIT_TFLG_TIF_MASK;       //���־
}
//���� ����ʼ�����Ƶ����ڣ�pid�����ڣ�
void init_control_circle()
{   
    set_vector_handler(PIT1_VECTORn,control_hander);   // �����жϷ��������ж���������
    set_vector_handler(PIT2_VECTORn,duoji_hander);   // �����жϷ��������ж���������
    pit_init_ms(PIT_PORT,PIT_CNT);
    pit_init_ms(PIT_PORT_time,PIT_time);//����ͼ��ɼ�����
    set_irq_priority((IRQn_Type)PIT1_VECTORn,2);
    set_irq_priority((IRQn_Type)PIT2_VECTORn,2);
}

void enable_pit_int(PITn_e pitn)
{
    enable_irq((IRQn_Type)((int)pitn + PIT0_IRQn));            //���ж�
}
void disable_pit_int(PITn_e pitn)
{
    disable_irq((IRQn_Type)((int)pitn + PIT0_IRQn));            //���ж�
}


void init_ctrl()
{
    init_control_circle(); //��ʼ��PIT1�����ڿ����ٶȲɼ����ڣ�û���ж�
    //MOTOR_init();//��ʼ�����
    enable_pit_int(PIT2);//��ʼͼ��ɼ�
    
#ifdef OPEN_CIRCLE_CONTROL //����������Ʋ����������ٶȵ��ж�
      //qianjin();           //����ֱ����
#else
    enable_pit_int(PIT1);  //��PIT1�ж��л�ȡ������ֵ������ס�ٶ�
//    lptmr_pulse_counter(BMQ_L_PIN);//PORTA19 Ϊ�������ź����룬���ڱ���������
#endif
    
//    InitPar();//��ʼ������//���ڴ�������� ��ֵ��ȫ�ֱ���
   
}
void pirntf_path_type()
{
    switch(gl_path_type)
    {
    case Short_zhidao: printf("Short_zhidao\n");break;
    case lean_zhidao: printf("lean_zhidao\n");break;
    case Shi_zi: printf("Shi_zi\n");break;
    case Zhang_ai: printf("Zhang_ai\n");break;
    case Xiao_S: printf("Xiao_S\n");break;
    case Zhong_S: printf("Zhong_S\n");break;
    case Da_S: printf("Da_S\n");break;
    case Xiao_wan: printf("Xiao_wan\n");break;
    case Zhong_wan: printf("Zhong_wan\n");break;
    case Da_wan: printf("Da_wan\n");break;
    case T_Da_wan: printf("T_Da_wan\n");break;
    }
}
//�Ķ���ȡ�����У��Ķ��˵������Ķ���,�Ķ���ҡͷ���Ƶ��
int loop_none_hang()//��·����� �ͳ���
{
    int lie=0;
    int ret=0,ret_1=0;
    int hang=0;
    for(hang=1;hang<6;hang++)//5
    {
        for(lie=0;lie<80;lie++)
        {
            if(get_img_point(hang,lie))
            {
                  ret++;      
            }    
        }
    }

    if(ret<3)//3
    {
        for(hang=6;hang<59;hang++)
        {
            if(get_img_point(hang,40))
            {
                  ret_1=hang;  
                  break;
            }    
        }
    }
    else
    {
        ret_1=1000;
    }
    return ret_1;

}
int ratio=0;

/*******************************************************************************
�������ƣ�stable_del
��������: ��ǰ�����ݴ�����Ƴ���
��������
*******************************************************************************/
int last_duoji_e=0;
int loop_ok_num=0;//Բ���˲�����

int in_shizi_flag=0;
int out_shizi_flag=0;
int other=0;
int other_one=0;
int shizi_on_off=0;
int loop_guaidian_flag=0;


void lose_line_find()
{
//  int hang=START_Y+3;
//  int left_lose_hang_pos=0,right_lose_hang_pos=0;
//  for(hang=8;hang<42;hang++)//���ж���߶������
//  {
//    if(left_pos[hang]>40&&left_pos[hang]<75)//ʵ�ʵı��߶�����
//    {
//        left_lose_hang_pos++;//��¼��������
//    }
//  }
//  for(hang=8;hang<42;hang++)
//  {
//    if(right_pos[hang]>1&&right_pos[hang]<40)
//    {
//        right_lose_hang_pos++;//��¼��������
//    }
//  }
////  if(left_lose_hang_pos>=30&&right_lose_hang_pos>=30)
////  {
////    line_lose_flag=1;
////  }
////  else 
//    
//   if((left_lose_hang_pos-right_lose_hang_pos)==0)
//  {
//    line_lose_flag=1;
//  }
//  else 
//  {
//    line_lose_flag=0;
//  }
  if(line_lockup(Bline_left,129)==1&&line_lockup(Bline_right,67)==1)
  {
    line_lose_flag=1;
  }
  else
  {
    line_lose_flag=0;
  }
    

}
//
//���ڹս�Բ���Ժ󣬲鿴�ױ��Ƿ����ߣ�
int lose_pos_flag()
{
 int num=0;
 int ret=0;
 int left_num=0,right_num=0;
 if(ring_right_flag)
 {
    for(num=START_Y;num<=50;num++)
     {
       if(Bline_left[num]>=Bline_left[num+2])
       {
            left_num++;
            if(num>35)
            {
                ret=1;
                return ret;
            }
       }
       else
       {
            break;
       } 
     }
 }
 if(ring_left_flag)
 {
    for(num=START_Y;num<=50;num++)
     {
           if(Bline_right[num]<=Bline_right[num+2])
            {
                left_num++;
                if(num>35)
                {
                    ret=1;
                    return ret;
                }
                else
                {
                    break;
                }
            }     
     }
 
 }
 
 
    return ret;
    
}


int get_flag()
{   
    int num=30,i=8;
    int flag=0;
    if(ring_right_flag)
    {
        for(i;i<num;i++)
        {
            if(right_pos[i]==1)
            {
                flag++;
            }        
        }
    }
    else if(ring_left_flag)
    {
        for(i;i<num;i++)
        {
            if(left_pos[i]==79)
            {
                flag++;
            }        
        }
    }
    return flag;

}

int get_out_ring_w(int start,int end)
{
    int num=0;
    int weight=0;
    int flag=0;
    for(num=start;num<end;num++)
    {
        if(line_weight[num]>75||line_weight[num]==0)
        {
            flag++;
        }        
    }
    //weight=weight/(end-start);
    return flag;

}

    


int normal_flag=0;
int enter_ring_kp_flag=0;
int val=0;
int w=0;
int ww=0;
int num=0;
int time_num=0;
int val_num=0;
int out=0;
int ring_size=0;
int size_flag=0;
int ring_enter_process=0;

void get_ring_size()
{
  int hang=0,lie=0;
  lie=ring_lie_pos+12;
    for(hang=ring_hang_pos;hang<55;hang++)
    {
        if(get_img_point(hang,lie)==0&&get_img_point(hang+1,lie)!=0)
        {
            ring_size=hang;
            break;
        }
    }

}
void sigle_buxian()//���ಹ��
{
    if(ring_right_flag)
    {
      if(even_diff>=0)
      {
        even_diff=-even_diff;
      }
      else 
      {
        with_right_to_center();  
      }
              
    }
    else if(ring_left_flag)
    {
      if(even_diff<=0)
      {
        even_diff=-even_diff;
      }
      else 
      {
        with_left_to_center();
      }
        
    }          
}

int again_clear_flag=0;
void stable_del()
{   
//        int side=38;  //35
        int side=38;  //35
  
        xielv_lvbo();//�����ֵ���Ʒ��˲� ȥ����Ч��
        lvbo(5);//�˲�
        ti_jiaozheng(START_Y,valid_line);//����ͼ�������ʧ��       	    
       
        bDistance();//������ȷ��˲� ȥ����Ч��
        //val=get_val(5,50,Pick_table,2);//�����·ʱ�����߹յ�λ��
        lose_line_find();//�鿴ʵ�ʶ������
        //lose_pos_flag();
        
        ring_size=fing_enter_pos();
        w=get_line_weight(5,10);//�������
        ww=get_line_weight(25,30);//�������
        
        //check_and_stop(total_time,mark_stop());//�����߼��
        //&&!start_line_ring_flag
//       if(!line_lose_flag&&!ring_fill_line_flag&&!out_ring_flag&&!out&&valid_line>=50&&!gl_zhangai_flag)//  �жϳ���·�Ժ�Ͳ����ж���
        if(!line_lose_flag&&!ring_fill_line_flag&&!out_ring_flag&&!out&&valid_line>=50&&!gl_zhangai_flag&&(total_time>=50))//  �жϳ���·�Ժ�Ͳ����ж���
       {//total_time
//           round_deal();//����ʶ�� ����   
         //��ͨ�����ܺ��Ժ�  ��ע��
       }    
        if(Shi_zi_flag&&!ring_fill_line_flag&&!out)//��·��ʮ������
        {   
            shizi_find_line();//��������//�������ж�Բ��©��
            bDistance();//������ȷ��˲� ȥ����Ч��  
        }
        /**************************************************************/
        /*************************************************************/
        if(ring_fill_line_flag&&!normal_flag&&ring_size>side)//Բ�����߱�־
        {//�뻷����           
          sigle_buxian();
        }
       if(ring_fill_line_flag&&!normal_flag&&ring_size<=side&&!find_diu_hang(ring_size))
       {
            sigle_buxian();
       }
       else
        {
            getBlineCenter();//��ϳ����� //�������� ����  
        }                    
      
      	averageLvBo();//��ֵ�˲�
        center_buxian();//Ҳ�˲�
        If_LStraight();  //�ж��Ƿ�Ϊֱ��        
       // check_and_stop(total_time,mark_stop());//�����߼��
        get_even_diff_s();//��ȡ��Ч����������ƫ��ƽ��ֵ����ȡ��
        /*************************************************************/   
        if(ring_fill_line_flag&&!normal_flag)//������·�ص�����
        {          //�뻷����
            zhidao_count_flag=0;//ֱ����־λ��0
            if(ring_right_flag&&((ring_size<=side)||Shi_zi_flag||valid_line<=30)&&find_diu_hang(ring_size))//
            {
                even_diff=-33;    //��ֹ���򶶶�
                ring_enter_process=1;                
                enter_ring_kp_flag=1;                
                if(Shi_zi_flag>=10) 
//                 if((Shi_zi_flag<15&&Shi_zi_flag!=0)) 
                {
                    guai_shizi_ring_flag=1;//��·��խ���ڶ������λ�ô���
                }
            }
//            else if(ring_right_flag&&even_diff>=0&&find_diu_hang(ring_size)&&(ring_size<=35))
            else if(ring_right_flag&&ring_enter_process)
            {
               enter_ring_kp_flag=1;
               even_diff=-33;
            }              
            if(ring_left_flag&&((ring_size<=side)||Shi_zi_flag||valid_line<=30)&&find_diu_hang(ring_size))//
            {
                even_diff=33;  
                ring_enter_process=1;
                enter_ring_kp_flag=1;
                 if(Shi_zi_flag>=10) 
 //               if((Shi_zi_flag<15&&Shi_zi_flag!=0)) 
                {
                    guai_shizi_ring_flag=1;
                }
            }
//            else if(ring_left_flag&&even_diff<=0&&find_diu_hang(ring_size)&&(ring_size<=35))
            else if(ring_left_flag&&ring_enter_process)
            {
              enter_ring_kp_flag=1;
              even_diff=33;  
            }                        
        }  
//       
        if(guai_shizi_ring_flag==1&&Shi_zi_flag==0&&!normal_flag)
        {
            zhidao_count_flag=0;//ֱ����־λ��0
            time_num++;//��¼��ʮ�ֵĴ���  
            //if(time_num>1)//�˲�
            {
                time_num=0;
                normal_flag=1;//��·����
            }
        }            
        if(normal_flag)//�ڻ�·����Ԥ�г���
        {//��������
          zhidao_count_flag=0;//ֱ����־λ��0
          enter_ring_kp_flag=0;
          ring_enter_process=0;
          val=get_out_ring_zhedian();//��������۵�
            if(val<=18)//�����۵�//  10  ��·�뾶ԼԽ��  Ӧ��ԽС
            //if(val<=get_guai_val())//�����۵�//  10  ��·�뾶ԼԽ��  Ӧ��ԽС              
            {
                if(ring_right_flag)
                {
                    even_diff=-33;    //��ֹ���򶶶�
                    out_dasi_flag=1;
                }             
                else if(ring_left_flag)
                {
                    even_diff=33;  
                    out_dasi_flag=1;
                }                
            }                   
            if(val>32)//�ڻ�·�����۵㿿��
            {
                val_num++;
                if(val_num>15)val_num=0;out_ring_flag=1;
            }           
        }

//        if(out_ring_flag&&out_dasi_flag&&out_dasi_flag&&(Shi_zi_flag<=15&&Shi_zi_flag>0))
        if(out_ring_flag&&out_dasi_flag&&out_dasi_flag&&Shi_zi_flag)
        {
          out=1;
          zhidao_count_flag=0;//ֱ����־λ��0
          if(ring_right_flag)
          {
              even_diff=-33;    //��ֹ���򶶶�
          }             
          else if(ring_left_flag)
          {
              even_diff=33;  
          }    
        }
//        if(out&&out_ring_flag&&out_dasi_flag&&valid_line>=50)//40
        if(out&&out_dasi_flag&&valid_line>=50)//40
        {
            zhidao_count_flag=0;//ֱ����־λ��0
            out_ring_flag=0;
            out=0;
            val=0;
            normal_flag=0;
            ring_size=0;
            enter_ring_kp_flag=0;
            ring_enter_process=0;
            out_dasi_flag=0;
            recognition_B_num=0;
            black_point_start=0;
            ring_clear();
        }        

        duoji_PD(even_diff);//�������ͨ�����PD��ȡ���ת��  even_diff���������                         
       
        way_control(); //���ת��

}
/*******************************************************************************
�������ƣ�clearDelPar
��������: ���㴦�����
��������
*******************************************************************************/
void clearDelPar()
{
	lost_already=0;
	Lost_Line_count=0;
	Lost_left_count=0;
	Lost_right_count=0;
	Pick_line=0;
	Shi_zi_line=0;
	Bline_diff=0;
	maxBline_diff=0;
	Shi_zi_flag=0;
	lost_w_count=0;
	lost_b_count=0;
        valid_line=0;
        gl_zhangai_start=0;
        gl_zhangai_end=0;
        gl_zhangai_flag=0;
        guai_dian_count=0;
        donw_guan_dian=0;
}
/******************************* ������  **************************************/
void ctrl_main()
{
    uint32 line;
 
    
    /////////////////////ͼ��ɼ����֣��ҵ���������///////////////////////////////
    clearDelPar();//���㴦�����
    for(line=0;line<IMG_H;line++)//��ű���������������
    {
        Bline_left[line]=V/2;
        Bline_right[line]=V/2;
        Pick_flag[line]=0;//������־����
        Deal_flag[line]=0;//�����־����        
        left_pos[line]=V/2;
        right_pos[line]=V/2;
        center_pos[line]=0;        
    }
    for(line=START_Y+2;line<IMG_H;line++) //��ȡ�������ĵ㲢��������
    //for(line=0;line<IMG_H;line++) //��ȡ�������ĵ㲢��������
    {
        if(line==START_Y+2)
        { 
            Near_lost=PickCenter_near();
        }
        else
        {
            PickCenter_diff(line);//Ѱ�����ұ��� �������е����꣬�ҵ��е����꣩ 
            //�ռ���·����Ҫ������
            left_pos[line]=Bline_left[line];//������ߵ�����
            right_pos[line]=Bline_right[line];//�����ұߵ�����
            fact_weight[line]=left_pos[line]-right_pos[line];//����ͼ����
            center_pos[line]=(left_pos[line]+right_pos[line])/2;//�ϳ���������
        }
////////////////////////////////ʮ���б�ʽ///////////////////////////////////////
         if(Shi_zi_flag==0&&lost_w_count>=8)//����Ч�ж�ʧ֮ǰ�ж϶��������ж�ʮ�ֵ�/8
        {           
              Shi_zi_flag=line;
              if(line>10)
                valid_line=line-10;
              else
                valid_line=0;
              break;
        }
///////////////////////////��Ч���ж�//////////////////////////////////////////
        if((Pick_flag[line]&LEFT_LOST_B)||(Pick_flag[line]&RIGHT_LOST_B))
          {
              if((Pick_flag[line-1]&LEFT_LOST_B)||(Pick_flag[line-1]&RIGHT_LOST_B))
                  lost_b_count++;
          }
          if(line<25)
          {
              if(maxBline_diff<Bline_diff)
                  maxBline_diff=Bline_diff;
          }
          if(lost_already==0)            
          {
              if((lost_b_count>3||(Bline_left[line]<5)||(Bline_right[line]>(V-5)))&&valid_line==0)
              {
                  lost_already=1;
                  valid_line=line-3;
              }
          }
        if(line==IMG_H-1&&valid_line==0)
        {
            valid_line=IMG_H-1-lost_b_count;
        }
     }
     out_ring_under_find_line();
     stable_del();//����ͼ��

}   
