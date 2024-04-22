#pragma once
#include "Component.h"

BEGIN(Engine)
class ENGINE_DLL CNavigation :
    public CComponent
{
private:
    CNavigation(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    CNavigation(const CNavigation& rhs);
    virtual ~CNavigation() = default;

public:
    void Set_Points(const _float3* vPoints);

public:
    virtual HRESULT Initialize_Prototype(); // Tool용
    virtual HRESULT Initialize_Prototype(const wstring strFilePath);
    virtual HRESULT Initialize(void* pArg) override;
    virtual void Tick() override;

#ifdef _DEBUG
public:
    HRESULT Render();
#endif // _DEBUG

private:
    class CShader* m_pShaderCom = { nullptr };
    vector<class CCell*> m_Cells; // 삼각형들의 집합 저장

    _uint m_iIndexCount = { 0 };

private:
    HRESULT Load_File(const wstring strFilePath);

public:
    static CNavigation* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring strFilePath);
    static CNavigation* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext); // Tool용
    virtual CComponent* Clone(void* pArg) override;
    virtual void Free() override;
};

END