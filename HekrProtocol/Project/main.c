/* MAIN.C file
 * 
 * 
 */

#include "stm8s103f.h"
#include "sys.h"
#include "delay.h"
#include "stm8_uart.h"
#include "hekr_protocol.h"

u8 RecvFlag = 0;
u8 RecvCount = 0;
u8 RecvBuffer[20];
u8 DateHandleFlag = 0;

@near unsigned char ProdKey[16] = {0x01,0x36,0xA6,0x6C,0x12,0x75,0x4E,0xE8,0x2F,0xFF,0x88,0x04,0xB7,0xFA,0xA5,0x3C};         //��Ʒ��Կ���ã���16���ֽ�,��ǰʾ��ProdKeyֵ��ӦLED�������ҳ�档ProdKey�Ļ�ȡ�����÷����ɲο���http://docs.hekr.me/v4/resourceDownload/protocol/uart/#44prodkey

void System_init(void);

/*********************HEKR USER API*********************/

/*******************************************************/
//#define USER_MAX_LEN 0x64u
//ʹ��ǰҪȷ���û�����Ҫ��������� Ĭ��Ϊ100 ����100��Ҫ�����޸�
//�����С���������޸�Ϊ�����  
//����ж������ȳ�����  ȡ�����  Ϊ�û����ݳ���  ����֡����
/*******************************************************/


/*******************************************************/
//void HekrInit(void (*fun)(unsigned char));
//eg:  HekrInit(UART_SendChar);   
//HekrInit����:
//�������Ϊ�û����ڷ���һ��byte�����ĺ�����
//Hekr Э���ʼ��
//ʹ��HekrЭ��ǰ����ɳ�ʼ��
//��ʼ����Ҫ�û��д��ڷ���һ��byte�ĳ���
/*******************************************************/


/*******************************************************/
//eg:  void UART_SendChar(u8 ch); �����������ֻ��һ��8bit����
//     �ú�����Ҫ�û������ڳ����ж���
/*******************************************************/


/*******************************************************/
//unsigned char HekrRecvDataHandle(unsigned char* data);
//�������ݽ��մ���
//����ֵ��ͷ�ļ� RecvDataHandleCode
//���ݱ����ڶ�Ӧ������ valid_data �� ModuleStatus ָ��
//ģ��״ֵ̬������module_status������
/*******************************************************/


/*******************************************************/
//void HekrValidDataUpload(unsigned char len);
//�ϴ��û���Ч����
//���ݴ����valid_data�����У�len Ϊ�û����ݳ��ȣ�����֡����
/*******************************************************/


//Э���޸����� 2016.06.23 
//Э����ַ  http://docs.hekr.me/v4/resourceDownload/protocol/uart/
//BUG ����  zejun.zhao@hekr.me
//					387606503@qq.com
/*******************************************************/

main()
{
	u8 temp;
	u8 UserValidLen = 9;
	System_init();
	
	HekrValidDataUpload(UserValidLen);	   //�ϴ���Ч����
	/*************ģ���������************/
	Module_State_Function();               //ģ��״̬��ѯ
	/*
	Module_Soft_Reboot_Function();         //ģ��������
	Module_Factory_Reset_Function();       //ģ��ָ���������
	Hekr_Config_Function();                //ģ�����һ������
	Module_Set_Sleep_Function();           //ģ���������
	Module_Weakup_Function();              //ģ�����߻���
  Module_Factory_Test_Function();        //ģ����볧��ģʽ
	Module_Firmware_Versions_Function();   //ģ��汾��ѯ
	Module_ProdKey_Get_Function();         //ģ���Ʒ��ԿProdKey��ѯ
	Set_ProdKey(ProdKey);                  //ģ��ProdKey���ã��û����ݴ�consoleƽ̨��ȡ��ProdKeyֵ���ò�Ʒ��Կ����ProdKey[16]
	*/
	
	while (1)
	{
		if(RecvFlag && !DateHandleFlag)
		{
			DateHandleFlag = 1;
			RecvFlag = 0;
		}
		if(DateHandleFlag)
		{
			temp = HekrRecvDataHandle(RecvBuffer);
			if(ValidDataUpdate == temp)
			{
				//���յĲ�Ʒҵ�����ݱ����� valid_data ������
				/************��Ʒҵ�����ݲ����û�����********/
				UART1_SendChar(valid_data[0]);
				/********************************************/
			}
			if(HekrModuleStateUpdate == temp)
			{
				//���յ�ģ��״̬ȡֵ������ ModuleStatus ָ����
				/*************ģ��״̬�����û�����************/
				UART1_SendChar(ModuleStatus->Mode);           //��ӡģ�鹤��ģʽָʾ�ֽ�
				UART1_SendChar(ModuleStatus->WIFI_Status);    //��ӡģ��WIFI״ָ̬ʾ�ֽ�
				UART1_SendChar(ModuleStatus->CloudStatus);    //��ӡģ��������״ָ̬ʾ�ֽ�
				UART1_SendChar(ModuleStatus->SignalStrength); //��ӡģ��״̬��ѯӦ��֡�����ֽ�
				/*************ģ��״̬ȡֵ�ο���http://docs.hekr.me/v4/resourceDownload/protocol/uart/#42  **********/
				
			}
			DateHandleFlag = 0;			
		}			
	}
}


void System_init(void)
{
	System_Clock_init();
	UART1_Init();
	delay_init(16);
	HekrInit(UART1_SendChar);
	_asm("rim");
}
 
@far @interrupt void UART1_Recv_IRQHandler(void)
{
  unsigned char ch;
	static unsigned char TempFlag = 0;
  ch = UART1_DR;   
	if(ch == HEKR_FRAME_HEADER)
	{
		TempFlag = 1;
		RecvCount = 0;
	}
	if(TempFlag)
	{
		RecvBuffer[RecvCount++] = ch;
		if(RecvCount > 4 && RecvCount >= RecvBuffer[1])
		{
			RecvFlag = 1;
			TempFlag = 0;
			RecvCount = 0;
		}
	}
}
	

