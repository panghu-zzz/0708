#ifndef __RING_H__
#define __RING_H__

/*** �궨�� ****/ 

#define Black_Point !0
#define White_Point 0
//����������ʼ������40  ����жϵ�40�˾��Ƕ�����
#define RowMax	    60-2  //����
#define ColumnMax	 80-2	  //����


/*** �ⲿ������������ ****/  


//extern uint8  img[CAMERA_H][CAMERA_W];//����洢����ͼ�������,CAMERA_H(59-0),CAMERA_W(0-79)

//extern uint8 imgbuff[CAMERA_SIZE];
extern int   AllLose;
extern int BlackEndL;
extern int BlackEndM;
extern int BlackEndR;
extern  int   RightEdge[RowMax+1];
extern  int   MiddleLine[RowMax+1];
extern int   LastLine;
extern int   LeftLose;
extern int   RightLose ;
extern unsigned char LoopFlag;

extern  unsigned char LoopRightControlFlag;
extern  unsigned char LoopLeftControlFlag;
extern  int MotivateLoopDlayFlagR;
extern  int MotivateLoopDlayFlagL;
extern  unsigned char MotivateLoopDlayFlagLOK;
extern  unsigned char MotivateLoopDlayFlagROK;
extern  unsigned char OpenLoopExitRepairFlagL;
extern  unsigned char OpenLoopExitRepairFlagR;
extern  unsigned char RightInflectionPointFlag;
extern  unsigned char LeftInflectionPointFlag;

extern  unsigned char  openLoopFlag;//�Ƿ���Բ����־λ

extern unsigned int  enter_loop;
extern unsigned int  out_loop;//����·
/*** �������� ****/    

extern void FindInflectionPoint();
//extern void SearchCenterBlackline(void);
//extern void GetBlackEndParam();
//extern void DrawBlackline();
//void MidValeFilte(void);
//void SendImage();
//int SortRank(int x,int y,int z);
//extern void FindLoopExit();
//extern void SetInitVal();
//extern  void LoopControl();
//extern  void LoopRepair();
//
//extern  void LoopExitRepair();
extern void FindInflectionPoint_RR();
#endif 








