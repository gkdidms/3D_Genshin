#include "FlowerArrow.h"

#include "MainApp.h"

CFlowerArrow::CFlowerArrow(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CSkillObj{ pDevice, pContext }
{
}

CFlowerArrow::CFlowerArrow(const CFlowerArrow& rhs)
	: CSkillObj{ rhs }
{
}

HRESULT CFlowerArrow::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CFlowerArrow::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Add_Components()))
		return E_FAIL;
	
	m_pTransformCom->Rotation(XMVectorSet(0.f, 1.f, 0.f, 0.f), XMConvertToRadians(-90.f));
	m_pTransformCom->WorldRotation(XMVectorSet(0.f, 0.f, 1.f, 0.f), XMConvertToRadians(45.f));
	m_pModelCom->Set_Animation(CModel::ANIM_DESC{ 0, false, false, false });

	return S_OK;
}

void CFlowerArrow::Priority_Tick(const _float& fTimeDelta)
{
}

void CFlowerArrow::Tick(const _float& fTimeDelta)
{
	if (m_pModelCom->Get_Animation_Finished())
	{
		m_isDead = true;
		return;
	}

	_float4x4 MoveMatrix;
	m_pModelCom->Play_Animation(fTimeDelta, &MoveMatrix);
}

void CFlowerArrow::Late_Tick(const _float& fTimeDelta)
{
	m_isHide = !(*m_pState == PLAYER_ELEMENTAL_BURST);
	if (!m_isHide)
	{
		_matrix		SocketMatrix = XMMatrixIdentity();

		SocketMatrix = XMLoadFloat4x4(m_pSocketMatrix);

		SocketMatrix.r[0] = XMVector3Normalize(SocketMatrix.r[0]);
		SocketMatrix.r[1] = XMVector3Normalize(SocketMatrix.r[1]);
		SocketMatrix.r[2] = XMVector3Normalize(SocketMatrix.r[2]);

		XMStoreFloat4x4(&m_pWorldMatrix, m_pTransformCom->Get_WorldMatrix() * SocketMatrix * XMLoadFloat4x4(m_pParentMatrix));

		m_pGameInstance->Add_Renderer(CRenderer::RENDER_NONLIGHT, this);
	}
}

HRESULT CFlowerArrow::Render()
{
	if (m_isHide)
		return S_OK;

	if (FAILED(Bind_ResourceData()))
		return E_FAIL;

	_uint iNumMeshes = m_pModelCom->Get_NumMeshes();

	for (int i = 0; i < iNumMeshes; ++i)
	{
		if (FAILED(m_pModelCom->Bind_BoneMatrices(m_pShaderCom, "g_BoneMatrices", i)))
			return E_FAIL;

		if (FAILED(m_pModelCom->Bind_Material(m_pShaderCom, "g_DiffuseTexture", i, aiTextureType_DIFFUSE)))
		{
			if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_DiffuseTexture", 0)))
				return E_FAIL;
		}

		m_pShaderCom->Begin(0);
		m_pModelCom->Render(i);
	}

	return S_OK;

	return S_OK;
}

HRESULT CFlowerArrow::Add_Components()
{
	if (FAILED(__super::Add_Component(CMainApp::g_iCurrentLevel, L"Prototype_Component_Shader_VtxAnimMesh", L"Com_Shader", reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;

	if (FAILED(__super::Add_Component(CMainApp::g_iCurrentLevel, L"Prototype_Component_Model_FlowerArrow", L"Com_Model", reinterpret_cast<CComponent**>(&m_pModelCom))))
		return E_FAIL;

	if (FAILED(__super::Add_Component(CMainApp::g_iCurrentLevel, L"Prototype_Component_Texture_Skill_Tighnari_Burst", L"Com_Texture", reinterpret_cast<CComponent**>(&m_pTextureCom))))
		return E_FAIL;

	return S_OK;
}

HRESULT CFlowerArrow::Bind_ResourceData()
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

void CFlowerArrow::Change_Animation(const _float& fTimeDelta)
{
}

CFlowerArrow* CFlowerArrow::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CFlowerArrow* pInstance = new CFlowerArrow(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
		Safe_Release(pInstance);

	return pInstance;
}

CGameObject* CFlowerArrow::Clone(void* pArg)
{
	CFlowerArrow* pInstance = new CFlowerArrow(*this);

	if (FAILED(pInstance->Initialize(pArg)))
		Safe_Release(pInstance);

	return pInstance;
}

void CFlowerArrow::Free()
{
	__super::Free();

	Safe_Release(m_pTextureCom);
}
