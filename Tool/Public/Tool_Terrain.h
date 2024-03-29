#pragma once
#include "GameObject.h"

#include "Tool_Defines.h"

BEGIN(Engine)
class CShader;
class CVIBuffer_Terrain;
class CTexture;
class CCalculator;
END

BEGIN(Tool)
class CTool_Terrain :
    public CGameObject
{
private:
    CTool_Terrain(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    CTool_Terrain(const CTool_Terrain& rhs);
    virtual ~CTool_Terrain() = default;

public:
    virtual HRESULT Initialize_Prototype();
    virtual HRESULT Initialize(void* pArg);
    virtual void Priority_Tick(const _float& fTimeDelta);
    virtual void Tick(const _float& fTimeDelta);
    virtual void Late_Tick(const _float& fTimeDelta);
    virtual HRESULT Render();

private:
    CVIBuffer_Terrain* m_pVIBufferCom = { nullptr };
    CShader* m_pShaderCom = { nullptr };
    CTexture* m_pTextureCom = { nullptr };
    CCalculator* m_pCalculatorCom = { nullptr };

    class CTool_Object_Manager* m_pObject_Manager = { nullptr };
    class CTool_Manager* m_pTool_Manager = { nullptr };

private:
    HRESULT Add_Components();
    HRESULT Bind_ResouceData();

    void Get_MousePos_On_Terrain();

public:
    static CTool_Terrain* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    virtual CGameObject* Clone(void* pArg) override;
    virtual void Free() override;
};
END
