#include "Slime_Mid_Water.h"

#include "GameInstance.h"

CSlime_Mid_Water::CSlime_Mid_Water(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CSlime_Large{ pDevice, pContext}
{
}

CSlime_Mid_Water::CSlime_Mid_Water(const CSlime_Mid_Water& rhs)
	: CSlime_Large{ rhs }
{
}

HRESULT CSlime_Mid_Water::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CSlime_Mid_Water::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	return S_OK;
}

void CSlime_Mid_Water::Priority_Tick(const _float& fTimeDelta)
{
}

void CSlime_Mid_Water::Tick(const _float& fTimeDelta)
{
	Change_Animation(fTimeDelta);

	_float4x4 MoveMatrix;
	m_pModelCom->Play_Animation(fTimeDelta, &MoveMatrix);
}

void CSlime_Mid_Water::Late_Tick(const _float& fTimeDelta)
{
	m_pGameInstance->Add_Renderer(CRenderer::RENDER_NONBLENDER, this);
}

HRESULT CSlime_Mid_Water::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	return S_OK;
}

HRESULT CSlime_Mid_Water::Add_Components()
{
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, L"Prototype_Component_Shader_VtxAnimMesh", L"Com_Shader", reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, L"Prototype_Component_Model_Nilou", L"Com_Model", reinterpret_cast<CComponent**>(&m_pModelCom))))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, L"Prototype_Component_Collider", L"Com_Collider", reinterpret_cast<CComponent**>(&m_pColliderCom))))
		return E_FAIL;

	return S_OK;
}

HRESULT CSlime_Mid_Water::Bind_ResourceData()
{
	if (FAILED(__super::Bind_ResourceData()))
		return E_FAIL;

	return S_OK;
}

void CSlime_Mid_Water::Change_Animation(const _float& fTimeDelta)
{
}

CSlime_Mid_Water* CSlime_Mid_Water::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CSlime_Mid_Water* pInstance = new CSlime_Mid_Water(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
		Safe_Release(pInstance);

	return pInstance;
}

CGameObject* CSlime_Mid_Water::Clone(void* pArg)
{
	CSlime_Mid_Water* pInstance = new CSlime_Mid_Water(*this);

	if (FAILED(pInstance->Initialize(pArg)))
		Safe_Release(pInstance);

	return pInstance;
}

void CSlime_Mid_Water::Free()
{
	__super::Free();
}
