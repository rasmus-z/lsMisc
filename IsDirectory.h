inline BOOL IsDirectory(LPCTSTR pDir)
{
	DWORD attr = GetFileAttributes(pDir);
	return (attr != 0xFFFFFFFF) && ((attr & FILE_ATTRIBUTE_DIRECTORY) != 0);
}
