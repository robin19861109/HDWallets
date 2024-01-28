#ifndef __24C02_H
#define __24C02_H
#include "common.h"  
#include "main.h"

////////////////////////////////////////////////////////////////////////////////// 	

//IO定义函数	 
#define IIC_SCL(x)      HAL_GPIO_WritePin(GPIOB,GPIO_PIN_8,x) //PBout(8) //SCL
#define IIC_SDAOUT(x)   HAL_GPIO_WritePin(GPIOB,GPIO_PIN_9,x)  //PBout(9) //输出SDA	 
#define IIC_SDAIN    HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_9)   

//IIC相关函数
void IIC_Init(void);          //初始化IIC的IO口				 
void IIC_Start(void);				  //发送IIC开始信号
void IIC_Stop(void);	  			//发送IIC停止信号
uint8_t MCU_Wait_Ack(void); 				//IIC等待ACK信号
void MCU_Send_Ack(void);			//IIC发送ACK信号
void MCU_NOAck(void);				  //IIC不发送ACK信号
void IIC_write_OneByte(uint8_t Senddata);
uint8_t IIC_Read_OneByte(uint8_t ack);
	
//EEPROM24c02相关函数
uint8_t AT24C02_ReadByte(uint8_t ReadAddr);							     //指定地址读取一个字节
void AT24C02_WriteByte(uint8_t WriteAddr,uint8_t DataToWrite);		//指定地址写入一个字节

void AT24C02_Write(uint8_t WriteAddr,uint8_t *pBuffer,uint8_t WriteNum);	//从指定地址开始写入指定长度的数据
void AT24C02_Read(uint8_t ReadAddr,uint8_t *pBuffer,uint8_t ReadNum);   	//从指定地址开始读出指定长度的数据

uint8_t AT24C02_Test(void);  //检查器件
void AT24C02_Init(void); //初始化IIC

#endif
















