#include "pch.h"
#include "CMediumExplode.h"
#include "CAnimation.h"
#include "CSoundManager.h"

CMediumExplode::CMediumExplode()
    : m_pAnim(nullptr)
{
}

CMediumExplode::~CMediumExplode()
{
    Release();
}

void CMediumExplode::Initialize()
{
    m_pAnim = new CAnimation();
    m_pAnim->Initialize();
    m_pAnim->SetParent(this);
    m_vSize = Vector2(150.f, 150.f);

    m_szFrameKey = L"Medium_Explode";
    m_pAnim->AddAnimation(m_szFrameKey, pair<int, int>{ 0, 28 });

    m_pAnim->ChangeAnimation(m_szFrameKey);
    m_pAnim->SetLoop(false);
    m_pAnim->SetFrameSpeed(0.025f);
    m_pAnim->SetEaseColor(RGB(255, 0, 255));

    CSoundManager::GetInstance().PlaySound(L"Medium_Explode.mp3", EXPLODE_EFFECT, 0.2f);

    __super::UpdateGameObject();
}

int CMediumExplode::Update()
{
    if (m_bDestroy) return OBJ_DESTROY;

    __super::UpdateGameObject();

    return OBJ_NOEVENT;
}

void CMediumExplode::LateUpdate()
{
    m_pAnim->UpdateAnimation();
}

void CMediumExplode::Render(HDC _hDC)
{
    if (m_vPivot == Vector2::Zero) return;

    m_pAnim->RenderAnimation(_hDC);

    if (m_pAnim->GetEndOneLoop())
        m_bDestroy = true;
}

void CMediumExplode::Release()
{
    SafeDelete<CAnimation*>(m_pAnim);
}

void CMediumExplode::OnCollision(CGameObject* _pCol, Vector2 _vColSize, COLLISION_COL_FLAG _eFlag)
{
}
