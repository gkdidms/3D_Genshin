#include "TreasureBox.h"

#include "GameInstance.h"
#include "Item.h"

CTreasureBox::CTreasureBox(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CSceneObj{ pDevice, pContext }
{
}

CTreasureBox::CTreasureBox(const CTreasureBox& rhs)
	: CSceneObj{ rhs }
{
}

HRESULT CTreasureBox::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CTreasureBox::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	m_pModelCom->Set_Animation(CModel::ANIM_DESC{ 0, false, false ,false });

	m_State = BOX_CLOSE;

	return S_OK;
}

void CTreasureBox::Priority_Tick(const _float& fTimeDelta)
{
}

void CTreasureBox::Tick(const _float& fTimeDelta)
{
	CCollider* pPlayerCollider = dynamic_cast<CCollider*>(m_pGameInstance->Get_GameObject_Component(LEVEL_GAMEPLAY, L"Layer_Player", L"Com_Collider"));
	if (m_pColliderCom->Intersect(pPlayerCollider)) // 플레이어와 충돌 시
	{
		if (m_State != BOX_OPEN)
		{
			if (m_pGameInstance->GetKeyState(DIK_F) == CInput_Device::TAP)
			{
				m_pModelCom->Set_Animation(CModel::ANIM_DESC{ 1, false, false, false });
				m_State = BOX_OPEN;

				if (FAILED(Create_Item()))
					return;
			}
		}
	}

	_float4x4 MoveMatrix;
	m_pModelCom->Play_Animation(fTimeDelta, &MoveMatrix);

	m_pColliderCom->Tick(m_pTransformCom->Get_WorldMatrix());
}

void CTreasureBox::Late_Tick(const _float& fTimeDelta)
{
	m_pGameInstance->Add_Renderer(CRenderer::RENDER_NONBLENDER, this);
}

HRESULT CTreasureBox::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

#ifdef _DEBUG
	m_pColliderCom->Render();
#endif // _DEBUG

	return S_OK;
}

HRESULT CTreasureBox::Add_Components()
{
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, L"Prototype_Component_Shader_VtxAnimMesh", L"Com_Shader", reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, L"Prototype_Component_Model_TreasureBox", L"Com_Model", reinterpret_cast<CComponent**>(&m_pModelCom))))
		return E_FAIL;

	CBounding_AABB::BOUNDING_AABB_DESC Desc{};

	Desc.eType = CCollider::COLLIDER_AABB;
	Desc.vExtents = _float3(0.6f, 0.5f, 0.6f);
	Desc.vCenter = _float3(0.f, Desc.vExtents.y, 0.f);

	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, L"Prototype_Component_Collider", L"Com_Collider", reinterpret_cast<CComponent**>(&m_pColliderCom), &Desc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CTreasureBox::Bind_ResourceData()
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

void CTreasureBox::Change_Animation(const _float& fTimeDelta)
{
}

HRESULT CTreasureBox::Create_Item()
{
	srand(unsigned(time(NULL)));

	_int iNum = rand() % 3 + 4;

	for (size_t i = 0; i < iNum; ++i)
	{
		CItem::ITEM_DESC ItemDesc{};
		ItemDesc.fHeight = XMVectorGetY(m_pTransformCom->Get_State(CTransform::STATE_POSITION)) + 0.2f;
		ItemDesc.fSpeedPecSec = 7.f;
		ItemDesc.fPower = 4.f;
		ItemDesc.CreateMatrix = *m_pTransformCom->Get_WorldFloat4x4();
		if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, L"Prototype_GameObject_SceneObj_Item", L"Layer_Item", &ItemDesc)))
			return E_FAIL;
	}

	return S_OK;
}

CTreasureBox* CTreasureBox::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CTreasureBox* pInstance = new CTreasureBox(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
		Safe_Release(pInstance);

	return pInstance;
}

CGameObject* CTreasureBox::Clone(void* pArg)
{
	CGameObject* pInstance = new CTreasureBox(*this);

	if (FAILED(pInstance->Initialize(pArg)))
		Safe_Release(pInstance);

	return pInstance;
}

void CTreasureBox::Free()
{
	__super::Free();
}
