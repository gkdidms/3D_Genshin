#pragma once
#include "Base.h"
class CCommand :
    public CBase
{
protected:
    CCommand();
    virtual ~CCommand() = default;

public:
    virtual void Execute(_uint iIndex) = 0; // ½ÇÇà
    virtual void Undo() = 0;

protected:
    _uint m_iIndex = 0;
    
public:
    virtual void Free() override;
};

