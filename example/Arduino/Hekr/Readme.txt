example ���� arduino ��ʹ��HekrЭ���ʾ��
��arduino IDE 1.6.5 ����ͨ��ִ������

arduino ���ļ�������.c ������.cpp
���Copy���������ڵ�Э�� ��Ҫ�޸ĺ�׺ ����ͨ��


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
//�������鳤��Ӧ�����û����ݳ��ȼ���HEKR_DATA_LEN
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
