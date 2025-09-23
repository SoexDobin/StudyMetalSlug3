#include "pch.h"
#include "CPlatform.h"
#include "CHitBox.h"
#include "CAnimation.h"

// Managers
#include "CColliderFactory.h"
#include "CBmpManager.h"
#include "CScrollManager.h"

CPlatform::CPlatform()
    : m_szImgKey(nullptr)
{

}

CPlatform::~CPlatform()
{
    Release();
}

void CPlatform::Initialize()
{
    m_eType = PLATFORM;
    m_pColBox = CColliderFactory::Create(this, HITBOX);
}

int CPlatform::Update()
{
    if (m_bDestroy) return OBJ_DESTROY;

    __super::UpdateGameObject();
    m_pColBox->UpdateCollision();

    return OBJ_NOEVENT;
}

void CPlatform::LateUpdate()
{
}

void CPlatform::Render(HDC _hDC)
{
    m_pColBox->RenderCollider(_hDC);

    if (m_szImgKey == nullptr) return;
}

void CPlatform::Release()
{
}

void CPlatform::OnCollision(CGameObject* _pCol, Vector2 _vColSize, COLLISION_COL_FLAG _eFlag)
{
    OBJECT_TYPE eType = _pCol->GetObjectType();

    if (eType == PLAYER)
    {
        PlayerPlatformCollision(_pCol, _vColSize, _eFlag);
    } 
    else if (eType == ENEMY || eType == NEUTRAL)
    {
        EnemyPlatformCollision(_pCol, _vColSize, _eFlag);
    }
    else if (eType == PROJECTILE)
    {

    }
}

void CPlatform::PlayerPlatformCollision(CGameObject* _pCol, Vector2 _vColSize, COLLISION_COL_FLAG _eFlag)
{
    switch (_eFlag)
    {
    case LEFT_COL:
        break;
    case RIGHT_COL:
        break;
    case UP_COL:
        break;
    case DOWN_COL:
        break;
    case COL_END:
        break;
    default:
        break;
    }
}

void CPlatform::EnemyPlatformCollision(CGameObject* _pCol, Vector2 _vColSize, COLLISION_COL_FLAG _eFlag)
{

}
