#include "myflash.h"
//扇区6 0x0804 0000 - 0x0805 ffff
//电压范围2.7-3.6V 并行行数 X32 PSIZE 10 VoltageRange_3


uint16_t MYFLASH_ReadHalfWord(uint32_t Address)
{
	return *((__IO uint16_t *)(Address));
}

void MyFLASH_EraseSector6(void)
{
	FLASH_Unlock();
	FLASH_EraseSector(FLASH_Sector_6, VoltageRange_3);
	FLASH_Lock();
}
void MyFLASH_ProgramHalfWord(uint32_t Address, uint32_t Data)
{
	FLASH_Unlock();
	FLASH_ProgramHalfWord(Address, Data);
	FLASH_Lock();
}

void Store_Flash_Data(void)
{

}
void Get_Flash_Data(void)
{
	
}