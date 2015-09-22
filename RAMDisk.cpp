/**     @file RAMDisk.cpp
 *
 *      RAM Disk structure implementation
 *
 *      @author Copyright Aniesh Chawla, Sept 2006
 **/

#include "RAMDisk.h"
#include <iostream>
RAMDisk::RAMDisk() : size(0), data(NULL)
{ } 

RAMDisk::~RAMDisk()
{
  if (data)
    delete data;
}

int RAMDisk::num_blocks() const
{
  return size;
}

int RAMDisk::block_size() const
{
  return RAM_BLOCK_SIZE;
}

int RAMDisk::format(int num_blocks)
{
  if (num_blocks < 1 || num_blocks > MAX_NUM_BLOCKS)
  {
    error1("Cannot format RAM disk with %d blocks\n", num_blocks);

    return DISK_ERROR;
  }

  if (data)
    delete data;

  size = num_blocks;
  data = new RAMDiskBlock[size];

  if (!data)
  {
    error("RAM disk allocation failed\n");
    size = 0;

    return DISK_ERROR;
  }

  memset(data, 0, size * RAM_BLOCK_SIZE);

  return DISK_OK;
}

int RAMDisk::read(int block_num, char *buf) const
{
  if (block_num < 0 || block_num >= size)
  {
    error1("Cannot read RAM disk block %d\n", block_num);

    return DISK_ERROR;
  }

  memcpy(buf, data[block_num], RAM_BLOCK_SIZE);

  return DISK_OK;
}

int RAMDisk::write(int block_num, const char *buf)
{
  if (block_num < 0 || block_num >= size)
  {
    error1("Cannot write RAM disk block %d\n", block_num);

    return DISK_ERROR;
  }
  memcpy(data[block_num], buf, RAM_BLOCK_SIZE);
  
  return DISK_OK;
}
/*RAMDisk&
RAMDisk::operator=(RAMDisk& rmdisk){
  if(this!=&rmdisk){
    std::cout<<"copying rmdisk"<<std::endl;
    size = rmdisk.size;
    data = (rmdisk.data);
  }
  return *this;
}
*/
