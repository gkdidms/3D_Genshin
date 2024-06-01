#pragma once
#include "GameObject.h"

#include "EffectTool_Defines.h"

BEGIN(Engine)
class CModel;
class CShader;
END

BEGIN(Effect)
class CEffect_Anim_Model :
    public CGameObject
{
public:
    typedef struct tEffectAnimModel {
        string strModelDataFilePath;
        string strModelFilePath;
    } EFFECT_ANIM_MODEL;

private:
    CEffect_Anim_Model(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    CEffect_Anim_Model(const CEffect_Anim_Model& rhs);
    virtual ~CEffect_Anim_Model() = default;

public:
    void Set_ModelAnimIndex(_uint iIndex);

public:
    HRESULT Initialize_Prototype();
    HRESULT Initialize(void* pArg);
    void Priority_Tick(const _float& fTimeDelta);
    void Tick(const _float& fTimeDelta);
    void Late_Tick(const _float& fTimeDelta);
    HRESULT Render();

private:
    CModel* m_pModelCom = { nullptr };
    CShader* m_pShaderCom = { nullptr };

    string m_strModelFilePath = { "" };
    string m_strModelDataFilePath = { "" };

private:
    HRESULT Add_Components();
    HRESULT Bind_ResourceData();

public:
    static CEffect_Anim_Model* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    virtual CGameObject* Clone(void* pArg);
    virtual void Free() override;
};

END