#pragma once
#include "DaemonService.h"


DaemonService::DaemonService(std::string Path){
	
	//WriteToLog("Initializing.", INFO);
	this->Path = Path;
	FlushTime = 5000;
}


DaemonService::~DaemonService(void){
}

int DaemonService::WriteToLog(char* Str, int LogType) {
	std::fstream Log(Path + "DaemonService.log", std::ios::app);
	
	time_t now_time;
	now_time = time(NULL);
	tm* t = localtime(&now_time);

	char Time[30];
	
	if (Log.fail()) {
		//std::cout << "No Log" << std::endl;
		return -1;
	}
	
	sprintf(Time,"[%d-%02d-%02d %02d:%02d:%02d]",
		t->tm_year + 1900,
		t->tm_mon + 1,
		t->tm_mday,
		t->tm_hour,
		t->tm_min,
		t->tm_sec);

	if (LogType >= INFO) {
		//std::cout << Time << Str << std::endl;
		Log << Time << Str << std::endl;
	}
		
	Log.close();

	return 0;
}

void DaemonService::ReadConfig(std::vector<Process>& ProcessList) {
	Process Temp;
	ProcessList.clear();
	std::fstream Config;
	char Str[COMMAND_SIZE];
	Config.open(Path + "DaemonService.config",std::ios::in);
	if (!Config) {
		Config.open(Path + "\\DaemonService.config",std::ios::out);
		Config << "2000" << std::endl;
		Config << "# example.exe" << std::endl;
		Config << "# \"C:\\example.exe\" -m -n" << std::endl;
		Config << "# 2" << std::endl;
		WriteToLog("Config do not exit, generated a exapmle config.", INFO);
	} else {
		// the first not empty line should be FlushTime
		Config.getline(Str, COMMAND_SIZE);
		std::stringstream ToInt(Str);
		ToInt >> FlushTime;
		//std::cout << FlushTime<< std::endl;
		// read process info from conifg
		while (Config.getline(Str, COMMAND_SIZE)) {
			// check if this line is empty or its Note
			if (strcmp(Str, "") && Str[0] != '#') {
				// the second not empty line should be ProcessName
				strcpy(Temp.ProcessName, Str);
				// Then the next line should be ProcessStartCommand
				Config.getline(Str, COMMAND_SIZE);
				// wrapped the command with double quotes to avoid error when space in command
				strcpy(Temp.ProcessStartCommand, Str);
				/*strcat(Temp.ProcessStartCommand, Str);
				strcat(Temp.ProcessStartCommand, "\"");*/
				Config.getline(Str, COMMAND_SIZE);
				//Then the next line should be number of arguments
				ToInt.clear();
				ToInt.str(Str);
				ToInt >> Temp.Argc;
				//std::cout << Str<<" "<<Temp.Argc << std::endl;
				// save the info to a list
				Temp.Alive = false;
				ProcessList.push_back(Temp);
			}
		}
		Config.close();
	}
	
	return;
}

int DaemonService::GetPrecessNum(char* ProcessName) {
	int ProcessCount = 0;
	HANDLE hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

	if (hProcessSnap == INVALID_HANDLE_VALUE) {
		WriteToLog("Call CreateToolhelp32Snapshot failed!", ERR);
	}

	PROCESSENTRY32 pe32;
	pe32.dwSize = sizeof(PROCESSENTRY32);

	BOOL bMore = Process32First(hProcessSnap, &pe32);

	while (bMore) {
		bMore = Process32Next(hProcessSnap, &pe32);
		if (strcmp((char*)pe32.szExeFile, ProcessName) == 0) {
			ProcessCount++;
		}
	}
	CloseHandle(hProcessSnap);

	//std::cout << ProcessName << " " << ProcessCount << std::endl;
	return ProcessCount;
}


void DaemonService::ServiceWorkerThread(void)
{
	// if use #pragma comment(linker,"/subsystem:\"Windows\" /entry:\"mainCRTStartup\"")
	// the limit have to me 2
	// i dont know why
	// else it should be 1

	if (GetPrecessNum("ProcessDaemon.exe") > 2) {
		WriteToLog("There is a Daemon running, please close it first.", WARNING);
		//std::cout << "There is a Daemon running, please close it first." << std::endl;
		return;
	}
	WriteToLog("Start service.", INFO);
	std::vector<Process> ProcessList;
	
	// Periodically check if the service is stopping.
	while (true) {
		ReadConfig(ProcessList);
		for (int i = 0; i < ProcessList.size(); i++) {
			if (GetPrecessNum(ProcessList[i].ProcessName) > 0) {
				sprintf(LogStr, "%s is Alive", ProcessList[i].ProcessName);
				WriteToLog(LogStr, DEBUG);
				//std::cout << ProcessList[i].ProcessName << " is alive" << std::endl;
				ProcessList[i].Alive = true;
			}

		}

		for (int i = 0; i < ProcessList.size(); i++) {	
			if (ProcessList[i].Alive == false) {
				sprintf(LogStr, "Auto restart dead process: %s", ProcessList[i].ProcessStartCommand);
				WriteToLog(LogStr, DEBUG);
				//std::cout << ProcessList[i].ProcessStartCommand << std::endl;
				WinExec(ProcessList[i].ProcessStartCommand, ProcessList[i].Argc);
			}
		}


		Sleep(FlushTime);  // Simulate some lengthy operations.
	}

	return;
}