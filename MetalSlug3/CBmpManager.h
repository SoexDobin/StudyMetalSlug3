#pragma once
#include "CSingleTon.h"
#include "CBmpObject.h"

class CBmpManager final : public CSingleTon<CBmpManager>
{
	friend class CSingleTon;
private:
	CBmpManager();
	~CBmpManager() override;

public:
	void		InsertBmp(const TCHAR* _pFilePath, const TCHAR* _pImgKey);
	HDC			FindBmpImg(const TCHAR* _pImgKey);
	void		Release();

private:
	unordered_map<const TCHAR*, CBmpObject*> m_umapBitMap;
};

