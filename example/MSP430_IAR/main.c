#include  <msp430x14x.h>
#include "hekr_protocol.h"

unsigned char RecvBuffer[20];
unsigned char Recv_STA = 0;
void SystemInit(void);
void SendChar(unsigned char ch);

void main(void)
{ 
    unsigned char temp,UserValidLen = 9;
    SystemInit();
    HekrInit(SendChar);
    HekrValidDataUpload(UserValidLen);
  	HekrModuleControl(HekrConfig);
    while(1)
    {                   
        LPM1;                                  //����LPM1ģʽ
        if(Recv_STA)
		{
			temp = HekrRecvDataHandle(RecvBuffer);
			if(ValidDataUpdate == temp)
			{
				//���յ����ݱ����� valid_data ������
				//User Code
				SendChar(valid_data[0]);
			}
			if(HekrModuleStateUpdate == temp)
			{
				//���յ����ݱ����� ModuleStatus ָ����
				//User Code.
				SendChar(ModuleStatus->CMD);
			}
			Recv_STA = 0;		
		}
    }
}

void SystemInit(void)
{
    /*�������г���ر����е�IO��*/
    P1DIR = 0XFF;P1OUT = 0XFF;
    P2DIR = 0XFF;P2OUT = 0XFF;
    P3DIR = 0XFF;P3OUT = 0XFF;
    P4DIR = 0XFF;P4OUT = 0XFF;
    P5DIR = 0XFF;P5OUT = 0XFF;
    P6DIR = 0XFF;P6OUT = 0XFF;
    
    WDTCTL = WDTPW + WDTHOLD;                 // �رտ��Ź�
    P6DIR |= BIT2;P6OUT |= BIT2;              //�رյ�ƽת��
    
    P3SEL |= 0x30;                            // ѡ��P3.4��P3.5��UARTͨ�Ŷ˿�
    ME1 |= UTXE0 + URXE0;                     // ʹ��USART0�ķ��ͺͽ���
    UCTL0 |= CHAR;                            // ѡ��8λ�ַ�
    UTCTL0 |= SSEL0;                          // UCLK = ACLK
    UBR00 = 0x03;                             // ������9600
    UBR10 = 0x00;                             //
    UMCTL0 = 0x4A;                            // Modulation
    UCTL0 &= ~SWRST;                          // ��ʼ��UART״̬��
    IE1 |= URXIE0;                            // ʹ��USART0�Ľ����ж�
    _EINT();                                  //��ȫ���ж�
    
}

void SendChar(unsigned char ch)
{
   while (!(IFG1 & UTXIFG0));         //�ȴ���ǰ���ַ��������
   TXBUF0 = ch;                       //���յ����ַ����ͳ�ȥ 
}

#pragma vector = UART0RX_VECTOR
__interrupt void UART0_RXISR(void)
{
  static unsigned char flag = 0,count = 0;
  LPM1_EXIT;                 //�˳��͹���ģʽ
  if(RXBUF0 == HEKR_FRAME_HEADER)
  {
      count = 0;
      flag = 1;
  }
  if(flag == 1)
  {
	RecvBuffer[count++] = RXBUF0;
	if(count > 4 && count >= RecvBuffer[1])
	{
		Recv_STA = 1;
		flag = 0;
		count = 0;
	}    
  }
}


