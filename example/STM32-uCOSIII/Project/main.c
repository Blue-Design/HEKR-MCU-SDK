#include "stm32f10x.h"                  // Device header
#include "uart.h"
#include "includes.h"
#include "task.h"
#include "LED.h"
#include "delay.h"

void BSP_init(void);

u8 UserValidLen = 9;
unsigned char ProdKey[16] = {0x01,0x36,0xA6,0x6C,0x12,0x75,0x4E,0xE8,0x2F,0xFF,0x88,0x04,0xB7,0xFA,0xA5,0x3C};         //��Ʒ��Կ����,��16���ֽ�,��ǰʾ��ProdKeyֵ��ӦLED��ʵ�����ҳ�档ProdKey�Ļ�ȡ�����÷����ɲο�:http://docs.hekr.me/v4/resourceDownload/protocol/uart/#44prodkey

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

void BSP_init(void)
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); 
	Uart3_init();
	delay_init();  
	LED_init();
	HekrInit(UART3_SendChar);
	
	/*************ģ���������************/
  HekrValidDataUpload(UserValidLen);
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
}


//	

