#pragma once
#include "Level.h"

#include "Tool_Defines.h"

BEGIN(Tool)
class CMain_Level :
    public CLevel
{
private:
    CMain_Level(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    virtual ~CMain_Level() = default;

public:
    HRESULT Initialize();
    void Tick(const _float& fTimeDelta);
    void Render();

private:
    HRESULT Ready_Layer_Camera(const wstring strName);
    HRESULT Ready_Layer_Terrain(const wstring strName);
    HRESULT Ready_Layer_Object(const wstring strName);

public:
    static CMain_Level* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    virtual void Free();
};
END
