#include <string.h>
#include <stdio.h>
#include "RC522_SPI.h" 


void RC522_GPIO_Init(void)
{
	
	GPIO_InitTypeDef GPIO_InitStructure;
  //打开串口GPIO的时钟
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB, ENABLE);
  
  //将RC522 rst引脚GPIO配置为推挽输出
  GPIO_InitStructure.GPIO_Pin = RC522_GPIO_RST_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(RC522_GPIO_RST_PORT, &GPIO_InitStructure);

  //将RC522 cs引脚GPIO配置为推挽输出
  GPIO_InitStructure.GPIO_Pin = RC522_SPI_CS_PIN;
  // GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  // GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(RC522_SPI_CS_PORT, &GPIO_InitStructure);

  //将RC522 SCk MOSI MISO的引脚GPIO配置为推挽复用
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
//函 数 名:  SPI_RC522_SendByte() 
//功 能：    使用SPI发送一个字节的数据
//参 数： 	 byte：要发送的数据
//返 回 值： 返回接收到的数据
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
//函 数 名:  SPI_RC522_ReadByte() 
//功 能：    使用SPI读取一个字节的数据
//参 数： 	 无
//返 回 值： 返回接收到的数据
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
  * @brief  读RC522寄存器
  * @param  ucAddress，寄存器地址
  * @retval 寄存器的当前值
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
  * @brief  写RC522寄存器
  * @param  ucAddress，寄存器地址
  * @param  ucValue，写入寄存器的值
  * @retval 无
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
  * @brief  对RC522寄存器置位
  * @param  ucReg，寄存器地址
  * @param  ucMask，置位值
  * @retval 无
  */
void SetBitMask ( uint8_t ucReg, uint8_t ucMask )  
{
  uint8_t ucTemp;

  ucTemp = ReadRawRC ( ucReg );
  WriteRawRC ( ucReg, ucTemp | ucMask ); // set bit mask
}


/**
  * @brief  对RC522寄存器清位
  * @param  ucReg，寄存器地址
  * @param  ucMask，清位值
  * @retval 无
  */
void ClearBitMask ( uint8_t ucReg, uint8_t ucMask )  
{
  uint8_t ucTemp;

  ucTemp = ReadRawRC ( ucReg );
  WriteRawRC ( ucReg, ucTemp & ( ~ ucMask) ); // clear bit mask
}



/*********************************************END OF FILE**********************/
