#pragma once
#include "SceneObj.h"

BEGIN(Client)
class CFireCore :
    public CSceneObj
{
public:
    enum FIRECORE_STATE { FIRECORE_START = 3, FIRECORE_REST = 2, FIRECORE_END = 1, STATE_END = 0};

    typedef struct tFireCoreDesc : public CSceneObj::SCENEOBJ_DESC {
        const _float4x4* pTargetMatrix;
    } FIRECORE_DESC;
private:
    CFireCore(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    CFireCore(const CFireCore& rhs);
    virtual ~CFireCore() = default;

public:
    virtual HRESULT Initialize_Prototype() override;
    virtual HRESULT Initialize(void* pArg) override;
    virtual void Priority_Tick(const _float& fTimeDelta) override;
    virtual void Tick(const _float& fTimeDelta) override;
    virtual void Late_Tick(const _float& fTimeDelta) override;
    virtual HRESULT Render() override;

private:
    const _float4x4* m_pTargetMatrix; 
    FIRECORE_STATE m_CurrentState = { STATE_END };

    _float m_fDistanceToTarget = { 10.f };

private:
    virtual HRESULT Add_Components() override;
    virtual HRESULT Bind_ResourceData() override;
    virtual void Change_Animation(const _float& fTimeDelta) override;
    
public:
    static CFireCore* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    virtual CGameObject* Clone(void* pArg);
    virtual void Free() override;
};
END
