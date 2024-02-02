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
    DIR tdir;                                                 /* ��ʱĿ¼ */
	  FILINFO dtfileinfo;
	
    FILINFO *tfileinfo=(FILINFO *)&dtfileinfo;                                       /* ��ʱ�ļ���Ϣ */

  //  tfileinfo = (FILINFO *)Mem_malloc(INSRAM, sizeof(FILINFO)); /* �����ڴ� */
	//  goto  _end_;

    res = f_opendir(&tdir, (const TCHAR *)path);              /* ��Ŀ¼ */

    if (res == FR_OK && tfileinfo)
    {
        while (1)                               /* ��ѯ�ܵ���Ч�ļ��� */
        {
            res = f_readdir(&tdir, tfileinfo);  /* ��ȡĿ¼�µ�һ���ļ� */

            if (res != FR_OK || tfileinfo->fname[0] == 0)
                break;                          /* ������/��ĩβ��,�˳� */

            res = exfuns_file_type(tfileinfo->fname);

            if ((res & 0XF0) == 0X50)           /* ȡ����λ,�����ǲ���ͼƬ�ļ� */
            {
                rval++;                         /* ��Ч�ļ�������1 */
            }
        }
    }

//		_end_:		    Mem_free(INSRAM, tfileinfo);                  /* �ͷ��ڴ� */
        return rval;
}


uint16_t pic_get_pngnum(char *path)
{
    uint8_t res;
    uint16_t rval = 0;
    DIR tdir;                                                 /* ��ʱĿ¼ */
	  FILINFO dtfileinfo;
	    DWORD temp;
     FILINFO *tfileinfo=(FILINFO *)&dtfileinfo;   /* ��ʱ�ļ���Ϣ */
	
	  printf("pic_get_pngnum path=%s\r\n",path);
    res = f_opendir(&tdir, (const TCHAR *)path);              /* ��Ŀ¼ */

    if (res == FR_OK && tfileinfo)
    {
			
			
        while (1)                               /* ��ѯ�ܵ���Ч�ļ��� */
        {
					  temp = tdir.dptr;                     /* ��¼��ǰdptrƫ�� */
					
            res = f_readdir(&tdir, tfileinfo);  /* ��ȡĿ¼�µ�һ���ļ� */

            if (res != FR_OK || tfileinfo->fname[0] == 0)
                break;                          /* ������/��ĩβ��,�˳� */
            printf("tfileinfo->fname = %s\r\n",tfileinfo->fname);
            res = exfuns_file_type1(tfileinfo->fname,"PNG");

            if(res==1)           /* ȡ����λ,�����ǲ���ͼƬ�ļ� */
            {
							
							  info_PicFile.picoffsettbl[rval] =temp;      /* ��¼���� */
                rval++;                         /* ��Ч�ļ�������1 */
            }
        }
    }
   info_PicFile.num_file = rval;
    return rval;
}
	//			strcpy((char *)pname, "0:/PICTURE/");                    /* ����·��(Ŀ¼) */
  //      strcat((char *)pname, (const char *)picfileinfo->fname); /* ���ļ������ں��� */
int chkPngFile(void)
{

	  uint16_t totpicnum=0;     /* ͼƬ�ļ����� */
	  printf("pmyPicDir = %s\r\n",pmyPicDir);
	  totpicnum = pic_get_pngnum(pmyPicDir);     /* �õ�����Ч�ļ��� */
	  printf("totpicnum=%d\r\n",totpicnum);
	
	  return totpicnum;
}
