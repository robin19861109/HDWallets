
/* pngrio.c - functions for data input
 *
 * Last changed in libpng 1.6.15 [November 20, 2014]
 * Copyright (c) 1998-2014 Glenn Randers-Pehrson
 * (Version 0.96 Copyright (c) 1996, 1997 Andreas Dilger)
 * (Version 0.88 Copyright (c) 1995, 1996 Guy Eric Schalnat, Group 42, Inc.)
 *
 * This code is released under the libpng license.
 * For conditions of distribution and use, see the disclaimer
 * and license in png.h
 *
 * This file provides a location for all input.  Users who need
 * special handling are expected to write a function that has the same
 * arguments as this and performs a similar function, but that possibly
 * has a different input method.  Note that you shouldn't change this
 * function, but rather write a replacement function and then make
 * libpng use it at run time with png_set_read_fn(...).
 */

#include "pngpriv.h"

#ifdef PNG_READ_SUPPORTED

/* Read the data from whatever input you are using.  The default routine
 * reads from a file pointer.  Note that this routine sometimes gets called
 * with very small lengths, so you should implement some kind of simple
 * buffering if you are using unbuffered reads.  This should never be asked
 * to read more then 64K on a 16 bit machine.
 */
void /* PRIVATE */
png_read_data(png_structrp png_ptr, png_bytep data, png_size_t length)
{
   png_debug1(4, "reading %d bytes", (int)length);

   if (png_ptr->read_data_fn != NULL)
      (*(png_ptr->read_data_fn))(png_ptr, data, length);

   else
      png_error(png_ptr, "Call to NULL read function");
}

#ifdef PNG_STDIO_SUPPORTED
/* This is the function that does the actual reading of data.  If you are
 * not reading from a standard C stream, you should create a replacement
 * read_data function and use it at run time with png_set_read_fn(), rather
 * than changing the library.
 */
void PNGCBAPI
png_default_read_data(png_structp png_ptr, png_bytep data, png_size_t length)
{
   png_size_t check;

   if (png_ptr == NULL)
      return;

   /* fread() returns 0 on error, so it is OK to store this in a png_size_t
    * instead of an int, which is what fread() actually returns.
    */
   check = fread(data, 1, length, png_voidcast(png_FILE_p, png_ptr->io_ptr));

   if (check != length)
      png_error(png_ptr, "Read Error");
}
#endif
/* 此函数允许应用程序为 libpng 提供一个新的输入函数，如果不使用标准 C 流。

该函数的参数包括：

png_ptr - 指向一个 png 输入数据结构的指针

io_ptr - 指向包含有关输入函数信息的用户提供的结构体的指针。可以为 NULL。

read_data_fn - 指向一个新的输入函数的指针，该函数的参数包括指向一个 png_struct 的指针、
一个指向可以存储输入数据的位置的指针，以及一个表示要读取的字节数的 32 位无符号整数。为了
退出并输出任何致命错误消息，新的写入函数应调用 png_error(png_ptr, "错误消息")。可以为 NULL，
如果为 NULL，则将使用 libpng 的默认函数。*/
/* This function allows the application to supply a new input function
 * for libpng if standard C streams aren't being used.
 *
 * This function takes as its arguments:
 *
 * png_ptr      - pointer to a png input data structure
 *
 * io_ptr       - pointer to user supplied structure containing info about
 *                the input functions.  May be NULL.
 *
 * read_data_fn - pointer to a new input function that takes as its
 *                arguments a pointer to a png_struct, a pointer to
 *                a location where input data can be stored, and a 32-bit
 *                unsigned int that is the number of bytes to be read.
 *                To exit and output any fatal error messages the new write
 *                function should call png_error(png_ptr, "Error msg").
 *                May be NULL, in which case libpng's default function will
 *                be used.
 */
void PNGAPI
png_set_read_fn(png_structrp png_ptr, png_voidp io_ptr,
   png_rw_ptr read_data_fn)
{
   if (png_ptr == NULL)
      return;

   png_ptr->io_ptr = io_ptr;

#ifdef PNG_STDIO_SUPPORTED
   if (read_data_fn != NULL)
      png_ptr->read_data_fn = read_data_fn;

   else
      png_ptr->read_data_fn = png_default_read_data;
#else
   png_ptr->read_data_fn = read_data_fn;
#endif

#ifdef PNG_WRITE_SUPPORTED
   /* It is an error to write to a read device */
   if (png_ptr->write_data_fn != NULL)
   {
      png_ptr->write_data_fn = NULL;
      png_warning(png_ptr,
          "Can't set both read_data_fn and write_data_fn in the"
          " same structure");
   }
#endif

#ifdef PNG_WRITE_FLUSH_SUPPORTED
   png_ptr->output_flush_fn = NULL;
#endif
}
#endif /* READ */
