// ece414FileSystem.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "FileDirectory.h"

int main()
{
	FileDirectory cpp;
	//	1.	create and write a file; file1;  of 40 bytes;
	cpp.create("file1", 40);

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
	return 0;
}


