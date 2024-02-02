#include "my_png.h"

#include "exfuns.h"
#include "malloc.h"


//Check if the PNG file exists.
char *pmyPicDir = "0:";
//char *pmyPicDir ="0:/PICTURE/PNG";
_info_PicFile info_PicFile;
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
	    DWORD temp;
     FILINFO *tfileinfo=(FILINFO *)&dtfileinfo;   /* 临时文件信息 */
	
	  printf("pic_get_pngnum path=%s\r\n",path);
    res = f_opendir(&tdir, (const TCHAR *)path);              /* 打开目录 */

    if (res == FR_OK && tfileinfo)
    {
			
			
        while (1)                               /* 查询总的有效文件数 */
        {
					  temp = tdir.dptr;                     /* 记录当前dptr偏移 */
					
            res = f_readdir(&tdir, tfileinfo);  /* 读取目录下的一个文件 */

            if (res != FR_OK || tfileinfo->fname[0] == 0)
                break;                          /* 错误了/到末尾了,退出 */
            printf("tfileinfo->fname = %s\r\n",tfileinfo->fname);
            res = exfuns_file_type1(tfileinfo->fname,"PNG");

            if(res==1)           /* 取高四位,看看是不是图片文件 */
            {
							
							  info_PicFile.picoffsettbl[rval] =temp;      /* 记录索引 */
                rval++;                         /* 有效文件数增加1 */
            }
        }
    }
   info_PicFile.num_file = rval;
    return rval;
}
	//			strcpy((char *)pname, "0:/PICTURE/");                    /* 复制路径(目录) */
  //      strcat((char *)pname, (const char *)picfileinfo->fname); /* 将文件名接在后面 */
int chkPngFile(void)
{

	  uint16_t totpicnum=0;     /* 图片文件总数 */
	  printf("pmyPicDir = %s\r\n",pmyPicDir);
	  totpicnum = pic_get_pngnum(pmyPicDir);     /* 得到总有效文件数 */
	  printf("totpicnum=%d\r\n",totpicnum);
	
	  return totpicnum;
}
