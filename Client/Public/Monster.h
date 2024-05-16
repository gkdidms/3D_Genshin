#pragma once
#include "GameObject.h"

#include "Client_Defines.h"

BEGIN(Engine)
class CShader;
class CModel;
class CCollider;
END

BEGIN(Client)
class CMonster abstract:
    public CGameObject
{
public:
    typedef struct tMonsterDesc : public CGameObject::GAMEOBJECT_DESC
    {
        _int iMonsterNavigationIndex;
        const _float4x4* TargetMatrix;
        _float4x4 WorldMatrix;
    } MONSTER_DESC;

protected:
    CMonster(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    CMonster(const CMonster& rhs);
    virtual ~CMonster() = default;

public:
    CCollider* Get_Coll() { return m_pColliderCom; }

public:
    HRESULT Initialize_Prototype() override;
    HRESULT Initialize(void* pArg) override;
    void Priority_Tick(const _float& fTimeDelta) override;
    void Tick(const _float& fTimeDelta) override;
    void Late_Tick(const _float& fTimeDelta) override;
    HRESULT Render() override;

protected:
    virtual HRESULT Add_Components() = 0;
    virtual HRESULT Bind_ResourceData() = 0;
    virtual void Change_Animation(const _float& fTimeDelta) = 0;

protected:
    CShader* m_pShaderCom = { nullptr };
    CModel* m_pModelCom = { nullptr };
    CCollider* m_pColliderCom = { nullptr };
    CNavigation* m_pNavigation = { nullptr };

protected:
    _int m_iMonsterNavigationIndex = { 0 };
    const _float4x4* m_pTargetMatrix = { nullptr };
    _bool m_isColl = { false };
    
public:
    virtual void Free() override;
};
END
