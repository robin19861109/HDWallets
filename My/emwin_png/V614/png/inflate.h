/* inflate.h -- internal inflate state definition
 * Copyright (C) 1995-2009 Mark Adler
 * For conditions of distribution and use, see copyright notice in zlib.h
 */

/* WARNING: this file should *not* be used by applications. It is
   part of the implementation of the compression library and is
   subject to change. Applications should only use zlib.h.
 */

/* define NO_GZIP when compiling if you want to disable gzip header and
   trailer decoding by inflate().  NO_GZIP would be used to avoid linking in
   the crc code when it is not needed.  For shared libraries, gzip decoding
   should be left enabled. */
#ifndef NO_GZIP
#  define GUNZIP
#endif

/* Possible inflate modes between inflate() calls */
typedef enum {
    HEAD,       /* i: waiting for magic header 等待魔术头部*/
    FLAGS,      /* i: waiting for method and flags (gzip) 等待方法和标志位（gzip）*/
    TIME,       /* i: waiting for modification time (gzip) 等待修改时间（gzip）*/
    OS,         /* i: waiting for extra flags and operating system (gzip) 3OS: 等待额外标志和操作系统（gzip）*/
    EXLEN,      /* i: waiting for extra length (gzip) 4 EXLEN: 等待额外长度（gzip）*/
    EXTRA,      /* i: waiting for extra bytes (gzip) 5 EXTRA: 等待额外字节（gzip）*/
    NAME,       /* i: waiting for end of file name (gzip) 6NAME: 等待文件名结束（gzip）*/
    COMMENT,    /* i: waiting for end of comment (gzip) 7 COMMENT: 等待注释结束（gzip）*/
    HCRC,       /* i: waiting for header crc (gzip) 8HCRC: 等待头部 CRC（gzip）*/
    DICTID,     /* i: waiting for dictionary check value 9DICTID: 等待字典检查值*/
    DICT,       /* waiting for inflateSetDictionary() call a DICT: 等待 inflateSetDictionary() 调用*/
        TYPE,       /* i: waiting for type bits, including last-flag bit b TYPE: 等待类型位，包括最后标志位*/
        TYPEDO,     /* i: same, but skip check to exit inflate on new block c TYPEDO: 同上，但跳过检查以退出新块上的解压*/
        STORED,     /* i: waiting for stored size (length and complement) d STORED: 等待存储大小（长度和补码）*/
        COPY_,      /* i/o: same as COPY below, but only first time in 输入或输出等待复制存储块，但仅第一次*/
        COPY,       /* i/o: waiting for input or output to copy stored block 等待输入或输出以复制存储块*/
        TABLE,      /* i: waiting for dynamic block table lengths 等待动态块表长度*/
        LENLENS,    /* i: waiting for code length code lengths 等待代码长度代码长度*/
        CODELENS,   /* i: waiting for length/lit and distance code lengths 等待长度/字面和距离代码长度 */
            LEN_,       /* i: same as LEN below, but only first time in 同下，但仅第一次*/
            LEN,        /* i: waiting for length/lit/eob code 等待长度/字面/结束码*/
            LENEXT,     /* i: waiting for length extra bits 等待长度额外位*/
            DIST,       /* i: waiting for distance code 等待距离代码*/
            DISTEXT,    /* i: waiting for distance extra bits  等待距离额外位*/
            MATCH,      /* o: waiting for output space to copy string 等待输出空间以复制字符串*/
            LIT,        /* o: waiting for output space to write literal 等待输出空间以写入字面值*/
    CHECK,      /* i: waiting for 32-bit check value  等待32位检查值*/
    LENGTH,     /* i: waiting for 32-bit length (gzip) 等待32位长度（gzip）*/
    DONE,       /* finished check, done -- remain here until reset 完成检查，完成 - 保持在此状态直到重置*/
    BAD,        /* got a data error -- remain here until reset 发生数据错误 - 保持在此状态直到重置*/
    MEM,        /* got an inflate() memory error -- remain here until reset发生 inflate() 内存错误 - 保持在此状态直到重置 */
    SYNC        /* looking for synchronization bytes to restart inflate() 寻找同步字节以重新启动 inflate()*/
} inflate_mode;

/*
    State transitions between above modes -

    (most modes can go to BAD or MEM on error -- not shown for clarity)

    Process header:
        HEAD -> (gzip) or (zlib) or (raw)
        (gzip) -> FLAGS -> TIME -> OS -> EXLEN -> EXTRA -> NAME -> COMMENT ->
                  HCRC -> TYPE
        (zlib) -> DICTID or TYPE
        DICTID -> DICT -> TYPE
        (raw) -> TYPEDO
    Read deflate blocks:
            TYPE -> TYPEDO -> STORED or TABLE or LEN_ or CHECK
            STORED -> COPY_ -> COPY -> TYPE
            TABLE -> LENLENS -> CODELENS -> LEN_
            LEN_ -> LEN
    Read deflate codes in fixed or dynamic block:
                LEN -> LENEXT or LIT or TYPE
                LENEXT -> DIST -> DISTEXT -> MATCH -> LEN
                LIT -> LEN
    Process trailer:
        CHECK -> LENGTH -> DONE
 */
/* 在 inflate() 调用之间维护的状态。大约 10K 字节。 */
struct inflate_state {
    inflate_mode mode;          /* 当前的解压模式 */
    int last;                   /* 如果正在处理最后一个块，则为 true */
    int wrap;                   /* zlib 为真的第 0 位，gzip 为真的第 1 位 */
    int havedict;               /* 如果提供了字典，则为 true */
    int flags;                  /* gzip 头部方法和标志（如果是 zlib 则为 0） */
    unsigned dmax;              /* zlib 头部最大距离（INFLATE_STRICT） */
    unsigned long check;        /* 校验值的受保护副本 */
    unsigned long total;        /* 输出计数的受保护副本 */
    gz_headerp head;            /* 保存 gzip 头部信息的位置 */
        /* 滑动窗口 */
    unsigned wbits;             /* 请求窗口大小的对数 */
    unsigned wsize;             /* 窗口大小，如果不使用窗口则为零 */
    unsigned whave;             /* 窗口中有效字节 */
    unsigned wnext;             /* 窗口写入索引 */
    unsigned char FAR *window;  /* 分配的滑动窗口，如果需要的话 */
        /* 位累加器 */
    unsigned long hold;         /* 输入位累加器 */
    unsigned bits;              /* "in" 中的位数 */
        /* 用于字符串和存储块复制 */
    unsigned length;            /* 要复制的数据的文字或长度 */
    unsigned offset;            /* 从中复制字符串的距离 */
        /* 用于表和代码解码 */
    unsigned extra;             /* 需要的额外位 */
        /* 固定和动态代码表 */
    code const FAR *lencode;    /* 长度/文字代码的起始表 */
    code const FAR *distcode;   /* 距离代码的起始表 */
    unsigned lenbits;           /* lencode 的索引位 */
    unsigned distbits;          /* distcode 的索引位 */
        /* 动态表构建 */
    unsigned ncode;             /* 代码长度代码长度 */
    unsigned nlen;              /* 长度代码长度 */
    unsigned ndist;             /* 距离代码长度 */
    unsigned have;              /* lens[] 中的代码长度数 */
    code FAR *next;             /* codes[] 中的下一个可用空间 */
    unsigned short lens[320];   /* 代码长度的临时存储 */
    unsigned short work[288];   /* 用于代码表构建的工作区域 */
    code codes[ENOUGH];         /* 代码表的空间 */
    int sane;                   /* 如果为 false，则允许无效距离过远 */
    int back;                   /* 未处理的最后一个长度/文字的位数 */
    unsigned was;               /* 匹配的初始长度 */
};

