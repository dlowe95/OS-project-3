#ifndef _DPARSER_H
#define _DPARSER_H

typedef struct DIR_Entry {
	char DIR_Name[32];
	unsigned char DIR_Attr;
	unsigned int DIR_Cluster;
	unsigned int DIR_FileSize;
	unsigned int DIR_ByteAddress;
	unsigned char END_OF_ARRAY;
} DIR_Type;

DIR_Type* GetDirectoryContents(unsigned int clusNum);
void PrintDirVector(const DIR_Type* d);
void WriteDirEntry(DIR_Type d, unsigned int clusNum);
unsigned int ClusterFormat(const char* name);

unsigned int GetCurrentDirectoryClusterNum();
void SetCDClusterNum(unsigned int clusNum);
unsigned int GetPrevDirectoryClusterNum();
unsigned int PopPrevDirectoryClusterNum();
void PushPrevDirectoryClusterNum(unsigned int clusNum);

unsigned int next_cluster(unsigned int);

#endif