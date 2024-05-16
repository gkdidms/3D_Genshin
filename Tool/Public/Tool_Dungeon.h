#pragma once
#include "GameObject.h"

#include "Tool_Defines.h"

#include "GameInstance.h"

BEGIN(Tool)
class  CTool_Dungeon final:
    public CGameObject
{
public:
    typedef struct tToolDungeonDesc : public CGameObject::GAMEOBJECT_DESC
    {
        char pObjectName[MAX_PATH];
        _float4 vPosition;

        wstring strPrototypeVIBufferCom = L"";
        wstring strComVIBufferCom = L"";
    }DUNGEON_DESC;

    typedef struct tToolCellDesc
    {
        _float3 Points[3];
        _int iOption;
        _int iIndex;
    }TOOL_CELL_DESC;

private:
    CTool_Dungeon(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    CTool_Dungeon(const CTool_Dungeon& rhs);
    virtual ~CTool_Dungeon() = default;

public:
    string Get_ObjectName() { return m_strObjectName; }
    const vector<TOOL_CELL_DESC> Get_Cells() { return m_Cells; }

public:
    void Set_Cells(vector<TOOL_CELL_DESC> Cells);

public:
    virtual HRESULT Initialize_Prototype() override;
    virtual HRESULT Initialize(void* pArg) override;
    virtual void Priority_Tick(const _float& fTimeDelta) override;
    virtual void Tick(const _float& fTimeDelta) override;
    virtual void Late_Tick(const _float& fTimeDelta) override;
    virtual HRESULT Render() override;

private:
    class CTool_Object_Manager* m_pObject_Manager = { nullptr };
    class CTool_Manager* m_pTool_Manager = { nullptr };

    CModel* m_pVIBufferCom = { nullptr };
    CShader* m_pShaderCom = { nullptr };
    CNavigation* m_pNavigationCom = { nullptr };

    string m_strObjectName = { "" };

    wstring m_strPrototypeVIBufferName = { L"" };
    wstring m_strComVIBufferName = { L"" };

private:
    _float3 m_Points[3] = {};
    _uint m_iPointCount = { 0 };

    vector<TOOL_CELL_DESC> m_Cells;

private:
    HRESULT Add_Components();
    HRESULT Bind_Resource();

    void Get_MousePos_On_Dungeon();
    void Picking_PlayerPos();
    void Picking_Cell();

    void Check_Point(_float3* vPoint);
    _bool isRemove();

public:
    static CTool_Dungeon* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    virtual CGameObject* Clone(void* pArg) override;
    virtual void Free() override;
};

END