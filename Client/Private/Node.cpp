#include "Node.h"

CNode::CNode()
{
}

_uint CNode::Random(_uint iMaxNumber)
{
	srand(unsigned(time(NULL)));

	return rand() % iMaxNumber;
}

_float CNode::Distance(_matrix TargetMatrix, _matrix FocusMatrix)
{
	_vector vTargetPos = TargetMatrix.r[3];
	_vector vBossPos = FocusMatrix.r[3];

	_vector vDistance = vTargetPos - vBossPos;

	_float fDistance = sqrtf(XMVectorGetX(vDistance) * XMVectorGetX(vDistance) + XMVectorGetY(vDistance) * XMVectorGetY(vDistance) + XMVectorGetZ(vDistance) * XMVectorGetZ(vDistance));

	return fDistance;
}

_bool CNode::AngleOfView(_float fLookAngle, _fvector vTargetPos, _fvector vCurrentPos, _fvector vLook)
{
	_vector vDesc = XMVector3Normalize(vTargetPos - vCurrentPos);
	_float fDot = XMVectorGetX(XMVector3Dot(vDesc, vLook));

	_float fAngle = XMConvertToDegrees(acosf(fDot));

	return fAngle < fLookAngle * 0.5f;
}

void CNode::Free()
{
}
