#include "Slime_Large.h"

#include "GameInstance.h"

#include "Slime_Root.h"

CSlime_Large::CSlime_Large(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CSlime{ pDevice, pContext}
{
}

CSlime_Large::CSlime_Large(const CSlime_Large& rhs)
	: CSlime{ rhs }
{
}

HRESULT CSlime_Large::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CSlime_Large::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Add_Components()))
		return E_FAIL;

	m_pTransformCom->Set_Scale(1.5f, 1.5f, 1.5f);
	m_pModelCom->Set_Animation(CModel::ANIM_DESC{ 26, false, false, false });

	CSlime_Root::SLIME_LARGE_ROOT_DESC Desc{};
	Desc.pModel = m_pModelCom;
	Desc.pState = &m_eCurrentState;
	Desc.pTargetMatrix = m_pTargetMatrix;
	Desc.pTransformCom = m_pTransformCom;

	m_pBT = CSlime_Root::Create(&Desc);
	if (nullptr == m_pBT)
		return E_FAIL;

	return S_OK;
}

void CSlime_Large::Priority_Tick(const _float& fTimeDelta)
{
}

void CSlime_Large::Tick(const _float& fTimeDelta)
{
	m_pBT->Evaluate();
}

void CSlime_Large::Late_Tick(const _float& fTimeDelta)
{
}

HRESULT CSlime_Large::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	return S_OK;
}

HRESULT CSlime_Large::Bind_ResourceData()
{
	if (FAILED(m_pTransformCom->Bind_ShaderMatrix(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", m_pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_VIEW))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", m_pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_PROJ))))
		return E_FAIL;


	return S_OK;
}

void CSlime_Large::Change_Animation(const _float& fTimeDelta)
{
	_uint iAnim = { 0 };
	_bool isLoop = { false };

	switch (m_eCurrentState)
	{
	case SLIME_LARGE_IDLE:
		iAnim = 26;
		isLoop = false;
		break;
	case SLIME_LARGE_JUMP:
		iAnim = 21;
		isLoop = false;
		break;
	case SLIME_LARGE_JUMP_ATK:
		iAnim = 3;
		isLoop = false;
		break;
	case SLIME_LARGE_HIT_L:
		iAnim = 13;
		isLoop = false;
		break;
	case SLIME_LARGE_HIT_H:
		iAnim = 15;
		isLoop = false;
		break;
	case SLIME_LARGE_DIE:
		iAnim = 11;
		isLoop = false;
		break;
	default:
		break;
	}

	m_pModelCom->Set_Animation(CModel::ANIM_DESC{ iAnim, isLoop, true, false });
}

void CSlime_Large::Free()
{
	__super::Free();
}
