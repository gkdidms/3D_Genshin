#pragma once
#include "Tool_Effect.h"


BEGIN(Engine)
class CModel;
class CTexture;
END

BEGIN(Effect)
class CEffect_Trail :
    public CTool_Effect
{
public:
    enum TYPE { NON_TYPE, NILOU_FlOWER, NOLOU_RING, TYPE_END };
    typedef struct tEffectMesh : public CTool_Effect::TOOL_EFFECT_DESC
    {
        _char szModelFilePath[MAX_PATH];
        _uint iTrailType = { 0 };
    } EFFECT_MESH_DESC;

private:
    CEffect_Trail(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    CEffect_Trail(const CEffect_Trail& rhs);
    virtual ~CEffect_Trail() = default;

public:
    _char* Get_MeshFilePath() { return m_szModelFilePath; }
    _uint Get_TrailMoveType() { return m_iTrailMoveType; }
    _float2 Get_UV() { return m_UV; }

public:
    void Set_UV(_float2 vUV) { m_UV = vUV; }
    void Set_TrailMoveType(_uint iType) { m_iTrailMoveType = iType; }

public:
    virtual HRESULT Initialize_Prototype() override;
    virtual HRESULT Initialize(void* pArg) override;
    virtual void Priority_Tick(const _float& fTimeDelta) override;
    virtual void Tick(const _float& fTimeDelta) override;
    virtual void Late_Tick(const _float& fTimeDelta) override;
    virtual HRESULT Render() override;

private:
    CModel* m_pModelCom = { nullptr };

    _char m_szModelFilePath[MAX_PATH] = {""};
    _uint m_iTrailMoveType = { 0 };

private:
    _float2 m_UV = {};

private:
    virtual HRESULT Add_Components();
    virtual HRESULT Bind_ResourceData();

public:
    static CEffect_Trail* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    virtual CGameObject* Clone(void* pArg);
    virtual void Free() override;
};
END
