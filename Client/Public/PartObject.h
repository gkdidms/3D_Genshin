#pragma once
#include "GameObject.h"

#include "GameInstance.h"
#include "Client_Defines.h"
#include "Player.h"

//BEGIN(Engine)
//class CShader;
//class CModel;
//END

BEGIN(Client)
class CPartObject abstract :
    public CGameObject
{
public:
    typedef struct tPartObjDesc: public CGameObject::GAMEOBJECT_DESC
    {
        const _float4x4* pParentMatrix;
        _uint* pState;
    }PART_DESC;

protected:
    CPartObject(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    CPartObject(const CPartObject& rhs);
    virtual ~CPartObject() = default;

public:
    void Set_PlayerPos(_float4x4* vParentPos) { *vParentPos = m_PlayerMovePos; }
   
public:
    const _float4x4* Get_CameraLook() const { return m_pModelCom->Get_BoneCombinedTransformationMatrix("CameraLook"); }
    CModel* Get_Model() { return m_pModelCom; }

public:
    virtual HRESULT Initialize_Prototype();
    virtual HRESULT Initialize(void* pArg);
    virtual void Priority_Tick(const _float& fTimeDelta);
    virtual void Tick(const _float& fTimeDelta);
    virtual void Late_Tick(const _float& fTimeDelta);
    virtual HRESULT Render();

protected:
    class CState_Manager* m_pState_Manager = { nullptr };
    CShader* m_pShaderCom = { nullptr };
    CModel* m_pModelCom = { nullptr };
   
    _float4x4 m_pWorldMatrix;

protected:
    const _float4x4* m_pParentMatrix = { nullptr };
    _uint* m_pState = { nullptr };

protected:
    _uint m_iAnim = { 0 };
    _bool m_IsLoop = { false };
    _bool m_IsLinear = { false };
    _bool m_IsLinearSpeed = { false };
    _int m_iPreAnim = { -1 };

protected:
    _float4 m_vCurrentPos;
    _float4 m_vPrePos;
    _float4x4 m_PlayerMovePos;

private:
    virtual void Change_Animation(const _float& fTimeDelta) {};

protected:
    virtual CGameObject* Clone(void* pArg) = 0;
    virtual void Free() override;
};
END