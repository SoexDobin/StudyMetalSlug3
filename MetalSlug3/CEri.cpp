#include "pch.h"
#include "CEri.h"
#include "CHitBox.h"
#include "CAnimation.h"
#include "CCQCChecker.h"
#include "CHMProjectile.h"
#include "CILProjectile.h"
#include "CFSProjectile.h"
#include "CPlatformChecker.h"
#include "CCQCArea.h"
#include "CExplodeArea.h"
#include "CEriSpawn.h"
#include "CScene.h"
#include "CBomb.h"

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
#include "CSoundManager.h"
#include "CSceneManager.h"

CEri::CEri()
	: m_pDeadAnim(nullptr), m_pBodyAnim(nullptr), m_pLegAnim(nullptr)
	, m_pSpawnAnim(nullptr), m_pSpawnAnimObj(nullptr)
	, m_pCQCCol(nullptr), m_pPlatformCol(nullptr)
	, m_eCurBodyState(PLAYER_STATE_END), m_ePrevBodyState(PLAYER_STATE_END)
	, m_eCurLegState(PLAYER_STATE_END), m_ePrevLegState(PLAYER_STATE_END)
	, m_fMoveSpeed(0.f), m_fCrawlSpeed(0.f)
	, m_fJumpSpeed(0.f), m_bIsJump(false)
	, m_fInvisibleDelta(0.f)
	, m_bIsDrop(true)
	, m_bMissionComplete(false)
	, m_iScatterIdx(0), m_fShootDelta(0.f), m_fBombDelta(0.f)
	, m_iArmo(INFINITE), m_iBomb(0), m_eCurWeapon(HM_GUN)
{
}

CEri::~CEri()
{
	Release();
}

void CEri::Initialize()
{
	LoadProjectileBmp();
	LoadEriBmp();

	m_vPivot = Vector2(200, 480);
	m_vSize = Vector2(PLAYER_BMPX, PLAYER_BMPY);

	m_vFace = Vector2::UnitX;
	m_vDirection = Vector2(0.f, 0.f);
	m_fMoveSpeed = 420.f;
	m_fCrawlSpeed = 200.f;
	m_fShootDelta = n_fShootDelta;
	m_fBombDelta = n_fBombDelta;
	m_eType = PLAYER;

	m_pColBox = CColliderFactory::Create(this, HITBOX);;
	m_pCQCCol = CGameObjectFactory<CCQCChecker>::Create(Vector2::Zero, Vector2::Zero, this);
	CObjectManager::GetInstance().AddGameObject(m_pCQCCol, NEUTRAL);

	m_pPlatformCol = CGameObjectFactory<CPlatformChecker>::Create(Vector2::Zero, Vector2::Zero, this);
	CObjectManager::GetInstance().AddGameObject(m_pPlatformCol, NEUTRAL);

	m_pSpawnAnimObj = new CEriSpawn();
	m_pSpawnAnimObj->Initialize();
	m_pSpawnAnim = dynamic_cast<CEriSpawn*>(m_pSpawnAnimObj)->GetSpawnAnimation();
	m_pSpawnAnimObj->SetPivot(m_vPivot);

	m_ePrevBodyState = RESPAWN;
	m_ePrevLegState = RESPAWN;
	m_eCurBodyState = RESPAWN;
	m_eCurLegState = RESPAWN;
	
	m_pBodyAnim->SetFrameSpeed(0.07f);
	m_pLegAnim->SetFrameSpeed(0.07f);

	m_pBodyAnim->ChangeAnimation(L"Eri_Blank_Body");
	m_pLegAnim->ChangeAnimation(L"Eri_Blank_Body");
	m_pSpawnAnim->ChangeAnimation(L"Eri_Spawn");

	SetStandCollider();
	srand(static_cast<int>(DELTA));
	m_fJumpSpeed = n_fSeedJumpSpeed;


	// Supply
	m_iBomb = 3;
}

int CEri::Update()
{

	if (m_bDestroy) return OBJ_DESTROY;

	if (CKeyManager::GetInstance().KeyPressing(VK_F4))
	{
		Dead();
	}
	
	__super::UpdateGameObject();

	if (m_fInvisibleDelta > 0.f)
		m_fInvisibleDelta -= DELTA;
	else if (m_fInvisibleDelta <= 0.f)
		m_fInvisibleDelta = 0.f;

	m_pSpawnAnimObj->SetPivot(Vector2(m_vPivot.x, m_vPivot.y - 240.f));
	if (m_eCurBodyState == RESPAWN)
	{
		m_pSpawnAnimObj->Update();
		if (m_pSpawnAnim->GetEndOneLoop())
		{
			m_pColBox->SetEnableCol(true);
			SetBodyAnim(L"Eri_Standing_Idle_Body", true, IDLE, 1);
			SetLegAnim(L"Eri_Standing_Idle_Leg", true, IDLE, 1);
			m_pSpawnAnim->ChangeAnimation(L"Eri_Spawn_Blank");

			m_eCurWeapon = HM_GUN;
			m_iArmo = INFINITE;
			m_iBomb = 0;
			return OBJ_NOEVENT;
		}
		return OBJ_NOEVENT;
	}

	if (m_eCurBodyState == DEAD)
	{
		if (m_pDeadAnim->GetEndOneLoop())
		{
			m_fInvisibleDelta = 3.f;
			m_eCurBodyState = RESPAWN;
			m_eCurLegState = RESPAWN;

			m_vPivot = CSceneManager::GetInstance().GetCurScene()->GetSpawnPoint();
			m_pSpawnAnimObj->SetPivot(m_vPivot);
			m_pDeadAnim->ChangeAnimation(L"Eri_Blank_Body");
			m_pSpawnAnim->ChangeAnimation(L"Eri_Spawn");
			m_pSpawnAnim->SetLoop(false);

			return OBJ_NOEVENT;
		}

		m_pDeadAnim->UpdateAnimation();
		return OBJ_NOEVENT;
	}

	CheckPlatform();

	if (m_bMissionComplete)
	{
		SetBodyAnim(L"Eri_Complete", true, PLAYER_STATE_END);
		SetLegAnim(L"Eri_Blank_Body", true, PLAYER_STATE_END);
		m_pDeadAnim->ChangeAnimation(L"Eri_Blank_Body");
		m_pBodyAnim->SetFrameSpeed(0.1f);
		m_pBodyAnim->UpdateAnimation();
		m_pLegAnim->UpdateAnimation();
		m_pDeadAnim->UpdateAnimation();
		return OBJ_NOEVENT;
	}

	m_pBodyAnim->UpdateAnimation();
	m_pLegAnim->UpdateAnimation();

	m_fBombDelta -= 1000.f * DELTA;
	m_fShootDelta -= 1000.f * DELTA;

	CheckOutOfBound();
	BehaviourKeyInput();
	AttackKeyInput();

	return OBJ_NOEVENT;
}

void CEri::LateUpdate()
{
	if (m_eCurBodyState == DEAD || m_eCurBodyState == RESPAWN)
	{
		return;
	}

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
	if (m_eCurBodyState == RESPAWN)
	{
		m_pSpawnAnimObj->Render(_hDC);
		return;
	}

	if (m_eCurBodyState == DEAD)
	{
		m_pDeadAnim->RenderAnimation(_hDC);
		return;
	}

	m_pLegAnim->RenderAnimation(_hDC);
	m_pBodyAnim->RenderAnimation(_hDC);

	m_ePrevLegState = m_eCurLegState;
	m_ePrevBodyState = m_eCurBodyState;
}

void CEri::Release()
{
	SafeDelete<CAnimation*>(m_pBodyAnim);
	SafeDelete<CAnimation*>(m_pLegAnim);
	SafeDelete<CAnimation*>(m_pDeadAnim);
	m_pSpawnAnim = nullptr;
	SafeDelete<CGameObject*>(m_pSpawnAnimObj);
}

void CEri::OnCollision(CGameObject* _pCol, Vector2 _vColSize, COLLISION_COL_FLAG _eFlag)
{
	if (m_fInvisibleDelta > 0.f) return;

	if (_pCol->GetObjectType() == PROJECTILE)
	{
		if (dynamic_cast<CProjectile*>(_pCol)->GetDamageFlag() == PLAYER)
		{
			Dead();
		}

	}
	else if (_pCol->GetObjectType() == EXPLODE)
	{
		if (dynamic_cast<CExplodeArea*>(_pCol)->GetDamageFlag() == PLAYER)
		{
			Dead();
		}
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
	bool bHasEnemy = dynamic_cast<CCQCChecker*>(m_pCQCCol)->CatchEnemyCQCZone();

	if (m_eCurBodyState == SIT) 
	{
		if (m_ePrevLegState == CQC && !m_pLegAnim->GetEndOneLoop()) 
			return;
		if (m_ePrevLegState == BOMB && !m_pLegAnim->GetEndOneLoop())
		{
			m_vDirection = Vector2::Zero;
			return;
		}
			
		m_pBodyAnim->SetLoop(true);


		if (CKeyManager::GetInstance().KeyDown(n_cBombKey))
		{
			if (m_fBombDelta > 0.f) return;
			
			Vector2 vPos = m_pColBox->GetPivot() + Vector2(m_vFace.x * 64.f, -64.f);

			if (m_vFace.x >= 0.f)
			{
				SetLegAnim(L"Eri_Sit_Throw", false, BOMB);
				if (m_iBomb == 0) return;
				CProjectileFactory<CBomb>
					::CreateProjectile(vPos, Vector2::UnitX, 0, 45.f);
				--m_iBomb;
			}
			else
			{
				SetLegAnim(L"Eri_Sit_Throw", false, BOMB);
				if (m_iBomb == 0) return;
				CProjectileFactory<CBomb>
					::CreateProjectile(vPos, Vector2::UnitX, 0, 135.f);
				--m_iBomb;
			}
			
			m_fBombDelta = n_fBombDelta;
			return;
		}
		else if (CKeyManager::GetInstance().KeyPressing(n_cAttackKey))
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
				if (m_pLegAnim->GetEndOneLoop() == false)
					return;
				
				SetLegAnim(L"Eri_Sit_CQC", false, CQC);
				CSoundManager::GetInstance().PlaySound(L"CQC_Tonfa.mp3", PLAYER_ATTACK, 0.2f);
				if (m_pLegAnim->GetEndOneLoop())
				{
					m_pLegAnim->SetEndOneLoop(false);
					
					CObjectManager::GetInstance().AddGameObject(
						CGameObjectFactory<CCQCArea>::Create(m_vPivot, Vector2(64.f, 64.f)), EXPLODE);
				}
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
		if (m_ePrevBodyState == CQC && !m_pBodyAnim->GetEndOneLoop())
			return;
		if (m_ePrevBodyState == BOMB && !m_pBodyAnim->GetEndOneLoop())
			return;

		if (CKeyManager::GetInstance().KeyDown(n_cBombKey))
		{
			if (m_fBombDelta > 0.f) return;

			Vector2 vPos = m_pColBox->GetPivot() + Vector2(0.f, -96.f);

			if (m_vFace.x >= 0.f)
			{
				SetBodyAnim(L"Eri_Standing_Throw_Body", false, BOMB);
				
				if (m_iBomb == 0) return;
				CProjectileFactory<CBomb>
					::CreateProjectile(vPos, Vector2::UnitX, 0, 45.f);
				--m_iBomb;
			}
			else
			{
				if (m_iBomb == 0) return;
				SetBodyAnim(L"Eri_Standing_Throw_Body", false, BOMB);
				CProjectileFactory<CBomb>
					::CreateProjectile(vPos, Vector2::UnitX, 0, 135.f);
				--m_iBomb;
			}
			
			m_fBombDelta = n_fBombDelta;
			return;
		}
		else if (CKeyManager::GetInstance().KeyPressing(n_cAttackKey))
		{
			if (bHasEnemy)
			{
				int irand = rand() % 2;

				if (irand) 
				{
					SetBodyAnim(L"Eri_Standing_AxeCQC_Body", false, CQC);
					CSoundManager::GetInstance().PlaySound(L"CQC_Axe.mp3", PLAYER_ATTACK, 0.2f);
					CObjectManager::GetInstance().AddGameObject(
						CGameObjectFactory<CCQCArea>::Create(m_vPivot, Vector2(64.f, 64.f)), EXPLODE);
					if (m_pBodyAnim->GetEndOneLoop())
					{
						m_pBodyAnim->SetEndOneLoop(false);
					}
				}
				else 
				{
					SetBodyAnim(L"Eri_Standing_TonfaCQC_Body", false, CQC);
					CSoundManager::GetInstance().PlaySound(L"CQC_Tonfa.mp3", PLAYER_ATTACK, 0.2f);
					CObjectManager::GetInstance().AddGameObject(
						CGameObjectFactory<CCQCArea>::Create(m_vPivot, Vector2(64.f, 64.f)), EXPLODE);
					if (m_pBodyAnim->GetEndOneLoop())
					{
						m_pBodyAnim->SetEndOneLoop(false);
					}
				}	
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

	ShootHeavyMachineGun(vPos);
	ShootIronLizzard(vPos);
	ShootFlameShot(vPos);
}

void CEri::ShootHeavyMachineGun(Vector2 _vPos)
{
	if (m_eCurWeapon != HM_GUN) return;

	if (m_vFace.x >= Vector2::UnitX.x && m_vFace.y == 0.f)
	{
		CProjectileFactory<CHMProjectile>
			::CreateProjectile(_vPos, Vector2(m_vFace.x, n_fScatterArg[m_iScatterIdx]), L"HeavyMachineGunProjectile_Front", 0);
	}
	else if (m_vFace.x <= Vector2::UnitX.x && m_vFace.y == 0.f)
	{
		CProjectileFactory<CHMProjectile>
			::CreateProjectile(_vPos, Vector2(m_vFace.x, n_fScatterArg[m_iScatterIdx]), L"HeavyMachineGunProjectile_Front", 1);
	}
	else if (m_vFace.y <= Vector2::UnitY.y * -1.f && m_vFace.x == 0.f)
	{
		CProjectileFactory<CHMProjectile>
			::CreateProjectile(_vPos, Vector2(n_fScatterArg[m_iScatterIdx], -1.f), L"HeavyMachineGunProjectile_Up", 0);
	}
	else if (m_vFace.y >= Vector2::UnitY.y && m_vFace.x == 0.f)
	{
		CProjectileFactory<CHMProjectile>
			::CreateProjectile(_vPos, Vector2(n_fScatterArg[m_iScatterIdx], 1.f), L"HeavyMachineGunProjectile_Down", 0);
	}

	if (m_eCurWeapon == HM_GUN && m_iScatterIdx % 2 == 1)
	{
		CSoundManager::GetInstance().PlaySound(L"HM_Shoot.mp3", PLAYER_ATTACK, 0.1f);
	}

	m_iScatterIdx = (m_iScatterIdx + 1) % 5;
	m_fShootDelta = n_fShootDelta;
}

void CEri::ShootIronLizzard(Vector2 _vPos)
{
	if (m_eCurWeapon != IRON_LIZZARD) return;

	if (m_vFace.x >= Vector2::UnitX.x && m_vFace.y == 0.f)
	{
		CProjectileFactory<CILProjectile>
			::CreateProjectile(_vPos, Vector2(m_vFace.x, 0.f)
				, L"Iron_Lizzard", 0);
	}
	else if (m_vFace.x <= Vector2::UnitX.x && m_vFace.y == 0.f)
	{
		CProjectileFactory<CILProjectile>
			::CreateProjectile(_vPos, Vector2(m_vFace.x, 0.f)
				, L"Iron_Lizzard", 1);
	}

	--m_iArmo;
	m_fShootDelta = 225.f;

	if (m_iArmo == 0)
	{
		m_eCurWeapon = HM_GUN;
		m_iArmo = INFINITE;

		m_fShootDelta = n_fShootDelta;
		return;
	}
}

void CEri::ShootFlameShot(Vector2 _vPos)
{
	if (m_eCurWeapon != FLAME_SHOT) return;

	if (m_vFace.x >= Vector2::UnitX.x && m_vFace.y == 0.f)
	{
		CProjectileFactory<CFSProjectile>
			::CreateProjectile(_vPos, Vector2(m_vFace.x, 0.f)
				, L"FlameShot_Front", 0);
	}
	else if (m_vFace.x <= Vector2::UnitX.x && m_vFace.y == 0.f)
	{
		CProjectileFactory<CFSProjectile>
			::CreateProjectile(_vPos, Vector2(m_vFace.x, 0.f)
				, L"FlameShot_Front", 1);
	}
	else if (m_vFace.y <= Vector2::UnitY.y * -1.f && m_vFace.x == 0.f)
	{
		CProjectileFactory<CFSProjectile>
			::CreateProjectile(_vPos, Vector2(0.f, -1.f)
				, L"FlameShot_Up", 0);
	}
	else if (m_vFace.y >= Vector2::UnitY.y && m_vFace.x == 0.f)
	{
		CProjectileFactory<CFSProjectile>
			::CreateProjectile(_vPos, Vector2(0.f, 1.f)
				, L"FlameShot_Down", 0);
	}

	--m_iArmo;
	m_fShootDelta = 350.f;

	if (m_iArmo == 0)
	{
		m_eCurWeapon = HM_GUN;
		m_iArmo = INFINITE;

		m_fShootDelta = n_fShootDelta;
		return;
	}
}


void CEri::Dead()
{
	SetBodyAnim(L"Eri_Blank_Body", false, DEAD);
	SetLegAnim(L"Eri_Blank_Body", false, DEAD);
	m_pSpawnAnim->ChangeAnimation(L"Eri_Spawn_Blank");

	m_pColBox->SetEnableCol(false);
	m_pDeadAnim->ChangeAnimation(L"Eri_Dead");
	m_pDeadAnim->SetLoop(false);
	CSoundManager::GetInstance().PlaySoundOnce(L"Eri_Dead.mp3", PLAYER_DEAD, 0.1f);
}

void CEri::CheckOutOfBound()
{
	float fMinX = CScrollManager::GetInstance().GetMinScrollLock().x;
	float fMaxX = CScrollManager::GetInstance().GetMaxScrollLock().x;
	if (m_vPivot.x < fMinX) m_vPivot.x += (fMinX - m_vPivot.x);
	if (m_vPivot.x > fMaxX) m_vPivot.x += fMaxX - (m_vPivot.x);

	//if (m_eCurBodyState = DEAD) return;
	//if (m_tRect.top > -CScrollManager::GetInstance().GetMinScrollLock().y + 960.f)
	//	Dead(); // ³«»ç
}

void CEri::CheckPlatform()
{
	bool bColPlatform = dynamic_cast<CPlatformChecker*>(m_pPlatformCol)->GetHasColWithPlatform();

	float fLineChecker = 0.f;
	Vector2 vPos = Vector2(static_cast<int>(m_pPlatformCol->GetRect().right), static_cast<int>(m_pPlatformCol->GetRect().top));
	if (CLineManager::GetInstance().CollisionLine(vPos, &fLineChecker))
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
	m_pDeadAnim = new CAnimation();


	CBmpManager::GetInstance().InsertBmp(L"../Resource/Bmp/Eri/Eri_Blank_Body.bmp"
		, L"Eri_Blank_Body");
	m_pBodyAnim->AddAnimation(L"Eri_Blank_Body", pair<int, int>{0, 1});
	m_pLegAnim->AddAnimation(L"Eri_Blank_Body", pair<int, int>{0, 1});
	m_pDeadAnim->AddAnimation(L"Eri_Blank_Body", pair<int, int>{0, 1});
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

	CBmpManager::GetInstance().InsertBmp(L"../Resource/Bmp/Eri/Eri_Standing_Throw_Body.bmp"
		, L"Eri_Standing_Throw_Body");
	m_pBodyAnim->AddAnimation(L"Eri_Standing_Throw_Body", pair<int, int>(0, 6));
	CBmpManager::GetInstance().InsertBmp(L"../Resource/Bmp/Eri/Eri_Sit_Throw.bmp"
		, L"Eri_Sit_Throw");
	m_pLegAnim->AddAnimation(L"Eri_Sit_Throw", pair<int, int>(0, 6));

	CBmpManager::GetInstance().InsertBmp(L"../Resource/Bmp/Eri/Eri_Complete.bmp"
		, L"Eri_Complete");
	m_pBodyAnim->AddAnimation(L"Eri_Complete", pair<int, int>{0, 7});

	CBmpManager::GetInstance().InsertBmp(L"../Resource/Bmp/Eri/Eri_Dead.bmp",
		L"Eri_Dead");
	m_pDeadAnim->AddAnimation(L"Eri_Dead", pair<int, int>{0, 20});

	m_pBodyAnim->Initialize();
	m_pLegAnim->Initialize();
	m_pDeadAnim->Initialize();
	m_pBodyAnim->SetParent(this);
	m_pLegAnim->SetParent(this);
	m_pDeadAnim->SetParent(this);
}

void CEri::LoadProjectileBmp()
{
	CBmpManager::GetInstance().InsertBmp(L"../Resource/Bmp/Projectile/Bomb/Bomb.bmp", L"Bomb");
	CBmpManager::GetInstance().InsertBmp(L"../Resource/Bmp/Item/Grenade.bmp", L"Grenade");
	CBmpManager::GetInstance().InsertBmp(L"../Resource/Bmp/Item/Iron_Lizzard.bmp", L"Iron_Lizzard");
	CBmpManager::GetInstance().InsertBmp(L"../Resource/Bmp/Item/Flame_Shot.bmp", L"Flame_Shot");
	
	CBmpManager::GetInstance().InsertBmp(L"../Resource/Bmp/Projectile/HeavyMachineGun/HeavyMachineGunProjectile_Front.bmp"
		, L"HeavyMachineGunProjectile_Front");
	CBmpManager::GetInstance().InsertBmp(L"../Resource/Bmp/Projectile/HeavyMachineGun/HeavyMachineGunProjectile_Up.bmp"
		, L"HeavyMachineGunProjectile_Up");
	CBmpManager::GetInstance().InsertBmp(L"../Resource/Bmp/Projectile/HeavyMachineGun/HeavyMachineGunProjectile_Down.bmp"
		, L"HeavyMachineGunProjectile_Down");

	CBmpManager::GetInstance().InsertBmp(L"../Resource/Bmp/Projectile/IronLizzard/Iron_Lizzard_Shoot.bmp"
		, L"Iron_Lizzard_Shoot");
	CBmpManager::GetInstance().InsertBmp(L"../Resource/Bmp/Projectile/IronLizzard/Iron_Lizzard_Attack.bmp"
		, L"Iron_Lizzard_Attack");

	CBmpManager::GetInstance().InsertBmp(L"../Resource/Bmp/Projectile/FlameShot/FlameShot_Front.bmp"
		, L"FlameShot_Front");
}

void CEri::WinOffsetX(const float& _fCurSpeed)
{
	int iOffsetminX = 0;
	int iOffsetmaxX = static_cast<int>(WINCX * 0.5f) - 96;

	int iScrollX = SCROLLX;

	if ((WINCX / 2.f) < m_vPivot.x)
		CScrollManager::GetInstance().SetMinScrollLockX(-SCROLLX + _fCurSpeed);

	
	if (_fCurSpeed < 0.f) return;

	if (iOffsetmaxX < m_vPivot.x + iScrollX)
		CScrollManager::GetInstance().SetScrollX(-_fCurSpeed);
}

void CEri::WinOffsetY(const float& _fCurSpeed)
{
	int iOffsetminY = static_cast<int>(WINCY * 0.5f);
	int iOffsetmaxY = WINCY;

	if ((WINCY / 2.f) < m_vPivot.y)
		CScrollManager::GetInstance().SetMinScrollLockX(-SCROLLX + _fCurSpeed * 0.5f);

	if (_fCurSpeed > 0.f) return;

	if (CScrollManager::GetInstance().GetMinScrollLock().y <= SCROLLY) return;
	if (iOffsetminY > m_vPivot.y + SCROLLY)
		CScrollManager::GetInstance().SetScrollY(-_fCurSpeed * 0.5f);
	//if (iOffsetmaxY > m_vPivot.y + iScrollY)
	//	CScrollManager::GetInstance().SetScrollY(-_fCurSpeed);
}
