#ifndef __RC522_SPI_H
#define __RC522_SPI_H


#include "stm32f10x.h"

//????RC55???RST??
#define RC522_GPIO_RST_PORT     GPIOA
#define RC522_GPIO_RST_PIN  GPIO_Pin_3

//rc522 spi??gpio?
//cs(NSS)??
#define RC522_SPI_CS_PORT         GPIOA
#define RC522_SPI_CS_PIN    GPIO_Pin_4

//rc522 spi ??
#define RC522_SPI_PORT      GPIOA

//sck??
#define RC522_SPI_SCK_PORT        GPIOA
#define RC522_SPI_SCK_PIN   GPIO_Pin_5

//miso??
#define RC522_SPI_MISO_PORT       GPIOA
#define RC522_SPI_MISO_PIN  GPIO_Pin_6

//mosi??
#define RC522_SPI_MOSI_PORT       GPIOA
#define RC522_SPI_MOSI_PIN  GPIO_Pin_7

#define RC522_SPIX      SPI1

#define RC522_CS_Enable()   GPIO_ResetBits(RC522_SPI_CS_PORT,RC522_SPI_CS_PIN)
#define RC522_CS_Disable()  GPIO_SetBits(RC522_SPI_CS_PORT,RC522_SPI_CS_PIN)

#define CLR_NFC_RST   GPIO_ResetBits(RC522_GPIO_RST_PORT,RC522_GPIO_RST_PIN)
#define SET_NFC_RST   GPIO_SetBits(RC522_GPIO_RST_PORT,RC522_GPIO_RST_PIN)

#define CLR_SPI_CK    GPIO_ResetBits(RC522_SPI_SCK_PORT,RC522_SPI_SCK_PIN)
#define SET_SPI_CK    GPIO_SetBits(RC522_SPI_SCK_PORT,RC522_SPI_SCK_PIN)

#define CLR_SPI_MOSI  GPIO_ResetBits(RC522_SPI_MOSI_PORT,RC522_SPI_MOSI_PIN)
#define SET_SPI_MOSI  GPIO_SetBits(RC522_SPI_MOSI_PORT,RC522_SPI_MOSI_PIN)

#define STU_SPI_MISO GPIO_ReadInputDataBit(RC522_SPI_MISO_PORT,RC522_SPI_MISO_PIN)


/**************************???????????????????********************************/
void RC522_GPIO_Init(void);
unsigned char ReadRawRC(unsigned char Address);
void WriteRawRC(unsigned char Address, unsigned char value);
void SetBitMask(unsigned char reg,unsigned char mask);
void ClearBitMask(unsigned char reg,unsigned char mask);


#endif	/* __RFID1356M_H */


