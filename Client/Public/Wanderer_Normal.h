#pragma once
#include "Player_Bullet.h"

BEGIN(Engine)
class CShader;
class CTexture;
class CModel;
class CCollider;
END

BEGIN(Client)
class CWanderer_Normal :
    public CPlayer_Bullet
{
public:
    enum DIR_STATE { DIR_RIGHT, DIR_LIFT, DIR_END };
    typedef struct tWandererNormalDesc : public CBullet::BULLET_DESC
    {
        _uint iDir;
    }WANDERER_NORMAL_DESC;
private:
    CWanderer_Normal(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    CWanderer_Normal(const CWanderer_Normal& rhs);
    virtual ~CWanderer_Normal() = default;

public:
    virtual HRESULT Initialize_Prototype() override;
    virtual HRESULT Initialize(void* pArg) override;
    virtual void Priority_Tick(const _float& fTimeDelta) override;
    virtual void Tick(const _float& fTimeDelta) override;
    virtual void Late_Tick(const _float& fTimeDelta) override;
    virtual HRESULT Render() override;

private:
    CShader* m_pShaderCom = { nullptr };
    CTexture* m_pTextureCom = { nullptr };
    CModel* m_pModelCom = { nullptr };



private:
    HRESULT Add_Components();
    HRESULT Bind_ResourceData();
    void Get_Look();

    
public:
    static CWanderer_Normal* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    virtual CGameObject* Clone(void* pArg);
    virtual void Free() override;
};
END
