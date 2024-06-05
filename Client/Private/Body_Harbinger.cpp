#include "Body_Harbinger.h"

#include "Boss.h"

CBody_Harbinger::CBody_Harbinger(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CBoss_Body{ pDevice, pContext }
{
}

CBody_Harbinger::CBody_Harbinger(const CBody_Harbinger& rhs)
	: CBoss_Body{ rhs }
{
}

HRESULT CBody_Harbinger::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CBody_Harbinger::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

    if (FAILED(Add_Components()))
        return E_FAIL;

    m_fLongDistanceSpeed = { 60.f };

	return S_OK;
}

void CBody_Harbinger::Priority_Tick(const _float& fTimeDelta)
{
}

void CBody_Harbinger::Tick(const _float& fTimeDelta)
{
    Change_Animation();

    m_pModelCom->Play_Animation(fTimeDelta, &m_PlayerMovePos);

    //BOSS_DUALBLADE_HIRAISHIN_BS 사용 시 플레이어의 좌표를 저장해서 BOSS_DUALBLADE_HIRAISHIN_AS때 해당 좌표로 넘어감
    if (*m_pState == CBoss::BOSS_DUALBLADE_HIRAISHIN_BS && *m_pState != m_iPreState)
    {
        XMStoreFloat4x4(&m_MoveToTargetMatrix, XMLoadFloat4x4(m_pTargetMatrix));
    }
    else if (*m_pState == CBoss::BOSS_DUALBLADE_HIRAISHIN_AS 
        || ((*m_pState == CBoss::BOSS_MOVE_FORWARD || *m_pState == CBoss::BOSS_MOVE_BACK) && m_pModelCom->Get_Animation_Finished()))
    {
        m_isMovePos = true;
        m_PlayerMovePos = m_MoveToTargetMatrix;
    }
    else if (*m_pState == CBoss::BOSS_MOVE_FORWARD && *m_pState != m_iPreState)
    {
        XMStoreFloat4x4(&m_MoveToTargetMatrix, XMLoadFloat4x4(m_pTargetMatrix));
    }
    else if (*m_pState == CBoss::BOSS_MOVE_BACK && *m_pState == m_iPreState)
    {
        //현재 위치에서 20.f 만큼 뒤로가기 
        _float fBackRange = { 20.f };

        _matrix LogMatirx = XMLoadFloat4x4(m_pParentMatrix);
        LogMatirx.r[3] = XMVectorSetZ(LogMatirx.r[3], XMVectorGetZ(LogMatirx.r[3]) + fBackRange);
        XMStoreFloat4x4(&m_MoveToTargetMatrix, LogMatirx);
    }
    else m_isMovePos = false;

    //Rush
// 러쉬할때 이동하는게 아니라 러쉬 자세를 취하고 이동해야 함.  -> 이펙트 배우면 수정
    if (*m_pState == CBoss::BOSS_DUALBLADE_NORMAL_ATTACK_2) 
    {
        _matrix MoveMatrix = XMMatrixIdentity();
        MoveMatrix.r[3] = XMVectorSet(0.f, 0.f, m_fLongDistanceSpeed * fTimeDelta * -1.f, 1.f);
        XMStoreFloat4x4(&m_PlayerMovePos, MoveMatrix);
    }
}

void CBody_Harbinger::Late_Tick(const _float& fTimeDelta)
{
    XMStoreFloat4x4(&m_pWorldMatrix, m_pTransformCom->Get_WorldMatrix() * XMLoadFloat4x4(m_pParentMatrix));

    if (*m_pState != m_iPreState)
        m_iPreState = *m_pState;

	m_pGameInstance->Add_Renderer(CRenderer::RENDER_NONBLENDER, this);
}

HRESULT CBody_Harbinger::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	return S_OK;
}

HRESULT CBody_Harbinger::Add_Components()
{
    if (FAILED(__super::Add_Component(LEVEL_STAGE_BOSS, L"Prototype_Component_Shader_VtxAnimMesh", L"Com_Shader", reinterpret_cast<CComponent**>(&m_pShaderCom))))
        return E_FAIL;

    if (FAILED(__super::Add_Component(LEVEL_STAGE_BOSS, L"Prototype_Component_Model_Boss_Harbinger", L"Com_Model", reinterpret_cast<CComponent**>(&m_pModelCom))))
        return E_FAIL;

    return S_OK;
}

HRESULT CBody_Harbinger::Bind_ResourceData()
{
    if (FAILED(__super::Bind_ResourceData()))
        return E_FAIL;

	return S_OK;
}

void CBody_Harbinger::Change_Animation()
{
	_uint iAnim = { 0 };
	_bool isLoop = { true };

    switch (*m_pState)
    {
    case CBoss::BOSS_IDLE:
    {
        iAnim = 18;
        isLoop = true;
        break;
    }
    case CBoss::BOSS_BOW_COVER_ATTACK:
    {
        iAnim = 3;
        isLoop = false;
        break;
    }
    case CBoss::BOSS_BOW_NORMAL_ATTACK_BS:
    {
        iAnim = 4;
        isLoop = false;
        break;
    }
    case CBoss::BOSS_BOW_POWER_ATTACK:
    {
        iAnim = 5;
        isLoop = false;
        break;
    }
    case CBoss::BOSS_BLADE_EXTRA_ATTACK:
    {
        iAnim = 0;
        isLoop = false;
        break;
    }
    case CBoss::BOSS_BLADE_NORMAL_ATTACK_1:
    {
        iAnim = 1;
        isLoop = false;
        break;
    }
    case CBoss::BOSS_BLADE_RANGE_ATTACK:
    {
        iAnim = 2;
        isLoop = false;
        break;
    }
    case CBoss::BOSS_DUALBLADE_CYCLE_ATTACK:
    {
        iAnim = 6;
        isLoop = false;
        break;
    }
    case CBoss::BOSS_DUALBLADE_HIRAISHIN_BS: 
    {
        iAnim = 7;
        isLoop = false;
        break;
    }
    case CBoss::BOSS_DUALBLADE_HIRAISHIN_AS:
    {
        iAnim = 8;
        isLoop = false;
        break;
    }
    case CBoss::BOSS_DUALBLADE_NORMAL_ATTACK_1:
    {
        iAnim = 9;
        isLoop = false;
        break;
    }
    case CBoss::BOSS_DUALBLADE_NORMAL_ATTACK_2:
    {
        iAnim = 10;
        isLoop = false;
        break;
    }
    case CBoss::BOSS_DUALBLADE_STRIKE_ATTACK:
    {
        iAnim = 11;
        isLoop = false;
        break;
    }
    case CBoss::BOSS_DUALBLADE_SWEEP_ATTACK_L:
    {
        iAnim = 12;
        isLoop = false;
        break;
    }
    case CBoss::BOSS_DUALBLADE_SWEEP_ATTACK_R:
    {
        iAnim = 13;
        isLoop = false;
        break;
    }
    case CBoss::BOSS_HARBINGER_NORMAL_ATTACK:
    {
        iAnim = 25;
        isLoop = false;
        break;
    }
    case CBoss::BOSS_HIT_H:
    {
        iAnim = 15;
        isLoop = false;
        break;
    }
    case CBoss::BOSS_TURN_L:
    {
        iAnim = 19;
        isLoop = false;
        break;
    }
    case CBoss::BOSS_TURN_R:
    {
        iAnim = 21;
        isLoop = false;
        break;
    }
    case CBoss::BOSS_MOVE_BACK:
    {
        iAnim = 16;
        isLoop = false;
        break;
    }
    case CBoss::BOSS_MOVE_FORWARD:
    {
        iAnim = 17;
        isLoop = false;
        break;
    }
    case CBoss::BOSS_WALK_R:
    {
        iAnim = 24;
        isLoop = false;
        break;
    }
    case CBoss::BOSS_WALK_L:
    {
        iAnim = 23;
        isLoop = false;
        break;
    }
    case CBoss::BOSS_DIE:
    {
        iAnim = 14;
        isLoop = false;
        break;
    }
    case CBoss::BOSS_END:
    {
        break;
    }
    default:
        break;
    }

	m_pModelCom->Set_Animation(CModel::ANIM_DESC{ iAnim, isLoop , true, true });
}

CBody_Harbinger* CBody_Harbinger::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CBody_Harbinger* pInstance = new CBody_Harbinger(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
		Safe_Release(pInstance);

	return pInstance;
}

CGameObject* CBody_Harbinger::Clone(void* pArg)
{
	CBody_Harbinger* pInstance = new CBody_Harbinger(*this);

	if (FAILED(pInstance->Initialize(pArg)))
		Safe_Release(pInstance);

	return pInstance;
}

void CBody_Harbinger::Free()
{
	__super::Free();
}
