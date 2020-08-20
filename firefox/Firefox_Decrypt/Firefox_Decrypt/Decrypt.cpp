#include"Firefox_decrypt.h"
unsigned char* decrypt(string encryptedString) {
	// Base64Ω‚¬Î
	size_t szDecoded = encryptedString.size() / 4 * 3 - charCount(encryptedString.c_str(), encryptedString.size(), '=');
	char* chDecoded = (char*)malloc(szDecoded + 1);
	//memset(chDecoded, NULL, szDecoded+1);

	SECItem encrypted, decrypted;
	decrypted.data = NULL;
	decrypted.len = NULL;
	encrypted.data = (unsigned char*)malloc(szDecoded + 1);
	encrypted.len = (unsigned int)szDecoded;
	//memset(encrypted.data, NULL, szDecoded + 1);
	
	// Firefoxº”√‹Ω‚¬Î
	if (PL_Base64Decode(encryptedString.c_str(), (unsigned int)encryptedString.size(), chDecoded)&&(chDecoded!=NULL)&&(encrypted.data!=NULL)) {
		memcpy(encrypted.data, chDecoded, szDecoded);
		//printf("encrypted.data is :%s\n", encrypted.data);
		PK11SlotInfo* objSlot = PK11_GetInternalKeySlot();
		if (objSlot) {
			if (PK11_Authenticate(objSlot, TRUE, NULL) == SECSuccess) {
				SECStatus s = PK11SDR_Decrypt(&encrypted, &decrypted, nullptr);
				//printf("decrypted.data is :%s\n", decrypted.data);
			}
			else {
				printf("Auth err!\n");
			}
		}
		else {
			printf("OBJ err!\n");
		}
		PK11_FreeSlot(objSlot);
	}
	unsigned char* temp = (unsigned char*)malloc((_int64)decrypted.len + 1);
	if (temp != NULL) {
		temp[(_int64)decrypted.len] = NULL;
		memcpy(temp, decrypted.data, (__int64)decrypted.len);
	}
	return temp;
}