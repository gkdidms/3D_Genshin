#include "UI_Atk.h"

#include "MainApp.h"
#include "GameInstance.h"

CUI_Atk::CUI_Atk(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CUI{ pDevice, pContext }
{
}

CUI_Atk::CUI_Atk(const CUI_Atk& rhs)
	: CUI { rhs }
{
}

HRESULT CUI_Atk::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CUI_Atk::Initialize(void* pArg)
{
	if (nullptr != pArg)
	{
		UI_ATK_DESC* pDesc = static_cast<UI_ATK_DESC*>(pArg);
		m_fAtk = pDesc->fAtk;
		m_vColor = pDesc->vColor;
		m_pParentMatrix = pDesc->pParentMatrix;
	}

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Add_Components()))
		return E_FAIL;

	_matrix PreMatrix = XMMatrixIdentity();
	PreMatrix.r[3] = XMVectorSet(0.f, 0.f, 1.f, 1.f);
	m_pTransformCom->Set_WorldMatrix(PreMatrix * XMLoadFloat4x4(m_pParentMatrix));

	return S_OK;
}

void CUI_Atk::Priority_Tick(const _float& fTimeDelta)
{
}

void CUI_Atk::Tick(const _float& fTimeDelta)
{
	m_fCurrentTime += fTimeDelta;

	if (m_fCurrentTime >= m_fDuration)
	{
		m_isDead = true;
		return;
	}
		
	//_vector vPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
	//_vector vDir = XMVectorSet(0.f, 1.f, 0.f, 0.f);
	//vPos += vDir * m_fSpeed * fTimeDelta;
	//m_pTransformCom->Set_State(CTransform::STATE_POSITION, vPos);
	m_pColliderCom->Tick(m_pTransformCom->Get_WorldMatrix());
}

void CUI_Atk::Late_Tick(const _float& fTimeDelta)
{
	m_pGameInstance->Add_Renderer(CRenderer::RENDER_BLENDER, this);
}

HRESULT CUI_Atk::Render()
{
	if (FAILED(Bind_ResourceData()))
		return E_FAIL;

	m_pShaderCom->Begin(0);

	_tchar szText[MAX_PATH];
	wsprintfW(szText, L"%d", _int(m_fAtk));
	
	_vector vTextPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
	if (FAILED(m_pGameInstance->Far_Render(L"Font_SDK_SC_24", szText, vTextPos, m_vColor)))
		return E_FAIL;

#ifdef _DEBUG
	m_pColliderCom->Render();
#endif // _DEBUG

	return S_OK;
}

HRESULT CUI_Atk::Add_Components()
{
	CBounding_AABB::BOUNDING_AABB_DESC BoundingBoxDesc{};
	BoundingBoxDesc.eType = CCollider::COLLIDER_AABB;
	BoundingBoxDesc.vExtents = _float3(0.5f, 0.5f, 0.5f);
	BoundingBoxDesc.vCenter = _float3(0.f, BoundingBoxDesc.vExtents.y, 0.f);

	if (FAILED(Add_Component(CMainApp::g_iCurrentLevel, L"Prototype_Component_Collider", L"Com_Collider", reinterpret_cast<CComponent**>(&m_pColliderCom), &BoundingBoxDesc)))
		return E_FAIL;

	if (FAILED(Add_Component(CMainApp::g_iCurrentLevel, L"Prototype_Component_Shader_VtxPosTex_Effect", L"Com_Shader", reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;
}

HRESULT CUI_Atk::Bind_ResourceData()
{
	if (FAILED(m_pTransformCom->Bind_ShaderMatrix(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", m_pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_VIEW))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", m_pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_PROJ))))
		return E_FAIL;
}

CUI_Atk* CUI_Atk::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CUI_Atk* pInstance = new CUI_Atk(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
		Safe_Release(pInstance);

	return pInstance;
}

CGameObject* CUI_Atk::Clone(void* pArg)
{
	CUI_Atk* pInstance = new CUI_Atk(*this);

	if (FAILED(pInstance->Initialize(pArg)))
		Safe_Release(pInstance);

	return pInstance;
}

void CUI_Atk::Free()
{
	__super::Free();

	Safe_Release(m_pColliderCom);
}
