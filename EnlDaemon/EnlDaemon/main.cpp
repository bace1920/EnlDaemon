#pragma once
#pragma comment(linker,"/subsystem:\"Windows\" /entry:\"mainCRTStartup\"")
#include "DaemonService.h"

int main(int argc, char* argv[]) {
	
	std::string Directory = argv[0];//program full path + name of binary file
	Directory.erase(Directory.find_last_of('\\') + 1);//remove name of binary file
	DaemonService Serv(Directory);
	
	Serv.ServiceWorkerThread();
	return 0;
}