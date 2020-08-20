#include"Firefox_decrypt.h"
// 动态加载库文件 

HMODULE loadLibrary(string installationPath) {
	const char nssLibraryName[] = "nss3.dll";	// 加载动态链接库文件nss.dll
	SetCurrentDirectory(installationPath.c_str());	// 切换到nss.dll文件所在目录

	HMODULE nssLib = LoadLibrary(nssLibraryName);	// 加载nss.dll文件
	if (nssLib == NULL) {	// 模块句柄返回为空，动态加载失败
		printf("Library couldnt loaded!.. %d\n", GetLastError());
	}
	return	nssLib;	// 返回模块句柄
}