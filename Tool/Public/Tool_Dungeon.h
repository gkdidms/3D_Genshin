#pragma once
#include "GameObject.h"

#include "Tool_Defines.h"

#include "GameInstance.h"

BEGIN(Tool)
class  CTool_Dungeon final:
    public CGameObject
{
public:
    typedef struct tToolDungeonDesc : public CGameObject::GAMEOBJECT_DESC {
        char pName[MAX_PATH];
        wstring strPrototypeVIBufferCom{ L"" };
        wstring strComVIBufferCom{ L"" };
    }TOOL_DUNGEON_DESC;

private:
    CTool_Dungeon(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    CTool_Dungeon(const CTool_Dungeon& rhs);
    virtual ~CTool_Dungeon() = default;

public:
    HRESULT Initialize_Prototype() override;
    HRESULT Initialize(void* pArg) override;
    void Priority_Tick(const _float& fTimeDelta) override;
    void Tick(const _float& fTimeDelta) override;
    void Late_Tick(const _float& fTimeDelta) override;
    HRESULT Render() override;

private:
    CShader* m_pShaderCom = { nullptr };
    CModel* m_pModelCom = { nullptr };
    CCalculator* m_pCalculatorCom = { nullptr };

    class CTool_Manager* m_pTool_Manager = { nullptr };
    class CTool_Object_Manager* m_pObject_Manager = { nullptr };

    wstring m_strPrototypeVIBufferName = { L"" };
    wstring m_strComVIBufferName = { L"" };

private:
    HRESULT Add_Components();
    HRESULT Bind_Resource();

    void Get_MousePos_On_Dungeon();

public:
    static CTool_Dungeon* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    virtual CGameObject* Clone(void* pArg) override;
    virtual void Free() override;
};

END