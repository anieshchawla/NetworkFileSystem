/**     @file filesystem.cpp
 *
 *      File System demo program
 *
 *      @author Copyright Aniesh Chawla, Sept 2006
 **/

#include "includes.h"
#include "RAMDisk.h"
#include "VDisk.h"
#include "Volume.h"
#include "FS.h"

void demo_dir(FS& fs, Volume& V);
void demo_file(FS& fs, Volume& V);

int main()
{
  FS fs;

  RAMDisk RAM;

  RAM.format(100);

  Volume A("A", RAM);

  demo_dir(fs, A);

  demo_file(fs, A);

  banner("Trying VDisk");

  VDisk VDSK;

  if (VDSK.format(50) != DISK_ERROR)
  {
    Volume B("B", VDSK);

    demo_dir(fs, B);

    demo_file(fs, B);
  }

  return 0;
}

void demo_dir(FS& fs, Volume& V)
{
  banner("Creating directory foo");

  fs.mkdir(V, "/", "foo");

  banner("Creating directory bar within foo");

  fs.mkdir(V, "/foo", "bar");

  banner("Listing /");

  fs.ls(V, "/");

  banner("Listing /foo");

  fs.ls(V, "/foo");

  banner("Removing directory bar from foo");

  fs.rmdir(V, "/foo", "bar");

  banner("Listing /foo");

  fs.ls(V, "/foo");
}

void demo_file(FS& fs, Volume& V)
{
  banner("Opening abc");

  int fd = fs.open(V, "/", "abc");

  if (fd != DISK_ERROR)
  {
    char buf[15];

    buf[14] = '\0';

    banner("Storing text in abc");

    fs.write(fd, "Hello World!", 14);
    
    fs.seek(fd, 0);

    fs.read(fd, buf, 14);

    printf("text: %s\n", buf);

    fs.seek(fd, 0);

    fs.write(fd, "Goodbye", 7);

    fs.seek(fd, 0);

    fs.read(fd, buf, 14);

    printf("text: %s\n", buf);

    banner("Closing abc");

    fs.close(fd);

    banner("Listing /");

    fs.ls(V, "/");

    banner("Removing abc");

    fs.rm(V, "/", "abc");

    banner("Listing /");

    fs.ls(V, "/");
  }
}
