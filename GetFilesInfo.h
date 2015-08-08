
struct myWIN32_FIND_DATA : public WIN32_FIND_DATA
{
	bool operator<(const WIN32_FIND_DATA &rhs) const
	{
		return lstrcmpi(cFileName, rhs.cFileName) < 0;
	}
};

class FILESINFOW {
public:
	std::wstring directry_;
	std::vector<myWIN32_FIND_DATA> wfds_;

	void Sort(){
		std::sort(wfds_.begin(), wfds_.end());
	}
	size_t GetCount() const {
		return wfds_.size();
	}
	const WIN32_FIND_DATA& operator[](int i) const
	{
		return wfds_[i];
	}
};

BOOL GetFilesInfoW(LPCWSTR pDirectory, FILESINFOW& ret);