#ifndef __SPI_FLASH_W25Q64_
#define __SPI_FLASH_W25Q64_

#include "sys.h"

#define pic_size        (uint32_t)38400
#define pic_1           (uint32_t)   0
#define pic_2           (uint32_t)pic_1+pic_size
#define pic_3           (uint32_t)pic_2+pic_size
#define pic_4           (uint32_t)pic_3+pic_size
/**
 * @brief W25Q64的初始化函数
 *
 * @param[in] 
 * 
 * @return :
 * 
**/
void W25Q64_Init(void);

/**
 * @brief W25Q64读函数
 *
 * @param[in] offset:读哪个地址
 * @param[in] buf   :用于保存数据
 * @param[in] len   :读多少字节
 * 
 * @return : 非负数 - 读取了多少字节的数据, (-1) - 失败
 * 
**/
int W25Q64_Read(uint32_t offset, uint8_t *buf, uint32_t len);

/**
 * @brief W25Q64写函数(需要先擦除)
 *
 * @param[in] offset:写哪个地址
 * @param[in] buf   :数据buffer
 * @param[in] len   :写多少字节
 * 
 * @return : 非负数 - 读取了多少字节的数据, (-1) - 失败
 * 
**/
int W25Q64_Write(uint32_t offset, uint8_t *buf, uint32_t len);

/**
 * @brief W25Q64擦除函数
 *
 * @param[in] offset:擦除哪个地址(4096对齐)
 * @param[in] len   :擦除多少字节(4096对齐)
 * 
 * @return : 非负数 - 读取了多少字节的数据, (-1) - 失败
 * 
**/
int W25Q64_Erase(uint32_t offset, uint32_t len);

/**
 * @brief W25Q64测试程序
 *
 * @param[in]
 * 
 * @return 
 * 
**/
void W25Q64_Test(void);

#endif

