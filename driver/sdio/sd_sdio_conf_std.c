/************************************************
 * @file sd_sdio_conf.c
 * @author Trisuborn (ttowfive@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2020-12-04
 * 
 * @copyright Copyright (c) 2020
 * 
 ************************************************/
#include "sd_sdio_conf.h"
#include <string.h>
#include "pro_conf.h"
#include "tim.h"

uint8_t conf_flag = 0;

static void sdio_nvic_configuration(void)
{
    NVIC_InitTypeDef NVIC_InitStructure;

    /* Configure the NVIC Preemption Priority Bits */
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);

    NVIC_InitStructure.NVIC_IRQChannel = SDIO_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
    NVIC_InitStructure.NVIC_IRQChannel = SD_SDIO_DMA_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_Init(&NVIC_InitStructure);
}

SD_Error sd_sdio_init(void)
{
    if (!conf_flag)
    {
        sdio_nvic_configuration();
        conf_flag = 1;
    }
    return SD_Init();
}

typedef enum
{
    FAILED = 0,
    PASSED = !FAILED
} TestStatus;

static void Fill_Buffer(uint8_t *pBuffer, uint32_t BufferLength, uint32_t Offset)
{
    uint16_t index = 0;

    /* Put in global buffer same values */
    for (index = 0; index < BufferLength; index++)
    {
        pBuffer[index] = index + Offset;
    }
}

static TestStatus Buffercmp(uint8_t *pBuffer1, uint8_t *pBuffer2, uint32_t BufferLength)
{
    while (BufferLength--)
    {
        if (*pBuffer1 != *pBuffer2)
        {
            return FAILED;
        }

        pBuffer1++;
        pBuffer2++;
    }

    return PASSED;
}


#define BLOCK_SIZE 512       /* Block Size in Bytes */
#define NUMBER_OF_BLOCKS 100 /* For Multi Blocks operation (Read/Write) */
#define MULTI_BUFFER_SIZE (BLOCK_SIZE * NUMBER_OF_BLOCKS)
uint8_t aBuffer_MultiBlock_Tx[MULTI_BUFFER_SIZE] = {0};
uint8_t aBuffer_MultiBlock_Rx[MULTI_BUFFER_SIZE] = {0};
void SD_MultiBlockTest(void)
{
    double wtime, rtime;
    double wsec, rsec;
    double wspeed, rspeed;

    SD_Error Status;
    
    /* Fill the buffer to send */
    Fill_Buffer(aBuffer_MultiBlock_Tx, MULTI_BUFFER_SIZE, 0x0);

    SD_HighSpeed();

    if (Status == SD_OK)
    {
        Clock_Start();

        /* Write multiple block of many bytes on address 0 */
        Status = SD_WriteMultiBlocks(aBuffer_MultiBlock_Tx, 0, BLOCK_SIZE, NUMBER_OF_BLOCKS);

        /* Check if the Transfer is finished */
        Status = SD_WaitWriteOperation();
        while (SD_GetStatus() != SD_TRANSFER_OK);

        wtime = (float)Clock_End();
        wsec = wtime / 1000000;
        wspeed = 50.0 / wsec;
    }

    if (Status == SD_OK)
    {
        Clock_Start();
        /* Read block of many bytes from address 0 */
        Status = SD_ReadMultiBlocks(aBuffer_MultiBlock_Rx, 0, BLOCK_SIZE, NUMBER_OF_BLOCKS);

        /* Check if the Transfer is finished */
        Status = SD_WaitReadOperation();
        while (SD_GetStatus() != SD_TRANSFER_OK);

        rtime = (float)Clock_End();
        rsec = rtime / 1000000;
        rspeed = 50.0 / rsec;
    }

    /* Check the correctness of written data */
    TestStatus TransferStatus2;
    if (Status == SD_OK)
    {
        TransferStatus2 = Buffercmp(aBuffer_MultiBlock_Tx, aBuffer_MultiBlock_Rx, MULTI_BUFFER_SIZE);
    }

    if (TransferStatus2 == PASSED)
    {
        DEBUG_PRINT("m ok\n");
    }
    else
    {
        DEBUG_PRINT("m err\n");
    }
    printf("sd write %0.4f s\n", wsec);
    printf("sd read %0.4f s\n", rsec);
    printf("sd write speed %0.4f MiB/s\n", wspeed / 1024);
    printf("sd read speed %0.4f MiB/s\n", rspeed / 1024);
}

