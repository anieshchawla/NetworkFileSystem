/**     @file FCB.h
 *
 *      File Control Block structure
 *
 *      @author Copyright Aniesh Chawla, Sept 2006
 **/

#ifndef FCB_H
#define FCB_H

#include "includes.h"

/// The type attribute of the FCB is a file.
#define FILE_TYPE	(0)
/// The type attribute of the FCB is a directory.
#define DIR_TYPE	(1)

/**	An FCB structure contains the file attributes.
 *
 *	The FCB in our implementation must be 16 bytes in size, so that
 *	MIN_BLOCK_SIZE = MAX_FILE_NAME + 16
 */
struct FCB
{
  /// Size of the file in bytes (might be left unused for directories).
  int size;
  /// FILE_TYPE or DIR_TYPE.
  int type;
  /// The file/directory creation time.
  time_t created;
  /// The starting block of the file or directory (or FAT_END).
  int start_block;
};

#endif
