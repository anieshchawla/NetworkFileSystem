/**     @file RPC.h
 *
 *      Simple RPC proxy and server object.
 *
 *	A packet is transmitted between the client proxy and server object.
 *	This packet has a header and message body, where the body contains a
 *	disk block to be read or written to disk at the server side:
 *
 *	 _____ _____ ______________________
 *	| cmd | num |         block        |
 *	|  4  |  4  |   NDISK_BLOCK_SIZE   |
 *      |_____|_____|______________________|
 *
 *	A command is a 4-byte string issued by the client to the server:
 *	"NUMB"	query number of disk blocks (server stores #blocks in block
 *		data on return)
 *	"FRMT"	request a disk format with 'num' blocks
 *	"WRTE"	request a disk write of the block data at block 'num'
 *	"READ"	request a disk read of the block data at block 'num'
 *
 *      @author Copyright Robert van Engelen, April 2006
 **/

#ifndef RPC_H
#define RPC_H

#include "includes.h"
#include "Disk.h"

#define NDISK_DEFAULT_PORT (19821)

/// An NDisk has fixed blocks of 1024 bytes
#define NDISK_BLOCK_SIZE	(1024)

#if NDISK_BLOCK_SIZE > MAX_BLOCK_SIZE
# error "NDISK block size too large"
#endif

#if NDISK_BLOCK_SIZE % MIN_BLOCK_SIZE
# error "NDISK block size must be multiple of MIN_BLOCK_SIZE"
#endif

class RPC
{
  private:
    /// The socket.
    int sock;

  public:
    RPC();
    ~RPC();
    /// Connect to the server with endpoint host:port.
    int connect(const char *host, int port);
    /// Disconnect from server.
    void disconnect();
    /// Request service: command contains the operation, block_num is an input parameter, and buf is an in/output parameter (or NULL).
    int call(const char command[5], int block_num, char *buf) const;
    /// Start a server at port to expose a local disk over the network.
    int serve(int port, Disk& disk);

  private:
    int dispatch(Disk& disk);
};

#endif
