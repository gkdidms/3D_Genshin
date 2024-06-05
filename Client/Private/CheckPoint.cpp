#include "CheckPoint.h"

#include "GameInstance.h"

CCheckPoint::CCheckPoint(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CSceneObj{ pDevice, pContext}
{
}

CCheckPoint::CCheckPoint(const CCheckPoint& rhs)
	: CSceneObj{ rhs }
{
}

HRESULT CCheckPoint::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CCheckPoint::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (nullptr != pArg)
	{
		CHECKPOINT_DESC* pDesc = static_cast<CHECKPOINT_DESC*>(pArg);

		m_pTargetMatrix = pDesc->pTargetMatrix;
	}

	return S_OK;
}

void CCheckPoint::Priority_Tick(const _float& fTimeDelta)
{
}

void CCheckPoint::Tick(const _float& fTimeDelta)
{
	if (m_isCheckPoint)
		m_fCurrentTime += fTimeDelta;

	Change_Animation(fTimeDelta);

	m_pColliderCom->Tick(m_pTransformCom->Get_WorldMatrix());
	m_pTriggerCollider->Tick(m_pTransformCom->Get_WorldMatrix());
}

void CCheckPoint::Late_Tick(const _float& fTimeDelta)
{
	m_pGameInstance->Add_Renderer(CRenderer::RENDER_NONBLENDER, this);
}

HRESULT CCheckPoint::Render()
{
	if (FAILED(Bind_ResourceData()))
		return E_FAIL;

	_uint iNumMeshes = m_pModelCom->Get_NumMeshes();

	for (int i = 0; i < iNumMeshes; ++i)
	{
		if (!m_isCheckPoint)
		{
			if (m_pModelCom->IsFindMesh(i, "Stages_Gear_CheckPoint_Leaves_Vo_effect"))
				continue;
		}

		if (FAILED(m_pModelCom->Bind_Material(m_pShaderCom, "g_DiffuseTexture", i, aiTextureType_DIFFUSE)))
			continue;

		m_pModelCom->Bind_Material(m_pShaderCom, "g_NormalTexture", i, aiTextureType_NORMALS);

		m_pShaderCom->Begin(m_pModelCom->IsFindMesh(i, "Stages_Gear_CheckPoint_Leaves_Vo_effect") ? 2 : 1);

		m_pModelCom->Render(i);
	}

#ifdef _DEBUG
	m_pColliderCom->Render();
	m_pTriggerCollider->Render();
#endif // _DEBUG

	return S_OK;
}

HRESULT CCheckPoint::Add_Components()
{
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, L"Prototype_Component_Shader_VtxMesh", L"Com_Shader", reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, L"Prototype_Component_Model_CheckPoint", L"Com_Model", reinterpret_cast<CComponent**>(&m_pModelCom))))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, L"Prototype_Component_Texture_Dissolve", L"Com_Texture", reinterpret_cast<CComponent**>(&m_pTextureCom))))
		return E_FAIL;

	CBounding_AABB::BOUNDING_AABB_DESC Desc{};

	Desc.eType = CCollider::COLLIDER_AABB;
	Desc.vExtents = _float3(2.f, 2.f, 2.f);
	Desc.vCenter = _float3(0.f, Desc.vExtents.y, 0.f);

	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, L"Prototype_Component_Collider", L"Com_Collider", reinterpret_cast<CComponent**>(&m_pColliderCom), &Desc)))
		return E_FAIL;

	CBounding_Sphere::BOUNDING_SPHERE_DESC TriggerDesc{};

	TriggerDesc.eType = CCollider::COLLIDER_SPHERE;
	TriggerDesc.fRadius = { m_fDistanceToTarget };
	TriggerDesc.vCenter = _float3(0.f, Desc.vExtents.y, 0.f);

	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, L"Prototype_Component_Collider", L"Com_Trigger", reinterpret_cast<CComponent**>(&m_pTriggerCollider), &TriggerDesc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CCheckPoint::Bind_ResourceData()
{
	if (FAILED(m_pTransformCom->Bind_ShaderMatrix(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", m_pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_VIEW))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", m_pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_PROJ))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_RawValue("g_fFar", m_pGameInstance->Get_CamFar(), sizeof(_float))))
		return E_FAIL;

	if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_NoiseTexture", 0)))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_RawValue("g_fTimeDelta", &m_fCurrentTime, sizeof(_float))))
		return E_FAIL;

	return S_OK;
}

void CCheckPoint::Change_Animation(const _float& fTimeDelta)
{
	if (m_isCheckPoint == true)
		return;

	// 반경 안으로 들어오면 체크포인트 활성화 됨.
	_vector vTargetPos = XMLoadFloat4x4(m_pTargetMatrix).r[3];
	_vector vPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);

	_float fDistance = XMVectorGetX(XMVector3Length(vTargetPos - vPos));
	if (fDistance <= m_fDistanceToTarget) // 플레이어 체크 포인트가 확인되면 해당 위치로 좌표가 찍힘.
	{
		m_isCheckPoint = true;

		CTransform* pPlayerTransform = dynamic_cast<CTransform*>(m_pGameInstance->Get_GameObject_Component(LEVEL_GAMEPLAY, L"Layer_Player", L"Com_Transform", 0));
		XMStoreFloat4x4(&m_vCheckPointPlayerPos, pPlayerTransform->Get_WorldMatrix());
	}
		
}

CCheckPoint* CCheckPoint::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CCheckPoint* pInstance = new CCheckPoint(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
		Safe_Release(pInstance);

	return pInstance;
}

CGameObject* CCheckPoint::Clone(void* pArg)
{
	CCheckPoint* pInstance = new CCheckPoint(*this);

	if (FAILED(pInstance->Initialize(pArg)))
		Safe_Release(pInstance);

	return pInstance;
}

void CCheckPoint::Free()
{
	__super::Free();

	Safe_Release(m_pTriggerCollider);
	Safe_Release(m_pTextureCom);
}
