/**     @file FAT.h
 *
 *      File Allocation Table structure
 *
 *      @author Copyright Aniesh Chawla, Sept 2006
 **/

#ifndef FAT_H
#define FAT_H

#include "includes.h"
#include "Disk.h"

/// A free entry in the FAT is marked FAT_FREE.
#define FAT_FREE	(0)
/// The end of a chain of blocks in the FAT is marked FAT_END.
#define FAT_END		(-1)

/**	The FAT class allocates and manages blocks of a Disk object.
 *
 *	A FAT object refers to a Disk object. The FAT object is stored in the
 *	Disk object's first N blocks, where
 *	N = FAT::fat_num_blocks()
 *	  = round-up(Disk::num_blocks() / entries_per_block),
 *	entries_per_block = Disk::block_size() / sizeof(int).
 **/
class FAT
{
  private:
    /// Refers to the Disk object that is managed
    Disk& disk;
    /// Table of FAT entries
    int *entry;

  public:
    FAT(Disk& disk);
    ~FAT();
    /// Return a free block number, or FAT_END
    int alloc();
    /// Return a free block number, or FAT_END, and link the block_num to it.
    int alloc_next(int block_num);
    /// Return the next block from the current block_num.
    int find_next(int block_num);
    /// Clear the chain of blocks starting with block_num.
    void clear(int block_num);
    /// Return the number of blocks in the FAT (= number of blocks on Disk).
    int num_blocks() const;
    /// Return the number of available free blocks.
    int num_free_blocks() const;
    /// Return the number of blocks occupied by the FAT.
    int fat_num_blocks() const;
    /// Return the Disk reference.
    Disk& get_disk();
    /// List the FAT content.
    void list();
    //copy operator for FAT
/*    FAT &operator=(const FAT&);*/

  private:
    /// Reset and initialize the FAT.
    void init();
    /// Retrieve the FAT from the first Disk blocks.
    int retrieve();
    /// Save the FAT to Disk.
    int flush();
};

#endif
