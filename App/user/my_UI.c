#include "my_UI.h"
#include "control.h"
#include "speed_new.h"
#include "VCAN_LCD.h"
#include "path.h"
#include "my_cfg.h"
#include "VCAN_key.h"
#include "ring.h"
#include "roundadout.h"
#include "roundadout.h"
//���� �����м����ٶ�ѡ�� ��ͼ����ͼ����ֵѡ��
//���������󷵻���һ�˵����Ҽ�������һ���������м�İ����������˵���
//******���˵���һ��������**** �м��ȷ�� ����0.3��
//
uint8 key_state;
uint8 UI_state=MAIN_UI;
uint8 choose_num=0;
uint16 cloor_table[16]={BCOLOUR,BCOLOUR,BCOLOUR,BCOLOUR,BCOLOUR,BCOLOUR,BCOLOUR,
                        BCOLOUR,BCOLOUR,BCOLOUR,BCOLOUR,BCOLOUR,BCOLOUR,BCOLOUR};
void draw_mark_line();
uint8 wait_key_down()
{
    uint8 ret_key=0xff;
    uint16 key_time=0;
    while(1)
    {
      if(key_check(KEY_U)==KEY_DOWN)
          ret_key=KEY_U;
      if(key_check(KEY_D)==KEY_DOWN)
          ret_key=KEY_D;
      if(key_check(KEY_L)==KEY_DOWN)
          ret_key=KEY_L;
      if(key_check(KEY_R)==KEY_DOWN)
          ret_key=KEY_R;
      if(key_check(KEY_A)==KEY_DOWN)
          ret_key=KEY_A;
      if(key_check(KEY_START)==KEY_DOWN)
          ret_key=KEY_START;
      if(key_check(KEY_B)==KEY_DOWN)
          ret_key=KEY_B;
      if(key_check(KEY_STOP)==KEY_DOWN)
          ret_key=KEY_STOP;
      if(ret_key!=0xff)
      {
          key_state=KEY_DOWN;
          while(key_get(ret_key)==KEY_DOWN)
          {
              DELAY_MS(1);
              key_time++;
          }
          if(key_time>300)
          {
              key_state=KEY_HOLD;
          }
          break;
      }
      camera_get_img();
      ctrl_main();
    }
    return ret_key;
}

void main_ui()
{
    uint8 str_buf[STR_BUF_LEN];
    uint8 char_H=16,key_num=0xff;
    Site_t site;
    while(1)
    {
        site.y = 5;
        site.x = 3;
        cloor_table[choose_num]=RED;
        LCD_clear(BCOLOUR);
        memset(str_buf,0,STR_BUF_LEN);
        sprintf((char *)str_buf,"0 run       ");
        LCD_str(site,str_buf,FCOLOUR,cloor_table[0]);   //��ʾ8*16�ַ���
        site.y += char_H;
        memset(str_buf,0,STR_BUF_LEN);
        sprintf((char *)str_buf,"1 speed_1   ");
        LCD_str(site,str_buf,FCOLOUR,cloor_table[1]);   //��ʾ8*16�ַ���
        site.y += char_H;
        memset(str_buf,0,STR_BUF_LEN);
        sprintf((char *)str_buf,"2 speed_2   ");
        LCD_str(site,str_buf,FCOLOUR,cloor_table[2]);   //��ʾ8*16�ַ���
        site.y += char_H;
        memset(str_buf,0,STR_BUF_LEN);
        sprintf((char *)str_buf,"3 speed_3   ");
        LCD_str(site,str_buf,FCOLOUR,cloor_table[3]);   //��ʾ8*16�ַ���
        site.y += char_H;
        memset(str_buf,0,STR_BUF_LEN);
        sprintf((char *)str_buf,"4 set speed ");
        LCD_str(site,str_buf,FCOLOUR,cloor_table[4]);   //��ʾ8*16�ַ���
        site.y += char_H;
        memset(str_buf,0,STR_BUF_LEN);
        sprintf((char *)str_buf,"5 open img  ");
        LCD_str(site,str_buf,FCOLOUR,cloor_table[5]);   //��ʾ8*16�ַ���
        site.y += char_H;
        memset(str_buf,0,STR_BUF_LEN);
        sprintf((char *)str_buf,"6 set img   ");
        LCD_str(site,str_buf,FCOLOUR,cloor_table[6]);   //��ʾ8*16�ַ���
        cloor_table[choose_num]=BCOLOUR;
        key_num=wait_key_down();
        if(key_num==KEY_U)
        {           
            choose_num--;
            if(choose_num>7)
              choose_num=6;
        }
        else if(key_num==KEY_D)
        {
            choose_num++;
            if(choose_num==7)
              choose_num=0;
        }
        else if(key_num==KEY_R)
        {
            break;
        }
        else if(key_num==KEY_B)
        {
            choose_num=0;
            UI_state=MAIN_UI;
            break;
        }
        
    }
    switch(choose_num)
    {
    case 0:UI_state=0xff;break;
    case 1:zhidao_speed=speed_table[0][0];CD_speed=speed_table[0][1];UI_state=0xff;break;
    case 2:zhidao_speed=speed_table[1][0];CD_speed=speed_table[1][1];UI_state=0xff;break;
    case 3:zhidao_speed=speed_table[2][0];CD_speed=speed_table[2][1];UI_state=0xff;break;
    case 4:UI_state=SET_SPEED_UI;break;
    case 5:UI_state=OPEN_IMG_UI;break;
    case 6:UI_state=SET_IMG_UI;break;
    }
    choose_num=0;
}
void set_speed_ui()
{
    uint8 str_buf[STR_BUF_LEN];
    uint8 char_H=22,key_num;
    Site_t site = {3,5};
    while(1)
    {
      //ring_road_w
        
        site.y = 5;
        site.x=3; 
        cloor_table[choose_num]=RED;
        LCD_clear(BCOLOUR);
        memset(str_buf,0,STR_BUF_LEN);
        sprintf((char *)str_buf,"max speed   ");
        LCD_str(site,str_buf,FCOLOUR,cloor_table[0]);   //��ʾ8*16�ַ���
        
        site.x += 12*8;
        memset(str_buf,0,STR_BUF_LEN);
        sprintf((char *)str_buf,"%d",zhidao_speed);
        LCD_str(site,str_buf,FCOLOUR,cloor_table[3]);   //��ʾ8*16�ַ���

        site.y += char_H;
        site.x = 3;
        memset(str_buf,0,STR_BUF_LEN);
        sprintf((char *)str_buf,"mix_speed");
        LCD_str(site,str_buf,FCOLOUR,cloor_table[1]);   //��ʾ8*16�ַ���
        site.x= 12*8;
        memset(str_buf,0,STR_BUF_LEN);
        sprintf((char *)str_buf,"%d",CD_speed);
        LCD_str(site,str_buf,FCOLOUR,cloor_table[4]);   //��ʾ8*16�ַ���
        
        site.y += char_H;
        site.x = 3;
        memset(str_buf,0,STR_BUF_LEN);
        sprintf((char *)str_buf,"l_z_speed");
        LCD_str(site,str_buf,FCOLOUR,cloor_table[2]);   //��ʾ8*16�ַ���
        site.x= 12*8;
        memset(str_buf,0,STR_BUF_LEN);
        sprintf((char *)str_buf,"%d",L_zhidao_speed);
        LCD_str(site,str_buf,FCOLOUR,cloor_table[5]);   //��ʾ8*16�ַ���
        cloor_table[choose_num]=BCOLOUR;
        
        key_num=wait_key_down();
        if(key_num==KEY_D)
        {   
            if(choose_num<3)
            {
                choose_num++;
                if(choose_num==3)
                  choose_num=0;
            }
            else  if(choose_num==3)
            {
                zhidao_speed-=2;
//                if(zhidao_speed<CD_speed)
//                  CD_speed=zhidao_speed;
            }
            else  if(choose_num==4)
            {
                CD_speed-=2;
                if(CD_speed>250)
                  CD_speed=0;
            }
            else  if(choose_num==5)
            {
                L_zhidao_speed-=2;
                if(L_zhidao_speed>250)
                  L_zhidao_speed=0;
            }
        }
        else if(key_num==KEY_U)
        {
            if(choose_num<3)
            {
                choose_num--;
                if(choose_num>4)
                  choose_num=2;
            }
            else  if(choose_num==3)
            {
                zhidao_speed+=2;
                if(zhidao_speed>250)
                  zhidao_speed=250;
//                if(zhidao_speed>L_zhidao_speed)
//                {
//                    L_zhidao_speed=zhidao_speed;
//                }
            }
            else  if(choose_num==4)
            {
                CD_speed+=2;
//                if(CD_speed>zhidao_speed)
//                  zhidao_speed=CD_speed;
            }
            else  if(choose_num==5)
            {
                L_zhidao_speed+=2;
                if(L_zhidao_speed>250)
                  L_zhidao_speed=250;
            }
        }
        else if(key_num==KEY_R)
        {
            if(choose_num<3)
              choose_num+=3;
//            else
//            {
//              UI_state=MAIN_UI;
//              choose_num=0;
//              break;
//            }
        }
        else if(key_num==KEY_L)
        {
            if(choose_num<3)
            {
                choose_num=0;
                UI_state=MAIN_UI;
                break;
            }
            else
              choose_num=0;
        }
        else if(key_num==KEY_B)
        {
            choose_num=0;
            UI_state=MAIN_UI;
            break;
        }
        
    }
}
void set_img_ui()
{
    uint8 str_buf[STR_BUF_LEN];
    uint8 char_H=16,key_num;
    uint8 list_line=6;//�м��в˵�
    Site_t site = {3,3};
    while(1)
    {
        site.y = 10;
        site.x=3;
        cloor_table[choose_num]=RED;
        LCD_clear(BCOLOUR);
        
        memset(str_buf,0,STR_BUF_LEN);
        sprintf((char *)str_buf,"diff maxH ");
        LCD_str(site,str_buf,FCOLOUR,cloor_table[0]);   //��ʾ8*16�ַ���        
        site.x= 12*8;
        memset(str_buf,0,STR_BUF_LEN);
        sprintf((char *)str_buf,"%d",diff_valid_line);
        LCD_str(site,str_buf,FCOLOUR,cloor_table[list_line+0]);   //��ʾ8*16�ַ���
        
        
        site.y += char_H;
        site.x = 3;
        memset(str_buf,0,STR_BUF_LEN);
        sprintf((char *)str_buf,"diff mixH ");
        LCD_str(site,str_buf,FCOLOUR,cloor_table[1]);   //��ʾ8*16�ַ���        
        site.x= 12*8;
        memset(str_buf,0,STR_BUF_LEN);
        sprintf((char *)str_buf,"%d",MIX_distance);
        LCD_str(site,str_buf,FCOLOUR,cloor_table[list_line+1]);   //��ʾ8*16�ַ���
        
        site.y += char_H;
        site.x = 3;
        memset(str_buf,0,STR_BUF_LEN);
        sprintf((char *)str_buf,"gl_var_1 ");
        LCD_str(site,str_buf,FCOLOUR,cloor_table[2]);   //��ʾ8*16�ַ���        
        site.x= 12*8;
        memset(str_buf,0,STR_BUF_LEN);
        sprintf((char *)str_buf,"%d",gl_var_1);
        LCD_str(site,str_buf,FCOLOUR,cloor_table[list_line+2]);   //��ʾ8*16�ַ���
        
        site.y += char_H;
        site.x = 3;
        memset(str_buf,0,STR_BUF_LEN);
        sprintf((char *)str_buf,"gl_var_2 ");
        LCD_str(site,str_buf,FCOLOUR,cloor_table[3]);   //��ʾ8*16�ַ���        
        site.x= 12*8;
        memset(str_buf,0,STR_BUF_LEN);
        sprintf((char *)str_buf,"%d",gl_var_2);
        LCD_str(site,str_buf,FCOLOUR,cloor_table[list_line+3]);   //��ʾ8*16�ַ���
        
        site.y += char_H;
        site.x = 3;
        memset(str_buf,0,STR_BUF_LEN);
        sprintf((char *)str_buf,"gl_var_3 ");
        LCD_str(site,str_buf,FCOLOUR,cloor_table[4]);   //��ʾ8*16�ַ���        
        site.x= 12*8;
        memset(str_buf,0,STR_BUF_LEN);
        sprintf((char *)str_buf,"%d",gl_var_3);
        LCD_str(site,str_buf,FCOLOUR,cloor_table[list_line+4]);   //��ʾ8*16�ַ���
        
        site.y += char_H;
        site.x = 3;
        memset(str_buf,0,STR_BUF_LEN);
        sprintf((char *)str_buf,"gl_var_4 ");
        LCD_str(site,str_buf,FCOLOUR,cloor_table[5]);   //��ʾ8*16�ַ���        
        site.x= 12*8;
        memset(str_buf,0,STR_BUF_LEN);
        sprintf((char *)str_buf,"%d",gl_var_4);
        LCD_str(site,str_buf,FCOLOUR,cloor_table[list_line+5]);   //��ʾ8*16�ַ���
        
        cloor_table[choose_num]=BCOLOUR;       
        key_num=wait_key_down();
        if(key_num==KEY_D)
        {   
            if(choose_num<list_line)
            {
                choose_num++;
                if(choose_num==list_line)
                {
                    choose_num=0;
                }
            }
            else if(choose_num==list_line)
            {
                diff_valid_line-=1;
                if(diff_valid_line<5)
                  diff_valid_line=59;
            }
            else if(choose_num==list_line+1)
            {
                MIX_distance-=1;
                if(MIX_distance<1)
                  MIX_distance=diff_valid_line-3;
            }
            else if(choose_num==list_line+2)
            {
                gl_var_1-=1;
            }
            else if(choose_num==list_line+3)
            {
                gl_var_2-=1;
            }
            else if(choose_num==list_line+4)
            {
                gl_var_3-=1;
            }
            else if(choose_num==list_line+5)
            {
                gl_var_4-=1;
            }
        }
        else if(key_num==KEY_U)
        {
            if(choose_num<list_line)
            {
                choose_num-=1;
                if(choose_num>100)
                {
                    choose_num=list_line-1;
                }
            }
            else if(choose_num==list_line)
            {
                diff_valid_line+=1;
                if(diff_valid_line>59)
                  diff_valid_line=5;
            }
            else if(choose_num==list_line+1)
            {
                MIX_distance+=1;
                if(MIX_distance>diff_valid_line-3)
                  diff_valid_line=MIX_distance+3;
            }
            else if(choose_num==list_line+2)
            {
                gl_var_1+=1;
            }
            else if(choose_num==list_line+3)
            {
                gl_var_2+=1;
            }
            else if(choose_num==list_line+4)
            {
                gl_var_3+=1;
            }
            else if(choose_num==list_line+5)
            {
                gl_var_4+=1;
            }
        }
        else if(key_num==KEY_R)
        {
              if(choose_num<list_line)
              {
                  choose_num+=list_line;
              }
              else
              {
                    UI_state=MAIN_UI;
                    choose_num=0;
                    break;    
              }
        }
        else if(key_num==KEY_L)
        {
            if(choose_num<list_line)
            {
                UI_state=MAIN_UI;
                choose_num=0;
                break;
            }
            else
            {
                choose_num-=list_line;    
            }
            
        }
        else if(key_num==KEY_B)
        {
            UI_state=MAIN_UI;
            choose_num=0;
            break;
        }
        
    }
}

void show_ring_data()
{
   uint8 str_buf[30];
    uint8 char_H=16;  
    Site_t site     = {0, 0}; 
    Size_t size;
    site.x = 80;
    memset(str_buf,0,30);
    sprintf((char *)str_buf,"out %d",out);//�������־λ��ʾ
    LCD_str(site,str_buf,FCOLOUR,BCOLOUR);   //��ʾ8*16�ַ���  
    //ring_fill_line_flag
    site.y += char_H;
    memset(str_buf,0,30);
    sprintf((char *)str_buf,"fil %d",ring_fill_line_flag);//�������־λ��ʾ
    LCD_str(site,str_buf,FCOLOUR,BCOLOUR);   //��ʾ8*16�ַ���    
    
    site.y += char_H;
    memset(str_buf,0,30);
    sprintf((char *)str_buf,"vl %d",valid_line);//��Ч�б�־��ʾ
    LCD_str(site,str_buf,FCOLOUR,BCOLOUR);   //��ʾ8*16�ַ���  
    
    site.y += char_H;
    memset(str_buf,0,30);
    sprintf((char *)str_buf,"sz %d",Shi_zi_flag);//��Ч�б�־��ʾ
    LCD_str(site,str_buf,FCOLOUR,BCOLOUR);   //��ʾ8*16�ַ���  
    
    site.y += char_H;
    memset(str_buf,0,30);
    sprintf((char *)str_buf,"val %d",val);//��Ч�б�־��ʾ  Բ���ڵĹյ�λ��
    LCD_str(site,str_buf,FCOLOUR,BCOLOUR);   //��ʾ8*16�ַ���
    //normal_flag
    
    site.y += char_H;
    memset(str_buf,0,30);
    sprintf((char *)str_buf,"nor %d",normal_flag);//��Ч�б�־��ʾ  Բ���ڵĹյ�λ��
    LCD_str(site,str_buf,FCOLOUR,BCOLOUR);   //��ʾ8*16�ַ���
    //w
    site.y += char_H;
    memset(str_buf,0,30);
    sprintf((char *)str_buf,"w %d",w);//��Ч�б�־��ʾ  Բ���ڵĹյ�λ��
    LCD_str(site,str_buf,FCOLOUR,BCOLOUR);   //��ʾ8*16�ַ���
    //ring_size
    site.y += char_H;
    memset(str_buf,0,30);
    sprintf((char *)str_buf,"r%d l%d",ring_right_flag,ring_left_flag);//��Ч�б�־��ʾ  Բ���ڵĹյ�λ��
    LCD_str(site,str_buf,FCOLOUR,BCOLOUR);   //��ʾ8*16�ַ���
    
    
//     sprintf((char *)str_buf,"vl %d",valid_line);//��Ч�б�־��ʾ
//     LCD_str(site,str_buf,FCOLOUR,BCOLOUR);   //��ʾ8*16�ַ���
//     //w
//     site.y += char_H;
//     memset(str_buf,0,30);
//     sprintf((char *)str_buf,"gz %d",gl_zhidao_count);//��Ч�б�־��ʾ  Բ���ڵĹյ�λ��
//     LCD_str(site,str_buf,FCOLOUR,BCOLOUR);   //��ʾ8*16�ַ���
//     //ring_size
//     site.y += char_H;
//     memset(str_buf,0,30);
//     sprintf((char *)str_buf,"gd %d",guai_dian_count);//��Ч�б�־��ʾ  Բ���ڵĹյ�λ��
//     LCD_str(site,str_buf,FCOLOUR,BCOLOUR);   //��ʾ8*16�ַ���
}

void open_img_ui()
{
   
    Site_t site     = {0, 0}; 
    Size_t size;
    size.H= CAMERA_H;
    size.W = CAMERA_W;
    Size_t imgsize  = {CAMERA_W, CAMERA_H};             //ͼ���С
    my_debug_flag=1;
    while(key_check(KEY_L)!=KEY_DOWN&&key_check(KEY_B)!=KEY_DOWN)
    {
        camera_get_img();                                   //����ͷ��ȡͼ�� 
        LCD_rectangle(site, size, BCOLOUR);     //��ʼ������
        LCD_Img_Binary_Z(site, size, imgbuff, imgsize);//��ʾ��ֵ��ͼ��(������)
        draw_mark_line();//��ͼ�����滮���ߣ����ڿ�ͼ��
        ctrl_main();//����������
       //show_ring_data();
               
    }
    UI_state=MAIN_UI;
    my_debug_flag=0;
}
void renew_UI()
{//
//   UI_state=OPEN_IMG_UI;//�����õ�   ���� �ܵ�ʱ��ע�͵�
 // UI_state=MAIN_UI;
    while(UI_state<5)
    {
        switch(UI_state)
        {
        case MAIN_UI:main_ui();break;
        case SET_SPEED_UI:set_speed_ui();break;
        case SET_IMG_UI:set_img_ui();break;
        case OPEN_IMG_UI:open_img_ui();break;
        }
    }
    LCD_clear(RED);
}
uint8 show_info_flag=0; 
void show_run_info()
{
    uint8 str_buf[STR_BUF_LEN];
    uint8 char_H=16;
    Site_t site = {3,5};
    if(key_check(KEY_L)==KEY_DOWN)
        show_info_flag=0;
    if(key_check(KEY_R)==KEY_DOWN)
        show_info_flag=1;
    if(show_info_flag)
    {
        site.y = 5;
        site.x = 3;
        LCD_clear(BCOLOUR);
        memset(str_buf,0,STR_BUF_LEN);
        sprintf((char *)str_buf,"sl %d %d",pidl.setPoint,pidl.pluse);
        LCD_str(site,str_buf,FCOLOUR,BCOLOUR);   //��ʾ8*16�ַ���
        site.y += char_H;
        
        memset(str_buf,0,STR_BUF_LEN);
        sprintf((char *)str_buf,"start%d",stop_car_flag);
        LCD_str(site,str_buf,FCOLOUR,BCOLOUR);   //��ʾ8*16�ַ���
        site.y += char_H;
        
        memset(str_buf,0,STR_BUF_LEN);
        sprintf((char *)str_buf,"Vsp %d",Vspeed);
        LCD_str(site,str_buf,FCOLOUR,BCOLOUR);   //��ʾ8*16�ַ���
        site.y += char_H;
        
        memset(str_buf,0,STR_BUF_LEN);
        sprintf((char *)str_buf,"e %d",even_diff);
        LCD_str(site,str_buf,FCOLOUR,BCOLOUR);   //��ʾ8*16�ַ���
        site.y += char_H;
        
        memset(str_buf,0,STR_BUF_LEN);
        //sprintf((char *)str_buf,"zhi_flag %d",zhidao_count_flag);
        sprintf((char *)str_buf,"out %d,out");//b=%lf,a,b
        LCD_str(site,str_buf,FCOLOUR,BCOLOUR);   //��ʾ8*16�ַ���
        site.y += char_H;
        
        memset(str_buf,0,STR_BUF_LEN);//normal_ring_flag
       // sprintf((char *)str_buf,"nor=%d val=%d",out_ring_ok_flag,val);//Բ����ǣ��ϰ����  start_fill_line_flag  in_ring_flag
         sprintf((char *)str_buf,"r%d l%d",ring_right_flag,ring_left_flag);//��Ч�б�־��ʾ  Բ���ڵĹյ�λ��
        LCD_str(site,str_buf,FCOLOUR,BCOLOUR);   //��ʾ8*16�ַ���
        site.y += char_H;
        
        memset(str_buf,0,STR_BUF_LEN);
        sprintf((char *)str_buf,"vl %d sz=%d za=%d",valid_line,Shi_zi_flag,gl_zhangai_flag);
        LCD_str(site,str_buf,FCOLOUR,BCOLOUR);   //��ʾ8*16�ַ���
    }
}