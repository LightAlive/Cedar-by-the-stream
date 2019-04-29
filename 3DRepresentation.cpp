#include "stdafx.h"
#include "3DRepresentation.h"
const double cCoef = 0.3;

//X�����ϵ�ɨ����
static CZLineScanSetting  scanOptionX =
{
	rtSLS,		//RPM Type
	scanX,	    //
	false,		//none bidirectional scan
	clDefault,	//line fill does not care of whether to scan ring line,
	//	since ring line will be drawed again
	0,			//ignored frame, RESET LATER
	1.0f,		//intver(step of basic scan 
	//rect now not setted, RESET line), RESET LATER
	{ { 0, 0 }, { 0, 0 } },
	//(rect of grid region, always the same as xy plane of solid cube)
	0,			//no multi scan
	0,			//miltiGate ignored

	false,		//no XY multi scan

	false,		//no gap multi
	0,			//gap ration ignored

	false,		//no Z multi-scan
	0			//z gate ignored	
};

int getMemoryEstimate(const CCubef &p_cube, float compenstae, float resolution)
{
	float f = compenstae + resolution;
	CPoint3f pf1 = p_cube.min - point3(f, f, f);
	CPoint3f pf2 = p_cube.max + point3(f, f, f);
	pf2 = pf2 - pf1;
	int x = (int)(pf2.x / resolution) + 3;
	int y = (int)(pf2.y / resolution) + 3;
	int z = (int)(pf2.z / resolution) + 3;
	int totalsize = x*y*z;
	return totalsize;
}

//����ά����ά��ת�� 
inline void forOne3D2D(const CPoint3d &p_3d, CPoint2d &p_out2d, CMatrix4d &p_m3d2d)
{
	CPoint3d p = p_m3d2d*p_3d;
	p_out2d.x = p.x;
	p_out2d.y = p.y;
}

//���ά����ά��ת��
inline void forOne2D3D(const CPoint2d &p_2d, CPoint3d &p_out3d, CMatrix4d &p_m2d3d)
{
	CPoint3d p3d;
	p3d.x = p_2d.x;
	p3d.y = p_2d.y;
	p3d.z = 0.0;
	p_out3d = p_m2d3d*p3d;
}

//������ά���굽��ά��ת��
void for3D2D(const CZArray<CPoint3d, false> &p_3d, CZArray<CPoint2d, false> &p_out2f, CMatrix4d &p_m3d2d)
{
	p_out2f.setSize(0);
	for (int i = 0; i < p_3d.size(); i++)
	{
		CPoint2d &p = p_out2f.add();
		forOne3D2D(p_3d[i], p, p_m3d2d);
	}
}

//��ά���굽��ά��ת��---------------------------------------------------------
void for2D3D(const  CZArray<CPoint2d, false> &p_2d, CZArray<CPoint3d, false> &p_out3d, CMatrix4d &p_m2d3d)
{
	CPoint3d p3d;
	p3d.z = 0.0;
	p_out3d.setSize(0);
	for (int i = 0; i<p_2d.size(); i++)
	{
		CPoint3d &p = p_out3d.add();
		forOne2D3D(p_2d[i], p, p_m2d3d);
	}
}

//����ֱ�ߵ�ֱ�ߵ���ת����  ---------------------------------------------------
void calcolateMatrix(const CPoint3d &p_p1, const CPoint3d &p_p2, const CPoint3d &p_p3, CMatrix4d &p_m3d2d, CMatrix4d &p_m2d3d, CPoint3d &p_normal)
{
	CPoint3d userZero;
	CPoint3d  zAxi = zeroPoint3d;
	zAxi.z = 1.0;
	double m;
	definePlane(p_p1, p_p2, p_p3, userZero, m);//userZeroΪ����, m=0.0f��ƽ���ԭ�㡣�����userZero��ԭ����������ƽ�������ߵĴ��㡣
	{
		CPoint3d temp = userZero;
		normalize(temp);
		if (temp.x == 0.0&&temp.y == 0.0 && (temp.z == 1.0))
		{
			userZero.z = p_p1.z;//��һ���������������������Z���ϣ�˵�����������Ѿ�ƽ����xoy�棬ֱ��ȡ����һ����Ϊԭ�㼴��
		}
	}
	rotateFromTo(userZero, zAxi, p_m3d2d);//����������������������໥��ת�ľ���
	rotateFromTo(zAxi, userZero, p_m2d3d);
	p_normal = userZero;
	normalize(p_normal);
	CPoint3d  off = p_m3d2d*userZero;//������ת����ƶ�����
	if (m == 0.0)
	{
		off = m*off;
		userZero = m*userZero;
	}
	p_m3d2d[3][0] = -off.x;
	p_m3d2d[3][1] = -off.y;
	p_m3d2d[3][2] = -off.z;   //�ƶ�

	p_m2d3d[3][0] = userZero.x;
	p_m2d3d[3][1] = userZero.y;
	p_m2d3d[3][2] = userZero.z;

}

//�����߶εĽ���---------------------------------------------------------------
CPoint2d calcCrossPoint(const CPoint2d& o1, const CPoint2d& o2, const CPoint2d& d1, const CPoint2d& d2)
{
	//calc cross point, see <<computer graphic>> pp393
	/*
	1. xo1+t*dx1=xo2+s*dx2
	2. yo1+t*dy1=yo2+s*dy2
	==>t= ( dx2(yo1-yo2) - dy2(xo1-xo2) )   / ( dx1*dy2 - dx2*dy1)
	*/
	double t = (d2.x*(o1.y - o2.y) - d2.y*(o1.x - o2.x)) / (d1.x*d2.y - d2.x*d1.y);
	//t=0;

	CPoint2d crossPoint;
	crossPoint.x = o1.x + t*d1.x;
	crossPoint.y = o1.y + t*d1.y;
	return crossPoint;
}

//���������ε�����
void circleCenter(const CPoint2d &p_p1, const CPoint2d &p_p2, const CPoint2d &p_p3, CPoint2d &p_cent, double &p_r)
{
	double  angleP1 = angle(p_p3, p_p1, p_p2) / 2;
	double  angleP2 = angle(p_p1, p_p2, p_p3) / 2;
	CPoint2d  o1, o2, p1, p2;
	o1 = point2(p_p2.x - p_p1.x, p_p2.y - p_p1.y);
	o2 = point2(p_p3.x - p_p2.x, p_p3.y - p_p2.y);
	p1 = point2(p_p1.x, p_p1.y);
	p2 = point2(p_p2.x, p_p2.y);
	CPoint2d  d1 = point2((o1.x*cos(angleP1) - o1.y*sin(angleP1)), (o1.x*sin(angleP1) + o1.y*cos(angleP1)));
	CPoint2d  d2 = point2((o2.x*cos(angleP2) - o2.y*sin(angleP2)), (o2.x*sin(angleP2) + o2.y*cos(angleP2)));
	CPoint2d cen = calcCrossPoint(p1, p2, d1, d2);
	p_cent.x = cen.x;
	p_cent.y = (float)cen.y;
	double  dis = dist(p1, cen);
	p_r = dis*sin((angleP1>0.0f) ? angleP1 : (-angleP1));

}

CZ3DRepresentation::CIntLine  const *allLinesData;
int comparePoint(const void *i1, const void *i2)
//ֱ�߰��յ������,���Ȱ���y��,��y��ͬ,�ٰ���x.������С����.
{
	const CPoint2i &p1 = allLinesData[*(int*)i1].start;
	const CPoint2i &p2 = allLinesData[*(int*)i2].start;

	int diff = (p1.y != p2.y) ? (p1.y - p2.y) : (p1.x - p2.x);
	return diff > 0 ? 1 : (diff == 0 ? 0 : -1);
}

//�۰���ҳ���һ���߽���
int	findNextPoindId(int p_keyPoint, CZArray<int> &index)
{
	int low = 0;
	int high = index.topI();
	while (low <= high)
	{
		int mid = (low + high) / 2;
		int compare = comparePoint(&p_keyPoint, &index[mid]);
		if (!compare)	return mid;
		else
		if (compare > 0)  low = mid + 1;
		else	high = mid - 1;
	}
	return -1;
}

void cutslice(vtkPolyData *p_solid, vtkCutter *p_cutter, vtkPolyData *p_slice, double height)
{
	vtkSmartPointer<vtkPlane> plane = vtkSmartPointer<vtkPlane>::New();
	plane->SetOrigin(0, 0, 0);
	plane->SetNormal(0, 0, 1);

	double minBound[3];
	minBound[0] = p_solid->GetBounds()[0];
	minBound[1] = p_solid->GetBounds()[2];
	minBound[2] = p_solid->GetBounds()[4];
	double maxBound[3];
	maxBound[0] = p_solid->GetBounds()[1];
	maxBound[1] = p_solid->GetBounds()[3];
	maxBound[2] = p_solid->GetBounds()[5];

	/*vtkSmartPointer<vtkCutter> cutter = vtkSmartPointer<vtkCutter>::New();*/
	p_cutter->SetCutFunction(plane);
	p_cutter->SetInputData(p_solid);
	p_cutter->GenerateValues(1, height, height);
	p_cutter->Update();

	p_slice = p_cutter->GetOutput();

	int num = p_slice->GetNumberOfCells();
	int num2 = p_slice->GetNumberOfPoints();
}

CBinarySTLExporter::CBinarySTLExporter(const char *fn)
{
	m_triNum=0;
	char headinf[80]="by chen";
	m_ostream.open(fn, ios::binary | ios::trunc);
	m_ostream.write(headinf, 80);
	int all=0;
	binWrite(m_ostream, all);
}

CBinarySTLExporter::~CBinarySTLExporter(void)
{
	m_ostream.seekp(80, ios::beg);
	binWrite(m_ostream, m_triNum);
	m_ostream.close();
}

inline void dump(CPoint3f p)
{
	//TRACE("(%f, %f, %f)\n", p.x, p.y, p.z);
}
inline void dumpi(CPoint3i p)
{
	//TRACE("p3i=(%d, %d, %d)\n", p.x, p.y, p.z);
}
inline  void dump(CPoint2f p)
{
	//TRACE("(%f, %f)\n", p.x, p.y);
}

void CBinarySTLExporter::exporter(CPoint3f p_p0, CPoint3f p_p1, CPoint3f p_p2)
{
	CZ3DRepresentation::CFace f;
	safeNormal(p_p0, p_p1, p_p2, f.normal);
	f.p[0]=p_p0;
	f.p[1]=p_p1;
	f.p[2]=p_p2;
	f.attrib=0;
	binWrite(m_ostream, f);
	m_triNum++;
}


//������������CZ3DRepresentation�Ĳ���ʵ��
CZ3DRepresentation::CZ3DRepresentation():m_buff(NULL){}
CZ3DRepresentation::CZ3DRepresentation(CCubef p_orginalSolidCube, float p_compensate, float p_resolution):m_buff(NULL)
{
	init(p_orginalSolidCube, p_compensate, p_resolution);
}

//CZ3DRepresentation::~CZ3DRepresentation()
//{
//	free();
//}

//��������������Ϊ�����buff����Ĵ�С��Ȼ�󴴽�һ��Ԫ��ȫΪ0�Ŀ�����buff��
void CZ3DRepresentation::init(CCubef p_orginalSolidCube,  float p_compensate, float p_resolution)                
{
	assert(!bInited());

	CPoint3f pf=p_orginalSolidCube.min;
	m_orgCube.min = point3(double(pf.x), double(pf.y), double(pf.z));                                        
	pf=p_orginalSolidCube.max;
	m_orgCube.max = point3(double(pf.x), double(pf.y), double(pf.z));

	//���ﲻ�Ͻ���float���ͺ�double���͵�ת������double���͵�Ŀ������߾���
	m_compensate = double(p_compensate);
	m_resolution = double(p_resolution);
	double  temp = m_compensate+m_resolution;
	CPoint3d  p3d = point3(temp, temp, temp);
	
	//STLʵ��ģ�͵İ�Χ���������Ѿ�ƫ��������һ���ˡ�
	//�����STLģ��������;��Ȳ�����Ϊʲô�����������������ʲô���ˣ�
	m_cube.min = m_orgCube.min-p3d;
	m_cube.max = m_orgCube.max+p3d;

	m_maxX = (int)(m_cube.w()/m_resolution)+2;                                                       
	m_maxY = (int)(m_cube.h()/m_resolution)+2;                                                       
	m_maxZ = (int)(m_cube.t()/m_resolution)+2;
	//+2��ԭ���ǣ�Ϊ����֮���ж�ĳһ�����������ҵ���������״̬ʱ���ᷢ��ĳһ��������û�������ˣ������ش��ڱ߽磩�����          
	//��˽�ʵ�����ڵ��壬��ÿ��ά����ǰ�������һ������λ�ã������Ƶ����м䣬����ܹ�Ҫ+2
	
	unsigned long long  totalsize = ((unsigned long long)m_maxX + 1)*
		((unsigned long long)m_maxY + 1)*((unsigned long long)m_maxZ + 1);
	//��������+1��Ϊ�˱�֤��ȡ��0-m_max             
	//������������������ģ���У�����ֵ���������ص�һ���ǵ㣻
	//��m_cube�����ʵ����������Ⱥ��Χ�⣬ÿ��ά�ȵ���β��������һ�����أ�������֮����жϱ߽��з�ֹ����Խ�硣
	//��ˣ�����1����m_cube�ĵ�һ�����ء�����m_max-1�����һ�����ء�

	m_buff = new Byte[totalsize];
	
	//memset()������m_buff��totalsize���ֽ����㣺void *memset(void *s, int ch, size_t n);
	//�������ͣ���s�е�ǰλ�ú����n���ֽ� ��typedef unsigned int size_t ���� ch �滻������ s 
	memset(m_buff, 0, totalsize);   

	m_buff_CurvedLayer = new Byte[totalsize];
	memset(m_buff_CurvedLayer, 0, totalsize);

	m_buff_CurvedLayer2 = new Byte[totalsize];
	memset(m_buff_CurvedLayer2, 0, totalsize);
	
	m_yCoefficient = m_maxX+1;                                                                                            
	m_zCoefficient = (m_maxY+1)*m_yCoefficient;                                                                            
}

//�жϱ߽����ʵ�����ϲ�------------------------------------------------------------
//�ж��Ƿ�Ϊ����ʵ��ı߽�
bool CZ3DRepresentation::bBoundary(int p_x, int p_y, int p_z)const
{
	if(!bDotInSolid(p_x-1, p_y, p_z))
		return true;
	if(!bDotInSolid(p_x+1, p_y, p_z))
		return true;
	if(!bDotInSolid(p_x, p_y-1, p_z))
		return true;
	if(!bDotInSolid(p_x, p_y+1, p_z))
		return true;
	if(!bDotInSolid(p_x, p_y, p_z-1))
		return true;
	if(!bDotInSolid(p_x, p_y, p_z+1))
		return true;
	return false;
	
}
//�ж��Ƿ�Ϊĳһzֵ���ڵ�ƽ���ı߽�
bool CZ3DRepresentation::bBoundaryAtZ(int p_x, int p_y, int p_z)const
{
	if (!bDotInSolid(p_x - 1, p_y, p_z))
		return true;
	if (!bDotInSolid(p_x + 1, p_y, p_z))
		return true;
	if (!bDotInSolid(p_x, p_y - 1, p_z))
		return true;
	if (!bDotInSolid(p_x, p_y + 1, p_z))
		return true;	
	return false;

}
//�ж��Ƿ�Ϊ���ϲ�
bool CZ3DRepresentation::bTopBoundary(int p_x, int p_y, int p_z)
{
	if (!bDotInSolid(p_x, p_y, p_z + 1))
	{
		return true;
	}	
	return false;
}

bool CZ3DRepresentation::bBottomBoundary(int p_x, int p_y, int p_z)
{
	if (!bDotInSolid(p_x, p_y, p_z - 1))
	{
		return true;
	}
	return false;
}



//�ҵ�ָ���㣨����zֵ��������߽��߲�������뵽����p_lines��
//void CZ3DRepresentation::findBoundaryLine(double p_z, CZArray<CIntLine, false> &p_lines, Byte p_maskneed)//=dmBoundary                     //CZArray��������һ����������ģ�壬�ú��������ص�ѧϰһ��
//{
//	int z=toIntZ(p_z);
//	CIntLine line;
//	/************************************************************************/
//	for(int y=1; y<m_maxY; y++ )
//	{
//		for(int x=1; x<m_maxX; x++)
//		{
//			if(bDotInSolid(x,y,z) && bBoundaryAtZ(x,y,z))
//			{
//                if(!bDotInSolid(x+1, y, z))
//				{
//					//д���ˣ���������ʱ���߼�
//					line.start = point2(x+1, y);
//					line.end = point2(x+1, y + 1);
//					p_lines.add(line);						
//				}
//				if(!bDotInSolid(x-1, y, z))
//				{
//					line.start=point2(x, y+1);
//					line.end=point2(x, y);
//					p_lines.add(line);
//				}
//				if(!bDotInSolid(x, y-1, z))
//				{
//					line.start=point2(x, y);
//					line.end=point2(x+1, y);
//					p_lines.add(line);
//				}
//				if(!bDotInSolid(x, y+1, z))
//				{
//					line.start=point2(x+1, y+1);
//					line.end=point2(x, y+1);
//					p_lines.add(line);
//				}
//			}
//		}
//	}
//}

//��p_z��߶Ƚ�����Ƭ�������p_slice��:���ҵ����е�����߽��ߣ����䱣�浽��ʱ�����У�
//Ȼ�������ߵ�������򣬲����۰���ҳ���β����������ɻ������Ի���ÿһ��������Բ��
//��˳��
//����������Ƭ���浽�ļ���
//void CZ3DRepresentation::exportBinarySTL(const char *p_fn, float p_smoothness, vtkPolyData *p_solid, const int &p_hellowtype, const int &p_suppertype, const float &p_diameter, const float &p_interval)
//{
//	assert(bInited());
//	switch(p_hellowtype)
//	{
//		case 0:   //��ɢSTLģ��
//			exportBinarySTL(p_fn, false);
//			removeVerySmallPart();
//			break;
//		case 1:   //ƫ�á�����ÿ�������ν�����ɢƫ�ã��ÿո�������
//			{
//				CTrianglef testf;
//				CTriangled testd;
//				for (int i = 0; i < p_solid->GetNumberOfCells(); i++)
//				{
//					vtkSmartPointer<vtkIdList> pointslist = vtkSmartPointer<vtkIdList>::New();
//					
//					p_solid->GetCellPoints(i, pointslist);
//					p_solid->GetPoint(pointslist->GetId(0));
//					//p_solid.getTri(i, testf.p[0], testf.p[1], testf.p[2]);
//					testd.p[0] = point3(double(p_solid->GetPoint(pointslist->GetId(0))[0]), double(p_solid->GetPoint(pointslist->GetId(0))[1]), double(p_solid->GetPoint(pointslist->GetId(0))[2]));
//					testd.p[1] = point3(double(p_solid->GetPoint(pointslist->GetId(1))[0]), double(p_solid->GetPoint(pointslist->GetId(1))[1]), double(p_solid->GetPoint(pointslist->GetId(1))[2]));
//					testd.p[2] = point3(double(p_solid->GetPoint(pointslist->GetId(2))[0]), double(p_solid->GetPoint(pointslist->GetId(2))[1]), double(p_solid->GetPoint(pointslist->GetId(2))[2]));
//					makeTriFacet(testd, 0x00, 0xfe);
//				}
//				removeVerySmallPart();
//				exportBinarySTL(p_fn, false);		
//				break;
//			}
//		case 2:    //�ο�
//			{
//				CTrianglef testf;
//				CTriangled testd;				
//				CBinarySTLExporter ex(p_fn);
//				const int step=1000;
//				int iPuted=step;
//				for(int i=0; i<p_solid->GetNumberOfCells(); i++)
//				{
//					vtkSmartPointer<vtkIdList> pointslist = vtkSmartPointer<vtkIdList>::New();
//					p_solid->GetCellPoints(i, pointslist);
//					p_solid->GetPoint(pointslist->GetId(0));
//					//p_solid.getTri(i, testf.p[0], testf.p[1], testf.p[2]);
//					testd.p[0] = point3(double(p_solid->GetPoint(pointslist->GetId(0))[0]), double(p_solid->GetPoint(pointslist->GetId(0))[1]), double(p_solid->GetPoint(pointslist->GetId(0))[2]));
//					testd.p[1] = point3(double(p_solid->GetPoint(pointslist->GetId(1))[0]), double(p_solid->GetPoint(pointslist->GetId(1))[1]), double(p_solid->GetPoint(pointslist->GetId(1))[2]));
//					testd.p[2] = point3(double(p_solid->GetPoint(pointslist->GetId(2))[0]), double(p_solid->GetPoint(pointslist->GetId(2))[1]), double(p_solid->GetPoint(pointslist->GetId(2))[2]));
//					makeTriFacet(testd, 0x00, 0xfe);
//					ex.exporter(testf.p[0], testf.p[1], testf.p[2]);//���ԭʼSTL��Ƭ
//					if(i>=iPuted)
//					{
//						//getMsgOut().puts("�Ѵ���%3.1f�����Ƭ��%2.0f%%���.\n", iPuted/10000.0, double(iPuted)/p_solid.getFaceCount()*100);
//						iPuted+=step;
//					}
//				}
//				//getMsgOut().puts("��Ƭ�������\n" );
//				removeVerySmallPart();
//				removeVerySmallPart();
//				//getMsgOut().puts("����ļ�..." );
//				//��������ģ��ֻʣ����ǻ���֣����������߽�����
//				exportFacets(ex, true);//���
//				break;	
//			}
//
//		case 3:    //�οղ�֧��
//			{
//
//				CTrianglef testf;
//				CTriangled testd;	
//				CBinarySTLExporter ex(p_fn);
//				const int step=1000;
//				int iPuted=step;
//				for(int i=0; i<p_solid->GetNumberOfCells(); i++)
//				{
//					vtkSmartPointer<vtkIdList> pointslist = vtkSmartPointer<vtkIdList>::New();
//					p_solid->GetCellPoints(i, pointslist);
//					p_solid->GetPoint(pointslist->GetId(0));
//					//p_solid.getTri(i, testf.p[0], testf.p[1], testf.p[2]);
//					testd.p[0] = point3(double(p_solid->GetPoint(pointslist->GetId(0))[0]), double(p_solid->GetPoint(pointslist->GetId(0))[1]), double(p_solid->GetPoint(pointslist->GetId(0))[2]));
//					testd.p[1] = point3(double(p_solid->GetPoint(pointslist->GetId(1))[0]), double(p_solid->GetPoint(pointslist->GetId(1))[1]), double(p_solid->GetPoint(pointslist->GetId(1))[2]));
//					testd.p[2] = point3(double(p_solid->GetPoint(pointslist->GetId(2))[0]), double(p_solid->GetPoint(pointslist->GetId(2))[1]), double(p_solid->GetPoint(pointslist->GetId(2))[2]));
//					makeTriFacet(testd, 0x00, 0xfe);
//					ex.exporter(testf.p[0], testf.p[1], testf.p[2]);
//					if(i>=iPuted)
//					{
//						//getMsgOut().puts("�Ѵ���%3.1f�����Ƭ��%2.0f%%���.\n", iPuted/10000.0, double(iPuted)/p_solid.getFaceCount()*100);
//						iPuted+=step;
//					}
//				}
//				//getMsgOut().puts("��Ƭ�������\n" );
//				//getMsgOut().puts("����֧��..." );
//				makeSupportMatrix((!p_suppertype)?stSquare:stCircle, double(p_diameter), double(p_interval), point2(0.0, 0.0) );	
//				//getMsgOut().puts("���\n" );
//				removeVerySmallPart();
//				//getMsgOut().puts("����ļ�..." );
//				exportFacets(ex, true);
//				break;
//			}
//	}
//}

//��VTK��ͼ����ǰ����OpenGL���Ƶģ�
void CZ3DRepresentation::glDraw(vtkSmartPointer<vtkPoints> ugpoints, vtkSmartPointer<vtkUnstructuredGrid> ug, vtkSmartPointer<vtkCellArray> cells)   //��VTK��ʾʵ�����ص�
{
	int pointsId = 0;
	for (int z = 1; z < m_maxZ; z++)
	{
		for (int y = 1; y < m_maxY; y++)
		{
			for (int x = 1; x < m_maxX; x++)
			{
				if (bDotInSolid(x, y, z))
				{
					if (bBoundary(x, y, z))
					//if(m_buff[z*m_zCoefficient + y*m_yCoefficient + x]==1&& bBoundary(x, y, z))   
					{
						float fz = toDoubleZ(z);
						cells->InsertNextCell(8);
						ugpoints->InsertNextPoint((x - 1)*m_resolution + m_cube.min.x, (y - 1)*m_resolution + m_cube.min.y, (fz));//�����������ʵ������ֵ
						cells->InsertCellPoint(pointsId++);

						ugpoints->InsertNextPoint((x)*m_resolution + m_cube.min.x, (y - 1)*m_resolution + m_cube.min.y, (fz));
						cells->InsertCellPoint(pointsId++);

						ugpoints->InsertNextPoint((x - 1)*m_resolution + m_cube.min.x, (y)*m_resolution + m_cube.min.y, (fz));
						cells->InsertCellPoint(pointsId++);

						ugpoints->InsertNextPoint((x)*m_resolution + m_cube.min.x, (y)*m_resolution + m_cube.min.y, (fz));
						cells->InsertCellPoint(pointsId++);

						ugpoints->InsertNextPoint((x - 1)*m_resolution + m_cube.min.x, (y - 1)*m_resolution + m_cube.min.y, (fz)+m_resolution);
						cells->InsertCellPoint(pointsId++);

						ugpoints->InsertNextPoint((x)*m_resolution + m_cube.min.x, (y - 1)*m_resolution + m_cube.min.y, (fz)+m_resolution);
						cells->InsertCellPoint(pointsId++);

						ugpoints->InsertNextPoint((x - 1)*m_resolution + m_cube.min.x, (y)*m_resolution + m_cube.min.y, (fz)+m_resolution);
						cells->InsertCellPoint(pointsId++);

						ugpoints->InsertNextPoint((x)*m_resolution + m_cube.min.x, (y)*m_resolution + m_cube.min.y, (fz)+m_resolution);
						cells->InsertCellPoint(pointsId++);
						
					}
					
				}
			}
		}
	}
	ug->SetPoints(ugpoints);
	ug->SetCells(VTK_VOXEL, cells);

	/*vtkSmartPointer<vtkDataSetMapper> ugmapper = vtkSmartPointer<vtkDataSetMapper>::New();
	vtkSmartPointer<vtkActor> ugactor = vtkSmartPointer<vtkActor>::New();
	ugmapper->SetInputData(ug);
	ugactor->SetMapper(ugmapper);*/
}
void CZ3DRepresentation::glDraw(vtkSmartPointer<vtkPoints> ugpoints, vtkSmartPointer<vtkUnstructuredGrid> ug, vtkSmartPointer<vtkCellArray> cells, double px, double py, double pz)
{
	int pointsId = 0;
	for (int z = 1; z < m_maxZ; z++)
	{
		for (int y = 1; y < m_maxY; y++)
		{
			for (int x = 1; x < m_maxX; x++)
			{
				if (bDotInSolid(x, y, z))
				{
					if (bBoundary(x, y, z))				 
					{
						
						float fz = toDoubleZ(z);
						cells->InsertNextCell(8);
						ugpoints->InsertNextPoint((x - 1)*m_resolution +px, (y - 1)*m_resolution+py, (fz)+pz);//�����������ʵ������ֵ
						cells->InsertCellPoint(pointsId++);

						ugpoints->InsertNextPoint((x)*m_resolution + px, (y - 1)*m_resolution + py, (fz)+pz);
						cells->InsertCellPoint(pointsId++);

						ugpoints->InsertNextPoint((x - 1)*m_resolution + px, (y)*m_resolution + py, (fz)+pz);
						cells->InsertCellPoint(pointsId++);

						ugpoints->InsertNextPoint((x)*m_resolution + px, (y)*m_resolution + py, (fz)+pz);
						cells->InsertCellPoint(pointsId++);

						ugpoints->InsertNextPoint((x - 1)*m_resolution + px, (y - 1)*m_resolution + py, (fz)+m_resolution + pz);
						cells->InsertCellPoint(pointsId++);

						ugpoints->InsertNextPoint((x)*m_resolution + px, (y - 1)*m_resolution + py, (fz)+m_resolution + pz);
						cells->InsertCellPoint(pointsId++);

						ugpoints->InsertNextPoint((x - 1)*m_resolution + px, (y)*m_resolution + py, (fz)+m_resolution + pz);
						cells->InsertCellPoint(pointsId++);

						ugpoints->InsertNextPoint((x)*m_resolution + px, (y)*m_resolution + py, (fz)+m_resolution + pz);
						cells->InsertCellPoint(pointsId++);
					}

				}
			}
		}
	}
	ug->SetPoints(ugpoints);
	ug->SetCells(VTK_VOXEL, cells);
}
//void CZ3DRepresentation::glDrawMain(vtkSmartPointer<vtkPoints> ugpoints, vtkSmartPointer<vtkUnstructuredGrid> ug, vtkSmartPointer<vtkCellArray> cells, float h)        //�����ӹ��йأ��Ȳ���
//{
//	int pointsId = 0;
//	for (int z = 1; z < m_maxZ; z++)
//	{
//		float fz = toDoubleZ(z);
//		if (fz>=h-m_toolRadius-m_resolution && fz<=h+m_toolRadius+m_resolution)
//		{
//			continue;
//		}
//		for (int y = 1; y < m_maxY; y++)
//		{
//			for (int x = 1; x < m_maxX; x++)
//			{
//				if (bDotInSolid(x, y, z))
//				{
//					if (bBoundary(x, y, z))
//					{
//						
//						cells->InsertNextCell(8);
//						ugpoints->InsertNextPoint((x - 1)*m_resolution + m_cube.min.x, (y - 1)*m_resolution + m_cube.min.y, (fz));
//						cells->InsertCellPoint(pointsId++);
//
//						ugpoints->InsertNextPoint((x)*m_resolution + m_cube.min.x, (y - 1)*m_resolution + m_cube.min.y, (fz));
//						cells->InsertCellPoint(pointsId++);
//
//						ugpoints->InsertNextPoint((x - 1)*m_resolution + m_cube.min.x, (y)*m_resolution + m_cube.min.y, (fz));
//						cells->InsertCellPoint(pointsId++);
//
//						ugpoints->InsertNextPoint((x)*m_resolution + m_cube.min.x, (y)*m_resolution + m_cube.min.y, (fz));
//						cells->InsertCellPoint(pointsId++);
//
//						ugpoints->InsertNextPoint((x - 1)*m_resolution + m_cube.min.x, (y - 1)*m_resolution + m_cube.min.y, (fz)+m_resolution);
//						cells->InsertCellPoint(pointsId++);
//
//						ugpoints->InsertNextPoint((x)*m_resolution + m_cube.min.x, (y - 1)*m_resolution + m_cube.min.y, (fz)+m_resolution);
//						cells->InsertCellPoint(pointsId++);
//
//						ugpoints->InsertNextPoint((x - 1)*m_resolution + m_cube.min.x, (y)*m_resolution + m_cube.min.y, (fz)+m_resolution);
//						cells->InsertCellPoint(pointsId++);
//
//						ugpoints->InsertNextPoint((x)*m_resolution + m_cube.min.x, (y)*m_resolution + m_cube.min.y, (fz)+m_resolution);
//						cells->InsertCellPoint(pointsId++);
//					}
//				}
//			}
//		}
//	}
//	ug->SetPoints(ugpoints);
//	ug->SetCells(VTK_VOXEL, cells);
//
//	vtkSmartPointer<vtkDataSetMapper> ugmapper = vtkSmartPointer<vtkDataSetMapper>::New();
//	vtkSmartPointer<vtkActor> ugactor = vtkSmartPointer<vtkActor>::New();
//	ugmapper->SetInputData(ug);
//	ugactor->SetMapper(ugmapper);
//}
//void CZ3DRepresentation::glDrawCurrent(vtkSmartPointer<vtkPoints> ugpoints, vtkSmartPointer<vtkUnstructuredGrid> ug, vtkSmartPointer<vtkCellArray> cells, float h)         //�����ӹ��йأ��Ȳ���
//{
//	int pointsId = 0;
//	for (int z = 1; z < m_maxZ; z++)
//	{
//		float fz = toDoubleZ(z);
//		if (!(fz >= h - m_toolRadius - m_resolution && fz <= h + m_toolRadius + m_resolution))
//		{
//			continue;
//		}
//		for (int y = 1; y < m_maxY; y++)
//		{
//			for (int x = 1; x < m_maxX; x++)
//			{
//				if (bDotInSolid(x, y, z))
//				{
//					if (bBoundary(x, y, z))
//					{
//
//						cells->InsertNextCell(8);
//						ugpoints->InsertNextPoint((x - 1)*m_resolution + m_cube.min.x, (y - 1)*m_resolution + m_cube.min.y, (fz));
//						cells->InsertCellPoint(pointsId++);
//
//						ugpoints->InsertNextPoint((x)*m_resolution + m_cube.min.x, (y - 1)*m_resolution + m_cube.min.y, (fz));
//						cells->InsertCellPoint(pointsId++);
//
//						ugpoints->InsertNextPoint((x - 1)*m_resolution + m_cube.min.x, (y)*m_resolution + m_cube.min.y, (fz));
//						cells->InsertCellPoint(pointsId++);
//
//						ugpoints->InsertNextPoint((x)*m_resolution + m_cube.min.x, (y)*m_resolution + m_cube.min.y, (fz));
//						cells->InsertCellPoint(pointsId++);
//
//						ugpoints->InsertNextPoint((x - 1)*m_resolution + m_cube.min.x, (y - 1)*m_resolution + m_cube.min.y, (fz)+m_resolution);
//						cells->InsertCellPoint(pointsId++);
//
//						ugpoints->InsertNextPoint((x)*m_resolution + m_cube.min.x, (y - 1)*m_resolution + m_cube.min.y, (fz)+m_resolution);
//						cells->InsertCellPoint(pointsId++);
//
//						ugpoints->InsertNextPoint((x - 1)*m_resolution + m_cube.min.x, (y)*m_resolution + m_cube.min.y, (fz)+m_resolution);
//						cells->InsertCellPoint(pointsId++);
//
//						ugpoints->InsertNextPoint((x)*m_resolution + m_cube.min.x, (y)*m_resolution + m_cube.min.y, (fz)+m_resolution);
//						cells->InsertCellPoint(pointsId++);
//					}
//				}
//			}
//		}
//	}
//	ug->SetPoints(ugpoints);
//	ug->SetCells(VTK_VOXEL, cells);
//
//	vtkSmartPointer<vtkDataSetMapper> ugmapper = vtkSmartPointer<vtkDataSetMapper>::New();
//	vtkSmartPointer<vtkActor> ugactor = vtkSmartPointer<vtkActor>::New();
//	ugmapper->SetInputData(ug);
//	ugactor->SetMapper(ugmapper);
//}
//void CZ3DRepresentation::glDrawNext(vtkSmartPointer<vtkPoints> ugpoints, vtkSmartPointer<vtkUnstructuredGrid> ug, vtkSmartPointer<vtkCellArray> cells, float h)    //�����ӹ��йأ��Ȳ���
//{
//	int pointsId = 0;
//	for (int z = 1; z < m_maxZ; z++)
//	{
//		float fz = toDoubleZ(z);
//		if (!(fz >= h + m_toolRadius + m_resolution && fz <= h + 1.5*m_toolRadius + m_resolution))
//		{
//			continue;
//		}
//		for (int y = 1; y < m_maxY; y++)
//		{
//			for (int x = 1; x < m_maxX; x++)
//			{
//				if (bDotInSolid(x, y, z))
//				{
//					if (bBoundary(x, y, z))
//					{
//
//						cells->InsertNextCell(8);
//						ugpoints->InsertNextPoint((x - 1)*m_resolution + m_cube.min.x, (y - 1)*m_resolution + m_cube.min.y, (fz));
//						cells->InsertCellPoint(pointsId++);
//
//						ugpoints->InsertNextPoint((x)*m_resolution + m_cube.min.x, (y - 1)*m_resolution + m_cube.min.y, (fz));
//						cells->InsertCellPoint(pointsId++);
//
//						ugpoints->InsertNextPoint((x - 1)*m_resolution + m_cube.min.x, (y)*m_resolution + m_cube.min.y, (fz));
//						cells->InsertCellPoint(pointsId++);
//
//						ugpoints->InsertNextPoint((x)*m_resolution + m_cube.min.x, (y)*m_resolution + m_cube.min.y, (fz));
//						cells->InsertCellPoint(pointsId++);
//
//						ugpoints->InsertNextPoint((x - 1)*m_resolution + m_cube.min.x, (y - 1)*m_resolution + m_cube.min.y, (fz)+m_resolution);
//						cells->InsertCellPoint(pointsId++);
//
//						ugpoints->InsertNextPoint((x)*m_resolution + m_cube.min.x, (y - 1)*m_resolution + m_cube.min.y, (fz)+m_resolution);
//						cells->InsertCellPoint(pointsId++);
//
//						ugpoints->InsertNextPoint((x - 1)*m_resolution + m_cube.min.x, (y)*m_resolution + m_cube.min.y, (fz)+m_resolution);
//						cells->InsertCellPoint(pointsId++);
//
//						ugpoints->InsertNextPoint((x)*m_resolution + m_cube.min.x, (y)*m_resolution + m_cube.min.y, (fz)+m_resolution);
//						cells->InsertCellPoint(pointsId++);
//					}
//				}
//			}
//		}
//	}
//	ug->SetPoints(ugpoints);
//	ug->SetCells(VTK_VOXEL, cells);
//
//	vtkSmartPointer<vtkDataSetMapper> ugmapper = vtkSmartPointer<vtkDataSetMapper>::New();
//	vtkSmartPointer<vtkActor> ugactor = vtkSmartPointer<vtkActor>::New();
//	ugmapper->SetInputData(ug);
//	ugactor->SetMapper(ugmapper);
//}

//ѡ��Ĭ�ϵ�ɨ����
ILineScan& getDefaultILineScan(void);


//�γ���ɢ����ʵ���ʾ-----------------------------------------------------------
//p_solid ԭʵ��
//���������İ�ʵ�����ػ���֮���buff�ж�Ӧ��ʵ�岿����ӦԪ�ظ�Ϊ1.
//˵�������������У�������ʶ�߽�����ɲ㣩ֻ���õ�  1��m_maxX-1; 1��m_maxY-1; 1��m_maxZ-1;(��ͷ�ͽ�β��Ԫ�ز���Ҫ��)
void CZ3DRepresentation::makeSolid(vtkPolyData *p_solid, Byte p_orMask, Byte p_andMask)//1,0
{
	assert(bInited());
	assert(p_solid->GetNumberOfCells()!=0);
	vtkSmartPointer<vtkPolyData> slice = vtkSmartPointer<vtkPolyData>::New();
	vtkSmartPointer<vtkCutter> p_cutter = vtkSmartPointer<vtkCutter>::New();
	vtkSmartPointer<vtkPlane> plane = vtkSmartPointer<vtkPlane>::New();             
	plane->SetOrigin(0, 0, 0);//�趨һ����ֱ��z���ƽ��
	plane->SetNormal(0, 0, 1);

	//double minBound[3];//GetBounds()��ȡ��Χ�е�x/y/z�����ֵ����Сֵ
	//minBound[0] = p_solid->GetBounds()[0];
	//minBound[1] = p_solid->GetBounds()[2];                   
	//minBound[2] = p_solid->GetBounds()[4];
	//double maxBound[3];
	//maxBound[0] = p_solid->GetBounds()[1];
	//maxBound[1] = p_solid->GetBounds()[3];
	//maxBound[2] = p_solid->GetBounds()[5];

	for (double  h = m_cube.min.z+0.1f*m_resolution; h <m_cube.max.z; h+=m_resolution)
	{
		//����h��ֵ��cube����Сzֵ�����zֵ����Ƭ�����������صı߳���
		//cube��������ȫ�ֱ���������ֵ��init�����г�ʼ���ˣ������ֵ���ƶ�����һ��float���͵�ʵ�壨����С���ƶ�����ԭ�㣩���ƶ�֮���ֽ���ƫ�ò��������������Сֵ�Ǹ���
		
		//����0.1f��ֻд0.1Ĭ����double���͵����ݣ�д0.1f�Ǳ�ʾfloat���͵�����
		//������ͣ�float�����ǵ����ȣ��ڴ���ռ4���ֽڣ�������7λ����һλ��������
		//double������˫���ȣ����ȸߣ�����ռ���ڴ�������ٶ���������float�Ͳ���double��
		{
			//������Ƭ������
			p_cutter->SetCutFunction(plane);//p_cutter��һ����Ƭ��������ƽ��ȥ��һ��ʵ��                 
			p_cutter->SetInputData(p_solid);
			p_cutter->GenerateValues(1, h, h);
			p_cutter->Update();
			slice = p_cutter->GetOutput();
		}
		scan(h, slice, p_orMask, p_andMask);//1,0
	}
}
//ɨ���ߺ�������Ϊ���ṩ���
void CZ3DRepresentation::scan(double p_z, vtkSmartPointer<vtkPolyData>p_slice, Byte p_orMask, Byte p_andMask)
{
	assert(bInited());
	int z = toIntZ(p_z);
	//assert(in(z, 0, m_maxZ));

	scanOptionX.rect.min = point2(float(p_slice->GetBounds()[0]), float(p_slice->GetBounds()[2]));       //�趨ɨ������
	scanOptionX.rect.max = point2(float(p_slice->GetBounds()[1]), float((p_slice->GetBounds()[3])));
	scanOptionX.intver = float(m_resolution/2);                     //�趨ɨ�貽��
	scanOptionX.nIgnoredFrame = /*p_slice.getFrameCount()*/0;   //���������ڿ�
	CZGrid scanGridX;

	scanGridX.make(&getDefaultILineScan(), scanOptionX, p_slice);

	const CZScanLines &lines = scanGridX.lines[scanX];
	for (int i = 0; i < lines.size(); ++i)
	{
		const CZScanLine &line = lines[i]; //��i��Xɨ����
		int y = toIntY(double(line.getMainCord()));
		for (int j = 0; j < (line.pointsCount()-1); j += 2)
		{
			int xStart = toIntX(double(line.subCords[j]));
			int xEnd = toIntX(double(line.subCords[j+1]));
			for(int x=xStart; x<=xEnd; x++)
			{
				Byte& mask=dot(x, y, z);            
				mask &= p_andMask;
				mask |= p_orMask;
			}
		}
	}
}


//��ʾ�߽�-----------------------------------------------------------------------
//�����ڱ߽��buff����Ԫ�ص�ֵ��2����һ���ֽڵĴ��ұ����ڶ�λ��ֵ��Ϊ1����
//������ͣ�0xfeΪ�����Ƶ�1111 1110��&��λ�룬ͬΪ1��Ϊ1��|��λ����һ��Ϊ1����1.(ox��ͷ��ʾʮ������)
//dotxyz&0xfe|dmBoundary���ͣ�dotxyzԭ��Ϊ1����0000 0001������oxfeλ����Ϊ0��0000 0000��������dmBoundary��2����0000 0010��λ�򣬱��2.
void CZ3DRepresentation::maskBoundary(void)
{
	int to=0;
	for(int z=1; z<m_maxZ; z++)
	{
		for(int y=1; y<m_maxY; y++)
		{
			for(int x=1; x<m_maxX; x++)
			{
				if(bDotInSolid(x, y, z))
				{
					if(bBoundary(x, y, z))
					{
						Byte & dotxyz=dot(x, y, z);
						dotxyz = dotxyz&0xfe|dmBoundary;//����ֵ��Ϊ2����0000 0010.
						to++;
					}
				}
			}

		}
	}
}


//�������-----------------------------------------------------------------------
//���ҳ����ϲ��������ƽ�棬����·���滮��
//Ȼ������ʵ���ȥ�Ѿ��滮�Ĳ��֣�
//�ظ��������ϲ��������ƽ�棬�ظ��õ�ʣ�ಿ�֣�ֱ���������ʵ���·���滮
void CZ3DRepresentation::makeTopCurvedLayer(void)
{
	for(int z=1; z<m_maxZ; z++)
	{
		for(int y=1; y<m_maxY; y++)
		{
			for(int x=1; x<m_maxX; x++)
			{
				if(bDotInSolid(x, y, z))
				{
					if(bTopBoundary(x, y, z))
					{
						Byte &dotxyz = m_buff_CurvedLayer[z*m_zCoefficient + y*m_yCoefficient + x];
						dotxyz = dmSolid;
					}
				}
			}
		}
	}
}

void CZ3DRepresentation::makeBottomCurvedLayer(void)
{
	for (int z = 1; z < m_maxZ; z++)
	{
		for (int y = 1; y < m_maxY; y++)
		{
			for (int x = 1; x < m_maxX; x++)
			{
				if (bDotInSolid(x, y, z))
				{
					if (bBottomBoundary(x, y, z))
					{
						Byte &dotxyz = m_buff_CurvedLayer[z*m_zCoefficient + y*m_yCoefficient + x];
						dotxyz = dmSolid;
					}
				}
			}
		}
	}
}


void CZ3DRepresentation::makeTopCurvedLayer2(Byte* a, int shift_x, int shift_y, int shift_z, int x1, int y1, int z1)
{
	//for (int z = 1; z < z1; z++)
	//{
	//	for (int y = 1; y < y1; y++)
	//	{
	//		for (int x =1; x < x1; x++)
	//		{
	//			if (a[z*m_zCoefficient + y*m_yCoefficient + x] ==1)
	//			{
	//				
	//				if (z + shift_z <m_maxZ && z + shift_z + 1>0 && y + shift_y< m_maxY && y + shift_y >0 && x + shift_x <m_maxX && x + shift_x >0)
	//				{
	//					a[(z + shift_z)*m_zCoefficient + (y + shift_y)*m_yCoefficient + (x + shift_x)] = 2;
	//				}
	//				/*if (z - shift_z < z1 && z - shift_z + 1 > 0 && y - shift_y < y1 && y - shift_y >0 && x - shift_x < x1 && x - shift_x >0)
	//				{
	//					a[(z - shift_z)*m_zCoefficient + (y - shift_y)*m_yCoefficient + (x - shift_x)] = 2;
	//				}*/
	//			}
	//			
	//			
	//			
	//		}
	//	}
	//}
	
	

	//for (int z = 1; z < m_maxZ; z++)
	//{
	//	for (int y = 1; y < m_maxY; y++)
	//	{
	//		for (int x = 1; x < m_maxX; x++)
	//		{
	//			if (z < z1 && y < y1 && x < x1 && a[z*m_zCoefficient + y*m_yCoefficient + x] == 1)
	//			{
	//				m_buff_CurvedLayer[z*m_zCoefficient + y*m_yCoefficient + x] = 1;
	//			}
	//		}
	//	}
	//}
}

void CZ3DRepresentation::makeZCurvedLayer(int zFromToP)
{
	//��m_buff_CurvedLayer2����Ϊ0
	unsigned long long  totalsize = ((unsigned long long)m_maxX + 1)*
		((unsigned long long)m_maxY + 1)*((unsigned long long)m_maxZ + 1);
	memset(m_buff_CurvedLayer2, 0, totalsize);

	for (int z = zFromToP; z < m_maxZ; z++)
	{
		for (int x = 1; x < m_maxX; x++)
		{
			for (int y = 1; y < m_maxY; y++)
			{
				m_buff_CurvedLayer2[(z - zFromToP)*m_zCoefficient + y*m_yCoefficient + x] = m_buff_CurvedLayer[z*m_zCoefficient + y*m_yCoefficient + x];
			}
		}
	}
	
}

void CZ3DRepresentation::makeZCurvedLayer2(int zFromBottom)
{
	//��m_buff_CurvedLayer2����Ϊ0
	unsigned long long  totalsize = ((unsigned long long)m_maxX + 1)*
		((unsigned long long)m_maxY + 1)*((unsigned long long)m_maxZ + 1);
	memset(m_buff_CurvedLayer2, 0, totalsize);

	for (int z = m_maxZ- zFromBottom; z> 1;  z--)
	{
		for (int x = 1; x < m_maxX; x++)
		{
			for (int y = 1; y < m_maxY; y++)
			{
				m_buff_CurvedLayer2[(z + zFromBottom)*m_zCoefficient + y*m_yCoefficient + x] = m_buff_CurvedLayer[z*m_zCoefficient + y*m_yCoefficient + x];
			}
		}
	}

}

void CZ3DRepresentation::getCurvedLayer(Byte *buff)
{
	for (int z = 1; z < m_maxZ; z++)
	{
		for (int y = 1; y < m_maxY; y++)
		{
			for (int x = 1; x < m_maxX; x++)
			{
				buff[z*m_zCoefficient + y*m_yCoefficient + x] = m_buff[z*m_zCoefficient + y*m_yCoefficient + x] & m_buff_CurvedLayer2[z*m_zCoefficient + y*m_yCoefficient + x];
			}
		}
	}
}


void CZ3DRepresentation::makeGrowthLineCurveLayer(double d, Buff_LineCurvedLayer &m_buff_LineCurvedLayer)
{
	int countLayer = ceil(m_maxZ / (d / m_resolution))+1;//����ȡ�������������Ҫ�Ĳ���
	for (int k = 0; k < countLayer; k++)
	{
		Byte* buffNew;
		unsigned long long  totalsize = ((unsigned long long)m_maxX + 1)*
			((unsigned long long)m_maxY + 1)*((unsigned long long)m_maxZ + 1);
		buffNew = new Byte[totalsize];
		memset(buffNew, 0, totalsize);
		m_buff_LineCurvedLayer.push_back(buffNew);
	}

	for (int x = 0; x < m_maxX; x+=1)//�������ܶȣ����ؾ��ȵ�4��.�����������㹻�ܼ�
	{
		for (int y = 0; y < m_maxY; y+=1)
		{
			//��ȡÿһ��XYֵ����Ӧ�������ߵĽ���
			//������XYֵ��Ӧ�������߶�vector����
			vector<int > startPointZ;//��ʼ��
			vector<int >endPointZ;//��ֹ��

			//���µ��ϼ���
			//for (int z = 1; z < m_maxZ-1; z++)
			//{
			//	//�жϸõ㼰ǰ���������Ƿ���ʵ���ڲ�
			//	if (m_buff[(z-1)*m_zCoefficient + y*m_yCoefficient + x] == 0 && m_buff[z*m_zCoefficient + y*m_yCoefficient + x] == 1 && m_buff[(z + 1)*m_zCoefficient + y*m_yCoefficient + x] == 1)//ǰһ���㲻��ʵ���ڣ���һ������ʵ����
			//	{
			//		startPointZ.push_back(z);
			//	}
			//	else if (m_buff[(z - 1)*m_zCoefficient + y*m_yCoefficient + x] == 1 && m_buff[z*m_zCoefficient + y*m_yCoefficient + x] == 1 && m_buff[(z + 1)*m_zCoefficient + y*m_yCoefficient + x] == 0)
			//	{
			//		endPointZ.push_back(z);
			//	}
			//}

			//if (startPointZ.size() > 0)
			//{
			//	int numLayer = 0;//���������㵱ǰ�����ڵĲ���
			//	for (int p = 0; p < startPointZ.size(); p ++)
			//	{
			//		int distanceAB = endPointZ.at(p) - startPointZ.at(p);//���㽻�߶εĳ���
			//		int D = d / m_resolution;
			//		for (int q = startPointZ.at(p); q < endPointZ.at(p); q+=D )
			//		{
			//			m_buff_LineCurvedLayer.at(numLayer)[q*m_zCoefficient + y*m_yCoefficient + x] = 1;
			//			numLayer++;
			//		}
			//		m_buff_LineCurvedLayer.at(numLayer)[endPointZ.at(p)*m_zCoefficient + y*m_yCoefficient + x] = 1;//�����һ�������
			//		numLayer++;
			//	}		
			//}

			//���ϵ��¼���
			for (int z = m_maxZ-2; z > 0; z--)
			{
				//�жϸõ㼰ǰ���������Ƿ���ʵ���ڲ�
				if (m_buff[(z - 1)*m_zCoefficient + y*m_yCoefficient + x] == 0 && m_buff[z*m_zCoefficient + y*m_yCoefficient + x] == 1 && m_buff[(z + 1)*m_zCoefficient + y*m_yCoefficient + x] == 1)//ǰһ���㲻��ʵ���ڣ���һ������ʵ����
				{
					endPointZ.push_back(z);
				}
				else if (m_buff[(z - 1)*m_zCoefficient + y*m_yCoefficient + x] == 1 && m_buff[z*m_zCoefficient + y*m_yCoefficient + x] == 1 && m_buff[(z + 1)*m_zCoefficient + y*m_yCoefficient + x] == 0)
				{
					startPointZ.push_back(z);
				}
			}

			if (startPointZ.size() > 0)
			{
				int numLayer = 0;//���������㵱ǰ�����ڵĲ���
				for (int p = 0; p < startPointZ.size(); p++)
				{
					int distanceAB = startPointZ.at(p) - endPointZ.at(p);//���㽻�߶εĳ���
					int D = d / m_resolution;
					for (int q = startPointZ.at(p); q > endPointZ.at(p); q -= D)
					{
						m_buff_LineCurvedLayer.at(numLayer)[q*m_zCoefficient + y*m_yCoefficient + x] = 1;
						numLayer++;
					}
					m_buff_LineCurvedLayer.at(numLayer)[endPointZ.at(p)*m_zCoefficient + y*m_yCoefficient + x] = 1;//�����һ�������
					numLayer++;
				}
			}


			startPointZ.clear();
			endPointZ.clear();
		}//������ǰ�����ߵı���
	}
}

void CZ3DRepresentation::makeGrowthLineVirousCurveLayer(double dmin, double dmax, Buff_LineCurvedLayer &m_buff_LineCurvedLayer)
{
	//���߶����ֵ
	int maxdistance = 0;
	for (int x = 0; x < m_maxX; x += 1)//�������ܶ�Ϊ1�����ؾ��ȣ������������㹻�ܼ���ʵ�ʿ�����4�������ؾ������ԡ�
	{
		for (int y = 0; y < m_maxY; y += 1)
		{
			vector<int > startPointZ;//��ʼ��
			vector<int >endPointZ;//��ֹ��
			for (int z = 1; z < m_maxZ - 1; z++)
			{
				if (m_buff[(z - 1)*m_zCoefficient + y*m_yCoefficient + x] == 0 && m_buff[z*m_zCoefficient + y*m_yCoefficient + x] == 1 && m_buff[(z + 1)*m_zCoefficient + y*m_yCoefficient + x] == 1)//ǰһ���㲻��ʵ���ڣ���һ������ʵ����
				{
					startPointZ.push_back(z);
				}
				else if (m_buff[(z - 1)*m_zCoefficient + y*m_yCoefficient + x] == 1 && m_buff[z*m_zCoefficient + y*m_yCoefficient + x] == 1 && m_buff[(z + 1)*m_zCoefficient + y*m_yCoefficient + x] == 0)
				{
					endPointZ.push_back(z);
				}
			}

			if (startPointZ.size() > 0)
			{
				for (int p = 0; p < startPointZ.size(); p++)
				{
					int distanceAB = endPointZ.at(p) - startPointZ.at(p);//���㽻�߶εĳ���

					if (distanceAB > maxdistance)
					{
						maxdistance = distanceAB;
					}
				}
			}

			startPointZ.clear();
			endPointZ.clear();
		}
	}

	//�����ݷֳ���������
	vector <int > range;
	double dmin_voxel = dmin / m_resolution;
	double dmax_voxel = dmax / m_resolution;
	range.push_back(maxdistance);
	while (maxdistance > 2*floor(dmax_voxel))
	{
		maxdistance = ceil((double)maxdistance * (dmin_voxel / dmax_voxel));
		range.push_back(maxdistance);
	}
	range.push_back(floor(dmax_voxel));
	range.push_back(0);


	int countLayer = ceil((double)range.at(0) / dmax_voxel);//����ȡ�������������Ҫ�Ĳ���
	for (int k = 0; k < countLayer; k++)
	{
		Byte* buffNew;
		unsigned long long  totalsize = ((unsigned long long)m_maxX + 1)*
			((unsigned long long)m_maxY + 1)*((unsigned long long)m_maxZ + 1);
		buffNew = new Byte[totalsize];
		memset(buffNew, 0, totalsize);
		m_buff_LineCurvedLayer.push_back(buffNew);
	}

	for (int x = 0; x < m_maxX; x += 1)//�������ܶȣ����ؾ��ȵ�4��.�����������㹻�ܼ�
	{
		for (int y = 0; y < m_maxY; y += 1)
		{
			//��ȡÿһ��XYֵ����Ӧ�������ߵĽ���
			//������XYֵ��Ӧ�������߶�vector����
			vector<int > startPointZ;//��ʼ��
			vector<int >endPointZ;//��ֹ��

			 //���ϵ��¼���
			for (int z = m_maxZ - 2; z > 0; z--)
			{
				//�жϸõ㼰ǰ���������Ƿ���ʵ���ڲ�
				if (m_buff[(z - 1)*m_zCoefficient + y*m_yCoefficient + x] == 0 && m_buff[z*m_zCoefficient + y*m_yCoefficient + x] == 1 && m_buff[(z + 1)*m_zCoefficient + y*m_yCoefficient + x] == 1)//ǰһ���㲻��ʵ���ڣ���һ������ʵ����
				{
					endPointZ.push_back(z);
				}
				else if (m_buff[(z - 1)*m_zCoefficient + y*m_yCoefficient + x] == 1 && m_buff[z*m_zCoefficient + y*m_yCoefficient + x] == 1 && m_buff[(z + 1)*m_zCoefficient + y*m_yCoefficient + x] == 0)
				{
					startPointZ.push_back(z);
				}
			}

			if (startPointZ.size() > 0)
			{
				int numLayer = 0;//���������㵱ǰ�����ڵĲ���
				for (int p = 0; p < startPointZ.size(); p++)
				{
					int distanceAB = startPointZ.at(p) - endPointZ.at(p);//���㽻�߶εĳ���
					
					//�ҳ��ý��߶ε�ֵ���ڵ�����
					int layerCout;//�ý��߶εĲ��
					for (int i = 0; i < range.size()-1; i++)
					{
						if (distanceAB <= range.at(i) && distanceAB > range.at(i+1))
						{
							layerCout = ceil((double)range.at(i) / dmax_voxel);
							break;//ֹͣ��ǰforѭ��
						}
					}
					
					for (int i = 1; i <= layerCout; i ++)
					{
						int currentZ = startPointZ.at(p) - ceil((double)(distanceAB*i) / (double)layerCout);
						m_buff_LineCurvedLayer.at(numLayer)[currentZ*m_zCoefficient + y*m_yCoefficient + x] = 1;
						numLayer++;
					}
				}
			}


			startPointZ.clear();
			endPointZ.clear();
		}//������ǰ�����ߵı���
	}

}

//�������ϲ�ƽ��
void CZ3DRepresentation::makeTopFlatLayer(void)
{

}
//���ɿ��壬���Ѿ�ת��Ϊ���ϲ�����ر�Ϊ��.ÿ������һ�����ϲ����أ��滮��·���Ͱ�����ɿ���
void CZ3DRepresentation::makeAirSolid(void)
{
	for(int z=1; z<m_maxZ; z++)
	{
		for(int y=1; y<m_maxY; y++)
		{
			for(int x=1; x<m_maxX; x++)
			{
				Byte & dotxyz=dot(x, y, z);
				if (dotxyz ==4)
				{
					dotxyz = 0;
				}
			}
		}
	}
}

//����������������---------------------------------------------------------------
//��������򵥵ĵײ㵶������
//�����������洢��������һ��int���͵Ķ�ά��ͨ���飬��Ϊ��������С����֪��
//void makeToolSurface(CurveArray p_curveArray)
//{
//	CurveArray curveArray;
//	unsigned long long  total = ((unsigned long long)m_maxX + 1) *((unsigned long long)m_maxY + 1);//��֤��ȡ��0-max
//	memset(curveArray, 0, total);
//	
//	for (int x = 1; x < m_maxX; x++)
//	{
//		for (int y = 1; y < m_maxY; y++)
//		{
//			for (int z = 1; z < m_maxZ; z++)
//			{
//				if (bDotInSolid(x, y, z))
//				{
//					if (bTopBoundary(x, y, z))
//					{
//						//ע�⵱ǰ������±��Ƿ���ȷ
//						curveArray[(x-1)(m_maxY+1) + y] = z;
//						break;
//					}
//				}
//			}
//		}
//	}
//
//	//�����Ƚ�����ֵ�ж�,ȥ��������ֵ�����ϲ����沿��
//	//ֱ�ӽ���ֵ��֮ǰ������ֵ�����жϣ������Ƕ��٣������趨һ���涨������
//	//�ж�֮��Ѻ��ʵ�ֵ���£�����zֵ��������Ԫ��
//	int interval = 2/m_resolution;
//	//�ж�X������
//	double m_XAngle = 0.0;
//	for (int x = 1; x < m_maxX; x++)
//	{
//		int num = 0;
//		for (int y = 1; y < m_maxY; y++)
//		{
//			if (curveArray[(x-1)(m_maxY+1) + y]>0)
//			{
//				num++;
//				if (num ==interval+1)//����ж���Ϊ��ȥ��Xmin֮ǰ�����ϲ�����
//				{
//					m_XAngle = atan( double( (curveArray[(x-1)(m_maxY+1) + y] - curveArray[(x-1)(m_maxY+1) + y - interval])/interval ));
//					if (m_XAngle < -3.14159265/4 || m_XAngle > 3.14159265/4)
//					{
//						//��ʱ��1-interval+1�⼸��Ԫ�ص�ֵ��Ϊ0
//						for (int i = 0; i < interval+1; i++)
//						{
//							curveArray[(x-1)(m_maxY+1) + y - i] =0;
//						}
//					}
//				}
//	
//				else if (num >interval+1)//�����ж�x����ĵĲ���
//				{
//					m_XAngle = atan( double( (curveArray[(x-1)(m_maxY+1) + y] - curveArray[(x-1)(m_maxY+1) + y - interval])/interval ));
//					if (m_XAngle < -3.14159265/4 || m_XAngle > 3.14159265/4)
//					{
//						curveArray[(x-1)(m_maxY+1) + y] =0;
//					}
//				}
//			}
//		}
//	}
//
//	//����������ʵ����δ����ֵ��Ԫ�أ���ֵΪ0��Ԫ�أ�
//	//�Ѿ������
//	for ()
//	{
//
//	}
//}


//�γ�֧��-----------------------------------------------------------------------
// p_type ֧�ŵ�����, 
//p_diameter ֧�ŵ�ֱ������, 
//p_interval ����֧�ż�����ļ��
//p_offset ֧�Ŵӱ�Ե����ƫ�Ƶ�ʵ�ʾ���
void CZ3DRepresentation::makeSupportMatrix(CSupportType p_type, double p_diameter, double p_interval, 
		CPoint2d p_offset) 
{
	int row =(int) (m_cube.h()/p_interval);      //y�᷽��֧����Ŀ
	int arrange = (int)(m_cube.w()/p_interval);  //x�᷽��֧����Ŀ
	CPoint3i d;
	for(d.z=1; d.z<m_maxZ; d.z++)
	{
		for(int i=0; i<row; i++)          //y��
		{
			for(int j=0; j<arrange; j++)  //x��
			{
				if(p_type==stSquare)
				{
					int minx=toIntX(j*p_interval+p_interval/2-p_diameter/4);
					int maxx=minx+(int)(p_diameter/m_resolution);
					int miny=toIntY(i*p_interval+p_interval/2-p_diameter/4);
					int maxy=miny+(int)(p_diameter/m_resolution);
					for(d.y=miny; d.y<maxy-1; d.y++)
						for(d.x=minx; d.x<maxx-1; d.x++)
						{
							if(dot(d)&dmSolid)
							{
								dot(d)&=0xfe;//��Ϊ��ʵ��
							}
						}
				}
				else
				{
					CPoint2d circle=point2(j*p_interval+p_interval/2, i*p_interval+p_interval/2);
					double delt=m_resolution*cCoef;
					int numring=(int)(p_diameter/(m_resolution*2*cCoef));
					double radious;
					double deltarc=2*PI/360;
					for(int n=1; n<numring; n++)
					{
						radious=p_diameter/2-n*delt;
						for(double arc=0; arc<2*PI; arc+=deltarc)
						{
							CPoint2d p=point2( radious*cos(arc), radious*sin(arc) );
							p=p+circle;
							d.x=toIntX(p.x);
							d.y=toIntX(p.y);
							if(dot(d)&dmSolid)
							{
								dot(d)&=0xfe;
							}
						}
					}
					
				}
			}
		}
	}
	
}


//������ģ�������������Ƭ��������-----------------------------------------------
//p_exporter������Ƭ������, 
//bConverse�Ƿ������ǻ���� 
//���ʵ�����صı߽���������
void CZ3DRepresentation::exportFacets(CVFacetExporter& p_exporter, bool bConverse)
{
	CPoint3i d;
	for(d.x=1; d.x<m_maxX-1; d.x++)
		for(d.y=1; d.y<m_maxY-1; d.y++)
			for(d.z=1; d.z<m_maxZ; d.z++)
				if (dot(d)&dmSolid)
					for(int i=0; i<6; i++)//��Ȼһ����ֻ����3���棬�������￼��6������ͳһ�������
					if ((dot(d + dirID2offset(i))&dmSolid) == 0)//�����+i�ǲ���Ӧ����+dirID2offset(i)������ȡ�ڽ�ĳ������¸����أ�
						{
							//dot(d+dirID2offset(i),�������ǽ����ؽǵ�����ƶ��������ڶ�һ����������ǵ���˵����������ƶ����������һ����ֱ���ƶ�������棻
							//�������ǵ��ƶ�����ʹ�����һ���棻

							//���ڽӵ�i������ڽ�����Ϊ���������i����������Σ�i������λ������cCubeFacetDirs[i]�����ϵ���

							exportSubCubeFacet(p_exporter, d, i, bConverse);
						}

}


//����ģ��ת����STL�ļ�----------------------------------------------------------
//p_fn������ļ�.bConverse�Ƿ������ǻ�
void CZ3DRepresentation::exportBinarySTL(const char* p_fn, bool bConverse)
{
	CBinarySTLExporter ex(p_fn);
	exportFacets(ex, bConverse);
}


//���߽�����������������Ƭ-----------------------------------------------------
//p_exporter�����ν�����, 
//p_subCubeCord�������ص�����, 
//p_dirID���ص���Ĵ���,
//bConverse �Ƿ������ǻ�  
//���ĳ�����ϵ�������  
void CZ3DRepresentation::exportSubCubeFacet(CVFacetExporter& p_exporter, const CPoint3i& p_subCubeCord, int p_dirID, bool cConverse)
{
	
	//һ�����ص�8���ǵ�ֲ���������ʱ��ֲ�0-3���Գ��ڵ����λ�÷ֲ�4-7
	const CPoint3i shifts[8]=//8���ǵ�
	{
		{0, 0, 0},
		{1, 0, 0},
		{1, 1, 0},
		{0, 1, 0},
		{0, 0, 1},
		{1, 0, 1},
		{1, 1, 1},
		{0, 1, 1}
	};

	//һ�����ص�6���棬������ɽǵ���ȷ��,
	//�����������ά�����У�ÿ�д�����棬�������ʼ��cCubeFacetDirs��ά������ÿ����������ָ�����һ��
	const int dotShiftID[6][4] =
	{
		{1, 2, 6, 5},//p_dirID=0����ʾ�����ؽǵ�x+1������һ�������أ���Ȼ�����ҵ���Ϊ�߽���
		{4, 7, 3, 0},//p_dirID=1����ʾ�����ؽǵ�x-1������һ�������أ���Ȼ���������Ϊ�߽���
		{2, 3, 7, 6},//p_dirID=2����ʾ�����ؽǵ�y+1������һ�������أ���Ȼ��������Ϊ�߽���
		{1, 5, 4, 0},//p_dirID=3����ʾ�����ؽǵ�y-1������һ�������أ���Ȼ����ǰ����Ϊ�߽���
		{4, 5, 6, 7},//p_dirID=4����ʾ�����ؽǵ�z+1������һ�������أ���Ȼ�����ϵ���Ϊ�߽���
		{0, 3, 2, 1}//p_dirID=5����ʾ�����ؽǵ�z-1������һ�������أ���Ȼ�����µ���Ϊ�߽���
	};
	//��ά����Ԫ�ش������ţ���ά������кŴ�������
	//һ�����ϵĵ㱣�����������˳���ǣ������϶��㿪ʼ˳����
	
#define pout(i) (toFloat(p_subCubeCord+shifts[dotShiftID[p_dirID][(i)]]))
	if(!cConverse)
	{	p_exporter.exporter(pout(0), pout(1), pout(2));
		p_exporter.exporter(pout(2), pout(3), pout(0));
	}
	else
	{
		p_exporter.exporter(pout(1), pout(0), pout(2));
		p_exporter.exporter(pout(2), pout(0), pout(3));
	}
#undef pout	

}


const double COMPARE_GATE_GREAT=1e-8;


//�γ�һ����ɢ��������������
//p_triΪ�����ε���������, p_orMask, p_andMask    ��������
void CZ3DRepresentation::makeTriFacet(CTriangled& p_tri, Byte p_orMask, Byte p_andMask)
{
	CMatrix4d from3d2d;
	CMatrix4d from2d3d;
	CPoint3d nor;
	/*
	normal(p_tri.p[0],p_tri.p[1], p_tri.p[2], nor);

	if(!(nor.x==0.0&&nor.y==0.0&&(nor.z==1.0)))
		return;
	*/
	calcolateMatrix(p_tri.p[0], p_tri.p[1], p_tri.p[2], from3d2d, from2d3d, nor);
	CZArray<CPoint2d, false> ring2d;
	CZArray<CPoint3d, false> ring3d;
	ring3d.add(p_tri.p[0]);
	ring3d.add(p_tri.p[1]);
	ring3d.add(p_tri.p[2]);
	for3D2D(ring3d, ring2d, from3d2d);//3ά��ת��Ϊ2ά��
    CPoint2d p1=ring2d[0];
	CPoint2d p2=ring2d[1];
	CPoint2d p3=ring2d[2];

	double radious;
    CPoint2d centpoint;
	circleCenter(p1, p2, p3, centpoint, radious);//���������ε�����Բ��Բ�ĺͰ뾶
    {
		double  angle1[3];
		angle1[0] = atan2(p3.y-p1.y, p3.x-p1.x)+ PI/2;
		angle1[1] = atan2(p1.y-p2.y, p1.x-p2.y)+ PI/2;
		angle1[2] = atan2(p2.y-p3.y, p2.x-p3.y)+ PI/2;
		double delt=m_resolution*cCoef;
		double hight;
		double offset=0.0;
		//���������Բ������
		for(offset=m_compensate; offset>0.0; offset-=delt)//���������ƫ���߿�ʼ����offset������Բ���뾶
		{
			ring2d.setSize(0);
			hight=sqrt(sqr(m_compensate)-sqr(offset) );//������������ƫ�����ϵĴ�ֱ�߶γ���
			int numberPoint=int(fabs(angle1[1]-angle1[0])*offset/delt);//����Ϊ����仡��Ҫ���ٸ���
			double  minAngle=(angle1[1]-angle1[0])/numberPoint;
			double  curAngle;
			CPoint2d addpoint;
			for(int i=0; i<numberPoint; i++)
			{
				CPoint2d &p = ring2d.add();
				curAngle = angle1[0]+i*minAngle;
				addpoint=point2(offset*cos(curAngle), offset*sin(curAngle));
				p=p1+addpoint;
			}
			numberPoint=int(fabs(angle1[2]-angle1[1])*offset*2/delt);
			minAngle=(angle1[2]-angle1[1])/numberPoint;
			for(int i=0; i<numberPoint; i++)
			{
				CPoint2d &p=ring2d.add();
				curAngle = angle1[1]+i*minAngle;
				addpoint = point2(offset*cos(curAngle), offset*sin(curAngle) );
				p=p2+addpoint;
			}
			numberPoint =(int) (fabs(angle1[0]-angle1[2])*offset*2/delt);
			minAngle = (angle1[0]-angle1[2])/numberPoint;
			for(int i=0; i<numberPoint; i++)
			{
				CPoint2d &p=ring2d.add();
				curAngle = angle1[2]+i*minAngle;
				addpoint = point2(offset*cos(curAngle), offset*sin(curAngle) );
				p = p3+addpoint;
			}
			for2D3D(ring2d, ring3d, from2d3d);
			makeRing(ring3d, nor, hight, p_orMask, p_orMask);
			//����һ����
		}

		// �������ڵĴ��� [11/21/2002]
		{
			for(double dist=radious; dist>0.0; dist-=delt)
			{   
				ring2d.setSize(0);
				double ratios=dist/radious;
				ring2d.add(centpoint+ratios*(p1-centpoint));
				ring2d.add(centpoint+ratios*(p2-centpoint));
				ring2d.add(centpoint+ratios*(p3-centpoint));
				for2D3D(ring2d, ring3d, from2d3d);
				makeRing(ring3d, nor, m_compensate, p_orMask, p_orMask);
				// ���� [11/21/2002]
			}
			ring2d.setSize(0);
			ring2d.add(centpoint);
			// ����һ�� [11/21/2002]
			for2D3D(ring2d, ring3d, from2d3d);
			makeRing(ring3d, nor, m_compensate, p_orMask, p_orMask);
		}
	}

	{
// 		#ifdef _DEBUG
// 			CZArray<CPoint2d, false> debug2d(3, 0);
// 			CZArray<CPoint3d, false> debug3d(3, 0);
// 			debug3d.add(p_tri.p[0]);
// 			debug3d.add(p_tri.p[1]);
// 			debug3d.add(p_tri.p[2]);
// 			for3D2D(debug3d, debug2d, from3d2d);//3ά��ת��Ϊ2ά��
// 			debug3d.setSize(0);
// 			for2D3D(debug2d, debug3d, from2d3d);
// 			for(int i=0; i<3; i++)
// 			{
// 				m_errorX.addNumber(p_tri.p[i].x, debug3d[i].x);
// 				m_errorY.addNumber(p_tri.p[i].y, debug3d[i].y);
// 				m_errorZ.addNumber(p_tri.p[i].z, debug3d[i].z);
// 			}
// 		#endif
	}
}


//�γ�һ����ɢ���Ļ�-----------------------------------------------------------
//p_ring3d��ά��,  
//p_normal�ռ�ֱ�ߵķ�ʸ����, 
//p_hight �ռ�ֱ�ߵĸ߶�, 
//p_orMask, p_andMask    ��������
void CZ3DRepresentation::makeRing(const CZArray<CPoint3d, false> &p_ring3d, const CPoint3d &p_normal, const double &p_hight, Byte p_orMask, Byte p_andMask)
{
	CPoint3d p1, p2;
	double delt=m_resolution*cCoef;
	if(p_hight<=delt)//�������ɨ��Ŀռ��߶γ���̫С��С��һ��ɨ�貽������ζ���������ֻ��һ���߻�
	{
		for(int i=0; i<p_ring3d.size(); i++)
		{
			p1=p_ring3d[i];
			if(i==p_ring3d.size()-1)
				p2=p_ring3d[0];
			else p2=p_ring3d[i+1];
			make3DLine(p1, p2, p_orMask, p_andMask);
		}

	}
	else
	{
		CPoint3d d1, d2, midpoint;
		double distence;
		for(int i=0; i<p_ring3d.size(); i++)
		{
			d1=p_ring3d[i];
			if(i==p_ring3d.size()-1)
				d2=p_ring3d[0];
			else d2=p_ring3d[i+1];
			distence=dist(d1, d2);
			if(distence<=delt)//����������������С�޷�����һ��ɨ�裬��ô�Ͷ���������д�ֱƽ�淽���ϵ�ɨ��
			{

				CPoint3d p=p_hight*p_normal;
				p1=d1+p;
				p2=d1-p;
				make3DLine(p1, p2, p_orMask, p_andMask);
				p1=d2+p;
				p2=d2-p;
				make3DLine(p1, p2, p_orMask, p_andMask);
				continue;
			}
			int allpoint=(int)(distence/delt);
			if(allpoint==0) allpoint++;
			double addlet=(float)(distence/allpoint);
			for(int j=0; j<=allpoint; j++)//������֮����д�ֱ�����ϵ���ɢ����ɨ��
			{
				midpoint=d1+(j*addlet/distence)*(d2-d1);
				p1=midpoint+p_hight*p_normal;
				p2=midpoint+(-1)*p_hight*p_normal;
				make3DLine(p1, p2, p_orMask, p_andMask);
			}
			
		}

	}
}


//�γ�һ����ɢ����ֱ��-----------------------------------------------------------
//p_orMask, p_andMask��������
//Ҫɨ������֮������أ��ÿգ�����
inline void CZ3DRepresentation::make3DLine(const CPoint3d& p_p1, const CPoint3d& p_p2, Byte p_orMask, Byte p_andMask )//0,0xfe
{
	double len = dist(p_p1, p_p2);
	int  dotNum = (int)(len/(m_resolution*cCoef));
	double delt=m_resolution*cCoef;
	CPoint3d  pf, df;
	df = p_p2 - p_p1;
	CPoint3i pi;
	if(len<delt)//�޷�����������ɨ�裬��ô��ֱ�Ӽ��������������
	{
		pi = toInt(p_p1);
		dot(pi) = dot(pi)&p_andMask|p_orMask;
		pi = toInt(p_p2);
		dot(pi) = dot(pi)&p_andMask|p_orMask;
		return;
	}
	for(int i = 0; i<=dotNum; i++)
	{
		pf = (i*delt/len)*df+p_p1;
		pi = toInt(pf);
		dot(pi) = dot(pi)&p_andMask|p_orMask;
	}
}


void CZ3DRepresentation::removeVerySmallPart()
{
	CPoint3i d;
	for(d.z=1; d.z<m_maxZ; d.z++)
		for(d.y=1; d.y<m_maxY-1; d.y++)
			for(d.x=1; d.x<m_maxX-1; d.x++)
				if (dot(d)&dmSolid)
				{
					int dir=0;
					for(int i=0; i<4; i++)
					{
						if ( (dot(d+dirID2offset(i))==dmSolid) )
						{
							dir++;
						}
					}
					if(dir<2)
						dot(d) &= 0x00;			
				}
}


