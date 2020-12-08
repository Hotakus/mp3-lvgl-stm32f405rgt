/************************************************
 * @file main.c
 * @author Trisuborn (ttowfive@gmail.com)
 * @brief
 * @version 0.1
 * @date 2020-12-01
 *
 * @copyright Copyright (c) 2020
 *
 ************************************************/
#include "main.h"

int main()
{
#if USER_USE_RTTHREAD == 0
    HAL_Init();
    uart1_conf(115200);
#endif

    return 0;
}

void fatfs_test()
{
    FATFS* fs = (FATFS*)MALLOC(sizeof(FATFS));
    FRESULT fres;
    if (fres != FR_OK) {
        fres = f_mount(fs, "SD_SDIO:", 1);
        if (fres != FR_OK) {
            DEBUG_PRINT("sd card mount error. (fr: %d)\n", fres);
            if (fres == FR_NOT_READY)
                DEBUG_PRINT("no sd card.\n");
        } else
            DEBUG_PRINT("sd card mount successfully.\n");
    }
    FREE(fs);
}
MSH_CMD_EXPORT(fatfs_test,fatfs_test);

