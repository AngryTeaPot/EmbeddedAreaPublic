/**
  **********************************************************************************************
  * @file    date.c
  * @brief   This file contains all the functions for
  *          the date.exe
  **********************************************************************************************
  * @attention
  *
  * Copyright (c) Embedded Area.
  * All rights reserved.
  *
  **********************************************************************************************
  */

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#include <windows.h>
#include <Lmcons.h>

DWORD unl = 1025;

typedef struct{
	unsigned char dirName[256];
	unsigned char fileName[256];
 	char userName[1024];
}date;
 
int main(int argc, char **argv) {
   
   printf("Date File generated");
   
   time_t t = time(0);
   struct tm *p = localtime(&t);
   date variables = {0};
   
   char *headerStart = "#ifndef __DATE_H\n#define __DATE_H";
   char *headerFinish = "#endif  /* __DATE_H */ ";
   
   // Project  file path
   GetCurrentDirectoryA(256,variables.dirName);
   // Project .exe file path
   GetModuleFileName(NULL,variables.fileName,256);
   // Windows User Name
   GetUserName(variables.userName, &unl);


	FILE *dosya;
	
	
	dosya = fopen("date.h","w");
		
			if(dosya == NULL)
	{
		printf("File creation is failed ");
		exit(EXIT_FAILURE);
	}
	

	fprintf(dosya,"\n%s\n\n\n",headerStart );

	fprintf(dosya,"\n#define USERNAME \"%s\" \n",&variables.userName );
	
	fprintf(dosya,"\n#define COMPILE_DAY    %d"  ,p->tm_mday );
	fprintf(dosya,"\n#define COMPILE_MONTH  %d"	,p->tm_mon+1 );
	fprintf(dosya,"\n#define COMPILE_YEAR   %d\n",p->tm_year+1900 );

	fprintf(dosya,"\n#define HOUR    %d", p->tm_hour );
	fprintf(dosya,"\n#define MINUTE  %d", p->tm_min );
	fprintf(dosya,"\n#define SECOND  %d", p->tm_sec );

	fprintf(dosya,"\n\n\n%s\n",headerFinish );

	
	fclose(dosya);
 
 
return 0;
}
