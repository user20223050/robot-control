#ifndef __THREEDOF_H
#define __THREEDOF_H

void ThreeDof_Init(void);
void Set_Joint_Pulse(uint16_t pulse1, uint16_t pulse2, uint16_t pulse3,uint16_t pulse4,uint16_t pulse5,uint16_t pulse6);
void Set_direction(uint8_t turn1,uint8_t turn2,uint8_t turn3,uint8_t turn4,uint8_t turn5,uint8_t turn6);
#endif
