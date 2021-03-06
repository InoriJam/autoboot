#include <Windows.h>
#include <fstream>
#include <string>

#pragma comment( linker, "/subsystem:\"windows\" /entry:\"mainCRTStartup\"" )
using namespace std;

LPWSTR ConvertCharToLPWSTR(const char * szString)
{
	int dwLen = strlen(szString) + 1;
	int nwLen = MultiByteToWideChar(CP_ACP, 0, szString, dwLen, NULL, 0);
	LPWSTR lpszPath = new WCHAR[dwLen];
	MultiByteToWideChar(CP_ACP, 0, szString, dwLen, lpszPath, nwLen);
	return lpszPath;
}

int main()
{
	STARTUPINFO si_arr[30];
	PROCESS_INFORMATION pi_arr[30];
	ZeroMemory(&si_arr, sizeof(si_arr));
	ZeroMemory(&pi_arr, sizeof(pi_arr));
	
	ifstream cfg("./boot.cfg");
	if (!cfg) {
		MessageBox(NULL, TEXT("Unable to find boot.cfg!"), NULL, NULL);
		return -1;
	}
	string command;
	int count = 0;
	while (getline(cfg, command)) {
		if (!CreateProcess(NULL, ConvertCharToLPWSTR(command.c_str()) , NULL, NULL, NULL, NULL, NULL, NULL, &si_arr[count], &pi_arr[count])) {
			MessageBox(NULL, TEXT("Opps, there is something wrong!"), NULL, NULL);
			return -2;
		}
		count += 1;
	}
	HANDLE hProcess_arr[30];
	for (int i = 0; i < count; i++) {
		hProcess_arr[i] = pi_arr[i].hProcess;
	}
	WaitForMultipleObjects(count, hProcess_arr, TRUE, INFINITE);
	return 0;
}
