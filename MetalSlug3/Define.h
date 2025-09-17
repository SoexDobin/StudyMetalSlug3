#pragma once // 삭제하지 맙시다

// Mecro
#define			WINCX			960
#define			WINCY			720

#define			PURE			= 0
#define			VK_MAX			0xff

#define			PLAYER_BMPX		192
#define			PLAYER_BMPY		288

#define			OBJ_DESTROY		1
#define			OBJ_NOEVENT		0

// World Variable
extern HWND			g_hWnd;

enum OBJECT_TYPE
{
	PLAYER,
	ENEMY,
	NEUTRAL,
	PROJECTILE,
	EXPLODE,
	PARTICLE,
	UI,
	OBJECT_END
};

enum COL_TYPE
{
	RECT_COL,
	CIRCLE_COL,
	COLTYPE_END
};

// Enum
enum ARMOR_TYPE
{
	BIO,
	MACHINE,
	ARMORTYPE_END
};

// World Function
template<typename T>
void	SafeDelete(T& _p)
{
	if (_p)
	{
		delete _p;
		_p = nullptr;
	}
}

