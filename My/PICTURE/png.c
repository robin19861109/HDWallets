
#include "png.h"
#include "piclib.h"


const uint16_t _aMaskTbl[16] =
{
    0x0000, 0x0001, 0x0003, 0x0007,
    0x000f, 0x001f, 0x003f, 0x007f,
    0x00ff, 0x01ff, 0x03ff, 0x07ff,
    0x0fff, 0x1fff, 0x3fff, 0x7fff,
};

const uint8_t _aInterlaceOffset[] = {8, 8, 4, 2};
const uint8_t _aInterlaceYPos  [] = {0, 4, 2, 1};

uint8_t g_gif_decoding = 0; /* 标记GIF正在解码 */



gif89a tgif89a;         /* gif89a文件 */
FIL f_gfile;            /* gif 文件 */
LZW_INFO tlzw;          /* lzw */


uint8_t png_decode(const char *filename)
{
	return 0;
}








