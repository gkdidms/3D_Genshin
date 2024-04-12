#include "PartObject_Body.h"

#include "GameInstance.h"

CPartObject_Body::CPartObject_Body(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CPartObject{ pDevice, pContext }
{
}

CPartObject_Body::CPartObject_Body(const CPartObject_Body& rhs)
	: CPartObject{ rhs }
{
}

HRESULT CPartObject_Body::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CPartObject_Body::Initialize(void* pArg)
{
	BODY_DESC* pDesc = (BODY_DESC*)pArg;

	if (nullptr != pDesc)
	{
		m_strPrototypeModelTag = pDesc->strPrototypeModelTag;
	}

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Add_Components()))
		return E_FAIL;

	m_pModelCom->Set_Animation(CModel::ANIM_DESC{ 45, true });
	return S_OK;
}

void CPartObject_Body::Priority_Tick(const _float& fTimeDelta)
{
}

void CPartObject_Body::Tick(const _float& fTimeDelta)
{
	if (m_pModelCom->Get_Animation_Finished() && m_iAnim != 32)
	{
		if (*m_pState == CPlayer::PLAYER_ATTACK_1 || *m_pState == CPlayer::PLAYER_ATTACK_2 || *m_pState == CPlayer::PLAYER_ATTACK_3 || *m_pState == CPlayer::PLAYER_ATTACK_4 ||
			*m_pState == CPlayer::PLAYER_ELEMENTAL_END || *m_pState == CPlayer::PLAYER_ELENENTAL_BURST_END)
		{
			*m_pState = CPlayer::PLAYER_ATTACK_IDLE;
		}
		else if (*m_pState == CPlayer::PLAYER_ELEMENTAL_1)
		{
			*m_pState = CPlayer::PLAYER_ELEMENTAL_END;
		}
		else if (*m_pState == CPlayer::PLAYER_ELENENTAL_BURST)
		{
			*m_pState = CPlayer::PLAYER_ELENENTAL_BURST_END;
		}
		else
			*m_pState = CPlayer::PLAYER_IDLE;
	}

	Change_Animation(fTimeDelta);

	m_pModelCom->Play_Animation(fTimeDelta, &m_vCurrentPos, m_IsLoop);
	
	XMStoreFloat4(&m_PlayerMovePos, XMLoadFloat4(&m_vCurrentPos) - XMLoadFloat4(&m_vPrePos));
	m_PlayerMovePos.w = 1.f;
	m_vPrePos = m_vCurrentPos;
}

void CPartObject_Body::Late_Tick(const _float& fTimeDelta)
{
	XMStoreFloat4x4(&m_pWorldMatrix, m_pTransformCom->Get_WorldMatrix() * XMLoadFloat4x4(m_pParentMatrix));

	m_pGameInstance->Add_Renderer(CRenderer::RENDER_NONBLENDER, this);
}

HRESULT CPartObject_Body::Render()
{
	if (FAILED(Bind_ResourceData()))
		return E_FAIL;

	_uint iNumMeshes = m_pModelCom->Get_NumMeshes();

	for (int i = 0; i < iNumMeshes; ++i)
	{
		if (FAILED(m_pModelCom->Bind_BoneMatrices(m_pShaderCom, "g_BoneMatrices", i)))
			return E_FAIL;

		if (FAILED(m_pModelCom->Bind_Material(m_pShaderCom, "g_Texture", i, aiTextureType_DIFFUSE)))
			continue;

		m_pShaderCom->Begin(0);
		m_pModelCom->Render(i);
	}

	return S_OK;
}

HRESULT CPartObject_Body::Add_Components()
{
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, L"Prototype_Component_Shader_VtxAnimMesh", L"Component_Shader_VtxAnimMesh", reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, m_strPrototypeModelTag, L"Com_Model", reinterpret_cast<CComponent**>(&m_pModelCom))))
		return E_FAIL;

	return S_OK;
}

HRESULT CPartObject_Body::Bind_ResourceData()
{
	if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_pWorldMatrix)))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", m_pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_VIEW))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", m_pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_PROJ))))
		return E_FAIL;

	return S_OK;
}

void CPartObject_Body::Change_Animation(const _float& fTimeDelta)
{
	switch (*m_pState)
	{
	case CPlayer::PLAYER_ATTACK_1:
	{
		m_iAnim = 0;
		m_IsLoop = false;
		break;
	}
	case CPlayer::PLAYER_ATTACK_2:
	{
		m_iAnim = 1;
		m_IsLoop = false;
		break;
	}
	case CPlayer::PLAYER_ATTACK_3:
	{
		m_iAnim = 2;
		m_IsLoop = false;
		break;
	}
	case CPlayer::PLAYER_ATTACK_4:
	{
		m_iAnim = 3;
		m_IsLoop = false;
		break;
	}
	case CPlayer::PLAYER_ATTACK_IDLE:
	{
		m_iAnim = 47;
		m_IsLoop = false;
		break;
	}
	case CPlayer::PLAYER_ELEMENTAL_1:
	{
		m_iAnim = 4;
		m_IsLoop = false;
		break;
	}
	case CPlayer::PLAYER_ELEMENTAL_END:
	{
		m_iAnim = 5;
		m_IsLoop = false;
		break;
	}
	case CPlayer::PLAYER_ELENENTAL_BURST:
	{
		m_iAnim = 6;
		m_IsLoop = false;
		break;
	}
	case CPlayer::PLAYER_ELENENTAL_BURST_END:
	{
		m_iAnim = 7;
		m_IsLoop = false;
		break;
	}
	case CPlayer::PLAYER_RUN:
	{
		m_iAnim = 32;
		m_IsLoop = false;

		if (m_PreState == *m_pState && m_pModelCom->Get_Animation_Finished(32))
		{
			m_iAnim = 33;
			m_IsLoop = true;
		}
		break;
	}
	case CPlayer::PLAYER_STOP:
	{
		m_iAnim = 36;
		m_IsLoop = false;
		break;
	}
	case CPlayer::PLAYER_IDLE:
	{
		m_iAnim = 45;
		m_IsLoop = true;
		break;
	}
	default:
		break;
	}

	if (m_pModelCom->Get_LoopAnimation_Finished() || m_iPreAnim != m_iAnim)
		XMStoreFloat4(&m_vPrePos, XMVectorSet(0.f, 0.f, 0.f, 1.f));

	m_pModelCom->Set_Animation(CModel::ANIM_DESC{ m_iAnim, m_IsLoop });

	m_PreState = *m_pState;
	m_iPreAnim = m_iAnim;

}

CPartObject_Body* CPartObject_Body::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CPartObject_Body* pInstance = new CPartObject_Body(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
		Safe_Release(pInstance);

	return pInstance;
}

CGameObject* CPartObject_Body::Clone(void* pArg)
{
	CPartObject_Body* pInstance = new CPartObject_Body(*this);

	if (FAILED(pInstance->Initialize(pArg)))
		Safe_Release(pInstance);

	return pInstance;
}

void CPartObject_Body::Free()
{
	__super::Free();
}
