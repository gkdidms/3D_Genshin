#include "Plane.h"

#include "GameInstance.h"

#include "Player.h"

CPlane::CPlane(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CSceneObj{ pDevice, pContext }
{
}

CPlane::CPlane(const CPlane& rhs)
	: CSceneObj { rhs }
{
}

HRESULT CPlane::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CPlane::Initialize(void* pArg)
{
	if (nullptr != pArg)
	{
		PLANE_DESC* pDesc = static_cast<PLANE_DESC*>(pArg);
		
		m_isMove = pDesc->isMove;
	}

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	return S_OK;
}

void CPlane::Priority_Tick(const _float& fTimeDelta)
{
	//충돌 하고 충돌체 높이에 탄다.
	if (Check_Coll())
	{
		//플레이어 높이 
		
	}
}

void CPlane::Tick(const _float& fTimeDelta)
{
	if (m_isMove)
	{
		// 움직이는 발판이면
		_matrix MoveMatrix;
		MoveMatrix.r[3] = XMVectorSet(m_fSpeed * fTimeDelta, 0.f, 0.f, 0.f);
		_vector vRadius = m_fSpeed > 0.f ? XMVectorSet(2.4f, 0.f, 0.f, 1.f) : XMVectorSet(-2.4f, 0.f, 0.f, 1.f);
		_bool isResult = m_pTransformCom->Check_Radius_AfterRun(MoveMatrix, m_pNavigationCom, vRadius);
		

		if (isResult == false) // 이동할 수 없을때마다 이동 방향을 변경
			m_fSpeed *= -1.f;
		if (isResult == true)
			m_pTransformCom->Go_Run(MoveMatrix, m_pNavigationCom);
	}

	m_pColliderCom->Tick(m_pTransformCom->Get_WorldMatrix());
}

void CPlane::Late_Tick(const _float& fTimeDelta)
{
	m_pGameInstance->Add_Renderer(CRenderer::RENDER_NONBLENDER, this);
}

HRESULT CPlane::Render()
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

HRESULT CPlane::Add_Components()
{
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, L"Prototype_Component_Shader_VtxMesh", L"Com_Shader", reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, L"Prototype_Component_Model_Plane", L"Com_Model", reinterpret_cast<CComponent**>(&m_pModelCom))))
		return E_FAIL;

	CBounding_AABB::BOUNDING_AABB_DESC Desc{};

	Desc.eType = CCollider::COLLIDER_AABB;
	Desc.vExtents = _float3(2.2f, 0.3f, 2.2f);
	Desc.vCenter = _float3(0.f, Desc.vExtents.y, 0.f);

	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, L"Prototype_Component_Collider", L"Com_Collider", reinterpret_cast<CComponent**>(&m_pColliderCom), &Desc)))
		return E_FAIL;

	CNavigation::NAVIGATION_DESC NavigationDesc{};
	NavigationDesc.iIndex = m_iNavigationIndex;

	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, L"Prototype_Component_Navigation", L"Com_Navigation", reinterpret_cast<CComponent**>(&m_pNavigationCom), &NavigationDesc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CPlane::Bind_ResourceData()
{
	if (FAILED(m_pTransformCom->Bind_ShaderMatrix(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", m_pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_VIEW))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", m_pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_PROJ))))
		return E_FAIL;

	return S_OK;
}

void CPlane::Change_Animation(const _float& fTimeDelta)
{
}

_bool CPlane::Check_Coll()
{
	CPlayer* pPlayer = dynamic_cast<CPlayer*>(m_pGameInstance->Get_GameObject(LEVEL_GAMEPLAY, TEXT("Layer_Player"), 0));
	CCollider* pPlayerColl = dynamic_cast<CCollider*>(pPlayer->Get_Component(TEXT("Com_Collider")));

	if (m_pColliderCom->Intersect(pPlayerColl))
	{
		// 플레이어가 위에 있는지 확인.
		_vector vPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
		_vector vPlayerPos = pPlayer->m_pTransformCom->Get_State(CTransform::STATE_POSITION); // 이러면 안되지 않나?
		_vector vUP = XMVector3Normalize(m_pTransformCom->Get_State(CTransform::STATE_UP));
		_vector vLookToPlayer = XMVector3Normalize(vPlayerPos - vPos);

		_float fAngle = XMConvertToDegrees(acosf(XMVectorGetX(XMVector3Dot(vUP, vLookToPlayer))));

		if (fAngle < 90.f)
		{
			_float fHeight = XMVectorGetY(vPos) + 0.3f;
			pPlayer->Set_Height(fHeight);
		}
	}

	return false;
}

CPlane* CPlane::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CPlane* pInstance = new CPlane(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
		Safe_Release(pInstance);

	return pInstance;
}

CGameObject* CPlane::Clone(void* pArg)
{
	CPlane* pInstance = new CPlane(*this);

	if (FAILED(pInstance->Initialize(pArg)))
		Safe_Release(pInstance);

	return pInstance;
}

void CPlane::Free()
{
	__super::Free();

	Safe_Release(m_pNavigationCom);
}
