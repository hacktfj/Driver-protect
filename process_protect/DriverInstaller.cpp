// DriverInstaller.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <windows.h>
#include <stdio.h>

#include "driver.h"


//static TCHAR GetSysFolder()
//{
//	TCHAR szPath[100] = { 0 };
//	GetSystemDirectory(szPath, 100);
//	return szPath;
//}

//用winhex将驱动转成字节数组就可以
int ReleaseFile(TCHAR FileName[])
{
	HANDLE hFile = CreateFile(FileName,
		GENERIC_ALL,
		FILE_SHARE_WRITE | FILE_SHARE_READ,
		NULL,
		CREATE_ALWAYS,
		FILE_ATTRIBUTE_NORMAL,
		NULL);
	if (hFile == INVALID_HANDLE_VALUE)
	{
		printf("文件创建失败%x\n", GetLastError());

		return -1;
	}

	DWORD dwret;
	BOOL bret = WriteFile(hFile, driverdata, sizeof(driverdata), &dwret, NULL);
	if (!bret)
	{
		printf("释放文件失败%x\n", GetLastError());
		CloseHandle(hFile);
		return -2;
	}

	CloseHandle(hFile);

	return 0;
}

VOID DriverInstall1()
{
	ReleaseFile((TCHAR*)TEXT("C:\\windows\\system32\\drivers\\loadfirstProtect.sys"));
	DWORD dwTag = 1;
	//服务控制管理器的方式
	SC_HANDLE hscmanager = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
	SC_HANDLE hService;
	hService = CreateService(
		hscmanager,
		TEXT("loadfirstProtect"),
		TEXT("loadfirstProtect"),
		SERVICE_ALL_ACCESS,
		SERVICE_KERNEL_DRIVER,
		SERVICE_BOOT_START,
		SERVICE_ERROR_IGNORE,
		TEXT("System32\\Drivers\\loadfirstProtect.sys"),
		TEXT("System Reserved"),
		&dwTag, NULL, NULL, NULL
	);
	//if (hService != NULL)
	//{
	//	StartService(hService,NULL,NULL);
	//}
	CloseServiceHandle(hscmanager);
	CloseServiceHandle(hService);
}
int main()
{
    std::cout << "Hello World! 驱动抢先加载器 running\n";
	DriverInstall1();
	return 0;
}

// 运行程序: Ctrl + F5 或调试 >“开始执行(不调试)”菜单
// 调试程序: F5 或调试 >“开始调试”菜单

// 入门使用技巧: 
//   1. 使用解决方案资源管理器窗口添加/管理文件
//   2. 使用团队资源管理器窗口连接到源代码管理
//   3. 使用输出窗口查看生成输出和其他消息
//   4. 使用错误列表窗口查看错误
//   5. 转到“项目”>“添加新项”以创建新的代码文件，或转到“项目”>“添加现有项”以将现有代码文件添加到项目
//   6. 将来，若要再次打开此项目，请转到“文件”>“打开”>“项目”并选择 .sln 文件
