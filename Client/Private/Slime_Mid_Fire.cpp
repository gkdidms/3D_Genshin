#include "Slime_Mid_Fire.h"

#include "GameInstance.h"

CSlime_Mid_FIre::CSlime_Mid_FIre(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CSlime_Large{ pDevice, pContext}
{
}

CSlime_Mid_FIre::CSlime_Mid_FIre(const CSlime_Mid_FIre& rhs)
	: CSlime_Large{ rhs }
{
}

HRESULT CSlime_Mid_FIre::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CSlime_Mid_FIre::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	return S_OK;
}

void CSlime_Mid_FIre::Priority_Tick(const _float& fTimeDelta)
{
}

void CSlime_Mid_FIre::Tick(const _float& fTimeDelta)
{
	Change_Animation(fTimeDelta);

	_float4x4 MoveMatrix;
	m_pModelCom->Play_Animation(fTimeDelta, &MoveMatrix);
}

void CSlime_Mid_FIre::Late_Tick(const _float& fTimeDelta)
{
	m_pGameInstance->Add_Renderer(CRenderer::RENDER_NONBLENDER, this);
}

HRESULT CSlime_Mid_FIre::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	return S_OK;
}

HRESULT CSlime_Mid_FIre::Add_Components()
{
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, L"Prototype_Component_Shader_VtxAnimMesh", L"Com_Shader", reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, L"Prototype_Component_Model_Nilou", L"Com_Model", reinterpret_cast<CComponent**>(&m_pModelCom))))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, L"Prototype_Component_Collider", L"Com_Collider", reinterpret_cast<CComponent**>(&m_pColliderCom))))
		return E_FAIL;

	return S_OK;
}

HRESULT CSlime_Mid_FIre::Bind_ResourceData()
{
	if (FAILED(__super::Bind_ResourceData()))
		return E_FAIL;

	return S_OK;
}

void CSlime_Mid_FIre::Change_Animation(const _float& fTimeDelta)
{
}

CSlime_Mid_FIre* CSlime_Mid_FIre::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CSlime_Mid_FIre* pInstance = new CSlime_Mid_FIre(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
		Safe_Release(pInstance);

	return pInstance;
}

CGameObject* CSlime_Mid_FIre::Clone(void* pArg)
{
	CSlime_Mid_FIre* pInstance = new CSlime_Mid_FIre(*this);

	if (FAILED(pInstance->Initialize(pArg)))
		Safe_Release(pInstance);

	return pInstance;
}

void CSlime_Mid_FIre::Free()
{
	__super::Free();
}
