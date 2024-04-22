#include "SkillObj.h"

CSkillObj::CSkillObj(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    : CPartObject{ pDevice, pContext }
{
}

CSkillObj::CSkillObj(const CSkillObj& rhs)
    : CPartObject{ rhs }
{
}

HRESULT CSkillObj::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CSkillObj::Initialize(void* pArg)
{
    if (nullptr == pArg)
        return E_FAIL;

    SKILLOBJ_DESC* pDesc = static_cast<SKILLOBJ_DESC*>(pArg);
    m_pSocketMatrix = pDesc->pHandCombinedTransformationMatrix;

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

    return S_OK;
}

void CSkillObj::Priority_Tick(const _float& fTimeDelta)
{
}

void CSkillObj::Tick(const _float& fTimeDelta)
{
}

void CSkillObj::Late_Tick(const _float& fTimeDelta)
{
}

HRESULT CSkillObj::Render()
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


void CSkillObj::Free()
{
    __super::Free();
}
