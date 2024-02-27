/* inftrees.c -- generate Huffman trees for efficient decoding
 * Copyright (C) 1995-2013 Mark Adler
 * For conditions of distribution and use, see copyright notice in zlib.h
 */

#include "zutil.h"
#include "inftrees.h"

#define MAXBITS 15

const char inflate_copyright[] =
   " inflate 1.2.8 Copyright 1995-2013 Mark Adler ";
/*

如果您在产品中使用zlib库，欢迎在产品文档中包含致谢。如果由于某种原因您无法
包含这样的致谢，我希望您能在产品的可执行文件中保留这个版权字符串。
  If you use the zlib library in a product, an acknowledgment is welcome
  in the documentation of your product. If for some reason you cannot
  include such an acknowledgment, I would appreciate that you keep this
  copyright string in the executable of your product.
 */

/*
构建一组表以解码提供的规范哈夫曼编码。
代码长度存储在lens[0..codes-1]中。结果从*table开始，其索引为0..2^bits-1。
work是一个至少有lens个short整数的可写数组，用作工作区域。type是要生成的代码
类型，可以是CODES、LENS或DISTS。返回值为零表示成功，-1表示无效代码，+1表示
空间不足。返回时，table指向下一个可用条目的地址。bits是请求的根表索引位数，
返回时它是实际的根表索引位数。如果请求大于最长代码或小于最短代码，则它将不
同。

   Build a set of tables to decode the provided canonical Huffman code.
   The code lengths are lens[0..codes-1].  The result starts at *table,
   whose indices are 0..2^bits-1.  work is a writable array of at least
   lens shorts, which is used as a work area.  type is the type of code
   to be generated, CODES, LENS, or DISTS.  On return, zero is success,
   -1 is an invalid code, and +1 means that ENOUGH isn't enough.  table
   on return points to the next available entry's address.  bits is the
   requested root table index bits, and on return it is the actual root
   table index bits.  It will differ if the request is greater than the
   longest code or if it is less than the shortest code.
 */

/*int ZLIB_INTERNAL inflate_table(codetype type,
                                unsigned short FAR *lens,
                                unsigned codes,
                                code FAR * FAR *table,
                                unsigned FAR *bits,
                                unsigned short FAR *work)
{*/
/*inflate_table(CODES, state->lens, 19, &(state->next),
                                &(state->lenbits), state->work);*/
int ZLIB_INTERNAL inflate_table(type, lens, codes, table, bits1, work)
codetype type;
unsigned short FAR *lens;
unsigned codes;
code FAR * FAR *table;
unsigned FAR *bits1;
unsigned short FAR *work;
{
    unsigned len1;               /* a code's length in bits 代码的长度（以位为单位）*/
    unsigned sym1;               /* index of code symbols 代码符号的索引*/
    unsigned min, max;          /* minimum and maximum code lengths 代码长度的最小值和最大值*/
    unsigned root;              /* number of index bits for root table 根表的索引位数*/
    unsigned curr;              /* number of index bits for current table 当前表的索引位数*/
    unsigned drop;              /* code bits to drop for sub-table 用于子表的要丢弃的代码位数*/
    int left;                   /* number of prefix codes available 可用的前缀代码数量*/
    unsigned used;              /* code entries in table used 表中已使用的代码条目*/
    unsigned huff;              /* Huffman code 哈夫曼编码*/
    unsigned incr;              /* for incrementing code, index 用于增加代码和索引*/
    unsigned fill;              /* index for replicating entries 复制条目的索引*/
    unsigned low;               /* low bits for current root entry 当前根条目的低位*/
    unsigned mask;              /* mask for low root bits 低位根码的掩码*/
    code here;                  /* table entry for duplication 用于复制的表条目*/
    code FAR *next;             /* next available space in table 表中下一个可用空间*/
    const unsigned short FAR *base;     /* base value table to use 要使用的基础值表*/
    const unsigned short FAR *extra;    /* extra bits table to use 要使用的额外位表*/
    int end;                    /* use base and extra for symbol > end 对于符号 > end，使用基础值和额外位*/
    unsigned short count[MAXBITS+1];    /* number of codes of each length 每个长度的代码数量*/
    unsigned short offs[MAXBITS+1];     /* offsets in table for each length 每个长度在表中的偏移量*/
    static const unsigned short lbase[31] = { /* Length codes 257..285 base 长度代码 257..285 的基础*/
        3, 4, 5, 6, 7, 8, 9, 10, 11, 13, 15, 17, 19, 23, 27, 31,
        35, 43, 51, 59, 67, 83, 99, 115, 131, 163, 195, 227, 258, 0, 0};
    static const unsigned short lext[31] = { /* Length codes 257..285 extra */
        16, 16, 16, 16, 16, 16, 16, 16, 17, 17, 17, 17, 18, 18, 18, 18,
        19, 19, 19, 19, 20, 20, 20, 20, 21, 21, 21, 21, 16, 72, 78};
    static const unsigned short dbase[32] = { /* Distance codes 0..29 base */
        1, 2, 3, 4, 5, 7, 9, 13, 17, 25, 33, 49, 65, 97, 129, 193,
        257, 385, 513, 769, 1025, 1537, 2049, 3073, 4097, 6145,
        8193, 12289, 16385, 24577, 0, 0};
    static const unsigned short dext[32] = { /* Distance codes 0..29 extra */
        16, 16, 16, 16, 17, 17, 18, 18, 19, 19, 20, 20, 21, 21, 22, 22,
        23, 23, 24, 24, 25, 25, 26, 26, 27, 27,
        28, 28, 29, 29, 64, 64};

    /*
		处理一组代码长度以创建一个规范的哈夫曼编码。代码长度存储在lens[0..codes-1]中。每个长度对应于符号0..codes-1。
		哈夫曼编码是通过首先按长度从短到长对符号进行排序，并保留相同长度的代码的符号顺序来生成的。然后，编码以最短长度的
		第一个代码的所有零位开始，并且相同长度的代码是整数增量，随着长度的增加会追加零位。对于deflate格式，这些位是以它们
		更自然的整数增量顺序相反存储的，因此在下面的大循环中构建解码表时，整数编码是以相反顺序增加的。

   此程序假设（但不检查）lens[]中的所有条目都在0..MAXBITS范围内。调用者必须确保这一点。
	1..MAXBITS被解释为该代码长度。零表示该符号在此代码中不存在。

   通过计算每个长度的代码计数来对代码进行排序，从而创建一个用于在排序表中每个长度的起始索引的表，
	然后按顺序将符号输入到排序表中。排序表是work[]，调用者提供了该空间。

   长度计数还用于其他目的，例如查找最小和最大长度代码，确定是否存在任何代码，检查有效长度集，并在构建解码表时提前查看
	长度计数以确定子表大小。
	
				
				
       Process a set of code lengths to create a canonical Huffman code.  The
       code lengths are lens[0..codes-1].  Each length corresponds to the
       symbols 0..codes-1.  The Huffman code is generated by first sorting the
       symbols by length from short to long, and retaining the symbol order
       for codes with equal lengths.  Then the code starts with all zero bits
       for the first code of the shortest length, and the codes are integer
       increments for the same length, and zeros are appended as the length
       increases.  For the deflate format, these bits are stored backwards
       from their more natural integer increment ordering, and so when the
       decoding tables are built in the large loop below, the integer codes
       are incremented backwards.

       This routine assumes, but does not check, that all of the entries in
       lens[] are in the range 0..MAXBITS.  The caller must assure this.
       1..MAXBITS is interpreted as that code length.  zero means that that
       symbol does not occur in this code.

       The codes are sorted by computing a count of codes for each length,
       creating from that a table of starting indices for each length in the
       sorted table, and then entering the symbols in order in the sorted
       table.  The sorted table is work[], with that space being provided by
       the caller.

       The length counts are used for other purposes as well, i.e. finding
       the minimum and maximum length codes, determining if there are any
       codes at all, checking for a valid set of lengths, and looking ahead
       at length counts to determine sub-table sizes when building the
       decoding tables.
     */

    /* accumulate lengths for codes (assumes lens[] all in 0..MAXBITS) 累积代码长度（假设 lens[] 都在 0 到 MAXBITS 之间）*/
   //lens[] 为符号表中符号的码长度 count[]为每个码长度的个数，0为此码没有出现

for (len1 = 0; len1 <= MAXBITS; len1++)//MAXBITS=15
        count[len1] = 0;
    for (sym1 = 0; sym1 < codes; sym1++)//codes=19
        count[lens[sym1]]++;

    /* bound code lenzgths, force root to be within code lengths 限制代码长度，强制根节点在代码长度范围内*/
    root = *bits1;
    for (max = MAXBITS; max >= 1; max--)
        if (count[max] != 0) break;
    if (root > max) root = max;
    if (max == 0) {                     /* no symbols to code at all */
        here.op = (unsigned char)64;    /* invalid code marker */
        here.bits = (unsigned char)1;
        here.val = (unsigned short)0;
        *(*table)++ = here;             /* make a table to force an error */
        *(*table)++ = here;
        *bits1 = 1;
        return 0;     /* no symbols, but wait for decoding to report error */
    }
    for (min = 1; min < max; min++)
        if (count[min] != 0) break;
    if (root < min) root = min;

    /* check for an over-subscribed or incomplete set of lengths 检查是否存在过度订阅或不完整的长度集合*/
							{
							int ii=0;
						  printf("count[len]:\r\n");
							for(ii=1;ii<=MAXBITS;ii++)
							{
								printf("%d ",count[ii]);
							}
							printf("\r\n");
						}	
    left = 1;
    for (len1 = 1; len1 <= MAXBITS; len1++) {
        left <<= 1;
        left -= count[len1];
        if (left < 0) return -1;        /* over-subscribed */
    }
    if (left > 0 && (type == CODES || max != 1))
        return -1;                      /* incomplete set */

    /* generate offsets into symbol table for each length for sorting 为每个长度生成符号表的偏移量，以便排序*/
   


		offs[1] = 0;
    for (len1 = 1; len1 < MAXBITS; len1++)
		{
        offs[len1 + 1] = offs[len1] + count[len1];
	//		  printf("len+1 = %d  offs[len + 1]=%d  offs[len]=%d  count[len]=%d\r\n",len1+1, offs[len1 + 1],  offs[len1],count[len1]);
		}
		
							{
							int ii=0;
						  printf("offs[len]:\r\n");
							for(ii=1;ii<=MAXBITS;ii++)
							{
								printf(" %d",offs[ii]);
							}
							printf("\r\n");
						}			
    printf("ppppppppppppppppppp\r\n");
    /* sort symbols by length, by symbol order within each length 按照长度以及每个长度内的符号顺序对符号进行排序*/
   
  //  for (sym = 0; sym < codes; sym++)
 //       if (lens[sym] != 0) work[offs[lens[sym]]++] = (unsigned short)sym;


    for (sym1 = 0; sym1 < codes; sym1++)
		{
	//printf("lens[%d]=%d	 offs[%d]=%d  work[%d]=%d\r\n",sym,lens[sym],lens[sym],offs[lens[sym]],offs[lens[sym]],work[offs[lens[sym]]]);
					
        if (lens[sym1] != 0) 
				{

					work[offs[lens[sym1]]] = (unsigned short)sym1;
					
	//				printf("lens[%d]=%d	 offs[%d]=%d  work[%d]=%d\r\n",sym,lens[sym],lens[sym],offs[lens[sym]],offs[lens[sym]],work[offs[lens[sym]]]);
					offs[lens[sym1]]++;
				}		
							
		}
    {
							int ii=0;
						  printf("work[]:\r\n");
							for(ii=0;ii<codes;ii++)
							{
								printf(" %d",work[ii]);
							}
							printf("\r\n");
		}	

					
    /*
		
	### 创建并填充解码表。在此循环中，被填充的表位于 next，并且具有 curr 索引位数。正在使用的代码是具有长度 len 的 huff。通过从底部删除 drop 位，
		将该代码转换为索引。对于长度小于 drop + curr 的代码，通过递增顶部的 drop + curr - len 位来填充表以复制条目。

### root 是根表的索引位数。当 len 超过 root 时，将创建由 root 条目指向的子表，其索引为 huff 的低 root 位。这被保存在 low 中，以检查何时应开始新的子表。
		当填充根表时，drop 为零，当填充子表时，drop 为 root。

### 当需要新的子表时，需要向前查看代码长度，以确定需要什么大小的子表。length 计数用于此目的，因此随着代码输入到表中，count[] 递减。

### used 跟踪从提供的 *table 空间中分配了多少表条目。对于 LENS 和 DIST 表，针对常量 ENOUGH_LENS 和 ENOUGH_DISTS 进行检查，以防止初始根表大小常量发
生变化。有关更多信息，请参阅 inftrees.h 文件中的注释。

### sym 递增遍历所有符号，并且循环在所有长度为 max 的代码（即所有代码）被处理时终止。此例程允许不完整的代码，因此在此后的另一个循环中，将使用无效代码标
记来填充解码表的其余部分。
	
		
		
       Create and fill in decoding tables.  In this loop, the table being
       filled is at next and has curr index bits.  The code being used is huff
       with length len.  That code is converted to an index by dropping drop
       bits off of the bottom.  For codes where len is less than drop + curr,
       those top drop + curr - len bits are incremented through all values to
       fill the table with replicated entries.

       root is the number of index bits for the root table.  When len exceeds
       root, sub-tables are created pointed to by the root entry with an index
       of the low root bits of huff.  This is saved in low to check for when a
       new sub-table should be started.  drop is zero when the root table is
       being filled, and drop is root when sub-tables are being filled.

       When a new sub-table is needed, it is necessary to look ahead in the
       code lengths to determine what size sub-table is needed.  The length
       counts are used for this, and so count[] is decremented as codes are
       entered in the tables.

       used keeps track of how many table entries have been allocated from the
       provided *table space.  It is checked for LENS and DIST tables against
       the constants ENOUGH_LENS and ENOUGH_DISTS to guard against changes in
       the initial root table size constants.  See the comments in inftrees.h
       for more information.

       sym increments through all symbols, and the loop terminates when
       all codes of length max, i.e. all codes, have been processed.  This
       routine permits incomplete codes, so another loop after this one fills
       in the rest of the decoding tables with invalid code markers.
     */

    /* set up for code type */
    switch (type) {
    case CODES:
        base = extra = work;    /* dummy value--not used */
        end = 19;
        break;
    case LENS:
        base = lbase;
        base -= 257;
        extra = lext;
        extra -= 257;
        end = 256;
        break;
    default:            /* DISTS */
        base = dbase;
        extra = dext;
        end = -1;
    }

    /* initialize state for loop */
		
		unsigned len;
    huff = 0;                   /* starting code 起始编码*/
    int sym = 0;                    /* starting code symbol 起始编码符号*/
    len = min;                  /* starting code length 起始编码长度，当前的编码长度*/
    next = *table;              /* current table to fill in 当前要填充的表*/
    curr = root;                /* current table index bits 当前表索引位，当前出现次数最多的符号的出现次数，理应编码长度最短*/
    drop = 0;                   /* current bits to drop from code for index 从编码中为索引丢弃的当前位数 */
    low = (unsigned)(-1);       /* trigger new sub-table when len > root 当 len > root 时，触发新的子表*/
    used = 1U << root;          /* use root table entries 使用根表条目*/
    mask = used - 1;            /* mask for comparing low 用于比较 low 的掩码*/

    /* check available table space */
    if ((type == LENS && used > ENOUGH_LENS) ||
        (type == DISTS && used > ENOUGH_DISTS))
        return 1;

    /* process all codes and make table entries 处理所有编码并生成表条目*/
    for (;;) {
        /* create table entry */
        here.bits = (unsigned char)(len - drop);
        if ((int)(work[sym]) < end) {
            here.op = (unsigned char)0;
            here.val = work[sym];
        }
        else if ((int)(work[sym]) > end) {
            here.op = (unsigned char)(extra[work[sym]]);
            here.val = base[work[sym]];
        }
        else {
            here.op = (unsigned char)(32 + 64);         /* end of block */
            here.val = 0;
        }
				 
				printf("type=%d here.bits=%d  here.op=%d  here.val=%d \r\n",type, here.bits, here.op, here.val);
 
        /* replicate for those indices with low len bits equal to huff 复制那些索引的低 len 位等于 huff 的条目*/
				
	        incr = 1U << (len - drop);
        fill = 1U << curr;
        min = fill;                 /* save offset to next table */
        do {
            fill -= incr;
            next[(huff >> drop) + fill] = here;
        } while (fill != 0);

        /* backwards increment the len-bit code huff 向后递增长度为len的哈夫曼编码*/
        incr = 1U << (len - 1);
        while (huff & incr)
            incr >>= 1;
        if (incr != 0) {
            huff &= incr - 1;
            huff += incr;
        }
        else
            huff = 0;			
	
        /* go to next symbol, update count, len 跳转到下一个符号，更新计数和长度*/
        sym++;
        if (--(count[len]) == 0) {
            if (len == max) 
							break;
            len = lens[work[sym]];
        }

        /* create new sub-table if needed 如果需要，创建新的子表*/
        if (len > root && (huff & mask) != low) {
            /* if first time, transition to sub-tables 如果是第一次，则转移到子表*/
            if (drop == 0)
                drop = root;

            /* increment past last table 将指针增加到最后一个表之后*/
            next += min;            /* here min is 1 << curr */

            /* determine length of next table 确定下一个表的长度*/
            curr = len - drop;
            left = (int)(1 << curr);
            while (curr + drop < max) {
                left -= count[curr + drop];
                if (left <= 0) break;
                curr++;
                left <<= 1;
            }

            /* check for enough space */
            used += 1U << curr;
            if ((type == LENS && used > ENOUGH_LENS) ||
                (type == DISTS && used > ENOUGH_DISTS))
                return 1;

            /* point entry in root table to sub-table 将根表中的条目指向子表*/
            low = huff & mask;
            (*table)[low].op = (unsigned char)curr;
            (*table)[low].bits = (unsigned char)root;
            (*table)[low].val = (unsigned short)(next - *table);
        }
    }

    /* 
		   如果代码不完整，则填充剩余的表项（保证最多只有一个剩余的表项，因为如果代码
		   不完整，到目前为止允许的最大代码长度是一位） 

		   fill in remaining table entry if code is incomplete (guaranteed to have
       at most one remaining entry, since if the code is incomplete, the
       maximum code length that was allowed to get this far is one bit) */
    if (huff != 0) {
        here.op = (unsigned char)64;            /* invalid code marker */
        here.bits = (unsigned char)(len - drop);
        here.val = (unsigned short)0;
        next[huff] = here;
    }

    /* set return parameters */
    *table += used;
    *bits1 = root;
    return 0;
}
