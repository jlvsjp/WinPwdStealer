#pragma once
#ifndef _FirefoxDecrypt_
#define _FirefoxDecrypt_
#include<iostream>
#include <Windows.h>
#include <string>
using std::string;

// Structs and typedefs
typedef enum {
	siBuffer,
	siClearDataBuffer,
	siCipherDataBuffer,
	siDERCertBuffer,
	siEncodedCertBuffer,
	siDERNameBuffer,
	siEncodedNameBuffer,
	siAsciiNameString,
	siAsciiString,
	siDEROID
} SECItemType;

typedef struct SECItemStr SECItem;

struct SECItemStr {
	SECItemType type;
	unsigned char* data;
	unsigned int len;
};

typedef enum _SECStatus {
	SECWouldBlock = -2,
	SECFailure = -1,
	SECSuccess = 0
} SECStatus;

typedef unsigned int PRUint32;// For PL_Base64Decode
typedef void PK11SlotInfo; // For PK11_Authenticate
typedef int PRBool; // For PK11_Authenticate

typedef SECStatus(*fpNSS_Init)(const char* configdir);
typedef char* (*fpPL_Base64Decode)(const char* src, PRUint32 srclen, char* dest);
typedef SECStatus(*fpPK11SDR_Decrypt)(SECItem* data, SECItem* result, void* cx);
typedef SECStatus(*fpPK11_Authenticate)(PK11SlotInfo* slot, PRBool loadCerts, void* wincx);
typedef PK11SlotInfo* (*fpPK11_GetInternalKeySlot)();
typedef void (*fpPK11_FreeSlot)(PK11SlotInfo* slot);
typedef SECStatus(*fpNSS_Shutdown)();

// 全局变量
extern fpNSS_Init NSS_Init;
extern fpPL_Base64Decode PL_Base64Decode;
extern fpPK11SDR_Decrypt PK11SDR_Decrypt;
extern fpPK11_Authenticate PK11_Authenticate;
extern fpPK11_GetInternalKeySlot PK11_GetInternalKeySlot;
extern fpPK11_FreeSlot PK11_FreeSlot;
extern fpNSS_Shutdown NSS_Shutdown;

// Functions
string getInstallationPath();
HMODULE loadLibrary(string installationPath);
void dllFunction(HMODULE lib);
string getProfilePath();
char* getBuffer(string profilePath);
unsigned char* decrypt(string encryptedString);
size_t charCount(const char* str, size_t size, const char ch);
char* U2G(const char* utf8);

#endif // !_FirefoxDecrypt_