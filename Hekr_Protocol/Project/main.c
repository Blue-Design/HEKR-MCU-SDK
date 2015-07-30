/* MAIN.C file
 * 
 * 
 */

#include "stm8s103f.h"
#include "sys.h"
#include "delay.h"
#include "stm8_uart.h"
#include "hekr_protocol.h"

u8 Recv_Flag = 0;
u8 Recv_Count = 0;
u8 Recv_Buffer[20];
u8 Date_Handle_Flag = 0;


void System_init(void);


// Hekr USER API **************************************************************

//���봮�ڽ��յ���������  
//����ֵ��ͷ�ļ� RecvData_Handle_Code
//���ݱ����ڶ�Ӧ������ Valid_Data �� Module_Status
//unsigned char Hekr_RecvData_Handle(unsigned char* data);

//���ü���ѯhekrģ��״̬ ������ֵ��ͷ�ļ� Hekr_Module_Control_Code
//״ֵ̬������Module_Status������
//void Hekr_Module_Control(unsigned char data);

//�ϴ��û���Ч����
//���ݴ����Valid_Data������
//void Hekr_ValidData_Upload(void);

//����޸Ĵ�������Ҫ�޸Ĵ˺��� ����Ӧͷ�ļ�
//static void Hekr_Send_Byte(unsigned char ch);

// Э����ַ  http://docs.hekr.me/protocol/
// BUG ����  pengyu.zhang@hekr.me
//					 965006619@qq.com
//*****************************************************************************

main()
{
	u8 temp;
	System_init();
	
	// �ϴ���Ч����
	Hekr_ValidData_Upload();
	// ���ü���ѯhekrģ��״̬
	Hekr_Module_Control(Module_Query);
	
	while (1)
	{
		if(Recv_Flag && !Date_Handle_Flag)
		{
			Date_Handle_Flag = 1;
			Recv_Flag = 0;
		}
		if(Date_Handle_Flag)
		{
			temp = Hekr_RecvData_Handle(Recv_Buffer);
			if(Valid_Data_Update == temp)
			{
				//���յ����ݱ����� Valid_Data ������
				//User Code
				UART1_SendChar(Valid_Data[0]);
			}
			if(Hekr_Module_State_Update == temp)
			{
				//���յ����ݱ����� Module_Status ������
				//User Code
				UART1_SendChar(Module_Status[1]);
			}
			Date_Handle_Flag = 0;			
		}			
	}
}


void System_init(void)
{
	System_Clock_init();
	UART1_Init();
	delay_init(16);
	_asm("rim");
}
 
@far @interrupt void UART1_Recv_IRQHandler(void)
{
  unsigned char ch;
	static unsigned char temp_flag = 0;
  ch = UART1_DR;   
	if(ch == Hekr_Frame_Header)
	{
		temp_flag = 1;
		Recv_Count = 0;
	}
	if(temp_flag)
	{
		Recv_Buffer[Recv_Count++] = ch;
		if(Recv_Count > 4 && Recv_Count >= Recv_Buffer[1])
		{
			Recv_Flag = 1;
			temp_flag = 0;
			Recv_Count = 0;
		}
		if(Recv_Count > 15)
		{
			temp_flag = 0;
			Recv_Count = 0;
		}
	}
}
	

