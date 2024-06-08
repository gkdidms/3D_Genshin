#include "DungeonGate.h"

#include "GameInstance.h"

CDungeonGate::CDungeonGate(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CSceneObj{ pDevice, pContext}
{
}

CDungeonGate::CDungeonGate(const CDungeonGate& rhs)
	: CSceneObj{ rhs }
{
}

HRESULT CDungeonGate::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CDungeonGate::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	m_CurrentState = GATE_IDEL;

	return S_OK;
}

void CDungeonGate::Priority_Tick(const _float& fTimeDelta)
{
	//if (m_CurrentState == GATE_OPEN)
	//	return;

	//__super::Priority_Tick(fTimeDelta);
}

void CDungeonGate::Tick(const _float& fTimeDelta)
{
	Change_Animation(fTimeDelta);

	_float4x4 MoveMatrix;
	m_pModelCom->Play_Animation(fTimeDelta, &MoveMatrix);
	
	m_pColliderCom->Tick(m_pTransformCom->Get_WorldMatrix());
}

void CDungeonGate::Late_Tick(const _float& fTimeDelta)
{
	m_pGameInstance->Add_Renderer(CRenderer::RENDER_NONBLENDER, this);
}

HRESULT CDungeonGate::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

#ifdef _DEBUG
	m_pColliderCom->Render();
#endif // _DEBUG

	return S_OK;
}

HRESULT CDungeonGate::Add_Components()
{
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, L"Prototype_Component_Shader_VtxAnimMesh", L"Com_Shader", reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, L"Prototype_Component_Model_DungeonGate", L"Com_Model", reinterpret_cast<CComponent**>(&m_pModelCom))))
		return E_FAIL;

	CBounding_AABB::BOUNDING_AABB_DESC Desc{};

	Desc.eType = CCollider::COLLIDER_AABB;
	Desc.vExtents = _float3(0.3f, 2.f, 2.f);
	Desc.vCenter = _float3(0.f, Desc.vExtents.y, 0.f);

	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, L"Prototype_Component_Collider", L"Com_Collider", reinterpret_cast<CComponent**>(&m_pColliderCom), &Desc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CDungeonGate::Bind_ResourceData()
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

void CDungeonGate::Change_Animation(const _float& fTimeDelta)
{
	if (m_CurrentState == GATE_OPEN) return;
	// 방 하나가 끝나면 게이트 오픈.
	// 게이트 애니메이션 수정하기

	//문에 가까이 가면 열림 m_iPathIndex == 2 일 경우
	if (m_iPathIndex == 2)
	{
		CTransform* pPlayerTransformCom = dynamic_cast<CTransform*>(m_pGameInstance->Get_GameObject_Component(LEVEL_GAMEPLAY, TEXT("Layer_Player"), TEXT("Com_Transform"), 0));

		_vector vDistance = XMVector3Length(pPlayerTransformCom->Get_State(CTransform::STATE_POSITION) - m_pTransformCom->Get_State(CTransform::STATE_POSITION));
		_float fDistance = XMVectorGetX(vDistance);

		if (fDistance < 2.f)
		{
			m_CurrentState = GATE_OPEN;
			m_pModelCom->Set_Animation(CModel::ANIM_DESC{ 2, false, false, false });
		}
	}

	//2. 몬스터를 다 죽여야 열림
	if (m_iPathIndex == 2 || m_iPathIndex == 4 || m_iPathIndex == 5)
		return;

	vector<CGameObject*> Monsters = m_pGameInstance->Get_GameObjects(LEVEL_GAMEPLAY, TEXT("Layer_Monster"));

	_uint iCount = 0;
	for (auto* pMonster : Monsters)
		if (m_iPathIndex == pMonster->Get_PathIndex())
			iCount++;

	if (iCount == 0)
	{
		m_CurrentState = GATE_OPEN;
		m_pModelCom->Set_Animation(CModel::ANIM_DESC{ 2, false, false, false });
	}
}

CDungeonGate* CDungeonGate::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CDungeonGate* pInstance = new CDungeonGate(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
		Safe_Release(pInstance);

	return pInstance;
}

CGameObject* CDungeonGate::Clone(void* pArg)
{
	CDungeonGate* pInstance = new CDungeonGate(*this);

	if (FAILED(pInstance->Initialize(pArg)))
		Safe_Release(pInstance);

	return pInstance;
}

void CDungeonGate::Free()
{
	__super::Free();
}
