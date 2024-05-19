#pragma once
#include "Level.h"
#include "Client_Defines.h"

class CGamePlay_Level :
    public CLevel
{
private:
    CGamePlay_Level(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    virtual ~CGamePlay_Level() = default;

public:
    virtual HRESULT Initialize() override;
    virtual void Tick(const _float& fTimeDelta);
    virtual void Render();

private:
    HRESULT Ready_Layer_Camera(const wstring& strLayerTag);
    HRESULT Ready_Layer_BackGround(const wstring& strLayerTag, const char* pObjectName, void* pArg);
    HRESULT Ready_Layer_Player(const wstring& strLayerTag, void* pArg);
    HRESULT Ready_Layer_UI(const wstring& strLayerTag);
    HRESULT Ready_Object(const char* strName, _matrix WorldMatrix, _uint iNavigationIndex);

private:
    HRESULT Load_File(LEVEL_STATE eNextLevel);

public:
    static CGamePlay_Level* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    virtual void Free();
};

