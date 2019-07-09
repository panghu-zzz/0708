#include "common.h"
#include "include.h"
#include "speed_new.h"
#include  "info_deal_save.h"
#include "control.h"
#include "usart_file.h"
#include "direction.h"
#include "ring.h"
#include "path.h"
#include "math.h"
#include "my_cfg.h"
#include "roundadout.h"
struct motor pidl={0};
struct motor pidr={0};

/*************************
���PID��ʼ��
*************************/
void PID_init(void)
{
	//����PID��ʼ��
	pidl.P = speed_P;
	pidl.I = speed_I;
	pidl.D = speed_D;
}
void MOTOR_init()
{
	PID_init();
	//����Լ������ʼ��
	ftm_pwm_init(MOTOR_FTM, MOTOR1_PWM,MOTOR_HZ,0);      //��ʼ�� ��� PWM
	ftm_pwm_init(MOTOR_FTM, FTM_CH7,MOTOR_HZ,0);      //��ʼ�� ��� PWM
	ftm_quad_init(FTM1);                                 //FTM1 ���������ʼ�������õĹܽſɲ� port_cfg.h ��
}
/******************************
        ��ȡ������
********************************/
void Getpluse(void)
{
	pidl.pluse = -ftm_quad_get(FTM1);    //��ȡFTM1 �������� ��������A8 A9
	ftm_quad_clean(FTM1);               //����������������ֵ��������գ��������ܱ�֤����ֵ׼ȷ��
}

/********************************
    ���PID����
**********************************/
void motorPID111()
{
    int16 error0,error1,error2;
    float P,I,D;

    error0 = error1 = error2 = 0;
    error0 = pidl.setPoint - (pidl.pluse);//I    
    error1 = error0 - pidl.lastError;//P
    error2 = error0 - 2*pidl.lastError + pidl.preError;//D
    pidl.preError = pidl.lastError;
    pidl.lastError = error0;
  
    P=pidl.P;    I=pidl.I;      D=pidl.D;
    pidl.PWM+= (int16)(P*error1 + I*error0 + D*error2);    
    //if(error0>60) pidl.PWM = 9999;
    //if(error0<-35) pidl.PWM = -9999;

    
    
    if(pidl.PWM > 9999)  pidl.PWM = 9999;
    if(pidl.PWM < -9999   )  pidl.PWM = -9999;

}

void motor_out()//1 3 ����
{       
  
   if(pidl.PWM >= 0)//
   {
       M1(pidl.PWM);//D5
       M2(10000);//D7
   }
   else
   {   
      M1(-pidl.PWM);
      M2(0); 
   }   
    
}




/********************************
    ���õ��Ŀ�����
*********************************/
int Vspeed=0;
int more_speed=0;
/*
�ٶȿ��Ʒ�����
max_sp    ����ٶ�
min_sp    ��С�ٶ�
e         ��ͬ��ʽ���ڲ�ͬ���
way_num   �������
*/
int speed_way(int max_sp,int min_sp,int diff)
{

    int more_speed=0;
//////////////  /**********************�����������ٶ�***********************************************/
//////////////    //��ʽ=����ٶ�-����ƽ��*�ٶȲ�/�������ƽ��  �õ��ľ�������ֵ�ٶ�
//////////////    //           more_speed=((abs(even_diff*even_diff))*(max_speed-min_speed))/(img_max_error*img_max_error);
//////////////    //           Vspeed=(int)(max_speed - more_speed);//��2�η��̵���
//////////////  /***********************������Ч�м����ٶ�******************************************/
////////////////             more_speed=((abs(img_error_v*img_error_v))*(max_speed-min_speed))/(900);//33*33
////////////////             Vspeed=(int)(max_speed - more_speed);//��2�η��̵���
//////////////
////////////////      if(zhidao_count_flag==2)//ֱ��
////////////////     {
////////////////         max_sp=L_zhidao_speed;
////////////////         min_sp=L_zhidao_speed-5;
////////////////     }
////////////////     else if(zhidao_count_flag==1)//��ֱ��
////////////////     {
////////////////          max_sp=L_zhidao_speed-20;
////////////////          min_sp=L_zhidao_speed-30;
////////////////     }
////////////////     more_speed=((abs(even_diff*even_diff))*(max_sp-min_sp))/(33*33);//33��ͼ���������,
////////////////     Vspeed=(int)(max_sp - more_speed);//��2�η��̵���
////////////////    
////////////////    return Vspeed;
//      if(zhidao_count_flag==2)//ֱ��
//     {
//        max_sp=L_zhidao_speed;
//        min_sp=L_zhidao_speed-5;
//         Vspeed=L_zhidao_speed;
//           Vspeed=180;   //218  180
//     }
//    else if(zhidao_count_flag==1)//��ֱ��
//     {
//          max_sp=L_zhidao_speed-20;
//         min_sp=L_zhidao_speed-30;
//         Vspeed=L_zhidao_speed-30;
//          Vspeed=150;   //188  160
//
//    }
////     more_speed=((abs(even_diff*even_diff))*(max_sp-min_sp))/(33*33);//33��ͼ���������,
////     Vspeed=(int)(max_sp - more_speed);//��2�η��̵���
//    else
//     {
//        Vspeed=112;//100
//     }
    if(gl_zhangai_flag)//�ϰ���־
    {
//        Vspeed=CD_speed-30; //95  
       Vspeed=80; //95  
    } 
    else if(ring_left_flag||ring_right_flag)//�������ȼ����
    {
         Vspeed=90;//��·�ٶȲ���  100����  10Ȧû���⣨72������  3����·��
    }
    else  if(zhidao_count_flag==2)//ֱ��
    {
//        Vspeed=180;
       Vspeed=L_zhidao_speed; //��ֱ���ٶ�       
    }
    else if(zhidao_count_flag==1)//��ֱ��
    {
//         Vspeed=L_zhidao_speed-20; //��ֱ���ٶ� zhidao_speed 
          Vspeed=zhidao_speed; //��ֱ���ٶ� zhidao_speed 
    }
     else
    {
        Vspeed=CD_speed;//ȫ���ٶȣ�����ٶȣ�
    }
   
    return Vspeed;

}

int delay_stop_car=20;
void set_ideal_speed(int16 error)
{
    int max_speed=zhidao_speed,min_speed=CD_speed;
    int speed_diff_mark=0;//�������
    int img_max_error=33;//ͼ��������
    int img_error_v=57-valid_line;
    if(img_error_v>img_max_error)
    {
        img_error_v=img_max_error;
    }
    Vspeed=speed_way(max_speed,min_speed,valid_line); 

    if(stop_car_flag==1)//ͣ����־λ
    {
        zhidao_speed=0;
        CD_speed=0;
        Vspeed=0;
  
    }    
    if((stop_car_flag==1))
    {
      delay_stop_car--;   
      if(delay_stop_car<1)
      {
        delay_stop_car=0;    
      }
      if(!(delay_stop_car))
      {
        max_speed=zhidao_speed=0;
        min_speed=CD_speed=0; 
      }     
    }
//	if(ramp_flag==1&&(abs(even_diff-0)<=2))//�µ���־
//	{
//		max_speed=CD_speed;//ȫ������ٶ�
//		min_speed=CD_speed;//ȫ������ٶ�
//	}
//	if(gl_zhangai_flag)//�ϰ���־
//	{
//	    max_speed=CD_speed-10;
//	    min_speed=CD_speed-10;
//	}                                               


     pidl.setPoint=(int16)Vspeed;

}


void control_speed()
{
    set_ideal_speed(jiaodu_num);
    Getpluse();
    motorPID111();
    motor_out();

}
