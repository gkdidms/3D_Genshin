#include "Feiyan_Normal_Trail.h"

#include "MainApp.h"
#include "GameInstance.h"

CFeiyan_Normal_Trail::CFeiyan_Normal_Trail(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CBullet{ pDevice, pContext }
{
}

CFeiyan_Normal_Trail::CFeiyan_Normal_Trail(const CFeiyan_Normal_Trail& rhs)
	: CBullet { rhs }
{
}

HRESULT CFeiyan_Normal_Trail::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CFeiyan_Normal_Trail::Initialize(void* pArg)
{
	if (nullptr == pArg)
		return E_FAIL;

	FEIYAN_NORMAL_TRAIL_DESC* pDesc = static_cast<FEIYAN_NORMAL_TRAIL_DESC*>(pArg);
	m_pParentMatrix = pDesc->pParentMatrix;

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Add_Components()))
		return E_FAIL;
	return S_OK;
}

void CFeiyan_Normal_Trail::Priority_Tick(const _float& fTimeDelta)
{
}

void CFeiyan_Normal_Trail::Tick(const _float& fTimeDelta)
{
	_float4x4 TrailMatrix = *m_pParentMatrix;
	m_pTransformCom->Get_Rotation(XMVectorSet(0.f, 1.f, 0.f, 0.f), XMConvertToRadians(90.f), &TrailMatrix);
	m_pVIBufferCom->Add_Trail(fTimeDelta, XMLoadFloat4x4(&TrailMatrix));
}

void CFeiyan_Normal_Trail::Late_Tick(const _float& fTimeDelta)
{
}

HRESULT CFeiyan_Normal_Trail::Render()
{
	if (FAILED(Bind_ResourceData()))
		return E_FAIL;

	m_pShaderCom->Begin(1);
	m_pVIBufferCom->Render();

	return S_OK;
}

HRESULT CFeiyan_Normal_Trail::Add_Components()
{
	if (FAILED(__super::Add_Component(CMainApp::g_iCurrentLevel, L"Prototype_Component_Shader_VtxPosTex_Skill", L"Com_Shader", reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;

	if (FAILED(__super::Add_Component(CMainApp::g_iCurrentLevel, L"Prototype_Component_Texture_Skill_Feiyan_Normal_Trail", L"Com_TrailTexture", reinterpret_cast<CComponent**>(&m_pTextureCom))))
		return E_FAIL;

	CVIBuffer_Trail::VIBUFFER_TRAIL_DESC TrailDesc{};
	TrailDesc.iMaxTrail = 20;
	TrailDesc.vInitPosA = _float3(-0.5f, 0.5f, 0.f);
	TrailDesc.vInitPosB = _float3(-0.5f, -0.5f, 0.f);
	if (FAILED(__super::Add_Component(CMainApp::g_iCurrentLevel, L"Prototype_Component_VIBuffer_Trail", L"Com_VIBuffer", reinterpret_cast<CComponent**>(&m_pVIBufferCom), &TrailDesc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CFeiyan_Normal_Trail::Bind_ResourceData()
{
	if (FAILED(m_pTransformCom->Bind_ShaderMatrix(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", m_pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_VIEW))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", m_pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_PROJ))))
		return E_FAIL;

	if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_TrailDiffuseTexture", 0)))
		return E_FAIL;

	return S_OK;
}

CFeiyan_Normal_Trail* CFeiyan_Normal_Trail::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CFeiyan_Normal_Trail* pInstance = new CFeiyan_Normal_Trail(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
		Safe_Release(pInstance);

	return pInstance;
}

CGameObject* CFeiyan_Normal_Trail::Clone(void* pArg)
{
	CFeiyan_Normal_Trail* pInstance = new CFeiyan_Normal_Trail(*this);

	if (FAILED(pInstance->Initialize(pArg)))
		Safe_Release(pInstance);

	return pInstance;
}

void CFeiyan_Normal_Trail::Free()
{
	__super::Free();

	Safe_Release(m_pShaderCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pVIBufferCom);
}
