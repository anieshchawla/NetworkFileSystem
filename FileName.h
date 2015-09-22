/**     @file FileName.h
 *
 *      File Name structure
 *
 *      @author Copyright Aniesh Chawla, Sept 2006
 **/

#ifndef FILENAME_H
#define FILENAME_H

#include "includes.h"

/**	File name structure.
 *
 *	File names have certain restrictions, such as length. The maximum
 *	length of a file name is MAX_FILE_NAME-1.
 **/
class FileName
{
  private:
    /// The file name string
    char str[MAX_FILE_NAME];

  public:
    /// Allocate a file name with an empty string.
    FileName();
    /// Allocate a file name copied from another file name.
    FileName(const FileName& name);
    /// Allocate a file name and set if to the C string (truncate if necessary).
    FileName(const char *name);
    ~FileName();
    /// Lexicographically compare two file names (0=eq, -1=less, 1=greater).
    int compare(const FileName& name) const;
    /// Return the 0-terminated C string of the name.
    char *cstr();
    /// Return the 0-terminated constant C string of the name.
    const char *cstr() const;
    /// Set the file name (truncate if necessary).
    const char *cstr(const char *str);
};

#endif
