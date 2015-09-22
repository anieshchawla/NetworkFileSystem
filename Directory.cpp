/**     @file Directory.cpp
 *
 *      Directory structure implementation
 *
 *      @author Copyright Aniesh Chawla, Sept 2015
 **/

#include "Directory.h"
 #include <iostream>

Directory::Directory(FAT& fat) : fat(fat)
{
  if (retrieve(fat.fat_num_blocks()) == DISK_ERROR)
    error("Could not retrieve directory\n");
}

Directory::~Directory()
{
  if (flush() == DISK_ERROR)
    error("Could not update directory\n");
}

bool Directory::empty()
{
  return dir.empty();
}

int Directory::chdir(const char *path)
{
  message1("Directory chdir %s\n", path);

  FileName name;

  while (path)
  {
    if (path[0] == '/')
      ++path;

    if (path[0] == '\0')
      break;

    const char *s = strchr(name.cstr(path), '/');
    if (s)
      s = '\0';

    const FCB *fcb = get_fcb(name);

    if (!fcb)
    {
      error1("Directory %s does not exist\n", name.cstr());

      return DISK_ERROR;
    }

    path = strchr(path, '/');

    if (fcb->type != DIR_TYPE)
    {
      error1("Name %s is not a directory\n", name.cstr());

      return DISK_ERROR;
    }

    if (flush() == DISK_ERROR)
      return DISK_ERROR;

    if (retrieve(fcb->start_block) == DISK_ERROR)
      return DISK_ERROR;
  }

  return DISK_OK;
}

int Directory::mkdir(const FileName& name)
{
  message1("Directory mkdir %s\n", name.cstr());

  if (exists(name))
  {
    error1("Name %s already exists\n", name.cstr());
    return DISK_ERROR;
  }

  FCB fcb;

  fcb.size = 0;
  fcb.type = DIR_TYPE;
  fcb.created = time(NULL);
  fcb.start_block = fat.alloc();
  set_fcb(name, fcb);

  return DISK_OK;
}

bool Directory::exists(const FileName& name) const
{
  return dir.find(name) != dir.end();
}

int Directory::erase(const FileName& name)
{
  const FCB *fcb = get_fcb(name);

  if (!fcb)
  {
    error1("File %s does not exists\n", name.cstr());
    return DISK_ERROR;
  }
  std::cout<<"clearing the block number "<<fcb->start_block<<std::endl;

  fat.clear(fcb->start_block);

  dir.erase(name);

  updated = true;

  return DISK_OK;
}

const FCB *Directory::get_fcb(const FileName& name) const
{
  DirectoryMap::const_iterator iter = dir.find(name);
  if (iter != dir.end())
    return &(*iter).second;

  return NULL;
}

void Directory::set_fcb(const FileName& name, const FCB& fcb)
{
  dir[name] = fcb;
  updated = true;
}

void Directory::list()
{
  printf("%-16s%8s  %4s  %s\n", "Name", "Size", "Type", "Created");
  for (DirectoryMap::const_iterator walker = dir.begin(); walker != dir.end(); ++walker)
    printf("%-16s%8d  %4s  %s", (*walker).first.cstr(), (*walker).second.size, (*walker).second.type == DIR_TYPE ? "dir" : "file", ctime(&(*walker).second.created));
}

int Directory::retrieve(int block_num)
{
  dir.clear();

  updated = false;

  Disk& disk = fat.get_disk();

  int entries_per_block = disk.block_size() / MIN_BLOCK_SIZE;

  start_block = block_num;
  
  // If we don't have a start block, this directory is empty

  if (start_block == FAT_END)
    return DISK_OK;
  
  // If the disk was formatted, the start block of the root dir is FAT_FREE

  if (fat.find_next(start_block) == FAT_FREE)
  {
    // The first block of the root dir must be right after the FAT

    start_block = fat.alloc();

    assert(start_block == fat.fat_num_blocks());

    return DISK_OK;
  }

  message1("Directory retrieve from block %d\n", block_num);

  while (block_num != FAT_END)
  {
    char buf[MAX_BLOCK_SIZE];

    if (disk.read(block_num, buf) == DISK_ERROR)
      {
        return DISK_ERROR;
      }

    for (int pos = 0; pos < entries_per_block; ++pos)
    {
      
      if (buf[pos*MIN_BLOCK_SIZE] == '\0')
        break;

      FileName name(buf + pos*MIN_BLOCK_SIZE);

      FCB fcb;

      fcb.size = ston(buf + pos*MIN_BLOCK_SIZE + MAX_FILE_NAME);
      fcb.type = ston(buf + pos*MIN_BLOCK_SIZE + MAX_FILE_NAME + 4);
      fcb.created = (time_t)ston(buf + pos*MIN_BLOCK_SIZE + MAX_FILE_NAME + 8);
      fcb.start_block = ston(buf + pos*MIN_BLOCK_SIZE + MAX_FILE_NAME + 12);

      dir[name] = fcb;

      // message2("Reading directory entry %s with start block %d\n", name.cstr(), fcb.start_block);
    }

    block_num = fat.find_next(block_num);
  }

  return DISK_OK;
}

int Directory::flush()
{
  // If nothing changed, return

  if (!updated)
    return DISK_OK;

  Disk& disk = fat.get_disk();

  int entries_per_block = disk.block_size() / MIN_BLOCK_SIZE;

  if (start_block == FAT_END && (start_block = fat.alloc()) == FAT_END)
  {
    error("Disk full\n");

    return DISK_ERROR;
  }

  message1("Directory flush to block %d\n", start_block);

  int block_num = start_block;

  DirectoryMap::const_iterator walker = dir.begin();

  do
  {
    char buf[MAX_BLOCK_SIZE];

    memset(buf, 0, MAX_BLOCK_SIZE);

    for (int pos = 0; pos < entries_per_block && walker != dir.end(); ++pos, ++walker)
    {
      const FileName& name = (*walker).first;

      strcpy(buf + pos*MIN_BLOCK_SIZE, name.cstr());

      const FCB& fcb = (*walker).second;

      ntos(buf + pos*MIN_BLOCK_SIZE + MAX_FILE_NAME, fcb.size);
      ntos(buf + pos*MIN_BLOCK_SIZE + MAX_FILE_NAME + 4, fcb.type);
      ntos(buf + pos*MIN_BLOCK_SIZE + MAX_FILE_NAME + 8, (int)fcb.created);
      ntos(buf + pos*MIN_BLOCK_SIZE + MAX_FILE_NAME + 12, fcb.start_block);

      // message2("Writing directory entry %s with start block %d\n", name.cstr(), fcb.start_block);
    }

    if (disk.write(block_num, buf) == DISK_ERROR)
      return DISK_ERROR;

    if (walker == dir.end())
      return DISK_OK;

    block_num = fat.alloc_next(block_num);
  }
  while (block_num != FAT_END);

  return DISK_ERROR;
}
