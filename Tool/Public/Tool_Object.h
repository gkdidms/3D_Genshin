#pragma once
#include "GameObject.h"

#include "Tool_Defines.h"

BEGIN(Engine)
class CModel;
class CShader;
END

BEGIN(Tool)
class CTool_Object:
    public CGameObject
{
public:
    typedef struct tToolObjectDesc : public CGameObject::GAMEOBJECT_DESC
    {
        _char pObjectName[MAX_PATH];
        _float4 vPosition;
    }OBJECT_DESC;

private:
    CTool_Object(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    CTool_Object(const CTool_Object& rhs);
    virtual ~CTool_Object() = default;

public:
    char* Get_ObjectName() { return m_strObjectName; }

public:
    HRESULT Initialize_Prototype();
    HRESULT Initialize(void* pArg);
    void Priority_Tick(const _float& fTimeDelta);
    void Tick(const _float& fTimeDelta);
    void Late_Tick(const _float& fTimeDelta);
    HRESULT Render();

private:
    CModel* m_pVIBufferCom = { nullptr };
    CShader* m_pShaderCom = { nullptr };

    char m_strObjectName[MAX_PATH] = {""};

private:
    HRESULT Add_Components();
    HRESULT Bind_ResourceData();

public:
    static CTool_Object* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    virtual CGameObject* Clone(void* pArg);
    virtual void Free() override;
};
END
