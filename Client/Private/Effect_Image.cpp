#include "Effect_Image.h"

#include "GameInstance.h"

CEffect_Image::CEffect_Image(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CEffectObject{ pDevice, pContext}
{
}

CEffect_Image::CEffect_Image(const CEffect_Image& rhs)
	: CEffectObject{ rhs },
	m_strFilePath{ rhs.m_strFilePath }
{
}

HRESULT CEffect_Image::Initialize_Prototype(void* pArg)
{
	if (FAILED(__super::Initialize_Prototype(pArg)))
		return E_FAIL;

	if (nullptr == pArg)
		return E_FAIL;

	EFFECT_IMAGE_DESC* pDesc = static_cast<EFFECT_IMAGE_DESC*>(pArg);
	string strFilePath = pDesc->szFilePath;
	m_strFilePath.assign(strFilePath.begin(), strFilePath.end());

	return S_OK;
}

HRESULT CEffect_Image::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Add_Components()))
		return E_FAIL;

	EFFECT_IMAGE_DESC* pDesc = static_cast<EFFECT_IMAGE_DESC*>(pArg);
	m_MoveType = pDesc->iMoveType;

	if (m_MoveType == INCREASE)
		m_pTransformCom->Set_Scale(0.5f, 0.5f, 0.5f);

	return S_OK;
}

void CEffect_Image::Priority_Tick(const _float& fTimeDelta)
{
}

void CEffect_Image::Tick(const _float& fTimeDelta)
{

	if (m_MoveType == SHRINK)
	{
		_float fTime = fTimeDelta;

		_float3 vScale = m_pTransformCom->Get_Scaled();
		m_pTransformCom->Set_Scale(vScale.x - fTime, vScale.y - fTime, vScale.z - fTime);
	}
	else if (m_MoveType == INCREASE)
	{
		_float fTime = fTimeDelta * 2.f;

		_float3 vScale = m_pTransformCom->Get_Scaled();
		m_pTransformCom->Set_Scale(vScale.x + fTime, vScale.y + fTime, vScale.z + fTime);
	}

}

void CEffect_Image::Late_Tick(const _float& fTimeDelta)
{
	XMStoreFloat4x4(&m_WorldMatrix, m_pTransformCom->Get_WorldMatrix() * XMLoadFloat4x4(m_ParentMatrix));

	m_pGameInstance->Add_Renderer(CRenderer::RENDER_NONLIGHT, this);
}

HRESULT CEffect_Image::Render()
{
	if (FAILED(Bind_ResourceData()))
		return E_FAIL;

	m_pShaderCom->Begin(m_iShaderPass);
	m_pVIBufferCom->Render();

	return S_OK;
}

HRESULT CEffect_Image::Add_Components()
{
	if (FAILED(Add_Component(LEVEL_GAMEPLAY, L"Prototype_Component_Shader_VtxPosTex_Effect", L"Com_Shader", reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;
	if (FAILED(Add_Component(LEVEL_STATIC, L"Prototype_Component_VIBuffer_Rect", L"Com_VIBuffer", reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
		return E_FAIL;

	m_pTextureCom = CTexture::Create(m_pDevice, m_pContext, m_strFilePath, 1);
	if (nullptr == m_pTextureCom)
		return E_FAIL;

	

	return S_OK;
}

HRESULT CEffect_Image::Bind_ResourceData()
{
	if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_WorldMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", m_pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_VIEW))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", m_pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_PROJ))))
		return E_FAIL;

	if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_DiffuseTexture", 0)))
		return E_FAIL;

	return S_OK;
}

CEffect_Image* CEffect_Image::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, void* pArg)
{
	CEffect_Image* pInstance = new CEffect_Image(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype(pArg)))
		Safe_Release(pInstance);

	return pInstance;
}

CGameObject* CEffect_Image::Clone(void* pArg)
{
	CEffect_Image* pInstance = new CEffect_Image(*this);

	if (FAILED(pInstance->Initialize(pArg)))
		Safe_Release(pInstance);

	return pInstance;
}

void CEffect_Image::Free()
{
	__super::Free();

	Safe_Release(m_pShaderCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pVIBufferCom);
}
