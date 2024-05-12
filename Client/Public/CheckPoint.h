#pragma once
#include "SceneObj.h"

BEGIN(Client)
class CCheckPoint :
    public CSceneObj
{
public:
    typedef struct tCheckPointDesc : public CSceneObj::SCENEOBJ_DESC {
        const _float4x4* pTargetMatrix;
    } CHECKPOINT_DESC;

private:
    CCheckPoint(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    CCheckPoint(const CCheckPoint& rhs);
    virtual ~CCheckPoint() = default;

public:
    virtual HRESULT Initialize_Prototype() override;
    virtual HRESULT Initialize(void* pArg) override;
    virtual void Priority_Tick(const _float& fTimeDelta) override;
    virtual void Tick(const _float& fTimeDelta) override;
    virtual void Late_Tick(const _float& fTimeDelta) override;
    virtual HRESULT Render() override;

private:
    const _float4x4* m_pTargetMatrix;
    _float m_fDistanceToTarget = { 7.f };

    _bool m_isCheckPoint = { false };

private:
    virtual HRESULT Add_Components();
    virtual HRESULT Bind_ResourceData();
    virtual void Change_Animation(const _float& fTimeDelta);

public:
    static CCheckPoint* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    virtual CGameObject* Clone(void* pArg)override;
    virtual void Free() override;
};
END
