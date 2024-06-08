#include "Effect_Image.h"

#include "MainApp.h"
#include "GameInstance.h"

CEffect_Image::CEffect_Image(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CEffectObject{ pDevice, pContext}
{
}

CEffect_Image::CEffect_Image(const CEffect_Image& rhs)
	: CEffectObject{ rhs },
	m_iMoveType{ rhs.m_iMoveType }
{
}

HRESULT CEffect_Image::Initialize_Prototype(void* pArg)
{
	if (FAILED(__super::Initialize_Prototype(pArg)))
		return E_FAIL;

	if (nullptr == pArg)
		return E_FAIL;

	EFFECT_IMAGE_DESC* pDesc = static_cast<EFFECT_IMAGE_DESC*>(pArg);
	m_iMoveType = pDesc->iTextureMoveType;

	return S_OK;
}

HRESULT CEffect_Image::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Add_Components()))
		return E_FAIL;

	if (m_iMoveType == SPREAT)
	{
		_vector vPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
		
		_vector vWorldPos = XMVector3TransformCoord(vPos, XMLoadFloat4x4(m_ParentMatrix));
		XMStoreFloat4(&m_vSpreatDir, XMVector3Normalize(vWorldPos - XMLoadFloat4x4(m_ParentMatrix).r[3]));
		m_vSpreatDir.y = 0.f;
	}

	return S_OK;
}

void CEffect_Image::Priority_Tick(const _float& fTimeDelta)
{
}

void CEffect_Image::Tick(const _float& fTimeDelta)
{
	m_fCurrentTime += fTimeDelta;

	if (m_fStartTime >= m_fCurrentTime)
		return;

	if (m_fDurationTime <= m_fCurrentTime && m_fDurationTime > 0.f)
		return;

	if (m_iTextureNum > 1)
	{
		if (m_isFrameStop == false)
		{
			m_fFrame += m_iTextureNum * fTimeDelta * 3.f;

			if (m_fFrame >= m_iTextureNum)
			{
				if (!m_isFrameLoop) {
					m_isFrameStop = true;
					m_isDead = true;
				}
				m_fFrame = 0.f;
			}
		}
	}

	if (m_iMoveType == SHRINK)
	{
		_float fTime = fTimeDelta;

		_float3 vScale = m_pTransformCom->Get_Scaled();
		m_pTransformCom->Set_Scale(vScale.x - fTime, vScale.y - fTime, vScale.z - fTime);
	}
	else if (m_iMoveType == INCREASE)
	{
		_float fTime = fTimeDelta * 5.f;

		_float3 vScale = m_pTransformCom->Get_Scaled();
		m_pTransformCom->Set_Scale(vScale.x + fTime, vScale.y + fTime, vScale.z + fTime);
	}
	else if (m_iMoveType == SPREAT)
	{
		_vector vPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
		vPos += XMLoadFloat4(&m_vSpreatDir) * 5.f * fTimeDelta;
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, vPos);
	}

}

void CEffect_Image::Late_Tick(const _float& fTimeDelta)
{
	if (m_fStartTime >= m_fCurrentTime)
		return;

	if (m_fDurationTime <= m_fCurrentTime && m_fDurationTime > 0.f)
		return;

	XMStoreFloat4x4(&m_WorldMatrix, m_pTransformCom->Get_WorldMatrix() * XMLoadFloat4x4(m_ParentMatrix));

	m_pGameInstance->Add_Renderer(CRenderer::RENDERER_STATE(m_iRendererType), this);
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
	if (FAILED(Add_Component(CMainApp::g_iCurrentLevel, L"Prototype_Component_Shader_VtxPosTex_Effect", L"Com_Shader", reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;
	if (FAILED(Add_Component(LEVEL_STATIC, L"Prototype_Component_VIBuffer_Rect", L"Com_VIBuffer", reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
		return E_FAIL;

	if (FAILED(__super::Add_Components()))
		return E_FAIL;

	return S_OK;
}

HRESULT CEffect_Image::Bind_ResourceData()
{
	if (FAILED(__super::Bind_ResourceData()))
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

	Safe_Release(m_pVIBufferCom);
}
