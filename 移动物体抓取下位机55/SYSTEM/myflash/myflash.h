#ifndef __MYFLASH_
#define __MYFLASH_

#include "stm32f4xx.h"

#define STORE_START_ADDRESS		0x08040000

void MyFLASH_EraseSector6(void);
uint16_t MYFLASH_ReadHalfWord(uint32_t Address);
void MyFLASH_ProgramHalfWord(uint32_t Address, uint32_t Data);
void Store_Flash_Data(void);
void Get_Flash_Data(void);
#endif
