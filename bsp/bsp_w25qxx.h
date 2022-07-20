#ifndef __BSP_W25QXX_H
#define __BSP_W25QXX_H

#include "main.h"

#define ZB25QV64
#ifdef ZB25QV64
    #define sFLASH_ID 0X5E16
#endif

#ifdef W25Q128
    #define sFLASH_ID 0XEF4018 
#endif

#ifdef W25Q256
    #define sFLASH_ID 0XEF4019 
#endif

#define W25QXX_PageSize                         256
#define W25QXX_PerWritePageSize                 256
#define W25X_WriteEnable                        0x06
#define W25X_WriteDisable                       0x04
#define W25X_ReadStatusReg                      0x05
#define W25X_WriteStatusReg                     0x01
#define W25X_ReadData                           0x03
#define W25X_FastReadData                       0x0B
#define W25X_FastReadDual                       0x3B
#define W25X_PageProgram                        0x02
#define W25X_BlockErase                         0xD8
#define W25X_SectorErase                        0x20
#define W25X_ChipErase                          0xC7
#define W25X_PowerDown                          0xB9
#define W25X_ReleasePowerDown                   0xAB
#define W25X_DeviceID                           0xAB
#define W25X_ManufactDeviceID                   0x90
#define W25X_JedecDeviceID                      0x9F
#define W25X_Enter4ByteMode                     0xB7
#define W25X_ReadStatusRegister3                0x15
#define WIP_Flag                                0x01 /* Write In Progress (WIP) flag */
#define Dummy_Byte                              0xFF


uint32_t w25qxx_readID(void);
void w25qxx_write_enable(void);
void w25qxx_wait_busy(void);
void w25qxx_wake_up(void);
void w25qxx_sectorerase(uint32_t addr);
void w25qxx_pagewrite(uint8_t *pbuff,uint32_t addr,uint16_t numofpage);
void w25qxx_buffwrite(uint8_t *pbuff,uint32_t writeaddr,uint16_t numtowrite);
void w25qxx_readbuff(uint8_t  *pbuff,uint32_t addr,uint32_t numtoread);

#endif /* __BSP_W25QXX_H */
