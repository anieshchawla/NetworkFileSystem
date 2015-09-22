/**     @file NDisk.h
 *
 *      Network Disk structure derived from Disk. Uses RPC to communicate to
 *      the server.
 *
 *      @author Copyright Robert van Engelen, April 2006
 **/

#ifndef NDISK_H
#define NDISK_H

#include "includes.h"
#include "Disk.h"
#include "RPC.h"

/**	The NDisk class is derived from Disk. It is an interface (proxy) for
 *	a remote VDisk on a server.
 */
class NDisk : public Disk
{
  private:
    /// The NDisk RPC proxy.
    RPC proxy;

  public:
    NDisk();
    NDisk(const char *host, int port);
    virtual ~NDisk();
    virtual int num_blocks() const;
    virtual int block_size() const;
    virtual int format(int num_blocks);
    virtual int read(int block_num, char *buf) const;
    virtual int write(int block_num, const char *buf);
};

#endif
