#include "SPI_Flash_w25q64.h"
#include "stm32f1xx_hal.h"
#include "tim.h"
#include "adc.h"
//#include "pic.h"

extern SPI_HandleTypeDef hspi2;
static SPI_HandleTypeDef *g_HSPI_Flash = &hspi2;

/* CS:        PB12
 * SPI1_MISO: PB14
 * SPI1_MOSI: PB15
 * SPI1_SCK:  PB13
 */

#define W25Q64_CS_GPIO_GROUP GPIOB
#define W25Q64_CS_GPIO_PIN   GPIO_PIN_12
#define W25Q64_TIMEOUT       500


/**
 * @brief 选中W25Q64
 *
 * @param  
 * 
 * @return  
 * 
**/
static void W25Q64_Select(void)
{
    HAL_GPIO_WritePin(W25Q64_CS_GPIO_GROUP, W25Q64_CS_GPIO_PIN, GPIO_PIN_RESET);
}

/**
 * @brief 不选中W25Q64
 *
 * @param  
 * 
 * @return  
 * 
**/
static void W25Q64_Deselect(void)
{
    HAL_GPIO_WritePin(W25Q64_CS_GPIO_GROUP, W25Q64_CS_GPIO_PIN, GPIO_PIN_SET);
}

/**
 * @brief 使用SPI发送/接收数据(注意这个函数没有设置片选信号)
 *
 * @param pTxData: 要发送的数据
 * @param pRxData: 接收缓冲区
 * @param Size:    数据长度
 * @param Timeout: 超时时间(单位ms)
 * 
 * @return : 0 - 成功, 1 - 失败
 * 
**/
static int  W25Q64_TxRx(uint8_t *pTxData, uint8_t *pRxData, uint16_t Size, uint32_t Timeout)
{
    if (HAL_OK == HAL_SPI_TransmitReceive(g_HSPI_Flash, pTxData, pRxData, Size, Timeout))
        return 0;
    else
        return 1;
}

/**
 * @brief 使用SPI发送数据(注意这个函数没有设置片选信号)
 *
 * @param pTxData: 要发送的数据
 * @param Size:    数据长度
 * @param Timeout: 超时时间(单位ms)
 * 
 * @return : 0 - 成功, 1 - 失败
 * 
**/
static int  W25Q64_Tx(uint8_t *pTxData, uint16_t Size, uint32_t Timeout)
{
    if (HAL_OK == HAL_SPI_Transmit(g_HSPI_Flash, pTxData, Size, Timeout))
        return 0;
    else
        return 1;
}

/**
 * @brief 使用SPI读取数据(注意这个函数没有设置片选信号)
 *
 * @param pRxData: 接收缓冲区
 * @param Size:    数据长度
 * @param Timeout: 超时时间(单位ms)
 * 
 * @return : 0 - 成功, 1 - 失败
 * 
**/
static int  W25Q64_Rx(uint8_t *pRxData, uint16_t Size, uint32_t Timeout)
{
    if (HAL_OK == HAL_SPI_Receive(g_HSPI_Flash, pRxData, Size, Timeout))
        return 0;
    else
        return 1;
}

/**
 * @brief 读取芯片ID
 *
 * @param 
 * 
 * @return : 0XEF13 - 表示芯片型号为W25Q80
 *           0XEF14 - 表示芯片型号为W25Q16    
 *           0XEF15 - 表示芯片型号为W25Q32  
 *           0XEF16 - 表示芯片型号为W25Q64 
 * 
**/  
uint16_t W25X_ReadID(void)
{
    uint16_t Temp = 0;
    unsigned char tx_buf[4] = {0x90,0x00,0x00,0x00};
    unsigned char rx_buf[4] = {0};
    W25Q64_Select();      
    W25Q64_TxRx(tx_buf,rx_buf,4,W25Q64_TIMEOUT);  
    //W25Q64_WaitReady(); 
    W25Q64_Deselect();

    printf("chip_id is 0x%02x \r\n",rx_buf[0]);
    printf("chip_id is 0x%02x \r\n",rx_buf[1]);
    printf("chip_id is 0x%02x \r\n",rx_buf[2]);
    printf("chip_id is 0x%02x \r\n",rx_buf[3]);

    Temp|= rx_buf[1]<<8;  
    Temp|= rx_buf[0];  

    return Temp;
}


/**
 * @brief 等待W25Q64就绪
 *
 * @param 
 * 
 * @return : 0 - 成功, 1 - 失败
 * 
**/
static int W25Q64_WaitReady(void)
{
    unsigned char tx_buf[2];
    unsigned char rx_buf[2];
    int timeout = W25Q64_TIMEOUT;

    tx_buf[0] = 0x05; /* 读状态 */
    tx_buf[1] = 0xff;
	
	while (timeout--)
	{
		rx_buf[0] = rx_buf[1] = 0;
        W25Q64_Select();
		W25Q64_TxRx(tx_buf, rx_buf, 2, W25Q64_TIMEOUT);
        W25Q64_Deselect();
        printf("chip_id2 is 0x%02x \r\n",rx_buf[0]);
        printf("chip_id2 is 0x%02x \r\n",rx_buf[1]);
		if ((rx_buf[1] & 1) == 0)
			return 0;
        HAL_Delay(1);
	}

    if (!timeout)
    {
    	return 1;
    }
	return 0;
}

/**
 * @brief 写使能
 *
 * @param 
 * 
 * @return : 0 - 成功, 1 - 失败
 * 
**/
static int W25Q64_WriteEnable(void)
{
    unsigned char tmpbuf[1];
    int err;
    
    tmpbuf[0] = 0x06; /* 写使能 */
    W25Q64_Select();
    err = W25Q64_Tx(tmpbuf, 1, W25Q64_TIMEOUT);
    W25Q64_Deselect();

    return err;
}

/**
 * @brief W25Q64的初始化函数
 *
 * @param 
 * 
 * @return :
 * 
**/
void W25Q64_Init(void)
{
    /* 片选信号PB9在MX_GPIO_Init中被配置为输出引脚 */
    /* SPI在MX_SPI1_Init中也被配置好了 */
}

/**
 * @brief W25Q64读函数
 *
 * @param offset:读哪个地址
 * @param buf   :用于保存数据
 * @param len   :读多少字节
 * 
 * @return : 非负数 - 读取了多少字节的数据, (-1) - 失败
 * 
**/
int W25Q64_Read(uint32_t offset, uint8_t *buf, uint32_t len)
{
    unsigned char tmpbuf[4];
    int err;
    
    /* 自己实现SPI Flash的读操作 */
    tmpbuf[0] = 0x03;
    tmpbuf[1] = (offset >> 16) & 0xff;
    tmpbuf[2] = (offset >> 8) & 0xff;
    tmpbuf[3] = (offset >> 0) & 0xff;
    
    W25Q64_Select();

    /* 发送读命令 */
    err = W25Q64_Tx(tmpbuf, 4, W25Q64_TIMEOUT);
    if (err)
    {
        W25Q64_Deselect();
        return -1;
    }

    /* 读数据 */
    err = W25Q64_Rx(buf, len, W25Q64_TIMEOUT);
    if (err)
    {
        W25Q64_Deselect();
        return -1;
    }

    W25Q64_Deselect();    
    return len;
}

/**
 * @brief W25Q64写函数(需要先擦除)
 *
 * @param offset:写哪个地址
 * @param buf   :数据buffer
 * @param len   :写多少字节
 * 
 * @return : 非负数 - 读取了多少字节的数据, (-1) - 失败
 * 
**/
int W25Q64_Write(uint32_t offset, uint8_t *buf, uint32_t len)
{
    uint8_t tmpbuf[4];
    uint32_t phy_pos = offset;
    int err;
    uint32_t cur_len;
    uint32_t remain_len;

    /* 写数据 */
    phy_pos = offset;
    remain_len = len;

    /* 可能不是从Page开头写数据(1个Page是256字节) */
    cur_len = offset & (256-1);
    cur_len = 256 - cur_len;
    if (cur_len > len)
        cur_len = len;
    
    for (; phy_pos < offset + len; )
    {
        /* 写使能 */
        err = W25Q64_WriteEnable();
        if (err)
        {
            return -1;
        }
        
        tmpbuf[0] = 0x02; /* page program */
        tmpbuf[1] = (phy_pos >> 16) & 0xff;
        tmpbuf[2] = (phy_pos >> 8) & 0xff;
        tmpbuf[3] = (phy_pos >> 0) & 0xff;

        /* 发送page program命令 */
        W25Q64_Select();
        err = W25Q64_Tx(tmpbuf, 4, W25Q64_TIMEOUT);
        if (err)
        {
            W25Q64_Deselect();
            return -1;
        }
        
        /* 发送数据 */
        err = W25Q64_Tx(buf, cur_len, W25Q64_TIMEOUT);
        if (err)
        {
            W25Q64_Deselect();
            return -1;
        }
        W25Q64_Deselect();
        
        /* 读状态 */
        err = W25Q64_WaitReady();
        if (err)
        {
            return -1;
        }

        phy_pos += cur_len;
        buf     += cur_len;
        remain_len -= cur_len;

        cur_len = (remain_len < 256) ? remain_len : 256;
    }
    return len;
}

/**
 * @brief W25Q64擦除函数
 *
 * @param offset:擦除哪个地址(4096对齐)
 * @param len   :擦除多少字节(4096对齐)
 * 
 * @return : 非负数 - 读取了多少字节的数据, (-1) - 失败
 * 
**/
int W25Q64_Erase(uint32_t offset, uint32_t len)
{
    unsigned char tmpbuf[4];
    uint32_t phy_pos = offset;
    int err;

    if ((offset & (4096-1)) && (len & (4096-1)))
        return -1;

    for (int sector = 0; sector < len/4096; sector++)
    {
        /* 写使能 */
        err = W25Q64_WriteEnable();
        if (err)
        {
            return -1;
        }
        
        tmpbuf[0] = 0x20; /* 擦除 */
        tmpbuf[1] = (phy_pos >> 16) & 0xff;
        tmpbuf[2] = (phy_pos >> 8) & 0xff;
        tmpbuf[3] = (phy_pos >> 0) & 0xff;
        
        W25Q64_Select();
        W25Q64_Tx(tmpbuf, 4, W25Q64_TIMEOUT);
        if (err)
        {
            W25Q64_Deselect();
            return -1;
        }
        W25Q64_Deselect();
        
        phy_pos += 4096;

        /* 读状态 */
        err = W25Q64_WaitReady();
        if (err)
        {
            return -1;
        }
    }

    return len;
}

/**
 * @brief W25Q64测试程序
 *
 * @param[in]
 * 
 * @return 
 * 
**/
void W25Q64_Test(void)
{
    // int sector;
    // int len;
//    uint8_t write_buf[4] = {0x0001,0x0002,0x0003,0x0004};
//    uint8_t read_buf[4];
    // int i;
    // uint32_t val1, val2;
    
   // W25Q64_Init();
    //W25Q64_Erase(0X0000,8192000);
    //W25Q64_Write(pic_4,gImage_pic_4,pic_size);
    // W25Q64_Write(0x0000,write_buf,4);
    // W25Q64_Read(0x0000,read_buf,4);
    // printf("The value of read_buf[0] is: 0x%2x \r\n",read_buf[0]);
    // printf("The value of read_buf[1] is: 0x%2x  \r\n",read_buf[1]);
    // printf("The value of read_buf[2] is: 0x%2x  \r\n",read_buf[2]);
    // printf("The value of read_buf[3] is: 0x%2x  \r\n",read_buf[3]);
    // HAL_Delay(1000);
    // while (1)
    // {
    //     LCD_PrintString(0, 0, "W25Q64 Test: ");

    //     for (sector = 0; sector < 2048; sector++)
    //     {
    //         /* 擦除测试 */
    //         LCD_ClearLine(0, 2);
    //         len = LCD_PrintString(0, 2, "Erase ");
    //         LCD_PrintSignedVal(len, 2, sector);
    //         W25Q64_Erase(sector * 4096, 4096);            
    //         W25Q64_Read(sector * 4096, buf, 4);
    //         //LCD_ClearLine(0, 4);
    //         for (i = 0; i < 4; i++)
    //         {
    //             LCD_PrintHex(i*3, 4, buf[i], 0);
    //         }
    //         HAL_Delay(1000);

            // /* 写入测试 */
            // LCD_ClearLine(0, 2);
            // len = LCD_PrintString(0, 2, "Write ");
            // LCD_PrintSignedVal(len, 2, sector);

            // val1 = system_get_ns();
            // W25Q64_Write(sector * 4096, (uint8_t *)&val1, 4);
            // LCD_ClearLine(0, 4);
            // LCD_PrintHex(0, 4, val1, 1);
            // mdelay(1000);

            // /* 读出测试 */
            // LCD_ClearLine(0, 2);
            // len = LCD_PrintString(0, 2, "Read ");
            // LCD_PrintSignedVal(len, 2, sector);

            // W25Q64_Read(sector * 4096, (uint8_t *)&val2, 4);
            // LCD_ClearLine(0, 4);
            // if (val1 == val2)
            //     LCD_PrintString(0, 4, "Test ok");
            // else
            // {
            //     LCD_PrintHex(0, 4, val2, 1);
            //     LCD_ClearLine(0, 6);
            //     LCD_PrintString(0, 6, "Test Failed");
            // }
            // mdelay(1000);
            
        // }
    // }
}
