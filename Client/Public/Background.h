#pragma once
#include "GameObject.h"
#include "GameInstance.h"

#include "Client_Defines.h"

BEGIN(Client)
class CBackground :
    public CGameObject
{
public:
    typedef struct tBackgroundDesc {
        const _float* pProgress;
    } BACKGROUND_DESC;
private:
    CBackground(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    CBackground(const CBackground& rhs);
    virtual ~CBackground() = default;

public:
    virtual HRESULT Initialize_Prototype() override;
    virtual HRESULT Initialize(void* pArg) override;
    virtual void Priority_Tick(const _float& fTimeDelta) override;
    virtual void Tick(const _float& fTimeDelta) override;
    virtual void Late_Tick(const _float& fTimeDelta) override;
    virtual HRESULT Render() override;

private:
    CVIBuffer_Rect* m_pVIBufferCom = { nullptr };
    CShader* m_pShaderCom = { nullptr };
    CTexture* m_pTextureCom = { nullptr };
    class CLoadingBar* m_pLoadingBar = { nullptr };

    _float4x4 m_WorldMatrix, m_ViewMatrix, m_ProjMatrix;
    _float m_fSizeX, m_fSizeY, m_fX, m_fY;

private:
    const _float* m_pProgress = { nullptr };

private:
    HRESULT Add_Components();
    HRESULT Bind_Render();

public:
    static CBackground* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    virtual CGameObject* Clone(void* pArg);
    virtual void Free() override;
};
END
