/**     @file RPC.cpp
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

#include "RPC.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>
#include <iostream>

RPC::RPC() : sock(-1)
{ }

RPC::~RPC()
{
  disconnect();
}

int RPC::connect(const char *host, int port)
{
  // Disconnect if necessary to reconnect
  disconnect();

  // New TCP/IP socket
  sock = (int)socket(AF_INET, SOCK_STREAM, 0);
  if (sock < 0)
    return DISK_ERROR;

  // Resolve host
  struct hostent *hostent = gethostbyname(host);
  if (hostent == NULL)
    return DISK_ERROR;

  // Set port
  struct sockaddr_in peer;
  memset(&peer, 0, sizeof(peer));
  peer.sin_family = AF_INET;
  memcpy(&peer.sin_addr, hostent->h_addr, hostent->h_length);
  peer.sin_port = htons((short)port);

  // Connect
  if (::connect(sock, (const struct sockaddr*)&peer, sizeof(peer)) < 0)
    return DISK_ERROR;

  message2("Connected to %s:%d\n", host, port);

  return DISK_OK;
}

void RPC::disconnect()
{
  if (sock >= 0)
  {
    shutdown(sock, 2); // Send EOF
    close(sock);
    sock = -1;
  }
}

int RPC::call(const char command[5], int block_num, char *buf) const
{
  if (sock < 0)
    return DISK_ERROR;

  char packet[NDISK_BLOCK_SIZE + 8];

  // Add the command to the packet header
  memcpy(packet, command, 4);

  // Add the block_num parameter to the packet header
  ntos(packet + 4, block_num);
  
  // Add the block to the packet
  if (buf)
    memcpy(packet + 8, buf, NDISK_BLOCK_SIZE);

  size_t len = NDISK_BLOCK_SIZE + 8;
  char *msg = packet;

  // Write the packet (with 8-byte header)
  do
  {
    ssize_t nwritten = send(sock, msg, len, 0);

    if (nwritten < 0)
      return DISK_ERROR;

    len -= nwritten;
    msg += nwritten;
  }
  while (len > 0);

  len = NDISK_BLOCK_SIZE + 8;
  msg = packet;

  // Read the packet (with 8-byte header)
  do
  {
    ssize_t nread = recv(sock, msg, len, 0);

    if (nread <= 0)
      return DISK_ERROR;

    len -= nread;
    msg += nread;
  }
  while (len > 0);

  if (buf){
      //buf = (char *) malloc(NDISK_BLOCK_SIZE);
      //buf = (char*) (packet+8); 
      if(strcmp(command, "WRTE")!=0){ 
        //std::cout<<"buf="<<buf<<", command is  "<<command<<std::endl;
        memcpy(buf, packet + 8, NDISK_BLOCK_SIZE);
      }
      //free(buf);
  }
  return DISK_OK;
}

int RPC::serve(int port, Disk& disk)
{
  int master = (int)socket(AF_INET, SOCK_STREAM, 0);

  int set = 1;
  setsockopt(master, SOL_SOCKET, SO_REUSEADDR, (char*)&set, sizeof(int));

  struct sockaddr_in peer;
  memset(&peer, 0, sizeof(peer));
  peer.sin_family = AF_INET;
  peer.sin_addr.s_addr = htonl(INADDR_ANY);
  peer.sin_port = htons((short)port);

  if (bind(master, (struct sockaddr*)&peer, sizeof(peer)))
    return DISK_ERROR;

  listen(master, 100);

  message("Server started\n");

  for (;;)
  {
    socklen_t n = sizeof(peer);
    
    sock = accept(master, (struct sockaddr*)&peer, &n);

    if (sock < 0)
      return DISK_ERROR;

    message("Connection established\n");

    while (dispatch(disk) == DISK_OK)
      ;

    shutdown(sock, 2); // Send EOF
    close(sock);

    message("Connection closed\n");
  }
}

int RPC::dispatch(Disk& disk)
{
  char buf[NDISK_BLOCK_SIZE];
  char packet[NDISK_BLOCK_SIZE + 8];

  size_t len = NDISK_BLOCK_SIZE + 8;
  char *msg = packet;

  if (disk.block_size() != NDISK_BLOCK_SIZE)
    return DISK_ERROR;

  // Read the packet (with 8-byte header)
  do
  {
    ssize_t nread = recv(sock, msg, len, 0);

    if (nread <= 0)
      return DISK_ERROR;

    len -= nread;
    msg += nread;
  }
  while (len > 0);

  // Get the command from the packet's first four bytes
  char command[5];
  memcpy(command, packet, 4);
  command[4] = '\0';

  // Get the block number parameter from the packet
  int block_num = ston(packet + 4);

  if (!strcmp(command, "NUMB"))
  {
    // Write num blocks into packet data
    ntos(packet + 8, disk.num_blocks());
  }
  else if (!strcmp(command, "FRMT"))
  {
    disk.format(block_num);
  }
  else if (!strcmp(command, "READ"))
  {
    disk.read(block_num, buf);
    memcpy(packet + 8, buf, NDISK_BLOCK_SIZE);
  }
  else if (!strcmp(command, "WRTE"))
  {
    memcpy(buf, packet + 8, NDISK_BLOCK_SIZE);
    disk.write(block_num, buf);
  }
  else
  {
    message1("Unknown RPC command %s\n", command);
  }

  message2("RPC command %s(%d)\n", command, block_num);

  len = NDISK_BLOCK_SIZE + 8;
  msg = packet;

  // Write the packet (with 8-byte header)
  do
  {
    ssize_t nwritten = send(sock, msg, len, 0);

    if (nwritten < 0)
      return DISK_ERROR;

    len -= nwritten;
    msg += nwritten;
  }
  while (len > 0);

  return DISK_OK;
}
