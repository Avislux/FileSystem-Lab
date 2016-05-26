#include "FileDirectory.h"
#include <iostream>
using namespace std;
FileDirectory::FileDirectory()
{
		//purpose: to initialize all entries in the fileDirectory and FAT16 to be 0; i.e.safe values.
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 32; j++)
			fileDirectory[i][j] = 0;	// a max of 4 file directories. Each has 32 bytes.
	}
		
	for( int i=0; i<256; i++) FAT16[i]=0;	//256 clusters only
	for(int i=0; i<1024; i++) data[i] =0;		//256 clusters * 4 bytes/cluster = 1;024 bytes.

}

bool FileDirectory::create(char   filename[], int numberBytes){
	bool output = false; int unused = 0; int k = 0;
	for (k = 0; k < 4; k++) {//(1)	to check if there is an unused entry in the File Directory;  
		if (FileDirectory::fileDirectory[k][0] == 0) { output = true; }//(i.e.the first character of the file name in the File Directory is zero).Return false if not true.
	}
	for (int i = 0; i < 256; i++) {//(2)	to check if there are enough unused clusters to store the file with the numberBytes.
		if (FileDirectory::FAT16[i] == 0) unused += 1;
	}
	if (unused >= numberBytes) {
		output = true;
	}
	else { output = false; }
	return output;//Return false if not true.
}
bool FileDirectory::deleteFile(char   filename[]){
	//(0)	to check if the file to be deleted; filename[]; is in the Directory.If not; return false.
	bool output = false; bool fileFound = false; int k = 0; int location = 0;
	for ( k = 0; k < 4; k++) { //each file in directory
		for (int j = 0; j < 8; j++) { //first eight bytes
			if (FileDirectory::fileDirectory[k][j] == filename[j]) {
				fileFound = true;
			}
			else {
				fileFound = false; //file not found
				break;
			}
		}
		if (fileFound == true) {
			location = k;
			break;
		}
	}
	if (fileFound == true) {
		//(1)	to change the first character of the file name in the File Directory to be zero;
		FileDirectory::fileDirectory[k][0] = 0;
		//(2)	to change all entries of the clusters of this file in the FAT to 1; i.e.; unused.
		for (int i = 0; i < 256; i++) { //should get clusters of files instead
			FileDirectory::FAT16[i] = 1;
		}
	}
	else return false;
}
bool FileDirectory::read(char   filename[], char filedata[]){
//purpose: to read  of data from data[] array from the file with the specified file name.
	int i, j;
	unsigned short int firstSectorAddress, sectorAddress, sectors[256];
	
	//(0)	to check if the file to be read ,filename[], is in the Directory.If not; return false.
	for ( i = 0; i < 4; i++)
	{
		for ( j = 0; j < 8; j++)
			if (filename[j] != fileDirectory[i][j])break;
		if (j == 8) break;
	}
	if(i==4	)return false;
//(1)	use the file name to get the file information from the File Directory; including date; time; number of bytes and the first cluster address;
	firstSectorAddress = fileDirectory[i][27] * 256 + fileDirectory[i][26];
//(2)	use the first cluster address to get all the cluster addresses of this file from the FAT - 16;
	sectorAddress = firstSectorAddress;
	j = 0;
	while (FAT16[sectorAddress] != 0xffff)
	{
		sectors[j++] = sectorAddress;
		sectorAddress = FAT16[sectorAddress];
	}
//(3)	use all the cluster addresses to read the data from the disk / flash memory.
	for (i = 0; sectors[i] !=0; i++)
	{
		for (int k = 0; k < 4; k++)
			filedata[i*4 + k] = data[sectors[i]*4 + k];
	}
}
bool FileDirectory::write(char filename[], int numberBytes, char fileData[], int year, int month, int day, int hour, int minute, int second)
{
	unsigned short int firstClusterAddress,clusterAddress,nextClusterAddress, date, time;
	int i, written;
	written = 0;
//purpose: to write numberBytes bytes of data from fileData[] array into the file with the specified file name
// (0)	to look for the first unused entry(0 or 1) in the FAT - 16; and use it as the First Cluster Address.
	for ( i = 2; i < 256; i++)
	{	
		if (FAT16[i] == 0) //address unused
		{
			break;
		}
	}
	clusterAddress = i;
	firstClusterAddress = i;
// (1)	to look for the next unused entry(0 or 1) in the FAT - 16; and use it as the Next Cluster Address; and write its value into the FAT - 16.
// (2)	Repeat Step 2 until all clusters are found and the FAT - 16 is updated.
	for (i = i+1; i < 256; i++)
	{
		if (FAT16[i] == 0 && written <=numberBytes)
		{
			written += 1;
			nextClusterAddress = i;
			FAT16[clusterAddress] = nextClusterAddress; //store next cluster into fat at current cluster
			clusterAddress = nextClusterAddress;
			
		}
	}
	
//(3)	to write / update the file name, extension, date, time, file length and first cluster address into the first unused entry in the File Directory;
	//assume file i is the file to be written into the Directory.
	for (int j = 0; j < 8; j++) { fileDirectory[i][j] = filename[j]; }
	unsigned short int date;
	date = (year - 1980)<<9 + month<<5 + day;
	fileDirectory[i][25] = date>>8;	//MS byte
	fileDirectory[i][24] = date; //LS byte
	time = (hour) << 9 + minute << 5 + second/2;
	fileDirectory[i][23] = time >> 8;	//MS byte
	fileDirectory[i][22] = time; //LS byte
}
bool FileDirectory::printClusters(char filename[]) {
	bool output = false; bool fileFound = false; int k = 0; int location = 0;
	for (k = 0; k < 4; k++) { //each file in directory
		for (int j = 0; j < 8; j++) { //first eight bytes
			if (FileDirectory::fileDirectory[k][j] == filename[j]) { //(1)	to check if the file to be printed; filename[]; is in the Directory.If not; return false.
				fileFound = true;
			}
			else {
				fileFound = false; //file not found
				break;
			}
		}
		
	}
	if (fileFound == false) {
		return false;
	}
	for (int j = 0; j < 32; j++) { //print all data
		cout << FileDirectory::fileDirectory[location][j] << " ";
		if (j % 8 == 0) { cout << endl; }
	}
}
/*
	purpose: to print all the clusters of a file.
			
			 (2)	use the file name to get the file information from the File Directory; including the first cluster address;
	(3)	use the first cluster address to get all cluster addresses from the FAT - 16;
	7.	void printDirectory() prints all the  files of the directory.
		(1)	use the file name to get the file information from the File Directory; including the first cluster address;
	(2)	use the first cluster address to get all cluster addresses from the FAT - 16;
	8.	void printData(char filename[]) prints the data of a file.
		(1)	use the file name to get the file information from the File Directory; including the first cluster address;
	(2)	use the first cluster address to get all cluster addresses from the FAT - 16;
	(3)	use cluster address to read the data of the file.Use the file length to print these data in hexadecimal format.
*/
void FileDirectory::printDirectory() {
	int k;
	for (k = 0; k < 4; k++) {
		for (int j = 0; j < 8; j++) { //print file name
			cout << FileDirectory::fileDirectory[k][j] ;
			
	}
}
/* prints all the  files of the directory.
(1)	use the file name to get the file information from the File Directory; including the first cluster address;
(2)	use the first cluster address to get all cluster addresses from the FAT - 16;
*/
void FileDirectory::printData(char filename[]) {}
/*prints the data of a file.
(1)	use the file name to get the file information from the File Directory; including the first cluster address;
(2)	use the first cluster address to get all cluster addresses from the FAT - 16;
(3)	use cluster address to read the data of the file.Use the file length to print these data in hexadecimal format.
*/