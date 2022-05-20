#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>


void main(int argc, char** argv){
	int h, m, s, total;
	if(argc != 2){
		printf("usage: time.exe hour:minute:second");
		exit(0);
	}
	sscanf(argv[1], "%d:%d:%d", &h, &m, &s);
	
	total = h*60*60+m*60+s;
	
	while(total > 0){
		printf("%02d:%02d:%02d\n", 
			total/3600,
			total/60%60,
			total%60);
		Sleep(1000);
		total--;
	}
	
	printf("\aTime's up!");
	MessageBox(NULL, TEXT("Time's up!"), TEXT("SimTimer"), MB_OK);
}