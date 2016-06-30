#include "protocol_task.h"
#include "task.h"
#include "uart.h"
#include "led.h"

//������ƿ�
OS_TCB ProtocolTaskTCB;
//�����ջ	
CPU_STK Protocol_TASK_STK[Protocol_STK_SIZE];

	
void protocol_task(void *p_arg)
{
	OS_ERR err;
	static uint8_t Date_Handle_Flag = 0;
	uint8_t temp = 0;
	p_arg = p_arg;
	while(1)
	{
		if(Uart3_Recv_STA && !Date_Handle_Flag)
		{
			Date_Handle_Flag = 1;
			Uart3_Recv_STA = 0;
		}
		if(Date_Handle_Flag)
		{
			temp = HekrRecvDataHandle(Uart3_Recv_Buffer);
			if(ValidDataUpdate == temp)
			{
				//���յĲ�Ʒҵ�����ݱ����� valid_data ������
				/************��Ʒҵ�����ݲ����û�����********/
				if(valid_data[0] == 0x01)
				{
					LED0 = 1;
					OSTaskSuspend((OS_TCB*)&Led0TaskTCB,&err);
					printf("����LED����\n\r");	
				}
				else
				{
					LED0 = 0;
					OSTaskResume((OS_TCB*)&Led0TaskTCB,&err);
					printf("�ָ�LED����\n\r");	
				}
				if(valid_data[1] == 0x01)
				{
					OSTaskDel((OS_TCB*)&FloatTaskTCB,&err);
					printf("ɾ��float����\n\r");	
				}
				else
				{
										//���������������
						OSTaskCreate((OS_TCB 	* )&FloatTaskTCB,		
									 (CPU_CHAR	* )"float test task", 		
													 (OS_TASK_PTR )float_task, 			
													 (void		* )0,					
													 (OS_PRIO	  )FLOAT_TASK_PRIO,     	
													 (CPU_STK   * )&FLOAT_TASK_STK[0],	
													 (CPU_STK_SIZE)FLOAT_STK_SIZE/10,	
													 (CPU_STK_SIZE)FLOAT_STK_SIZE,		
													 (OS_MSG_QTY  )0,					
													 (OS_TICK	  )0,					
													 (void   	* )0,				
													 (OS_OPT      )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR, 
													 (OS_ERR 	* )&err);		
				}
				/********************************************/
			}
			if(HekrModuleStateUpdate == temp)
			{
				//���յ�ģ��״̬ȡֵ������ ModuleStatus ָ����
				/*************ģ��״̬�����û�����************/
				printf("ModuleStatus CMD %d \n\r",ModuleStatus->Mode);                //��ӡģ�鹤��ģʽָʾ�ֽ�
				printf("ModuleStatus CMD %d \n\r",ModuleStatus->WIFI_Status);         //��ӡģ��WIFI״ָ̬ʾ�ֽ�
				printf("ModuleStatus CMD %d \n\r",ModuleStatus->CloudStatus);         //��ӡģ��������״ָ̬ʾ�ֽ�
				printf("ModuleStatus CMD %d \n\r",ModuleStatus->SignalStrength);      //��ӡģ��״̬��ѯӦ��֡�����ֽ�
				/*************ģ��״̬ȡֵ�ο�:http://docs.hekr.me/v4/resourceDownload/protocol/uart/#42**********/
			}
			Date_Handle_Flag = 0;		
		}
		OSTimeDlyHMSM(0,0,0,10,OS_OPT_TIME_HMSM_STRICT,&err); //��ʱ10ms
	}
}
