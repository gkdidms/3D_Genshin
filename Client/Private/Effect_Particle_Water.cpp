#include "Effect_Particle_Water.h"

#include "GameInstance.h"

CEffect_Particle_Water::CEffect_Particle_Water(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CBlendObject { pDevice, pContext }
{
}

CEffect_Particle_Water::CEffect_Particle_Water(const CEffect_Particle_Water& rhs)
	: CBlendObject{ rhs }
{
}

HRESULT CEffect_Particle_Water::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CEffect_Particle_Water::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Add_Components()))
		return E_FAIL;

	if (nullptr == pArg)
		return E_FAIL;

	PARTICLE_WATER_DESC* pDesc = static_cast<PARTICLE_WATER_DESC*>(pArg);
	m_pTransformCom->Set_WorldMatrix(pDesc->ParentMatrix);
	return S_OK;
}

void CEffect_Particle_Water::Priority_Tick(const _float& fTimeDelta)
{

}

void CEffect_Particle_Water::Tick(const _float& fTimeDelta)
{
	m_fFrame +=  31.f * fTimeDelta;

	if (m_fFrame >= 31.f)
	{
		m_isDead = true;
	}
		

	m_pVIBufferCom->Spread(fTimeDelta);
}

void CEffect_Particle_Water::Late_Tick(const _float& fTimeDelta)
{
	m_pGameInstance->Add_Renderer(CRenderer::RENDER_NONLIGHT, this);
}

HRESULT CEffect_Particle_Water::Render()
{
	if (FAILED(Bind_ResourceData()))
		return E_FAIL;

	m_pShaderCom->Begin(2);
	m_pVIBufferCom->Render();

	return S_OK;
}

HRESULT CEffect_Particle_Water::Add_Components()
{
	if (FAILED(Add_Component(LEVEL_GAMEPLAY, L"Prototype_Component_Shader_Instance_Point", L"Com_Shader", reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;
	
	if (FAILED(Add_Component(LEVEL_GAMEPLAY, L"Prototype_Component_Particle_Water", L"Com_VIBuffer", reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
		return E_FAIL;

	if (FAILED(Add_Component(LEVEL_GAMEPLAY, L"Prototype_Component_Texture_Particle_Water", L"Com_Texture", reinterpret_cast<CComponent**>(&m_pTextureCom))))
		return E_FAIL;

	return S_OK;
}

HRESULT CEffect_Particle_Water::Bind_ResourceData()
{
	if (FAILED(m_pTransformCom->Bind_ShaderMatrix(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", m_pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_VIEW))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", m_pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_PROJ))))
		return E_FAIL;

	if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_Texture", m_fFrame)))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_RawValue("g_vCamPosition", m_pGameInstance->Get_CamPosition_Float4(), sizeof(_float4))))
		return E_FAIL;

	return S_OK;
}

CEffect_Particle_Water* CEffect_Particle_Water::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CEffect_Particle_Water* pInstance = new CEffect_Particle_Water(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
		Safe_Release(pInstance);

	return pInstance;
}

CGameObject* CEffect_Particle_Water::Clone(void* pArg)
{
	CEffect_Particle_Water* pInstance = new CEffect_Particle_Water(*this);

	if (FAILED(pInstance->Initialize(pArg)))
		Safe_Release(pInstance);

	return pInstance;
}

void CEffect_Particle_Water::Free()
{
	__super::Free();

	Safe_Release(m_pShaderCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pTextureCom);
}
