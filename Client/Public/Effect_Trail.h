#pragma once
#include "EffectObject.h"

#include "Client_Defines.h"

BEGIN(Engine)
class CShader;
class CTexture;
class CModel;
END

BEGIN(Client)
class CEffect_Trail :
    public CEffectObject
{
public:
    enum TRAIL_TYPE { NON_TYPE, NILOU_FlOWER, NOLOU_RING, TRAIL_TYPE_END };
    typedef struct tEffectTrailDesc : public CEffectObject::EFFECT_OBJECT_DESC
    {
        _char strMeshFilePath[MAX_PATH];
        _uint iTrailType;
    }EFFECT_TRAIL_DESC;

private:
    CEffect_Trail(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    CEffect_Trail(const CEffect_Trail& rhs);
    virtual ~CEffect_Trail() = default;

public:
    virtual HRESULT Initialize_Prototype(void* pArg) override;
    virtual HRESULT Initialize(void* pArg) override;
    virtual void Priority_Tick(const _float& fTimeDelta) override;
    virtual void Tick(const _float& fTimeDelta) override;
    virtual void Late_Tick(const _float& fTimeDelta) override;
    virtual HRESULT Render() override;

private:
    _char m_szMeshTextureFilePath[MAX_PATH] = "";
    CModel* m_pModelCom = { nullptr };

    _uint m_iTrailType = { TRAIL_TYPE_END };

private:
    virtual HRESULT Add_Components();
    virtual HRESULT Bind_ResourceData();

public:
    static CEffect_Trail* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, void* pArg);
    virtual CGameObject* Clone(void* pArg);
    virtual void Free();
};

END