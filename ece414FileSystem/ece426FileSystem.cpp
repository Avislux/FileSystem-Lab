// ece414FileSystem.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "FileDirectory.h"
#include <iostream>
int main()
{
	FileDirectory cpp;
	//	1.	create and write a file; file1;  of 40 bytes;
	cpp.create("file1", 40);
	char file1[8], file1data[20];
	for (int i = 0; i < 8; i++) {
		file1[i] = 0x66;
	}
	for (int i = 0; i < 20; i++) {
		file1[i] = 0x22;
	}
	cpp.write(file1, 20, file1data, 2016, 12, 12, 12, 12, 12);
	
	cpp.printClusters(file1);
	std::cout << "\n";
	cpp.printData(file1);
	std::cout << "\n";
	cpp.read(file1, file1data);
	//cpp.printDirectory();
	//	2.	create and write a file; file2;  of 200 bytes;
	cpp.create("file2", 200);
	//	3.	create and write a file; file3;  of 300 bytes;
	cpp.create("file3", 300);
	//	4.	create and write a file; file4;  of 500 bytes.
	cpp.create("file4", 500);
	//		5.	delete file2;
	cpp.deleteFile("file2");
	//	6.	create and write a file; file4;  of 500 bytes.
	cpp.create("file4", 500);
	char f;
	std::cin >> f;
	return 0;
}


