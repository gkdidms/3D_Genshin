#pragma once
#include "Tool_Object.h"

#include "Tool_Defines.h"

BEGIN(Engine)
class CModel;
class CShader;
END

BEGIN(Tool)
class CEffect_Anim_Model :
    public CTool_Object
{
private:
    CEffect_Anim_Model(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    CEffect_Anim_Model(const CEffect_Anim_Model& rhs);
    virtual ~CEffect_Anim_Model() = default;

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
    static CEffect_Anim_Model* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    virtual CGameObject* Clone(void* pArg);
    virtual void Free() override;
};

END