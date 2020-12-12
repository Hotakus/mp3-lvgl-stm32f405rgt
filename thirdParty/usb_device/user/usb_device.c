
/* Includes ------------------------------------------------------------------*/

#include "usb_device.h"
#include "usbd_core.h"

#include "usbd_msc.h"

#include "usbd_cdc_if.h"
#include "usbd_storage_if.h"

#include "usbd_cdc_desc.h"
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
INIT_APP_EXPORT(usb_device_cdc_init);


void usb_device_cdc_init(void)
{
  /* USER CODE BEGIN USB_DEVICE_Init_PreTreatment */

  /* USER CODE END USB_DEVICE_Init_PreTreatment */

  /* Init Device Library, add supported class and start the library. */
  if (USBD_Init(&hUsbDeviceFS, &FS_CDC_Desc, DEVICE_FS) != USBD_OK)
  {
    Error_Handler();
  }
  if (USBD_RegisterClass(&hUsbDeviceFS, &USBD_CDC) != USBD_OK)
  {
    Error_Handler();
  }
  if (USBD_CDC_RegisterInterface(&hUsbDeviceFS, &USBD_CDC_Interface_fops_FS) != USBD_OK)
  {
    Error_Handler();
  }
  if (USBD_Start(&hUsbDeviceFS) != USBD_OK)
  {
    Error_Handler();
  }

  /* USER CODE BEGIN USB_DEVICE_Init_PostTreatment */

  /* USER CODE END USB_DEVICE_Init_PostTreatment */
}
