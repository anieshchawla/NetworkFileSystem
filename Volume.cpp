/**     @file Volume.cpp
 *
 *      Volume structure implementation
 *
 *      @author Copyright Aniesh Chawla, Sept 2006
 **/

#include "Volume.h"
 #include <iostream>

Volume::Volume(const char *name, Disk& disk) : name(strdup(name)), fat(disk)
{ }

Volume::~Volume()
{
  if (name)
    free((void*)name);
}

const char *Volume::get_name() const
{
  return name;
}

FAT& Volume::get_fat()
{
  return fat;
}
/*Volume&
Volume::operator=(const Volume&vol){
	if(this!= &vol){
		name = vol.name;
		std::cout<<"we are trying to copy volume using operator name "<<vol.name<<std::endl;
		//fat = vol.fat;
	}
	return *this;
}*/
