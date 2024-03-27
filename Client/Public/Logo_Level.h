#pragma once
#include "Level.h"
#include "Client_Defines.h"

BEGIN(Client)
class CLogo_Level :
    public CLevel
{
private:
    CLogo_Level(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    virtual ~CLogo_Level() = default;

public:
    virtual HRESULT Initialize() override;
    virtual void Tick(const _float& fTimeDelta) override;
    virtual void Render() override;

public:
    static CLogo_Level* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    virtual void Free();
};
END
