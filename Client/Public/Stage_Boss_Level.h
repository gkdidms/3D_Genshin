#pragma once
#include "Level.h"

#include "Client_Defines.h"

BEGIN(Client)
class CStage_Boss_Level :
    public CLevel
{
private:
    CStage_Boss_Level(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    CStage_Boss_Level(const CStage_Boss_Level& rhs);
    virtual ~CStage_Boss_Level() = default;

public:
    virtual HRESULT Initialize() override;
    virtual void Tick(const _float& fTimeDelta);
    virtual void Render();

private:
    HRESULT Ready_Lights();
    HRESULT Ready_Layer_Camera(const wstring& strLayerTag);
    HRESULT Ready_Layer_BackGround(const wstring& strLayerTag, const char* pObjectName, void* pArg);
    HRESULT Ready_Layer_Player(const wstring& strLayerTag, void* pArg);
    HRESULT Ready_UI(const wstring& strLayerTag);
    HRESULT Ready_Object(const char* strName, _matrix WorldMatrix, _uint iNavigationIndex);

private:
    HRESULT Load_File();

public:
    static CStage_Boss_Level* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    virtual void Free() override;
};
END
