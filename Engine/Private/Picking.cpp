#include "Picking.h"

#include "GameInstance.h"

CPicking::CPicking(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: m_pDevice{pDevice},
	m_pContext{pContext},
	m_pGameInstance{ CGameInstance::GetInstance() }
{
	Safe_AddRef(m_pGameInstance);
}

HRESULT CPicking::Initialize(HWND hWnd)
{
	m_hWnd = hWnd;

	D3D11_VIEWPORT ViewPort;
	_uint iNumViewPort = { 1 };
	m_pContext->RSGetViewports(&iNumViewPort, &ViewPort);

	m_fWinSizeX = ViewPort.Width;
	m_fWinSizeY = ViewPort.Height;

	return S_OK;
}

_vector CPicking::Picking(_bool* isSuccess)
{
	*isSuccess = true;

	POINT ptMouse;
	
	GetCursorPos(&ptMouse);
	ScreenToClient(m_hWnd, &ptMouse);

	if (ptMouse.x < 0)
		ptMouse.x = 0;
	else if (ptMouse.x > m_fWinSizeX)
		ptMouse.x = m_fWinSizeX;
	if (ptMouse.y < 0)
		ptMouse.y = 0;
	else if (ptMouse.y > m_fWinSizeY)
		ptMouse.y = m_fWinSizeY;

	_vector vMousePos = XMVectorZero();

	vMousePos = XMVectorSetX(vMousePos, ptMouse.x / (m_fWinSizeX * 0.5f) - 1.f);
	vMousePos = XMVectorSetY(vMousePos, ptMouse.y / -(m_fWinSizeY * 0.5f) + 1.f);
	vMousePos = XMVectorSetZ(vMousePos, m_pGameInstance->Compute_ProjZ(ptMouse));
	vMousePos = XMVectorSetW(vMousePos, 1.f);

	if (XMVectorGetZ(vMousePos) < 0.f)
		*isSuccess = false;

	vMousePos = XMVector3TransformCoord(vMousePos, m_pGameInstance->Get_Transform_Inverse_Matrix(CPipeLine::D3DTS_PROJ));
	vMousePos = XMVector3TransformCoord(vMousePos, m_pGameInstance->Get_Transform_Inverse_Matrix(CPipeLine::D3DTS_VIEW));

	return vMousePos;
}

CPicking* CPicking::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, HWND hWnd)
{
	CPicking* pInstance = new CPicking(pDevice, pContext);

	if (FAILED(pInstance->Initialize(hWnd)))
		Safe_Release(pInstance);

	return pInstance;
}

void CPicking::Free()
{
	Safe_Release(m_pGameInstance);
}
