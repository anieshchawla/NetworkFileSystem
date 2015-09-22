/**     @file FileName.cpp
 *
 *      File Name structure implementation
 *
 *      @author Copyright Aniesh Chawla, Sept 2006
 **/

#include "FileName.h"

FileName::FileName()
{
  str[0] = '\0';
}

FileName::FileName(const char *name)
{
  strncpy(str, name, MAX_FILE_NAME);
  str[MAX_FILE_NAME - 1] = '\0';
}

FileName::FileName(const FileName& name)
{
  strncpy(str, name.str, MAX_FILE_NAME);
}

FileName::~FileName()
{ }

int FileName::compare(const FileName& name) const
{
  return strncmp(str, name.str, MAX_FILE_NAME);
}

char *FileName::cstr()
{
  return str;
}

const char *FileName::cstr() const
{
  return str;
}

const char *FileName::cstr(const char *s)
{
  strncpy(str, s, MAX_FILE_NAME);
  str[MAX_FILE_NAME - 1] = '\0';

  return str;
}
