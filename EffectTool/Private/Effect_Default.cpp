#include "Effect_Default.h"

#include "GameInstance.h"

CEffect_Default::CEffect_Default(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CTool_Effect{ pDevice, pContext }
{
}

CEffect_Default::CEffect_Default(const CEffect_Default& rhs)
	: CTool_Effect { rhs }
{
}

HRESULT CEffect_Default::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CEffect_Default::Initialize(void* pArg)
{
	if (nullptr == pArg)
		return E_FAIL;

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Add_Components()))
		return E_FAIL;

	return S_OK;
}

void CEffect_Default::Priority_Tick(const _float& fTimeDelta)
{
}

void CEffect_Default::Tick(const _float& fTimeDelta)
{
	if (m_iNumTexture > 1)
	{
		if (m_isFrameStop == false)
		{
			m_fFrame += m_iNumTexture * fTimeDelta;

			if (m_fFrame >= m_iNumTexture)
			{
				if (!m_isFrameLoop) m_isFrameStop = true;
				m_fFrame = 0.f;
			}
		}
	}

	if (m_iTextureMoveType == SHRINK)
	{
		_float fTime = fTimeDelta * m_fTextureMoveSpeed;

		_float3 vScale = m_pTransformCom->Get_Scaled();
		m_pTransformCom->Set_Scale(vScale.x - fTime, vScale.y - fTime, vScale.z - fTime);
	}
	else if (m_iTextureMoveType == INCREASE)
	{
		_float fTime = fTimeDelta * m_fTextureMoveSpeed;

		_float3 vScale = m_pTransformCom->Get_Scaled();
		m_pTransformCom->Set_Scale(vScale.x + fTime, vScale.y + fTime, vScale.z + fTime);
	}
}

void CEffect_Default::Late_Tick(const _float& fTimeDelta)
{
	m_pGameInstance->Add_Renderer(CRenderer::RENDERER_STATE(m_iRendererType), this);
}

HRESULT CEffect_Default::Render()
{
	if (FAILED(Bind_ResourceData()))
		return E_FAIL;

	m_pShaderCom->Begin(m_iShaderPass);
	m_pVIBufferCom->Render();

	return S_OK;
}

HRESULT CEffect_Default::Add_Components()
{
	if (FAILED(Add_Component(LEVEL_MAIN, L"Prototype_Component_Shader_VtxPosTex", L"Com_Shader", reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;

	if (FAILED(Add_Component(LEVEL_MAIN, L"Prototype_Component_VIBuffer_Rect", L"Com_VIBuffer", reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
		return E_FAIL;

	if (FAILED(__super::Add_Components()))
		return E_FAIL;

	return S_OK;
}

HRESULT CEffect_Default::Bind_ResourceData()
{
	if (FAILED(__super::Bind_ResourceData()))
		return E_FAIL;

	return S_OK;
}

CEffect_Default* CEffect_Default::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CEffect_Default* pInstance = new CEffect_Default(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
		Safe_Release(pInstance);

	return pInstance;
}

CGameObject* CEffect_Default::Clone(void* pArg)
{
	CEffect_Default* pInstance = new CEffect_Default(*this);

	if (FAILED(pInstance->Initialize(pArg)))
		Safe_Release(pInstance);

	return pInstance;
}

void CEffect_Default::Free()
{
	__super::Free();

	Safe_Release(m_pVIBufferCom);
}
