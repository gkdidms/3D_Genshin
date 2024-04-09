#pragma once

#include "Transform.h"

BEGIN(Engine)
class ENGINE_DLL CGameObject abstract :
    public CBase
{
public:
    typedef struct tGameObjectDesc: public CTransform::TRANSFORM_DESC
    {

    } GAMEOBJECT_DESC;

protected:
    CGameObject(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    CGameObject(const CGameObject& rhs);
    virtual ~CGameObject() = default;

public:
    class CTransform* m_pTransformCom = { nullptr };

public:
    virtual HRESULT Initialize_Prototype();
    virtual HRESULT Initialize(void* pArg);
    virtual void Priority_Tick(const _float& fTimeDelta);
    virtual void Tick(const _float& fTimeDelta);
    virtual void Late_Tick(const _float& fTimeDelta);
    virtual HRESULT Render();

public:
    class CComponent* Get_Component(wstring strComponentTag);

protected:
    ID3D11Device* m_pDevice = { nullptr };
    ID3D11DeviceContext* m_pContext = { nullptr };
    class CGameInstance* m_pGameInstance = { nullptr };

protected:
    map<const wstring, class CComponent*> m_Components;

protected:
    HRESULT Add_Component(_uint iLevelIndex, const wstring strComponentPrototypeTag, const wstring strComponentTag, class CComponent** pComponent, void* pArg = nullptr);

private:
    class CComponent* Find_Component(const wstring StrComponentTag);

public:
    virtual CGameObject* Clone(void* pArg) = 0;
    virtual void Free() override;
};

END

