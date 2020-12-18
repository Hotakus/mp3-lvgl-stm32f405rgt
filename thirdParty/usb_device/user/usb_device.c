
/* Includes ------------------------------------------------------------------*/

#include "usb_device.h"
#include "usbd_core.h"

#include "usbd_msc.h"
#include "usbd_storage_if.h"
#include "usbd_storage_desc.h"



/* USB Device Core handle declaration. */
USBD_HandleTypeDef hUsbDeviceFS;


/**
  * Init USB device Library, add supported class and start the library
  * @retval None
  */
void usb_device_msc_init(void)
{

  /* Init Device Library, add supported class and start the library. */
  if (USBD_Init(&hUsbDeviceFS, &FS_Storage_Desc, DEVICE_FS) != USBD_OK) {
    DEBUG_PRINT("USBD_Init error.\n");
    return;
  }
  if (USBD_RegisterClass(&hUsbDeviceFS, &USBD_MSC) != USBD_OK) {
    DEBUG_PRINT("USBD_RegisterClass error.\n");
    return;
  }
  if (USBD_MSC_RegisterStorage(&hUsbDeviceFS, &USBD_Storage_Interface_fops_FS) != USBD_OK) {
    DEBUG_PRINT("USBD_MSC_RegisterStorage error.\n");
    return;
  }
  if (USBD_Start(&hUsbDeviceFS) != USBD_OK) {
    DEBUG_PRINT("USBD_Start error.\n");
    return;
  }

}
