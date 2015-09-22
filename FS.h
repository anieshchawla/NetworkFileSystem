/**     @file FS.h
 *
 *      File System
 *
 *      @author Copyright Aniesh Chawla, Sept 2006
 **/

#ifndef FS_H
#define FS_H

#include "includes.h"
#include "Volume.h"
#include "OpenFile.h"

/**	FS File System.
 *
 *	An FS object provides a high-level interface to a collection of
 *	Volumes. The FS API supports open, close, read, write, seek, and close
 *	operations on files, file removal, and directory creation, listing, and
 *	removal. An FS object maintains open files in the open file table,
 *	which should normally be placed in a monitor for concurrency control. 
 *
 *	Example:
 *	@code
 *	FS fs;                      // create a file system object
 *	RAMDisk ram1;               // create a RAM disk object
 *	ram1.format(100);           // format the RAM disk with 100 blocks
 *	Volume A("A", ram1);        // create a volume "A" for the RAM disk
 *	fs.mkdir(A, "/", "foo");    // Create a subdir "foo" on volume A
 *	fs.mkdir(A, "/foo", "bar"); // Create a sub-sub dir "bar" under "foo"
 *	fs.ls(A, "/");              // list the root directory of the RAM disk
 *	fs.ls(A, "/foo");           // list the "foo" directory
 *	fs.rmdir(A. "/foo", "bar"); // remove the "bar" directory	
 *	@endcode
 **/
class FS
{
  private:
    /// The internal open file table of the FS object.
    OpenFile open_file_table[MAX_OPEN_FILES];

  public:
    FS();
    ~FS();
    /// Open takes a Volume, a path, and a file name, and returns a file descriptor to the open file. Returns DISK_ERROR on error.
    int open(Volume& volume, const char *path, const char *name);
    /// Close takes a file descriptor and closes the file.
    void close(int fd);
    /// Read takes a file descriptor, a buffer, and buffer length arguments, fills the buffer, and returns the number of bytes written in the buffer. Zero is returned to indicate EOF. DISK_ERROR is returned for errors.
    int read(int fd, char *buf, int len);
    /// Write takes a file descriptor, a buffer with content, the length of the buffer, and writes the data to file. DISK_OK is returned on success and DISK_ERROR if an error occurred.
    int write(int fd, char *buf, int len);
    /// Seek moves the current pointer to a new position within the file. Returns DISK_OK on success, and DISK_ERROR when an error occurred.
    int seek(int fd, int pos);
    /// Ls lists the directory contents of a Volume given a path.
    int ls(Volume& volume, const char *path);
    /// Rm removes a file from a Volume on a path.
    int rm(Volume& volume, const char *path, const char *name);
    /// Mkdir creates a new directory in a Volume on a path.
    int mkdir(Volume& volume, const char *path, const char *name);
    /// Rmdir removes a directory of a Volume on a path.
    int rmdir(Volume& volume, const char *path, const char *name);
};

#endif
