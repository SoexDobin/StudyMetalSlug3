#include "pch.h"
#include "CPlatformChecker.h"

#include "CColliderFactory.h"

CPlatformChecker::CPlatformChecker()
	: m_bFindPlatform(false), m_fTop(0.f)
{
	ZeroMemory(&m_vOffset, sizeof(Vector2));
}

CPlatformChecker::~CPlatformChecker()
{
    Release();
}

void CPlatformChecker::Initialize()
{
	m_vPivot = m_pParent->GetPivot() + m_vOffset;
	m_eType = NEUTRAL;
	m_vSize = Vector2(16.f, 16.f);

	m_pColBox = CColliderFactory::Create(this, HITBOX);
}

int CPlatformChecker::Update()
{
	if (m_pParent == nullptr && m_bDestroy) 
		return OBJ_DESTROY;

	__super::UpdateGameObject();
	m_vPivot = m_pParent->GetPivot() + m_vOffset;

	return OBJ_NOEVENT;
}

void CPlatformChecker::LateUpdate()
{

}

void CPlatformChecker::Render(HDC _hDC)
{
	m_bFindPlatform = false;
}

void CPlatformChecker::Release()
{
	m_pParent = nullptr;
	SafeDelete<CCollider*>(m_pColBox);
}

void CPlatformChecker::OnCollision(CGameObject* _pCol, Vector2 _vColSize, COLLISION_COL_FLAG _eFlag)
{
	if (m_bFindPlatform) return;

	if (_pCol->GetObjectType() == PLATFORM)
	{
		m_bFindPlatform = true;
		m_fTop = static_cast<float>(_pCol->GetRect().top);
	}
	else
		m_bFindPlatform = false;
}
