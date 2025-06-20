#include "Operator.h"

#include "GameInstance.h"

COperator::COperator(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CSceneObj{ pDevice, pContext }
{
}

COperator::COperator(const COperator& rhs)
	: CSceneObj{ rhs }
{
}

HRESULT COperator::Initialize_Prototype()
{
	return S_OK;
}

HRESULT COperator::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	m_State = CLOSE;
	m_pModelCom->Set_Animation(CModel::ANIM_DESC{ 0, false, false ,false });

	return S_OK;
}

void COperator::Priority_Tick(const _float& fTimeDelta)
{
}

void COperator::Tick(const _float& fTimeDelta)
{
	Change_Animation(fTimeDelta);

	_float4x4 MoveMatrix;
	m_pModelCom->Play_Animation(fTimeDelta, &MoveMatrix);

	m_pColliderCom->Tick(m_pTransformCom->Get_WorldMatrix());
}

void COperator::Late_Tick(const _float& fTimeDelta)
{
	m_pGameInstance->Add_Renderer(CRenderer::RENDER_NONBLENDER, this);
}

HRESULT COperator::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

#ifdef _DEBUG
	m_pColliderCom->Render();
#endif // _DEBUG

	return S_OK;
}

HRESULT COperator::Add_Components()
{
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, L"Prototype_Component_Shader_VtxAnimMesh", L"Com_Shader", reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, L"Prototype_Component_Model_Operator", L"Com_Model", reinterpret_cast<CComponent**>(&m_pModelCom))))
		return E_FAIL;

	CBounding_AABB::BOUNDING_AABB_DESC Desc{};

	Desc.eType = CCollider::COLLIDER_AABB;
	Desc.vExtents = _float3(0.6f, 0.5f, 0.6f);
	Desc.vCenter = _float3(0.f, Desc.vExtents.y, 0.f);

	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, L"Prototype_Component_Collider", L"Com_Collider", reinterpret_cast<CComponent**>(&m_pColliderCom), &Desc)))
		return E_FAIL;

	return S_OK;
}

HRESULT COperator::Bind_ResourceData()
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

void COperator::Change_Animation(const _float& fTimeDelta)
{
	if (m_pModelCom->Get_Animation_Finished(2))
	{
		m_State = OPEN_EFF;
		m_pModelCom->Set_Animation(CModel::ANIM_DESC{ 2, false, false, false });
	}

	CCollider* pPlayerCollider = dynamic_cast<CCollider*>(m_pGameInstance->Get_GameObject_Component(LEVEL_GAMEPLAY, L"Layer_Player", L"Com_Collider"));
	if (m_pColliderCom->Intersect(pPlayerCollider)) // 플레이어와 충돌 시
	{
		if (m_pGameInstance->GetKeyState(DIK_F) == CInput_Device::TAP)
		{
			if (m_State == CLOSE)
			{
				m_State = OPEN;
				m_pModelCom->Set_Animation(CModel::ANIM_DESC{ 3, false, false, false });
			}
		}
	}
}

COperator* COperator::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	COperator* pInstance = new COperator(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
		Safe_Release(pInstance);

	return pInstance;
}

CGameObject* COperator::Clone(void* pArg)
{
	COperator* pInstance = new COperator(*this);

	if (FAILED(pInstance->Initialize(pArg)))
		Safe_Release(pInstance);

	return pInstance;
}

void COperator::Free()
{
	__super::Free();
}
