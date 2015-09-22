/**     @file includes.h
 *
 *      Global settings.
 *
 *      @author Copyright Robert van Engelen, March 2006
 **/

#ifndef INCLUDES_H
#define INCLUDES_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <assert.h>
#include <fstream>
#include <unistd.h>


/// The maximum file name length.
#define MAX_FILE_NAME	(16)
/// The minumum block size, which must also exactly divide the block size of each device.
#define MIN_BLOCK_SIZE	(MAX_FILE_NAME + 16)
/// The maximum block size.
#define MAX_BLOCK_SIZE	(8192)
/// The maximum number of blocks for a device (just to avoid over-allocation).
#define MAX_NUM_BLOCKS	(65536)
/// The maximum number of open files in the open file table in the FS.
#define MAX_OPEN_FILES	(10)

#ifndef NDEBUG
# define message(m) fprintf(stdout, "%s", m)
# define message1(f, m) fprintf(stdout, f, m)
# define message2(f, m1, m2) fprintf(stdout, f, m1, m2)
#else
# define message(m)
# define message1(f, m)
# define message2(f, m1, m2)
#endif

#define error(m) fprintf(stderr, "%s", m)
#define error1(f, m) fprintf(stderr, f, m)
#define error2(f, m1, m2) fprintf(stderr, f, m1, m2)

#define TODO(text) todo(__FILE__, __LINE__, text)

void ntos(char *buf, int n);
int ston(char *buf);
void dumphex(const char *buf, int len);
void banner(const char *text);
void todo(const char *file, int line, const char *text);

#endif
