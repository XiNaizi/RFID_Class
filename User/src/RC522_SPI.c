#include <string.h>
#include <stdio.h>
#include "RC522_SPI.h" 


void RC522_GPIO_Init(void)
{
	
	GPIO_InitTypeDef GPIO_InitStructure;
  //�򿪴���GPIO��ʱ��
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB, ENABLE);
  
  //��RC522 rst����GPIO����Ϊ�������
  GPIO_InitStructure.GPIO_Pin = RC522_GPIO_RST_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(RC522_GPIO_RST_PORT, &GPIO_InitStructure);

  //��RC522 cs����GPIO����Ϊ�������
  GPIO_InitStructure.GPIO_Pin = RC522_SPI_CS_PIN;
  // GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  // GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(RC522_SPI_CS_PORT, &GPIO_InitStructure);

  //��RC522 SCk MOSI MISO������GPIO����Ϊ���츴��
  GPIO_InitStructure.GPIO_Pin = RC522_SPI_SCK_PIN|RC522_SPI_MOSI_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(RC522_SPI_SCK_PORT, &GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Pin = RC522_SPI_MISO_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(RC522_SPI_MISO_PORT, &GPIO_InitStructure);
	
	
	
}


//============================================================================= 
//�� �� ��:  SPI_RC522_SendByte() 
//�� �ܣ�    ʹ��SPI����һ���ֽڵ�����
//�� ���� 	 byte��Ҫ���͵�����
//�� �� ֵ�� ���ؽ��յ�������
//============================================================================= 
uint8_t SPI_RC522_SendByte(uint8_t byte)
{
	unsigned char SPICount;                                       // Counter used to clock out the data

  for(SPICount = 0; SPICount < 8; SPICount++)
  {
    if (byte & 0x80)
    {
      SET_SPI_MOSI;
    }
    else
    {
      CLR_SPI_MOSI;
    } 
 
    SET_SPI_CK;   
    CLR_SPI_CK;

    
    byte <<= 1;
  } 
	return 0;
}
//============================================================================= 
//�� �� ��:  SPI_RC522_ReadByte() 
//�� �ܣ�    ʹ��SPI��ȡһ���ֽڵ�����
//�� ���� 	 ��
//�� �� ֵ�� ���ؽ��յ�������
//============================================================================= 
uint8_t SPI_RC522_ReadByte(void)
{
    unsigned char SPICount;                                       // Counter used to clock out the data
  unsigned char SPIData;                  

  SPIData = 0;
  for (SPICount = 0; SPICount < 8; SPICount++)                  // Prepare to clock in the data to be read
  {
    SPIData <<=1;                                               // Rotate the data
    SET_SPI_CK;
    if(STU_SPI_MISO)
    {
      SPIData|=0x01;
    }                                         
 		CLR_SPI_CK;
  }                                                             // and loop back
  return (SPIData);                              // Finally return the read data
}


/**
  * @brief  ��RC522�Ĵ���
  * @param  ucAddress���Ĵ�����ַ
  * @retval �Ĵ����ĵ�ǰֵ
  */
uint8_t ReadRawRC ( uint8_t ucAddress )
{
	uint8_t ucAddr, ucReturn;
	
	ucAddr = ( ( ucAddress << 1 ) & 0x7E ) | 0x80;	
	RC522_CS_Enable();
  
	SPI_RC522_SendByte ( ucAddr );
	ucReturn = SPI_RC522_ReadByte ();
  
	RC522_CS_Disable();
	
	return ucReturn;	
}

/**
  * @brief  дRC522�Ĵ���
  * @param  ucAddress���Ĵ�����ַ
  * @param  ucValue��д��Ĵ�����ֵ
  * @retval ��
  */
void WriteRawRC ( uint8_t ucAddress, uint8_t ucValue )
{  
	uint8_t ucAddr;
	
	ucAddr = ( ucAddress << 1 ) & 0x7E;	
	RC522_CS_Enable();
	
	SPI_RC522_SendByte ( ucAddr );	
	SPI_RC522_SendByte ( ucValue );
  
	RC522_CS_Disable();		
}


/**
  * @brief  ��RC522�Ĵ�����λ
  * @param  ucReg���Ĵ�����ַ
  * @param  ucMask����λֵ
  * @retval ��
  */
void SetBitMask ( uint8_t ucReg, uint8_t ucMask )  
{
  uint8_t ucTemp;

  ucTemp = ReadRawRC ( ucReg );
  WriteRawRC ( ucReg, ucTemp | ucMask ); // set bit mask
}


/**
  * @brief  ��RC522�Ĵ�����λ
  * @param  ucReg���Ĵ�����ַ
  * @param  ucMask����λֵ
  * @retval ��
  */
void ClearBitMask ( uint8_t ucReg, uint8_t ucMask )  
{
  uint8_t ucTemp;

  ucTemp = ReadRawRC ( ucReg );
  WriteRawRC ( ucReg, ucTemp & ( ~ ucMask) ); // clear bit mask
}



/*********************************************END OF FILE**********************/
