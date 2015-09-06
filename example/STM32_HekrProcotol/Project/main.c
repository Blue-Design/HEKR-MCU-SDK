#include "stm32f10x.h"                  // Device header
#include "uart.h"
#include "hekr_protocol.h"

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

u8 DateHandleFlag = 0;

void System_Init(void);

int main()
{
	u8 temp;
	u8 UserValidLen = 9;
	System_Init();
	HekrInit(UART3_SendChar);
	
	HekrValidDataUpload(UserValidLen);
  	HekrModuleControl(HekrConfig);
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
				//���յ����ݱ����� valid_data ������
				//User Code
				UART3_SendChar(valid_data[0]);
			}
			if(HekrModuleStateUpdate == temp)
			{
				//���յ����ݱ����� ModuleStatus ָ����
				//User Code.
				UART3_SendChar(ModuleStatus->CMD);
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

