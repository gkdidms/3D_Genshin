#include "Effect_Particle.h"

#include "GameInstance.h"

CEffect_Particle::CEffect_Particle(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CEffectObject { pDevice, pContext }
{
}

CEffect_Particle::CEffect_Particle(const CEffect_Particle& rhs)
	: CEffectObject{ rhs },
	m_strFilePath { rhs.m_strFilePath },
	m_Desc { rhs.m_Desc }
{
}

HRESULT CEffect_Particle::Initialize_Prototype(void* pArg)
{
	if (FAILED(__super::Initialize_Prototype(pArg)))
		return E_FAIL;

	if (nullptr == pArg)
		return E_FAIL;

	EFFECT_PARTICLE_DESC* pDesc = static_cast<EFFECT_PARTICLE_DESC*>(pArg);

	m_Desc = *pDesc;
	string strFilePath = pDesc->szFilePath;
	m_strFilePath.assign(strFilePath.begin(), strFilePath.end());

	return S_OK;
}

HRESULT CEffect_Particle::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Add_Components()))
		return E_FAIL;

	m_iParticleType = m_Desc.iParticleType;
	
	return S_OK;
}

void CEffect_Particle::Priority_Tick(const _float& fTimeDelta)
{

}

void CEffect_Particle::Tick(const _float& fTimeDelta)
{
	if (m_iParticleType == PARTICLE_SPREAT)
		m_pVIBufferCom->Spread(fTimeDelta);
	else if (m_iParticleType == PARTICLE_DROP)
		m_pVIBufferCom->Drop(fTimeDelta);
}

void CEffect_Particle::Late_Tick(const _float& fTimeDelta)
{
	XMStoreFloat4x4(&m_WorldMatrix, m_pTransformCom->Get_WorldMatrix() * XMLoadFloat4x4(m_ParentMatrix));

	m_pGameInstance->Add_Renderer(CRenderer::RENDER_NONLIGHT, this);
}

HRESULT CEffect_Particle::Render()
{
	if (FAILED(Bind_ResourceData()))
		return E_FAIL;

	m_pShaderCom->Begin(m_iShaderPass);
	m_pVIBufferCom->Render();

	return S_OK;
}

HRESULT CEffect_Particle::Add_Components()
{
	if (FAILED(Add_Component(LEVEL_GAMEPLAY, L"Prototype_Component_Shader_Instance_Point", L"Com_Shader", reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;

	//값을 받아와서 저장하게 하기
	CVIBuffer_Instance::INSTANCE_DESC InstanceDesc{};
	InstanceDesc.iNumInstance = m_Desc.iNumInstance;
	InstanceDesc.vOffsetPos = m_Desc.vOffsetPos;
	InstanceDesc.vPivotPos = m_Desc.vPivotPos;
	InstanceDesc.vRange = m_Desc.vRange;
	InstanceDesc.vSize = m_Desc.vSize;
	InstanceDesc.vSpeed = m_Desc.vSpeed;
	InstanceDesc.vLifeTime = m_Desc.vLifeTime;
	InstanceDesc.isLoop = m_Desc.isLoop;
	InstanceDesc.vPower = m_Desc.vPower;

	m_pVIBufferCom = CVIBuffer_Instance_Point::Create(m_pDevice, m_pContext, InstanceDesc);
	if (nullptr == m_pVIBufferCom)
		return E_FAIL;

	m_pTextureCom = CTexture::Create(m_pDevice, m_pContext, m_strFilePath, 1);
	if (nullptr == m_pTextureCom)
		return E_FAIL;

	return S_OK;
}

HRESULT CEffect_Particle::Bind_ResourceData()
{
	if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_WorldMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", m_pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_VIEW))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", m_pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_PROJ))))
		return E_FAIL;

	if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_Texture", 0)))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_RawValue("g_vCamPosition", m_pGameInstance->Get_CamPosition_Float4(), sizeof(_float4))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_RawValue("g_fDuration", &m_fDuration, sizeof(_float))))
		return E_FAIL;

	return S_OK;
}

CEffect_Particle* CEffect_Particle::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, void* pArg)
{
	CEffect_Particle* pInstance = new CEffect_Particle(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype(pArg)))
		Safe_Release(pInstance);

	return pInstance;
}

CGameObject* CEffect_Particle::Clone(void* pArg)
{
	CEffect_Particle* pInstance = new CEffect_Particle(*this);

	if (FAILED(pInstance->Initialize(pArg)))
		Safe_Release(pInstance);

	return pInstance;
}

void CEffect_Particle::Free()
{
	__super::Free();

	Safe_Release(m_pShaderCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pTextureCom);
}
