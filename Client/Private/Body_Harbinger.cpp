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

	return S_OK;
}

void CBody_Harbinger::Priority_Tick(const _float& fTimeDelta)
{
}

void CBody_Harbinger::Tick(const _float& fTimeDelta)
{
	Change_Animation();

	m_pModelCom->Play_Animation(fTimeDelta, &m_PlayerMovePos);
}

void CBody_Harbinger::Late_Tick(const _float& fTimeDelta)
{
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
    if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, L"Prototype_Component_Shader_VtxAnimMesh", L"Com_Shader", reinterpret_cast<CComponent**>(&m_pShaderCom))))
        return E_FAIL;

    if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, L"Prototype_Component_Model_Boss_Harbinger", L"Com_Model", reinterpret_cast<CComponent**>(&m_pModelCom))))
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
