#include "pch.h"
#include "CEri.h"
#include "CHitBox.h"
#include "CAnimation.h"
#include "CCQCArea.h"
#include "CHMProjectile.h"
#include "CPlatformChecker.h"

// Managers
#include "CBmpManager.h"
#include "CKeyManager.h"
#include "CTimeManager.h"
#include "CScrollManager.h"
#include "CColliderFactory.h"
#include "CGameObjectFactory.h"
#include "CObjectManager.h"
#include "CProjectileFactory.h"
#include "CLineManager.h"

CEri::CEri()
	: m_pBodyAnim(nullptr), m_pLegAnim(nullptr), m_pCQCCol(nullptr), m_pPlatformCol(nullptr)
	, m_eCurBodyState(PLAYER_STATE_END), m_ePrevBodyState(PLAYER_STATE_END)
	, m_eCurLegState(PLAYER_STATE_END), m_ePrevLegState(PLAYER_STATE_END)
	, m_fMoveSpeed(0.f), m_fCrawlSpeed(0.f)
	, m_fJumpSpeed(0.f), m_bIsJump(false)
	, m_bIsDrop(true)
	, m_iScatterIdx(0), m_fShootDelta(0.f)
{
}

CEri::~CEri()
{
	Release();
}

void CEri::Initialize()
{
	m_vPivot = Vector2(200, 480);
	m_vSize = Vector2(PLAYER_BMPX, PLAYER_BMPY);
	m_vFace = Vector2::UnitX;
	m_vDirection = Vector2(0.f, 0.f);
	m_fMoveSpeed = 500.f;
	m_fCrawlSpeed = 100.f;
	m_fShootDelta = n_fShootDelta;
	m_eType = PLAYER;

	m_pColBox = CColliderFactory::Create(this, HITBOX, nullptr, nullptr);;

	m_pCQCCol = CGameObjectFactory<CCQCArea>::Create(Vector2::Zero, Vector2::Zero, this);
	CObjectManager::GetInstance().AddGameObject(m_pCQCCol, PLAYER);

	m_pPlatformCol = CGameObjectFactory<CPlatformChecker>::Create(Vector2::Zero, Vector2::Zero, this);
	CObjectManager::GetInstance().AddGameObject(m_pPlatformCol, NEUTRAL);

	SetStandCollider();

	LoadProjectileBmp();
	LoadEriBmp();
	m_ePrevBodyState = IDLE;
	m_ePrevLegState = IDLE;
	m_pBodyAnim->ChangeAnimation(L"Eri_Standing_Idle_Body");
	m_pBodyAnim->SetFrameSpeed(0.07f);
	m_pLegAnim->ChangeAnimation(L"Eri_Standing_Idle_Leg");
	m_pLegAnim->SetFrameSpeed(0.07f);

	srand(static_cast<int>(DELTA));
	m_fJumpSpeed = n_fSeedJumpSpeed;
}

int CEri::Update()
{
	if (m_bDestroy) return OBJ_DESTROY;

	m_pBodyAnim->UpdateAnimation();
	m_pLegAnim->UpdateAnimation();
	__super::UpdateGameObject();

	m_fShootDelta -= 1000.f * DELTA;

	CheckPlatform();
	CheckOutOfBound();
	BehaviourKeyInput();
	AttackKeyInput();

	return OBJ_NOEVENT;
}

void CEri::LateUpdate()
{
	if (m_eCurBodyState == SIT)
		SetSitCollider();
	else
		SetStandCollider();

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

void CEri::OnCollision(CGameObject* _pCol, Vector2 _vColSize, COLLISION_COL_FLAG _eFlag)
{
	switch (_pCol->GetObjectType())
	{
	case ENEMY:
		break;
	case PLATFORM:
		break;
	default:
		break;
	}
}

void CEri::BehaviourKeyInput()
{
	if (CKeyManager::GetInstance().KeyPressing(VK_DOWN)
		&& !CKeyManager::GetInstance().KeyPressing(VK_UP))
	{
		if (m_bIsJump || m_bIsDrop) return;

		m_eCurBodyState = SIT;
		m_pBodyAnim->ChangeAnimation(L"Eri_Blank_Body");
		m_pLegAnim->SetLoop(true);
		return;
	}
	else if (CKeyManager::GetInstance().KeyUp(VK_DOWN)
		|| CKeyManager::GetInstance().KeyPressing(VK_UP))
	{
		m_eCurBodyState = STAND;
	}
		
	if (m_bIsDrop)
	{
		SetLegAnim(L"Eri_Standing_Drop_Leg", false, DROP);

		if (CKeyManager::GetInstance().KeyPressing(VK_RIGHT))
			m_vDirection = Vector2(1.f, 1.f);
		else if (CKeyManager::GetInstance().KeyPressing(VK_LEFT))
			m_vDirection = Vector2(-1.f, 1.f);

		return;
	}
	if (CKeyManager::GetInstance().KeyPressing(n_cJumpKey))
		m_bIsJump = true;

	if (m_bIsJump) 
	{
		if (CKeyManager::GetInstance().KeyPressing(VK_RIGHT))
		{
			m_vDirection = Vector2(1.f, -1.f);
			SetLegAnim(L"Eri_Standing_JumpFront_Leg", false, MOVEJUMP);
		}
		else if (CKeyManager::GetInstance().KeyPressing(VK_LEFT))
		{
			m_vDirection = Vector2(-1.f, -1.f);
			SetLegAnim(L"Eri_Standing_JumpFront_Leg", false, MOVEJUMP);
		}
		else
		{
			m_vDirection = Vector2::Zero;
			SetLegAnim(L"Eri_Standing_Jump_Leg", false, JUMP);
		}
		return;
	}

	if (CKeyManager::GetInstance().KeyPressing(VK_RIGHT))
	{
		m_vFace = Vector2::UnitX;
		m_vDirection = Vector2::UnitX;

		SetLegAnim(L"Eri_Standing_Move_Leg", true, MOVE);
		m_pLegAnim->SetDeltaFrame(m_pLegAnim->GetDeltaFrame() * 2.f);
		m_pLegAnim->SetLastFacingX(1);
		m_pBodyAnim->SetLastFacingX(1);
	}
	else if (CKeyManager::GetInstance().KeyPressing(VK_LEFT))
	{
		m_vFace = Vector2::UnitX * -1.f;
		m_vDirection = Vector2::UnitX * -1.f;

		SetLegAnim(L"Eri_Standing_Move_Leg", true, MOVE);
		m_pLegAnim->SetDeltaFrame(m_pLegAnim->GetDeltaFrame() * 2.f);
		m_pLegAnim->SetLastFacingX(-1);
		m_pBodyAnim->SetLastFacingX(-1);
	}
	else
	{
		if (m_bIsJump) return;

		m_vDirection = Vector2::Zero;
		SetLegAnim(L"Eri_Standing_Idle_Leg", true, IDLE);
	}
}

void CEri::AttackKeyInput()
{
	bool bHasEnemy = dynamic_cast<CCQCArea*>(m_pCQCCol)->CatchEnemyCQCZone();

	if (m_eCurBodyState == SIT)
	{
		m_pBodyAnim->SetLoop(true);
		if (CKeyManager::GetInstance().KeyPressing(n_cAttackKey))
		{
			if (CKeyManager::GetInstance().KeyPressing(VK_RIGHT))
				m_vFace = Vector2::UnitX;
			else if (CKeyManager::GetInstance().KeyPressing(VK_LEFT))
				m_vFace = Vector2::UnitX * -1.f;
			else
				m_vFace = Vector2((float)m_pBodyAnim->GetLastFacingX(), 0.f);

			m_vDirection = Vector2::Zero;

			if (bHasEnemy)
			{
				if (m_pBodyAnim->GetCurrentFrameIndex() < m_pBodyAnim->GetEndFrameIndex() - 1)
					return;

				SetLegAnim(L"Eri_Sit_CQC", true, CQC);
				return;
			}

			SetLegAnim(L"Eri_Sit_Shoot", true, SHOOT);
		}
		else if (CKeyManager::GetInstance().KeyPressing(VK_RIGHT))
		{
			m_vFace = Vector2::UnitX;
			m_vDirection = Vector2::UnitX;
			SetLegAnim(L"Eri_Sit_Move", true, MOVE);
		}
		else if (CKeyManager::GetInstance().KeyPressing(VK_LEFT))
		{
			m_vFace = Vector2::UnitX * -1.f;
			m_vDirection = Vector2::UnitX * -1.f;
			SetLegAnim(L"Eri_Sit_Move", true, MOVE);
		}
		else
		{
			m_vDirection = Vector2::Zero;
			SetLegAnim(L"Eri_Sit_Idle", true, IDLE);
		}
	}
	else
	{
		if (CKeyManager::GetInstance().KeyPressing(n_cAttackKey))
		{
			if (bHasEnemy)
			{
				int irand = rand() % 2;
				if (m_pBodyAnim->GetCurrentFrameIndex() < m_pBodyAnim->GetEndFrameIndex() - 1)
					return;

				if (irand)		SetBodyAnim(L"Eri_Standing_AxeCQC_Body", true, CQC);
				else			SetBodyAnim(L"Eri_Standing_TonfaCQC_Body", true, CQC);
				return;
			}

			if (CKeyManager::GetInstance().KeyPressing(VK_UP))
			{
				m_vFace = Vector2::UnitY * -1.f;
				SetBodyAnim(L"Eri_Standing_ShootUp_Body", true, SHOOT);
				return;
			}
			else if (CKeyManager::GetInstance().KeyPressing(VK_DOWN))
			{
				m_vFace = Vector2::UnitY;
				SetBodyAnim(L"Eri_Standing_ShootDown_Body", true, SHOOT);
				return;
			}
			else if (CKeyManager::GetInstance().KeyPressing(VK_RIGHT))
				m_vFace = Vector2::UnitX;
			else if (CKeyManager::GetInstance().KeyPressing(VK_LEFT))
				m_vFace = Vector2::UnitX * -1.f;
			else
				m_vFace = Vector2(m_pBodyAnim->GetLastFacingX(), 0);
			
			SetBodyAnim(L"Eri_Standing_ShootFront_Body", true, SHOOT);
		}
		else if (m_eCurLegState == JUMP || m_eCurLegState == MOVEJUMP)
		{
			SetBodyAnim(L"Eri_Standing_Jump_Body", false, JUMP);
		}
		else if (m_eCurLegState == DROP)
		{
			SetBodyAnim(L"Eri_Standing_Drop_Body", false, DROP);
		}
		else
		{
			SetBodyAnim(L"Eri_Standing_Idle_Body", true, IDLE);
		}
	}
}

void CEri::Move()
{
	float fSpeed(0.f);
	if (m_eCurBodyState == SIT)
	{
		fSpeed = m_fCrawlSpeed * DELTA * m_vDirection.x;;
		WinOffsetX(fSpeed);
		
		m_vPivot.x += static_cast<int>(fSpeed);
	}	
	else if (m_eCurLegState == MOVE || m_eCurLegState == MOVEJUMP || m_eCurLegState == DROP)
	{
		fSpeed = m_fMoveSpeed * DELTA * m_vDirection.x;
		WinOffsetX(fSpeed);
		m_vPivot.x += static_cast<int>(fSpeed);
	}
}

void CEri::Jump()
{
	if (m_bIsJump)
	{
		m_fJumpSpeed += DELTA * n_fFallSpeed;
		m_vPivot.y += m_fJumpSpeed * DELTA;
		
		// TODO : Y스크롤 액션을 처리해야 한다.
		if (m_fJumpSpeed > 0.f)
		{
			m_bIsDrop = true;
			m_bIsJump = false;
		}
		else
			WinOffsetY(m_fJumpSpeed * DELTA);
	}
}

void CEri::Drop()
{
	// TODO : 그냥 드롭일 때 m_fJumpSpeed 에 -1곱해야 함

	if (m_bIsDrop)
	{
		m_fJumpSpeed += DELTA * n_fFallSpeed;

		if (m_fJumpSpeed > n_fMaxFall)
			m_fJumpSpeed = n_fMaxFall;

		m_vPivot.y += m_fJumpSpeed * DELTA;
	}	
}

void CEri::Shoot()
{
	if (m_fShootDelta > 0.f) return;

	bool bIsShoot = false;
	if (m_eCurBodyState == SIT && m_eCurLegState == SHOOT)
		bIsShoot = true;
	else if (m_eCurBodyState == SHOOT) 
		bIsShoot = true;

	if (bIsShoot == false) return;
	
	Vector2 vPos = m_vPivot + m_vFace * (m_vSize.x / 2.f);

	if (m_eCurBodyState == SIT)
		vPos += n_vShootSitOffset;
	else
		vPos += n_vShootOffset;

	if (m_vFace.x >= Vector2::UnitX.x && m_vFace.y == 0.f)
	{
		CProjectileFactory<CHMProjectile>
			::CreateProjectile(vPos, Vector2(m_vFace.x, n_fScatterArg[m_iScatterIdx]), L"HeavyMachineGunProjectile_Front", 0);
	}
	else if (m_vFace.x <= Vector2::UnitX.x && m_vFace.y == 0.f)
	{
		CProjectileFactory<CHMProjectile>
			::CreateProjectile(vPos, Vector2(m_vFace.x, n_fScatterArg[m_iScatterIdx]), L"HeavyMachineGunProjectile_Front", 1);
	}
	else if (m_vFace.y <= Vector2::UnitY.y * -1.f && m_vFace.x == 0.f)
	{
		CProjectileFactory<CHMProjectile>
			::CreateProjectile(vPos, Vector2(n_fScatterArg[m_iScatterIdx], -1.f), L"HeavyMachineGunProjectile_Up", 0);
	}
	else if (m_vFace.y >= Vector2::UnitY.y && m_vFace.x == 0.f)
	{
		CProjectileFactory<CHMProjectile>
			::CreateProjectile(vPos, Vector2(n_fScatterArg[m_iScatterIdx], 1.f), L"HeavyMachineGunProjectile_Down", 0);
	}

	
	m_iScatterIdx = (m_iScatterIdx + 1) % 5;
	m_fShootDelta = n_fShootDelta;
}

void CEri::CheckOutOfBound()
{
	float fMinX = CScrollManager::GetInstance().GetMinScrollLock().x;
	float fMaxX = CScrollManager::GetInstance().GetMaxScrollLock().x;
	if (m_vPivot.x < fMinX) m_vPivot.x += fMinX - (m_vPivot.x);
	if (m_vPivot.x > fMaxX) m_vPivot.x += fMaxX - (m_vPivot.x);

	if (m_vPivot.y > WINCY)
		; // TODO : 낙사 판정
}

void CEri::CheckPlatform()
{
	bool bColPlatform = dynamic_cast<CPlatformChecker*>(m_pPlatformCol)->GetHasColWithPlatform();
	//if (bColPlatform)
	//{ 
	//	float fPosY = dynamic_cast<CPlatformChecker*>(m_pPlatformCol)->GetColTopPosition();
	//	m_vPivot.y = fPosY - (m_pColBox->GetSize().y / 2.f) - m_pColBox->GetOffset().y;
	//	//m_vPivot.y -= fabsf(fPosY - (m_pColBox->GetPivot().y + m_pColBox->GetSize().y / 2.f));
	//}	
	float fLineChecker = 0.f;
	if (CLineManager::GetInstance().CollisionLine(m_pPlatformCol->GetRect().right, &fLineChecker))
	{
		if (m_bIsJump) return;

		if (m_bIsDrop && m_vPivot.y + (m_vSize.y / 2.f) >= fLineChecker)
		{
			m_bIsDrop = false;
			m_fJumpSpeed = n_fSeedJumpSpeed;
			m_vPivot.y = fLineChecker - (m_pColBox->GetSize().y / 2.f) - m_pColBox->GetOffset().y;
		}
		else if (!m_bIsDrop && m_eCurLegState == MOVE)
			m_vPivot.y = fLineChecker - (m_pColBox->GetSize().y / 2.f) - m_pColBox->GetOffset().y;

		return;
	}
	if (!bColPlatform && !m_bIsJump && !m_bIsDrop)
	{
		m_bIsDrop = true;
		m_fJumpSpeed = -n_fSeedJumpSpeed / 10.f;
	}
	if (m_bIsJump || !m_bIsDrop) return;

	if (m_bIsDrop && bColPlatform)
	{
		float fPosY = dynamic_cast<CPlatformChecker*>(m_pPlatformCol)->GetColTopPosition();
		m_bIsDrop = false;
		m_vPivot.y = fPosY - (m_pColBox->GetSize().y / 2.f) - m_pColBox->GetOffset().y;
		m_eCurLegState = STAND;
		m_fJumpSpeed = n_fSeedJumpSpeed;
		return;
	}
	else if (bColPlatform == false)
	{	
		m_bIsDrop = true;
		m_eCurBodyState = DROP;
		m_eCurLegState = DROP;
	}
}

void CEri::IdentifyPlatform(CGameObject* _pCol, Vector2 _vColSize, COLLISION_COL_FLAG _eFlag)
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

inline void CEri::SetStandCollider()
{
	m_pColBox->SetSize(m_vSize / 3.f);
	m_pColBox->SetOffset(n_vPlayerColOffset);
	m_pPlatformCol->GetCollider()->SetOffset({0.f, 134.f });
}

inline void CEri::SetSitCollider()
{
	m_pColBox->SetSize({ m_vSize.x / 3.f, m_vSize.y / 4.f });
	m_pColBox->SetOffset(n_vPlayerColSitOffset);
}

inline void CEri::SetLegAnim(const TCHAR* _szKey, bool _bIsRepeat, PLAYER_STATE _eLegState, int _iLastFacingX)
{
	m_pLegAnim->ChangeAnimation(_szKey);
	m_pLegAnim->SetLoop(_bIsRepeat);
	m_eCurLegState = _eLegState;

	if (_iLastFacingX == 0) return;
	m_pLegAnim->SetLastFacingX(_iLastFacingX);
}

inline void CEri::SetBodyAnim(const TCHAR* _szKey, bool _bIsRepeat, PLAYER_STATE _eBodyState, int _iLastFacingX)
{
	m_pBodyAnim->ChangeAnimation(_szKey);
	m_pBodyAnim->SetLoop(_bIsRepeat);
	m_eCurBodyState = _eBodyState;

	if (_iLastFacingX == 0) return;
	m_pBodyAnim->SetLastFacingX(_iLastFacingX);
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

void CEri::WinOffsetX(const float& _fCurSpeed)
{
	int iOffsetminX = 0;
	int iOffsetmaxX = static_cast<int>(WINCX * 0.5f);

	int iScrollX = SCROLLX;

	if (iOffsetminX > m_vPivot.x + iScrollX)
		CScrollManager::GetInstance().SetScrollX(_fCurSpeed);
	if (iOffsetmaxX < m_vPivot.x + iScrollX)
		CScrollManager::GetInstance().SetScrollX(-_fCurSpeed);
}

void CEri::WinOffsetY(const float& _fCurSpeed)
{
	int iOffsetminY = static_cast<int>(WINCY * 0.5f);
	int iOffsetmaxY = static_cast<int>(WINCY * 0.5f);

	int iScrollX = SCROLLX;
	int iScrollY = SCROLLY;

	if (iOffsetminY < m_vPivot.y + iScrollY)
		CScrollManager::GetInstance().SetScrollY(_fCurSpeed);
	if (iOffsetmaxY > m_vPivot.y + iScrollY)
		CScrollManager::GetInstance().SetScrollY(-_fCurSpeed);
}
