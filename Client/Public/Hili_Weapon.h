#pragma once
#include "GameObject.h"

#include "Client_Defines.h"

BEGIN(Engine)
class CShader;
class CModel;
class CCollider;
END

BEGIN(Client)
class CHili_Weapon :
  public CGameObject
{

public:
	typedef struct tHiliWeaponDesc : public CGameObject::GAMEOBJECT_DESC{
		const _float4x4* pTargetCombinedTransformationMatrix;
		const _float4x4* pHandCombinedTransformationMatrix;
		const _uint* pState;
	}HILI_WEAPON_DESC;

protected:
	CHili_Weapon(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CHili_Weapon(const CHili_Weapon& rhs);
	virtual ~CHili_Weapon() = default;

public:
	HRESULT Initialize_Prototype() override;
	HRESULT Initialize(void* pArg) override;
	void Priority_Tick(const _float& fTimeDelta) override;
	void Tick(const _float& fTimeDelta) override;
	void Late_Tick(const _float& fTimeDelta) override;
	HRESULT Render() override;

protected:
	CShader* m_pShaderCom = { nullptr };
	CModel* m_pModelCom = { nullptr };
	CCollider* m_pColliderCom = { nullptr };
	const _uint* m_pState = { nullptr };

	const _float4x4* m_TargetMatrix;
	const _float4x4* m_SocketMatrix;
	_float4x4 m_WorldMatrix = {};

private:
	HRESULT Bind_ResourceData();
	HRESULT Add_Components();

public:
	virtual void Free() override;
};

END