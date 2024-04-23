#include "GameInstance.h"

#include "Graphic_Device.h"
#include "Input_Manager.h"
#include "Input_Device.h"
#include "Level_Manager.h"
#include "GameObject_Manager.h"
#include "Component_Manager.h"
#include "Timer_Manager.h"
#include "Renderer.h"
#include "Picking.h"

IMPLEMENT_SINGLETON(CGameInstance)

CGameInstance::CGameInstance()
{
}

HRESULT CGameInstance::Initialize_Engine(_uint iMaxLevelIndex, const ENGINE_DESC& Engine_Desc, ID3D11Device** ppDevice, ID3D11DeviceContext** ppContext)
{
	m_pGraphic_Device = CGraphic_Device::Create(Engine_Desc, ppDevice, ppContext);
	if (nullptr == m_pGraphic_Device)
		return E_FAIL;

	m_pTimer_Manager = CTimer_Manager::Create();
	if (nullptr == m_pTimer_Manager)
		return E_FAIL;

	m_pInput_Manager = CInput_Manager::Create(Engine_Desc.hWnd);
	if (nullptr == m_pInput_Manager)
		return E_FAIL;

	m_pInput_Device = CInput_Device::Create(Engine_Desc.hInst, Engine_Desc.hWnd);
	if (nullptr == m_pInput_Manager)
		return E_FAIL;

	m_pLevel_Manager = CLevel_Manager::Create(iMaxLevelIndex);
	if (nullptr == m_pLevel_Manager)
		return E_FAIL;

	m_pGameObject_Manager = CGameObject_Manager::Create(iMaxLevelIndex);
	if (nullptr == m_pGameObject_Manager)
		return E_FAIL;

	m_pRenderer = CRenderer::Create(iMaxLevelIndex);
	if (nullptr == m_pRenderer)
		return E_FAIL;

	m_pComponent_Manager = CComponent_Manager::Create(iMaxLevelIndex);
	if (nullptr == m_pComponent_Manager)
		return E_FAIL;

	m_pPipeLine = CPipeLine::Create();
	if (nullptr == m_pPipeLine)
		return E_FAIL;

	m_pPicking = CPicking::Create(*ppDevice, *ppContext, Engine_Desc.hWnd);
	if (nullptr == m_pPipeLine)
		return E_FAIL;

	return S_OK;
}

void CGameInstance::Tick(const _float& fTimeDelta)
{
	m_pInput_Device->Update_InputDev();
	m_pInput_Manager->Tick();

	m_pGameObject_Manager->Priority_Tick(fTimeDelta);

	m_pGameObject_Manager->Tick(fTimeDelta);

	m_pPipeLine->Tick();

	m_pGameObject_Manager->Late_Tick(fTimeDelta);

	m_pLevel_Manager->Tick(fTimeDelta);
}

void CGameInstance::Draw(const _float4 vClearColor)
{
	Clear_BackBuffer_View(vClearColor);
	Clear_DepthStencil_View();

	m_pRenderer->Draw();

	Present();
}

void CGameInstance::Draw_From_Tool()
{
	m_pRenderer->Draw();
}

HRESULT CGameInstance::Clear_BackBuffer_View(_float4 vClearColor)
{
	return m_pGraphic_Device->Clear_BackBuffer_View(vClearColor);
}

HRESULT CGameInstance::Clear_DepthStencil_View()
{
	return m_pGraphic_Device->Clear_DepthStencil_View();
}

HRESULT CGameInstance::Present()
{
	return m_pGraphic_Device->Present();
}

_float CGameInstance::Compute_ProjZ(const POINT& ptWindowPos)
{
	return m_pGraphic_Device->Compute_ProjZ(ptWindowPos);
}

HRESULT CGameInstance::Open_Level(_uint iLevelIndex, CLevel* pLevel)
{
	return m_pLevel_Manager->Open_Level(iLevelIndex, pLevel);
}

HRESULT CGameInstance::Add_GameObject_Prototype(const wstring strGameObjectTag, CGameObject* pGameObject)
{
	return m_pGameObject_Manager->Add_GameObject_Prototype(strGameObjectTag, pGameObject);
}

HRESULT CGameInstance::Add_GameObject(_uint iLevelIndex, const wstring strGameObjectTag, const wstring strLayerTag, void* pArg)
{
	return m_pGameObject_Manager->Add_GameObject(iLevelIndex, strGameObjectTag, strLayerTag, pArg);
}

CGameObject* CGameInstance::Clone_Object(const wstring strGameObjectTag, void* pArg)
{
	return m_pGameObject_Manager->Clone_Object(strGameObjectTag, pArg);
}

void CGameInstance::Clear_Object(_uint iLevelIndex)
{
	m_pGameObject_Manager->Clear(iLevelIndex);
	m_pComponent_Manager->Clear(iLevelIndex);
}

CComponent* CGameInstance::Get_GameObject_Component(_uint iLevelIndex, const wstring strLayerTag, const wstring strComponentTag, _uint iIndex)
{
	return m_pGameObject_Manager->Get_GameObject_Component(iLevelIndex, strLayerTag, strComponentTag, iIndex);
}

CGameObject* CGameInstance::Get_GameObject(_uint iLevelIndex, const wstring strLayerTag, _uint iIndex)
{
	return m_pGameObject_Manager->Get_GameObject(iLevelIndex, strLayerTag, iIndex);
}

void CGameInstance::Add_Renderer(CRenderer::RENDERER_STATE eRenderState, CGameObject* pGameObject)
{
	m_pRenderer->Add_Renderer(eRenderState, pGameObject);
}

HRESULT CGameInstance::Add_Component_Prototype(_uint iLevelIndex, const wstring strComponentTag, CComponent* pComponent)
{
	return m_pComponent_Manager->Add_Component_Prototype(iLevelIndex, strComponentTag, pComponent);
}

CComponent* CGameInstance::Add_Component_Clone(_uint iLevelIndex, const wstring strComponentTag, void* pArg)
{
	return m_pComponent_Manager->Add_Component_Clone(iLevelIndex, strComponentTag, pArg);
}

const _float4x4* CGameInstance::Get_Transform_Float4x4(CPipeLine::D3DTRANSFORMSTATE eState)
{
	return m_pPipeLine->Get_Transform_Float4x4(eState);
}

_matrix CGameInstance::Get_Transform_Matrix(CPipeLine::D3DTRANSFORMSTATE eState)
{
	return m_pPipeLine->Get_Transform_Matrix(eState);
}

const _float4x4* CGameInstance::Get_Transform_Inverse_Float4x4(CPipeLine::D3DTRANSFORMSTATE eState)
{
	return m_pPipeLine->Get_Transform_Inverse_Float4x4(eState);
}

_matrix CGameInstance::Get_Transform_Inverse_Matrix(CPipeLine::D3DTRANSFORMSTATE eState)
{
	return m_pPipeLine->Get_Transform_Inverse_Matrix(eState);
}

const _float4* CGameInstance::Get_CamPosition_Float4()
{
	return m_pPipeLine->Get_CamPosition_Float4();
}

_vector CGameInstance::Get_CamPosition()
{
	return m_pPipeLine->Get_CamPosition();
}

void CGameInstance::Set_Transform(CPipeLine::D3DTRANSFORMSTATE eState, _fmatrix matTransform)
{
	m_pPipeLine->Set_Transform(eState, matTransform);
}

_vector CGameInstance::Get_CamLook()
{
	return m_pPipeLine->Get_CamLook();
}

_float CGameInstance::Get_TimeDelta(const _tchar* pTimerTag)
{
	return m_pTimer_Manager->Get_TimeDelta(pTimerTag);
}

void CGameInstance::Update_TimeDelta(const _tchar* pTimerTag)
{
	m_pTimer_Manager->Update_TimeDelta(pTimerTag);
}

HRESULT CGameInstance::Ready_Timer(const _tchar* pTimerTag)
{
	return m_pTimer_Manager->Ready_Timer(pTimerTag);
}

void CGameInstance::Get_ProcessMessageKeyboard(UINT message, WPARAM wParam, LPARAM lParam)
{
	m_pInput_Manager->Get_ProcessMessageKeyboard(message, wParam, lParam);
}

void CGameInstance::Get_ProcessMessageMouse(UINT message, WPARAM wParam, LPARAM lParam)
{
	m_pInput_Manager->Get_ProcessMessageMouse(message, wParam, lParam);
}

_int CGameInstance::Get_MouseX()
{
	return m_pInput_Manager->Get_MouseX();
}

_int CGameInstance::Get_MouseY()
{
	return m_pInput_Manager->Get_MouseY();
}

_bool CGameInstance::Is_KeyPressed(Keyboard::Keys eKeys)
{
	return m_pInput_Manager->Is_KeyPressed(eKeys);
}

_bool CGameInstance::Is_MouseLeftPressed()
{
	return m_pInput_Manager->Is_MouseLeftPressed();
}

_byte CGameInstance::Get_DIKeyState(_ubyte byKeyID)
{
	return m_pInput_Device->Get_DIKeyState(byKeyID);
}

_byte CGameInstance::Get_DIMouseState(MOUSEKEYSTATE eMouse)
{
	return m_pInput_Device->Get_DIMouseState(eMouse);
}

_long CGameInstance::Get_DIMouseMove(MOUSEMOVESTATE eMouseState)
{
	return m_pInput_Device->Get_DIMouseMove(eMouseState);
}

CInput_Device::eKEY_STATE CGameInstance::GetKeyState(_ubyte byKeyID)
{
	return m_pInput_Device->GetKeyState(byKeyID);
}

CInput_Device::eKEY_STATE CGameInstance::GetMouseState(MOUSEKEYSTATE eMouseState)
{
	return m_pInput_Device->GetMouseState(eMouseState);
}

_vector CGameInstance::Picking(_bool* isSuccess)
{
	return m_pPicking->Picking(isSuccess);
}

_vector CGameInstance::Get_RayDir()
{
	return m_pPicking->Get_RayDir();
}

void CGameInstance::Release_Engine()
{
	Free();

	this->DestroyInstance();
}

void CGameInstance::Free()
{
	Safe_Release(m_pLevel_Manager);
	Safe_Release(m_pGameObject_Manager);
	Safe_Release(m_pComponent_Manager);
	Safe_Release(m_pTimer_Manager);
	Safe_Release(m_pRenderer);
	Safe_Release(m_pPipeLine);
	Safe_Release(m_pGraphic_Device);
	Safe_Release(m_pInput_Manager);
	Safe_Release(m_pInput_Device);
	Safe_Release(m_pPicking);
}
