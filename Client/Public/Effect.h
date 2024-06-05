#pragma once
#include "BlendObject.h"

#include "Client_Defines.h"

#include "EffectObject.h"

BEGIN(Engine)
class CCollider;
END

BEGIN(Client)
class CEffect:
    public CBlendObject
{
public:
    typedef struct tEffectDesc
    {
        const _float4x4* pPlayerMatrix; // 생성한 플레이어의 월드행렬
        _bool isFollowPlayer = { false }; // 플레이어를 따라오는가?
        _bool isBullet = { false }; // 총알인가?
        _float fDuration = { 0.f }; // 지속시간
        _float fSpeed = { 0.f };

        _vector vTargetDir = {};// 타겟이 있다면 타겟의 방향
    } EFFECT_DESC;

private:
    CEffect(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    CEffect(const CEffect& rhs);
    virtual ~CEffect() = default;

public:
    virtual HRESULT Initialize_Prototype(const _char* szFileName);
    virtual HRESULT Initialize(void* pArg) override;
    virtual void Priority_Tick(const _float& fTimeDelta) override;
    virtual void Tick(const _float& fTimeDelta) override;
    virtual void Late_Tick(const _float& fTimeDelta) override;
    virtual HRESULT Render() override;

private:
    CCollider* m_pColliderCom = { nullptr };

private:
    const _float4x4* m_pParentMatrix;
    vector<CGameObject*> m_EffectObjects;
    vector<CGameObject*> m_CloneEffectObject;
    _float4x4 m_ParentMatrix = {};

    _float m_fDutation = {0.f};
    _float m_fCurrentTime = { 0.f };

    _bool m_isFollowPlayer = { false };
    _bool m_isBullet = { false };
    _vector m_vTargetDir = {};

    _float m_fSpeed = {0.f};

private:
    HRESULT Add_Components();
    HRESULT File_Open(const _char* szFileName);

public:
    static CEffect* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const _char* szFilePath);
    virtual CGameObject* Clone(void* pArg);
    virtual void Free() override;
};
END
