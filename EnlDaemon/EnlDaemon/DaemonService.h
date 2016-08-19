#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <windows.h>
#include <tlhelp32.h>
#include <cstring>
#include <vector>
#include <fstream>
#include <sstream>
#include <iostream>

const int NAME_SIZE = 50;	//size of process name
const int COMMAND_SIZE = 200;	//size of prcessStartCommand, also be used when read file - to be the size of line 

//
//   STRUCT: Process
//
//   PURPOSE: Abstract a monitored object.
//
//   PARAMETERS:
//   * ProcessName   - Name of the ProgeamFile
//   * ProcessStartCommand - Command to start the program be wrapped in double quotes, include Path, FileName, Arguments
//	 * Alive - True if this process still runing, else False
//
typedef struct {
	char ProcessName[NAME_SIZE];
	char ProcessStartCommand[COMMAND_SIZE];
	int Argc;
	bool Alive;
}Process;

class DaemonService {
public:
	DaemonService(std::string Path);
	~DaemonService(void);
	void ServiceWorkerThread(void);

private:
	std::string Path;
	std::string LogStr;
	int FlushTime;
	int WriteToLog(std::string str);
	int GetPrecessNum(char* ProcessName);
	void ReadConfig(std::vector<Process>& ProcessList);
};