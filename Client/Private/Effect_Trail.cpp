#include "Effect_Trail.h"

#include "MainApp.h"
#include "GameInstance.h"

CEffect_Trail::CEffect_Trail(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CEffectObject{ pDevice, pContext}
{
}

CEffect_Trail::CEffect_Trail(const CEffect_Trail& rhs)
	: CEffectObject{ rhs }	
{
	strcpy_s(m_szMeshTextureFilePath, rhs.m_szMeshTextureFilePath);
}

HRESULT CEffect_Trail::Initialize_Prototype(void* pArg)
{
	if (FAILED(__super::Initialize_Prototype(pArg)))
		return E_FAIL;

	EFFECT_TRAIL_DESC* pDesc = static_cast<EFFECT_TRAIL_DESC*>(pArg);
 	strcpy_s(m_szMeshTextureFilePath, pDesc->strMeshFilePath);

	return S_OK;
}

HRESULT CEffect_Trail::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Add_Components()))
		return E_FAIL;

	return S_OK;
}

void CEffect_Trail::Priority_Tick(const _float& fTimeDelta)
{
}

void CEffect_Trail::Tick(const _float& fTimeDelta)
{
	m_fCurrentTime += fTimeDelta * 2;

	if (m_iTextureNum > 1)
	{
		if (m_isFrameStop == false)
		{
			m_fFrame += m_iTextureNum * fTimeDelta * 2.f;

			if (m_fFrame >= m_iTextureNum)
			{
				if (!m_isFrameLoop)
				{
					m_isFrameStop = true;
					m_isDead = true;
				}
					
				m_fFrame = 0.f;
			}
		}
	}
}

void CEffect_Trail::Late_Tick(const _float& fTimeDelta)
{
	XMStoreFloat4x4(&m_WorldMatrix, m_pTransformCom->Get_WorldMatrix() * XMLoadFloat4x4(m_ParentMatrix));
	
	m_pGameInstance->Add_Renderer(CRenderer::RENDERER_STATE(m_iRendererType), this);
}

HRESULT CEffect_Trail::Render()
{
	if (FAILED(Bind_ResourceData()))
		return E_FAIL;

	_uint iNumMeshes = m_pModelCom->Get_NumMeshes();

	for (int i = 0; i < iNumMeshes; ++i)
	{
		m_pShaderCom->Begin(m_iShaderPass);
		m_pModelCom->Render(i);
	}

	return S_OK;
}

HRESULT CEffect_Trail::Add_Components()
{
	if (FAILED(Add_Component(CMainApp::g_iCurrentLevel, L"Prototype_Component_Shader_VtxMesh_Effect", L"Com_Shader", reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;

	_matrix PreMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f);
	m_pModelCom = CModel::Create(m_pDevice, m_pContext, m_szMeshTextureFilePath, PreMatrix, m_szMeshTextureFilePath);
	if (nullptr == m_pModelCom)
		return E_FAIL;

	if (FAILED(__super::Add_Components()))
		return E_FAIL;

	return S_OK;
}

HRESULT CEffect_Trail::Bind_ResourceData()
{
	if (FAILED(__super::Bind_ResourceData()))
		return E_FAIL;

	return S_OK;
}

CEffect_Trail* CEffect_Trail::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, void* pArg)
{
	CEffect_Trail* pInstance = new CEffect_Trail(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype(pArg)))
		Safe_Release(pInstance);

	return pInstance;
}

CGameObject* CEffect_Trail::Clone(void* pArg)
{
	CEffect_Trail* pInstance = new CEffect_Trail(*this);

	if (FAILED(pInstance->Initialize(pArg)))
		Safe_Release(pInstance);

	return pInstance;
}

void CEffect_Trail::Free()
{
	__super::Free();

	Safe_Release(m_pModelCom);
}
