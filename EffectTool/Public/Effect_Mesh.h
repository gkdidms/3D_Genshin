#pragma once
#include "Tool_Effect.h"


BEGIN(Engine)
class CModel;
END

BEGIN(Effect)
class CEffect_Mesh :
    public CTool_Effect
{
public:
    typedef struct tEffectMesh : public CTool_Effect::TOOL_EFFECT_DESC
    {
        _char szModelFilePath[MAX_PATH];
        string strModelFileName;
        _char strTextureFilePath[MAX_PATH];
        _int iNumTexture;
    } EFFECT_MESH_DESC;

private:
    CEffect_Mesh(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    CEffect_Mesh(const CEffect_Mesh& rhs);
    virtual ~CEffect_Mesh() = default;

public:
    wstring Get_TextureFilePath() { return m_strTextureFilePath; }
    _char* Get_MeshFilePath() { return m_szModelFilePath; }
    string Get_ModelFileName() { return m_strModelFileName; }
    _float2 Get_UV() { return m_UV; }
public:
    void Set_UV(_float2 vUV) { m_UV = vUV; }

public:
    virtual HRESULT Initialize_Prototype() override;
    virtual HRESULT Initialize(void* pArg) override;
    virtual void Priority_Tick(const _float& fTimeDelta) override;
    virtual void Tick(const _float& fTimeDelta) override;
    virtual void Late_Tick(const _float& fTimeDelta) override;
    virtual HRESULT Render() override;

private:
    class CModel* m_pModelCom = { nullptr };
    _char m_szModelFilePath[MAX_PATH] = {""};
    wstring m_strTextureFilePath = { L"" };
    string m_strModelFileName = { "" };
    _int m_iNumTexture = { 0 };

private:
    _float2 m_UV = {};


private:
    virtual HRESULT Add_Components();
    virtual HRESULT Bind_ResourceData();

public:
    static CEffect_Mesh* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    virtual CGameObject* Clone(void* pArg);
    virtual void Free() override;
};
END
