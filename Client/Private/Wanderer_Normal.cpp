#include "Wanderer_Normal.h"

#include "MainApp.h"
#include "GameInstance.h"

CWanderer_Normal::CWanderer_Normal(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CPlayer_Bullet{pDevice, pContext}
{
}

CWanderer_Normal::CWanderer_Normal(const CWanderer_Normal& rhs)
	: CPlayer_Bullet{ rhs }
{
}

HRESULT CWanderer_Normal::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CWanderer_Normal::Initialize(void* pArg)
{
	if (nullptr == pArg)
		return E_FAIL;

	WANDERER_NORMAL_DESC* pDesc = static_cast<WANDERER_NORMAL_DESC*>(pArg);
	m_fSpeed = pDesc->fSpeedPecSec;
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Add_Components()))
		return E_FAIL;

	_matrix WorldMatrix;
	_matrix HandMatrix = XMLoadFloat4x4(&pDesc->HandCombinedTransformationMatrix);
	HandMatrix.r[0] = XMVector3Normalize(HandMatrix.r[0]);
	HandMatrix.r[1] = XMVector3Normalize(HandMatrix.r[1]);
	HandMatrix.r[2] = XMVector3Normalize(HandMatrix.r[2]);

	m_pTransformCom->Rotation(XMVectorSet(0.f, 1.f, 0.f, 0.f), pDesc->iDir == DIR_RIGHT ? XMConvertToRadians(-45.f) : XMConvertToRadians(45.f));

	WorldMatrix = m_pTransformCom->Get_WorldMatrix() * HandMatrix * XMLoadFloat4x4(pDesc->ParentMatrix);

	m_pTransformCom->Set_WorldMatrix(WorldMatrix);
	m_vTargetPos.y = m_vTargetPos.y + 1.f;
	m_pTransformCom->Set_Scale(1.5f, 1.f, 1.5f);

	// 방향 구하기
	Get_Look();

	return S_OK;
}

void CWanderer_Normal::Priority_Tick(const _float& fTimeDelta)
{
}

void CWanderer_Normal::Tick(const _float& fTimeDelta)
{
	m_fCurrentTime += fTimeDelta;

	if (m_fCurrentTime > m_fDuration)
	{
		m_isDead = true;
		return;
	}
		
	//이동
	_vector vPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
	vPos += XMLoadFloat4(&m_vTargetLook) * m_fSpeed * fTimeDelta;

	m_pTransformCom->Set_State(CTransform::STATE_POSITION, vPos);

	m_pColliderCom->Tick(m_pTransformCom->Get_WorldMatrix());
}

void CWanderer_Normal::Late_Tick(const _float& fTimeDelta)
{
	Check_Coll();

	m_pGameInstance->Add_Renderer(CRenderer::RENDER_NONLIGHT, this);
}

HRESULT CWanderer_Normal::Render()
{
	if (FAILED(Bind_ResourceData()))
		return E_FAIL;

	_uint iNumMeshes = m_pModelCom->Get_NumMeshes();

	for (int i = 0; i < iNumMeshes; ++i)
	{
		m_pShaderCom->Begin(0);
		m_pModelCom->Render(i);
	}

#ifdef _DEBUG
	m_pColliderCom->Render();
#endif // _DEBUG


	return S_OK;
}

HRESULT CWanderer_Normal::Add_Components()
{
	if (FAILED(__super::Add_Component(CMainApp::g_iCurrentLevel, L"Prototype_Component_Shader_VtxNorTex_Skill", L"Com_Shader", reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;

	if (FAILED(__super::Add_Component(CMainApp::g_iCurrentLevel, L"Prototype_Component_Texture_Skill_Wanderer_Normal", L"Com_Texture", reinterpret_cast<CComponent**>(&m_pTextureCom))))
		return E_FAIL;

	if (FAILED(__super::Add_Component(CMainApp::g_iCurrentLevel, L"Prototype_Component_Model_Wanderer_Normal", L"Com_Model", reinterpret_cast<CComponent**>(&m_pModelCom))))
		return E_FAIL;

	CBounding_AABB::BOUNDING_AABB_DESC BoundingBoxDesc{};
	BoundingBoxDesc.eType = CCollider::COLLIDER_AABB;
	BoundingBoxDesc.vExtents = _float3(0.5f, 0.3f, 0.5f);
	BoundingBoxDesc.vCenter = _float3(0.f, -BoundingBoxDesc.vExtents.y, 0.f);

	if (FAILED(__super::Add_Component(CMainApp::g_iCurrentLevel, L"Prototype_Component_Collider", L"Com_Collider", reinterpret_cast<CComponent**>(&m_pColliderCom), &BoundingBoxDesc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CWanderer_Normal::Bind_ResourceData()
{
	if (FAILED(m_pTransformCom->Bind_ShaderMatrix(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", m_pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_VIEW))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", m_pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_PROJ))))
		return E_FAIL;

	if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_DiffuseTexture", 0)))
		return E_FAIL;

	return S_OK;
}

void CWanderer_Normal::Get_Look()
{
	XMStoreFloat4(&m_vTargetLook, XMVector3Normalize(XMLoadFloat4(&m_vTargetPos) - m_pTransformCom->Get_State(CTransform::STATE_POSITION)));
}

CWanderer_Normal* CWanderer_Normal::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CWanderer_Normal* pInstance = new CWanderer_Normal(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
		Safe_Release(pInstance);

	return pInstance;
}

CGameObject* CWanderer_Normal::Clone(void* pArg)
{
	CWanderer_Normal* pInstance = new CWanderer_Normal(*this);

	if (FAILED(pInstance->Initialize(pArg)))
		Safe_Release(pInstance);

	return pInstance;
}

void CWanderer_Normal::Free()
{
	__super::Free();

	Safe_Release(m_pModelCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pTextureCom);
}
