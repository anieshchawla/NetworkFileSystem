CPP=g++
CFLAGS=-g -Wall
#CFLAGS=-DNDEBUG

all:		fsclient fsserver
fsclient:	fsclient.cpp FS.o Volume.o RAMDisk.o VDisk.o NDisk.o RPC.o Directory.o FAT.o OpenFile.o FileName.o Util.o
		$(CPP) $(CFLAGS) -o $@ fsclient.cpp FS.o Volume.o RAMDisk.o VDisk.o NDisk.o RPC.o Directory.o FAT.o OpenFile.o FileName.o Util.o
fsserver:	fsserver.cpp VDisk.o RPC.o Util.o
		$(CPP) $(CFLAGS) -o $@ fsserver.cpp VDisk.o RPC.o Util.o
Directory.o:	Directory.cpp Directory.h Disk.h FAT.h FCB.h FS.h FileName.h OpenFile.h RAMDisk.h VDisk.h NDisk.h RPC.h Volume.h includes.h
		$(CPP) $(CFLAGS) -c $<
FAT.o:		FAT.cpp Directory.h Disk.h FAT.h FCB.h FS.h FileName.h OpenFile.h RAMDisk.h VDisk.h NDisk.h RPC.h Volume.h includes.h
		$(CPP) $(CFLAGS) -c $<
FS.o:		FS.cpp Directory.h Disk.h FAT.h FCB.h FS.h FileName.h OpenFile.h RAMDisk.h VDisk.h NDisk.h RPC.h Volume.h includes.h
		$(CPP) $(CFLAGS) -c $<
FileName.o:	FileName.cpp Directory.h Disk.h FAT.h FCB.h FS.h FileName.h OpenFile.h RAMDisk.h VDisk.h NDisk.h RPC.h Volume.h includes.h
		$(CPP) $(CFLAGS) -c $<
OpenFile.o:	OpenFile.cpp Directory.h Disk.h FAT.h FCB.h FS.h FileName.h OpenFile.h RAMDisk.h VDisk.h NDisk.h RPC.h Volume.h includes.h
		$(CPP) $(CFLAGS) -c $<
RAMDisk.o:	RAMDisk.cpp Directory.h Disk.h FAT.h FCB.h FS.h FileName.h OpenFile.h RAMDisk.h VDisk.h NDisk.h RPC.h Volume.h includes.h
		$(CPP) $(CFLAGS) -c $<
VDisk.o:	VDisk.cpp Directory.h Disk.h FAT.h FCB.h FS.h FileName.h OpenFile.h RAMDisk.h VDisk.h NDisk.h RPC.h Volume.h includes.h
		$(CPP) $(CFLAGS) -c $<
NDisk.o:	NDisk.cpp Directory.h Disk.h FAT.h FCB.h FS.h FileName.h OpenFile.h RAMDisk.h VDisk.h NDisk.h RPC.h Volume.h includes.h
		$(CPP) $(CFLAGS) -c $<
RPC.o:		RPC.cpp Directory.h Disk.h FAT.h FCB.h FS.h FileName.h OpenFile.h RAMDisk.h VDisk.h NDisk.h RPC.h Volume.h includes.h
		$(CPP) $(CFLAGS) -c $<
Volume.o:	Volume.cpp Directory.h Disk.h FAT.h FCB.h FS.h FileName.h OpenFile.h RAMDisk.h VDisk.h NDisk.h RPC.h Volume.h includes.h
		$(CPP) $(CFLAGS) -c $<
.cpp.o:
		$(CPP) $(CFLAGS) -c $<
.PHONY:		clean
clean:          
		rm *.o
