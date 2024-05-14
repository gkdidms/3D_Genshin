#pragma once
#include "SceneObj.h"

BEGIN(Client)
class COperator :
    public CSceneObj
{
public:
    enum STATE { OPEN, CLOSE, OPEN_EFF, STATE_END };
private:
    COperator(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    COperator(const COperator& rhs);
    virtual ~COperator() = default;

public:
    virtual HRESULT Initialize_Prototype() override;
    virtual HRESULT Initialize(void* pArg) override;
    virtual void Priority_Tick(const _float& fTimeDelta) override;
    virtual void Tick(const _float& fTimeDelta) override;
    virtual void Late_Tick(const _float& fTimeDelta) override;
    virtual HRESULT Render() override;

private:
    STATE m_State = { STATE_END };

private:
    virtual HRESULT Add_Components() override;
    virtual HRESULT Bind_ResourceData() override;
    virtual void Change_Animation(const _float& fTimeDelta) override;

public:
    static COperator* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    virtual CGameObject* Clone(void* pArg) override;
    virtual void Free() override;
};
END
