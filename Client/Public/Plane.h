#pragma once
#include "SceneObj.h"

BEGIN(Engine)
class CNavigation;
END

BEGIN(Client)
class CPlane :
    public CSceneObj
{
public:
    typedef struct tPlaneDesc : public CSceneObj::SCENEOBJ_DESC
    {
        _bool isMove;
    }PLANE_DESC;

private:
    CPlane(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    CPlane(const CPlane& rhs);
    virtual ~CPlane() = default;

public:
    virtual HRESULT Initialize_Prototype() override;
    virtual HRESULT Initialize(void* pArg) override;
    virtual void Priority_Tick(const _float& fTimeDelta) override;
    virtual void Tick(const _float& fTimeDelta) override;
    virtual void Late_Tick(const _float& fTimeDelta) override;
    virtual HRESULT Render() override;

private:
    CNavigation* m_pNavigationCom = { nullptr };

private:
    _float m_fSpeed = { 3.f };
    _bool m_isMove = { false };

private:
    virtual HRESULT Add_Components() override;
    virtual HRESULT Bind_ResourceData() override;
    virtual void Change_Animation(const _float& fTimeDelta) override;
    _bool Check_Coll();

public:
    static CPlane* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    virtual CGameObject* Clone(void* pArg);
    virtual void Free() override;
};
END
