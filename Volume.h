/**     @file Volume.h
 *
 *      Volume structure
 *
 *      @author Copyright Aniesh Chawla, Sept 2006
 **/

#ifndef VOLUME_H
#define VOLUME_H

#include "includes.h"
#include "Disk.h"
#include "FAT.h"

/**	A Volume object associates a volume name with a formatted Disk.
 *
 *	The Volume object contains a FAT and a reference to the Disk object.
 **/
class Volume
{
  private:
    /// The volume name.
    const char *name;
    /// The FAT with the Disk reference.
    FAT fat;

  public:
    /// Create a new named Volume (and FAT) for a formatted Disk object.
    Volume(const char *name, Disk& disk);
    ~Volume();
    /// Return the name of this volume.
    const char *get_name() const;
    /// Return a reference to the FAT of the Disk for this volume.
    FAT& get_fat();
/*    Volume& operator=(const Volume&);*/
};

#endif
