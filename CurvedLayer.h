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
	int count_d;//层高对应的体素层数，如层高d=2mm，体素精度为0.5mm，则对应的体素层数为2/0.5=4
	int interval_X;
	int interval_Y;

	CZ3DRepresentation m_pSolid;

};

