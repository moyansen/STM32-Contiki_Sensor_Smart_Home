#include <stdarg.h>
#include "SPI.h"
#include "wifi_config.h"



/**
*@brief SPI Initialization
**/
void SPI1_Initialization(void)
{
    SPI_InitTypeDef  SPI_InitStructure;
    GPIO_InitTypeDef GPIO_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;
    
    RCC_APB2PeriphClockCmd( RCC_APB2Periph_SPI1 | RCC_APB2Periph_GPIOA, ENABLE);

    /* NSS---->GPIO(LED) */
    SPI_SSOutputCmd(SPI1, ENABLE);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7 ;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);


    /* SPI1 Config -------------------------------------------------------------*/
    SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;//全双工通信
    SPI_InitStructure.SPI_Mode = SPI_Mode_Master;//做主机
    SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;//8位数据帧
    SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;//空闲时为低电平
    SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;//数据在第1个跳变沿被采集
    SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;//CS脚为软件模式
    SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_2;//2分频
    SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;//高字节在前
    SPI_InitStructure.SPI_CRCPolynomial = 7;
    SPI_Init(SPI1, &SPI_InitStructure);


    NVIC_InitStructure.NVIC_IRQChannel = SPI1_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 14;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
    SPI_I2S_ITConfig(SPI1,SPI_I2S_IT_RXNE,ENABLE);


    /* Enable SPI1 */
    SPI_Cmd(SPI1, ENABLE);
    SPI_I2S_ClearITPendingBit(SPI1, SPI_I2S_IT_RXNE);
}


void SPI1SendOneByte(uint8_t byteData)
{

    while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);    /* Loop while DR register in not emplty */
    SPI_I2S_SendData(SPI1, byteData);    /* Send byte through the SPI peripheral */
}

uint8_t SPI1ReceiveOneByte(void)
{
    uint8_t receiveByte=0;
  
    while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET);/* Loop while DR register in not emplty */
    receiveByte = (uint8_t)SPI_I2S_ReceiveData(SPI1);
    return receiveByte;
}

/**
* @brief This function handles SPI1 global interrupt.
*/
void SPI1_IRQHandler(void)
{
    sendUart1OneByte(SPI1ReceiveOneByte());
    GPIO_SetBits(GPIOD, GPIO_Pin_2);
}



