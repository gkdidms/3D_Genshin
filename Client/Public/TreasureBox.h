#pragma once
#include "SceneObj.h"

BEGIN(Client)
class CTreasureBox :
    public CSceneObj
{
public:
    enum STATE { BOX_OPEN, BOX_CLOSE, BOX_END };
private:
    CTreasureBox(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    CTreasureBox(const CTreasureBox& rhs);
    virtual ~CTreasureBox() = default;

public:
    virtual HRESULT Initialize_Prototype() override;
    virtual HRESULT Initialize(void* pArg) override;
    virtual void Priority_Tick(const _float& fTimeDelta) override;
    virtual void Tick(const _float& fTimeDelta) override;
    virtual void Late_Tick(const _float& fTimeDelta) override;
    virtual HRESULT Render() override;

private:
    STATE m_State = { BOX_END };

private:
    virtual HRESULT Add_Components() override;
    virtual HRESULT Bind_ResourceData() override;
    virtual void Change_Animation(const _float& fTimeDelta) override;

    HRESULT Create_Item();

public:
    static CTreasureBox* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    virtual CGameObject* Clone(void* pArg);
    virtual void Free();
};

END