#include "pch.h"
#include "CChangeTrigger.h"
#include "CScene.h"

#include "CColliderFactory.h"
#include "CObjectManager.h" 
#include "CSceneManager.h"

CChangeTrigger::CChangeTrigger()
{
}

CChangeTrigger::~CChangeTrigger()
{
    Release();
}

void CChangeTrigger::Initialize()
{
    m_eType = NEUTRAL;
    m_pColBox = CColliderFactory::Create(this, HITBOX);
    __super::UpdateGameObject();
}

int CChangeTrigger::Update()
{
    if (m_bDestroy) return OBJ_DESTROY;

    __super::UpdateGameObject();

    return OBJ_NOEVENT;
}

void CChangeTrigger::LateUpdate()
{

}

void CChangeTrigger::Render(HDC _hDC)
{

}

void CChangeTrigger::Release()
{
    SafeDelete<CCollider*>(m_pColBox);
}

void CChangeTrigger::OnCollision(CGameObject* _pCol, Vector2 _vColSize, COLLISION_COL_FLAG _eFlag)
{
    if (_pCol->GetObjectType() == PLAYER
        && CObjectManager::GetInstance().GetGameObjectList(ENEMY).empty())
    {
        m_bDestroy = true;
        SafeDelete<CCollider*>(m_pColBox);
        CSceneManager::GetInstance().GetCurScene()->SetSceneState(SCENE_DESTROY);
    }
}
