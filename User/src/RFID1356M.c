#include <string.h>
#include <stdio.h>
#include "rc522_spi.h"
#include "rfid1356M.h" 
unsigned char PassWd[6]={0xff,0xff,0xff,0xff,0xff,0xff};
unsigned char RWDATA[16];

unsigned char  IDB[10];//����B ��ID
unsigned char  IDA[10];//����A ��ID
unsigned char CT[2];//������

void delay_us(uint16_t   us)
{
	uint16_t i;
	while(us--)
		for(i=0;i<72;i++);
}

void delay_ms(uint32_t   ms)
{
	uint8_t i;
	for(i=0;i<ms;i++)	
		delay_us(1000);
}

/////////////////////////////////////////////////////////////////////
//��    �ܣ���֤ͨѶ�Ƿ�ɹ�
//����˵����cnt[IN]:���Ĵ�������
//��    �أ��ɹ�����MI_OK��ʧ�ܷ���MI_ERR
/////////////////////////////////////////////////////////////////////
unsigned char Test_Communication(unsigned char cnt)
{
	unsigned char i,j;
  SET_NFC_RST;
  delay_us(200);
	for(i=0;( i<cnt && (j == 0x12 || j == 0x15) ) ;i++)
	{	
	delay_us(100);	
	j=ReadRawRC(0X37); 
	}
	if( j == 0x12 || j == 0x15 )
		return MI_OK;
	else
		return MI_ERR;
}




/////////////////////////////////////////////////////////////////////
//��    �ܣ���λоƬ
//��    ��: �ɹ�����MI_OK
/////////////////////////////////////////////////////////////////////
void PcdReset(void)
{

  delay_us(40);
#if 1
  SET_NFC_RST;
  delay_us(2);
  CLR_NFC_RST;
  delay_us(2);
  SET_NFC_RST;
  delay_us(2);
#endif 
  delay_us(500); //delay 500us ��ͻ�ȷ��delayus������ʱ׼ȷ��
  
  WriteRawRC(CommandReg,PCD_RESETPHASE);//��λ
  delay_us(2);

  WriteRawRC(ModeReg,0x3D); //��Mifare��ͨѶ��CRC��ʼֵ0x6363

  WriteRawRC(TReloadRegL,30);  //��װ��ʱ��ֵ��λ        
  WriteRawRC(TReloadRegH,0);//��װ��ʱ��ֵ��λ
  WriteRawRC(TModeReg,0x8D);//����Э��������ֹͣ
  WriteRawRC(TPrescalerReg,0x3E);//6.78/3390,15ms�����ж�

  WriteRawRC(TxASKReg,0x40);//����Ҫ
 // ClearBitMask(TestPinEnReg,0x80);//off MX and DTRQ out
}

/////////////////////////////////////////////////////////////////////
//��������  
//ÿ��������ر����շ���֮��Ӧ������1ms�ļ��
/////////////////////////////////////////////////////////////////////
void PcdAntennaOn()
{
	WriteRawRC(TxControlReg, ReadRawRC(TxControlReg) | 0x03); //Tx1RFEn=1 Tx2RFEn=1
}

/////////////////////////////////////////////////////////////////////
//�ر�����
/////////////////////////////////////////////////////////////////////
void PcdAntennaOff(void)
{
  WriteRawRC(TxControlReg, ReadRawRC(TxControlReg) & (~0x03));
}


/**
 ****************************************************************
 * @brief pcd_config() 
 *
 * ����оƬ��A/Bģʽ
 *
 * @param: u8 type   
 * @return: 
 * @retval: 
 ****************************************************************
 */

void PcdConfig(char type)
{
	  CLR_NFC_RST;
    delay_us(500);
	  SET_NFC_RST;
    delay_us(500);
		PcdAntennaOff();
		delay_us(500);

		if ('A' == type)
		{
				ClearBitMask(Status2Reg, BIT3);
				ClearBitMask(ComIEnReg, BIT7); // �ߵ�ƽ
				WriteRawRC(ModeReg,0x3D);	// 11 // CRC seed:6363
				WriteRawRC(RxSelReg, 0x86);//RxWait
				WriteRawRC(RFCfgReg, 0x58); // 
				WriteRawRC(TxASKReg, 0x40);//15  //typeA
				WriteRawRC(TxModeReg, 0x00);//12 //Tx Framing A
				WriteRawRC(RxModeReg, 0x00);//13 //Rx framing A
				WriteRawRC(0x0C, 0x10);	//^_^
	
			//��������
			{
					unsigned char backup;
					backup = ReadRawRC(0x37);
					WriteRawRC(0x37, 0x00);	
				{
					// ���¼Ĵ������밴˳������
					WriteRawRC(0x37, 0x5E);
					WriteRawRC(0x26, 0x48);
					WriteRawRC(0x17, 0x88);
					WriteRawRC(0x29, 0x12);//0x0F); //����ָ��	
					WriteRawRC(0x35, 0xED);
					WriteRawRC(0x3b, 0xA5);
					WriteRawRC(0x37, 0xAE);
					WriteRawRC(0x3b, 0x72);	
				}
				WriteRawRC(0x27, 0xf0);//RxWait
				WriteRawRC(0x28, 0x3f); // 
				WriteRawRC(0x37, backup);
			}		
		}
		else if ('B' == type)
		{
				WriteRawRC(Status2Reg, 0x00);	//��MFCrypto1On
				ClearBitMask(ComIEnReg, BIT7);// �ߵ�ƽ�����ж�
				WriteRawRC(ModeReg, 0x3F);	// CRC seed:FFFF
				WriteRawRC(RxSelReg, 0x85);	//RxWait
				WriteRawRC(RFCfgReg, 0x58);	//	
				//Tx
				WriteRawRC(GsNReg, 0xF8);	//����ϵ��
				WriteRawRC(CWGsPReg, 0x3F);	// 
				WriteRawRC(ModGsPReg, 0x20);	//����ָ��
				WriteRawRC(AutoTestReg, 0x00);
				WriteRawRC(TxASKReg, 0x00);	// typeB
				WriteRawRC(TypeBReg, 0x13);
				WriteRawRC(TxModeReg, 0x83);	//Tx Framing B
				WriteRawRC(RxModeReg, 0x83);	//Rx framing B
				WriteRawRC(BitFramingReg, 0x00);	//TxLastBits=0
	
			//��������
			{
					unsigned char backup;
					backup = ReadRawRC(0x37);
					WriteRawRC(0x37, 0x00);
				{	
						WriteRawRC(0x37, 0x5E);
						WriteRawRC(0x26, 0x48);
						WriteRawRC(0x17, 0x88);
						WriteRawRC(0x29, 0x20);
						WriteRawRC(0x35, 0xED);
						WriteRawRC(0x3b, 0xA5);
						WriteRawRC(0x37, 0xAE);
						WriteRawRC(0x3b, 0x72);
				}
				WriteRawRC(0x37, backup);
			}
		}
		else
		{
//			return USER_ERROR;
		}
		
		PcdAntennaOn();
		delay_us(100);
		
}	


/////////////////////////////////////////////////////////////////////
//����PCD��ʱ��
//input:fwi=0~15
/////////////////////////////////////////////////////////////////////
void PcdSetTmo(unsigned char fwi)

{
	WriteRawRC(TPrescalerReg, (TP_FWT_302us) & 0xFF);
	WriteRawRC(TModeReg, BIT7 | (((TP_FWT_302us)>>8) & 0xFF));

	WriteRawRC(TReloadRegL, (1 << fwi)  & 0xFF);
	WriteRawRC(TReloadRegH, ((1 << fwi)  & 0xFF00) >> 8);
}

////////////////////////////////////////////////////////////////////////

void CheckIrq(void)
{
#if 1
unsigned char waitFor = 0x00; 
unsigned char n;
unsigned int i; 
 i = 900;//�����Լ�����ʱ��
  do 
  {
    n = ReadRawRC(ComIrqReg);  //?????????
    i--;
  }
 while ((i!=0) && !(n&0x01) && !(n&waitFor));
#endif 
 
#if 0
delay_ms(2);
#endif 
 
} 

///////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////
////��    �ܣ���ISO14443��ͨѶ
////����˵����Command[IN]:������
////          pInData[IN]:���͵���Ƭ������
////          InLenByte[IN]:�������ݵ��ֽڳ���
////          pOutData[OUT]:���յ��Ŀ�Ƭ��������
////          *pOutLenBit[OUT]:�������ݵ�λ����
///////////////////////////////////////////////////////////////////////
unsigned char PcdComMF522(signed char Command,
								unsigned char *pInData,
								unsigned char InLenByte,
								unsigned char *pOutData,
								unsigned int *pOutLenBit)
{
	unsigned char recebyte=0;
	unsigned char val;
	unsigned char err=0;	
	unsigned char irq_inv;
	unsigned char len_rest=0;
	unsigned char len=0;
	unsigned char status;
	unsigned char irqEn   = 0x00;
	unsigned char waitFor = 0x00;
	unsigned char lastBits;
	u16 i;

	switch (Command)
	{
	  case PCD_IDLE:
	     irqEn   = 0x00;
	     waitFor = 0x00;
	     break;
	  case PCD_AUTHENT:    
		irqEn = IdleIEn | TimerIEn;
		waitFor = IdleIRq;
		break;
	  case PCD_RECEIVE:
	     irqEn   = RxIEn | IdleIEn;
	     waitFor = RxIRq;
	     recebyte=1;
	     break;
	  case PCD_TRANSMIT:
	     irqEn   = TxIEn | IdleIEn;
	     waitFor = TxIRq;
	     break;
	  case PCD_TRANSCEIVE:   
		 irqEn = RxIEn | IdleIEn | TimerIEn | TxIEn;
	     waitFor = RxIRq;
	     recebyte=1;
	     break;
	  default:
	     Command = MI_UNKNOWN_COMMAND;
	     break;
	}
   
	if (Command != MI_UNKNOWN_COMMAND
		&& (((Command == PCD_TRANSCEIVE || Command == PCD_TRANSMIT) && InLenByte > 0)
		|| (Command != PCD_TRANSCEIVE && Command != PCD_TRANSMIT))
		)
	{		
		WriteRawRC(CommandReg, PCD_IDLE);
		
		irq_inv = ReadRawRC(ComIEnReg) & BIT7;
		WriteRawRC(ComIEnReg, irq_inv |irqEn | BIT0);//ʹ��Timer ��ʱ���ж�
		WriteRawRC(ComIrqReg, 0x7F); //Clear INT
		WriteRawRC(DivIrqReg, 0x7F); //Clear INT
		//Flush Fifo
		SetBitMask(FIFOLevelReg, BIT7);
		if (Command == PCD_TRANSCEIVE || Command == PCD_TRANSMIT || Command == PCD_AUTHENT)
		{
			len_rest = InLenByte;
			if (len_rest >= FIFO_SIZE)
			{
				len = FIFO_SIZE;
			}else
			{
				len = len_rest;
			}
			
			for (i = 0; i < len; i++)
			{
				WriteRawRC(FIFODataReg, pInData[i]);
			}
			len_rest -= len;//Rest bytes
			if (len_rest != 0)
			{
				WriteRawRC(ComIrqReg, BIT2); // clear LoAlertIRq
				SetBitMask(ComIEnReg, BIT2);// enable LoAlertIRq
			}

			WriteRawRC(CommandReg, Command);
			if (Command == PCD_TRANSCEIVE)
		    {    
				SetBitMask(BitFramingReg,0x80);  
			}
		
			while (len_rest != 0)
			{
				 CheckIrq();
			  	//delay_ms(2);		
				if (len_rest > (FIFO_SIZE - WATER_LEVEL))
				{
					len = FIFO_SIZE - WATER_LEVEL;
				}
				else
				{
					len = len_rest;
				}
				for (i = 0; i < len; i++)
				{
					WriteRawRC(FIFODataReg, pInData[InLenByte - len_rest + i]);
				}

				WriteRawRC(ComIrqReg, BIT2);//��write fifo֮��������жϱ�ǲſ���
				len_rest -= len;//Rest bytes
				if (len_rest == 0)
				{
					ClearBitMask(ComIEnReg, BIT2);// disable LoAlertIRq
				}	
			}
			//Wait TxIRq
			CheckIrq();
			  	//delay_ms(2);
                        //while (INT_PIN == 0);
			val = ReadRawRC(ComIrqReg);
			if (val & TxIRq)
			{
				WriteRawRC(ComIrqReg, TxIRq);
			}
		}
		if (PCD_RECEIVE == Command)
		{	
			SetBitMask(ControlReg, BIT6);// TStartNow
		}
	
		len_rest = 0; // bytes received
		WriteRawRC(ComIrqReg, BIT3); // clear HoAlertIRq
		SetBitMask(ComIEnReg, BIT3); // enable HoAlertIRq
	
		CheckIrq();
			  	//delay_ms(2);
	
		while(1)
		{
      CheckIrq();
			  	//delay_ms(2);
			val = ReadRawRC(ComIrqReg);
			if ((val & BIT3) && !(val & BIT5))
			{
				if (len_rest + FIFO_SIZE - WATER_LEVEL > 255)
				{
					break;
				}
		    for (i = 0; i <FIFO_SIZE - WATER_LEVEL; i++)
		    {
					pOutData[len_rest + i] = ReadRawRC(FIFODataReg);
		    }
				WriteRawRC(ComIrqReg, BIT3);//��read fifo֮��������жϱ�ǲſ���
				len_rest += FIFO_SIZE - WATER_LEVEL; 
			}
			else
			{
				ClearBitMask(ComIEnReg, BIT3);//disable HoAlertIRq
				break;
			}			
		}

		val = ReadRawRC(ComIrqReg);

		WriteRawRC(ComIrqReg, val);// ���ж�
		
		if (val & BIT0)
		{//������ʱ
			status = MI_NOTAGERR;
		}
		else
		{
			err = ReadRawRC(ErrorReg);
			
			status = MI_COM_ERR;
			if ((val & waitFor) && (val & irqEn))
			{
				if (!(val & ErrIRq))
				 {//ָ��ִ����ȷ
				    status = MI_OK;

				    if (recebyte)
				    {
						val = 0x7F & ReadRawRC(FIFOLevelReg);
				      	lastBits = ReadRawRC(ControlReg) & 0x07;
						if (len_rest + val > MAX_TRX_BUF_SIZE)
						{//���ȹ�����������
							status = MI_COM_ERR;
						}
						else
						{	
							if (lastBits && val) //��ֹspi����� val-1��Ϊ��ֵ
							{
								*pOutLenBit = (val-1)*8 + lastBits;
							}
							else
							{
								*pOutLenBit = val*8;
							}
							*pOutLenBit += len_rest*8;


							if (val == 0)
							{
								val = 1;
							}
							for (i = 0; i < val; i++)
							{
								pOutData[len_rest + i] = ReadRawRC(FIFODataReg);
							}					
						}
				   }
				 }					
				 else if ((err & CollErr) && (!(ReadRawRC(CollReg) & BIT5)))
				 {//a bit-collision is detected				 	
				    status = MI_COLLERR;
				    if (recebyte)
				    {
								val = 0x7F & ReadRawRC(FIFOLevelReg);
				      	lastBits = ReadRawRC(ControlReg) & 0x07;
						if (len_rest + val > MAX_TRX_BUF_SIZE)
						{//���ȹ�����������
							;
						}
						else
						{
					     if (lastBits && val) //��ֹspi����� val-1��Ϊ��ֵ
					     {
					        *pOutLenBit = (val-1)*8 + lastBits;
					     }
					     else
					     {
					        *pOutLenBit = val*8;
					     }		
							*pOutLenBit += len_rest*8;
					     if (val == 0)
					     {
					        val = 1;
					     }
							for (i = 0; i < val; i++)
					    {
								pOutData[len_rest + i +1] = ReadRawRC(FIFODataReg);				
					    }				
						}
				    }
					pOutData[0] = (ReadRawRC(CollReg) & CollPos);
					if (pOutData[0] == 0)
					{
						pOutData[0] = 32;
					}
				
					pOutData[0]--;// ��֮ǰ�汾�е�ӳ������Ϊ�˲��ı��ϲ���룬����ֱ�Ӽ�һ��

				}
				else if ((err & CollErr) && (ReadRawRC(CollReg) & BIT5))
				{
					;		
				}
				//else if (err & (CrcErr | ParityErr | ProtocolErr))
				else if (err & (ProtocolErr))
				{
					status = MI_FRAMINGERR;				
				}
				else if ((err & (CrcErr | ParityErr)) && !(err &ProtocolErr))
				{
					//EMV  parity err EMV 307.2.3.4		
					val = 0x7F & ReadRawRC(FIFOLevelReg);
			      	lastBits = ReadRawRC(ControlReg) & 0x07;
					if (len_rest + val > MAX_TRX_BUF_SIZE)
					{//���ȹ�����������
						status = MI_COM_ERR;
					}
					else
					{
				        if (lastBits && val)
				        {
				           *pOutLenBit = (val-1)*8 + lastBits;
				        }
				        else
				        {
				           *pOutLenBit = val*8;
				        }
						*pOutLenBit += len_rest*8;
					}
					status = MI_INTEGRITY_ERR;
				}				
				else
				{
					status = MI_INTEGRITY_ERR;
				}
			}
			else
			{   
				status = MI_COM_ERR;
			}
		}	
 		SetBitMask(ControlReg, BIT7);// TStopNow =1,��Ҫ�ģ�
		WriteRawRC(ComIrqReg, 0x7F);// ���ж�0
		WriteRawRC(DivIrqReg, 0x7F);// ���ж�1
		ClearBitMask(ComIEnReg, 0x7F);//���ж�ʹ��,���λ�ǿ���λ
		ClearBitMask(DivIEnReg, 0x7F);//���ж�ʹ��,���λ�ǿ���λ
		WriteRawRC(CommandReg, PCD_IDLE);
	}
	else
	{
		status = USER_ERROR;
	}
	return status;
}
                      




/////////////////////////////////////////////////////////////////////
//��    �ܣ�Ѱ��
//����˵��: req_code[IN]:Ѱ����ʽ
//                0x52 = Ѱ��Ӧ�������з���14443A��׼�Ŀ�
//                0x26 = Ѱδ��������״̬�Ŀ�
//          pTagType[OUT]����Ƭ���ʹ���
//                0x4400 = Mifare_UltraLight
//                0x0400 = Mifare_One(S50)
//                0x0200 = Mifare_One(S70)
//                0x0800 = Mifare_Pro(X)
//                0x4403 = Mifare_DESFire
//��    ��: �ɹ�����MI_OK
/////////////////////////////////////////////////////////////////////
unsigned char PcdRequest(unsigned char req_code,unsigned char *pTagType)
{	
   unsigned char status; 
   unsigned char	i;
   unsigned int  unLen;
   unsigned char ucComMF522Buf[MAXRLEN]; 

   ClearBitMask(Status2Reg,0x08);
   WriteRawRC(BitFramingReg,0x07);
   SetBitMask(TxControlReg,0x03);   
	 i=ReadRawRC(0X37);
	if(i == 0x12 || i == 0x15)
	 {
	 delay_ms(3);
	 }
	
   ucComMF522Buf[0] = req_code;

   status = PcdComMF522(PCD_TRANSCEIVE,ucComMF522Buf,1,ucComMF522Buf,&unLen);
//   UART_send_byte(status);
   if ((status == MI_OK) && (unLen == 0x10))
   {    
       *pTagType     = ucComMF522Buf[0];
       *(pTagType+1) = ucComMF522Buf[1];
   }
   else
   {   status = MI_ERR;   }
   
   return status;
}

unsigned char PcdFastSearchCard(void)
{
  unsigned char errmask = 0x30;
  unsigned char errflag;
  unsigned char times;
#if 1
  SET_NFC_RST;
	delay_us(500); //delay 
 #endif  

  
	//WriteRawRC(0x27,0x10);
	//WriteRawRC(0x28,0x0f);
	WriteRawRC(TxASKReg,0x40);//100%ASK����
  WriteRawRC(ComIrqReg,0x14); //
	WriteRawRC(TxControlReg,0x83); //T1��T2���;����������Ƶ��ز��������ز�0X83
  WriteRawRC(FIFODataReg,PICC_REQIDL); //�Ƚ�����д��fifo  0X26
  WriteRawRC(CommandReg, PCD_TRANSCEIVE);//ִ�в�������д�뿨��
  WriteRawRC(BitFramingReg,0x87);//�������ݷ���
  times = 15;
  do 
  {
    errflag = ReadRawRC(ComIrqReg);  //�ж��Ƿ����жϷ���
    times--;
  }
  while ((times!=0)&& !(errflag&errmask));  
  if (times==0) 
  {	
    return MI_ERR;
  }
  
  return MI_OK;
  
}


/////////////////////////////////////////////////////////////////////
//��    �ܣ�����ײ
//����˵��: pSnr[OUT]:��Ƭ���кţ�4�ֽ�
//��    ��: �ɹ�����MI_OK
/////////////////////////////////////////////////////////////////////  
unsigned char PcdAnticoll(unsigned char *pSnr)
{
    unsigned char status;
    unsigned char i,snr_check=0;
    unsigned int  unLen;
    unsigned char ucComMF522Buf[MAXRLEN]; 
    

    ClearBitMask(Status2Reg,0x08);
    WriteRawRC(BitFramingReg,0x00);
    ClearBitMask(CollReg,0x80);
 
    ucComMF522Buf[0] = PICC_ANTICOLL1;
    ucComMF522Buf[1] = 0x20;

    status = PcdComMF522(PCD_TRANSCEIVE,ucComMF522Buf,2,ucComMF522Buf,&unLen);

    if (status == MI_OK)
    {
    	 for (i=0; i<4; i++)
         {   
             *(pSnr+i)  = ucComMF522Buf[i];
             snr_check ^= ucComMF522Buf[i];
         }
         if (snr_check != ucComMF522Buf[i])
         {   status = MI_ERR;    }
    }
    
    SetBitMask(CollReg,0x80);
    return status;
}

/////////////////////////////////////////////////////////////////////
//��    �ܣ�ѡ����Ƭ
//����˵��: pSnr[IN]:��Ƭ���кţ�4�ֽ�
//��    ��: �ɹ�����MI_OK
/////////////////////////////////////////////////////////////////////
unsigned char PcdSelect(unsigned char *pSnr)
{
    unsigned char status;
    unsigned char i;
    unsigned int  unLen;
    unsigned char ucComMF522Buf[MAXRLEN]; 
    
    ucComMF522Buf[0] = PICC_ANTICOLL1;
    ucComMF522Buf[1] = 0x70;
    ucComMF522Buf[6] = 0;
    for (i=0; i<4; i++)
    {
    	ucComMF522Buf[i+2] = *(pSnr+i);
    	ucComMF522Buf[6]  ^= *(pSnr+i);
    }
    CalulateCRC(ucComMF522Buf,7,&ucComMF522Buf[7]);
  
    ClearBitMask(Status2Reg,0x08);

    status = PcdComMF522(PCD_TRANSCEIVE,ucComMF522Buf,9,ucComMF522Buf,&unLen);
    
    if ((status == MI_OK) && (unLen == 0x18))
    {   status = MI_OK;  }
    else
    {   status = MI_ERR;    }

    return status;
}

/////////////////////////////////////////////////////////////////////
//��    �ܣ���֤��Ƭ����
//����˵��: auth_mode[IN]: ������֤ģʽ
//                 0x60 = ��֤A��Կ
//                 0x61 = ��֤B��Կ 
//          addr[IN]�����ַ
//          pKey[IN]������
//          pSnr[IN]����Ƭ���кţ�4�ֽ�
//��    ��: �ɹ�����MI_OK
/////////////////////////////////////////////////////////////////////                   
unsigned char PcdAuthState(unsigned char auth_mode,unsigned char addr,unsigned char *pKey,unsigned char *pSnr)
{
    unsigned char status;
    unsigned int  unLen;
    unsigned char i,ucComMF522Buf[MAXRLEN]; 

    ucComMF522Buf[0] = auth_mode;
    ucComMF522Buf[1] = addr;
    for (i=0; i<6; i++)
    {    ucComMF522Buf[i+2] = *(pKey+i);   }
    for (i=0; i<6; i++)
    {    ucComMF522Buf[i+8] = *(pSnr+i);   }
    
    status = PcdComMF522(PCD_AUTHENT,ucComMF522Buf,12,ucComMF522Buf,&unLen);
    if ((status != MI_OK) || (!(ReadRawRC(Status2Reg) & 0x08)))
    {   //status = MI_ERR;   
		    status = MI_AUTHERR;
		}
    
    return status;
}

/////////////////////////////////////////////////////////////////////
//��    �ܣ���ȡM1��һ������
//����˵��: addr[IN]�����ַ
//          p [OUT]�����������ݣ�16�ֽ�
//��    ��: �ɹ�����MI_OK
///////////////////////////////////////////////////////////////////// 
unsigned char PcdRead(unsigned char addr,unsigned char *pData)
{
    u8 status;
    unsigned int  unLen;
    unsigned char i,ucComMF522Buf[MAXRLEN]; 

    ucComMF522Buf[0] = PICC_READ;
    ucComMF522Buf[1] = addr;
    CalulateCRC(ucComMF522Buf,2,&ucComMF522Buf[2]);
   
    status = PcdComMF522(PCD_TRANSCEIVE,ucComMF522Buf,4,ucComMF522Buf,&unLen);
    if ((status == MI_OK) && (unLen == 0x90))
    {
        for (i=0; i<16; i++)
        {    *(pData+i) = ucComMF522Buf[i];   }
    }
    else
    {  // status = MI_ERR; 
        status =  MI_READERR;
		}
    
    return status;
}

/////////////////////////////////////////////////////////////////////
//��    �ܣ�д���ݵ�M1��һ��
//����˵��: addr[IN]�����ַ
//          p [IN]��д������ݣ�16�ֽ�
//��    ��: �ɹ�����MI_OK
/////////////////////////////////////////////////////////////////////                 
unsigned char PcdWrite(unsigned char addr,unsigned char *pData)
{
    unsigned char status;
    unsigned int  unLen;
    unsigned char i,ucComMF522Buf[MAXRLEN]; 
    
    ucComMF522Buf[0] = PICC_WRITE;
    ucComMF522Buf[1] = addr;
    CalulateCRC(ucComMF522Buf,2,&ucComMF522Buf[2]);
 
    status = PcdComMF522(PCD_TRANSCEIVE,ucComMF522Buf,4,ucComMF522Buf,&unLen);

    if ((status != MI_OK) || (unLen != 4) || ((ucComMF522Buf[0] & 0x0F) != 0x0A))
    {   status = MI_ERR;   }
        
    if (status == MI_OK)
    {
        for (i=0; i<16; i++)
        {    ucComMF522Buf[i] = *(pData+i);   }
        CalulateCRC(ucComMF522Buf,16,&ucComMF522Buf[16]);

        status = PcdComMF522(PCD_TRANSCEIVE,ucComMF522Buf,18,ucComMF522Buf,&unLen);
        if ((status != MI_OK) || (unLen != 4) || ((ucComMF522Buf[0] & 0x0F) != 0x0A))
        {   //status = MI_ERR; 
            status =MI_WRITEERR;
				}
    }
    
    return status;
}



/////////////////////////////////////////////////////////////////////
//��MF522����CRC16����
/////////////////////////////////////////////////////////////////////
void CalulateCRC(unsigned char *pIndata,unsigned char len,unsigned char *pOutData)
{
    unsigned char i,n;
    ClearBitMask(DivIrqReg,0x04);
    WriteRawRC(CommandReg,PCD_IDLE);
    SetBitMask(FIFOLevelReg,0x80);
    for (i=0; i<len; i++)
    {   WriteRawRC(FIFODataReg, *(pIndata+i));   }
    WriteRawRC(CommandReg, PCD_CALCCRC);
    i = 0xFF;
    do 
    {
        n = ReadRawRC(DivIrqReg);
        i--;
    }
    while ((i!=0) && !(n&0x04));
    pOutData[0] = ReadRawRC(CRCResultRegL);
    pOutData[1] = ReadRawRC(CRCResultRegM);
}

void M500PcdConfigISOTypeA(void)//ISO14443_A//A:NXP,B:MOTO
{
  ClearBitMask(Status2Reg,0x08);//����Ĵ���
  WriteRawRC(ModeReg,0x3D);//3F ѡ��ģʽ
  WriteRawRC(RxSelReg,0x86);//84  �ڲ���������
  WriteRawRC(RFCfgReg,0x7F);   //4F  ��������
  WriteRawRC(TReloadRegL,30);//tmoLength);// TReloadVal = 'h6a =tmoLength(dec) 
  WriteRawRC(TReloadRegH,0);
  WriteRawRC(TModeReg,0x8D);
  WriteRawRC(TPrescalerReg,0x3E);

  delay_us(400);
  PcdAntennaOn();
}




void	RFID1356M_init ( void )
{
	RC522_GPIO_Init();
	PcdReset ();
	PcdAntennaOff();
	delay_ms(1);
  PcdAntennaOn();
  /*���ù�����ʽ*/
	M500PcdConfigISOTypeA();
}


/**
  * @brief  ��ȡIC�����ݣ�����15����60 61  62 
  * @param  pName��������16�ֽ�
	* @param  pNum�����ţ�16�ֽ�
	* @param  pPart�����ţ�16�ֽ�
  * @retval ״ֵ̬= MI_OK���ɹ�
  */
char IC_read( uint8_t * pName,uint8_t * pNum,uint8_t * pPart)//  uint8_t *pkey)
{
  char cStatus = 0;
	
  uint8_t ucArray_ID [ 4 ];  /*�Ⱥ���IC�������ͺ�UID(IC�����к�)*/      
	uint8_t ucStatusReturn;    /*����״̬*/                                                                                         
	uint8_t pkey[6]={0xff,0xff,0xff,0xff,0xff,0xff};
	
		/*Ѱ��*/
		if ( ( ucStatusReturn = PcdRequest ( PICC_REQALL, ucArray_ID ) ) != MI_OK )  
      /*��ʧ���ٴ�Ѱ��*/
			ucStatusReturn = PcdRequest ( PICC_REQALL, ucArray_ID );		 
		
		if ( ucStatusReturn == MI_OK  )
		{
      /*����ײ�����ж��ſ������д��������Χʱ������ͻ���ƻ������ѡ��һ�Ž��в�����*/
			if ( PcdAnticoll ( ucArray_ID ) == MI_OK )                                                                   
			{
				/*ѡ����Ƭ*/
				if(PcdSelect(ucArray_ID)==MI_OK)
				{
					if(PcdAuthState ( PICC_AUTHENT1A, 63, pkey,ucArray_ID )==MI_OK)
					{
							if(PcdRead(60,pName)==MI_OK && PcdRead(61,pNum)==MI_OK && PcdRead(62,pPart)==MI_OK)
									cStatus = USERCARD_OK;	
							else
								  cStatus = RC522_Read_Err;//��Ƭ��ȡʧ��

					}
					else
						cStatus = RC522_Auth_Err;	//��Ƭ��֤ʧ��
					
				}
							
			}	
			else
				cStatus = RC522_Anticoll_Err;	//���Ż�ȡʧ��
		}	
		else
			cStatus = RC522_Request_Err;		//δ��⵽IC��

  return cStatus;		
}

/*********************************************END OF FILE**********************/
