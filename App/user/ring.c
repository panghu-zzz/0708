#include "ring.h"
#include  "common.h"
#include "include.h"
#include "path.h"
#include "fun.h"
#include "control.h"

int   MidPri         = 40;
int   LastLine       = 0;
float AvaliableLines = 0;
int   LeftLose       = 0;
int   RightLose      = 0; 
int   AllLose        = 0;
int   LeftLoseStart  = 0;//��¼��߶��ߵĿ�ʼ��
int   RightLoseStart = 0;//��¼�ұ߱߶��ߵĿ�ʼ��
int   WhiteStart     = 0;


/*********define for GetBlackEndParam**********/
int BlackEndMR      = 0;
int BlackEndML      = 0;
int BlackEndLL      = 0;
int BlackEndRR      = 0;
int BlackEndL       = 0;
int BlackEndM       = 0;
int BlackEndR       = 0;
int BlackEndMaxMax  = 0;
int BlackEndMax     = 0;
int DropRow         = 0;

/*********define for FindInflectionPoint()**********/

int RightInflectionPointRow=0;
int RightInflectionPointCol=0;
int LeftInflectionPointRow=0;
int LeftInflectionPointCol=0;
unsigned char RightInflectionPointFlag=0;
unsigned char LeftInflectionPointFlag=0;
unsigned char LeftInflectionPointSecondFlag=0;
unsigned char RightInflectionPointSecondFlag=0;
unsigned char LoopFlag=0;
unsigned char LoopRightOkFlag=0;
unsigned char LoopLeftOkFlag=0;
unsigned int StartRow=0;
unsigned int StartCol=0;
unsigned char MilldleBlack=0;
unsigned int LoopTop=0;
unsigned int LoopRightBorderLose=0;
unsigned int LoopLeftBorderLose=0;
int LoopBorttomFlag=0;
int LoopBorttomRow=0;
int LoopMilldleRow=0;
unsigned int LoopMilldleFlag=0;
unsigned int LoopTopRow=0;
unsigned int LoopLeft=0;
unsigned int MilldlePonit=0;
unsigned int LoopRight=0;
unsigned int LoopRightR=0;
unsigned int LoopLeftL=0;
int BigLoopLeftUp[60];
int BigLoopRightUp[60];
int BigLooptUp[80];

unsigned char LoopRightControlFlag=0;

unsigned char  openLoopFlag=0;//�Ƿ���Բ����־λ

unsigned int  process_flag=0;

unsigned int  enter_loop=0;//���뻷·
unsigned int  out_loop=0;//����·

int get_img_point(uint16 h,uint16 w);//����
//�����ҹյ㣬ʶ��
//һֱ��Ҫ����
int left_diuxian_num=0;
int right_diuxian_num=0;
int xianzhi_daxiao(int a[],int num)
{
    int i=0;
    int ret=0;
    for(;i<6;i++)
    {
        if(a[i]!=0&&a[i]>=num)
        {
            ret++;
        }    
    }
    return ret;
}
//void FindInflectionPoint()//���� ������
//{
//	char i=0;
//	int distance=0;
//	int g_point_flag=0;
//    int num_line_i=0;
//    int left_total_none_num=0,right_total_none_num=0;
//    int project_i=0;
//    int LoopBorttomRow_arrary[10]={0};
//	//��������
//	RightInflectionPointRow=0;//�ұߵĹյ��������
//	RightInflectionPointCol=0;//�ұߵĹյ��������
//	LeftInflectionPointRow=0;//��ߵĹյ��������
//	LeftInflectionPointCol=0;//��ߵĹյ��������
//	RightInflectionPointFlag=0;//�ҵ���յ�ı�־λ
//	LeftInflectionPointFlag=0;//�ҵ��ҹյ�ı�־λ
//
//	StartRow=0;//�ӵײ�����ɨ��Բ���м�ĺ�Բ����ʼ��
//	StartCol=0;//�ӵײ�����ɨ��Բ���м�ĺ�Բ����ʼ��
//
//	LoopBorttomFlag=0;//�ҵ�Բ���м�ĺ�Բ�ĵײ��ı�־λ
//	LoopBorttomRow=0;//Բ���м�ĺ�Բ�ĵײ���������
//	LoopMilldleRow=0;//Բ���м�ĺ�Բ�Ķ�����������
//	LoopMilldleFlag=0;//�ҵ�Բ���м�ĺ�Բ�Ķ����ı�־λ
//	LoopTopRow=0;//�ҵ��⻷�Ķ���
//	MilldleBlack=0;//Բ���м�ĺ�Բ�Ķ�����Բ���м�ĺ�Բ�ĵײ����������ƽ��ֵ
//	LoopFlag=0;//�ҵ������������͵ı�־
//   
//
//	//��յ�
//	for(i=3;i<=32;i++) //35 30
//	{
//		if((left_pos[i]!=40)&&(left_pos[i]!=79)) 
//		{     
//			if((left_pos[i]-left_pos[i+1]<0))//�ҵ��յ�
//			{
//				LeftInflectionPointRow=i;//��¼�ùյ����           
//				LeftInflectionPointCol=left_pos[i];//��¼�ùյ����           
//				LeftInflectionPointFlag=1;//����ҵ���յ�              
//				break;//�ҵ��˳�                                  
//			}
//		}                                                                                                                                                                                                                                            
//	} 
//
//	//�ҹյ� 
//	for(i=3;i<=32;i++)//����ɨ��̫Զ�����������//35 30
//	{
//		if((right_pos[i]!=40)&&(right_pos[i]!=1)) //�������в�����
//		{     
//			if((right_pos[i]-right_pos[i+1]>0))//�ҵ��ұ����йյ�
//			{         
//				RightInflectionPointRow=i;//��¼�յ����
//				RightInflectionPointCol=right_pos[i];//��¼�յ����
//				RightInflectionPointFlag=1;//����ҵ���յ�              
//				break;//�ҵ��˳�
//			}      
//		} 
//	}
//	//��ʮ���п������гɻ�· ���е�ʱ�����ҹյ������ �����2���� ��69����
//	//
//	//����ͬʱ�ҵ������յ㣬��ʼʶ��·(������Կ���һ�£����ֻ�ҵ�һ���յ��������������ܸ�����ʶ�𵽻�·)
//
//	if(LeftInflectionPointFlag&&RightInflectionPointFlag)//ͬʱ�ҵ������յ�
//	{
//		StartCol=(unsigned int)((LeftInflectionPointCol+RightInflectionPointCol)/2);// ȡ���ҹյ��������ƽ��ֵ    
//		StartRow=(unsigned int)((LeftInflectionPointRow+RightInflectionPointRow)/2);//ȡ���ҹյ���������ƽ��ֵ    
////        if(StartRow<35)
////        {
//            g_point_flag=1;
////        }
////        else 
////        {
////            g_point_flag=0;
////            shizi_wait=0;
////        }	
//
//		for(i=StartRow;i<60&&g_point_flag;i++)//�̶�һ�У�StartCol���ӿ�ʼ��������ɨ�裬Ѱ�һ�·�м��Բ���������ҵ�һ���׵��ڵ����䣬Ȼ����кڵ�Ȼ�󵽰ף�
//		{
//			if(!get_img_point(i,StartCol)&&get_img_point(i+1,StartCol))//������λ��
//			{
//				LoopBorttomRow=i;//��¼��һ��������� ����ʱ�ҵ��˰ױ�ڵ�λ�ò���¼��
//				distance=LoopBorttomRow-StartRow;
//				if(distance>8&&distance<35&&LoopBorttomRow<50)
//				{
//					LoopBorttomFlag=1;//��λ��־   ;           
//					break; //�����Ѿ��ҵ������յ��һ�������
//				}
//				else if(LoopBorttomRow>52&&StartRow<6)
//				{
//				//           Shi_zi_flag=1;
//				//           break; 
//				}
//
//			}     
//		} 
//		//�����ҵ��� �յ㣨�����������ͻ���λ�ã� �׺� �ڰ�����������λ�� ���Ҽ�¼��
//		/********************************************************************/    
//		if(LoopBorttomFlag)//����֮ǰ�Ѿ��ҵ�Բ�������������--�ж�������ȵ�ͻ��
//		{//
//			if(ABS(left_pos[StartRow+2]-right_pos[StartRow+2])-35>=4//4,8������������Ҫ����
//			&&ABS(left_pos[StartRow+4]-right_pos[StartRow+4])-40>=8)//�������ͻ��
//			{
//				LoopFlag=1;//����������־λ��
//                loop_guaidian_flag=1;//˵���ҵ������յ�
//				LoopRightControlFlag=1;//�õ���·��־λ�͸�����
//				enter_loop=1;
//			}                           
//		}
//	}
//    else //���û���ҵ��յ㣬˵���յ���ʧ�������ǵײ����ߣ�ǰ��һ�ź�
//    {
//        for(num_line_i=0;num_line_i<58;num_line_i++)
//        {
//            if(left_pos[num_line_i]==79)
//            {
//                left_total_none_num=left_total_none_num+num_line_i;
//                project_i++;
//            }
//            else
//            {
//                if(project_i>3)
//                {
//                    left_diuxian_num=left_total_none_num/project_i;
//                    break;
//                }
//            }            
//        }
//         project_i=0;
//        /******************************************************/
//        for(num_line_i=0;num_line_i<58;num_line_i++)
//        {
//            if(right_pos[num_line_i]==1)
//            {
//                right_total_none_num=right_total_none_num+num_line_i;
//                project_i++;
//            }
//            else
//            {
//                if(project_i>3)
//                {
//                    right_diuxian_num=right_total_none_num/project_i;
//                    break;
//                }
//            }            
//        }        
//        /****************************************************/
//        if(abs(right_diuxian_num-left_diuxian_num)<2)//�ײ������ҵ�
//        {
//            //������ǰ��һ�Ѻ�
//            for(num_line_i=0;num_line_i<40;num_line_i++)
//            {
//                if(!get_img_point(num_line_i,40)&&get_img_point(num_line_i+1,40))
//                {
//                    LoopBorttomRow=num_line_i;
//                    if(LoopBorttomRow<=34)//�޶��ڵ�λ�� 34
//                    {
//                        LoopFlag=1;
//                        break;
//                    }
//                }
//            }   
//            i=0;
//            /***********************************************/ 
//            if(LoopFlag&&LoopBorttomRow>=26)//̫����Ҫ����д��ʮ�� �п������� 25
//            {//����ж��ǻ�·���ڽ�һ���ж�
//                for(num_line_i=LoopBorttomRow;num_line_i<59;num_line_i++)//��
//                {
//                     for(StartCol=20;StartCol<61;StartCol=StartCol+10)//��  20 30 40 50 60
//                    {
//                        if(!get_img_point(num_line_i,StartCol)&&get_img_point(num_line_i+1,StartCol))
//                        {
//                            LoopBorttomRow_arrary[i++]=num_line_i;
//                            if(i>4)
//                            {
//                                break;
//                            }
//                      
//                        }                    
//                    }                
//                }  
//                //xianzhi_daxiao(int a[],int num)
//                if(xianzhi_daxiao(LoopBorttomRow_arrary,50)>=4)
//                {
//                    LoopFlag=0;
//                }
//                else if(abs(abs(LoopBorttomRow_arrary[1]-LoopBorttomRow_arrary[2])-abs(LoopBorttomRow_arrary[4]-LoopBorttomRow_arrary[3]))<4)
//                {
//                    LoopFlag=1;
//                }
//                else if(abs(LoopBorttomRow_arrary[4]-LoopBorttomRow_arrary[2])>=10)
//                {
//                    LoopFlag=0;
//                }
//            }
//            
//       }
//   
//    }
//}
void FindInflectionPoint()//����û���� ���� ������
{
	char i=0;
	int distance=0;
	int g_point_flag=0;
    int num_line_i=0;
    int left_total_none_num=0,right_total_none_num=0;
    int project_i=0;
    int LoopBorttomRow_arrary[10]={0};
	//��������
	RightInflectionPointRow=0;//�ұߵĹյ��������
	RightInflectionPointCol=0;//�ұߵĹյ��������
	LeftInflectionPointRow=0;//��ߵĹյ��������
	LeftInflectionPointCol=0;//��ߵĹյ��������
	RightInflectionPointFlag=0;//�ҵ���յ�ı�־λ
	LeftInflectionPointFlag=0;//�ҵ��ҹյ�ı�־λ

	StartRow=0;//�ӵײ�����ɨ��Բ���м�ĺ�Բ����ʼ��
	StartCol=0;//�ӵײ�����ɨ��Բ���м�ĺ�Բ����ʼ��

	LoopBorttomFlag=0;//�ҵ�Բ���м�ĺ�Բ�ĵײ��ı�־λ
	LoopBorttomRow=0;//Բ���м�ĺ�Բ�ĵײ���������
	LoopMilldleRow=0;//Բ���м�ĺ�Բ�Ķ�����������
	LoopMilldleFlag=0;//�ҵ�Բ���м�ĺ�Բ�Ķ����ı�־λ
	LoopTopRow=0;//�ҵ��⻷�Ķ���
	MilldleBlack=0;//Բ���м�ĺ�Բ�Ķ�����Բ���м�ĺ�Բ�ĵײ����������ƽ��ֵ
	LoopFlag=0;//�ҵ������������͵ı�־
   

	//��յ�
	for(i=3;i<=32;i++) //35 30
	{
		if((left_pos[i]!=40)&&(left_pos[i]!=79)) 
		{     
			if((left_pos[i]-left_pos[i+1]<0))//�ҵ��յ�
			{
				LeftInflectionPointRow=i;//��¼�ùյ����           
				LeftInflectionPointCol=left_pos[i];//��¼�ùյ����           
				LeftInflectionPointFlag=1;//����ҵ���յ�              
				break;//�ҵ��˳�                                  
			}
		}                                                                                                                                                                                                                                            
	} 

	//�ҹյ� 
	for(i=3;i<=32;i++)//����ɨ��̫Զ�����������//35 30
	{
		if((right_pos[i]!=40)&&(right_pos[i]!=1)) //�������в�����
		{     
			if((right_pos[i]-right_pos[i+1]>0))//�ҵ��ұ����йյ�
			{         
				RightInflectionPointRow=i;//��¼�յ����
				RightInflectionPointCol=right_pos[i];//��¼�յ����
				RightInflectionPointFlag=1;//����ҵ���յ�              
				break;//�ҵ��˳�
			}      
		} 
	}
	//��ʮ���п������гɻ�· ���е�ʱ�����ҹյ������ �����2���� ��69����
	//
	//����ͬʱ�ҵ������յ㣬��ʼʶ��·(������Կ���һ�£����ֻ�ҵ�һ���յ��������������ܸ�����ʶ�𵽻�·)

	if(LeftInflectionPointFlag&&RightInflectionPointFlag)//ͬʱ�ҵ������յ�
	{
		StartCol=(unsigned int)((LeftInflectionPointCol+RightInflectionPointCol)/2);// ȡ���ҹյ��������ƽ��ֵ    
		StartRow=(unsigned int)((LeftInflectionPointRow+RightInflectionPointRow)/2);//ȡ���ҹյ���������ƽ��ֵ    
//        if(StartRow<35)
//        {
            g_point_flag=1;
//        }
//        else 
//        {
//            g_point_flag=0;
//            shizi_wait=0;
//        }	

		for(i=StartRow;i<60&&g_point_flag;i++)//�̶�һ�У�StartCol���ӿ�ʼ��������ɨ�裬Ѱ�һ�·�м��Բ���������ҵ�һ���׵��ڵ����䣬Ȼ����кڵ�Ȼ�󵽰ף�
		{
			if(!get_img_point(i,StartCol)&&get_img_point(i+1,StartCol))//������λ��
			{
				LoopBorttomRow=i;//��¼��һ��������� ����ʱ�ҵ��˰ױ�ڵ�λ�ò���¼��
				distance=LoopBorttomRow-StartRow;
				if(distance>8&&distance<35&&LoopBorttomRow<50)
				{
					LoopBorttomFlag=1;//��λ��־   ;           
					break; //�����Ѿ��ҵ������յ��һ�������
				}
				else if(LoopBorttomRow>52&&StartRow<6)
				{
				//           Shi_zi_flag=1;
				//           break; 
				}

			}     
		} 
		//�����ҵ��� �յ㣨�����������ͻ���λ�ã� �׺� �ڰ�����������λ�� ���Ҽ�¼��
		/********************************************************************/    
		if(LoopBorttomFlag)//����֮ǰ�Ѿ��ҵ�Բ�������������--�ж�������ȵ�ͻ��
		{//
			if(ABS(left_pos[StartRow+2]-right_pos[StartRow+2])-35>=4//4,8������������Ҫ����
			&&ABS(left_pos[StartRow+4]-right_pos[StartRow+4])-40>=8)//�������ͻ��
			{
				LoopFlag=1;//����������־λ��
                loop_guaidian_flag=1;//˵���ҵ������յ�
				LoopRightControlFlag=1;//�õ���·��־λ�͸�����
				enter_loop=1;
			}                           
		}
	}
    else //���û���ҵ��յ㣬˵���յ���ʧ�������ǵײ����ߣ�ǰ��һ�ź�
    {
        for(num_line_i=0;num_line_i<58;num_line_i++)
        {
            if(left_pos[num_line_i]==79)
            {
                left_total_none_num=left_total_none_num+num_line_i;
                project_i++;
            }
            else
            {
                if(project_i>3)
                {
                    left_diuxian_num=left_total_none_num/project_i;
                    break;
                }
            }            
        }
         project_i=0;
        /******************************************************/
        for(num_line_i=0;num_line_i<58;num_line_i++)
        {
            if(right_pos[num_line_i]==1)
            {
                right_total_none_num=right_total_none_num+num_line_i;
                project_i++;
            }
            else
            {
                if(project_i>3)
                {
                    right_diuxian_num=right_total_none_num/project_i;
                    break;
                }
            }            
        }        
        /****************************************************/
        if(abs(right_diuxian_num-left_diuxian_num)<2)//�ײ������ҵ�
        {
            //������ǰ��һ�Ѻ�
            for(num_line_i=0;num_line_i<40;num_line_i++)
            {
                if(!get_img_point(num_line_i,40)&&get_img_point(num_line_i+1,40))
                {
                    LoopBorttomRow=num_line_i;
                    if(LoopBorttomRow<=34)//�޶��ڵ�λ�� 34
                    {
                        LoopFlag=1;
                        break;
                    }
                }
            }   
            i=0;
            /***********************************************/
            if(LoopBorttomRow>=25)//̫����Ҫ����д��ʮ�� �п�������
            {
                for(num_line_i=LoopBorttomRow;num_line_i<59;num_line_i++)//��
                {
                     for(StartCol=20;StartCol<61;StartCol=StartCol+10)//��  20 30 40 50 60
                    {
                        if(!get_img_point(num_line_i,StartCol)&&get_img_point(num_line_i+1,StartCol))
                        {
                            LoopBorttomRow_arrary[i++]=num_line_i;
                            if(i>4)
                            {
                                break;
                            }
                      
                        }                    
                    }                
                }  
                //xianzhi_daxiao(int a[],int num)
                if(xianzhi_daxiao(LoopBorttomRow_arrary,50)>=3)
                {
                    LoopFlag=0;
                }
                else if(abs(abs(LoopBorttomRow_arrary[1]-LoopBorttomRow_arrary[2])-abs(LoopBorttomRow_arrary[2]-LoopBorttomRow_arrary[3]))<4)
                {
                    LoopFlag=1;
                }
                else if(abs(LoopBorttomRow_arrary[4]-LoopBorttomRow_arrary[2])>=10)
                {
                    LoopFlag=0;
                }
            }
            
       }
   
    }
}



///*
//x [0-80]
//y [0-60]
//*/
int xielv_k[80]={0};
void two_point_xielv(int hang[],int num)
{
    int lie=0;
    int k=0;
    for(lie=10;lie<=70-num;lie++)
    {
//      if(hang[lie]!=0)
//      {
        k=(hang[lie]-hang[lie+num])/num;
        xielv_k[lie]=k;
//      }         
    }    
}
void FindInflectionPoint_RR()
{
	char i=0;
	int distance=0;
	int g_point_flag=0;
//    int num_line_i=0;
//    int left_total_none_num=0,right_total_none_num=0;
//    int project_i=0;
//    int LoopBorttomRow_arrary[10]={0};
//    int w_h_point[80]={0},lie=0,hang=0;
//    int temp_zeor=0,temp_zeor_2=0;
//    int guaidian_lie=0;
//    int radio_out_loop_1[20],radio_out_loop_2[20];
	//��������
	RightInflectionPointRow=0;//�ұߵĹյ��������
	RightInflectionPointCol=0;//�ұߵĹյ��������
	LeftInflectionPointRow=0;//��ߵĹյ��������
	LeftInflectionPointCol=0;//��ߵĹյ��������
	RightInflectionPointFlag=0;//�ҵ���յ�ı�־λ
	LeftInflectionPointFlag=0;//�ҵ��ҹյ�ı�־λ

	StartRow=0;//�ӵײ�����ɨ��Բ���м�ĺ�Բ����ʼ��
	StartCol=0;//�ӵײ�����ɨ��Բ���м�ĺ�Բ����ʼ��

	LoopBorttomFlag=0;//�ҵ�Բ���м�ĺ�Բ�ĵײ��ı�־λ
	LoopBorttomRow=0;//Բ���м�ĺ�Բ�ĵײ���������
	LoopMilldleRow=0;//Բ���м�ĺ�Բ�Ķ�����������
	LoopMilldleFlag=0;//�ҵ�Բ���м�ĺ�Բ�Ķ����ı�־λ
	LoopTopRow=0;//�ҵ��⻷�Ķ���
	MilldleBlack=0;//Բ���м�ĺ�Բ�Ķ�����Բ���м�ĺ�Բ�ĵײ����������ƽ��ֵ
	LoopFlag=0;//�ҵ������������͵ı�־
   

	//��յ�
	for(i=3;i<=32;i++) //35 30
	{
		if((left_pos[i]!=40)&&(left_pos[i]!=79)) 
		{     
			if((left_pos[i]-left_pos[i+1]<0))//�ҵ��յ�
			{
				LeftInflectionPointRow=i;//��¼�ùյ����           
				LeftInflectionPointCol=left_pos[i];//��¼�ùյ����           
				LeftInflectionPointFlag=1;//����ҵ���յ�              
				break;//�ҵ��˳�                                  
			}
		}                                                                                                                                                                                                                                            
	} 

	//�ҹյ� 
	for(i=3;i<=32;i++)//����ɨ��̫Զ�����������//35 30
	{
		if((right_pos[i]!=40)&&(right_pos[i]!=1)) //�������в�����
		{     
			if((right_pos[i]-right_pos[i+1]>0))//�ҵ��ұ����йյ�
			{         
				RightInflectionPointRow=i;//��¼�յ����
				RightInflectionPointCol=right_pos[i];//��¼�յ����
				RightInflectionPointFlag=1;//����ҵ���յ�              
				break;//�ҵ��˳�
			}      
		} 
	}
	//��ʮ���п������гɻ�· ���е�ʱ�����ҹյ������ �����2���� ��69����
	//
	//����ͬʱ�ҵ������յ㣬��ʼʶ��·(������Կ���һ�£����ֻ�ҵ�һ���յ��������������ܸ�����ʶ�𵽻�·)

	if(LeftInflectionPointFlag&&RightInflectionPointFlag)//ͬʱ�ҵ������յ�
	{
		StartCol=(unsigned int)((LeftInflectionPointCol+RightInflectionPointCol)/2);// ȡ���ҹյ��������ƽ��ֵ    
		StartRow=(unsigned int)((LeftInflectionPointRow+RightInflectionPointRow)/2);//ȡ���ҹյ���������ƽ��ֵ    
        g_point_flag=1;
		for(i=StartRow;i<60&&g_point_flag;i++)//�̶�һ�У�StartCol���ӿ�ʼ��������ɨ�裬Ѱ�һ�·�м��Բ���������ҵ�һ���׵��ڵ����䣬Ȼ����кڵ�Ȼ�󵽰ף�
		{
			if(!get_img_point(i,StartCol)&&get_img_point(i+1,StartCol))//������λ��
			{
				LoopBorttomRow=i;//��¼��һ��������� ����ʱ�ҵ��˰ױ�ڵ�λ�ò���¼��
				distance=LoopBorttomRow-StartRow;
				if(distance>8&&distance<35&&LoopBorttomRow<50)
				{
					LoopBorttomFlag=1;//��λ��־   ;           
					break; //�����Ѿ��ҵ������յ��һ�������
				}
				else if(LoopBorttomRow>52&&StartRow<6)
				{
				//           Shi_zi_flag=1;
				//           break; 
				}

			}     
		} 
		//�����ҵ��� �յ㣨�����������ͻ���λ�ã� �׺� �ڰ�����������λ�� ���Ҽ�¼��
		/********************************************************************/    
		if(LoopBorttomFlag)//����֮ǰ�Ѿ��ҵ�Բ�������������--�ж�������ȵ�ͻ��
		{//
			if(ABS(left_pos[StartRow+2]-right_pos[StartRow+2])-35>=4//4,8������������Ҫ����
			&&ABS(left_pos[StartRow+4]-right_pos[StartRow+4])-40>=8)//�������ͻ��
			{
				LoopFlag=1;//����������־λ��
                loop_guaidian_flag=1;//˵���ҵ������յ�
				LoopRightControlFlag=1;//�õ���·��־λ�͸�����
				enter_loop=1;
			}                           
		}
	}
//    else //���û���ҵ��յ㣬˵���յ���ʧ�������ǵײ����ߣ�ǰ��һ�ź�
//    {
//        for(num_line_i=0;num_line_i<58;num_line_i++)
//        {
//            if(left_pos[num_line_i]==79)
//            {
//                left_total_none_num=left_total_none_num+num_line_i;
//                project_i++;
//            }
//            else
//            {
//                if(project_i>3)
//                {
//                    left_diuxian_num=left_total_none_num/project_i;
//                    break;
//                }
//            }            
//        }
//         project_i=0;
//        /******************************************************/
//        for(num_line_i=0;num_line_i<58;num_line_i++)
//        {
//            if(right_pos[num_line_i]==1)
//            {
//                right_total_none_num=right_total_none_num+num_line_i;
//                project_i++;
//            }
//            else
//            {
//                if(project_i>3)
//                {
//                    right_diuxian_num=right_total_none_num/project_i;
//                    break;
//                }
//            }            
//        }        
//        /****************************************************/
//        if(abs(right_diuxian_num-left_diuxian_num)<2)//�ײ������ҵ�
//        {
//            //������ǰ��һ�Ѻ�
//            for(lie=20;lie<=60;lie++)
//            {
//              for(hang=10;hang<=55;hang++)
//              {
//                if(!get_img_point(hang,lie)&&get_img_point(hang+1,lie))
//                {
//                  w_h_point[lie]=hang;//�ռ������
//                  if(w_h_point[lie]<35)
//                  {
//                    temp_zeor++;
//                  }
//                  if(w_h_point[lie]>45)
//                  {
//                    temp_zeor_2++;
//                  }
//                  break;
//                }
//              }
//            }
//            for(lie=20;lie<58;lie++)
//            {
//                if(w_h_point[lie]<w_h_point[lie+1])//��б�ʹյ�
//                {
//                  guaidian_lie=lie;        
//                  break;
//                }
//            }
////            i=0;
////            for(lie=20;lie<guaidian_lie;lie=lie+5)
////            {
////                radio_out_loop_1[i++]=regression((uint16 *)w_h_point,20,lie);
////            }
//            
//            //radio_out_loop_2=regression((uint16 *)w_h_point,guaidian_lie,60);
//            if(temp_zeor>30)//�뻷
//            {
//                LoopFlag=1;
//            }
//            else if(temp_zeor_2>9)
//            {
//                LoopFlag=0;
//            }
//            
////            for(project_i=0;project_i<80;project_i++)//
////            {
////                if(w_h_point[project_i]!=0)
////                {
////                    temp_zeor=project_i;//�ҵ���һ������0��
////                }
////                else
////                {
////                    if(temp_zeor>30)
////                    {
////                        break;
////                    }
////                }            
////            }
//            /***************************************************///������б��
//           // two_point_xielv(w_h_point,3);
//            
//           
//            
//       }
//   
//    }
}




       

