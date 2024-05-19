#pragma once
#include "UI.h"

BEGIN(Client)
class CSkillBtn_Icon :
    public CUI
{
public:
    typedef struct tSkillBtnIconDesc : public CGameObject::GAMEOBJECT_DESC {
        wstring strComponentPrototypeTag;
        _float4x4 ParentMatrix;
        _float fSizeX;
        _float fSizeY;
    } SKILLBTN_ICON_DESC;

private:
    CSkillBtn_Icon(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    CSkillBtn_Icon(const CSkillBtn_Icon& rhs);
    virtual ~CSkillBtn_Icon() = default;

public:
    virtual HRESULT Initialize_Prototype() override;
    virtual HRESULT Initialize(void* pArg) override;
    virtual void Priority_Tick(const _float& fTimeDelta) override;
    virtual void Tick(const _float& fTimeDelta) override;
    virtual void Late_Tick(const _float& fTimeDelta) override;
    virtual HRESULT Render() override;

private:
    wstring m_strComponentPrototypeTag = {L""};
    _float4x4 m_ParentMatrix = {};

private:
    _float4x4 m_WorldMatrix = {};

private:
    virtual HRESULT Add_Components();
    virtual HRESULT Bind_ResourceData();

public:
    static CSkillBtn_Icon* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    virtual CGameObject* Clone(void* pArg);
    virtual void Free() override;
};
END
