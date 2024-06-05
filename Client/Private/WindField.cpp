#include "WindField.h"

#include "GameInstance.h"
#include "StateManager.h"
#include "Player.h"

CWindField::CWindField(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CSceneObj{ pDevice, pContext },
	m_pStateManager{ CStateManager::GetInstance() }
{
	Safe_AddRef(m_pStateManager);
}

CWindField::CWindField(const CWindField& rhs)
	: CSceneObj{ rhs },
	m_pStateManager{ rhs.m_pStateManager }
{
	Safe_AddRef(m_pStateManager);
}

HRESULT CWindField::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CWindField::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	return S_OK;
}

void CWindField::Priority_Tick(const _float& fTimeDelta)
{
}

void CWindField::Tick(const _float& fTimeDelta)
{
	CCollider* pTargetCollider =  dynamic_cast<CCollider*>(m_pGameInstance->Get_GameObject_Component(LEVEL_STATIC, TEXT("Layer_Player"), TEXT("Com_Collider", 0)));

	m_pStateManager->Set_CollWindField(m_pColliderCom->Intersect(pTargetCollider));

	m_pColliderCom->Tick(m_pTransformCom->Get_WorldMatrix());
}

void CWindField::Late_Tick(const _float& fTimeDelta)
{
	m_pGameInstance->Add_Renderer(CRenderer::RENDER_NONBLENDER, this);
}

HRESULT CWindField::Render()
{
	if (FAILED(Bind_ResourceData()))
		return E_FAIL;

	_uint iNumMeshes = m_pModelCom->Get_NumMeshes();

	for (int i = 0; i < iNumMeshes; ++i)
	{
		//if (FAILED(m_pModelCom->Bind_Material(m_pShaderCom, "g_Texture", i, aiTextureType_DIFFUSE)))
		//	continue;
		m_pModelCom->Bind_Material(m_pShaderCom, "g_DiffuseTexture", i, aiTextureType_DIFFUSE);

		m_pModelCom->Bind_Material(m_pShaderCom, "g_NormalTexture", i, aiTextureType_NORMALS);

		m_pShaderCom->Begin(1);
		m_pModelCom->Render(i);
	}

#ifdef _DEBUG
	m_pColliderCom->Render();
#endif // _DEBUG

	return S_OK;
}

HRESULT CWindField::Add_Components()
{
	if (FAILED(__super::Add_Component(LEVEL_STATIC, L"Prototype_Component_Shader_VtxMesh", L"Com_Shader", reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, L"Prototype_Component_Model_WindField", L"Com_Model", reinterpret_cast<CComponent**>(&m_pModelCom))))
		return E_FAIL;

	CBounding_AABB::BOUNDING_AABB_DESC Desc{};

	Desc.eType = CCollider::COLLIDER_AABB;
	Desc.vExtents = _float3(3.f, 100.f, 3.f);
	Desc.vCenter = _float3(0.f, Desc.vExtents.y, 0.f);

	if (FAILED(__super::Add_Component(LEVEL_STATIC, L"Prototype_Component_Collider", L"Com_Collider", reinterpret_cast<CComponent**>(&m_pColliderCom), &Desc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CWindField::Bind_ResourceData()
{
	if (FAILED(m_pTransformCom->Bind_ShaderMatrix(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", m_pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_VIEW))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", m_pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_PROJ))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_RawValue("g_fFar", m_pGameInstance->Get_CamFar(), sizeof(_float))))
		return E_FAIL;

	return S_OK;
}

void CWindField::Change_Animation(const _float& fTimeDelta)
{
}

CWindField* CWindField::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CWindField* pInstance = new CWindField(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
		Safe_Release(pInstance);

	return pInstance;
}

CGameObject* CWindField::Clone(void* pArg)
{
	CWindField* pInstance = new CWindField(*this);

	if (FAILED(pInstance->Initialize(pArg)))
		Safe_Release(pInstance);

	return pInstance;
}

void CWindField::Free()
{
	__super::Free();

	Safe_Release(m_pStateManager);
}
