#pragma once
#include "Base.h"

#include "Component.h"

#include "VIBuffer_Rect.h"
#include "VIBuffer_Terrain.h"
#include "Shader.h"
#include "Texture.h"
#include "Transform.h"
#include "Model.h"
#include "Calculator.h"

BEGIN(Engine)
class CComponent_Manager :
    public CBase
{
private:
    CComponent_Manager();
    virtual ~CComponent_Manager() = default;

public:
    HRESULT Initialize(_uint iMaxLevelIndex);
    HRESULT Add_Component_Prototype(_uint iLevelIndex, const wstring strComponentTag, CComponent* pComponent);
    CComponent* Add_Component_Clone(_uint iLevelIndex, const wstring strComponentTag, void* pArg);
    void Clear(_uint iLevelIndex);

private:
    CComponent* Find_Component_Prototype(_uint iLevelIndex, const wstring strComponentTag);

private:
    map<const wstring, class CComponent*>* m_Prototypes;

    _uint m_iMaxLevelIndex = { 0 };

public:
    static CComponent_Manager* Create(_uint iMaxLevelIndex);
    virtual void Free();
};
END
