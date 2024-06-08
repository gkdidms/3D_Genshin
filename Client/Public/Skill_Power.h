#pragma once
#include "C:\Users\yeeun\dev\std\Genshin_Project\EngineSDK\Inc\GameObject.h"
#include "Client_Defines.h"

BEGIN(Engine)
class CModel;
END

BEGIN(Client)
class CSkill_Power :
    public CGameObject
{
public:
    typedef struct tSkillPowerDesc {
        _matrix ParentMatrix;
    } SKILL_POWER_DESC;
private:
    CSkill_Power(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    CSkill_Power(const CSkill_Power& rhs);
    virtual ~CSkill_Power() = default;

public:
    virtual HRESULT Initialize_Prototype() override;
    virtual HRESULT Initialize(void* pArg) override;
    virtual void Priority_Tick(const _float& fTimeDelta) override;
    virtual void Tick(const _float& fTimeDelta) override;
    virtual void Late_Tick(const _float& fTimeDelta) override;
    virtual HRESULT Render() override;

private:
    CShader* m_pShaderCom = { nullptr };
    CModel* m_pModelCom = { nullptr };

    _float m_fAccelTime = { 0.f };
    _float m_fPower = { 5.f };
    _float4 m_vTargetLook = {};
    _float m_fSpeed = { 10.f };
    _float m_fHeight = { 0.f };

private:
    HRESULT Add_Components();
    HRESULT Bind_ResourceData();

public:
    static CSkill_Power* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    virtual CGameObject* Clone(void* pArg);
    virtual void Free() override;
};
END
