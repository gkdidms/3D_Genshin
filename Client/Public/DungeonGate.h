#pragma once
#include "SceneObj.h"

BEGIN(Client)
class CDungeonGate :
    public CSceneObj
{
public:
    enum STATE { GATE_IDEL, GATE_OPEN, GATE_END, STATE_END };
private:
    CDungeonGate(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    CDungeonGate(const CDungeonGate& rhs);
    virtual ~CDungeonGate() = default;

public:
    virtual HRESULT Initialize_Prototype() override;
    virtual HRESULT Initialize(void* pArg) override;
    virtual void Priority_Tick(const _float& fTimeDelta) override;
    virtual void Tick(const _float& fTimeDelta) override;
    virtual void Late_Tick(const _float& fTimeDelta) override;
    virtual HRESULT Render() override;

private:
    STATE m_CurrentState = { STATE_END };

private:
    virtual HRESULT Add_Components() override;
    virtual HRESULT Bind_ResourceData() override;
    virtual void Change_Animation(const _float& fTimeDelta) override;

public:
    static CDungeonGate* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    virtual CGameObject* Clone(void* pArg) override;
    virtual void Free() override;
};
END
