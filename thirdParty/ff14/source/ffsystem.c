/*------------------------------------------------------------------------*/
/* Sample Code of OS Dependent Functions for FatFs                        */
/* (C)ChaN, 2018                                                          */
/*------------------------------------------------------------------------*/

#include "pro_conf.h"
#include "ff.h"
#if USER_USE_UCOS == 1
#include "ucos_inc.h"
#endif

#if FF_USE_LFN == 3	/* Dynamic memory allocation */

/*------------------------------------------------------------------------*/
/* Allocate a memory block                                                */
/*------------------------------------------------------------------------*/

void* ff_memalloc (	/* Returns pointer to the allocated memory block (null if not enough core) */
	UINT msize		/* Number of bytes to allocate */
)
{
	return malloc(msize);	/* Allocate a new memory block with POSIX API */
}


/*------------------------------------------------------------------------*/
/* Free a memory block                                                    */
/*------------------------------------------------------------------------*/

void ff_memfree (
	void* mblock	/* Pointer to the memory block to free (nothing to do if null) */
)
{
	free(mblock);	/* Free the memory block with POSIX API */
}

#endif



#if FF_FS_REENTRANT	/* Mutal exclusion */

/*------------------------------------------------------------------------*/
/* Create a Synchronization Object                                        */
/*------------------------------------------------------------------------*/
/* This function is called in f_mount() function to create a new
/  synchronization object for the volume, such as semaphore and mutex.
/  When a 0 is returned, the f_mount() function fails with FR_INT_ERR.
*/

//const osMutexDef_t Mutex[FF_VOLUMES];	/* Table of CMSIS-RTOS mutex */


int ff_cre_syncobj (	/* 1:Function succeeded, 0:Could not create the sync object */
	BYTE vol,			/* Corresponding volume (logical drive number) */
	FF_SYNC_t* sobj		/* Pointer to return the created sync object */
)
{
	/* Win32 */
//	*sobj = CreateMutex(NULL, FALSE, NULL);
//	return (int)(*sobj != INVALID_HANDLE_VALUE);

	/* uITRON */
//	T_CSEM csem = {TA_TPRI,1,1};
//	*sobj = acre_sem(&csem);
//	return (int)(*sobj > 0);

#if USER_USE_RTTHREAD == 1
    *sobj = rt_mutex_create( &vol, RT_IPC_FLAG_PRIO );
    if ( sobj == RT_NULL )
        return 0;
    else 
        return 1;
#endif

}


/*------------------------------------------------------------------------*/
/* Delete a Synchronization Object                                        */
/*------------------------------------------------------------------------*/
/* This function is called in f_mount() function to delete a synchronization
/  object that created with ff_cre_syncobj() function. When a 0 is returned,
/  the f_mount() function fails with FR_INT_ERR.
*/

int ff_del_syncobj (	/* 1:Function succeeded, 0:Could not delete due to an error */
	FF_SYNC_t sobj		/* Sync object tied to the logical drive to be deleted */
)
{
//	/* Win32 */
//	return (int)CloseHandle(sobj);

	/* uITRON */
//	return (int)(del_sem(sobj) == E_OK);

#if USER_USE_RTTHREAD == 1
    rt_err_t err = rt_mutex_delete( sobj );
    if ( err != RT_EOK )
        return 0;
    else 
        return 1;
#endif

}


/*------------------------------------------------------------------------*/
/* Request Grant to Access the Volume                                     */
/*------------------------------------------------------------------------*/
/* This function is called on entering file functions to lock the volume.
/  When a 0 is returned, the file function fails with FR_TIMEOUT.
*/

int ff_req_grant (	/* 1:Got a grant to access the volume, 0:Could not get a grant */
	FF_SYNC_t sobj	/* Sync object to wait */
)
{
	/* FreeRTOS */
//	return (int)(xSemaphoreTake(sobj, FF_FS_TIMEOUT) == pdTRUE);

	/* CMSIS-RTOS */
//	return (int)(osMutexWait(sobj, FF_FS_TIMEOUT) == osOK);

#if USER_USE_RTTHREAD == 1
    rt_err_t err = rt_mutex_take( sobj, FF_FS_TIMEOUT );
    if ( err != RT_EOK )
        return 0;
    else 
        return 1;
#endif
    

}


/*------------------------------------------------------------------------*/
/* Release Grant to Access the Volume                                     */
/*------------------------------------------------------------------------*/
/* This function is called on leaving file functions to unlock the volume.
*/

void ff_rel_grant (
	FF_SYNC_t sobj	/* Sync object to be signaled */
)
{
//	/* Win32 */
//	ReleaseMutex(sobj);

	/* uITRON */
//	sig_sem(sobj);


#if USER_USE_RTTHREAD == 1
    rt_mutex_release( sobj );
#endif

}

#endif

