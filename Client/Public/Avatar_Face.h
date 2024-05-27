#pragma once
#include "UI.h"

BEGIN(Client)
class CAvatar_Face :
    public CUI
{
public:
    typedef struct tAvatarFaceDesc : public CGameObject::GAMEOBJECT_DESC
    {
        _uint iPlayerble;
        _float4x4 ParentMatrix;
        _float2 vSize;
        _float2 vPos;
    }AVATAR_FACE_DESC;
private:
    CAvatar_Face(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    CAvatar_Face(const CAvatar_Face& rhs);
    virtual ~CAvatar_Face() = default;

public:
    virtual HRESULT Initialize_Prototype() override;
    virtual HRESULT Initialize(void* pArg) override;
    virtual void Priority_Tick(const _float& fTimeDelta) override;
    virtual void Tick(const _float& fTimeDelta) override;
    virtual void Late_Tick(const _float& fTimeDelta) override;
    virtual HRESULT Render() override;

private:
    _uint m_iPlayerble = { 0 };

private:
    virtual HRESULT Add_Components();
    virtual HRESULT Bind_ResourceData();

public:
    static CAvatar_Face* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    virtual CGameObject* Clone(void* pArg);
    virtual void Free() override;
};
END
