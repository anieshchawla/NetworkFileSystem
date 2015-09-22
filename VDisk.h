/**     @file VDisk.h
 *
 *      Virtual Disk structure derived from Disk
 *
 *      @author Copyright Aniesh Chawla, Sept 2006
 **/

#ifndef VDISK_H
#define VDISK_H

#include "includes.h"
#include "Disk.h"

/// A VDisk has fixed blocks of 1024 bytes
#define VDISK_BLOCK_SIZE	(1024)

#if VDISK_BLOCK_SIZE > MAX_BLOCK_SIZE
# error "VDISK block size too large"
#endif

#if VDISK_BLOCK_SIZE % MIN_BLOCK_SIZE
# error "VDISK block size must be multiple of MIN_BLOCK_SIZE"
#endif

/**	The VDisk class is derived from Disk. It implements a virtual block
 *	device in an existing regular UNIX/Linux file.
 */
class VDisk : public Disk
{
  private:
    /// The number of blocks.
    int size;
    /// The VDisk data file (you may use a std::iostream instead).
    FILE *fd;

  public:
    VDisk();
    virtual ~VDisk();
    virtual int num_blocks() const;
    virtual int block_size() const;
    virtual int format(int num_blocks);
    virtual int read(int block_num, char *buf) const;
    virtual int write(int block_num, const char *buf);
/*    VDisk &operator=(VDisk&vdisk);*/
};

#endif
