#include "stm32f10x.h"                  // Device header
#include "uart.h"
#include "hekr_protocol.h"

/*********************HEKR USER API*********************/

/*******************************************************/
//#define USER_MAX_LEN 0x64u
//ʹ��ǰҪȷ���û�����Ҫ��������� Ĭ��Ϊ100 ����100��Ҫ�����޸�
//�����С���������޸�Ϊ�����
//����ж������ȳ����ȡ����� �˴�����ָ�û����ݳ��� ����֡����
/*******************************************************/


/*******************************************************/
//void HekrInit(void (*fun)(unsigned char));
//eg:  HekrInit(UART_SendChar);   
//HekrInit����:
//�������Ϊ�û����ڷ���һ��byte�����ĺ�����
//Hekr Э���ʼ��
//ʹ��HekrЭ��ǰ��Ҫ��ɳ�ʼ��
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
//���ݴ����valid_data������,len Ϊ�û����ݳ���,����֡����
/*******************************************************/


//Э���޸����� 2016.06.23 
//Э����ַ  http://docs.hekr.me/v4/resourceDownload/protocol/uart/
//BUG ����  zejun.zhao@hekr.me
//					387606503@qq.com
/*******************************************************/

u8 DateHandleFlag = 0;
unsigned char ProdKey[16] = {0x01,0x36,0xA6,0x6C,0x12,0x75,0x4E,0xE8,0x2F,0xFF,0x88,0x04,0xB7,0xFA,0xA5,0x3C};         //��Ʒ��Կ����,��16���ֽ�,��ǰʾ��ProdKeyֵ��ӦLED��ʵ�����ҳ�档ProdKey�Ļ�ȡ�����÷����ɲο�:http://docs.hekr.me/v4/resourceDownload/protocol/uart/#44prodkey


void System_Init(void);

int main()
{
	u8 temp;
	u8 UserValidLen = 9;
	System_Init();
	HekrInit(UART3_SendChar);	
	HekrValidDataUpload(UserValidLen);
	
	/*************ģ���������************/

	Module_State_Function();               //ģ��״̬��ѯ
  /*
	Module_Soft_Reboot_Function();         //ģ��������
	Module_Factory_Reset_Function();       //ģ��ָ���������
	Hekr_Config_Function();                //ģ�����һ������ģʽ
	Module_Set_Sleep_Function();           //ģ���������
	Module_Weakup_Function();              //ģ�����߻���
	Module_Firmware_Versions_Function();   //ģ��汾��ѯ
	Module_ProdKey_Get_Function();         //ģ���Ʒ��ԿProdKey��ѯ
	Module_Factory_Test_Function();        //ģ����볧��ģʽ
	Set_ProdKey(ProdKey);                  //ģ��ProdKey����,�û����ݴ�consoleƽ̨��ȡ��ProdKeyֵ���ò�Ʒ��Կ����ProdKey[16]
  */
	
	while (1)
	{
		if(Uart3_Recv_STA && !DateHandleFlag)
		{
			DateHandleFlag = 1;
			Uart3_Recv_STA = 0;
		}
		if(DateHandleFlag)
		{ 
			temp = HekrRecvDataHandle(Uart3_Recv_Buffer);
			if(ValidDataUpdate == temp)
			{
				//���յĲ�Ʒҵ�����ݱ����� valid_data ������
				/************��Ʒҵ�����ݲ����û�����********/
				UART3_SendChar(valid_data[0]);
				/********************************************/
			}
			if(HekrModuleStateUpdate == temp)
			{
				//���յ�ģ��״̬ȡֵ������ ModuleStatus ָ����
				/*************ģ��״̬�����û�����************/
				UART3_SendChar(ModuleStatus->Mode);           //��ӡģ�鹤��ģʽָʾ�ֽ�
				UART3_SendChar(ModuleStatus->WIFI_Status);    //��ӡģ��WIFI״ָ̬ʾ�ֽ�
				UART3_SendChar(ModuleStatus->CloudStatus);    //��ӡģ��������״ָ̬ʾ�ֽ�
				UART3_SendChar(ModuleStatus->SignalStrength); //��ӡģ��״̬��ѯӦ��֡�����ֽ�
				/*************ģ��״̬ȡֵ�ο�:http://docs.hekr.me/v4/resourceDownload/protocol/uart/#42  **********/
			}
			DateHandleFlag = 0;			
		}			
	} 
	return 0;
}



void System_Init(void)
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); 
	Uart3_init();
}

