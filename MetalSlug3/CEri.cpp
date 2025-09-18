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
	, m_iLastFaceX(1)
{
}

CEri::~CEri()
{
	Release();
}

void CEri::Initialize()
{
	m_vPivot = Vector2(200, 500);
	m_vSize = Vector2(PLAYER_BMPX, PLAYER_BMPY);
	m_vFace = Vector2::UnitX;
	m_vDirection = Vector2(0.f, 0.f);
	m_eType = PLAYER;

	LoadEriBmp();
	m_ePrevBodyState = IDLE;
	m_ePrevLegState = IDLE;
	m_pBodyAnim->ChangeAnimation(L"Eri_Standing_Idle_Body");
	m_pLegAnim->ChangeAnimation(L"Eri_Standing_Idle_Leg");
}

int CEri::Update()
{
	if (m_bDestroy) return OBJ_DESTROY;

	m_pBodyAnim->UpdateAnimation();
	m_pLegAnim->UpdateAnimation();
	__super::UpdateGameObject();

	return OBJ_NOEVENT;
}

void CEri::LateUpdate()
{
	// DROP여부 미리 확인해서 키입력에 활용할 것

	BehaviourKeyInput();
	AttackKeyInput();

	m_ePrevBodyState = m_eCurBodyState;
	m_ePrevLegState = m_eCurLegState;
}

void CEri::Render(HDC _hDC)
{
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
		m_pBodyAnim->ChangeAnimation(L"Eri_Blank_Body");
		return;
	}
	else
	{
		m_eCurBodyState = IDLE;
	}


	if (CKeyManager::GetInstance().KeyPressing(m_cJumpKey))
	{
		// TODO : 점프 중일 때는 현재 프레임 받아와서 그시점부터 적용
		// TODO : DROP도 동일 && 단 DROP은 마지막 프레임이면 마지막으로 고정


		m_eCurLegState = JUMP;
		return;
	}

	if (CKeyManager::GetInstance().KeyPressing(VK_RIGHT))
	{
		m_vFace = Vector2::UnitX;
		m_vDirection = Vector2::UnitX;
		m_eCurLegState = MOVE;
		m_pLegAnim->ChangeAnimation(L"Eri_Standing_Move_Leg");
	}
	else if (CKeyManager::GetInstance().KeyPressing(VK_LEFT))
	{
		m_vFace = Vector2::UnitX * -1.f;
		m_vDirection = Vector2::UnitX * -1.f;
		m_eCurLegState = MOVE;
		m_pLegAnim->ChangeAnimation(L"Eri_Standing_Move_Leg");
	}
	else
	{
		m_vDirection = Vector2::Zero;
		m_eCurLegState = IDLE;
		m_pLegAnim->ChangeAnimation(L"Eri_Standing_Idle_Leg");
	}
}

void CEri::AttackKeyInput()
{
	if (m_eCurBodyState == SIT)
	{
		// TODO 근접 공격 여부 조건 추가
		if (CKeyManager::GetInstance().KeyPressing(m_cAttackKey))
		{
			if (CKeyManager::GetInstance().KeyPressing(VK_RIGHT))
				m_vFace = Vector2::UnitX;
			else if (CKeyManager::GetInstance().KeyPressing(VK_LEFT))
				m_vFace = Vector2::UnitX * -1.f;

			m_vDirection = Vector2::Zero;
			m_eCurLegState = SHOOT;
			m_pLegAnim->ChangeAnimation(L"Eri_Sit_Shoot");
		}
		else if (CKeyManager::GetInstance().KeyPressing(VK_RIGHT))
		{
			m_vFace = Vector2::UnitX;
			m_vDirection = Vector2::UnitX;
			m_iLastFaceX = m_vFace.x;
			m_eCurLegState = MOVE;
			m_pLegAnim->ChangeAnimation(L"Eri_Sit_Move");
		}
		else if (CKeyManager::GetInstance().KeyPressing(VK_LEFT))
		{
			m_vFace = Vector2::UnitX * -1.f;
			m_vDirection = Vector2::UnitX * -1.f;
			m_iLastFaceX = m_vFace.x;
			m_eCurLegState = MOVE;
			m_pLegAnim->ChangeAnimation(L"Eri_Sit_Move");
		}
		else
		{
			m_vDirection = Vector2::Zero;
			m_eCurLegState = IDLE;
			m_pLegAnim->ChangeAnimation(L"Eri_Sit_Idle");
		}
	}
	else
	{
		// TODO 점프중일때
		if (m_eCurBodyState == JUMP || m_eCurBodyState == MOVEJUMP)
			;
		else if (m_eCurBodyState == DROP)
			;

		// TODO 근접 공격 여부 조건 추가
		if (CKeyManager::GetInstance().KeyPressing(m_cAttackKey))
		{
			m_eCurBodyState = SHOOT;
			
			
			
			if (CKeyManager::GetInstance().KeyPressing(VK_UP)
				&& CKeyManager::GetInstance().KeyPressing(VK_RIGHT))
			{
				m_vFace = Vector2::UnitX;
				m_pBodyAnim->ChangeAnimation(L"Eri_Standing_ShootUp_Body");
			}
			else if (CKeyManager::GetInstance().KeyPressing(VK_UP)
				&& CKeyManager::GetInstance().KeyPressing(VK_LEFT))
			{
				m_vFace = Vector2::UnitX * -1.f;
				m_pBodyAnim->ChangeAnimation(L"Eri_Standing_ShootUp_Body");
			}
			else if (CKeyManager::GetInstance().KeyPressing(VK_UP))
			{
				m_vFace = Vector2::UnitY * -1.f;
				m_pBodyAnim->ChangeAnimation(L"Eri_Standing_ShootUp_Body");
			}
			else if (CKeyManager::GetInstance().KeyPressing(VK_RIGHT))
			{
				m_vFace = Vector2::UnitX;
				m_pBodyAnim->ChangeAnimation(L"Eri_Standing_ShootFront_Body");
			}
			else if (CKeyManager::GetInstance().KeyPressing(VK_LEFT))
			{
				m_vFace = Vector2::UnitX * -1.f;
				m_pBodyAnim->ChangeAnimation(L"Eri_Standing_ShootFront_Body");
			}
			else
			{
				m_pBodyAnim->ChangeAnimation(L"Eri_Standing_ShootFront_Body");
			}
		}
		else
		{

			m_eCurBodyState = IDLE;
			m_pBodyAnim->ChangeAnimation(L"Eri_Standing_Idle_Body");
		}
	}
}

void CEri::LoadEriBmp()
{
	m_pBodyAnim = new CAnimation();
	m_pLegAnim = new CAnimation();

	CBmpManager::GetInstance().InsertBmp(L"../Resource/Bmp/Eri/Eri_Blank_Body.bmp"
		, L"Eri_Blank_Body");
	m_pBodyAnim->AddAnimation(L"Eri_Blank_Body", pair<int, int>{0, 1});
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

	m_pBodyAnim->Initialize();
	m_pLegAnim->Initialize();
	m_pBodyAnim->SetParent(this);
	m_pLegAnim->SetParent(this);
}
