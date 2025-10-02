#include "pch.h"
#include "CFSProjectile.h"
#include "CAnimation.h"
#include "CPlatformChecker.h"
#include "CBombExplode.h"

#include "CBmpManager.h"
#include "CColliderFactory.h"
#include "CGameObjectFactory.h"
#include "CObjectManager.h"
#include "CTimeManager.h"
#include "CScoreManager.h"
#include "CSoundManager.h"

CFSProjectile::CFSProjectile()
    : m_pAnim(nullptr)
{
}

CFSProjectile::~CFSProjectile()
{
    Release();
}

void CFSProjectile::Initialize()
{
    LoadBmp();
    m_fSpeed = 150.f;
    m_vSize = Vector2(288.f, 288.f);
    m_iDamage = 3;
    m_eType = PROJECTILE;
    m_eDamageFlag = ENEMY;
    m_eDamageType = FIRE;

    m_pColBox = CColliderFactory::Create(this, HITBOX);
    m_pColBox->SetSize(m_vSize / 2.f);
    CSoundManager::GetInstance().PlaySound(L"FlameShot_Shoot.mp3", PLAYER_ATTACK, 0.2f);
    __super::UpdateGameObject();
}

int CFSProjectile::Update()
{
    if (m_pAnim->GetEndOneLoop()) m_bDestroy = true;

    if (m_bDestroy) return OBJ_DESTROY;

    __super::UpdateGameObject();

    if (m_vDirection.x < 0)
        m_vFace = Vector2::UnitX * -1.f;
    else
        m_vFace = Vector2::UnitX;

    if (m_vDirection.y == 0.f)
    {
        m_vPivot.x += m_vDirection.x * m_fSpeed * DELTA;
        m_vPivot.y -= 0.2f * m_fSpeed * DELTA;
        m_pAnim->SetDeltaFrame(m_pAnim->GetDeltaFrame() * 3.f);
    }
    else
    {
        m_vPivot.y += m_vDirection.y * m_fSpeed * DELTA;
        m_vPivot.x -= 0.1f * m_fSpeed * DELTA;
        m_pAnim->SetDeltaFrame(m_pAnim->GetDeltaFrame() * 3.f);
    }
    

    return OBJ_NOEVENT;
}

void CFSProjectile::LateUpdate()
{
    m_pAnim->UpdateAnimation();
}

void CFSProjectile::Render(HDC _hDC)
{
    m_pAnim->RenderAnimation(_hDC);
}

void CFSProjectile::Release()
{
    SafeDelete<CAnimation*>(m_pAnim);
    SafeDelete<CCollider*>(m_pColBox);
}

void CFSProjectile::OnCollision(CGameObject* _pCol, Vector2 _vColSize, COLLISION_COL_FLAG _eFlag)
{
    if (_pCol->GetObjectType() == ENEMY)
    {
        CScoreManager::GetInstance().ACCScore(100);
    }
}

void CFSProjectile::LoadBmp()
{
    m_pAnim = new CAnimation();
    m_pAnim->AddAnimation(L"FlameShot_Front", pair<int, int>{0, 37});
    m_pAnim->ChangeAnimation(L"FlameShot_Front");
    m_pAnim->SetLoop(false);
    m_pAnim->SetFrameSpeed(0.05f);

    m_pAnim->Initialize();
    m_pAnim->SetParent(this);
}