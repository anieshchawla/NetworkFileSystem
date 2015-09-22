/**     @file Util.cpp
 *
 *      Utility functions
 *
 *      @author Copyright Aniesh Chawla, Sept 2006
 **/

#include "includes.h"

/// A utility function to convert a 4-bit int into little-endian word.
void ntos(char *buf, int n)
{
  buf[0] = ((unsigned int)n >> 24) & 0xFF;
  buf[1] = ((unsigned int)n >> 16) & 0xFF;
  buf[2] = ((unsigned int)n >> 8) & 0xFF;
  buf[3] = (unsigned int)n & 0xFF;
}

/// A utility function to convert a 4-bit little-endian word into int.
int ston(char *buf)
{
  return ((unsigned char)buf[0] << 24) | ((unsigned char)buf[1] << 16) | ((unsigned char)buf[2] << 8) | (unsigned char)buf[3];
}

/// A utility function to dump memory content in hex.
void dumphex(const char *buf, int len)
{
  for (int i = 0; i < len; i++)
    printf("%2.2hhX ", buf[i]);
  printf("\n");
}

void banner(const char *text)
{
  int i;

  printf("\n/");

  for (i = 0; i < 78; i++)
    putchar('*');

  printf("\\\n *%76s*\n * %-75s*\n *%76s*\n\\", "", text, "");

  for (i = 0; i < 78; i++)
    putchar('*');

  printf("/\n\n");
}

void todo(const char *file, int line, const char *text)
{
  printf("\n** TODO: %s in file %s at line %d\n\n", text, file, line);
}
