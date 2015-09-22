/**     @file fsserver.cpp
 *
 *      Simple Network File System server
 *
 *      @author Copyright Robert van Engelen, April 2006
 **/

#include "includes.h"
#include "VDisk.h"
#include "RPC.h"

int main()
{
  VDisk vdisk;
  RPC rpc;

  return rpc.serve(NDISK_DEFAULT_PORT, vdisk);
}
