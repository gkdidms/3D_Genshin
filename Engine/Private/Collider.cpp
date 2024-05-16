#include "Collider.h"

#include "GameInstance.h"

#include "Bounding_AABB.h"
#include "Bounding_OBB.h"

CCollider::CCollider(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CComponent{ pDevice, pContext }
{
}

CCollider::CCollider(const CCollider& rhs)
	: CComponent{ rhs },
	m_pBatch { rhs.m_pBatch },
	m_pShader { rhs.m_pShader },
	m_pInputLayout { rhs.m_pInputLayout }
{
	Safe_AddRef(m_pInputLayout);
}

HRESULT CCollider::Initialize_Prototype()
{
#ifdef _DEBUG
	m_pBatch = new PrimitiveBatch<VertexPositionColor>(m_pContext);
	m_pShader = new BasicEffect(m_pDevice);

	m_pShader->SetVertexColorEnabled(true);

	const void* pShaderByteCode = { nullptr };
	size_t		iShaderByteCodeLength = { 0 };

	m_pShader->GetVertexShaderBytecode(&pShaderByteCode, &iShaderByteCodeLength);

	m_pDevice->CreateInputLayout(VertexPositionColor::InputElements, VertexPositionColor::InputElementCount, pShaderByteCode, iShaderByteCodeLength, &m_pInputLayout);
#endif
	return S_OK;
}

HRESULT CCollider::Initialize(void* pArg)
{
	if (nullptr == pArg)
		return E_FAIL;

	COLLIDER_DESC* pDesc = static_cast<COLLIDER_DESC*>(pArg);

	m_ColliderType = pDesc->eType;

	switch (m_ColliderType)
	{
	case COLLIDER_AABB:
		m_pCurrentBounding = CBounding_AABB::Create(pArg);
		break;
	case COLLIDER_OBB:
		m_pCurrentBounding = CBounding_OBB::Create(pArg);
		break;
	case COLLIDER_SPHERE:
		break;
	case COLLIDER_END:
		break;
	default:
		break;
	}

	if (nullptr == m_pCurrentBounding)
		return E_FAIL;

	return S_OK;
}

void CCollider::Tick(_fmatrix WorldMatirx)
{
	m_pCurrentBounding->Tick(WorldMatirx);
}

_bool CCollider::Intersect(CCollider* pTargetCollider)
{
	return m_isColl = m_pCurrentBounding->Intersect(pTargetCollider->m_ColliderType, pTargetCollider->m_pCurrentBounding);
}

void CCollider::Compute_Rank(_char strRank, _char strTargetRank, CTransform* pTransform, CTransform* pTargetTransform, class CNavigation* pNavigation, class CNavigation* pTargetNavigation, const float& fTimeDelta)
{
	_matrix MoveMatrix = XMMatrixIdentity();

	_vector vPos = pTransform->Get_State(CTransform::STATE_POSITION);
	_vector vTargetPos = pTargetTransform->Get_State(CTransform::STATE_POSITION);

	// A < B = true; 아스키코드로 비교함.
	if (strRank < strTargetRank)
	{
		//타켓이 B라면
		_vector vSour = XMVector3Normalize(vTargetPos - vPos);

		vTargetPos += vSour * 3.f * fTimeDelta;
		pTargetTransform->Go_Coll(vTargetPos, pTargetNavigation);
	}
	else
	{
		// 주체 객체가 B이거나 같으면
		_vector vSour = XMVector3Normalize(vPos - vTargetPos);

		vPos += vSour * 3.f * fTimeDelta;
		pTransform->Go_Coll(vPos, pNavigation);
	}
}

#ifdef _DEBUG
HRESULT CCollider::Render()
{
	m_pShader->SetWorld(XMMatrixIdentity());
	m_pShader->SetView(m_pGameInstance->Get_Transform_Matrix(CPipeLine::D3DTS_VIEW));
	m_pShader->SetProjection(m_pGameInstance->Get_Transform_Matrix(CPipeLine::D3DTS_PROJ));

	m_pContext->IASetInputLayout(m_pInputLayout);

	m_pShader->Apply(m_pContext);

	m_pBatch->Begin();

	m_pCurrentBounding->Render(m_pBatch);

	m_pBatch->End();

	return S_OK;
}
#endif // _DEBUG

CCollider* CCollider::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CCollider* pInstance = new CCollider(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
		Safe_Release(pInstance);

	return pInstance;
}

CComponent* CCollider::Clone(void* pArg)
{
	CCollider* pInstance = new CCollider(*this);

	if (FAILED(pInstance->Initialize(pArg)))
		Safe_Release(pInstance);

	return pInstance;
}

void CCollider::Free()
{
	__super::Free();
#ifdef _DEBUG
	if (false == m_isCloned)
	{
		Safe_Delete(m_pBatch);
		Safe_Delete(m_pShader);
	}

	Safe_Release(m_pInputLayout);
#endif

	Safe_Release(m_pCurrentBounding);
}
