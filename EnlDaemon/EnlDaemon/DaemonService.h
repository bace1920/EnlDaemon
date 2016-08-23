#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <windows.h>
#include <tlhelp32.h>
#include <cstring>
#include <vector>
#include <fstream>
#include <sstream>
#include <iostream>
#include <ctime>

const int NAME_SIZE = 50;	//size of process name
const int COMMAND_SIZE = 200;	//size of prcessStartCommand, also be used when read file - to be the size of line 
const int DEBUG = 0xFFFFFFFC;
const int INFO = 0xFFFFFFFD;
const int WARNING = 0xFFFFFFFE;
const int ERR = 0xFFFFFFFF;

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
	int MaxProcess;
	bool Alive;
}Process;

class DaemonService {
public:
	DaemonService(std::string Path);
	~DaemonService(void);
	void ServiceWorkerThread(void);

private:
	std::string Path;
	char LogStr[1005];
	int FlushTime;
	inline int WriteToLog(char* Str = "", int LogType = INFO);
	inline int GetPrecessNum(char* ProcessName);
	inline int KillProcess(char* ProcessName);
	inline void ReadConfig(std::vector<Process>& ProcessList);
};