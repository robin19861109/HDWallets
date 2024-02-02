#ifndef __MY_PNG_H_
#define __MY_PNG_H_

#include "main.h"
#include "ff.h"
#define FF_USE_LFN		3
#define FF_MAX_LFN		255
typedef struct
{
   int num_file;
	 DWORD picoffsettbl[64];
} _info_PicFile;

extern _info_PicFile info_PicFile;
extern char *pmyPicDir;

int chkPngFile(void);
FRESULT dir_sdi (	/* FR_OK(0):succeeded, !=0:error */
	DIR* dp,		/* Pointer to directory object */
	DWORD ofs		/* Offset of directory table */
);
#endif