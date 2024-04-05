#pragma once
#include "Tool_Object.h"

#include "Tool_Defines.h"

BEGIN(Engine)
class CModel;
class CShader;
END

BEGIN(Tool)
class CTool_Anim_Object :
    public CTool_Object
{
private:
    CTool_Anim_Object(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    CTool_Anim_Object(const CTool_Anim_Object& rhs);
    virtual ~CTool_Anim_Object() = default;

public:
    HRESULT Initialize_Prototype();
    HRESULT Initialize(void* pArg);
    void Priority_Tick(const _float& fTimeDelta);
    void Tick(const _float& fTimeDelta);
    void Late_Tick(const _float& fTimeDelta);
    HRESULT Render();

private:
    HRESULT Add_Components();
    HRESULT Bind_ResourceData();

public:
    static CTool_Anim_Object* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    virtual CGameObject* Clone(void* pArg);
    virtual void Free() override;
};

END