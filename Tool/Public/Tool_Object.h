#pragma once
#include "GameObject.h"

#include "Tool_Defines.h"

BEGIN(Engine)
class CModel;
class CShader;
END

BEGIN(Tool)
class CTool_Object abstract:
    public CGameObject
{
public:
    typedef struct tToolObjectDesc : public CGameObject::GAMEOBJECT_DESC
    {
        char pObjectName[MAX_PATH];
        _float4 vPosition;

        wstring strPrototypeVIBufferCom = L"";
        wstring strComVIBufferCom = L"";
        _uint iObjectNavigationIndex = 0;
    }OBJECT_DESC;

protected:
    CTool_Object(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    CTool_Object(const CTool_Object& rhs);
    virtual ~CTool_Object() = default;

public:
    string Get_ObjectName() { return m_strObjectName; }
    _uint Get_NavigationIndex() { return m_iObjectNavigationIndex; }

public:
    virtual HRESULT Initialize_Prototype();
    virtual HRESULT Initialize(void* pArg);
    virtual void Priority_Tick(const _float& fTimeDelta);
    virtual void Tick(const _float& fTimeDelta);
    virtual void Late_Tick(const _float& fTimeDelta);
    virtual HRESULT Render();

protected:
    class CTool_Object_Manager* m_pObjectManager = { nullptr };

    CModel* m_pVIBufferCom = { nullptr };
    CShader* m_pShaderCom = { nullptr };

    string m_strObjectName = { "" };

    wstring m_strPrototypeVIBufferName = { L"" };
    wstring m_strComVIBufferName = { L"" };

protected:
    _uint m_iObjectNavigationIndex = { 0 };

protected:
    virtual CGameObject* Clone(void* pArg) = 0;
    virtual void Free() override;
};

END