#include "KasaRing.h"

#include "StateManager.h"

CKasaRing::CKasaRing(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CSkillObj{ pDevice, pContext }
{
}

CKasaRing::CKasaRing(const CKasaRing& rhs)
	: CSkillObj { rhs }
{
}

HRESULT CKasaRing::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CKasaRing::Initialize(void* pArg)
{
	if (nullptr == pArg)
		return E_FAIL;

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Add_Components()))
		return E_FAIL;

	
	return S_OK;
}

void CKasaRing::Priority_Tick(const _float& fTimeDelta)
{
}

void CKasaRing::Tick(const _float& fTimeDelta)
{
	if (!m_pStateManager->isElementalArt())
		return;

	Change_Animation(fTimeDelta);

	_float4x4 MoveMatrix;
	m_pModelCom->Play_Animation(fTimeDelta, &MoveMatrix);
}

void CKasaRing::Late_Tick(const _float& fTimeDelta)
{
	_matrix		SocketMatrix = XMMatrixIdentity();

	if (m_pStateManager->isElementalArt())
	{
		SocketMatrix = XMLoadFloat4x4(m_pSocketMatrix);

		SocketMatrix.r[0] = XMVector3Normalize(SocketMatrix.r[0]);
		SocketMatrix.r[1] = XMVector3Normalize(SocketMatrix.r[1]);
		SocketMatrix.r[2] = XMVector3Normalize(SocketMatrix.r[2]);

		XMStoreFloat4x4(&m_pWorldMatrix, m_pTransformCom->Get_WorldMatrix() * SocketMatrix * XMLoadFloat4x4(m_pParentMatrix));

		m_pGameInstance->Add_Renderer(CRenderer::RENDER_NONBLENDER, this);
	}

}

HRESULT CKasaRing::Render()
{
	if (!m_pStateManager->isElementalArt())
		return E_FAIL;

	if (FAILED(__super::Render()))
		return E_FAIL;

	return S_OK;
}

HRESULT CKasaRing::Add_Components()
{
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, L"Prototype_Component_Shader_VtxAnimMesh", L"Com_Shader", reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, L"Prototype_Component_Model_KasaRing", L"Com_Model", reinterpret_cast<CComponent**>(&m_pModelCom))))
		return E_FAIL;

	return S_OK;
}

HRESULT CKasaRing::Bind_ResourceData()
{
	if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_pWorldMatrix)))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", m_pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_VIEW))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", m_pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_PROJ))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_RawValue("g_fFar", m_pGameInstance->Get_CamFar(), sizeof(_float))))
		return E_FAIL;

	return S_OK;
}

void CKasaRing::Change_Animation(const _float& fTimeDelta)
{
	_uint iAnim = { 0 };
	_bool isLoop = { false };
	switch (*m_pState)
	{
		case PLAYER_ELEMENTAL_START: // Air
		{
			iAnim = 0;
			isLoop = false;
			break;
		}
		case PLAYER_RUN:
		{
			iAnim = 5;
			isLoop = true;
		}
		default:
		{
			iAnim = 4;
			isLoop = true;
			break;
		}
	}

	m_pModelCom->Set_Animation(CModel::ANIM_DESC{ iAnim, isLoop, true, false });
}

CKasaRing* CKasaRing::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CKasaRing* pInstance = new CKasaRing(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
		Safe_Release(pInstance);

	return pInstance;
}

CGameObject* CKasaRing::Clone(void* pArg)
{
	CKasaRing* pInstance = new CKasaRing(*this);

	if (FAILED(pInstance->Initialize(pArg)))
		Safe_Release(pInstance);
		
	return pInstance;
}

void CKasaRing::Free()
{
	__super::Free();
}
