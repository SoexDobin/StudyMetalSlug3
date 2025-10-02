#pragma once // 삭제하지 맙시다

// Mecro
#define			WINCX				960
#define			WINCY				720

#define			PURE				= 0
#define			VK_MAX				0xff

#define			PLAYER_BMPX			192
#define			PLAYER_BMPY			288

#define			OBJ_DESTROY			1
#define			OBJ_NOEVENT			0
#define			SCENE_DESTROY		1
#define			SCENE_NOEVENT		0

#define			DELTA				CTimeManager::GetInstance().GetDeltaTime()
#define			SCROLLX				static_cast<int>(CScrollManager::GetInstance().GetScrollX())
#define			SCROLLY				static_cast<int>(CScrollManager::GetInstance().GetScrollY())

// World Variable
extern HWND			g_hWnd;

// Enum
enum OBJECT_TYPE
{
	ENEMY,
	NEUTRAL,
	PROJECTILE,
	EXPLODE,
	PARTICLE,
	PLATFORM,
	PLAYER,
	ITEM,
	UI,
	OBJECT_END
};

enum WEAPON_TYPE
{ 
	HM_GUN, 
	IRON_LIZZARD, 
	FLAME_SHOT,
	WEAPON_END 
};

enum Damage_TYPE
{
	DANMAK,
	FIRE,
	DAMAGE_END
};

enum ITEM_TYPE
{
	DEFAULT,
	WEAPON,
	BOMB,
	ITEM_END
};

enum SCENETAG
{
	MAIN_SCENE,
	BARRACKS_SCENE,
	ARMORY_SCENE,
	MISSION_SCENE_1,
	MISSION_SCENE_2,
	MISSION_SCENE_3,
	PREV_SCENE,

	// BOSS_SCENE,
	EDIT,
	SCENE_END
};

enum ARMOR_TYPE
{
	BIO,
	MACHINE,
	ARMOR_TYPE_END
};

enum COL_RENDER_TYPE
{
	RECT_COL,
	CIRCLE_COL,
	COLTYPE_END
};

enum COLLISION_FLAG
{
	RECT_TO_RECT,
	CIRCLE_TO_CIRCLE,
	RECT_TO_CIRCLE,
	CIRCLE_TO_RECT,
	CHECK_COL_END
};

enum COLLISION_COL_FLAG
{
	LEFT_COL,
	RIGHT_COL,
	UP_COL,
	DOWN_COL,
	COL_END
};

enum COLLIDER_TYPE
{
	HITBOX,
	TRIGGER,
	COL_TYPE_END
};

enum CHANNELID { 
	SOUND_BGM
	, SOUND_EFFECT
	, PLAYER_ATTACK
	, PLAYER_DEAD
	, ENEMY_READY
	, ENEMY_ATTACK
	, ENEMY_SHOOT
	, ENEMY_DEAD1
	, ENEMY_DEAD2
	, ENEMY_DEAD3
	, ENEMY_DEAD4
	, BULLET_HIT
	, NEUTRAL_SOUND
	, EXPLODE_EFFECT
	, ANOUNCE
	, MAXCHANNEL };

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
