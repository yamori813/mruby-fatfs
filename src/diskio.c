/*-----------------------------------------------------------------------*/
/* Low level disk I/O module SKELETON for FatFs     (C)ChaN, 2019        */
/*-----------------------------------------------------------------------*/
/* If a working storage control module is available, it should be        */
/* attached to the FatFs via a glue function rather than modifying it.   */
/* This is an example of glue functions to attach various exsisting      */
/* storage control modules to the FatFs module with a defined API.       */
/*-----------------------------------------------------------------------*/

#include <stdio.h>
#include <unistd.h>
#include <time.h>

#include "ff.h"		/* Obtains integer types */
#include "diskio.h"	/* Declarations of disk functions */

extern int fatfd;

/*-----------------------------------------------------------------------*/
/* Get Drive Status                                                      */
/*-----------------------------------------------------------------------*/

DSTATUS disk_status (
	BYTE pdrv	/* Physical drive nmuber to identify the drive */
)
{


//	return STA_NOINIT;
	return 0;
}



/*-----------------------------------------------------------------------*/
/* Inidialize a Drive                                                    */
/*-----------------------------------------------------------------------*/

DSTATUS disk_initialize (
	BYTE pdrv	/* Physical drive nmuber to identify the drive */
)
{

	return RES_OK;
}



/*-----------------------------------------------------------------------*/
/* Read Sector(s)                                                        */
/*-----------------------------------------------------------------------*/

DRESULT disk_read (
	BYTE pdrv,	/* Physical drive nmuber to identify the drive */
	BYTE *buff,	/* Data buffer to store read data */
	LBA_t sector,	/* Start sector in LBA */
	UINT count	/* Number of sectors to read */
)
{

	lseek(fatfd, 512 * sector, SEEK_SET);
	read(fatfd, buff, 512 * count);

	return RES_OK;
//	return RES_PARERR;
}



/*-----------------------------------------------------------------------*/
/* Write Sector(s)                                                       */
/*-----------------------------------------------------------------------*/

#if FF_FS_READONLY == 0

DRESULT disk_write (
	BYTE pdrv,	/* Physical drive nmuber to identify the drive */
	const BYTE *buff,	/* Data to be written */
	LBA_t sector,	/* Start sector in LBA */
	UINT count	/* Number of sectors to write */
)
{

	lseek(fatfd, 512 * sector, SEEK_SET);
	write(fatfd, buff, 512 * count);

	return RES_OK;
//	return RES_PARERR;
}

#endif


/*-----------------------------------------------------------------------*/
/* Miscellaneous Functions                                               */
/*-----------------------------------------------------------------------*/

DRESULT disk_ioctl (
	BYTE pdrv,	/* Physical drive nmuber (0..) */
	BYTE cmd,	/* Control code */
	void *buff	/* Buffer to send/receive control data */
)
{
//printf("MORIMORI CT");

	return RES_OK;
//	return RES_PARERR;
}

/*-----------------------------------------------------------------------*/
/* Get Time Functions                                                    */
/*-----------------------------------------------------------------------*/

DWORD get_fattime (void)
{
	DWORD res;
	struct tm tm;
	time_t t = time(NULL);
	localtime_r(&t, &tm);

	res =  ((tm.tm_year - 80) << 25) | ((tm.tm_mon + 1) << 21) |
	    (tm.tm_mday << 16) | (tm.tm_hour << 11) | (tm.tm_min << 5) |
	    (tm.tm_sec / 2);

	return res;
}
