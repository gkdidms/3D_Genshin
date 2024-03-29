#pragma once
#include "Component.h"

BEGIN(Engine)
class ENGINE_DLL CCalculator :
    public CComponent
{
private:
    CCalculator(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    CCalculator(const CCalculator& rhs);
    virtual ~CCalculator() = default;

public:
    HRESULT Initialize_Prototype() override;
    HRESULT Initialize(void* pArg) override;

public:
    _bool Picking_OnTerrain(_fvector vMouseCurserPos, class CTransform* pTransformCom, class CVIBuffer_Terrain* pTerrainVIBuffer, const _ulong& dwCntX, const _ulong& dwCntZ, _float3* vWorldMouse);
    _bool Picking_OnMesh(_fvector vMouseCurserPos, class CTransform* pTransformCom, class CMesh* pMeshBuffer, const _ulong& dwCntVertices, _float3* vWorldMouse);

private:
    class CGameInstance* m_pGameInstance = { nullptr };

public:
    static CCalculator* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    virtual CComponent* Clone(void* pArg) override;
    virtual void Free() override;
};

END