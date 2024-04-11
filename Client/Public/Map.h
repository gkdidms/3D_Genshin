#pragma once
#include "GameObject.h"

#include "Client_Defines.h"

BEGIN(Engine)
class CShander;
class CModel;
END

BEGIN(Client)
class CMap abstract:
    public CGameObject
{
protected:
    CMap(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    CMap(const CMap& rhs);
    virtual ~CMap() = default;

public:
    virtual HRESULT Initialize_Prototype();
    virtual HRESULT Initialize(void* pArg);
    virtual void Priority_Tick(const _float& fTimeDelta);
    virtual void Tick(const _float& fTimeDelta);
    virtual void Late_Tick(const _float& fTimeDelta);
    virtual HRESULT Render();

protected:
    CShader* m_pShaderCom = { nullptr };
    CModel* m_pModelCom = { nullptr };

protected:
    virtual CGameObject* Clone(void* pArg) = 0;
    virtual void Free() override;
};

END