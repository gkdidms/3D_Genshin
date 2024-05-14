#pragma once
#include "SceneObj.h"

BEGIN(Client)
class CThornWall :
    public CSceneObj
{
private:
    CThornWall(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    CThornWall(const CThornWall& rhs);
    virtual ~CThornWall() = default;

public:
    virtual HRESULT Initialize_Prototype() override;
    virtual HRESULT Initialize(void* pArg) override;
    virtual void Priority_Tick(const _float& fTimeDelta) override;
    virtual void Tick(const _float& fTimeDelta) override;
    virtual void Late_Tick(const _float& fTimeDelta) override;
    virtual HRESULT Render() override;

private:
    virtual HRESULT Add_Components() override;
    virtual HRESULT Bind_ResourceData() override;
    virtual void Change_Animation(const _float& fTimeDelta) override;

public:
    static CThornWall* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    virtual CGameObject* Clone(void* pArg);
    virtual void Free() override;

};
END

