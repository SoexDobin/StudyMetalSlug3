#include "pch.h"
#include "CBombExplode.h"
#include "CAnimation.h"

#include "CBmpManager.h"
#include "CSoundManager.h"
#include "CColliderFactory.h"

CBombExplode::CBombExplode()
	: m_pAnim(nullptr), m_iLivingFrame(1)
{
}

CBombExplode::~CBombExplode()
{
	Release();
}

void CBombExplode::Initialize()
{
	LoadBmp();
	m_eType = EXPLODE;
	m_eDamageFlag = ENEMY;
	m_iDamage = 5;

	m_vSize = Vector2(100.f, 200.f);
	m_iLivingFrame = 1;
	m_pAnim->AddAnimation(L"Bomb_Explode", pair<int, int>{0, 23});
	m_pAnim->ChangeAnimation(L"Bomb_Explode");
	m_pAnim->SetFrameSpeed(0.05f);

	m_pColBox = CColliderFactory::Create(this, HITBOX);
	CSoundManager::GetInstance().PlaySound(L"Bomb_Explode.mp3", SOUND_EFFECT, 0.2f);
}

int CBombExplode::Update()
{
	if (m_iLivingFrame <= 0)
		m_pColBox->SetEnableCol(false);

	if (m_pAnim->GetEndOneLoop())
		m_bDestroy = true;

	if (m_bDestroy) return OBJ_DESTROY;

	__super::UpdateGameObject();

	--m_iLivingFrame;

	return OBJ_NOEVENT;
}

void CBombExplode::LateUpdate()
{
	m_pAnim->UpdateAnimation();
}

void CBombExplode::Render(HDC _hDC)
{
	m_pAnim->RenderAnimation(_hDC);
}

void CBombExplode::Release()
{
	SafeDelete<CAnimation*>(m_pAnim);
	SafeDelete<CCollider*>(m_pColBox);
}

void CBombExplode::OnCollision(CGameObject* _pCol, Vector2 _vColSize, COLLISION_COL_FLAG _eFlag)
{
	
}

void CBombExplode::LoadBmp()
{
	m_pAnim = new CAnimation();
	m_pAnim->SetLastFacingX(1);
	m_pAnim->SetLoop(false);

	m_pAnim->SetParent(this);
	m_pAnim->Initialize();
	m_pAnim->SetEaseColor(RGB(255, 0, 255));
}
