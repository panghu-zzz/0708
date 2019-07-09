/*!*****************************************************************
 *  �ڹ�Ժ ��˼���� ���ڵ��γ�������
 *  ���� ��ͨ�������������ò�����֧���Զ�������ĵ��ι�����ʷ��¼
*          ���Զ��屣����ԺõĲ���������������ṹ�壩����֧��ɾ��
 *  ʹ�÷�ʽ ��ͨ��ָ����Ʋ�����ָ��������е�ָ����������
 *���� ������Э���˼�����Ŷ�--л����
********************************************************************
  ����ʹ�þ��飺�տ�ʼʹ�ô������ǽ����жϾͳ������ˣ����������Ϻ�ʵ���ҵ�
�������취�� K60���жϱ�־λ�������ֶ���Ӳ���Լ��������־λ��*��*��*��
ֻ�в���˳����˵�����²Ż��Զ���������򽫳���ȥ�жϡ�
  ��ȷ����˳��һ������Ҫ���ȶ�ȡ���λ if(UART_S1_REG(UARTN[uratn]) & UART_S1_RDRF_MASK) 
               Ȼ���ٶ�ȡ�������� uart_querychar(UART_PORT,(uint8 *)cmd_buf);
               ������������жϱ�־λ��
  *ע*��*,��û���أ����� �ϱ���ֻ�ǽ���һ���ַ�������������������д�ģ�����
    һ�㲻����������ȥ�жϵ����⡣��Ϊ��ֱ�����жϵ��ò�ѯ�����ַ����ĺ���������
      �����ȥ�жϺ�������Ϊֻ�е�һ���ֽ�����ʱ���жϻ�ɹ��������ߵ��ֽ��ж�
        ��û�����������취�ǽ���ײ�Ľ���һ���ֽڵĺ�����Ӳ�ѯ���λ��һ�仰
        char uart_querychar (UARTn_e uratn, char *ch)//ɽ���
        {
            if( UART_RCFIFO_REG(UARTN[uratn]) )         //��ѯ�Ƿ���ܵ�����
            {
                UART_S1_REG(UARTN[uratn]) &UART_S1_RDRF_MASK;//�Һ�ӵģ������ܱ�֤
                                                            //��ȷ�Ķ�ȡ˳�����ж���ʹ���ܳɹ�������λ
                *ch  =   UART_D_REG(UARTN[uratn]);      //���ܵ�8λ������
                return  1;                              //���� 1 ��ʾ���ճɹ�
            }

            *ch = 0;                                    //���ղ�����Ӧ������˽�����
            return 0;                                   //����0��ʾ����ʧ��
        }  
********************************************************************/
#include  "info_deal_save.h"
#include <stdio.h>
#include "speed_new.h"
#include "control.h"
#include "my_cfg.h"

uint8_t cmd_buf[30];
uint8_t rec_cmd_flag=0;
ParValue myPar_num;
/*!
 *  @brief      д�����ݻ������� flashָ����ַ
 *  @param      addr         Ҫд��ĵ�ַ
*  @param      cnt      д����ֽ���
 *  @param      buf             ��Ҫд������ݻ������׵�ַ
 *  @return     ִ�н��(1�ɹ���0ʧ��)
 *  @since      v5.0
 *  Sample usage:           uint32 buff[10];
                            flash_write_buf(127,0,sizeof(buff),buff);        //����127,ƫ�Ƶ�ַΪ0,д������Դ��ַ:buff,��Ŀsizeof(buff)
 *****************�ĵ�ɽ���****************
*/
__RAMFUNC uint8 flash_write_data(uint32  addr, uint16 cnt, uint8 *buf)
{
    uint32  size;
    uint32  data;
    // ����д������
    FCMD = FLASH_WRITE_CMD;

    for(size = 0; size < cnt; size += FLASH_ALIGN )
    {

        // ����Ŀ���ַ
        FADDR2 = ((Dtype *)&addr)->B[2];
        FADDR1 = ((Dtype *)&addr)->B[1];
        FADDR0 = ((Dtype *)&addr)->B[0];

        // ���� ��32λ����
        data =  *(uint32 *)buf;

        FDATA0 = ((Dtype *)&data)->B[3];                    // ����д������
        FDATA1 = ((Dtype *)&data)->B[2];
        FDATA2 = ((Dtype *)&data)->B[1];
        FDATA3 = ((Dtype *)&data)->B[0];

#if defined(MK60F15)
        // ���� ��32λ����
        data = *(uint32 *)(buf+4);

        FDATA4 = ((Dtype *)&data)->B[3];                    // ����д������
        FDATA5 = ((Dtype *)&data)->B[2];
        FDATA6 = ((Dtype *)&data)->B[1];
        FDATA7 = ((Dtype *)&data)->B[0];
#endif

        if(flash_cmd() == 0)
        {
            return 0;
        }

        addr += FLASH_ALIGN;
        buf += FLASH_ALIGN;
    }
    return 1;  //�ɹ�ִ��
}
//*****************����������ĵĺ���****************************
//��ʼ��ȫ�ֽṹ�������ֵ��Ȼ�󱣴浽flash�У�
//ֻ�е�һ��flash��û�����ݵ�ʱ����ã��Ժ������س����ʱ��Ͳ�Ҫ�����ˣ�
//��Ȼ�Ḳ������
void init_myPar_num()
{
    ParValue pv;
    myPar_num.SET_FLAG=0;
    myPar_num.ZHIDAO_SPEED=zhidao_speed;
    myPar_num.CD_SPEED[0]=CD_speed;
    myPar_num.CD_SPEED[1]=0;
    myPar_num.CD_SPEED[2]=0;
    myPar_num.CD_SPEED[3]=0;
    myPar_num.CD_SPEED[4]=0;
    myPar_num.CD_SPEED[5]=0;
    myPar_num.CD_SPEED[6]=0;
    myPar_num.CD_SPEED[7]=0;
    myPar_num.DJ_KP[0] = 1;
    myPar_num.DJ_KP[1] = 1;
    myPar_num.DJ_KP[2] = 1;
    myPar_num.DJ_KP[3] = 1;
    myPar_num.DJ_KP[4] = 1;
    myPar_num.DJ_KP[5] = 1;
    myPar_num.KP = speed_P;
    myPar_num.KD = speed_D;
    myPar_num.KI = speed_I;
    flash_erase_sector(Par_DEBUG_ADDR);                   //��������
    flash_write_data(Par_Debug_head_addr,PAR_STRUCT_SIZE,(uint8 *)&myPar_num);
    get_debug_Par_from_FLASH(&pv,0);//����һ��д�ĶԲ��ԣ��ڵ���ʱ������
}
/*******************************************************************************
�������ƣ�InitPar
��������: ��ʼ����������,���ṹ��Ĳ���ֵ��ֵ��ȫ�ֱ���
��������
***��������Լ��Ľṹ�����͸�***
*******************************************************************************/
void InitPar()
{
     //evaluate_all(myPar_num);
     myPar_num.SET_FLAG=0;
     zhidao_speed=(uint8)myPar_num.ZHIDAO_SPEED;
     CD_speed=(uint8)myPar_num.CD_SPEED[0];
     speed_P = myPar_num.KP;
     speed_D = myPar_num.KD;
     speed_I = myPar_num.KI;
}

//���� ����������
//ע�� ����Ϊһ�����̫�����ղ�ȫ�����Զ��ȡ����
void dir_cmd()
{
    static uint8 flag=0;
    if(flag==0)
    {
        flag++;
        printf("-----\n");
        printf("lc->get_dir_cmd\n");//lc �õ������
        printf("ldx->get debug[x]\n");//ldx ��ȡ��x�����Բ���
        printf("lsx->get save_Par[x]\n");//lsx ��ȡ��x���������
        printf("gx->debug[x]=save[x]\n");//����x�������õĲ������ص���ǰ�ĵ��Ա���
        printf("Dx->delete save[x]\n");  //ɾ������õĲ����е�x��������
    }
    else if(flag==1)
    {
        flag=0;
        printf("z-zhidao_speed\n");
        printf("cx-CD_speed[x] \n");
        printf("b-biLi \n");
        printf("p-Kp \n");
        printf("d-Kd \n");
        printf("Px-duoji_Kp[x] \n");
    }
}
//���� �������Ӧ�Ĳ�������Ҫ�ĳ��Լ��ṹ��Ĳ���
void printf_Par(ParValue pv)
{
//    static uint8 flag=0;
//    uint8 i;
//    if(flag==0)
//    {
//          flag++;
//          printf("------\n");
//          printf("zhidao_speed =%d\n",pv.ZHIDAO_SPEED);
//          printf("CD_speed =%d\n",pv.CD_SPEED[0]);
//          printf("biLi =%d.%d\n",(int)pv.BILI,((int)(pv.BILI*100))%100);   
//          printf("Kp =%d\n",pv.KP);
//          printf("Kd =%d\n",pv.KD);
//    }
//    else if(flag==1)
//    {
//        flag++;
//        printf("duoji_Kp =%d.%d\n",(int)pv.DJ_KP[0],((int)(pv.DJ_KP[0]*100))%100);
//        printf("duoji_Kp1 =%d.%d\n",(int)pv.DJ_KP[1],((int)(pv.DJ_KP[1]*100))%100);
//        printf("duoji_Kp2 =%d.%d\n",(int)pv.DJ_KP[2],((int)(pv.DJ_KP[2]*100))%100);
//        printf("duoji_Kp3 =%d.%d\n",(int)pv.DJ_KP[3],((int)(pv.DJ_KP[3]*100))%100);
//        printf("duoji_Kp4 =%d.%d\n",(int)pv.DJ_KP[4],((int)(pv.DJ_KP[4]*100))%100);
//        printf("duoji_Kp5 =%d.%d\n",(int)pv.DJ_KP[5],((int)(pv.DJ_KP[5]*100))%100);
//    }
//    else if(flag==2)
//    {
//        flag=0;
//        printf("CD_speed[1] =%d\n",pv.CD_SPEED[1]);
//        printf("CD_speed[2] =%d\n",pv.CD_SPEED[2]);
//        printf("CD_speed[3] =%d\n",pv.CD_SPEED[3]);
//        printf("CD_speed[4] =%d\n",pv.CD_SPEED[4]);
//        printf("CD_speed[5] =%d\n",pv.CD_SPEED[5]);
//        printf("CD_speed[6] =%d\n",pv.CD_SPEED[6]);
//        printf("CD_speed[7] =%d\n",pv.CD_SPEED[7]);
//    }
//   
}
int max_speed=0;
int min_speed=0;
//���ڵ��κ�ֵ��P,I,D,max_speed,mix_speed��
void evaluate_all(ParValue pv)
{
	//�����õĲ������и�ֵ��ȫ�ֱ���
//	speed_D  =pv.KD;
//	speed_I  =pv.KI;
//	speed_P  =pv.KP;
	zhidao_speed=pv.ZHIDAO_SPEED;//����ٶ�
	CD_speed=pv.CD_SPEED[0];//��С�ٶ�
	kp_val=myPar_num.DJ_KP[0];//��׼P
	diff_speed=(float)myPar_num.BILI;//���ٱ��������ͣ�
	PID_init(); //
}
//�����������η���
void printf_val(ParValue pv)
{
// printf("max_speed =%d\n",pv.ZHIDAO_SPEED);
// printf("min_speed =%d\n",pv.CD_SPEED[0]);
// printf("Kp =%d\n",pv.KP);
// printf("Ki =%lf\n",pv.KI);
// printf("Kd =%d\n",pv.KD);

}
//float pintf_f(float val)
//{
//    int a,b;
//    float ret=0.0;
//    int shi_fen,bai_fen,qian_fen;
//    a=(int)(val*1000);//0.321*1000=321
//    shi_fen=a/100;//3
//    bai_fen=(a%100)/10;//2
//    qian_fen=(a%100)%10;//1
//    ret=(float)shi_fen*0.1+(float)bai_fen*0.01+(float)qian_fen*0.001;
//    
//    return ret;
//}
//���� �����ݴ����յ������ݽ�����ִ������
//���� ��cmd ���ھ��յ����ַ���
//ע�⣺������ֵ�ĵط���Ҫ�Լ���
void usart_Par_cmd(uint8 *cmd)
{
    float par=str2float(cmd);//����ǰ��û�õ��ַ������ַ���ת��������
    switch(cmd[0])
    {
        //*******������ָ����Բ���**********
        case 'l': if(cmd[1]=='c') printf_val(myPar_num);//dir_cmd();//lc ���������
                   else if(cmd[1]=='d')get_debug_and_printf_Par((char)par);//ldx ��ȡ���Ա��е�x������ ����ld5 
                   else if(cmd[1]=='s')get_save_and_printf_Par((char)par); //lsx ��ȡ�����õĲ�����ĵ�x��
                      return;
        case 's':save_Save_par();return;//������ڵĲ��������ˣ���sָ����ڵ��ԵĲ���������������Ҫ������ٸ��ɸ�д��MAX_Par_SAVE_COUNT
        case 'g':get_savePar_to_debugPar((char)par);return;//gx �����úõĲ������е�x���������ڵ��ԵĲ���
        case 'D':del_save_par((char)par);return;    //Dxɾ������ĺõĵ�x�����ݱ�
        case 'r':run_start_flag=~run_start_flag;return;    //Dxɾ������ĺõĵ�x�����ݱ�
        case 'I':if(my_debug_flag) my_debug_flag=0;
                  else my_debug_flag=1;return;   
        //********�±��Ƕ��Լ��Ĳ�����ֵ����ĸ����֪����*******************************                  
        case 'y':if(cmd[1]=='1') //���ٵ�
                  {
                    myPar_num.ZHIDAO_SPEED=136;myPar_num.CD_SPEED[0]=100;
                    printf("gaosu");
                  }
                  else if(cmd[1]=='2')//ɲ��
                  {
                    myPar_num.ZHIDAO_SPEED=0;myPar_num.CD_SPEED[0]=0;
                    printf("stop");                  
                  } 
                  else if(cmd[1]=='3')//����
                  {
                    myPar_num.ZHIDAO_SPEED=80;myPar_num.CD_SPEED[0]=60;                          
                  }  
                  else if(cmd[1]=='4')//����
                  {
                    myPar_num.ZHIDAO_SPEED=40;myPar_num.CD_SPEED[0]=30;                          
                  }  
                  else if(cmd[1]=='5')//������
                  {
                    myPar_num.ZHIDAO_SPEED=8;myPar_num.CD_SPEED[0]=3;                          
                  }  
                  break;
                  break;                 
                  
        case 'z':if(cmd[1]=='a') //����ٶ�
                    myPar_num.ZHIDAO_SPEED++;//ֱ���ٶȼ�һ
                  else if(cmd[1]=='d')  myPar_num.ZHIDAO_SPEED--;
                  else  myPar_num.ZHIDAO_SPEED=(char)par  ;
                  if(myPar_num.ZHIDAO_SPEED>240) myPar_num.ZHIDAO_SPEED=240;
                  printf("max_speed=%d",myPar_num.ZHIDAO_SPEED);
                  break;
        case 'c':par=str2float(cmd+2); //��С�ٶ�
                  if(cmd[2]=='a') myPar_num.CD_SPEED[cmd[1]-'0']++;//ֱ���ٶȼ�һ
                  else if(cmd[2]=='d')  myPar_num.CD_SPEED[cmd[1]-'0']--;
                  else  myPar_num.CD_SPEED[cmd[1]-'0']= (char)par  ;
                  printf("min_speed=%d",myPar_num.CD_SPEED[cmd[1]-'0']);
                  break;//ȫ���ٶ�
        case 't':if(cmd[1]=='a') myPar_num.BILI+=1;//��������
                  else if(cmd[1]=='d')  myPar_num.BILI-=1;
                  else  myPar_num.BILI=(char)par;
                  printf("biLi =%d\n",myPar_num.BILI); break;//ȫ�ֱ���
//                  printf("biLi =%d.%d\n",(int)myPar_num.BILI,((int)(myPar_num.BILI*100))%100); break;//ȫ�ֱ���
        case 'P':par=str2float(cmd+2);          //Px ע���Ǵ�д�ģ�����ĵ�x������P
                 if(cmd[2]=='a')   myPar_num.DJ_KP[cmd[1]-'0']+=1;
                 else if(cmd[2]=='d')   myPar_num.DJ_KP[cmd[1]-'0']-=1;
                 else myPar_num.DJ_KP[cmd[1]-'0']=par;
                printf("DJ_KP[%d] =%d.%d\n",cmd[1]-'0',(int)myPar_num.DJ_KP[cmd[1]-'0'],((int)(myPar_num.DJ_KP[cmd[1]-'0']*100))%100); break;
                
        case 'i': if(cmd[1]=='a') myPar_num.KI+=0.1;//���I
                  else if(cmd[1]=='d')  myPar_num.KI-=0.1;
                  
                  else  myPar_num.KI=par;
                 // printf("p=%.3f",myPar_num.KI);  
                  printf("i =%d.%d\n",(int)myPar_num.KI,((int)(myPar_num.KI*100))%100); 
                  break;//ȫ�ֱ���
        case 'p': if(cmd[1]=='a') myPar_num.KP+=0.1;//���P
                  else if(cmd[1]=='d')  myPar_num.KP-=0.1;
                  else  myPar_num.KP=(char)par;
//                  printf("p=%d",myPar_num.KP);
                  printf("p =%d.%d\n",(int)myPar_num.KP,((int)(myPar_num.KP*100))%100); 
                  break;
        case 'd': if(cmd[1]=='a') myPar_num.KD+=0.1;//���D
                  else if(cmd[1]=='d')  myPar_num.KD-=0.1;
                  else  myPar_num.KD=(char)par;
//                  printf("d=%d",myPar_num.KD);
                  printf("d =%d.%d\n",(int)myPar_num.KD,((int)(myPar_num.KD*100))%100); 
                  break;
        default :printf("cmd err\n"); return;
    }
    save_Bebug_par();//��������޸�
}
//**********�±ߵĲ��ø�***********************************************************

//���� ����flash�õ�ָ����ַ�Ĳ�����
//���� pv�ṹ�����ָ�� num-Ҫ�ҵĵڼ����ṹ��
void get_debug_Par_from_FLASH(PAR_STRUCT_TYPE_NAME *pv,uint8 num)
{
    if(num>=MAX_Par_DEBUG_COUNT)
    {
        printf("cmd err\n");
        return;
    }
    memcpy((char *)pv,(char *)(Par_Debug_head_addr+PAR_STRUCT_SIZE*num),sizeof(PAR_STRUCT_TYPE_NAME));
}
//���� ����flash�õ�ָ����ַ�Ĳ�����
//���� pv�ṹ�����ָ�� num-Ҫ�ҵĵڼ����ṹ��
void get_save_Par_from_FLASH(PAR_STRUCT_TYPE_NAME *pv,uint8 num)
{
    if(num>=MAX_Par_SAVE_COUNT)
    {
        printf("cmd err\n");
        return;
    }
    memcpy((char *)pv,(char *)(Par_Save_head_addr+PAR_STRUCT_SIZE*num),sizeof(PAR_STRUCT_TYPE_NAME));
}
//���� ����flash�õ�ָ����ַ�Ĳ����������
//���� ��num-Ҫ�ҵĵڼ����ṹ��
void get_debug_and_printf_Par(uint8 num)
{
     PAR_STRUCT_TYPE_NAME pv;
     if(num<MAX_Par_DEBUG_COUNT)
     {
       get_debug_Par_from_FLASH(&pv,num);
       printf_Par(pv);
     }
     else
     {
        printf("cmd err\n");
     }
}
//���� ����flash�õ�ָ����ַ�Ĳ����������
//���� ��num-Ҫ�ҵĵڼ����ṹ��
void get_save_and_printf_Par(uint8 num)
{
     PAR_STRUCT_TYPE_NAME pv;
     if(num<MAX_Par_SAVE_COUNT)
     {
       get_save_Par_from_FLASH(&pv,num);
       printf_Par(pv);
     }
     else
     {
        printf("cmd err\n");
     }
}
void save_Bebug_par()
{
    uint8 flag=1;
    char pv_data_buf[PAR_STRUCT_SIZE*(MAX_Par_DEBUG_COUNT)];
    memcpy(pv_data_buf+PAR_STRUCT_SIZE,(uint8*)Par_Debug_head_addr,sizeof(pv_data_buf)-PAR_STRUCT_SIZE);
    memcpy(pv_data_buf,&PAR_STRUCT_NAME,PAR_STRUCT_SIZE);
    flash_erase_sector(Par_DEBUG_ADDR);                     //��������
    flash_write_data(Par_Debug_head_addr,sizeof(pv_data_buf),(unsigned char *)pv_data_buf);
    if(flag)
    {
        get_debug_Par_from_FLASH(&PAR_STRUCT_NAME,0);
        InitPar();
//        printf_Par(PAR_STRUCT_NAME);
        printf("Save OK \n");
    }
    else
    {
        printf("Save ERR \n");
    }
}
void save_Save_par()
{
    uint8 flag=1;
    char pv_data_buf[PAR_STRUCT_SIZE*(MAX_Par_SAVE_COUNT)];
    memcpy(pv_data_buf+PAR_STRUCT_SIZE,(uint8*)Par_Save_head_addr,sizeof(pv_data_buf)-PAR_STRUCT_SIZE);
    memcpy(pv_data_buf,&PAR_STRUCT_NAME,PAR_STRUCT_SIZE);
    flash_erase_sector(Par_SAVE_ADDR);                     //��������
    flag=flash_write_data(Par_Save_head_addr,sizeof(pv_data_buf),(unsigned char *)pv_data_buf);
    if(flag)
    {
        get_save_Par_from_FLASH(&PAR_STRUCT_NAME,0);
        printf_Par(PAR_STRUCT_NAME);
        printf("Save OK \n");
    }
    else
    {
        printf("Save ERR \n");
    }
}
//���� ��ɾ�����ԺõĲ������е�num��������
void del_save_par(uint8 num)
{
    uint8 flag=1;
    char pv_data_buf[PAR_STRUCT_SIZE*(MAX_Par_SAVE_COUNT)];
    memcpy(pv_data_buf,(uint8*)Par_Save_head_addr,PAR_STRUCT_SIZE*num);
    memcpy(pv_data_buf+PAR_STRUCT_SIZE*num,(uint8*)Par_Save_head_addr+PAR_STRUCT_SIZE*(num+1),PAR_STRUCT_SIZE*(MAX_Par_SAVE_COUNT-num-1));
    flash_erase_sector(Par_SAVE_ADDR);                     //��������
    flag=flash_write_data(Par_Save_head_addr,sizeof(pv_data_buf),(unsigned char *)pv_data_buf);
    if(flag)
    {
        get_save_Par_from_FLASH(&PAR_STRUCT_NAME,0);
        printf_Par(PAR_STRUCT_NAME);
        printf("delete OK \n");
    }
    else
    {
        printf("delete ERR \n");
    }
}
//���� �������õĲ�����num�����ǵ���ǰ�ĵ��Բ���
void get_savePar_to_debugPar(uint8 num)
{
    get_save_Par_from_FLASH(&PAR_STRUCT_NAME,num);
    save_Bebug_par();
}
//���� ����ȡ���յ�����ֵ
float str2float(uint8 *cmd)
{
    float f=0,i=10;
    while(*cmd<'0'||*cmd>'9')//ȥ�������ֲ���
    {
        if(*cmd==0)
          break;
        cmd++;
    }
    while(*cmd!=0)//ת����������
    {
        if(*cmd<'0'||*cmd>'9')
            break;
        f=f*10+(*cmd-'0');
        cmd++;
    }
    if(*cmd=='.')
    {   
        cmd++;
        while(*cmd>='0'&&*cmd<='9')
        {
            f+=((*cmd-'0')/(i));
            i*=10;
            cmd++;
        }
    }
    return f;    
}

//void init_val_myPar_num()
//{
//  myPar_num.ZHIDAO_SPEED=145;//ȫ������ٶ�
//  myPar_num.CD_SPEED[0]=110;//ȫ����С�ٶ�
//  
//  myPar_num.KP=72;//���P
//  myPar_num.KI=239;//���I
//  myPar_num.KD=15;//���D
//  
//  myPar_num.BILI=14;//��������  
//  
//}
void init_Par_from_FLASH()
{
    flash_init(); //�õ�Ұ��Ŀ⺯��
    //init_myPar_num(); //��ʼ��ȫ�ֽṹ�������ֵ��Ȼ�󱣴浽flash�У�
                        //ֻ�е�һ��flash��û�����ݵ�ʱ����ã��Ժ������س����ʱ��Ͳ�Ҫ�����ˣ�
                        //��Ȼ�Ḳ������  
   // init_val_myPar_num();
    get_debug_Par_from_FLASH(&PAR_STRUCT_NAME,0);
    //InitPar();
    printf_Par(PAR_STRUCT_NAME);
}
void cmd_deal()
{
       if(rec_cmd_flag==1)
      {
          rec_cmd_flag=0;
          usart_Par_cmd(cmd_buf);
          memset(&cmd_buf,0,sizeof(cmd_buf)); 
      }
      evaluate_all(myPar_num);
}
