#include "SceneObj.h"

#include "GameInstance.h"

#include "Player.h"

CSceneObj::CSceneObj(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CGameObject{ pDevice, pContext}
{
}

CSceneObj::CSceneObj(const CSceneObj& rhs)
	: CGameObject{ rhs }
{
}

HRESULT CSceneObj::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CSceneObj::Initialize(void* pArg)
{
	if (nullptr == pArg)
		return E_FAIL;

	SCENEOBJ_DESC* pDesc = static_cast<SCENEOBJ_DESC*>(pArg);
	m_iNavigationIndex = pDesc->iNavigationIndex;

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Add_Components()))
		return E_FAIL;


	
	m_pTransformCom->Set_WorldMatrix(XMLoadFloat4x4(&pDesc->WorldMatrix));

	return S_OK;
}

void CSceneObj::Priority_Tick(const _float& fTimeDelta)
{
	CPlayer* pPlayer = dynamic_cast<CPlayer*>(m_pGameInstance->Get_GameObject(LEVEL_GAMEPLAY, TEXT("Layer_Player"), 0));

	CCollider* pPlayerCollider = dynamic_cast<CCollider*>(pPlayer->Get_Component(TEXT("Com_Collider")));

	if (m_pColliderCom->Intersect(pPlayerCollider))
	{
		pPlayer->Set_Coll(true);
	}
}

void CSceneObj::Tick(const _float& fTimeDelta)
{
}

void CSceneObj::Late_Tick(const _float& fTimeDelta)
{
}

HRESULT CSceneObj::Render()
{
	if (FAILED(Bind_ResourceData()))
		return E_FAIL;

	_uint iNumMeshes = m_pModelCom->Get_NumMeshes();

	for (int i = 0; i < iNumMeshes; ++i)
	{
		if (FAILED(m_pModelCom->Bind_BoneMatrices(m_pShaderCom, "g_BoneMatrices", i)))
			return E_FAIL;

		if (FAILED(m_pModelCom->Bind_Material(m_pShaderCom, "g_Texture", i, aiTextureType_DIFFUSE)))
			return E_FAIL;

		m_pShaderCom->Begin(0);
		m_pModelCom->Render(i);
	}

	return S_OK;
}

void CSceneObj::Free()
{
	__super::Free();

	Safe_Release(m_pShaderCom);
	Safe_Release(m_pModelCom);
	Safe_Release(m_pColliderCom);
}
