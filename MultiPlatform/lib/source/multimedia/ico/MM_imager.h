/***************************************************************************

		MM_imager.h

		defalut.

		TIME LIST:
		CREATE	Rainy	2009-02-26 12:58:23

		Copyright (c)  Yu.liang <lzysoft@126.com> 2007-2009
		All rights reserved.
***************************************************************************/
#ifndef _MM_imager_h_
#define _MM_imager_h_

#ifdef	__cplusplus
extern "C" {
#endif

ssize_t  img_fread(io_glue *ig, void *buf, size_t count);
ssize_t img_fwrite(io_glue *ig, const void *buf, size_t count);
off_t  img_fseek(io_glue *ig, off_t offset, int whence);
int    img_fclose(io_glue *ig);
ssize_t img_size (io_glue *ig);

#ifdef __cplusplus
}
#endif

#endif
