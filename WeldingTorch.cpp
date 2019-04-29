#include "WeldingTorch.h"
WeldingTorch::WeldingTorch()
{
	axisLine1= vtkSmartPointer<vtkLineSource>::New();
	tubeFilter1 = vtkSmartPointer<vtkTubeFilter>::New();
	weldingMapper1= vtkSmartPointer<vtkPolyDataMapper>::New();
	axisLine2 = vtkSmartPointer<vtkLineSource>::New();
	tubeFilter2 = vtkSmartPointer<vtkTubeFilter>::New();
	weldingMapper2= vtkSmartPointer<vtkPolyDataMapper>::New();

	
	for (int i = 0; i<5000; i++) {
		f[i][0] = 0; f[i][1] = 0;
	}
}
bool WeldingTorch:: Detection(Byte *a, int m_yCoefficient, int m_zCoefficient, unsigned long long sum)
{
	
	double x1, y1, z1, x2, y2, z2, x3, y3, z3;
	bool det = true; 
	for (int y = y0 - d/2+1; y < y0 + d/2; y++) {
		for (int x = x0 - d/2+1; x < x0 + d/2; x++) {
			if((z0*m_zCoefficient + y*m_yCoefficient + x)<sum)
			if(x>0&&y>0&& a[z0*m_zCoefficient + y*m_yCoefficient + x] == 1)
				return det;
		}
	}
	
	int k = 0; 
	for (int i = 0; i<5000; i++) {
		f[i][0] = 0; f[i][1] = 0;
	}
	
	for (int z = z0; z <= z0 + (H+h) * cos(lx)*cos(ly) + D; z++) {
		if (ly >= 0 && lx <= 0) {
			for (int y = y0 - 5;y <= y0 + 5 - (H + h) * sin(lx)*cos(ly) + D; y++) {
				for (int x = x0 - 5;x <= x0 + 5 + (H + h) * sin(ly)*cos(lx) + D; x++) {
					x1 = x - x0; y1 = y - y0; z1 = z - z0;                              //平移
					x2 = x1*cos(ly) - z1*sin(ly); y2 = y1; z2 = x1*sin(ly) + z1*cos(ly);//沿y轴旋转
					x3 = x2; y3 = y2*cos(lx) + z2*sin(lx); z3 = -y2*sin(lx) + z2*cos(lx);//沿x轴旋转
					if ((z*m_zCoefficient + y*m_yCoefficient + x)<sum)
					if (((x3*x3 + y3*y3) <= d*d/4 && z3 <= D && z3 >= 0 || (x3*x3 + y3*y3) <= D*D/4 && z3 <= (H + h) && z3 >= h)
						&& a[z*m_zCoefficient + y*m_yCoefficient + x] ==1) {                                                              //判断是否重合
						f[k][0] = x; f[k][1] = y;  
						k++; 
						det = false;

					}
				}
			}
		}
		if (ly >= 0 && lx >= 0) {
			for (int y = y0 - 5 - (H + h) * sin(lx)*cos(ly) - D; y <= y0 + 5; y++) {
				for (int x = x0 - 5; x <= x0 + 5 + (H + h) * sin(ly)*cos(lx) + D; x++) {
					x1 = x - x0; y1 = y - y0; z1 = z - z0;                              //平移
					x2 = x1*cos(ly) - z1*sin(ly); y2 = y1; z2 = x1*sin(ly) + z1*cos(ly);//沿y轴旋转
					x3 = x2; y3 = y2*cos(lx) + z2*sin(lx); z3 = -y2*sin(lx) + z2*cos(lx);//沿x轴旋转
					if ((z*m_zCoefficient + y*m_yCoefficient + x)<sum)
					if (((x3*x3 + y3*y3) <= d*d / 4 && z3 <= D && z3 >= 0 || (x3*x3 + y3*y3) <= D*D / 4 && z3 <= (H + h) && z3 >= h)
						&& a[z*m_zCoefficient + y*m_yCoefficient + x] == 1) {                                                              //判断是否重合
						f[k][0] = x; f[k][1] = y;  
						k++;
						det = false;

					}
				}
			}
		}
		if (ly <= 0 && lx <= 0) {
			for (int y = y0 - 5; y <= y0 + 5 - (H + h) * sin(lx)*cos(ly) + D; y++) {
				for (int x = x0 - 5 + (H + h) * sin(ly)*cos(lx) - D; x <= x0 + 5; x++) {
					x1 = x - x0; y1 = y - y0; z1 = z - z0;                              //平移
					x2 = x1*cos(ly) - z1*sin(ly); y2 = y1; z2 = x1*sin(ly) + z1*cos(ly);//沿y轴旋转
					x3 = x2; y3 = y2*cos(lx) + z2*sin(lx); z3 = -y2*sin(lx) + z2*cos(lx);//沿x轴旋转
					if ((z*m_zCoefficient + y*m_yCoefficient + x)<sum)
					if (((x3*x3 + y3*y3) <= d*d / 4 && z3 <= D && z3 >= 0 || (x3*x3 + y3*y3) <= D*D / 4 && z3 <= (H + h) && z3 >= h)
						&& a[z*m_zCoefficient + y*m_yCoefficient + x] == 1) {                                                              //判断是否重合
						f[k][0] = x; f[k][1] = y;  
						k++;
						det = false;

					}
				}
			}
		}
		if (ly <= 0 && lx >= 0) {
			for (int y = y0 - 5 - (H + h) * sin(lx)*cos(ly) - D; y <= y0 + 5; y++) {
				for (int x = x0 - 5 + (H + h) * sin(ly)*cos(lx) - D; x <= x0 + 5; x++) {
					x1 = x - x0; y1 = y - y0; z1 = z - z0;                              //平移
					x2 = x1*cos(ly) - z1*sin(ly); y2 = y1; z2 = x1*sin(ly) + z1*cos(ly);//沿y轴旋转
					x3 = x2; y3 = y2*cos(lx) + z2*sin(lx); z3 = -y2*sin(lx) + z2*cos(lx);//沿x轴旋转
					if ((z*m_zCoefficient + y*m_yCoefficient + x)<sum)
					if (((x3*x3 + y3*y3) <= d*d / 4 && z3 <= D && z3 >= 0 || (x3*x3 + y3*y3) <= D*D / 4 && z3 <= (H + h) && z3 >= h)
						&& a[z*m_zCoefficient + y*m_yCoefficient + x] == 1) {                                                              //判断是否重合
						f[k][0] = x; f[k][1] = y;  
						k++;
						det = false;

					}
				}
			}
		}
	}
	
	return det;
}

void WeldingTorch::Dispose(Byte *a, int m_yCoefficient, int m_zCoefficient, unsigned long long sum)
{
	int xp = 0, yp = 0, xn = 0, yn = 0,f1=0,f2=0,f3=0,f4=0;
	for (int i = 0; i<5000; i++) {
		if (f[i][0] == 0 && f[i][1] == 0)break;
		
		if (f[i][0] > x0)xp++;
		if (f[i][0] < x0)xn++;
		if (f[i][1] > y0)yp++;
		if (f[i][1] < y0)yn++;
	}
	

	if (xp > xn) {  f1 = 1; }
	if (xp < xn) {  f2 = 1; }
	if (yp > yn) { f3 = 1;}
	if (yp < yn) { f4 = 1;}
	int m = -1,s=0;//s表示调整总次数，若s过大表示此位置无法调节位姿，放弃
	while (m != 0) {
		s++;
		if(f1==1)ly -= Pi / 48;
		if(f2==1)ly += Pi / 48;
		if(f3==1)lx += Pi / 48;
		if(f4==1)lx -= Pi / 48;

		int k = 0;
		for (int i = 0; i<5000; i++) {
			f[i][0] = 0; f[i][1] = 0;
		}
		double x1, y1, z1, x2, y2, z2, x3, y3, z3;
		for (int z = z0; z <= z0 + (H + h) * cos(lx)*cos(ly) + D; z++) {
			if (ly >= 0 && lx <= 0) {
				for (int y = y0 - 5; y <= y0 + 5 - (H + h) * sin(lx)*cos(ly) + D; y++) {
					for (int x = x0 - 5; x <= x0 + 5 + (H + h) * sin(ly)*cos(lx) + D; x++) {
						x1 = x - x0; y1 = y - y0; z1 = z - z0;                              //平移
						x2 = x1*cos(ly) - z1*sin(ly); y2 = y1; z2 = x1*sin(ly) + z1*cos(ly);//沿y轴旋转
						x3 = x2; y3 = y2*cos(lx) + z2*sin(lx); z3 = -y2*sin(lx) + z2*cos(lx);//沿x轴旋转
						if ((z*m_zCoefficient + y*m_yCoefficient + x)<sum)
						if (((x3*x3 + y3*y3) <= d*d / 4 && z3 <= D && z3 >= 0 || (x3*x3 + y3*y3) <= D*D / 4 && z3 <= (H + h) && z3 >= h)
							&& a[z*m_zCoefficient + y*m_yCoefficient + x] == 1) {                                                              //判断是否重合
							f[k][0] = x; f[k][1] = y;
							k++;
							

						}
					}
				}
			}
			if (ly >= 0 && lx >= 0) {
				for (int y = y0 - 5 - (H + h) * sin(lx)*cos(ly) - D; y <= y0 + 5; y++) {
					for (int x = x0 - 5; x <= x0 + 5 + (H + h) * sin(ly)*cos(lx) + D; x++) {
						x1 = x - x0; y1 = y - y0; z1 = z - z0;                              //平移
						x2 = x1*cos(ly) - z1*sin(ly); y2 = y1; z2 = x1*sin(ly) + z1*cos(ly);//沿y轴旋转
						x3 = x2; y3 = y2*cos(lx) + z2*sin(lx); z3 = -y2*sin(lx) + z2*cos(lx);//沿x轴旋转
						if ((z*m_zCoefficient + y*m_yCoefficient + x)<sum)
						if (((x3*x3 + y3*y3) <= d*d / 4 && z3 <= D && z3 >= 0 || (x3*x3 + y3*y3) <= D*D / 4 && z3 <= (H + h) && z3 >= h)
							&& a[z*m_zCoefficient + y*m_yCoefficient + x] == 1) {                                                              //判断是否重合
							f[k][0] = x; f[k][1] = y;
							k++;
							

						}
					}
				}
			}
			if (ly <= 0 && lx <= 0) {
				for (int y = y0 - 5; y <= y0 + 5 - (H + h) * sin(lx)*cos(ly) + D; y++) {
					for (int x = x0 - 5 + (H + h) * sin(ly)*cos(lx) - D; x <= x0 + 5; x++) {
						x1 = x - x0; y1 = y - y0; z1 = z - z0;                              //平移
						x2 = x1*cos(ly) - z1*sin(ly); y2 = y1; z2 = x1*sin(ly) + z1*cos(ly);//沿y轴旋转
						x3 = x2; y3 = y2*cos(lx) + z2*sin(lx); z3 = -y2*sin(lx) + z2*cos(lx);//沿x轴旋转
						if ((z*m_zCoefficient + y*m_yCoefficient + x)<sum)
						if (((x3*x3 + y3*y3) <= d*d / 4 && z3 <= D && z3 >= 0 || (x3*x3 + y3*y3) <= D*D / 4 && z3 <= (H + h) && z3 >= h)
							&& a[z*m_zCoefficient + y*m_yCoefficient + x] == 1) {                                                              //判断是否重合
							f[k][0] = x; f[k][1] = y; 
							k++;
							

						}
					}
				}
			}
			if (ly <= 0 && lx >= 0) {
				for (int y = y0 - 5 - (H + h) * sin(lx)*cos(ly) - D; y <= y0 + 5; y++) {
					for (int x = x0 - 5 + (H + h) * sin(ly)*cos(lx) - D; x <= x0 + 5; x++) {
						x1 = x - x0; y1 = y - y0; z1 = z - z0;                              //平移
						x2 = x1*cos(ly) - z1*sin(ly); y2 = y1; z2 = x1*sin(ly) + z1*cos(ly);//沿y轴旋转
						x3 = x2; y3 = y2*cos(lx) + z2*sin(lx); z3 = -y2*sin(lx) + z2*cos(lx);//沿x轴旋转
						if ((z*m_zCoefficient + y*m_yCoefficient + x)<sum)
						if (((x3*x3 + y3*y3) <= d*d / 4 && z3 <= D && z3 >= 0 || (x3*x3 + y3*y3) <= D*D / 4 && z3 <= (H + h) && z3 >= h)
							&& a[z*m_zCoefficient + y*m_yCoefficient + x] == 1) {                                                              //判断是否重合
							f[k][0] = x; f[k][1] = y;
							k++;
							
						}
					}
				}
			}
		}
	    xp = 0, yp = 0, xn = 0, yn = 0;
		int i = 0;
		for (i = 0; i<5000; i++) {
			if (f[i][0] == 0 && f[i][1] == 0)break;

			if (f[i][0] > x0)xp++;
			if (f[i][0] < x0)xn++;
			if (f[i][1] > y0)yp++;
			if (f[i][1] < y0)yn++;
		} 
		
		m = i;

		/*if (f1&&xp < xn) { ly -= Pi / 48; f1 = 0; }
		if (f2&&xp > xn) { ly += Pi / 48; f2 = 0; }
		if (f3&&yp < yn) { lx+= Pi / 48; f3 = 0; }
		if (f4&&yp > yn) { lx -= Pi / 48; f4 = 0; }*/
		if (xp == xn) { f1 = 0; f2 = 0; }
		if (yp == yn) { f3 = 0; f4 = 0; }
		if (f1 == 0 && f2 == 0 && f3 == 0 && f4 == 0)break;
		if (s > 20)break;
	}
	
	
}
vtkSmartPointer<vtkPolyDataMapper> WeldingTorch::draw1()
{
	double x1, y1, z1,x2,y2,z2,x3,y3,z3;
	x1 = 0; y1 = 0; z1 = h;          //平移至原点
	x2 = x1*cos(-ly) - z1*sin(-ly); y2 = y1; z2 = x1*sin(-ly) + z1*cos(-ly);//沿y轴旋转
	x3 = x2; y3 = y2*cos(-lx) + z2*sin(-lx); z3 = -y2*sin(-lx) + z2*cos(-lx);//沿x轴旋转
	x3 += x0; y3 += y0; z3 += z0;      //再平移至原位置

	
	axisLine1->SetPoint1(x0, y0, z0); axisLine1->SetPoint2(x3, y3, z3);       //设置焊枪轴线端点坐标
	
	tubeFilter1->SetInputConnection(axisLine1->GetOutputPort());
	tubeFilter1->SetRadius(d/2);
	tubeFilter1->SetNumberOfSides(16);
	tubeFilter1->CappingOn();

	
	weldingMapper1->SetInputConnection(tubeFilter1->GetOutputPort());


	return weldingMapper1;

}

vtkSmartPointer<vtkPolyDataMapper> WeldingTorch::draw2()
{
	double x1, y1, z1, x2, y2, z2, x3, y3, z3;
	x1 = 0; y1 = 0; z1 = h;          //平移至原点
	x2 = x1*cos(-ly) - z1*sin(-ly); y2 = y1; z2 = x1*sin(-ly) + z1*cos(-ly);//沿y轴旋转
	x3 = x2; y3 = y2*cos(-lx) + z2*sin(-lx); z3 = -y2*sin(-lx) + z2*cos(-lx);//沿x轴旋转
	x3 += x0; y3 += y0; z3 += z0;      //再平移至原位置
	double x4, y4, z4, x5, y5, z5, x6, y6, z6;
	x4 = 0; y4 = 0; z4 = H+h;          //平移
	x5 = x4*cos(-ly) - z4*sin(-ly); y5 = y4; z5 = x4*sin(-ly) + z4*cos(-ly);//沿y轴旋转
	x6 = x5; y6 = y5*cos(-lx) + z5*sin(-lx); z6 = -y5*sin(-lx) + z5*cos(-lx);//沿x轴旋转
	x6 += x0; y6 += y0; z6 += z0;
	
	axisLine2->SetPoint1(x3, y3, z3); axisLine2->SetPoint2(x6, y6, z6);       //设置焊枪轴线端点坐标
	
	tubeFilter2->SetInputConnection(axisLine2->GetOutputPort());
	tubeFilter2->SetRadius(D/2);
	tubeFilter2->SetNumberOfSides(20);
	tubeFilter2->CappingOn();
	
	weldingMapper2->SetInputConnection(tubeFilter2->GetOutputPort());


	return weldingMapper2;

}

