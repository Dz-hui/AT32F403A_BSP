#include "bsp_w25qxx.h"

uint32_t w25qxx_readID(void){

    uint8_t temp0 = 0,temp1 = 0,temp2 = 0;
    uint16_t temp =0;

    SPI_CS_LOW();
    bsp_spi_senddata(W25X_ManufactDeviceID);
    bsp_spi_senddata(Dummy_Byte);
    bsp_spi_senddata(Dummy_Byte);
    bsp_spi_senddata(0x00);
    temp0 = bsp_spi_readdata();
    temp1 = bsp_spi_readdata();
    SPI_CS_HIGH();

    temp = (temp0<<8)|(temp1);
    return temp;
}

void w25qxx_write_enable(void){

    SPI_CS_LOW();
    bsp_spi_senddata(W25X_WriteEnable);
    SPI_CS_HIGH();

}

void w25qxx_wait_busy(void){
    
    SPI_CS_LOW();
    bsp_spi_senddata(W25X_ReadStatusReg);
    while((bsp_spi_readdata() & WIP_Flag ) == WIP_Flag);
    SPI_CS_HIGH();

}

void w25qxx_wake_up(void){

    SPI_CS_LOW();
    bsp_spi_senddata(W25X_ReleasePowerDown);
    SPI_CS_HIGH();

}

void w25qxx_sectorerase(uint32_t addr){

    w25qxx_write_enable();
    w25qxx_wait_busy();

    SPI_CS_LOW();
    bsp_spi_senddata(W25X_SectorErase);
    bsp_spi_senddata((addr & 0XFF0000) >> 16);
    bsp_spi_senddata((addr & 0X00FF00) >> 8);
    bsp_spi_senddata(addr & 0X0000FF);
    SPI_CS_HIGH();

    w25qxx_wait_busy();

}

void w25qxx_pagewrite(uint8_t *pbuff,uint32_t addr,uint16_t numofpage){

    w25qxx_write_enable();

    SPI_CS_LOW();
    bsp_spi_senddata(W25X_PageProgram);
    bsp_spi_senddata((addr & 0XFF0000) >> 16);
    bsp_spi_senddata((addr & 0X00FF00) >> 8);
    bsp_spi_senddata(addr & 0X0000FF);

    if(numofpage > W25QXX_PageSize){
        printf("W25QXX_PageWrite is too large\n");
    }

    while(numofpage--){
       bsp_spi_senddata(*pbuff); 
       pbuff ++;
    }
    SPI_CS_HIGH();

    w25qxx_wait_busy();

}

void w25qxx_buffwrite(uint8_t *pbuff,uint32_t writeaddr,uint16_t numtowrite){

    uint8_t sectoroffset,sectorcount,numofpage,numofsingle ;
    uint8_t temp = 0;
    w25qxx_write_enable();

    sectoroffset = writeaddr % W25QXX_PerWritePageSize;
    sectorcount = W25QXX_PerWritePageSize - sectoroffset;
    numofpage = numtowrite / W25QXX_PerWritePageSize;
    numofsingle = numtowrite % W25QXX_PerWritePageSize;
    if(sectoroffset == 0){

        if(numofpage == 0){
            w25qxx_pagewrite(pbuff,writeaddr,numtowrite);
        }
        else{
            while(numofpage--){
                w25qxx_pagewrite(pbuff,writeaddr,W25QXX_PerWritePageSize);
                pbuff = pbuff + W25QXX_PerWritePageSize;
                writeaddr = writeaddr + W25QXX_PerWritePageSize;
            }
            w25qxx_pagewrite(pbuff,writeaddr,numofsingle);    
        }    
    }
    else{

        if(numofpage == 0){
            if(sectorcount > numtowrite){
                w25qxx_pagewrite(pbuff,writeaddr,numtowrite);
            }
            else{
                
                w25qxx_pagewrite(pbuff,writeaddr,sectorcount);
                temp = numtowrite-sectorcount;
                pbuff += sectorcount;
                writeaddr += sectorcount;
                w25qxx_pagewrite(pbuff,writeaddr,temp);
            }
        }
        else{
            numtowrite -= sectorcount;
            numofpage = numtowrite / W25QXX_PerWritePageSize;
            numofsingle = numtowrite % W25QXX_PerWritePageSize;

            w25qxx_pagewrite(pbuff,writeaddr,sectorcount);
            writeaddr += sectorcount;
            pbuff += sectorcount;

            while(numofpage--){
                w25qxx_pagewrite(pbuff,writeaddr,W25QXX_PerWritePageSize);
                pbuff = pbuff + W25QXX_PerWritePageSize;
                writeaddr = writeaddr + W25QXX_PerWritePageSize;
            }
            if(numofsingle !=0){
                w25qxx_pagewrite(pbuff,writeaddr,numofsingle);
            }           
        }       
    }

}

void w25qxx_readbuff(uint8_t  *pbuff,uint32_t addr,uint32_t numtoread){
    
    SPI_CS_LOW();
    bsp_spi_senddata(W25X_ReadData);
    bsp_spi_senddata((addr & 0xFF0000)>>16);
    bsp_spi_senddata((addr & 0x00FF00)>>8);
    bsp_spi_senddata(addr & 0x0000FF);
    while(numtoread --){
        *pbuff = bsp_spi_readdata();
        pbuff ++;
    }
    SPI_CS_HIGH();
	
}
