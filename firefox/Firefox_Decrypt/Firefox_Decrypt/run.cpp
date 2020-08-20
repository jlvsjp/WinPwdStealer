#include"Firefox_decrypt.h"
#include<regex>

int main() {
	string installationPath = getInstallationPath();	// 获取Firefox的安装路径（通过注册表）
	HMODULE lib = loadLibrary(installationPath);	// 动态加载nss库(Network Security Services)(网络安全服务)，加载动态链接库文件nss.dll，返回模块句柄
	if (lib == NULL)	// 模块句柄为空，则动态加载失败
		return -1;
	dllFunction(lib);	// 获取nss库中等会需要用到的方法
	string profilePath = getProfilePath();	// 获取Firefox保存登录信息(用户名，密码)的文件logins.json的路径
	SECStatus s = NSS_Init(profilePath.c_str());	// 初始化NSS库
	if (s != SECSuccess) {
		printf("Error when initialization!\n");
	}
	string loginStrings = getBuffer(profilePath);	// 获取logins.json中保存的登录信息
	// 正则表达式匹配
	std::regex reHostname("\"hostname\":\"([^\"]+)\"");
	std::regex reUsername("\"encryptedUsername\":\"([^\"]+)\"");
	std::regex rePassword("\"encryptedPassword\":\"([^\"]+)\"");
	std::smatch match;
	string::const_iterator searchStart(loginStrings.cbegin());	// 循环迭代
	while (std::regex_search(searchStart, loginStrings.cend(), match, reHostname)) {
		printf("Host\t: %s \n", U2G(match.str(1).c_str()));
		std::regex_search(searchStart, loginStrings.cend(), match, reUsername);
		printf("Username: %s \n", U2G((const char*)decrypt(match.str(1))));	// decrypt用户名并转码输出
		std::regex_search(searchStart, loginStrings.cend(), match, rePassword);
		printf("Password: %s \n", U2G((const char*)decrypt(match.str(1))));	// decrypt密码并转码输出
		searchStart += match.position() + match.length();
		printf("-----------------------------------------\n");
	}
	NSS_Shutdown();	// 关闭NSS库
	//system("PAUSE");
	return 0;
}