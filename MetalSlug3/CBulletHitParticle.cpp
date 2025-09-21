#include "pch.h"
#include "CBulletHitParticle.h"
#include "CAnimation.h"

CBulletHitParticle::CBulletHitParticle()
    : m_pAnim(nullptr), m_szFrameKey(nullptr)
{
    ZeroMemory(&m_pairFrame, sizeof(pair<int, int>));
}

CBulletHitParticle::~CBulletHitParticle()
{
    Release();
}

void CBulletHitParticle::Initialize()
{
    m_pAnim = new CAnimation();
    m_pAnim->Initialize();
    m_pAnim->SetParent(this);
    m_vSize = Vector2(48.f, 48.f);

    m_pairFrame = pair<int, int>{ 0, 10 };
    m_szFrameKey = L"BulletHit";
    m_pAnim->AddAnimation(m_szFrameKey, m_pairFrame);

    m_pAnim->ChangeAnimation(m_szFrameKey);
    m_pAnim->SetRepeat(false); 
    m_pAnim->SetFrameSpeed(0.025f);

    __super::UpdateGameObject();
}

int CBulletHitParticle::Update()
{
    if (m_bDestroy) return OBJ_DESTROY;

    __super::UpdateGameObject();

    return OBJ_NOEVENT;
}

void CBulletHitParticle::LateUpdate()
{
    m_pAnim->UpdateAnimation();
}

void CBulletHitParticle::Render(HDC _hDC)
{
    m_pAnim->RenderAnimation(_hDC);
    
    if (m_pAnim->GetCurrentFrameIndex() == m_pairFrame.second - 1)
        m_bDestroy = true;
}

void CBulletHitParticle::Release()
{
    SafeDelete<CAnimation*>(m_pAnim);
}

void CBulletHitParticle::OnCollision(CGameObject* _pCol, Vector2 _vColSize, COLLISION_COL_FLAG _eFlag)
{
}
