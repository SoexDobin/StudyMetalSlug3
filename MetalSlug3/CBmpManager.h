#pragma once
#include "CBmpObject.h"

class CBmpManager
{
private:
	CBmpManager();
	~CBmpManager();
	CBmpManager(const CBmpManager& _rhs) = delete;
	CBmpManager& operator=(CBmpManager& _rhs) = delete;

public:
	static CBmpManager&		GetInstance();
	static void				DeleteInstance();

public:
	void		InsertBmp(const TCHAR* _pFilePath, const TCHAR* _pImgKey);
	HDC			FindBmpImg(const TCHAR* _pImgKey);
	void		Release();

private:
	static CBmpManager*		m_pInstance;
	unordered_map<const TCHAR*, CBmpObject*> m_umapBitMap;
};

