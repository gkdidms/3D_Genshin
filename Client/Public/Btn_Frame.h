#pragma once
#include "UI.h"

BEGIN(Client)
class CBtn_Frame :
    public CUI
{
public:
    typedef struct tBtnFrameDesc : public CGameObject::GAMEOBJECT_DESC
    {
        wstring strText;
        _float2 vPos;
        _float4x4 ParentMatrix;
    } BTN_FRAME_DESC;
private:
    CBtn_Frame(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    CBtn_Frame(const CBtn_Frame& rhs);
    virtual ~CBtn_Frame() = default;

public:
    virtual HRESULT Initialize_Prototype() override;
    virtual HRESULT Initialize(void* pArg) override;
    virtual void Priority_Tick(const _float& fTimeDelta) override;
    virtual void Tick(const _float& fTimeDelta) override;
    virtual void Late_Tick(const _float& fTimeDelta) override;
    virtual HRESULT Render() override;

private:
    wstring m_strText = { L"" };
    _float4x4 m_ParentMatrix = {};

private:
    virtual HRESULT Add_Components();
    virtual HRESULT Bind_ResourceData();

public:
    static CBtn_Frame* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    virtual CGameObject* Clone(void* pArg);
    virtual void Free() override;
};
END
