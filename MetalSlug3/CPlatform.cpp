#include "pch.h"
#include "CPlatform.h"
#include "CHitBox.h"
#include "CAnimation.h"

// Managers
#include "CColliderFactory.h"
#include "CBmpManager.h"
#include "CScrollManager.h"

CPlatform::CPlatform()
{

}

CPlatform::~CPlatform()
{

}

void CPlatform::Initialize()
{
    //m_eType = FIELD;
    m_pColBox = CColliderFactory<CHitBox>::CreateHitBox(this);
    
}

int CPlatform::Update()
{
    if (m_bDestroy) return OBJ_DESTROY;

    __super::UpdateGameObject();

    return OBJ_NOEVENT;
}

void CPlatform::LateUpdate()
{
}

void CPlatform::Render(HDC _hDC)
{
    int iX = SCROLLX;
    int iY = SCROLLY;


}

void CPlatform::Release()
{
}

void CPlatform::OnCollision(CGameObject* _pCol, Vector2 _vColSize)
{
    if (_pCol->GetObjectType() == PLAYER
        || _pCol->GetObjectType() == ENEMY)
    {
        _pCol->SetPivot(_pCol->GetPivot() - _vColSize);
    }
}
