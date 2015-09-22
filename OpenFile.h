/**     @file OpenFile.h
 *
 *      Open File structure
 *
 *      @author Copyright Aniesh Chawla, Sept 2006
 **/

#ifndef OPENFILE_H
#define OPENFILE_H

#include "includes.h"
#include "Volume.h"
#include "FileName.h"

/**	The OpenFile object is an entry in the open file table kept by the FS.
 *
 */
class OpenFile
{
  public:
    /// Number of open files, count = 0 if entry is free.
    int count;
    /// The Volume object for this file.
    Volume* volume;
    /// The path to this file.
    char *path;
    /// The name of this file.
    FileName name;
    /// The size of this file.
    int size;
    /// The current read/write position of this file.
    int pos;
    /// The starting block on Disk for this file.
    int start_block;
    /// The current block for the read/write position.
    int current_block;
    /// The block offset for the read/write position.
    int current_block_pos;
    /// True if a write operation was performed on this file, or attributes have changed.
    bool updated;

  public:
    OpenFile();
    ~OpenFile();
};

#endif
