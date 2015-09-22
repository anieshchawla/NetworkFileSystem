/**     @file FS.cpp
 *
 *      File System implementation
 *
 *      @author Copyright Aniesh Chawla, Sept 2015
 **/

#include "FS.h"
#include "Directory.h"
 #include <iostream>

FS::FS()
{ }

FS::~FS()
{ }

int FS::open(Volume& volume, const char *path, const char *name)
{
  int fd = 0;

  //TODO("Implement FS::open()");
  // Check if file was already opened (has entry in open file table):
  // Check all open file entries for count > 0.
  // If the volume, path, and file name match, then ++count and return fd.

  // Otherwise, set fd to a free entry in the open file table with count == 0
  // and set the open_file.name to the open() name argument.

  // If no entries are available then report error and return DISK_ERROR.
  while((fd < MAX_OPEN_FILES)){
    if(strcmp(open_file_table[fd].name.cstr() , name)==0){
      break;
    }
    fd++;
  }
  if (fd==MAX_OPEN_FILES){
    fd = 0;
    while(open_file_table[fd].count!=0 && fd<MAX_OPEN_FILES){
      fd++;
    }
    if(fd == MAX_OPEN_FILES) fd=DISK_ERROR;
    else {
      assert(open_file_table[fd].count == 0);
      open_file_table[fd].name = name;
    }
  }
  OpenFile& open_file = open_file_table[fd];//we used & symbol because if we change open_file we want corresponding
  //change to happend in open_file_table entry also

  // Create a Directory object for the volume's FAT and use Directory::chdir()
  // to move to the path location of the file.
  // The dir should now point to the directory of the file.
  // If the dir does not exist, then report and return DISK_ERROR.
  Directory dir(volume.get_fat());
  dir.chdir(path);
  //if(!dir.exists(open_file.name)) return DISK_ERROR;

  // Get the FCB of the file via the dir object.

  const FCB *fcb_file = dir.get_fcb(open_file.name);

  // If there is no FCB, then this is a new file so we create a new FCB with
  // size = 0, type = FILE_TYPE, created = time(NULL), and we allocate a new
  // start_block from the FAT of the volume.
  if(fcb_file == NULL) {
    FCB fcb;
    fcb.size = 0;
    fcb.type = FILE_TYPE;
    fcb.created = time(NULL);
    fcb.start_block = volume.get_fat().alloc();
    open_file.start_block = fcb.start_block;
    dir.set_fcb(open_file.name, fcb);
  }

  // Otherwise, if there is an FCB, then set the open_file.size and
  // open_file.start_block from the FCB attributes.
  else{
    open_file.size = fcb_file->size;
    open_file.start_block = fcb_file->start_block;
  }

  // Set the other open_file attributes:
  open_file.pos = 0;
  open_file.volume= &volume;
  open_file.current_block = open_file.start_block;
  open_file.current_block_pos = 0;
  open_file.updated = false;
  // Increment the open_file.count.
  open_file.count++;

  //fd = DISK_ERROR;

  return fd;
}

void FS::close(int fd)
{
  OpenFile& open_file = open_file_table[fd];

  assert(open_file.count > 0);

  //TODO("Implement FS::close()");

  // If the file was updated, update its FCB in the directory:
  if (open_file.updated)
  {
  // Create a Directory object for the volume's FAT and use Directory::chdir()
  // to move to the path location of the file.
  // The dir should now point to the directory of the file.
  // If the dir does not exist, then report and return DISK_ERROR.
    Directory dir(open_file.volume->get_fat());
    dir.chdir(open_file.path);
    if(!dir.exists(open_file.name)) exit DISK_ERROR;

  // Get the FCB object of the file, update it with the open_file attributes
  // such as size, and set the FCB of the file
    const FCB *fcb_file = dir.get_fcb(open_file.name);
    if(fcb_file==NULL){

    }
    open_file.count--;
/*    if(open_file.count==0){
      open_file.size = 0;
      open_file.start_block = 0;
      FCB fcb;
      fcb.size = 0;
      fcb.start_block = 0;
      dir.set_fcb(open_file.name, fcb);
    }*/
  // What to do if the FCB was not found? This could happen when the file is
  // deleted before closed. So rm() must check if the file is not open!
  }

  // Decrement the open_file.count
}

int FS::read(int fd, char *buf, int len)
{
  OpenFile& open_file = open_file_table[fd];

  assert(open_file.count > 0);

  //TODO("Implement FS::read()");
  int block_num = open_file.current_block;
  int byte_to_read = len;
  Volume &vol = *open_file.volume;
  FAT &file_table = vol.get_fat();
  Disk &read_fm_disk = file_table.get_disk();
  int block_size = read_fm_disk.block_size();
  char blk[block_size];
  // We need to copy data of at most len bytes from disk to the buf argument.
  //std::cout<<"we are reading the file with length "<<len<<std::endl;
  while(len>0){
    open_file.current_block = block_num;
    size_t read_len = std::min(len,block_size);
    read_fm_disk.read(block_num,blk);
    int read_length = open_file.current_block_pos + read_len;
    //std::cout<<"read length= "<<read_length<<" current_block_pos= "<<open_file.current_block_pos<<std::endl;
    //std::cout<<"block_size= "<<read_fm_disk.block_size()<<" block_num "<<block_num<<std::endl;
    open_file.pos +=read_len;
    if(read_length > block_size ){
      memcpy(buf,blk+open_file.current_block_pos,block_size-open_file.current_block_pos);
      block_num = file_table.find_next(block_num);
      open_file.current_block = block_num;
      read_fm_disk.read(block_num,blk);
      //std::cout<<"value copied till now is "<< buf<<std::endl;
      //std::cout<<"value to be copied "<< blk<<std::endl;
      read_len -= (block_size - open_file.current_block_pos);
      buf+= (block_size - open_file.current_block_pos);
      len-= (block_size - open_file.current_block_pos);
      open_file.current_block_pos = 0;
    }
    //if(block_num==24) std::cout<<"value should be "<<blk+open_file.current_block_pos<<std::endl;
    memcpy(buf,blk+open_file.current_block_pos,read_len);
    buf+=read_len;
    if(read_len<block_size) {
      open_file.current_block_pos += read_len;
      if(open_file.current_block_pos > block_size) open_file.current_block_pos-=block_size;
    }
    else{

      block_num = file_table.find_next(block_num);
    }
    len-=read_len;
  }
  // To do so, use a loop to copy the data in chunks.
  // Within the loop, update the open_file.pos position in the file,
  // the open_file.current_block, and open_file.current_block_pos offset into
  // the current block being read.

  // You need to get the FAT of the volume of the open file and use find_next()
  // to find the blocks.

  // Return the number of bytes copied, or DISK_ERROR when an error occurred.
  return byte_to_read;
}

int FS::write(int fd, char *buf, int len)
{
  OpenFile& open_file = open_file_table[fd];
  assert(open_file.count > 0);

  open_file.updated = true;

  //TODO("Implement FS::write()");


  // We need to copy data from buf of length len to disk.

  // To do so, use a loop to copy the data in chunks.
  // Within the loop, update the open_file.size, open_file.pos,
  // the open_file.current_block, and open_file.current_block_pos offset into
  // the current block being written.
  Volume &vol = *open_file.volume;
  FAT &file_table = vol.get_fat();
  Disk &fm_disk = file_table.get_disk();
  Directory dir(file_table);
  FCB fcb_of_file;
  int block_size = fm_disk.block_size();
  char blk[block_size];
  const FCB *old_fcb_file = dir.get_fcb(open_file.name);
  int block_num = open_file.current_block;
  // We need to copy data of at most len bytes from disk to the buf argument.
  while(len>0){
    size_t read_len = std::min(len,block_size);
    char buf_disk[block_size];
    //std::cout<<"the block number is "<<block_num<<std::endl;
    if(open_file.current_block_pos == block_size){
      //std::cout<<"we are inside this block num "<<block_num<<std::endl;
      block_num = file_table.alloc_next(block_num);
      open_file.current_block = block_num;
      open_file.current_block_pos = 0;
      fm_disk.read(block_num,buf_disk);
    }
    fm_disk.read(block_num,buf_disk);
    char write_buf[block_size];
    memcpy(write_buf,buf_disk,open_file.current_block_pos);
    int read_length = open_file.current_block_pos+read_len;
    open_file.pos +=read_len;
    open_file.size+=read_len;

    if(read_length > block_size){
      memcpy(write_buf+open_file.current_block_pos,buf,block_size- open_file.current_block_pos);
      fm_disk.write(block_num,write_buf);  
      block_num = file_table.alloc_next(block_num);
      open_file.current_block = block_num;
      buf+=block_size-open_file.current_block_pos;
      read_len -= block_size - open_file.current_block_pos;
      len-= block_size - open_file.current_block_pos;
      open_file.current_block_pos = 0;
      fm_disk.read(block_num,buf_disk);
      read_length = read_len;
    }
    memcpy(write_buf+open_file.current_block_pos,buf,read_len);
    memcpy(write_buf+read_length, buf_disk + read_length,block_size- read_length);
    if(read_len<block_size){
      open_file.current_block_pos+=read_len;
      if(open_file.current_block_pos>block_size){
        open_file.current_block_pos-=block_size;
      }
    }
    fm_disk.write(block_num,write_buf);
    
    buf+=read_len;
    len-=read_len;
  }
  fcb_of_file.type = old_fcb_file->type;
  fcb_of_file.size = open_file.size;
  fcb_of_file.created = old_fcb_file->created;
  fcb_of_file.start_block = old_fcb_file->start_block;
  dir.set_fcb(open_file.name,fcb_of_file);
  // You need to get the FAT of the volume of the open file and use
  // alloc_next() to find or get new blocks.

  // Note that writing data inside blocks may require reading blocks first.

  // Return DISK_OK or DISK_ERROR.

  return DISK_OK;
}

int FS::seek(int fd, int pos)
{
  OpenFile& open_file = open_file_table[fd];
  assert(open_file.count > 0);
  FAT &fat_file = (open_file.volume->get_fat());
  int block_size = fat_file.get_disk().block_size();
  //TODO("Implement FS::seek()");
  //std::cout<<"the file position= "<<open_file.pos<<" requested pos= "<<pos<<std::endl;
  int block_shifts = (pos - open_file.pos);
  int offset = pos%block_size;
  //std::cout<<"block shifts "<<block_shifts<<" offset "<<offset<<std::endl;
  if(open_file.size < pos) return DISK_ERROR;
  else{
    if(block_shifts < 0) {
      block_shifts = (int)(pos/block_size) - (int)(open_file.pos/block_size);
      if(open_file.pos == ((int)(open_file.pos/block_size))*block_size) block_shifts++;
      //std::cout<<"block shifts negative is "<<block_shifts<<std::endl;
      block_shifts = open_file.current_block - open_file.start_block + block_shifts;
      //std::cout<<"the block shifts from start are "<<block_shifts<<std::endl;
      open_file.current_block = open_file.start_block;
    }
    open_file.pos = pos;

    while(block_shifts > 0 ){
        open_file.current_block = fat_file.find_next(open_file.current_block);
        block_shifts--;
    }
    open_file.current_block_pos = offset;
    return DISK_OK;
    
  }

  // Change the open_file.pos, open_file.current_block, and
  // open_file.current_block_pos within the allowable range of the file
  // where the range is 0 to size-1.


  // Return DISK_OK or DISK_ERROR when an error occurred

  return DISK_ERROR;
}

int FS::ls(Volume& volume, const char *path)
{
  Directory dir(volume.get_fat());

  dir.chdir(path);

  printf("Volume %s:%s (%d blocks, %d free)\n", volume.get_name(), path, volume.get_fat().num_blocks(), volume.get_fat().num_free_blocks());

  dir.list();

  return DISK_OK;
}

int FS::rm(Volume& volume, const char *path, const char *name)
{
  //TODO("Implement FS::rm()");
  int fd = -1;
  Directory dir(volume.get_fat());
  dir.chdir(path);
  FileName file_name(name);
  const FCB* file_fcb = dir.get_fcb(file_name);

  // Check if this is a regular file.

  if(file_fcb->type !=FILE_TYPE) return DISK_ERROR;

  // Check if the file is not open.
  while((fd < MAX_OPEN_FILES)){
    if(strcmp(open_file_table[fd].name.cstr() , name)==0){
      break;
    }
    fd++;
  }
  // Create a Directory object for the volume's FAT and use Directory::chdir()
  // to move to the path location of the file.
  // The dir should now point to the directory of the file.
  // If the dir does not exist, then report and return DISK_ERROR.

  // Use Directory::erase() to remove the file.
  if(fd != MAX_OPEN_FILES){
    assert(open_file_table[fd].count ==0);
    
    dir.erase(file_name);
    return DISK_OK;
  }


  // Return DISK_OK or DISK_ERROR.

  return DISK_ERROR;
}

int FS::mkdir(Volume& volume, const char *path, const char *name)
{
  Directory dir(volume.get_fat());

  dir.chdir(path);

  FileName dir_name(name);

  return dir.mkdir(dir_name);
}

int FS::rmdir(Volume& volume, const char *path, const char *name)
{
  Directory dir(volume.get_fat());

  dir.chdir(path);

  FileName dir_name(name);
  const FCB* file_fcb = dir.get_fcb(dir_name);
  //TODO("Complete FS::rmdir()");
  // Check if this is a directory.
  if(file_fcb->type !=DIR_TYPE) return DISK_ERROR;
  if(!dir.exists(dir_name)) return DISK_ERROR;
  // Check if the directory is empty.
  //if(!dir.empty()) return DISK_ERROR;
  dir.erase(dir_name);
  return DISK_OK;
}
