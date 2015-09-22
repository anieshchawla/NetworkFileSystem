/**     @file RAMDisk.h
 *
 *      RAM Disk structure derived from Disk
 *
 *      @author Copyright Aniesh Chawla, Sept 2006
 **/

#ifndef RAMDISK_H
#define RAMDISK_H

#include "includes.h"
#include "Disk.h"

/// A RAMDisk has fixed blocks of 512 bytes
#define RAM_BLOCK_SIZE	(512)

#if RAM_BLOCK_SIZE > MAX_BLOCK_SIZE
# error "RAM block size too large"
#endif

#if RAM_BLOCK_SIZE % MIN_BLOCK_SIZE
# error "RAM block size must be multiple of MIN_BLOCK_SIZE"
#endif

typedef char RAMDiskBlock[RAM_BLOCK_SIZE];

/**	The RAMDisk class is derived from Disk. It implements a block device in
 *	memory.
 */
class RAMDisk : public Disk
{
  private:
    /// The number of blocks.
    int size;
    /// The RAMDisk data content.
    RAMDiskBlock *data;

  public:
    RAMDisk();
    virtual ~RAMDisk();
    virtual int num_blocks() const;
    virtual int block_size() const;
    virtual int format(int num_blocks);
    virtual int read(int block_num, char *buf) const;
    virtual int write(int block_num, const char *buf);
/*    RAMDisk& operator=(RAMDisk&);*/
};

#endif
