#include "pch.h"
#include "CEri.h"
#include "CHitBox.h"
#include "CAnimation.h"
#include "CCQCArea.h"
#include "CHMProjectile.h"

// Managers
#include "CBmpManager.h"
#include "CKeyManager.h"
#include "CTimeManager.h"
#include "CScrollManager.h"
#include "CColliderFactory.h"
#include "CGameObjectFactory.h"
#include "CObjectManager.h"
#include "CProjectileFactory.h"

CEri::CEri()
	: m_pBodyAnim(nullptr), m_pLegAnim(nullptr)
	, m_eCurBodyState(PLAYER_STATE_END), m_ePrevBodyState(PLAYER_STATE_END)
	, m_eCurLegState(PLAYER_STATE_END), m_ePrevLegState(PLAYER_STATE_END)
	, m_fMoveSpeed(0.f), m_fCrawlSpeed(0.f)
	, m_fJumpDeltaTime(0.f), m_bIsJump(false)
	, m_iScatterIdx(-1)
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
	m_fMoveSpeed = 200.f;
	m_fCrawlSpeed = 100.f;
	m_eType = PLAYER;

	m_pCQCCollider = CGameObjectFactory<CCQCArea>::Create();
	CObjectManager::GetInstance().AddGameObject(m_pCQCCollider, PLAYER);
	m_pColBox = CColliderFactory<CHitBox>::CreateHitBox(this);

	LoadEriBmp();
	m_ePrevBodyState = IDLE;
	m_ePrevLegState = IDLE;
	m_pBodyAnim->ChangeAnimation(L"Eri_Standing_Idle_Body");
	m_pLegAnim->ChangeAnimation(L"Eri_Standing_Idle_Leg");

	srand(GetTickCount64());
}

int CEri::Update()
{
	if (m_bDestroy) return OBJ_DESTROY;

	m_pBodyAnim->UpdateAnimation();
	m_pLegAnim->UpdateAnimation();
	__super::UpdateGameObject();


	BehaviourKeyInput();
	AttackKeyInput();

	

	return OBJ_NOEVENT;
}

void CEri::LateUpdate()
{
	// DROP여부 미리 확인해서 키입력에 활용할 것
	Move();
	Jump();
	Drop();
	Shoot();
	
}

void CEri::Render(HDC _hDC)
{
	m_pLegAnim->RenderAnimation(_hDC);
	m_pBodyAnim->RenderAnimation(_hDC);

	m_ePrevLegState = m_eCurLegState;
	m_ePrevBodyState = m_eCurBodyState;
}

void CEri::Release()
{
	SafeDelete<CAnimation*>(m_pBodyAnim);
	SafeDelete<CAnimation*>(m_pLegAnim);
}

void CEri::OnCollision(CGameObject* _pCol, Vector2 _vColSize)
{
	// TODO 좌우 상항 충돌 enum 전달해서 해당 Rect 포지션에서 발생
}

void CEri::BehaviourKeyInput()
{

	if (CKeyManager::GetInstance().KeyPressing(VK_DOWN)
		&& !CKeyManager::GetInstance().KeyPressing(VK_UP))
	{
		m_eCurBodyState = SIT;
		m_pBodyAnim->ChangeAnimation(L"Eri_Blank_Body");
		m_pLegAnim->SetRepeat(true);
		return;
	}
	else if (CKeyManager::GetInstance().KeyUp(VK_DOWN)
		|| CKeyManager::GetInstance().KeyPressing(VK_UP))
	{
		m_eCurBodyState = STAND;
	}
		
	if (CKeyManager::GetInstance().KeyDown(m_cJumpKey))
	{
		if (m_bIsJump) return;
		// TODO : 점프 중일 때는 현재 프레임 받아와서 그시점부터 적용
		// TODO : DROP도 동일 && 단 DROP은 마지막 프레임이면 마지막으로 고정
		std::wcout << L"Press" << L"\n";
		m_bIsJump = true;

		m_eCurLegState = JUMP;
		m_pLegAnim->ChangeAnimation(L"Eri_Standing_Jump_Leg");
		m_pLegAnim->SetRepeat(false);
	}

	if (CKeyManager::GetInstance().KeyPressing(VK_RIGHT))
	{
		m_vFace = Vector2::UnitX;
		m_vDirection = Vector2::UnitX;

		if (m_bIsJump)
		{
			m_eCurLegState == MOVEJUMP;
			m_pLegAnim->ChangeAnimation(L"Eri_Standing_JumpFront_Leg");
			m_pLegAnim->SetRepeat(false);
			return;
		}

		m_eCurLegState = MOVE;
		m_pLegAnim->ChangeAnimation(L"Eri_Standing_Move_Leg");
		// TODO 테스트 코드 입니다.
		m_pLegAnim->SetDeltaFrame(m_pLegAnim->GetDeltaFrame() * 2.f);

		m_pLegAnim->SetRepeat(true);
	}
	else if (CKeyManager::GetInstance().KeyPressing(VK_LEFT))
	{
		m_vFace = Vector2::UnitX * -1.f;
		m_vDirection = Vector2::UnitX * -1.f;

		if (m_bIsJump)
		{
			m_eCurLegState == MOVEJUMP;
			m_pLegAnim->ChangeAnimation(L"Eri_Standing_JumpFront_Leg");
			m_pLegAnim->SetRepeat(false);
			return;
		}

		m_eCurLegState = MOVE;
		m_pLegAnim->ChangeAnimation(L"Eri_Standing_Move_Leg");
		m_pLegAnim->SetRepeat(true);
	}
	else
	{
		m_vDirection = Vector2::Zero;
		if (m_bIsJump) return;

		m_eCurLegState = IDLE;
		m_pLegAnim->ChangeAnimation(L"Eri_Standing_Idle_Leg");
		m_pLegAnim->SetRepeat(true);
	}
}

void CEri::AttackKeyInput()
{
	bool bHasEnemy = dynamic_cast<CCQCArea*>(m_pCQCCollider)->CatchEnemyCQCZone();

	if (m_eCurBodyState == SIT)
	{
		// TODO 근접 공격 여부 조건 추가
		m_pBodyAnim->SetRepeat(true);
		if (CKeyManager::GetInstance().KeyPressing(m_cAttackKey))
		{
			if (CKeyManager::GetInstance().KeyPressing(VK_RIGHT))
				m_vFace = Vector2::UnitX;
			else if (CKeyManager::GetInstance().KeyPressing(VK_LEFT))
				m_vFace = Vector2::UnitX * -1.f;

			m_vDirection = Vector2::Zero;

			if (bHasEnemy)
			{
				m_eCurLegState = CQC;
				m_pLegAnim->ChangeAnimation(L"Eri_Sit_CQC");
				m_pBodyAnim->SetRepeat(false);
				return;
			}

			m_eCurLegState = SHOOT;
			m_pLegAnim->ChangeAnimation(L"Eri_Sit_Shoot");
			m_pBodyAnim->SetRepeat(true);
		}
		else if (CKeyManager::GetInstance().KeyPressing(VK_RIGHT))
		{
			m_vFace = Vector2::UnitX;
			m_vDirection = Vector2::UnitX;
			
			m_eCurLegState = MOVE;
			m_pLegAnim->ChangeAnimation(L"Eri_Sit_Move");
			m_pBodyAnim->SetRepeat(true);
		}
		else if (CKeyManager::GetInstance().KeyPressing(VK_LEFT))
		{
			m_vFace = Vector2::UnitX * -1.f;
			m_vDirection = Vector2::UnitX * -1.f;
			
			m_eCurLegState = MOVE;
			m_pLegAnim->ChangeAnimation(L"Eri_Sit_Move");
			m_pBodyAnim->SetRepeat(true);
		}
		else
		{
			m_vDirection = Vector2::Zero;
			m_eCurLegState = IDLE;
			m_pLegAnim->ChangeAnimation(L"Eri_Sit_Idle");
			m_pBodyAnim->SetRepeat(true);
		}
	}
	else
	{
		
		// TODO 근접 공격 여부 조건 추가
		if (CKeyManager::GetInstance().KeyPressing(m_cAttackKey))
		{
			if (bHasEnemy)
			{
				int irand = rand() % 2;
				m_eCurBodyState = CQC;
				if (irand)
					m_pBodyAnim->ChangeAnimation(L"Eri_Standing_AxeCQC_Body");
				else
					m_pBodyAnim->ChangeAnimation(L"Eri_Standing_TonfaCQC_Body");
				m_pBodyAnim->SetRepeat(true);
				return;
			}

			m_eCurBodyState = SHOOT;
			m_pBodyAnim->SetRepeat(true);
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
		else if (m_eCurLegState == JUMP || m_eCurLegState == MOVEJUMP)
		{
			m_eCurBodyState = JUMP;
			m_pBodyAnim->ChangeAnimation(L"Eri_Standing_Jump_Body");
			m_pBodyAnim->SetRepeat(false);
		}
		else
		{
			m_eCurBodyState = IDLE;
			m_pBodyAnim->ChangeAnimation(L"Eri_Standing_Idle_Body");
			m_pBodyAnim->SetRepeat(true);
		}
	}
}

void CEri::Move()
{
	float iX = CScrollManager::GetInstance().GetScrollX();
	float iY = CScrollManager::GetInstance().GetScrollY();
	float fSpeed(0.f);
	if (m_eCurBodyState == SIT)
	{
		fSpeed = m_fCrawlSpeed * CTimeManager::GetInstance().GetDeltaTime() * m_vDirection.x;;
		WinOffset(fSpeed);
		
		m_vPivot.x += static_cast<int>(fSpeed);
	}	
	else if (m_eCurLegState == MOVE || m_eCurLegState == MOVEJUMP)
	{
		fSpeed = m_fMoveSpeed * CTimeManager::GetInstance().GetDeltaTime() * m_vDirection.x;
		WinOffset(fSpeed);
		m_vPivot.x += static_cast<int>(fSpeed);
	}
}

void CEri::Jump()
{
	if (m_eCurLegState == JUMP || m_eCurLegState == MOVEJUMP)
	{
		float fDT = CTimeManager::GetInstance().GetDeltaTime();
		m_fJumpDeltaTime += fDT;

		//void CAnimateObject::Gravity(float fFallSpeed, float fMaxFallSpeed)
		//{
		//	if (!m_bGravityOn)
		//		return;
		//
		//	m_fSpeedY += fFallSpeed * DELTA;
		//
		//	if (m_fSpeedY > fMaxFallSpeed)
		//		m_fSpeedY = fMaxFallSpeed;
		//
		//	AddPosY(m_fSpeedY * DELTA);
		//}

		float fJumpPower = ((100.f * m_fJumpDeltaTime) 
			- (530.f * m_fJumpDeltaTime * m_fJumpDeltaTime * 0.5f)) * 2.f;
		m_vPivot.y -= fJumpPower;

		std::wcout << m_vPivot.y << "\t\t" << fJumpPower << L"\n";
		//if (fJumpPower <= 0.f)
		//{
		//	m_fJumpDeltaTime = 0.f;
		//
		//	m_eCurLegState = DROP;
		//}
		
	}
	if (m_tRect.bottom >= WINCY)
	{
		m_bIsJump = false;
		m_fJumpDeltaTime = 0.f;
		m_vPivot.y = WINCY - (PLAYER_BMPY / 2) - 100.f;
	}

}

void CEri::Drop()
{

}

void CEri::Shoot()
{
	if (m_eCurBodyState != SHOOT) return;

	m_iScatterIdx *= -1;
	Vector2 vPos = m_vPivot + m_vFace * (m_vSize.x / 2.f);
	if (m_vFace.x >= Vector2::UnitX.x && m_vFace.y == 0.f)
	{
		CProjectileFactory<CHMProjectile>
			::CreateProjectile(vPos, Vector2(1.f, 0.001f * m_iScatterIdx), L"HeavyMachineGunProjectile_Front", 0);
	}
	else if (m_vFace.x >= Vector2::UnitX.x * -1.f && m_vFace.y == 0.f)
	{
		CProjectileFactory<CHMProjectile>
			::CreateProjectile(vPos, Vector2(-1.f, 0.001f * m_iScatterIdx), L"HeavyMachineGunProjectile_Up", 1);
	}
	else if (m_vFace.y <= Vector2::UnitY.x * -1.f && m_vFace.y == 0.f)
	{
		CProjectileFactory<CHMProjectile>
			::CreateProjectile(vPos, Vector2(0.001f * m_iScatterIdx, -1.f), L"HeavyMachineGunProjectile_Down", 0);
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

void CEri::LoadProjectileBmp()
{
	CBmpManager::GetInstance().InsertBmp(L"../Resource/Bmp/Projectile/HeavyMachineGun/HeavyMachineGunProjectile_Front.bmp"
		, L"HeavyMachineGunProjectile_Front");
	CBmpManager::GetInstance().InsertBmp(L"../Resource/Bmp/Projectile/HeavyMachineGun/HeavyMachineGunProjectile_Up.bmp"
		, L"HeavyMachineGunProjectile_Up");
	CBmpManager::GetInstance().InsertBmp(L"../Resource/Bmp/Projectile/HeavyMachineGun/HeavyMachineGunProjectile_Down.bmp"
		, L"HeavyMachineGunProjectile_Down");
}

void CEri::WinOffset(const float& _fCurSpeed)
{
	int iOffsetminX = 100;
	int iOffsetmaxX = static_cast<int>(WINCX * 0.5f);

	int iOffsetminY = 100;
	int iOffsetmaxY = 500;


	int iScrollX = (int)CScrollManager::GetInstance().GetScrollX();
	int iScrollY = (int)CScrollManager::GetInstance().GetScrollY();

	if (iOffsetminX > m_vPivot.x + iScrollX)
		CScrollManager::GetInstance().SetScrollX(_fCurSpeed);
	if (iOffsetmaxX < m_vPivot.x + iScrollX)
		CScrollManager::GetInstance().SetScrollX(-_fCurSpeed);

	if (iOffsetminX > m_vPivot.y + iScrollY)
		CScrollManager::GetInstance().SetScrollY(_fCurSpeed);

	if (iOffsetminX > m_vPivot.y + iScrollY)
		CScrollManager::GetInstance().SetScrollY(-_fCurSpeed);
}
