#pragma once
#include "C:\Users\yeeun\dev\std\Genshin_Project\EngineSDK\Inc\GameObject.h"

#include "Client_Defines.h"

BEGIN(Engine)
class CVIBuffer_Rect;
class CShader;
class CTexture;
END

BEGIN(Client)
class CAlertCircle :
    public CGameObject
{
public:
    typedef struct tAlertCircleDesc : public CGameObject::GAMEOBJECT_DESC {
        _vector vCreatePos;
    } ALERTCIRCLE_DESC;

private:
    CAlertCircle(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    CAlertCircle(const CAlertCircle& rhs);
    virtual ~CAlertCircle() = default;

public:
    virtual HRESULT Initialize_Prototype() override;
    virtual HRESULT Initialize(void* pArg) override;
    virtual void Priority_Tick(const _float& fTimeDelta) override;
    virtual void Tick(const _float& fTimeDelta) override;
    virtual void Late_Tick(const _float& fTimeDelta) override;
    virtual HRESULT Render() override;

private:
    CShader* m_pShaderCom = { nullptr };
    CVIBuffer_Rect* m_pVIBufferCom = { nullptr };
    CTexture* m_pTextureCom = { nullptr };

private:
    _float m_fMaxSize = { 5.f };
    _float m_fSpeed = { 5.f };

private:
    HRESULT Add_Components();
    HRESULT Bind_ResourceData();
    void Change_Animation();

public:
    static CAlertCircle* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    virtual CGameObject* Clone(void* pArg) override;
    virtual void Free() override;
};
END
