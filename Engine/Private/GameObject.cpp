#include "GameObject.h"

#include "GameInstance.h"
#include "Component.h"

CGameObject::CGameObject(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: m_pDevice {pDevice},
	m_pContext {pContext},
	m_pGameInstance {CGameInstance::GetInstance()}
{
	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pContext);
	Safe_AddRef(m_pGameInstance);
}

CGameObject::CGameObject(const CGameObject& rhs)
	: m_pDevice{rhs.m_pDevice},
	m_pContext{rhs.m_pContext},
	m_pGameInstance{rhs.m_pGameInstance}
{
	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pContext);
	Safe_AddRef(m_pGameInstance);
}

HRESULT CGameObject::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CGameObject::Initialize(void* pArg)
{
	m_pTransformCom = CTransform::Create(m_pDevice, m_pContext);
	if (nullptr == m_pTransformCom)
		return E_FAIL;

	if (FAILED(m_pTransformCom->Initialize(pArg)))
		return E_FAIL;

	m_Components.emplace(L"Com_Transform", m_pTransformCom);

	return S_OK;
}

void CGameObject::Priority_Tick(const _float& fTimeDelta)
{
}

void CGameObject::Tick(const _float& fTimeDelta)
{
}

void CGameObject::Late_Tick(const _float& fTimeDelta)
{
}

HRESULT CGameObject::Render()
{
	return S_OK;
}

CComponent* CGameObject::Get_Component(wstring strComponentTag)
{
	CComponent* pComponent = Find_Component(strComponentTag);

	if (nullptr == pComponent)
		return nullptr;

	return pComponent;
}

HRESULT CGameObject::Add_Component(_uint iLevelIndex, const wstring strComponentPrototypeTag, const wstring strComponentTag, class CComponent** pComponent, void* pArg)
{
	if (nullptr != Find_Component(strComponentTag))
		return E_FAIL;

	*pComponent = m_pGameInstance->Add_Component_Clone(iLevelIndex, strComponentPrototypeTag, pArg);
	if (nullptr == *pComponent)
		return E_FAIL;

	m_Components.emplace(strComponentTag, *pComponent);

	Safe_AddRef(*pComponent);

	return S_OK;
}

CComponent* CGameObject::Find_Component(const wstring StrComponentTag)
{
	auto Pair = m_Components.find(StrComponentTag);

	if (m_Components.end() == Pair)
		return nullptr;

	return Pair->second;
}

void CGameObject::Free()
{
	for (auto& Pair : m_Components)
		Safe_Release(Pair.second);
	m_Components.clear();

	Safe_Release(m_pDevice);
	Safe_Release(m_pContext);
	Safe_Release(m_pGameInstance);
	Safe_Release(m_pTransformCom);
}
