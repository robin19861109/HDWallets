#include "my_png.h"
#include "ff.h"
#include "exfuns.h"
#include "malloc.h"

//Check if the PNG file exists.
#define FF_USE_LFN		3
#define FF_MAX_LFN		255


uint16_t pic_get_tnum(char *path)
{
    uint8_t res;
    uint16_t rval = 0;
    DIR tdir;                                                 /* 临时目录 */
	  FILINFO dtfileinfo;
	
    FILINFO *tfileinfo=(FILINFO *)&dtfileinfo;                                       /* 临时文件信息 */

  //  tfileinfo = (FILINFO *)Mem_malloc(INSRAM, sizeof(FILINFO)); /* 申请内存 */
	//  goto  _end_;

    res = f_opendir(&tdir, (const TCHAR *)path);              /* 打开目录 */

    if (res == FR_OK && tfileinfo)
    {
        while (1)                               /* 查询总的有效文件数 */
        {
            res = f_readdir(&tdir, tfileinfo);  /* 读取目录下的一个文件 */

            if (res != FR_OK || tfileinfo->fname[0] == 0)
                break;                          /* 错误了/到末尾了,退出 */

            res = exfuns_file_type(tfileinfo->fname);

            if ((res & 0XF0) == 0X50)           /* 取高四位,看看是不是图片文件 */
            {
                rval++;                         /* 有效文件数增加1 */
            }
        }
    }

//		_end_:		    Mem_free(INSRAM, tfileinfo);                  /* 释放内存 */
        return rval;
}



uint16_t pic_get_pngnum(char *path)
{
    uint8_t res;
    uint16_t rval = 0;
    DIR tdir;                                                 /* 临时目录 */
	  FILINFO dtfileinfo;
	
     FILINFO *tfileinfo=(FILINFO *)&dtfileinfo;                                       /* 临时文件信息 */

    tfileinfo = (FILINFO *)Mem_malloc(EXSRAM, sizeof(FILINFO)); /* 申请内存 */
	  if(tfileinfo==NULL)
			goto  _end_;

    res = f_opendir(&tdir, (const TCHAR *)path);              /* 打开目录 */

    if (res == FR_OK && tfileinfo)
    {
        while (1)                               /* 查询总的有效文件数 */
        {
            res = f_readdir(&tdir, tfileinfo);  /* 读取目录下的一个文件 */

            if (res != FR_OK || tfileinfo->fname[0] == 0)
                break;                          /* 错误了/到末尾了,退出 */

            res = exfuns_file_type1(tfileinfo->fname,"PNG");

            if(res==1)           /* 取高四位,看看是不是图片文件 */
            {
                rval++;                         /* 有效文件数增加1 */
            }
        }
    }

		_end_:		    Mem_free(EXSRAM, tfileinfo);                  /* 释放内存 */
        return rval;
}

int chkPngFile(void)
{
		DIR picdir;             /* 图片目录 */
	  uint16_t totpicnum=0;     /* 图片文件总数 */
	  uint32_t *picoffsettbl; /* 图片文件offset索引表 */
		FILINFO *picfileinfo;   /* 文件信息 */
	  uint8_t *pname;	
	
	  totpicnum = pic_get_pngnum("0:/PICTURE/PNG");     /* 得到总有效文件数 */
				
	  printf("000totpicnum = %d\r\n",totpicnum);	
		picoffsettbl =(uint32_t *) Mem_malloc(EXSRAM,4 * totpicnum);             /* 申请4*totpicnum个字节的内存,用于存放图片索引 */	
	  if(!picoffsettbl)
				   				printf("malloc picoffsettbl err err !!!\r\n");
		else
		printf("111malloc picoffsettbl succ !!!\r\n");
	/*	pname =(uint8_t *) malloc(FF_MAX_LFN * 2 + 1);            // 为带路径的文件名分配内存 
		if(!pname)
					printf("malloc pname err err !!!\r\n");
		else
		{

					printf("malloc pname succ !!!\r\n");	
		}*/

	 return totpicnum;
}
