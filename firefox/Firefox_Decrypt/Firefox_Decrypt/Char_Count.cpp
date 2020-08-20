// 获得Base64编码填充字符的数目
size_t charCount(const char* str, size_t size, const char ch) {
	size_t count = 0;
	for (size_t i = size - 1; i > size - 4; i--) {
		if (str[i] == ch)
			count++;
		else
			break;
	}
	return count;
}