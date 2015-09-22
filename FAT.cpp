/**     @file FAT.cpp
 *
 *      File Allocation Table implementation
 *
 *      @author Copyright Aniesh Chawla, Sept 2015
 **/

#include "FAT.h"
 #include <iostream>

FAT::FAT(Disk& disk) : disk(disk)
{
  // Allocate the FAT entries

  entry = new int[disk.num_blocks()];

  // Populate the FAT from Disk

  retrieve();
}

FAT::~FAT()
{
  if (entry)
  {
    flush();
    delete entry;
  }
}

int FAT::alloc()
{
  // Search the FAT for a free entry

  for (int block_num = 0; block_num < disk.num_blocks(); ++block_num)
  {
    if (entry[block_num] == FAT_FREE)
    {
      entry[block_num] = FAT_END;

      return block_num;
    }
  }

  return FAT_END;
}

int FAT::alloc_next(int block_num)
{
  assert(block_num >= 0 && block_num < disk.num_blocks());

  // Check if the entry is FAT_END, if not then return the entry's value

  if (entry[block_num] != FAT_END)
    return entry[block_num];

  // Allocate a new block and link it to the entry for block_num.

  return entry[block_num] = alloc();
}

int FAT::find_next(int block_num)
{
  assert(block_num >= 0 && block_num < disk.num_blocks());

  // Check if the entry is FAT_END, if not then return the entry's value

  if (entry[block_num] != FAT_END)
    return entry[block_num];

  return FAT_END;
} 

void FAT::clear(int block_num)
{
  assert(block_num >= 0 && block_num < disk.num_blocks());

  // Clear the chain of blocks starting at block_num

  while (block_num != FAT_END)
  {
    int next_block = entry[block_num];
    entry[block_num] = FAT_FREE;
    block_num = next_block;
  }
}

int FAT::num_blocks() const
{
  return disk.num_blocks();
}

int FAT::num_free_blocks() const
{
  int count = 0;

  for (int block_num = 0; block_num < disk.num_blocks(); ++block_num)
    if (entry[block_num] == FAT_FREE)
      ++count;

  return count;
}

int FAT::fat_num_blocks() const
{
  // How many entries fit into a disk block?

  int entries_per_block = disk.block_size() / sizeof(int);

  // How many blocks are reserved for the FAT?

  return (disk.num_blocks() + entries_per_block - 1) / entries_per_block;
}

Disk& FAT::get_disk()
{
  return disk;
}

void FAT::init()
{
  int block_num;

  // Reserve the first fat_num_blocks() for the FAT

  for (block_num = 0; block_num < fat_num_blocks(); ++block_num)
    entry[block_num] = FAT_END;

  // Set the remaining entries to FAT_FREE

  for (; block_num < disk.num_blocks(); ++block_num)
    entry[block_num] = FAT_FREE;
}

int FAT::retrieve()
{
  message("FAT retrieve\n");

  // How many entries fit into a disk block?

  int entries_per_block = disk.block_size() / sizeof(int);

  // Get the FAT from Disk

  for (int block_num = 0; block_num < fat_num_blocks(); ++block_num)
  {
    char buf[MAX_BLOCK_SIZE];

    if (disk.read(block_num, buf) == DISK_ERROR)
      return DISK_ERROR;
    
    int index = entries_per_block * block_num;
    int entries;
    
    if (index + entries_per_block > disk.num_blocks())
      entries = disk.num_blocks() - index;
    else
      entries = entries_per_block;

    for (int pos = 0; pos < entries; ++pos)
      entry[index + pos] = ston(buf + sizeof(int)*pos);
  }

  // If the first entry is not FAT_END, the FAT on disk is invalid and we assume the disk was formatted.
  if (entry[0] != FAT_END)
    init();

  return DISK_OK;
}

int FAT::flush()
{
  message("FAT flush\n");

  // How many entries fit into a disk block?

  int entries_per_block = disk.block_size() / sizeof(int);

  // Save the FAT to disk

  for (int block_num = 0; block_num < fat_num_blocks(); ++block_num)
  {
    int index = entries_per_block * block_num;
    int entries;
    
    if (index + entries_per_block > disk.num_blocks())
      entries = disk.num_blocks() - index;
    else
      entries = entries_per_block;

    char buf[MAX_BLOCK_SIZE];

    for (int pos = 0; pos < entries; ++pos)
      ntos(buf + sizeof(int)*pos, entry[index + pos]);

    if (disk.write(block_num, buf) == DISK_ERROR)
      return DISK_ERROR;
  }

  return DISK_OK;
}

void FAT::list()
{
  message1("FAT (first %d blocks contain the FAT):\n", fat_num_blocks());

  for (int block_num = fat_num_blocks(); block_num < disk.num_blocks(); ++block_num)
  {
    if (entry[block_num] != FAT_FREE)
    {
      bool show = true;

      for (int block_check = 0; block_check < block_num; ++block_check)
      {
	if (find_next(block_check) == block_num)
	{
	  show = false;
	  break;
	}
      }
      if (show)
      {
        for (int block_chain = block_num; block_chain != FAT_END; block_chain = find_next(block_chain))
	  message1("%d -> ", block_chain);

        message("end\n");
      }
    }
  }
}
/*FAT &
FAT::operator=(const FAT &fat_file){
  if(this!=&fat_file){
    std::cout<<"copying fat_file"<<std::endl;
    disk = fat_file.disk;
    *entry = *(fat_file.entry);
  }
  return *this;
}
*/
