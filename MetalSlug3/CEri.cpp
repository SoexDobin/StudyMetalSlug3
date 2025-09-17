#include "pch.h"
#include "CEri.h"
#include "CAnimation.h"

// Managers
#include "CBmpManager.h"
#include "CKeyManager.h"

CEri::CEri()
	: m_pBodyAnim(nullptr), m_pLegAnim(nullptr)
	, m_eCurBodyState(PLAYER_STATE_END), m_ePrevBodyState(PLAYER_STATE_END)
	, m_eCurLegState(PLAYER_STATE_END), m_ePrevLegState(PLAYER_STATE_END)
{
}

CEri::~CEri()
{
	Release();
}

void CEri::Initialize()
{
	m_vPivot = Vector2(120, 600);
	m_vSize = Vector2(PLAYER_BMPX, PLAYER_BMPY);
	m_vFace = Vector2::UnitX;
	m_vDirection = Vector2(0.f, 0.f);
	
	

	LoadEriBmp();
}

int CEri::Update()
{
	if (m_bDestroy) return OBJ_DESTROY;

	__super::UpdateGameObject();

	return OBJ_NOEVENT;
}

void CEri::LateUpdate()
{
	// DROP여부 미리 확인해서 키입력에 활용할 것

	BehaviourKeyInput();
	AttackKeyInput();

	m_ePrevBodyState = m_ePrevBodyState;
	m_ePrevLegState = m_ePrevLegState;
}

void CEri::Render(HDC _hDC)
{
	Rectangle(_hDC, m_tRect.left, m_tRect.top, m_tRect.right, m_tRect.bottom);
	m_pLegAnim->RenderAnimation(_hDC);
	m_pBodyAnim->RenderAnimation(_hDC);
}

void CEri::Release()
{
	SafeDelete<CAnimation*>(m_pBodyAnim);
	SafeDelete<CAnimation*>(m_pLegAnim);
}

void CEri::OnCollision(CGameObject* _pCol, Vector2 _vColSize)
{
}

void CEri::BehaviourKeyInput()
{
	if (CKeyManager::GetInstance().KeyPressing(VK_DOWN)
		&& !CKeyManager::GetInstance().KeyPressing(VK_UP))
	{

		m_eCurBodyState = SIT;
		return;
	}

	if (CKeyManager::GetInstance().KeyPressing(m_cJumpKey))
	{
		// TODO : 점프 중일 때는 현재 프레임 받아와서 그시점부터 적용
		// TODO : DROP도 동일 && 단 DROP은 마지막 프레임이면 마지막으로 고정

		m_eCurBodyState = JUMP;
		m_eCurLegState = JUMP;
	}

	if (CKeyManager::GetInstance().KeyPressing(VK_RIGHT))
	{

		m_eCurLegState = MOVE;
	}
	else if (CKeyManager::GetInstance().KeyPressing(VK_LEFT))
	{

		m_eCurLegState = MOVE;
	}

	if (CKeyManager::GetInstance().KeyPressing(VK_UP))
	{
		


	}
	else
	{

		m_eCurLegState = IDLE;
	}
}

void CEri::AttackKeyInput()
{
	if (m_eCurBodyState == SIT)
	{
		// TODO 근접 공격 여부 조건 추가
		if (CKeyManager::GetInstance().KeyPressing(m_cAttackKey))
		{

			m_eCurLegState = SHOOT;
		}
		else if (CKeyManager::GetInstance().KeyPressing(VK_RIGHT))
		{

			m_eCurLegState = MOVE;
		}
		else if (CKeyManager::GetInstance().KeyPressing(VK_LEFT))
		{

			m_eCurLegState = MOVE;
		}
		else
		{
			
			m_eCurLegState = IDLE;
		}
	}
	else
	{
		// TODO 점프중일때
		if (m_eCurBodyState == JUMP || m_eCurBodyState == MOVEJUMP)
			;
		else if (m_eCurBodyState == DROP)

		// TODO 근접 공격 여부 조건 추가
		if (CKeyManager::GetInstance().KeyPressing(m_cAttackKey))
		{

			m_eCurBodyState = SHOOT;
		}
		else
		{

			m_eCurBodyState = IDLE;
		}
	}
}

void CEri::LoadEriBmp()
{
	m_pBodyAnim = new CAnimation();
	m_pLegAnim = new CAnimation();
	m_pBodyAnim->SetParent(this);
	m_pLegAnim->SetParent(this);

	// sit
	CBmpManager::GetInstance().InsertBmp(L"../Resource/Bmp/Eri/Eri_Sit_CQC.bmp"
		, L"Eri_Sit_CQC");
	CBmpManager::GetInstance().InsertBmp(L"../Resource/Bmp/Eri/Eri_Sit_Idle.bmp"
		, L"Eri_Sit_Idle");
	CBmpManager::GetInstance().InsertBmp(L"../Resource/Bmp/Eri/Eri_Sit_Move.bmp"
		, L"Eri_Sit_Move");
	CBmpManager::GetInstance().InsertBmp(L"../Resource/Bmp/Eri/Eri_Sit_Shoot.bmp"
		, L"Eri_Sit_Shoot");
	m_pLegAnim->AddAnimation(L"Eri_Sit_CQC", pair<int, int>{0, 6});
	m_pLegAnim->AddAnimation(L"Eri_Sit_Idle", pair<int, int>{0, 4});
	m_pLegAnim->AddAnimation(L"Eri_Sit_Move", pair<int, int>{0, 7});
	m_pLegAnim->AddAnimation(L"Eri_Sit_Shoot", pair<int, int>{0, 4});

	// idle
	CBmpManager::GetInstance().InsertBmp(L"../Resource/Bmp/Eri/Eri_Standing_Idle_Body.bmp"
		, L"Eri_Standing_Idle_Body");
	CBmpManager::GetInstance().InsertBmp(L"../Resource/Bmp/Eri/Eri_Standing_Idle_Leg.bmp"
		, L"Eri_Standing_Idle_Leg");
	m_pBodyAnim->AddAnimation(L"Eri_Standing_Idle_Body", pair<int, int>{0, 4});
	m_pLegAnim->AddAnimation(L"Eri_Standing_Idle_Leg", pair<int, int>{0, 4});

	// move
	CBmpManager::GetInstance().InsertBmp(L"../Resource/Bmp/Eri/Eri_Standing_Move_Body.bmp"
		, L"Eri_Standing_Move_Body");
	CBmpManager::GetInstance().InsertBmp(L"../Resource/Bmp/Eri/Eri_Standing_Move_Leg.bmp"
		, L"Eri_Standing_Move_Leg");
	m_pBodyAnim->AddAnimation(L"Eri_Standing_Move_Body", pair<int, int>{0, 12});
	m_pLegAnim->AddAnimation(L"Eri_Standing_Move_Leg", pair<int, int>{0, 12});

	// CQC
	CBmpManager::GetInstance().InsertBmp(L"../Resource/Bmp/Eri/Eri_Standing_AxeCQC_Body.bmp"
		, L"Eri_Standing_AxeCQC_Body");
	CBmpManager::GetInstance().InsertBmp(L"../Resource/Bmp/Eri/Eri_Standing_TonfaCQC_Body.bmp"
		, L"Eri_Standing_TonfaCQC_Body");
	m_pBodyAnim->AddAnimation(L"Eri_Standing_AxeCQC_Body", pair<int, int>{0, 6});
	m_pBodyAnim->AddAnimation(L"Eri_Standing_TonfaCQC_Body", pair<int, int>{0, 6});

	// jump
	CBmpManager::GetInstance().InsertBmp(L"../Resource/Bmp/Eri/Eri_Standing_Jump_Body.bmp"
		, L"Eri_Standing_Jump_Body");
	CBmpManager::GetInstance().InsertBmp(L"../Resource/Bmp/Eri/Eri_Standing_Jump_Leg.bmp"
		, L"Eri_Standing_Jump_Leg");
	CBmpManager::GetInstance().InsertBmp(L"../Resource/Bmp/Eri/Eri_Standing_JumpFront_Leg.bmp"
		, L"Eri_Standing_JumpFront_Leg");
	m_pBodyAnim->AddAnimation(L"Eri_Standing_Jump_Body", pair<int, int>{0, 6});
	m_pLegAnim->AddAnimation(L"Eri_Standing_Jump_Leg", pair<int, int>{0, 6});
	m_pLegAnim->AddAnimation(L"Eri_Standing_JumpFront_Leg", pair<int, int>{0, 6});

	// drop
	CBmpManager::GetInstance().InsertBmp(L"../Resource/Bmp/Eri/Eri_Standing_Drop_Body.bmp"
		, L"Eri_Standing_Drop_Body");
	CBmpManager::GetInstance().InsertBmp(L"../Resource/Bmp/Eri/Eri_Standing_Drop_Leg.bmp"
		, L"Eri_Standing_Drop_Leg");
	m_pBodyAnim->AddAnimation(L"Eri_Standing_Drop_Body", pair<int, int>{0, 6});
	m_pLegAnim->AddAnimation(L"Eri_Standing_Drop_Leg", pair<int, int>{0, 6});

	// shoot
	CBmpManager::GetInstance().InsertBmp(L"../Resource/Bmp/Eri/Eri_Standing_ShootFront_Body.bmp"
		, L"Eri_Standing_ShootFront_Body");
	CBmpManager::GetInstance().InsertBmp(L"../Resource/Bmp/Eri/Eri_Standing_ShootUp_Body.bmp"
		, L"Eri_Standing_ShootUp_Body");
	CBmpManager::GetInstance().InsertBmp(L"../Resource/Bmp/Eri/Eri_Standing_ShootDown_Body.bmp"
		, L"Eri_Standing_ShootDown_Body");
	m_pBodyAnim->AddAnimation(L"Eri_Standing_ShootFront_Body", pair<int, int>{0, 4});
	m_pBodyAnim->AddAnimation(L"Eri_Standing_ShootUp_Body", pair<int, int>{0, 4});
	m_pBodyAnim->AddAnimation(L"Eri_Standing_ShootDown_Body", pair<int, int>{0, 4});

	CBmpManager::GetInstance().InsertBmp(L"../Resource/Bmp/Eri/Eri_Standing_ShootFrontToDown_Body.bmp"
		, L"Eri_Standing_ShootFrontToDown_Body");
	CBmpManager::GetInstance().InsertBmp(L"../Resource/Bmp/Eri/Eri_Standing_ShootDownToFront_Body.bmp"
		, L"Eri_Standing_ShootDownToFront_Body");
	m_pBodyAnim->AddAnimation(L"Eri_Standing_ShootFrontToDown_Body", pair<int, int>{0, 2});
	m_pBodyAnim->AddAnimation(L"Eri_Standing_ShootDownToFront_Body", pair<int, int>{0, 2});

	CBmpManager::GetInstance().InsertBmp(L"../Resource/Bmp/Eri/Eri_Standing_ShootFrontToUp_Body.bmp"
		, L"Eri_Standing_ShootFrontToUp_Body");
	CBmpManager::GetInstance().InsertBmp(L"../Resource/Bmp/Eri/Eri_Standing_ShootUpToFront_Body.bmp"
		, L"Eri_Standing_ShootUpToFront_Body");
	m_pBodyAnim->AddAnimation(L"Eri_Standing_ShootFrontToUp_Body", pair<int, int>{0, 2});
	m_pBodyAnim->AddAnimation(L"Eri_Standing_ShootUpToFront_Body", pair<int, int>{0, 2});
}
