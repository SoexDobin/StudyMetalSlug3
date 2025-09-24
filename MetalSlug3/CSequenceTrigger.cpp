#include "pch.h"
#include "CSequenceTrigger.h"

#include "CColliderFactory.h"
#include "CObjectManager.h" 
#include "CSceneManager.h"

CSequenceTrigger::CSequenceTrigger()
{
}

CSequenceTrigger::~CSequenceTrigger()
{
    Release();
}

void CSequenceTrigger::Initialize()
{
    m_eType = NEUTRAL;
    m_pColBox = CColliderFactory::Create(this, HITBOX);
    __super::UpdateGameObject();
}

int CSequenceTrigger::Update()
{
    if (m_bDestroy) return OBJ_DESTROY;

     
    __super::UpdateGameObject();


    return OBJ_NOEVENT;
}

void CSequenceTrigger::LateUpdate()
{
    
}

void CSequenceTrigger::Render(HDC _hDC)
{
}

void CSequenceTrigger::Release()
{
    SafeDelete<CCollider*>(m_pColBox);
}

void CSequenceTrigger::OnCollision(CGameObject* _pCol, Vector2 _vColSize, COLLISION_COL_FLAG _eFlag)
{
    if (_pCol->GetObjectType() == PLAYER
        && CObjectManager::GetInstance().GetGameObjectList(ENEMY).empty())
    {
        m_bDestroy = true;
        SafeDelete<CCollider*>(m_pColBox);
        CSceneManager::GetInstance().NextSceneSequence();
    }
}
