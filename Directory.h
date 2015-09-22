/**     @file Directory.h
 *
 *      Directory structure
 *
 *      @author Copyright Aniesh Chawla, Sept 2015
 **/

#ifndef DIRECTORY_H
#define DIRECTORY_H

#include <map>
#include "includes.h"
#include "FAT.h"
#include "FileName.h"
#include "FCB.h"

struct ltname
{
  bool operator()(FileName name1, FileName name2) const
  {
    return name1.compare(name2) < 0;
  }
};

typedef std::map<FileName, FCB, ltname> DirectoryMap;

/**	Directory helper object.
 *
 *	Stores directory information obtained from disk. The root directory is
 *	assumed to be located right after the FAT on disk. A search starts at
 *	the root directory and recursively moves down a path. The directory
 *	object is used to obtain and set file FCBs, list directory content,
 *	create directory entries, erase directory entries, and more.
 **/
class Directory
{
  private:
    /// A reference to the FAT of a Disk
    FAT& fat;
    /// The directory map from file names to FCBs
    DirectoryMap dir;
    /// The start block of the directory on Disk
    int start_block;
    /// Indicates that the directory has changed and need to be flushed
    bool updated;

  public:
    /// Create a new directory helper object from the root directory of the FAT.
    Directory(FAT& fat);
    ~Directory();
    /// Checks if the directory is empty.
    bool empty();
    /// Change the current directory by searching down the path.
    int chdir(const char *path);
    /// Create a new directory in the current directory.
    int mkdir(const FileName& name);
    /// Check if a file or directory exists in the current directory.
    bool exists(const FileName& name) const;
    /// Erase a file or directory from the current directory.
    int erase(const FileName& name);
    /// Get the FCB of a file in the current directory.
    const FCB *get_fcb(const FileName& name) const;
    /// Set the FCB for a file in the current directory.
    void set_fcb(const FileName& name, const FCB& fcb);
    /// List the current directory.
    void list();

  private:
    int retrieve(int block_num);
    int flush();
};

#endif
