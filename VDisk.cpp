/**     @file VDisk.cpp
 *
 *      Virtual Disk structure implementation
 *
 *      @author Copyright Aniesh Chawla, Sept 2006
 **/

#include "VDisk.h"
 #include <iostream>

VDisk::VDisk() : size(0), fd(NULL)
{ } 

VDisk::~VDisk()
{
  if (fd)
    fclose(fd);
}

int VDisk::num_blocks() const
{
  return size;
}

int VDisk::block_size() const
{
  return VDISK_BLOCK_SIZE;
}

int VDisk::format(int num_blocks)
{
  if (num_blocks < 1 || num_blocks > MAX_NUM_BLOCKS)
  {
    error1("Cannot format VDisk with %d blocks\n", num_blocks);

    return DISK_ERROR;
  }

  //TODO("Implement VDisk::format()");

  size = num_blocks;
  if (fd == NULL) fd = fopen("vdisk.dat","r+");

  if(fd==NULL) {
    error("failed to open VDisk ");
    return DISK_ERROR;
  }
  else {
/*    fseek(fd,0,SEEK_END);
    size_t size_of_file = ftell(fd);
    std::cout<<"size of file is "<<std::endl;
    if(size_of_file < size){*/
        fseek(fd,0,SEEK_SET);
        char buffer[]={'0'};
        fwrite(buffer,VDISK_BLOCK_SIZE,size,fd);
      //}
    //memset(fd,'0',size*VDISK_BLOCK_SIZE);
  }

  // If fd == NULL, then open the vdisk.dat file for read/write and set fd.

  // If the vdisk.dat file does not exist, create it

  // Report error if the vdisk.dat file could not be opened

  // If the file was created, populate it with size*VDISK_BLOCK_SIZE zeros

  // If the old vdisk.dat file was opened, check if the size is too small and
  // enlarge it if necessary.

  // Hint: use libc fopen(), fseek(), and fwrite().

  // Return DISK_OK or DISK_ERROR.

  return DISK_OK;
}

int VDisk::read(int block_num, char *buf) const
{
  if (block_num < 0 || block_num >= size)
  {
    error1("Cannot read VDisk block %d\n", block_num);

    return DISK_ERROR;
  }

  if (!fd)
  {
    error("vdisk.dat not formatted\n");
    
    return DISK_ERROR;
  }

//  TODO("Implement VDisk::read()");
  // Seek to the block.
  fseek(fd,block_num*VDISK_BLOCK_SIZE,SEEK_SET);
  // Read the block.
  fread(buf,VDISK_BLOCK_SIZE,1,fd);
  //memcpy(buf,fd,VDISK_BLOCK_SIZE);
  return DISK_OK;
  // Hint: use libc fseek(), and fread().

  // Return DISK_OK or DISK_ERROR.

}

int VDisk::write(int block_num, const char *buf)
{
  if (block_num < 0 || block_num >= size)
  {
    error1("Cannot write VDisk block %d\n", block_num);

    return DISK_ERROR;
  }

  if (!fd)
  {
    error("vdisk.dat not formatted\n");

    return DISK_ERROR;
  } 
  // Seek to the block.
    fseek(fd,block_num*VDISK_BLOCK_SIZE,SEEK_SET);
    // Write the block.
    fwrite(buf,VDISK_BLOCK_SIZE,1,fd);
    return DISK_OK;

  // Hint: use libc fseek(), and fwrite().

  // Return DISK_OK or DISK_ERROR.
}
/*VDisk &
VDisk::operator=(VDisk&vdisk){
  size = vdisk.size;
  *fd = *(vdisk.fd);
  return *this;
}*/
