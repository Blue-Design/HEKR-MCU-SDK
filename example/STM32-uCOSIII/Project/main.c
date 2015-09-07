#include "stm32f10x.h"                  // Device header
#include "uart.h"
#include "includes.h"
#include "task.h"
#include "LED.h"
#include "delay.h"

void BSP_init(void);
 
int main()
{  
	OS_ERR err;
	CPU_SR_ALLOC();
	BSP_init();
	
	OSInit(&err); 
	OS_CRITICAL_ENTER();
	OSTaskCreate((OS_TCB 	* )&StartTaskTCB,		//������ƿ�
				 (CPU_CHAR	* )"start task", 		//��������
                 (OS_TASK_PTR )start_task, 			//������
                 (void		* )0,					//���ݸ��������Ĳ���
                 (OS_PRIO	  )START_TASK_PRIO,     //�������ȼ�
                 (CPU_STK   * )&START_TASK_STK[0],	//�����ջ����ַ
                 (CPU_STK_SIZE)START_STK_SIZE/10,	//�����ջ�����λ
                 (CPU_STK_SIZE)START_STK_SIZE,		//�����ջ��С
                 (OS_MSG_QTY  )0,					//�����ڲ���Ϣ�����ܹ����յ������Ϣ��Ŀ,Ϊ0ʱ��ֹ������Ϣ
                 (OS_TICK	  )0,					//��ʹ��ʱ��Ƭ��תʱ��ʱ��Ƭ���ȣ�Ϊ0ʱΪĬ�ϳ��ȣ�
                 (void   	* )0,					//�û�����Ĵ洢��
                 (OS_OPT      )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR, //����ѡ��
                 (OS_ERR 	* )&err);				//��Ÿú�������ʱ�ķ���ֵ
	OS_CRITICAL_EXIT();
	OSStart(&err);
	
	while(1){}
}

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

void BSP_init(void)
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); 
	Uart3_init();
	delay_init();  
	LED_init();
	HekrInit(UART3_SendChar);
}


//	

