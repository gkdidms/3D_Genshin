#include "EffectTool_Terrain.h"

#include "GameInstance.h"

CEffectTool_Terrain::CEffectTool_Terrain(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CGameObject{ pDevice, pContext }
{
}

CEffectTool_Terrain::CEffectTool_Terrain(const CEffectTool_Terrain& rhs)
	: CGameObject{ rhs }
{
}

HRESULT CEffectTool_Terrain::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CEffectTool_Terrain::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Add_Components()))
		return E_FAIL;


	return S_OK;
}

void CEffectTool_Terrain::Priority_Tick(const _float& fTimeDelta)
{
}

void CEffectTool_Terrain::Tick(const _float& fTimeDelta)
{
}

void CEffectTool_Terrain::Late_Tick(const _float& fTimeDelta)
{
	m_pGameInstance->Add_Renderer(CRenderer::RENDER_NONBLENDER, this);
}

HRESULT CEffectTool_Terrain::Render()
{
	if (FAILED(Bind_ResouceData()))
		return E_FAIL;

	m_pShaderCom->Begin(0);
	m_pVIBufferCom->Render();


	return S_OK;
}

HRESULT CEffectTool_Terrain::Add_Components()
{
	if (FAILED(Add_Component(LEVEL_MAIN, L"Prototype_Component_Shader", L"Component_Shader", reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;

	if (FAILED(Add_Component(LEVEL_MAIN, L"Prototype_Component_Texture", L"Component_Texture", reinterpret_cast<CComponent**>(&m_pTextureCom))))
		return E_FAIL;

	if (FAILED(Add_Component(LEVEL_MAIN, L"Prototype_Component_VIBuffer_Terrain", L"Component_VIBuffer_Terrain", reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
		return E_FAIL;

	return S_OK;
}

HRESULT CEffectTool_Terrain::Bind_ResouceData()
{
	if (FAILED(m_pTransformCom->Bind_ShaderMatrix(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", m_pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_VIEW))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", m_pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_PROJ))))
		return E_FAIL;

	//m_pTextureCom->Bind_ShaderResource
	if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_DiffuseTexture", 0)))
		return E_FAIL;

	return S_OK;
}

void CEffectTool_Terrain::Get_MousePos_On_Terrain()
{

	_bool IsPicking = { false };
	_vector vMousePos = m_pGameInstance->Picking(&IsPicking);

	if (IsPicking)
	{
		_float3 vMousePosFloat3;
		XMStoreFloat3(&vMousePosFloat3, vMousePos);
	}
}

CEffectTool_Terrain* CEffectTool_Terrain::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CEffectTool_Terrain* pInstance = new CEffectTool_Terrain(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
		Safe_Release(pInstance);

	return pInstance;
}

CGameObject* CEffectTool_Terrain::Clone(void* pArg)
{
	CEffectTool_Terrain* pInstance = new CEffectTool_Terrain(*this);

	if (FAILED(pInstance->Initialize(pArg)))
		Safe_Release(pInstance);

	return pInstance;
}

void CEffectTool_Terrain::Free()
{
	__super::Free();

	Safe_Release(m_pShaderCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pVIBufferCom);
}
