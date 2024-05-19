#pragma once
#include "SkillObj.h"

/*
���ӽð� : 14��
����ð� : 4��

���� Ÿ�������� �ֺ��� �ִ� ������ �ڵ����� ����Ѵ�. 
*/

BEGIN(Client)
class CFoxTower :
    public CSkillObj
{
private:
    CFoxTower(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    CFoxTower(const CFoxTower& rhs);
    virtual ~CFoxTower() = default;

public:
    virtual HRESULT Initialize_Prototype();
    virtual HRESULT Initialize(void* pArg);
    virtual void Priority_Tick(const _float& fTimeDelta);
    virtual void Tick(const _float& fTimeDelta);
    virtual void Late_Tick(const _float& fTimeDelta);
    virtual HRESULT Render();

private:
    _float m_fDuration = { 14.f };
    _float m_fCurrentTime = { 0.f };

private:
    HRESULT Add_Components() override;
    HRESULT Bind_ResourceData() override;
    void Change_Animation(const _float& fTimeDelta) override;

public:
    static CFoxTower* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    virtual CGameObject* Clone(void* pArg) override;
    virtual void Free() override;
};
END
