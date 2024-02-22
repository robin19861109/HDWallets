/* inftrees.h -- header to use inftrees.c
 * Copyright (C) 1995-2005, 2010 Mark Adler
 * For conditions of distribution and use, see copyright notice in zlib.h
 */

/* WARNING: this file should *not* be used by applications. It is
   part of the implementation of the compression library and is
   subject to change. Applications should only use zlib.h.
 */

/* 
   每个条目提供执行由索引该表条目的代码请求的操作所需的信息，或者提
	 供指向另一个表的指针，该表索引更多的代码位。op 指示条目是指向另一
	 个表、字面值、长度或距离、块结束，还是无效代码。对于表指针，op 的
	 低四位是该表的索引位数。对于长度或距离，op 的低四位是代码后的额外
	 位数。bits 是要从位缓冲器中删除的代码或代码部分的位数。val 是字面值、
	 基本长度或距离，或者是从当前表到下一个表的偏移量。每个条目占四个字节。

   Structure for decoding tables.  Each entry provides either the
   information needed to do the operation requested by the code that
   indexed that table entry, or it provides a pointer to another
   table that indexes more bits of the code.  op indicates whether
   the entry is a pointer to another table, a literal, a length or
   distance, an end-of-block, or an invalid code.  For a table
   pointer, the low four bits of op is the number of index bits of
   that table.  For a length or distance, the low four bits of op
   is the number of extra bits to get after the code.  bits is
   the number of bits in this code or part of the code to drop off
   of the bit buffer.  val is the actual byte to output in the case
   of a literal, the base length or distance, or the offset from
   the current table to the next table.  Each entry is four bytes. */
typedef struct {
    unsigned char op;           /* operation, extra bits, table bits 操作，额外位数，表位数*/
    unsigned char bits;         /* bits in this part of the code 该代码部分中的位数*/
    unsigned short val;         /* offset in table or code value 表中的偏移量或代码值*/
} code;

/* op values as set by inflate_table():
    00000000 - literal
    0000tttt - table link, tttt != 0 is the number of table index bits
    0001eeee - length or distance, eeee is the number of extra bits
    01100000 - end of block
    01000000 - invalid code
 */

/* 
### 动态表的最大尺寸。代码结构的最大数量是 1444，其中 852 用于字面/长度代码，
592 用于距离代码。通过使用 zlib 发行版中的程序 examples/enough.c 进行详尽搜索，
找到了这些值。该程序的参数是符号数量、初始根表大小和代码的最大位长度。对于字面/长度代码，
"enough 286 9 15" 返回 852；对于距离代码，"enough 30 6 15" 返回 592。初始根表大小（9 或 6）
位于 inflate.c 和 infback.c 中的 inflate_table() 调用的第五个参数中。如果更改根表大小，
那么这些最大尺寸需要重新计算和更新。


   Maximum size of the dynamic table.  The maximum number of code structures is
   1444, which is the sum of 852 for literal/length codes and 592 for distance
   codes.  These values were found by exhaustive searches using the program
   examples/enough.c found in the zlib distribtution.  The arguments to that
   program are the number of symbols, the initial root table size, and the
   maximum bit length of a code.  "enough 286 9 15" for literal/length codes
   returns returns 852, and "enough 30 6 15" for distance codes returns 592.
   The initial root table size (9 or 6) is found in the fifth argument of the
   inflate_table() calls in inflate.c and infback.c.  If the root table size is
   changed, then these maximum sizes would be need to be recalculated and
   updated. */
#define ENOUGH_LENS 852
#define ENOUGH_DISTS 592
#define ENOUGH (ENOUGH_LENS+ENOUGH_DISTS)

/* Type of code to build for inflate_table() */
typedef enum {
    CODES,
    LENS,
    DISTS
} codetype;

int ZLIB_INTERNAL inflate_table OF((codetype type, unsigned short FAR *lens,
                             unsigned codes, code FAR * FAR *table,
                             unsigned FAR *bits, unsigned short FAR *work));
