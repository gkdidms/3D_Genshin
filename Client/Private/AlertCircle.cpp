#include "AlertCircle.h"

#include "GameInstance.h"

CAlertCircle::CAlertCircle(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CGameObject{ pDevice, pContext }
{
}

CAlertCircle::CAlertCircle(const CAlertCircle& rhs)
	: CGameObject{ rhs }

{
}

HRESULT CAlertCircle::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CAlertCircle::Initialize(void* pArg)
{
	if (nullptr == pArg)
		return E_FAIL;

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Add_Components()))
		return E_FAIL;

	ALERTCIRCLE_DESC* pDesc = static_cast<ALERTCIRCLE_DESC*>(pArg);
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, pDesc->vCreatePos);
	m_pTransformCom->Rotation(XMVectorSet(0.f, 0.f, 1.f, 0.f), XMConvertToRadians(90.f));

	return S_OK;
}

void CAlertCircle::Priority_Tick(const _float& fTimeDelta)
{
}

void CAlertCircle::Tick(const _float& fTimeDelta)
{
	//TODO:: 타탈 장판 수정하기 
	if (m_pTransformCom->Get_Scaled().x < m_fMaxSize)
	{
		m_pTransformCom->Set_Scale(m_fSpeed * fTimeDelta, m_fSpeed * fTimeDelta, 0.f);
	}
}

void CAlertCircle::Late_Tick(const _float& fTimeDelta)
{
	m_pGameInstance->Add_Renderer(CRenderer::RENDER_NONBLENDER, this);
}

HRESULT CAlertCircle::Render()
{
	if (FAILED(Bind_ResourceData()))
		return E_FAIL;

	m_pShaderCom->Begin(0);
	m_pVIBufferCom->Render();

	return S_OK;
}

HRESULT CAlertCircle::Add_Components()
{
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, L"Prototype_Component_Shader_VtxAnimMesh", L"Com_Shader", reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, L"Prototype_Component_VIBuffer_Rect", L"Com_VIBuffer_Rect", reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
		return E_FAIL;

	m_pTextureCom = dynamic_cast<CTexture*>(m_pGameInstance->Add_Component_Clone(LEVEL_STAGE_BOSS, L"Prototype_Component_Texture_AlertCircle", nullptr));
	if (nullptr == m_pTextureCom)
		return E_FAIL;

	return S_OK;
}

HRESULT CAlertCircle::Bind_ResourceData()
{
	if (FAILED(m_pTransformCom->Bind_ShaderMatrix(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", m_pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_VIEW))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", m_pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_PROJ))))
		return E_FAIL;

	if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_Texture", 0)))
		return E_FAIL;

	return S_OK;
}

void CAlertCircle::Change_Animation()
{
}

CAlertCircle* CAlertCircle::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CAlertCircle* pInstance = new CAlertCircle(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
		Safe_Release(pInstance);

	return pInstance;
}

CGameObject* CAlertCircle::Clone(void* pArg)
{
	CAlertCircle* pInstance = new CAlertCircle(*this);

	if (FAILED(pInstance->Initialize(pArg)))
		Safe_Release(pInstance);

	return pInstance;
}

void CAlertCircle::Free()
{
	__super::Free();
}
