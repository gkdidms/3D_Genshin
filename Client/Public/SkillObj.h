#pragma once
#include "PartObject.h"

BEGIN(Client)
class CSkillObj :
    public CPartObject
{
public:
    typedef struct tSkillObjDesc : public CPartObject::PART_DESC {
        const _float4x4* pHandCombinedTransformationMatrix;
    }SKILLOBJ_DESC;

protected:
    CSkillObj(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    CSkillObj(const CSkillObj& rhs);
    virtual ~CSkillObj() = default;

public:
    virtual HRESULT Initialize_Prototype();
    virtual HRESULT Initialize(void* pArg);
    virtual void Priority_Tick(const _float& fTimeDelta);
    virtual void Tick(const _float& fTimeDelta);
    virtual void Late_Tick(const _float& fTimeDelta);
    virtual HRESULT Render();

protected:
    const _float4x4* m_pSocketMatrix = { nullptr };

    _bool m_isHide = { false };

protected:
    virtual HRESULT Add_Components() = 0;
    virtual HRESULT Bind_ResourceData() = 0;
    virtual void Change_Animation(const _float& fTimeDelta) = 0;

protected:
    virtual CGameObject* Clone(void* pArg) = 0;
    virtual void Free();
};

END