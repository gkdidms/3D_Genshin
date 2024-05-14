#include "ThornWall.h"

#include "GameInstance.h"

CThornWall::CThornWall(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CSceneObj{ pDevice, pContext}
{
}

CThornWall::CThornWall(const CThornWall& rhs)
	: CSceneObj{rhs }
{
}

HRESULT CThornWall::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CThornWall::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	return S_OK;
}

void CThornWall::Priority_Tick(const _float& fTimeDelta)
{
}

void CThornWall::Tick(const _float& fTimeDelta)
{
	//플레이어와 충돌하면 플레이어 사망함 

	m_pColliderCom->Tick(m_pTransformCom->Get_WorldMatrix());
}

void CThornWall::Late_Tick(const _float& fTimeDelta)
{
	m_pGameInstance->Add_Renderer(CRenderer::RENDER_NONBLENDER, this);
}

HRESULT CThornWall::Render()
{
	if (FAILED(Bind_ResourceData()))
		return E_FAIL;

	_uint iNumMeshes = m_pModelCom->Get_NumMeshes();

	for (int i = 0; i < iNumMeshes; ++i)
	{
		if (FAILED(m_pModelCom->Bind_Material(m_pShaderCom, "g_Texture", i, aiTextureType_DIFFUSE)))
			continue;

		m_pShaderCom->Begin(1);
		m_pModelCom->Render(i);
	}

#ifdef _DEBUG
	m_pColliderCom->Render();
#endif // _DEBUG

	return S_OK;
}

HRESULT CThornWall::Add_Components()
{
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, L"Prototype_Component_Shader_VtxMesh", L"Com_Shader", reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, L"Prototype_Component_Model_ThornWall", L"Com_Model", reinterpret_cast<CComponent**>(&m_pModelCom))))
		return E_FAIL;

	CBounding_AABB::BOUNDING_AABB_DESC Desc{};

	Desc.eType = CCollider::COLLIDER_AABB;
	Desc.vExtents = _float3(1.f, 0.7f, 1.f);
	Desc.vCenter = _float3(0.f, Desc.vExtents.y, 0.f);

	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, L"Prototype_Component_Collider", L"Com_Collider", reinterpret_cast<CComponent**>(&m_pColliderCom), &Desc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CThornWall::Bind_ResourceData()
{
	if (FAILED(m_pTransformCom->Bind_ShaderMatrix(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", m_pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_VIEW))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", m_pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_PROJ))))
		return E_FAIL;

	return S_OK;
}

void CThornWall::Change_Animation(const _float& fTimeDelta)
{
}

CThornWall* CThornWall::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CThornWall* pInstance = new CThornWall(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
		Safe_Release(pInstance);

	return pInstance;
}

CGameObject* CThornWall::Clone(void* pArg)
{
	CThornWall* pInstance = new CThornWall(*this);

	if (FAILED(pInstance->Initialize(pArg)))
		Safe_Release(pInstance);

	return pInstance;
}

void CThornWall::Free()
{
	__super::Free();
}
