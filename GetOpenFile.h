#define OFNBUFFERSIZE 1024
struct OFNBUFFER
{
	
	LPTSTR pFile_;
	LPTSTR pFileTitle_;
	LPTSTR pFilter_;
	OFNBUFFER():
		pFile_(new TCHAR[OFNBUFFERSIZE]),
		pFileTitle_(new TCHAR[OFNBUFFERSIZE]),
		pFilter_(new TCHAR[OFNBUFFERSIZE]) {
			pFile_[0]=0;
			pFileTitle_[0]=0;
			pFilter_[0]=0;
		}

	~OFNBUFFER() {
		delete [] pFile_;
		delete [] pFileTitle_;
		delete [] pFilter_;
	}
};

BOOL GetOpenFile(
				 HWND hWnd, 
				 LPCTSTR pFilter, 
				 LPCTSTR pInitialDir,
				 LPCTSTR pTitle,
				 wstring* fullpath, wstring* path=NULL);

BOOL GetSaveFile(
				 HWND hWnd, 
				 LPCTSTR pFilter, 
				 LPCTSTR pInitialDir,
				 LPCTSTR pInitialFile,
				 LPCTSTR pDefExt,
				 LPCTSTR pTitle,
				 wstring* fullpath, wstring* path=NULL);