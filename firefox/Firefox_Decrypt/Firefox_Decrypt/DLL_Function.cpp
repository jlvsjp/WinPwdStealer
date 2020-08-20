#include"Firefox_decrypt.h"
// Global Functions
fpNSS_Init NSS_Init;
fpPL_Base64Decode PL_Base64Decode;
fpPK11SDR_Decrypt PK11SDR_Decrypt;
fpPK11_Authenticate PK11_Authenticate;
fpPK11_GetInternalKeySlot PK11_GetInternalKeySlot;
fpPK11_FreeSlot PK11_FreeSlot;
fpNSS_Shutdown NSS_Shutdown;

void dllFunction(HMODULE lib) {
	// 通过模块句柄，GetProcAddress函数检索指定的动态链接库(DLL)中的输出库函数地址。
	NSS_Init = (fpNSS_Init)GetProcAddress(lib, "NSS_Init");	// 初始化
	PL_Base64Decode = (fpPL_Base64Decode)GetProcAddress(lib, "PL_Base64Decode");	// Base64解码
	PK11SDR_Decrypt = (fpPK11SDR_Decrypt)GetProcAddress(lib, "PK11SDR_Decrypt");	// SDR解密
	PK11_Authenticate = (fpPK11_Authenticate)GetProcAddress(lib, "PK11_Authenticate");	// 使用主密码对slot鉴权
	PK11_GetInternalKeySlot = (fpPK11_GetInternalKeySlot)GetProcAddress(lib, "PK11_GetInternalKeySlot");	// 得到内部key槽
	PK11_FreeSlot = (fpPK11_FreeSlot)GetProcAddress(lib, "PK11_FreeSlot");	// 释放获得的key槽
	NSS_Shutdown = (fpNSS_Shutdown)GetProcAddress(lib, "NSS_Shutdown");	// 关闭
}