#include "Body_Tartaglia.h"

#include "Boss.h"
#include "BT_Tartaglia.h"

CBody_Tartaglia::CBody_Tartaglia(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    : CBoss_Body{ pDevice, pContext }
{
}

CBody_Tartaglia::CBody_Tartaglia(const CBody_Tartaglia& rhs)
    : CBoss_Body{ rhs }
{
}

HRESULT CBody_Tartaglia::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CBody_Tartaglia::Initialize(void* pArg)
{
    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    if (FAILED(Add_Components()))
        return E_FAIL;

    m_Info.fMaxHp = { 146125.f };
    m_Info.fHp = { 146125.f };
    m_Info.fMaxDefendHp = { 6125.f };
    m_Info.fDefendHp = { 6125.f };

    m_fLongDistanceSpeed = 50.f;

    return S_OK;
}

void CBody_Tartaglia::Priority_Tick(const _float& fTimeDelta)
{
}

void CBody_Tartaglia::Tick(const _float& fTimeDelta)
{
    Change_Animation();

    m_pModelCom->Play_Animation(fTimeDelta, &m_PlayerMovePos);

    //Extra_Attack일때 이동 값 수정 
    if (*m_pState == CBoss::BOSS_BLADE_EXTRA_ATTACK)
    {
        m_fCurrentTime += fTimeDelta;

        if (m_fTime > m_fCurrentTime)
        {
            // 플레이어 위치와 보스의 위치가 가깝다면 이동 끝
            _vector vTargetPos = XMLoadFloat4x4(m_pTargetMatrix).r[3];
            _vector vBossPos = XMLoadFloat4x4(m_pParentMatrix).r[3];

            _vector vDistance = vTargetPos - vBossPos;
            
            _float fDistance = sqrtf(XMVectorGetX(vDistance) * XMVectorGetX(vDistance) + XMVectorGetY(vDistance) * XMVectorGetY(vDistance) + XMVectorGetZ(vDistance) * XMVectorGetZ(vDistance));

            _matrix MoveMatrix = XMMatrixIdentity();

            if (fDistance > 0.3f)
                MoveMatrix.r[3] = XMVectorSet(0.f, 0.f, m_fLongDistanceSpeed  * fTimeDelta * -1.f, 1.f);

            XMStoreFloat4x4(&m_PlayerMovePos, MoveMatrix);
        }
    }
    else m_fCurrentTime = 0.f;

    //Power_Attack
    if (*m_pState == CBoss::BOSS_BOW_POWER_ATTACK_BS)
    {
        _matrix MoveMatrix = XMMatrixIdentity();
        MoveMatrix.r[3] = XMVectorSet(0.f, 0.f, m_fLongDistanceSpeed * fTimeDelta, 1.f);

        XMStoreFloat4x4(&m_PlayerMovePos, MoveMatrix);
    }

    //Rush
    // 러쉬할때 이동하는게 아니라 러쉬 자세를 취하고 이동해야 함.  -> 이펙트 배우면 수정
    if (*m_pState == CBoss::BOSS_RUSH_BS)
    {
        _matrix MoveMatrix = XMMatrixIdentity();
        MoveMatrix.r[3] = XMVectorSet(0.f, 0.f, m_fLongDistanceSpeed * fTimeDelta * -1.f, 1.f);
        XMStoreFloat4x4(&m_PlayerMovePos, MoveMatrix);
    }
    
}

void CBody_Tartaglia::Late_Tick(const _float& fTimeDelta)
{
    XMStoreFloat4x4(&m_pWorldMatrix, m_pTransformCom->Get_WorldMatrix() * XMLoadFloat4x4(m_pParentMatrix));

    m_pGameInstance->Add_Renderer(CRenderer::RENDER_NONBLENDER, this);
}

HRESULT CBody_Tartaglia::Render()
{
    if (FAILED(__super::Render()))
        return E_FAIL;

    return S_OK;
}

HRESULT CBody_Tartaglia::Add_Components()
{
    if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, L"Prototype_Component_Shader_VtxAnimMesh", L"Com_Shader", reinterpret_cast<CComponent**>(&m_pShaderCom))))
        return E_FAIL;

    if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, L"Prototype_Component_Model_Boss_Tartaglia", L"Com_Model", reinterpret_cast<CComponent**>(&m_pModelCom))))
        return E_FAIL;

    return S_OK;
}

HRESULT CBody_Tartaglia::Bind_ResourceData()
{
    if (FAILED(__super::Bind_ResourceData()))
        return E_FAIL;

    return S_OK;
}

void CBody_Tartaglia::Change_Animation()
{
    _uint iAnim = { 0 };
    _bool isLoop = { false };

    switch (*m_pState)
    {
    case CBoss::BOSS_IDLE:
    {
        iAnim = 26;
        isLoop = true;
        break;
    }
    case CBoss::BOSS_BOW_IDLE:
    {
        iAnim = 26;
        isLoop = true;
        break;
    }
    case CBoss::BOSS_BLADE_IDEL:
    {
        iAnim = 11;
        isLoop = true;
        break;
    }
    case CBoss::BOSS_BOW_COMBO_ATTACK:
    {
        iAnim = 12;
        isLoop = false;
        break;
    }
    case CBoss::BOSS_BOW_COVER_ATTACK_BS:
    {
        iAnim = 14;
        isLoop = false;
        break;
    }
    case CBoss::BOSS_BOW_COVER_ATTACK_AS:
    {
        iAnim = 13;
        isLoop = false;
        break;
    }
    case CBoss::BOSS_BOW_COVER_ATTACK_LOOP:
    {
        iAnim = 15;
        isLoop = true;
        break;
    }
    case CBoss::BOSS_BOW_NORMAL_ATTACK_BS:
    {
        iAnim = 17;
        isLoop = false;
        break;
    }
    case CBoss::BOSS_BOW_NORMAL_ATTACK_AS:
    {
        iAnim = 16;
        isLoop = false;
        break;
    }
    case CBoss::BOSS_BOW_NORMAL_ATTACK_LOOP:
    {
        iAnim = 18;
        isLoop = true;
        break;
    }
    case CBoss::BOSS_BOW_POWER_ATTACK_BS:
    {
        iAnim = 20;
        isLoop = false;
        break;
    }
    case CBoss::BOSS_BOW_POWER_ATTACK_AS:
    {
        iAnim = 19;
        isLoop = false;
        break;
    }
    case CBoss::BOSS_BOW_POWER_ATTACK_LOOP:
    {
        iAnim = 21;
        isLoop = true;
        break;
    }
    case CBoss::BOSS_BOW_RANGE_ATTACK_BS:
    {
        iAnim = 23;
        isLoop = false;
        break;
    }
    case CBoss::BOSS_BOW_RANGE_ATTACK_AS:
    {
        iAnim = 22;
        isLoop = false;
        break;
    }
    case CBoss::BOSS_BOW_RANGE_ATTACK_LOOP_1:
    {
        iAnim = 24;
        isLoop = true;
        break;
    }
    case CBoss::BOSS_BOW_RANGE_ATTACK_LOOP_2:
    {
        iAnim = 25;
        isLoop = true;
        break;
    }
    case CBoss::BOSS_RUSH_BS:
    {
        iAnim = 34;
        isLoop = false;
        break;
    }
    case CBoss::BOSS_RUSH_AS:
    {
        iAnim = 32;
        isLoop = false;
        break;
    }
    case CBoss::BOSS_DEFEND_1:
    {
        iAnim = 0;
        isLoop = false;
        break;
    }
    case CBoss::BOSS_DEFEND_2:
    {
        iAnim = 1;
        isLoop = false;
        break;
    }
    case CBoss::BOSS_DEFEND_3:
    {
        iAnim = 2;
        isLoop = false;
        break;
    }
    case CBoss::BOSS_DEFEND_ATTACK_1:
    {
        iAnim = 3;
        isLoop = false;
        break;
    }
    case CBoss::BOSS_DEFEND_ATTACK_2:
    {
        iAnim = 4;
        isLoop = false;
        break;
    }
    case CBoss::BOSS_BLADE_EXTRA_ATTACK:
    {
        iAnim = 5;
        isLoop = false;
        break;
    }
    case CBoss::BOSS_BLADE_NORMAL_ATTACK_1:
    {
        iAnim = 6;
        isLoop = false;
        break;
    }
    case CBoss::BOSS_BLADE_NORMAL_ATTACK_2:
    {
        iAnim = 7;
        isLoop = false;
        break;
    }
    case CBoss::BOSS_BLADE_NORMAL_ATTACK_3:
    {
        iAnim = 8;
        isLoop = false;
        break;
    }
    case CBoss::BOSS_BLADE_NORMAL_ATTACK_4:
    {
        iAnim = 9;
        isLoop = false;
        break;
    }
    case CBoss::BOSS_HIT_H:
    {
        iAnim = 27;
        isLoop = false;
        break;
    }
    case CBoss::BOSS_HIT_L:
    {
        iAnim = 29;
        isLoop = false;
        break;
    }
    case CBoss::BOSS_WALK_R:
    {
        iAnim = 37;
        isLoop = false;
        break;
    }
    case CBoss::BOSS_WALK_L:
    {
        iAnim = 36;
        isLoop = false;
        break;
    }
    case CBoss::BOSS_WALK_R_To_L:
    {
        iAnim = 39;
        isLoop = false;
        break;
    }
    case CBoss::BOSS_WALK_L_TO_R:
    {
        iAnim = 38;
        isLoop = false;
        break;
    }
    case CBoss::BOSS_DIE:
    {
        break;
    }
    case CBoss::BOSS_END:
    {
        break;
    }
    default:
        break;
    }

    m_pModelCom->Set_Animation(CModel::ANIM_DESC{ iAnim, isLoop, true, true });
}

CBody_Tartaglia* CBody_Tartaglia::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CBody_Tartaglia* pInstance = new CBody_Tartaglia(pDevice, pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
        Safe_Release(pInstance);

    return pInstance;
}

CGameObject* CBody_Tartaglia::Clone(void* pArg)
{
    CBody_Tartaglia* pInstance = new CBody_Tartaglia(*this);

    if (FAILED(pInstance->Initialize(pArg)))
        Safe_Release(pInstance);

    return pInstance;
}

void CBody_Tartaglia::Free()
{
    __super::Free();
}
