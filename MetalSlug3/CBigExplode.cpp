#include "pch.h"
#include "CBigExplode.h"
#include "CAnimation.h"
#include "CSoundManager.h"

CBigExplode::CBigExplode()
    : m_pAnim(nullptr)
{
}

CBigExplode::~CBigExplode()
{
    Release();
}

void CBigExplode::Initialize()
{
    m_pAnim = new CAnimation();
    m_pAnim->Initialize();
    m_pAnim->SetParent(this);
    m_vSize = Vector2(510.f, 510.f);

    m_szFrameKey = L"Big_Explode";
    m_pAnim->AddAnimation(m_szFrameKey, pair<int, int>{ 0, 26 });

    m_pAnim->ChangeAnimation(m_szFrameKey);
    m_pAnim->SetLoop(false);
    m_pAnim->SetFrameSpeed(0.025f);
    m_pAnim->SetEaseColor(RGB(255, 0, 255));

    CSoundManager::GetInstance().PlaySoundOnce(L"Big_Explode_Long.mp3", EXPLODE_EFFECT, 0.1f);

    __super::UpdateGameObject();
}

int CBigExplode::Update()
{
    if (m_bDestroy) return OBJ_DESTROY;

    __super::UpdateGameObject();

    return OBJ_NOEVENT;
}

void CBigExplode::LateUpdate()
{
    m_pAnim->UpdateAnimation();
}

void CBigExplode::Render(HDC _hDC)
{
    if (m_vPivot == Vector2::Zero) return;

    m_pAnim->RenderAnimation(_hDC);

    if (m_pAnim->GetEndOneLoop())
        m_bDestroy = true;
}

void CBigExplode::Release()
{
    SafeDelete<CAnimation*>(m_pAnim);
}

void CBigExplode::OnCollision(CGameObject* _pCol, Vector2 _vColSize, COLLISION_COL_FLAG _eFlag)
{
}
