#include"Firefox_decrypt.h"
#include <ShlObj_core.h>

constexpr auto Get_failed = "get_ProfilePath Failed";

string getProfilePath() {
	char* appDataPath = (char*)malloc(sizeof(char) * MAX_PATH);
	if (appDataPath != NULL) {
		SHGetFolderPathA(NULL, CSIDL_APPDATA, NULL, 0, appDataPath);	// 获取当前用户的文件系统目录C:\Users\username\AppData\Roaming（CSIDL_APPDATA默认为AppData下的Roaming）
		string profileName = "";
		string sAppDataPath = appDataPath;
		//printf("%s\n", appDataPath);
		sAppDataPath = sAppDataPath + "\\Mozilla\\Firefox\\Profiles\\";	// Firefox的profiles路径
		WIN32_FIND_DATA ffd;	// WIN32_FIND_DATA包含文件的属性信息
		HANDLE hFind = FindFirstFile((sAppDataPath + "\\*").c_str(), &ffd);	// 根据文件名查找文件，成功返回一个句柄
		do {
			if (ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {	// dwFileAttributes是目标文件标记，&表示做与运算，FILE_ATTRIBUTE_DIRECTORY是文件夹的标志符，这句if的功能就是判断目标文件是否为文件夹
				string str = ffd.cFileName;

				if (str.find("release") != str.npos) {	// profiles下带有release的文件夹即为包含logins.json的文件夹，如845f4a08.default-release，安装时随机生成带有release名字的文件
					profileName = ffd.cFileName;
				}
			}
		} while (FindNextFile(hFind, &ffd) != 0);	// 遍历profiles文件夹
		//printf("appdata: %s\n", sAppDataPath.c_str());

		string profilePath = sAppDataPath + profileName;	// logins.json的路径
		return profilePath;
	}
	else
		return Get_failed;
}
	