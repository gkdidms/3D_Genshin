#include "Body_EvilEye.h"

#include "Boss.h"

CBody_EvilEye::CBody_EvilEye(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CBoss_Body{ pDevice, pContext }
{
}

CBody_EvilEye::CBody_EvilEye(const CBody_EvilEye& rhs)
	: CBoss_Body{ rhs }
{
}

HRESULT CBody_EvilEye::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CBody_EvilEye::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Add_Components()))
		return E_FAIL;

    m_Info.fMaxHp = { 146125.f };
    m_Info.fHp = { 146125.f };

	return S_OK;
}

void CBody_EvilEye::Priority_Tick(const _float& fTimeDelta)
{
}

void CBody_EvilEye::Tick(const _float& fTimeDelta)
{
	Change_Animation();

	m_pModelCom->Play_Animation(fTimeDelta, &m_PlayerMovePos);
}

void CBody_EvilEye::Late_Tick(const _float& fTimeDelta)
{
	XMStoreFloat4x4(&m_pWorldMatrix, m_pTransformCom->Get_WorldMatrix() * XMLoadFloat4x4(m_pParentMatrix));

	m_pGameInstance->Add_Renderer(CRenderer::RENDER_NONBLENDER, this);
}

HRESULT CBody_EvilEye::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	return S_OK;
}

HRESULT CBody_EvilEye::Add_Components()
{
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, L"Prototype_Component_Shader_VtxAnimMesh", L"Com_Shader", reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, L"Prototype_Component_Model_Boss_EvilEye", L"Com_Model", reinterpret_cast<CComponent**>(&m_pModelCom))))
		return E_FAIL;

	return S_OK;
}

HRESULT CBody_EvilEye::Bind_ResourceData()
{
	if (FAILED(__super::Bind_ResourceData()))
		return E_FAIL;

	return S_OK;
}

void CBody_EvilEye::Change_Animation()
{
	_uint iAnim = { 0 };
	_bool isLoop = { false };

    switch (*m_pState)
    {
    case CBoss::BOSS_IDLE:
    {
        iAnim = 31;
        isLoop = true;
        break;
    }
    case CBoss::BOSS_BLADE_IDEL:
    {
        iAnim = 11;
        isLoop = true;
        break;
    }
    case CBoss::BOSS_RUSH_BS:
    {
        iAnim = 30;
        isLoop = false;
        break;
    }
    case CBoss::BOSS_RUSH_AS:
    {
        iAnim = 28;
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
        iAnim = 23;
        isLoop = false;
        break;
    }
    case CBoss::BOSS_HIT_L:
    {
        iAnim = 25;
        isLoop = false;
        break;
    }
    case CBoss::BOSS_WALK_R:
    {
        iAnim = 33;
        isLoop = false;
        break;
    }
    case CBoss::BOSS_WALK_L:
    {
        iAnim = 32;
        isLoop = false;
        break;
    }
    case CBoss::BOSS_WALK_R_To_L:
    {
        iAnim = 35;
        isLoop = false;
        break;
    }
    case CBoss::BOSS_WALK_L_TO_R:
    {
        iAnim = 34;
        isLoop = false;
        break;
    }
    case CBoss::BOSS_DUALBLADE_STRIKE_ATTACK_BS:
    {
        iAnim = 19;
        isLoop = false;
        break;
    }
    case CBoss::BOSS_DUALBLADE_STRIKE_ATTACK_AS:
    {
        iAnim = 18;
        isLoop = false;
        break;
    }
    case CBoss::BOSS_DUALBLADE_STRIKE_ATTACK_LOOP:
    {
        iAnim = 20;
        isLoop = true;
        break;
    }
    case CBoss::BOSS_DUALBLADE_SWEEP_ATTACK_L:
    {
        iAnim = 21;
        isLoop = false;
        break;
    }
    case CBoss::BOSS_DUALBLADE_SWEEP_ATTACK_R:
    {   
        iAnim = 22;
        isLoop = false;
        break;
    }
    case CBoss::BOSS_DUALBLADE_HIRAISHIN_BS:
    {
        iAnim = 16;
        isLoop = false;
        break;
    }
    case CBoss::BOSS_DUALBLADE_HIRAISHIN_AS:
    {
        iAnim = 15;
        isLoop = false;
        break;
    }
    case CBoss::BOSS_DUALBLADE_HIRAISHIN_LOOP:
    {
        iAnim = 17;
        isLoop = true;
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

CBody_EvilEye* CBody_EvilEye::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CBody_EvilEye* pInstance = new CBody_EvilEye(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
		Safe_Release(pInstance);

	return pInstance;
}

CGameObject* CBody_EvilEye::Clone(void* pArg)
{
	CBody_EvilEye* pInstance = new CBody_EvilEye(*this);

	if (FAILED(pInstance->Initialize(pArg)))
		Safe_Release(pInstance);

	return pInstance;
}

void CBody_EvilEye::Free()
{
	__super::Free();
}
