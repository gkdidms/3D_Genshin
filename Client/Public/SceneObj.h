#pragma once
#include "GameObject.h"

#include "Client_Defines.h"

BEGIN(Engine)
class CShader;
class CModel;
class CCollider;
END 

BEGIN(Client)
class CSceneObj :
    public CGameObject
{
public:
    typedef struct tSceneObjDesc : public CGameObject::GAMEOBJECT_DESC
    {
        _float4x4 WorldMatrix;
    } SCENEOBJ_DESC;

protected:
    CSceneObj(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    CSceneObj(const CSceneObj& rhs);
    virtual ~CSceneObj() = default;

public:
    virtual HRESULT Initialize_Prototype() override;
    virtual HRESULT Initialize(void* pArg) override;
    virtual void Priority_Tick(const _float& fTimeDelta) override;
    virtual void Tick(const _float& fTimeDelta) override;
    virtual void Late_Tick(const _float& fTimeDelta) override;
    virtual HRESULT Render() override;

protected:
    CShader* m_pShaderCom = { nullptr };
    CModel* m_pModelCom = { nullptr };
    CCollider* m_pColliderCom = { nullptr };

private:
    virtual HRESULT Add_Components() = 0;
    virtual HRESULT Bind_ResourceData() = 0;
    virtual void Change_Animation(const _float& fTimeDelta) = 0;

public:
    virtual void Free();
};

END