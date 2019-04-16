#include "utils.h"
#include "FATImg.h"
#include <stdio.h>
#include <stdlib.h>

FILE* ImageFile = NULL;

struct ReservedRegion rr;

const int ENDOFCLUSTER = 268435448;

unsigned int FAT_StartLoc, FAT_EndLoc;

void OpenImgFile(const char* name) {

}

void CloseImgFile() {

}

FILE* GetImageFile() {
  return ImageFile;
}

short GetBytesPerSec(void) {
  return rr.BPB_BytsPerSec;
}

short GetSecPerClus(void) {
  return rr.BPB_SecPerClus;
}

unsigned int GetRootClusterNum(void) {
  return rr.BPB_RootClus;
}

void ParseBootSector(void) {
  unsigned short store_bytes[11];
  unsigned char SPC[1];
  int FATOffset, ThisFATSecNum;

  fseek(ImageFile, 0, SEEK_SET);
  fread(store_bytes, sizeof(char), 3, ImageFile);
  rr.BS_jmpBoot = little_to_big(store_bytes, 3) & 0xff;

  fseek(ImageFile, 3, SEEK_SET);
  fread(store_bytes, sizeof(char), 8, ImageFile);
  store_bytes[8] = '\0';
  strcpy(rr.BS_OEMName, store_bytes);

  fseek(ImageFile, 11, SEEK_SET);
  fread(store_bytes, sizeof(char), 2, ImageFile);		
  rr.BPB_BytsPerSec = store_bytes[0];

  fseek(ImageFile, 13, SEEK_SET);
  fread(SPC, sizeof(char), 1, ImageFile);
  rr.BPB_SecPerClus = little_to_big(SPC, 1);

  fseek(ImageFile, 14, SEEK_SET);
  fread(store_bytes, sizeof(char), 2, ImageFile);
  rr.BPB_RsvdSecCnt = little_to_big(store_bytes, 2);

  fseek(ImageFile, 16, SEEK_SET);
  fread(store_bytes, sizeof(char), 1, ImageFile);
  rr.BPB_NumFATs = little_to_big(store_bytes, 1);

  fseek(ImageFile, 17, SEEK_SET);
  fread(store_bytes, sizeof(char), 2, ImageFile);
  rr.BPB_RootEntCnt = little_to_big(store_bytes, 2);

  fseek(ImageFile, 19, SEEK_SET);
  fread(store_bytes, sizeof(char), 2, ImageFile);
  rr.BPB_TotSec16 = little_to_big(store_bytes, 2);

  fseek(ImageFile, 21, SEEK_SET);
  fread(store_bytes, sizeof(char), 1, ImageFile);
  rr.BPB_Media = little_to_big(store_bytes, 1);

  fseek(ImageFile, 22, SEEK_SET);
  fread(store_bytes, sizeof(char), 2, ImageFile);
  rr.BPB_FATSz16 = little_to_big(store_bytes, 2);

  fseek(ImageFile, 24, SEEK_SET);
  fread(store_bytes, sizeof(char), 2, ImageFile);
  rr.BPB_SecPerTrk = little_to_big(store_bytes, 2);

  fseek(ImageFile, 26, SEEK_SET);
  fread(store_bytes, sizeof(char), 2, ImageFile);
  rr.BPB_NumHeads = little_to_big(store_bytes, 2);

  fseek(ImageFile, 28, SEEK_SET);
  fread(store_bytes, sizeof(char), 4, ImageFile);
  rr.BPB_HiddSec = little_to_big(store_bytes, 4);

  fseek(ImageFile, 32, SEEK_SET);
  fread(store_bytes, sizeof(char), 4, ImageFile);
  rr.BPB_TotSec32 = little_to_big(store_bytes, 4);

  fseek(ImageFile, 36, SEEK_SET);
  fread(store_bytes, sizeof(char), 4, ImageFile);
  rr.BPB_FATSz32 = little_to_big(store_bytes, 4);

  fseek(ImageFile, 40, SEEK_SET);
  fread(store_bytes, sizeof(char), 2, ImageFile);
  rr.BPB_ExtFlags = little_to_big(store_bytes, 2);

  fseek(ImageFile, 42, SEEK_SET);
  fread(store_bytes, sizeof(char), 2, ImageFile);
  rr.BPB_FSVer = little_to_big(store_bytes, 2);
  
  fseek(ImageFile, 44, SEEK_SET);
  fread(store_bytes, sizeof(char), 4, ImageFile);
  rr.BPB_RootClus = little_to_big(store_bytes, 4);

  fseek(ImageFile, 48, SEEK_SET);
  fread(store_bytes, sizeof(char), 2, ImageFile);
  rr.BPB_FSInfo = little_to_big(store_bytes, 2);

  fseek(ImageFile, 50, SEEK_SET);
  fread(store_bytes, sizeof(char), 2, ImageFile);
  rr.BPB_BkBootSec = little_to_big(store_bytes, 2);
  
  fseek(ImageFile, 52, SEEK_SET);
  fread(store_bytes, sizeof(char), 12, ImageFile);
  rr.BPB_Reserved = little_to_big(store_bytes, 12);

  fseek(ImageFile, 64, SEEK_SET);
  fread(store_bytes, sizeof(char), 1, ImageFile);
  rr.BS_DrvNum = little_to_big(store_bytes, 1);

  fseek(ImageFile, 65, SEEK_SET);
  fread(store_bytes, sizeof(char), 1, ImageFile);
  rr.BS_Reserved1 = little_to_big(store_bytes, 1);

  fseek(ImageFile, 66, SEEK_SET);
  fread(store_bytes, sizeof(char), 1, ImageFile);
  rr.BS_BootSig = little_to_big(store_bytes, 1);

  fseek(ImageFile, 67, SEEK_SET);
  fread(store_bytes, sizeof(char), 4, ImageFile);
  rr.BS_VolID = little_to_big(store_bytes, 4);

  fseek(ImageFile, 71, SEEK_SET);
  fread(store_bytes, sizeof(char), 11, ImageFile);
  store_bytes[11] = '\0';
  strcpy(rr.BS_VolLab, store_bytes);

  fseek(ImageFile, 82, SEEK_SET);
  fread(store_bytes, sizeof(char), 8, ImageFile);
  store_bytes[8] = '\0';
  strcpy(rr.BS_FilSysType, store_bytes);

  FindFirstSectorOfCluster(rr.BPB_RootClus);
    
  FATOffset = rr.BPB_RootClus * 4;
  ThisFATSecNum =  rr.BPB_RsvdSecCnt + (FATOffset / rr.BPB_BytsPerSec);
    
  FAT_StartLoc = ThisFATSecNum * rr.BPB_BytsPerSec;
  FAT_EndLoc = rr.BPB_FATSz32 * rr.BPB_BytsPerSec + FAT_StartLoc;
}

void PrintBootSector() {
  printf("BS_jmpBoot\t= 0x%X\n", rr.BS_jmpBoot);
  printf("BS_OEMName\t= \"%s\"\n", rr.BS_OEMName);
  printf("BPB_BytsPerSec\t= %u\n", rr.BPB_BytsPerSec);
  printf("BPB_SecPerClus\t= %u\n", rr.BPB_SecPerClus);
  printf("BPB_RsvdSecCnt\t= %u\n", rr.BPB_RsvdSecCnt);
  printf("BPB_NumFATs\t= %u\n", rr.BPB_NumFATs);
  printf("BPB_RootEntCnt\t= %u\n", rr.BPB_RootEntCnt);
  printf("BPB_TotSec16\t= %u\n", rr.BPB_TotSec16);
  printf("BPB_Media\t= 0x%X\n", rr.BPB_Media);
  printf("BPB_FATSz16\t= %u\n", rr.BPB_FATSz16);
  printf("BPB_SecPerTrk\t= %u\n", rr.BPB_SecPerTrk);
  printf("BPB_NumHeads\t= %u\n", rr.BPB_NumHeads);
  printf("BPB_HiddSec\t= %u\n", rr.BPB_HiddSec);
  printf("BPB_TotSec32\t= %u\n", rr.BPB_TotSec32);
  printf("BPB_FATSz32\t= %u\n", rr.BPB_FATSz32);
  printf("BPB_ExtFlags\t= %u\n", rr.BPB_ExtFlags);
  printf("BPB_FSVer\t= %u\n", rr.BPB_FSVer);
  printf("BPB_RootClus\t= %u\n", rr.BPB_RootClus);
  printf("BPB_FSInfo\t= %u\n", rr.BPB_FSInfo);
  printf("BPB_BkBootSec\t= %u\n", rr.BPB_BkBootSec);
  printf("BPB_Reserved\t= %s\n", rr.BPB_Reserved);
  printf("BS_DrvNum\t= %u\n", rr.BS_DrvNum);
  printf("BS_Reserved1\t= %u\n", rr.BS_Reserved1);
  printf("BS_BootSig\t= %u\n", rr.BS_BootSig);
  printf("BS_VolID\t= %u\n", rr.BS_VolID);
  printf("BS_VolLab\t= \"%s\"\n", rr.BS_VolLab);
  printf("BS_FilSysType\t= \"%s\"\n", rr.BS_FilSysType);
}

int FindFirstSectorOfCluster(int n) {

}

unsigned int little_to_big(unsigned char *array, int bytes){

}

unsigned char* big_to_little(unsigned int value, unsigned int size){

}

void WriteIntToImage(unsigned int value, unsigned int offset) {

}

void WriteShortToImage(unsigned short value, unsigned int offset) {

}

void WriteCharToImage(unsigned char value, unsigned int offset) {

}

unsigned int FindNextFreeCluster(void) {

}

void WriteToFAT(unsigned int cluster_number, unsigned int next_cluster) {


}

unsigned int FindFirstFreeDirectoryEntry(unsigned int clusterNum) {

}

unsigned int FAT_Start(void) {
  return FAT_StartLoc;
}

unsigned int FAT_End(void) {
  return FAT_EndLoc;
}
