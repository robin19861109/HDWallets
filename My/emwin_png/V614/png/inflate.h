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
    HEAD,       /* i: waiting for magic header �ȴ�ħ��ͷ��*/
    FLAGS,      /* i: waiting for method and flags (gzip) �ȴ������ͱ�־λ��gzip��*/
    TIME,       /* i: waiting for modification time (gzip) �ȴ��޸�ʱ�䣨gzip��*/
    OS,         /* i: waiting for extra flags and operating system (gzip) 3OS: �ȴ������־�Ͳ���ϵͳ��gzip��*/
    EXLEN,      /* i: waiting for extra length (gzip) 4 EXLEN: �ȴ����ⳤ�ȣ�gzip��*/
    EXTRA,      /* i: waiting for extra bytes (gzip) 5 EXTRA: �ȴ������ֽڣ�gzip��*/
    NAME,       /* i: waiting for end of file name (gzip) 6NAME: �ȴ��ļ���������gzip��*/
    COMMENT,    /* i: waiting for end of comment (gzip) 7 COMMENT: �ȴ�ע�ͽ�����gzip��*/
    HCRC,       /* i: waiting for header crc (gzip) 8HCRC: �ȴ�ͷ�� CRC��gzip��*/
    DICTID,     /* i: waiting for dictionary check value 9DICTID: �ȴ��ֵ���ֵ*/
    DICT,       /* waiting for inflateSetDictionary() call a DICT: �ȴ� inflateSetDictionary() ����*/
        TYPE,       /* i: waiting for type bits, including last-flag bit b TYPE: �ȴ�����λ����������־λ*/
        TYPEDO,     /* i: same, but skip check to exit inflate on new block c TYPEDO: ͬ�ϣ�������������˳��¿��ϵĽ�ѹ*/
        STORED,     /* i: waiting for stored size (length and complement) d STORED: �ȴ��洢��С�����ȺͲ��룩*/
        COPY_,      /* i/o: same as COPY below, but only first time in ���������ȴ����ƴ洢�飬������һ��*/
        COPY,       /* i/o: waiting for input or output to copy stored block �ȴ����������Ը��ƴ洢��*/
        TABLE,      /* i: waiting for dynamic block table lengths �ȴ���̬�����*/
        LENLENS,    /* i: waiting for code length code lengths �ȴ����볤�ȴ��볤��*/
        CODELENS,   /* i: waiting for length/lit and distance code lengths �ȴ�����/����;�����볤�� */
            LEN_,       /* i: same as LEN below, but only first time in ͬ�£�������һ��*/
            LEN,        /* i: waiting for length/lit/eob code �ȴ�����/����/������*/
            LENEXT,     /* i: waiting for length extra bits �ȴ����ȶ���λ*/
            DIST,       /* i: waiting for distance code �ȴ��������*/
            DISTEXT,    /* i: waiting for distance extra bits  �ȴ��������λ*/
            MATCH,      /* o: waiting for output space to copy string �ȴ�����ռ��Ը����ַ���*/
            LIT,        /* o: waiting for output space to write literal �ȴ�����ռ���д������ֵ*/
    CHECK,      /* i: waiting for 32-bit check value  �ȴ�32λ���ֵ*/
    LENGTH,     /* i: waiting for 32-bit length (gzip) �ȴ�32λ���ȣ�gzip��*/
    DONE,       /* finished check, done -- remain here until reset ��ɼ�飬��� - �����ڴ�״ֱ̬������*/
    BAD,        /* got a data error -- remain here until reset �������ݴ��� - �����ڴ�״ֱ̬������*/
    MEM,        /* got an inflate() memory error -- remain here until reset���� inflate() �ڴ���� - �����ڴ�״ֱ̬������ */
    SYNC        /* looking for synchronization bytes to restart inflate() Ѱ��ͬ���ֽ����������� inflate()*/
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
/* �� inflate() ����֮��ά����״̬����Լ 10K �ֽڡ� */
struct inflate_state {
    inflate_mode mode;          /* ��ǰ�Ľ�ѹģʽ */
    int last;                   /* ������ڴ������һ���飬��Ϊ true */
    int wrap;                   /* zlib Ϊ��ĵ� 0 λ��gzip Ϊ��ĵ� 1 λ */
    int havedict;               /* ����ṩ���ֵ䣬��Ϊ true */
    int flags;                  /* gzip ͷ�������ͱ�־������� zlib ��Ϊ 0�� */
    unsigned dmax;              /* zlib ͷ�������루INFLATE_STRICT�� */
    unsigned long check;        /* У��ֵ���ܱ������� */
    unsigned long total;        /* ����������ܱ������� */
    gz_headerp head;            /* ���� gzip ͷ����Ϣ��λ�� */
        /* �������� */
    unsigned wbits;             /* ���󴰿ڴ�С�Ķ��� */
    unsigned wsize;             /* ���ڴ�С�������ʹ�ô�����Ϊ�� */
    unsigned whave;             /* ��������Ч�ֽ� */
    unsigned wnext;             /* ����д������ */
    unsigned char FAR *window;  /* ����Ļ������ڣ������Ҫ�Ļ� */
        /* λ�ۼ��� */
    unsigned long hold;         /* ����λ�ۼ��� */
    unsigned bits;              /* "in" �е�λ�� */
        /* �����ַ����ʹ洢�鸴�� */
    unsigned length;            /* Ҫ���Ƶ����ݵ����ֻ򳤶� */
    unsigned offset;            /* ���и����ַ����ľ��� */
        /* ���ڱ�ʹ������ */
    unsigned extra;             /* ��Ҫ�Ķ���λ */
        /* �̶��Ͷ�̬����� */
    code const FAR *lencode;    /* ����/���ִ������ʼ�� */
    code const FAR *distcode;   /* ����������ʼ�� */
    unsigned lenbits;           /* lencode ������λ */
    unsigned distbits;          /* distcode ������λ */
        /* ��̬���� */
    unsigned ncode;             /* ���볤�ȴ��볤�� */
    unsigned nlen;              /* ���ȴ��볤�� */
    unsigned ndist;             /* ������볤�� */
    unsigned have;              /* lens[] �еĴ��볤���� */
    code FAR *next;             /* codes[] �е���һ�����ÿռ� */
    unsigned short lens[320];   /* ���볤�ȵ���ʱ�洢 */
    unsigned short work[288];   /* ���ڴ�������Ĺ������� */
    code codes[ENOUGH];         /* �����Ŀռ� */
    int sane;                   /* ���Ϊ false����������Ч�����Զ */
    int back;                   /* δ��������һ������/���ֵ�λ�� */
    unsigned was;               /* ƥ��ĳ�ʼ���� */
};

