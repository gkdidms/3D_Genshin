#pragma once
#include "GameObject.h"

#include "Client_Defines.h"

BEGIN(Engine)
class CShader;
class CModel;
class CCollider;
class CNavigation;
END

BEGIN(Client)
class CItem :
    public CGameObject
{
private:
    CItem(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    CItem(const CItem& rhs);
    virtual ~CItem() = default;

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
    CNavigation* m_pNavigationCom = { nullptr };

private:
    HRESULT Add_Components();
    HRESULT Bind_ResourceData();

public:
    static CItem* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    virtual CGameObject* Clone(void* pArg);
    virtual void Free() override;
};
END
