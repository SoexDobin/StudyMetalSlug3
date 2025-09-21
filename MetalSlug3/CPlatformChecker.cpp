#include "pch.h"
#include "CPlatformChecker.h"
#include "CHitBox.h"

#include "CColliderFactory.h"

CPlatformChecker::CPlatformChecker()
	: m_iFindPlatform(false), m_fTop(0.f)
{
}

CPlatformChecker::~CPlatformChecker()
{
    Release();
}

void CPlatformChecker::Initialize()
{
	m_eType = NEUTRAL;
	m_vPivot = m_pParent->GetPivot();
	m_vSize = Vector2(16.f, 16.f);

	m_pColBox = CColliderFactory<CHitBox>::CreateHitBox(this);
	CCollider* pParentCol = m_pParent->GetCollider();
}

int CPlatformChecker::Update()
{
	if (m_pParent == nullptr) return OBJ_NOEVENT;

	__super::UpdateGameObject();

	

	return OBJ_NOEVENT;
}

void CPlatformChecker::LateUpdate()
{

}

void CPlatformChecker::Render(HDC _hDC)
{
	m_iFindPlatform = false;
}

void CPlatformChecker::Release()
{
	m_pParent = nullptr;
}

void CPlatformChecker::OnCollision(CGameObject* _pCol, Vector2 _vColSize, COLLISION_COL_FLAG _eFlag)
{
	if (m_iFindPlatform) return;

	if (_pCol->GetObjectType() == PLATFORM)
	{
		m_iFindPlatform = true;
		m_fTop = static_cast<float>(_pCol->GetRect().top);
	}
	else
		m_iFindPlatform = false;
}
