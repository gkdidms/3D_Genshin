#include "Yae_Body.h"

#include "GameInstance.h"

CYae_Body::CYae_Body(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CPartObject_Body{ pDevice, pContext } 
{
}

CYae_Body::CYae_Body(const CYae_Body& rhs)
	: CPartObject_Body{ rhs }
{
}

HRESULT CYae_Body::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CYae_Body::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Add_Components()))
		return E_FAIL;

	m_pModelCom->Set_Animation(CModel::ANIM_DESC{ 48, true });
	return S_OK;
}

void CYae_Body::Priority_Tick(const _float& fTimeDelta)
{
}

void CYae_Body::Tick(const _float& fTimeDelta)
{
	__super::Tick(fTimeDelta);

	if (m_pModelCom->Get_Animation_Finished() && m_iAnim != 35)
	{
		//if (*m_pState == CPlayer::PLAYER_ATTACK_1 || *m_pState == CPlayer::PLAYER_ATTACK_2 || *m_pState == CPlayer::PLAYER_ATTACK_3 || *m_pState == CPlayer::PLAYER_ATTACK_4 ||
		//	*m_pState == CPlayer::PLAYER_ELEMENTAL_END || *m_pState == CPlayer::PLAYER_ELENENTAL_BURST_END)
		//{
		//	*m_pState = CPlayer::PLAYER_ATTACK_IDLE;
		//}
		//else if (*m_pState == CPlayer::PLAYER_ELEMENTAL_1)
		//{
		//	*m_pState = CPlayer::PLAYER_ELEMENTAL_END;
		//}
		//else if (*m_pState == CPlayer::PLAYER_ELENENTAL_BURST)
		//{
		//	*m_pState = CPlayer::PLAYER_ELENENTAL_BURST_END;
		//}
		//else
		//	*m_pState = CPlayer::PLAYER_IDLE;
	}

	Change_Animation(fTimeDelta);

	m_pModelCom->Play_Animation(fTimeDelta, &m_vCurrentPos, m_IsLoop);

	XMStoreFloat4(&m_PlayerMovePos, XMLoadFloat4(&m_vCurrentPos) - XMLoadFloat4(&m_vPrePos));
	m_PlayerMovePos.w = 1.f;
	m_vPrePos = m_vCurrentPos;
}

void CYae_Body::Late_Tick(const _float& fTimeDelta)
{
	XMStoreFloat4x4(&m_pWorldMatrix, m_pTransformCom->Get_WorldMatrix() * XMLoadFloat4x4(m_pParentMatrix));

	m_pGameInstance->Add_Renderer(CRenderer::RENDER_NONBLENDER, this);
}

HRESULT CYae_Body::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	return S_OK;
}

HRESULT CYae_Body::Add_Components()
{
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, L"Prototype_Component_Shader_VtxAnimMesh", L"Com_Shader", reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, L"Prototype_Component_Model_Yae", L"Com_Model", reinterpret_cast<CComponent**>(&m_pModelCom))))
		return E_FAIL;

	return S_OK;
}

HRESULT CYae_Body::Bind_ResourceData()
{
	if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_pWorldMatrix)))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", m_pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_VIEW))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", m_pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_PROJ))))
		return E_FAIL;

	return S_OK;
}

void CYae_Body::Change_Animation(const _float& fTimeDelta)
{
}

//void CYae_Body::Change_Animation(const _float& fTimeDelta)
//{
//	switch (*m_pState)
//	{
//	case CPlayer::PLAYER_ATTACK_1:
//	{
//		m_iAnim = 0;
//		m_IsLoop = false;
//		break;
//	}
//	case CPlayer::PLAYER_ATTACK_2:
//	{
//		m_iAnim = 2;
//		m_IsLoop = false;
//		break;
//	}
//	case CPlayer::PLAYER_ATTACK_3:
//	{
//		m_iAnim = 3;
//		m_IsLoop = false;
//		break;
//	}
//	case CPlayer::PLAYER_ATTACK_IDLE:
//	{
//		m_iAnim = 51;
//		m_IsLoop = false;
//		break;
//	}
//	case CPlayer::PLAYER_ELEMENTAL_1:
//	{
//		m_iAnim = 7;
//		m_IsLoop = false;
//		break;
//	}
//	case CPlayer::PLAYER_ELEMENTAL_END:
//	{
//		m_iAnim = 8;
//		m_IsLoop = false;
//		break;
//	}
//	case CPlayer::PLAYER_ELENENTAL_BURST:
//	{
//		m_iAnim = 11;
//		m_IsLoop = false;
//		break;
//	}
//	case CPlayer::PLAYER_ELENENTAL_BURST_END:
//	{
//		m_iAnim = 12;
//		m_IsLoop = false;
//		break;
//	}
//	case CPlayer::PLAYER_RUN:
//	{
//		m_iAnim = 35;
//		m_IsLoop = false;
//
//		if (m_PreState == *m_pState && m_pModelCom->Get_Animation_Finished(35))
//		{
//			m_iAnim = 36;
//			m_IsLoop = true;
//		}
//		break;
//	}
//	case CPlayer::PLAYER_STOP:
//	{
//		m_iAnim = 39;
//		m_IsLoop = false;
//		break;
//	}
//	case CPlayer::PLAYER_IDLE:
//	{
//		m_iAnim = 48;
//		m_IsLoop = true;
//		break;
//	}
//	default:
//		break;
//	}
//
//	if (m_pModelCom->Get_LoopAnimation_Finished() || m_iPreAnim != m_iAnim)
//		XMStoreFloat4(&m_vPrePos, XMVectorSet(0.f, 0.f, 0.f, 1.f));
//
//	m_pModelCom->Set_Animation(CModel::ANIM_DESC{ m_iAnim, m_IsLoop });
//
//	m_PreState = *m_pState;
//	m_iPreAnim = m_iAnim;
//
//}

CYae_Body* CYae_Body::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CYae_Body* pInstance = new CYae_Body(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
		Safe_Release(pInstance);

	return pInstance;
}

CGameObject* CYae_Body::Clone(void* pArg)
{
	CYae_Body* pInstance = new CYae_Body(*this);

	if (FAILED(pInstance->Initialize(pArg)))
		Safe_Release(pInstance);

	return pInstance;
}

void CYae_Body::Free()
{
	__super::Free();
}
