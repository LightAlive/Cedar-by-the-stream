#pragma once
#include "3DRepresentation.h"

class CurvedLayer
{
public:
	CurvedLayer();
	~CurvedLayer();

	void makeTopCurvedLayer();
	void creatCurvedLayer();
	void creatCurve();
	void creatSolidPath();

private:
	int count_d;//��߶�Ӧ�����ز���������d=2mm�����ؾ���Ϊ0.5mm�����Ӧ�����ز���Ϊ2/0.5=4
	int interval_X;
	int interval_Y;

	CZ3DRepresentation m_pSolid;

};

