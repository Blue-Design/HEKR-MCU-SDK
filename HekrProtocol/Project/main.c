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

@near unsigned char ProdKey[16] = {0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0A,0x0B,0x0C,0x0D,0x0E,0x0F,0x10};         //��Ʒ��Կ���ã���16���ֽ�

void System_init(void);


// HEKR USER API **************************************************************

//ʹ��ǰҪȷ���û�����Ҫ���������  Ĭ��Ϊ100 ����100��Ҫ�����޸�
//�����С���������޸�Ϊ�����  
//����ж������ȳ�����  ȡ�����  Ϊ�û����ݳ���  ����֡����
//#define USER_MAX_LEN 0x64u

//Hekr Э���ʼ��
//ʹ��HekrЭ��ǰ����ɳ�ʼ��
//��ʼ����Ҫ�û��д��ڷ���һ��byte�ĳ���
//eg:  void UART_SendChar(u8 ch); �����������ֻ��һ��8bit����
//     �ú�����Ҫ�û������ڳ����ж���
//HekrInit����:
//�������Ϊ�û����ڷ���һ��byte�����ĺ�����
//void HekrInit(void (*fun)(unsigned char));
//eg:  HekrInit(UART_SendChar);   

//���봮�ڽ��յ���������  
//����ֵ��ͷ�ļ� RecvDataHandleCode
//���ݱ����ڶ�Ӧ������ valid_data �� ModuleStatus ָ��
//unsigned char HekrRecvDataHandle(unsigned char* data);

//���ü���ѯhekrģ��״̬ ������ֵ��ͷ�ļ� HekrModuleControlCode
//״ֵ̬������module_status������
//void HekrModuleControl(unsigned char data);


//�ϴ��û���Ч����
//���ݴ����valid_data������ len Ϊ�û����ݳ���  ����֡����
//void HekrValidDataUpload(unsigned char len);

//Э���޸����� 2015.09.01 
//Э����ַ  http://docs.hekr.me/protocol/
//BUG ����  pengyu.zhang@hekr.me
//					965006619@qq.com
//*****************************************************************************

main()
{
	u8 temp;
	u8 UserValidLen = 9;
	System_init();
	/*************ģ���������************
	Module_State_Function();               //ģ��״̬��ѯ
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
	// �ϴ���Ч����
	HekrValidDataUpload(UserValidLen);
	// ���ü���ѯhekrģ��״̬
	HekrModuleControl(ModuleQuery);
	
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
				//���յ����ݱ����� valid_data ������
				//User Code
				UART1_SendChar(valid_data[0]);
			}
			if(HekrModuleStateUpdate == temp)
			{
				//���յ����ݱ����� ModuleStatus ָ����
				//User Code
				UART1_SendChar(ModuleStatus->CMD);
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
	

