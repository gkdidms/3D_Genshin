#include "Tool_Effect_Point.h"

#include "GameInstance.h"

CTool_Effect_Point::CTool_Effect_Point(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CTool_Effect{ pDevice, pContext }
{
}

CTool_Effect_Point::CTool_Effect_Point(const CTool_Effect_Point& rhs)
	: CTool_Effect{ rhs }
{
}

HRESULT CTool_Effect_Point::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CTool_Effect_Point::Initialize(void* pArg)
{
	if (nullptr == pArg)
		return E_FAIL;

	EFFECT_POINT_DESC* pDesc = static_cast<EFFECT_POINT_DESC*>(pArg);
	m_ParticleType = pDesc->iParticleType;
	
	string strFilePath(pDesc->strTextureFilePath);
	
	m_strTextureFilePath.assign(strFilePath.begin(), strFilePath.end());
	m_EffectDesc = *pDesc;

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Add_Components()))
		return E_FAIL;

	return S_OK;
}

void CTool_Effect_Point::Priority_Tick(const _float& fTimeDelta)
{
}

void CTool_Effect_Point::Tick(const _float& fTimeDelta)
{
	if (m_ParticleType == SPREAD)
		m_pVIBufferCom->Spread(fTimeDelta);
	else if (m_ParticleType == DROP)
		m_pVIBufferCom->Drop(fTimeDelta);
}

void CTool_Effect_Point::Late_Tick(const _float& fTimeDelta)
{
	m_pGameInstance->Add_Renderer(CRenderer::RENDER_NONLIGHT, this);
}

HRESULT CTool_Effect_Point::Render()
{
	if (FAILED(Bind_ResourceData()))
		return E_FAIL;

	m_pShaderCom->Begin(m_ParticleType);
	m_pVIBufferCom->Render();

	return S_OK;
}

HRESULT CTool_Effect_Point::Add_Components()
{
	if (FAILED(Add_Component(LEVEL_MAIN, L"Prototype_Component_Shader_Instance_Point", L"Com_Shader", reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;

	//값을 받아와서 저장하게 하기
	CVIBuffer_Instance::INSTANCE_DESC InstanceDesc{};
	InstanceDesc.iNumInstance = m_EffectDesc.iNumInstance;
	InstanceDesc.vOffsetPos = m_EffectDesc.vOffsetPos;
	InstanceDesc.vPivotPos = m_EffectDesc.vPivotPos;
	InstanceDesc.vRange = m_EffectDesc.vRange;
	InstanceDesc.vSize = m_EffectDesc.vSize;
	InstanceDesc.vSpeed = m_EffectDesc.vSpeed;
	InstanceDesc.vLifeTime = m_EffectDesc.vLifeTime;
	InstanceDesc.isLoop = m_EffectDesc.isLoop;

	m_pVIBufferCom = CVIBuffer_Instance_Point::Create(m_pDevice, m_pContext, InstanceDesc);
	if (nullptr == m_pVIBufferCom)
		return E_FAIL;

	m_pTextureCom = CTexture::Create(m_pDevice, m_pContext, m_strTextureFilePath, 1);
	if (nullptr == m_pTextureCom)
		return E_FAIL;

	return S_OK;
}

HRESULT CTool_Effect_Point::Bind_ResourceData()
{
	if (FAILED(m_pTransformCom->Bind_ShaderMatrix(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", m_pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_VIEW))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", m_pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_PROJ))))
		return E_FAIL;

	if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_Texture", 0)))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_RawValue("g_vCamPosition", m_pGameInstance->Get_CamPosition_Float4(), sizeof(_float4))))
		return E_FAIL;

	return S_OK;
}

CTool_Effect_Point* CTool_Effect_Point::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CTool_Effect_Point* pInstance = new CTool_Effect_Point(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
		Safe_Release(pInstance);

	return pInstance;
}

CGameObject* CTool_Effect_Point::Clone(void* pArg)
{
	CTool_Effect_Point* pInstance = new CTool_Effect_Point(*this);

	if (FAILED(pInstance->Initialize(pArg)))
		Safe_Release(pInstance);

	return pInstance;
}

void CTool_Effect_Point::Free()
{
	__super::Free();

	Safe_Release(m_pVIBufferCom);
}
