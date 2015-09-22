/**     @file NDisk.cpp
 *
 *      Network Disk structure derived from Disk. Uses RPC to communicate to
 *      the server.
 *
 *      @author Copyright Aniesh Chawla, Sept 2006
 **/

#include "NDisk.h"
#include <iostream>


NDisk::NDisk()
{
  proxy.connect("localhost", NDISK_DEFAULT_PORT);
}

NDisk::NDisk(const char *host, int port)
{
  proxy.connect(host, port);
} 

NDisk::~NDisk()
{
  proxy.disconnect();
}

int NDisk::num_blocks() const
{
  char buf[NDISK_BLOCK_SIZE];

  proxy.call("NUMB", 0, buf);

  // Get num blocks from packet data
  return ston(buf);
}

int NDisk::block_size() const
{
  return NDISK_BLOCK_SIZE;
}

int NDisk::format(int num_blocks)
{
  return proxy.call("FRMT", num_blocks, NULL);
}

int NDisk::read(int block_num, char *buf) const
{
  return proxy.call("READ", block_num, buf);
}

int NDisk::write(int block_num, const char *buf)
{
  //std::cout<<"writing "<<buf<<" at "<<block_num<<std::endl;
  return proxy.call("WRTE", block_num, (char*)buf);
}
