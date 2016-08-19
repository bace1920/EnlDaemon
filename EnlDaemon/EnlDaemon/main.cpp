#pragma comment(linker,"/subsystem:\"Windows\" /entry:\"mainCRTStartup\"")
#include "DaemonService.h"
#include<iostream>

int main(int argc, char* argv[]) {
	std::string directory = argv[0];//program full path + name of binary file
	directory.erase(directory.find_last_of('\\') + 1);//remove name of binary file
	DaemonService serv(directory);
	
	serv.ServiceWorkerThread();
	
	return 0;
}