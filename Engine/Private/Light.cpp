#include "Light.h"

#include "GameInstance.h"


CLight::CLight()
{
}

HRESULT CLight::Initialize(const LIGHT_DESC& LightDesc)
{
	m_LightDesc = LightDesc;
	return S_OK;
}

HRESULT CLight::Render(CShader* pShader, CVIBuffer_Rect* pVIBuffer)
{
	_uint iShaderIndex = { 0 };

	if (m_LightDesc.eType == LIGHT_DESC::TYPE_DIRECTIONAL) //방향광원은 어떤 방향으로 빛을 보낼것인지에 대한 방향벡터가 필요하다.
	{
		if (FAILED(pShader->Bind_RawValue("g_vLightDir", &m_LightDesc.vDirection, sizeof(_float4))))
			return E_FAIL;

		iShaderIndex = 1;
	}
	else if (m_LightDesc.eType == LIGHT_DESC::TYPE_POINT) // 점광원은 점의 위치와 범위가 필요하다. (방향은 필요하지 않다)
	{
		if (FAILED(pShader->Bind_RawValue("g_fLightRange", &m_LightDesc.fRange, sizeof(_float))))
			return E_FAIL;
		if (FAILED(pShader->Bind_RawValue("g_vLightPos", &m_LightDesc.vPosition, sizeof(_float4))))
			return E_FAIL;

		iShaderIndex = 2;
	}
	if (FAILED(pShader->Bind_RawValue("g_vLightDiffuse", &m_LightDesc.vDiffuse, sizeof(_float4))))
		return E_FAIL;
	if (FAILED(pShader->Bind_RawValue("g_vLightAmbient", &m_LightDesc.vAmbient, sizeof(_float4))))
		return E_FAIL;
	if (FAILED(pShader->Bind_RawValue("g_vLightSpecular", &m_LightDesc.vSpecular, sizeof(_float4))))
		return E_FAIL;


	pShader->Begin(iShaderIndex);
	pVIBuffer->Render();

	return S_OK;
}

CLight* CLight::Create(const LIGHT_DESC& LightDesc)
{
	CLight* pInstance = new CLight();

	if (FAILED(pInstance->Initialize(LightDesc)))
		Safe_Release(pInstance);

	return pInstance;
}

void CLight::Free()
{
}
