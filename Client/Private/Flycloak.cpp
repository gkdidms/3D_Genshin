#include "Flycloak.h"

#include "StateManager.h"

CFlycloak::CFlycloak(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CPartObject{pDevice, pContext }
{
}

CFlycloak::CFlycloak(const CFlycloak& rhs)
	: CPartObject { rhs }
{
}

HRESULT CFlycloak::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CFlycloak::Initialize(void* pArg)
{
	if (nullptr == pArg)
		return E_FAIL;

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Add_Components()))
		return E_FAIL;

	FLYCLOAK_DESC* pDesc = static_cast<FLYCLOAK_DESC*>(pArg);
	m_pSocketMatrix = pDesc->pSocketCombinedTransformationMatrix;

	return S_OK;
}

void CFlycloak::Priority_Tick(const _float& fTimeDelta)
{
}

void CFlycloak::Tick(const _float& fTimeDelta)
{
	Change_Animation(fTimeDelta);

	_float4x4 pMoveMatrix;
	m_pModelCom->Play_Animation(fTimeDelta, &pMoveMatrix);
}

void CFlycloak::Late_Tick(const _float& fTimeDelta)
{
	_matrix		SocketMatrix = XMMatrixIdentity();

	if (m_pStateManager->isFly())
	{
		SocketMatrix = XMLoadFloat4x4(m_pSocketMatrix);

		SocketMatrix.r[0] = XMVector3Normalize(SocketMatrix.r[0]);
		SocketMatrix.r[1] = XMVector3Normalize(SocketMatrix.r[1]);
		SocketMatrix.r[2] = XMVector3Normalize(SocketMatrix.r[2]);

		XMStoreFloat4x4(&m_pWorldMatrix, m_pTransformCom->Get_WorldMatrix() * SocketMatrix * XMLoadFloat4x4(m_pParentMatrix));

		m_pGameInstance->Add_Renderer(CRenderer::RENDER_NONBLENDER, this);
	}
}

HRESULT CFlycloak::Render()
{
	if (!m_pStateManager->isFly()) return S_OK;

	if (FAILED(Bind_ResourceData()))
		return E_FAIL;

	_uint iNumMeshes = m_pModelCom->Get_NumMeshes();

	for (int i = 0; i < iNumMeshes; ++i)
	{
		if (FAILED(m_pModelCom->Bind_BoneMatrices(m_pShaderCom, "g_BoneMatrices", i)))
			return E_FAIL;

		if (FAILED(m_pModelCom->Bind_Material(m_pShaderCom, "g_DiffuseTexture", i, aiTextureType_DIFFUSE)))
			continue;

		m_pShaderCom->Begin(1);
		m_pModelCom->Render(i);
	}

	return S_OK;
}

HRESULT CFlycloak::Add_Components()
{
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, L"Prototype_Component_Shader_VtxAnimMesh", L"Com_Shader", reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, L"Prototype_Component_Model_Flycloak", L"Com_Model", reinterpret_cast<CComponent**>(&m_pModelCom))))
		return E_FAIL;

	return S_OK;
}

HRESULT CFlycloak::Bind_ResourceData()
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

void CFlycloak::Change_Animation(const _float& fTimeDelta)
{
	_uint iAnim = { 0 };

	iAnim = *m_pState == PLAYER_FLY_START ? 1 : 0;

	m_pModelCom->Set_Animation(CModel::ANIM_DESC{ iAnim, true, true, true });
}

CFlycloak* CFlycloak::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CFlycloak* pInstance = new CFlycloak(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
		Safe_Release(pInstance);

	return pInstance;
}

CGameObject* CFlycloak::Clone(void* pArg)
{
	CFlycloak* pInstance = new CFlycloak(*this);

	if (FAILED(pInstance->Initialize(pArg)))
		Safe_Release(pInstance);

	return pInstance;
}

void CFlycloak::Free()
{	
	__super::Free();
}
