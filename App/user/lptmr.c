//============================================================================
// �ļ����ƣ�lptmr.c                                                          
// ���ܸ�Ҫ��lptmr����Դ�ļ�(Low Power Timer (LPTMR))
//============================================================================
//#include "MK60F15.h"
//#include "MK60DZ10.H"
#include "common.h"   
//============================================================================
//�������ƣ�lptmr_internal_ref_init
//�������أ���
//����˵�����ڲ��ο�ʱ�ӳ�ʼ��
//���ܸ�Ҫ�������ڲ��ο�ʱ�ӣ�PSC=0X0��
//         �ڲ��ο�ʱ��������ʱ��Դ:(1)MCG_C2[IRCS]=0,ʹ�������ڲ�ʱ��(32kHz)
//                                 (2)MCG_C2[IRCS]=1,ʹ�ÿ����ڲ�ʱ��(2Mhz)   
//         �������õ��ǿ���ʱ��Դ������= compare_value/ClkBus/prescale=4��
//         prescale = 2^(8+1)=512,ClkBus=2Mhz,compare_value=15625
//============================================================================
#define LPTMR_ALT1 0x1  // PORTA19 pin 18
#define LPTMR_ALT2 0x2  // PORTA19 pin 18
#define OSC_BASE_PTR                             ((OSC_MemMapPtr)0x40065000u)
#define OSC_CR                                   OSC_CR_REG(OSC_BASE_PTR)
void lptmr_clear_registers();
void lptmr_internal_ref_init()
{
	unsigned int compare_value=15625; //4��
	
	lptmr_clear_registers();          //��λLPTMRģ��
	SIM_SCGC5|=SIM_SCGC5_LPTIMER_MASK;//ʹ��LPTģ��ʱ��
	MCG_C1|=MCG_C1_IRCLKEN_MASK;      //ʹ���ڲ��ο�ʱ��
	 
	MCG_C2|=MCG_C2_IRCS_MASK; //MCG_C2[IRCS]=1,ʹ�ܿ����ڲ��ο�ʱ�ӣ�2MHz��
							  //MCG_C2[IRCS]=0,Ϊ����ģʽ��32Khz��
	
	// ���� LPTMR 
	LPTMR0_CMR=LPTMR_CMR_COMPARE(compare_value);          //���ñȽϼĴ���ֵ
	LPTMR0_PSR=LPTMR_PSR_PCS(0x0)|LPTMR_PSR_PRESCALE(0x8);//ʹ���ڲ�ʱ�ӣ�512Ԥ��Ƶ(2^(n+1))

	LPTMR0_CSR|=LPTMR_CSR_TEN_MASK; //����LPTģ������
}

//============================================================================
//�������ƣ�lptmr_LPO_init
//�������أ���
//����˵����LPOʱ�ӳ�ʼ��
//���ܸ�Ҫ������LPOʱ�ӣ�PSC=0X1,1kHz��   
//         �������õ��ǿ���ʱ��Դ������= compare_value/ClkBus/prescale=5��
//         prescale = ��,ClkBus=1kHz,compare_value=5000
//============================================================================
void lptmr_LPO_init()
{
	int compare_value=1000;  //ֵ����С�� (0xFFFF,65535)
	
	lptmr_clear_registers();          //��λLPTMRģ��
	SIM_SCGC5|=SIM_SCGC5_LPTIMER_MASK;//ʹ��LPTģ��ʱ��
	
	//���� LPTMR 
	LPTMR0_CMR=LPTMR_CMR_COMPARE(compare_value);      //���ñȽϼĴ���ֵ
	LPTMR0_PSR=LPTMR_PSR_PCS(0x1)|LPTMR_PSR_PBYP_MASK;
	
	
	LPTMR0_CSR|=LPTMR_CSR_TEN_MASK; //����LPTģ������
}

//============================================================================
//�������ƣ�lptmr_32khz_init
//�������أ���
//����˵����LDOʱ�ӳ�ʼ��
//���ܸ�Ҫ�����ö����ⲿ�ο�ʱ�ӣ�PSC=0x2,32kHz��   
//         SOPT1[OSC32KSEL]=1,ʹ�����ӵ�XTAL32�ϵ�32kHz RTC crystal
//         SOPT1[OSC32KSEL]=0,ʹ����32kHz ϵͳ crystal����Ҫ��������32kHz��crystal
//
//         ����= compare_value/ClkBus/prescale=5��
//         prescale = ��,ClkBus=1kHz,compare_value=5000
//============================================================================
void lptmr_32khz_init()
{
	unsigned int compare_value=32768; //4 second delay with prescale=1
	
	lptmr_clear_registers();//��λLPTMRģ��
	SIM_SCGC5|=SIM_SCGC5_LPTIMER_MASK;//ʹ��LPTģ��ʱ��
	
	// ����ʹ������RTCʱ�ӵĶ����ⲿ�ο�ʱ�ӣ�32Khz��
	SIM_SCGC6 |= SIM_SCGC6_RTC_MASK;      //ʹ�� RTC �Ĵ���
	RTC_CR    |= RTC_CR_OSCE_MASK;        //����  RTC oscillator
	SIM_SOPT1 |= SIM_SOPT1_OSC32KSEL_MASK;//ѡ�� RTC OSC ��Ϊ ERCLK32K��ʱ��Դ
	
	//����Configure LPTMR
	LPTMR0_CMR=LPTMR_CMR_COMPARE(compare_value);          //���ñȽϼĴ���ֵ
	LPTMR0_PSR=LPTMR_PSR_PCS(0x2)|LPTMR_PSR_PRESCALE(0x1);//ʹ�� 32khz clock (ERCLK32K)�ҽ��� 4��Ƶ
	
	LPTMR0_CSR|=LPTMR_CSR_TEN_MASK; //����LPTģ������
}

//============================================================================
//�������ƣ�lptmr_external_clk_init
//�������أ���
//����˵�����ⲿʱ�ӳ�ʼ��
//���ܸ�Ҫ�����ö����ⲿ�ο�ʱ�ӣ�PSC=0x3,50MHz��   
//
//         ����= compare_value/ClkBus/prescale=1��
//         prescale = 1024,ClkBus=48MHz,compare_value=46875
//============================================================================
void lptmr_external_clk_init()
{
	unsigned int compare_value=46875;
	
	lptmr_clear_registers();          //��λLPTMRģ��
	SIM_SCGC5|=SIM_SCGC5_LPTIMER_MASK;//ʹ��LPTģ��ʱ��
	
	//���ⲿ�ο�ʱ��
	MCG_C2&=~MCG_C2_EREFS_MASK;     //���� extal to drive
	OSC_CR |= OSC_CR_ERCLKEN_MASK;  //ѡ�� EXTAL to drive  XOSCxERCLK
	
	// ���� LPTMR
	LPTMR0_CMR=LPTMR_CMR_COMPARE(compare_value);          //���ñȽ�ֵ
	LPTMR0_PSR=LPTMR_PSR_PCS(0x3)|LPTMR_PSR_PRESCALE(0x09);//ʹ���ⲿʱ�ӣ�Ԥ��ƵΪ 65536
	
	LPTMR0_CSR|=LPTMR_CSR_TEN_MASK; //����LPTģ������
}

//============================================================================
//�������ƣ�lptmr_pulse_counter
//�������أ���
//����˵����pin_select:ѡ������ţ���1,2��
//���ܸ�Ҫ�������ۼӹ��� ��LPTMR0_ALT1��LPTMR0_ALT2����
//         LPTMR0_ALT1������PORTA19 (ALT6)
//         LPTMR0_ALT2������PORTC5 (ALT4).
//         
//============================================================================
void lptmr_pulse_counter(char pin_select)
{
	unsigned int compare_value=0xffff;  //�ڱ���������ģʽ��ֻ�趨ʱȥ��ȡ����ֵ�Ϳ��ԣ�����Ҫ�Ƚ�
//	char input;

	lptmr_clear_registers();          //��λLPTMRģ��
	SIM_SCGC5|=SIM_SCGC5_LPTIMER_MASK;//ʹ��LPTģ��ʱ��

	//Set up GPIO
	if(pin_select==LPTMR_ALT1)
	{
		SIM_SCGC5 |= SIM_SCGC5_PORTA_MASK; //�� PORTA ʱ��
		PORTA_PCR19=PORT_PCR_MUX(0x6);     //��PTA19��ʹ�� ALT6
	}
	else if(pin_select==LPTMR_ALT2)
	{
		SIM_SCGC5 |= SIM_SCGC5_PORTC_MASK; //�� PORTC ʱ�� 
		PORTC_PCR5=PORT_PCR_MUX(0x4);      //��PTC5��ʹ�� ALT4
	}
	else
	{
		return;
	}

	LPTMR0_PSR=LPTMR_PSR_PCS(0x1)|LPTMR_PSR_PBYP_MASK; //ʹ�� LPO clock �� bypass glitch filter
	LPTMR0_CMR=LPTMR_CMR_COMPARE(compare_value);        //���ñȽ�ֵ
	LPTMR0_CSR=LPTMR_CSR_TPS(pin_select)|LPTMR_CSR_TMS_MASK; //����LPTʹ��ѡ�������,�ҽ��������ۼ�ģʽ,�����ز�׽
	
	LPTMR0_CSR|=LPTMR_CSR_TEN_MASK; //���� LPTģ��
}

//============================================================================
//�������ƣ�lptmr_clear_registers
//�������أ���
//����˵������
//���ܸ�Ҫ����LPTģ������мĴ���
//============================================================================
void lptmr_clear_registers()
{
//	LPTMR0_CSR=0x00;
//	LPTMR0_PSR=0x00;
//	LPTMR0_CMR=0x00;
}


//============================================================================
//�������ƣ�enable_lptmr_int
//�������أ���
//����˵������
//���ܸ�Ҫ������LPT��ʱ���ж�
//============================================================================
void enable_lptmr_int()
{
	LPTMR0_CSR|=LPTMR_CSR_TIE_MASK; //����LPT��ʱ���ж�            
	enable_irq(LPTMR_IRQn);	    //�����ŵ�IRQ�ж�	
}

//============================================================================
//�������ƣ�disable_lptmr_int
//�������أ���
//����˵������
//���ܸ�Ҫ���ر�LPT��ʱ���ж�
//============================================================================
void disable_lptmr_int()
{
	LPTMR0_CSR &=~LPTMR_CSR_TIE_MASK;   //��ֹLPT��ʱ���ж�          
	disable_irq(LPTMR_IRQn);	    //�����ŵ�IRQ�ж�
}


