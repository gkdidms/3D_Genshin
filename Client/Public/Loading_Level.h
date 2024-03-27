#pragma once

#include "Level.h"
#include "Client_Defines.h"

BEGIN(Client)

class CLoading_Level :
    public CLevel
{
private:
    CLoading_Level(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    virtual ~CLoading_Level() = default;

public:
    virtual HRESULT Initialize(LEVEL_STATE eNextLevel);
    virtual void Tick(const _float& fTimeDelta) override;
    virtual void Render() override;

private:
    LEVEL_STATE m_eNextLevel = { LEVEL_END };

    class CLoader* m_pLoader = { nullptr };

private:
    HRESULT Ready_Background(const wstring strLayerTag);

public:
    static CLoading_Level* Create(LEVEL_STATE eNextLevel, ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    virtual void Free() override;
};

END
