/**     @file fsclient.cpp
 *
 *      File System demo client program
 *
 *      @author Copyright Aniesh Chawla, Sept 2006
 **/

#include "includes.h"
#include "NDisk.h"
#include "Volume.h"
#include "FS.h"

void demo_dir(FS& fs, Volume& V);
void demo_file(FS& fs, Volume& V);

int main()
{
  FS fs;

  banner("Trying NDisk");

  NDisk ndisk;

  if (ndisk.format(50) != DISK_ERROR)
  {
    Volume C("C", ndisk);

    demo_dir(fs, C);

    demo_file(fs, C);
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

  banner("Creating subdirs in /foo");

  char buf[16];

  for (int i = 0; i < 10; ++i)
  {
    sprintf(buf, "dir%d", i);

    fs.mkdir(V, "/foo", buf);
  }

  banner("Listing /foo");

  fs.ls(V, "/foo");
}

void demo_file(FS& fs, Volume& V)
{
  banner("Opening abc");

  int fd = fs.open(V, "/", "abc");

  if (fd != DISK_ERROR)
  {
    char buf[256];

    banner("Storing text in abc");

    fs.write(fd, "Hello   World!", 14);
    //fs.write(fd, "Hel", 3);

    banner("we have completed hello");
    
    fs.seek(fd, 0);

    fs.read(fd, buf, 14);

    buf[14] = '\0';

    printf("text: %s\n", buf);

    fs.seek(fd, 0);

    fs.write(fd, "Goodbye", 7);

    fs.seek(fd, 0);

    fs.read(fd, buf, 14);

    buf[14] = '\0';

    printf("text: %s\n", buf);

    banner("Closing abc");

    fs.close(fd);

    banner("Listing /");

    fs.ls(V, "/");

    banner("Re-open abc to test write/read/seek");

    fd = fs.open(V, "/", "abc");

    if (fd != DISK_ERROR)
    {
      for (int i = 0; i < 1000; ++i)
      {
        if (fs.write(fd, "abcdefghijklmnopqrstuvwxyz", 26) == DISK_ERROR)
	{
	  error("Error in write()\n");
          break;
        }
      }
      

      if (fs.seek(fd, 0) == DISK_ERROR)
	error("Error in seek()\n");
      


      for (int i = 0; i < 1000; ++i)
      {
	memset(buf, 0, 26);

        if (fs.read(fd, buf, 26) != 26)
	{
	  error("Error in read()\n");
          break;
        }

	if (strncmp(buf, "abcdefghijklmnopqrstuvwxyz", 26))
        {
	  error("String error in before success read()\n");
          break;
        }
      }

      banner("successfully completed this seeking and comparing");

      for (int i = 0; i < 1000; ++i)
      {
	memset(buf, 0, 26);

	if (fs.seek(fd, 26 * (999 - i)) == DISK_ERROR)
	{
	  error("Error in after success seek()\n");
          break;
        }

        if (fs.read(fd, buf, 26) != 26)
	{
	  error("Error in after success read()\n");
          break;
        }
	if (strncmp(buf, "abcdefghijklmnopqrstuvwxyz", 26))
        {
	  error("String error in after success read()\n");
          break;
        }
      }

      fs.close(fd);

      banner("Listing /");

      fs.ls(V, "/");
    }

    banner("Removing abc");

    if (fs.rm(V, "/", "abc") == DISK_ERROR)
      error("Error in rm()\n");

    banner("Listing /");

    fs.ls(V, "/");
  }
}
