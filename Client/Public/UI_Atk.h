#pragma once
#include "UI.h"

BEGIN(Engine)
class CCollider;
class CShader;
class CTexture;
class CVIBuffer_Rect;
END
BEGIN(Client)
class CUI_Atk :
    public CUI
{
public:
    typedef struct tAtkDesc {
        const _float4x4* pParentMatrix;
        _float fAtk;
        _vector vColor;
    } UI_ATK_DESC;

private:
    CUI_Atk(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    CUI_Atk(const CUI_Atk& rhs);
    virtual ~CUI_Atk() = default;

public:
    virtual HRESULT Initialize_Prototype() override;
    virtual HRESULT Initialize(void* pArg) override;
    virtual void Priority_Tick(const _float& fTimeDelta) override;
    virtual void Tick(const _float& fTimeDelta) override;
    virtual void Late_Tick(const _float& fTimeDelta) override;
    virtual HRESULT Render() override;

private:
    CCollider* m_pColliderCom = { nullptr };
    const _float4x4* m_pParentMatrix = { nullptr };
    _float m_fAtk = { 0.f };
    _vector m_vColor = {};

private:
    _float m_fSpeed = { 3.f };
    _float m_fDuration = { 0.6f };
    _float m_fCurrentTime = { 0.f };

private:
    virtual HRESULT Add_Components();
    virtual HRESULT Bind_ResourceData();

public:
    static CUI_Atk* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    virtual CGameObject* Clone(void* pArg) override;
    virtual void Free() override;
};

END