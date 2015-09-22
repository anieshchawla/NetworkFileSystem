/**     @file Disk.h
 *
 *      Abstract Disk structure
 *
 *      @author Copyright Aniesh Chawla, Sept 2015
 **/

#ifndef DISK_H
#define DISK_H

#include "includes.h"

/// All successful I/O operations return DISK_OK.
#define DISK_OK		(0)

/// All unsuccessful I/O operations return DISK_ERROR.
#define DISK_ERROR	(-1)

/**	Abstract Disk base type.
 *
 *	Disks should be derived from the Disk class. An example derived disk
 *	class is RAMDisk. To implement a new disk class, define its five basic
 *	API methods. A disk must be formatted with zeros (at least four zeros
 *	at the start), to allow the Directory object to detect a fresh medium
 *	and automatically install a FAT.
 *
 **/
class Disk
{
  public:
    ///	Returns the number of blocks on the device.
    virtual int num_blocks() const = 0;
    ///	Returns the size (in bytes) of each block on the device.
    virtual int block_size() const = 0;
    ///	Format the device and allocate the number of blocks given (or less).
    virtual int format(int num_blocks) = 0;
    ///	Read a block from the device and copy it to buf, which must be large enough to hold block_size() bytes.
    virtual int read(int block_num, char *buf) const = 0;
    ///	Write a block to the device by copying it from buf holding block_size() bytes.
    virtual int write(int block_num, const char *buf) = 0;

};

#endif
