#include "stdafx.h"
#include "3DRepresentation.h"
const double cCoef = 0.3;

//X方向上的扫描线
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

//点三维到二维的转化 
inline void forOne3D2D(const CPoint3d &p_3d, CPoint2d &p_out2d, CMatrix4d &p_m3d2d)
{
	CPoint3d p = p_m3d2d*p_3d;
	p_out2d.x = p.x;
	p_out2d.y = p.y;
}

//点二维到三维的转化
inline void forOne2D3D(const CPoint2d &p_2d, CPoint3d &p_out3d, CMatrix4d &p_m2d3d)
{
	CPoint3d p3d;
	p3d.x = p_2d.x;
	p3d.y = p_2d.y;
	p3d.z = 0.0;
	p_out3d = p_m2d3d*p3d;
}

//环的三维坐标到二维的转化
void for3D2D(const CZArray<CPoint3d, false> &p_3d, CZArray<CPoint2d, false> &p_out2f, CMatrix4d &p_m3d2d)
{
	p_out2f.setSize(0);
	for (int i = 0; i < p_3d.size(); i++)
	{
		CPoint2d &p = p_out2f.add();
		forOne3D2D(p_3d[i], p, p_m3d2d);
	}
}

//二维坐标到三维的转化---------------------------------------------------------
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

//计算直线到直线的旋转矩阵  ---------------------------------------------------
void calcolateMatrix(const CPoint3d &p_p1, const CPoint3d &p_p2, const CPoint3d &p_p3, CMatrix4d &p_m3d2d, CMatrix4d &p_m2d3d, CPoint3d &p_normal)
{
	CPoint3d userZero;
	CPoint3d  zAxi = zeroPoint3d;
	zAxi.z = 1.0;
	double m;
	definePlane(p_p1, p_p2, p_p3, userZero, m);//userZero为垂足, m=0.0f则平面过原点。求出的userZero是原点向三角形平面做垂线的垂足。
	{
		CPoint3d temp = userZero;
		normalize(temp);
		if (temp.x == 0.0&&temp.y == 0.0 && (temp.z == 1.0))
		{
			userZero.z = p_p1.z;//归一化后如果发现这个垂足就在Z轴上，说明该三角形已经平行于xoy面，直接取其中一点作为原点即可
		}
	}
	rotateFromTo(userZero, zAxi, p_m3d2d);//计算出从两个点代表的向量相互旋转的矩阵
	rotateFromTo(zAxi, userZero, p_m2d3d);
	p_normal = userZero;
	normalize(p_normal);
	CPoint3d  off = p_m3d2d*userZero;//计算旋转后的移动距离
	if (m == 0.0)
	{
		off = m*off;
		userZero = m*userZero;
	}
	p_m3d2d[3][0] = -off.x;
	p_m3d2d[3][1] = -off.y;
	p_m3d2d[3][2] = -off.z;   //移动

	p_m2d3d[3][0] = userZero.x;
	p_m2d3d[3][1] = userZero.y;
	p_m2d3d[3][2] = userZero.z;

}

//计算线段的交点---------------------------------------------------------------
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

//计算三角形的中心
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
//直线按终点的排序,首先按照y由,在y相同,再按照x.都是由小到大.
{
	const CPoint2i &p1 = allLinesData[*(int*)i1].start;
	const CPoint2i &p2 = allLinesData[*(int*)i2].start;

	int diff = (p1.y != p2.y) ? (p1.y - p2.y) : (p1.x - p2.x);
	return diff > 0 ? 1 : (diff == 0 ? 0 : -1);
}

//折半查找出下一条边界线
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


//接下来才是类CZ3DRepresentation的部分实现
CZ3DRepresentation::CZ3DRepresentation():m_buff(NULL){}
CZ3DRepresentation::CZ3DRepresentation(CCubef p_orginalSolidCube, float p_compensate, float p_resolution):m_buff(NULL)
{
	init(p_orginalSolidCube, p_compensate, p_resolution);
}

//CZ3DRepresentation::~CZ3DRepresentation()
//{
//	free();
//}

//整个函数的作用为：算出buff数组的大小，然后创建一个元素全为0的空数组buff。
void CZ3DRepresentation::init(CCubef p_orginalSolidCube,  float p_compensate, float p_resolution)                
{
	assert(!bInited());

	CPoint3f pf=p_orginalSolidCube.min;
	m_orgCube.min = point3(double(pf.x), double(pf.y), double(pf.z));                                        
	pf=p_orginalSolidCube.max;
	m_orgCube.max = point3(double(pf.x), double(pf.y), double(pf.z));

	//这里不断进行float类型和double类型的转换，用double类型的目的是提高精度
	m_compensate = double(p_compensate);
	m_resolution = double(p_resolution);
	double  temp = m_compensate+m_resolution;
	CPoint3d  p3d = point3(temp, temp, temp);
	
	//STL实体模型的包围体在这里已经偏置扩大了一次了。
	//这里对STL模型做距离和精度补偿，为什么补偿这两项？补偿后变成什么样了？
	m_cube.min = m_orgCube.min-p3d;
	m_cube.max = m_orgCube.max+p3d;

	m_maxX = (int)(m_cube.w()/m_resolution)+2;                                                       
	m_maxY = (int)(m_cube.h()/m_resolution)+2;                                                       
	m_maxZ = (int)(m_cube.t()/m_resolution)+2;
	//+2的原因是：为了在之后判断某一体素上下左右的其他体素状态时不会发生某一个方向上没有体素了（该体素处于边界）情况，          
	//因此将实体所在的体，在每个维度上前后各增加一个体素位置，将体移到正中间，因此总共要+2
	
	unsigned long long  totalsize = ((unsigned long long)m_maxX + 1)*
		((unsigned long long)m_maxY + 1)*((unsigned long long)m_maxZ + 1);
	//索引总数+1是为了保证能取到0-m_max             
	//经过多次扩大后，在体素模型中，索引值代表了体素的一个角点；
	//在m_cube代表的实体所在体素群范围外，每个维度的首尾各自增加一个体素，用于在之后的判断边界中防止数组越界。
	//因此，索引1才是m_cube的第一个体素。索引m_max-1是最后一个体素。

	m_buff = new Byte[totalsize];
	
	//memset()函数将m_buff的totalsize个字节置零：void *memset(void *s, int ch, size_t n);
	//函数解释：将s中当前位置后面的n个字节 （typedef unsigned int size_t ）用 ch 替换并返回 s 
	memset(m_buff, 0, totalsize);   

	m_buff_CurvedLayer = new Byte[totalsize];
	memset(m_buff_CurvedLayer, 0, totalsize);

	m_buff_CurvedLayer2 = new Byte[totalsize];
	memset(m_buff_CurvedLayer2, 0, totalsize);
	
	m_yCoefficient = m_maxX+1;                                                                                            
	m_zCoefficient = (m_maxY+1)*m_yCoefficient;                                                                            
}

//判断边界或者实体最上层------------------------------------------------------------
//判断是否为整个实体的边界
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
//判断是否为某一z值所在的平面层的边界
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
//判断是否为最上层
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



//找到指定层（给定z值）的有向边界线并将其加入到数组p_lines中
//void CZ3DRepresentation::findBoundaryLine(double p_z, CZArray<CIntLine, false> &p_lines, Byte p_maskneed)//=dmBoundary                     //CZArray用来定义一个数组类型模板，该函数可以重点学习一下
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
//					//写错了？不符合逆时针逻辑
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

//对p_z层高度进行切片并输出到p_slice中:先找到所有的有向边界线，将其保存到临时数组中；
//然后按有向线的起点排序，并用折半查找出首尾相连的线组成环，并对环的每一个顶点用圆弧
//光顺。
//将三角形面片保存到文件中
//void CZ3DRepresentation::exportBinarySTL(const char *p_fn, float p_smoothness, vtkPolyData *p_solid, const int &p_hellowtype, const int &p_suppertype, const float &p_diameter, const float &p_interval)
//{
//	assert(bInited());
//	switch(p_hellowtype)
//	{
//		case 0:   //离散STL模型
//			exportBinarySTL(p_fn, false);
//			removeVerySmallPart();
//			break;
//		case 1:   //偏置——对每个三角形进行离散偏置，置空附近体素
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
//		case 2:    //镂空
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
//					ex.exporter(testf.p[0], testf.p[1], testf.p[2]);//输出原始STL面片
//					if(i>=iPuted)
//					{
//						//getMsgOut().puts("已处理%3.1f万个面片，%2.0f%%完成.\n", iPuted/10000.0, double(iPuted)/p_solid.getFaceCount()*100);
//						iPuted+=step;
//					}
//				}
//				//getMsgOut().puts("面片处理完毕\n" );
//				removeVerySmallPart();
//				removeVerySmallPart();
//				//getMsgOut().puts("输出文件..." );
//				//现在体素模型只剩下内腔部分，反向输出其边界三角
//				exportFacets(ex, true);//输出
//				break;	
//			}
//
//		case 3:    //镂空并支撑
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
//						//getMsgOut().puts("已处理%3.1f万个面片，%2.0f%%完成.\n", iPuted/10000.0, double(iPuted)/p_solid.getFaceCount()*100);
//						iPuted+=step;
//					}
//				}
//				//getMsgOut().puts("面片处理完毕\n" );
//				//getMsgOut().puts("生成支撑..." );
//				makeSupportMatrix((!p_suppertype)?stSquare:stCircle, double(p_diameter), double(p_interval), point2(0.0, 0.0) );	
//				//getMsgOut().puts("完成\n" );
//				removeVerySmallPart();
//				//getMsgOut().puts("输出文件..." );
//				exportFacets(ex, true);
//				break;
//			}
//	}
//}

//用VTK绘图（以前是用OpenGL绘制的）
void CZ3DRepresentation::glDraw(vtkSmartPointer<vtkPoints> ugpoints, vtkSmartPointer<vtkUnstructuredGrid> ug, vtkSmartPointer<vtkCellArray> cells)   //用VTK显示实体体素的
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
						ugpoints->InsertNextPoint((x - 1)*m_resolution + m_cube.min.x, (y - 1)*m_resolution + m_cube.min.y, (fz));//换算成这个点的实际坐标值
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
						ugpoints->InsertNextPoint((x - 1)*m_resolution +px, (y - 1)*m_resolution+py, (fz)+pz);//换算成这个点的实际坐标值
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
//void CZ3DRepresentation::glDrawMain(vtkSmartPointer<vtkPoints> ugpoints, vtkSmartPointer<vtkUnstructuredGrid> ug, vtkSmartPointer<vtkCellArray> cells, float h)        //跟机加工有关，先不管
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
//void CZ3DRepresentation::glDrawCurrent(vtkSmartPointer<vtkPoints> ugpoints, vtkSmartPointer<vtkUnstructuredGrid> ug, vtkSmartPointer<vtkCellArray> cells, float h)         //跟机加工有关，先不管
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
//void CZ3DRepresentation::glDrawNext(vtkSmartPointer<vtkPoints> ugpoints, vtkSmartPointer<vtkUnstructuredGrid> ug, vtkSmartPointer<vtkCellArray> cells, float h)    //跟机加工有关，先不管
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

//选用默认的扫描线
ILineScan& getDefaultILineScan(void);


//形成离散化的实体表示-----------------------------------------------------------
//p_solid 原实体
//这里真正的把实体体素化，之后把buff中对应的实体部分相应元素赋为1.
//说明：后续处理中（包括标识边界和生成层）只会用到  1到m_maxX-1; 1到m_maxY-1; 1到m_maxZ-1;(开头和结尾的元素不需要用)
void CZ3DRepresentation::makeSolid(vtkPolyData *p_solid, Byte p_orMask, Byte p_andMask)//1,0
{
	assert(bInited());
	assert(p_solid->GetNumberOfCells()!=0);
	vtkSmartPointer<vtkPolyData> slice = vtkSmartPointer<vtkPolyData>::New();
	vtkSmartPointer<vtkCutter> p_cutter = vtkSmartPointer<vtkCutter>::New();
	vtkSmartPointer<vtkPlane> plane = vtkSmartPointer<vtkPlane>::New();             
	plane->SetOrigin(0, 0, 0);//设定一个垂直于z轴的平面
	plane->SetNormal(0, 0, 1);

	//double minBound[3];//GetBounds()获取包围盒的x/y/z的最大值，最小值
	//minBound[0] = p_solid->GetBounds()[0];
	//minBound[1] = p_solid->GetBounds()[2];                   
	//minBound[2] = p_solid->GetBounds()[4];
	//double maxBound[3];
	//maxBound[0] = p_solid->GetBounds()[1];
	//maxBound[1] = p_solid->GetBounds()[3];
	//maxBound[2] = p_solid->GetBounds()[5];

	for (double  h = m_cube.min.z+0.1f*m_resolution; h <m_cube.max.z; h+=m_resolution)
	{
		//分析h的值：cube的最小z值到最大z值，切片递增的是体素的边长；
		//cube是这个类的全局变量，它的值在init函数中初始化了，给予的值是移动过的一个float类型的实体（将最小点移动到了原点），移动之后又进行偏置补偿，因此它的最小值是负的
		
		//解释0.1f，只写0.1默认是double类型的数据，写0.1f是表示float类型的数据
		//补充解释：float类型是单精度，内存中占4个字节，精度是7位（有一位是正负）
		//double类型是双精度，精度高，但是占用内存大，运算速度慢（能用float就不用double）
		{
			//设置切片的属性
			p_cutter->SetCutFunction(plane);//p_cutter是一个切片，这里用平面去切一个实体                 
			p_cutter->SetInputData(p_solid);
			p_cutter->GenerateValues(1, h, h);
			p_cutter->Update();
			slice = p_cutter->GetOutput();
		}
		scan(h, slice, p_orMask, p_andMask);//1,0
	}
}
//扫描线函数可以为我提供借鉴
void CZ3DRepresentation::scan(double p_z, vtkSmartPointer<vtkPolyData>p_slice, Byte p_orMask, Byte p_andMask)
{
	assert(bInited());
	int z = toIntZ(p_z);
	//assert(in(z, 0, m_maxZ));

	scanOptionX.rect.min = point2(float(p_slice->GetBounds()[0]), float(p_slice->GetBounds()[2]));       //设定扫描区域
	scanOptionX.rect.max = point2(float(p_slice->GetBounds()[1]), float((p_slice->GetBounds()[3])));
	scanOptionX.intver = float(m_resolution/2);                     //设定扫描步距
	scanOptionX.nIgnoredFrame = /*p_slice.getFrameCount()*/0;   //忽略外框和内框
	CZGrid scanGridX;

	scanGridX.make(&getDefaultILineScan(), scanOptionX, p_slice);

	const CZScanLines &lines = scanGridX.lines[scanX];
	for (int i = 0; i < lines.size(); ++i)
	{
		const CZScanLine &line = lines[i]; //第i条X扫描线
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


//标示边界-----------------------------------------------------------------------
//把属于边界的buff数组元素的值加2（把一个字节的从右边数第二位的值变为1）。
//代码解释：0xfe为二进制的1111 1110；&是位与，同为1才为1；|是位或，有一个为1就是1.(ox开头表示十六进制)
//dotxyz&0xfe|dmBoundary解释：dotxyz原本为1（即0000 0001），与oxfe位与后变为0（0000 0000），再与dmBoundary（2，即0000 0010）位或，变成2.
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
						dotxyz = dotxyz&0xfe|dmBoundary;//将该值变为2，即0000 0010.
						to++;
					}
				}
			}

		}
	}
}


//生成面层-----------------------------------------------------------------------
//先找出最上层曲面或者平面，对其路径规划；
//然后将整个实体减去已经规划的部分；
//重复生成最上层曲面或者平面，重复得到剩余部分，直至完成整个实体的路径规划
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
	//把m_buff_CurvedLayer2重置为0
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
	//把m_buff_CurvedLayer2重置为0
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
	int countLayer = ceil(m_maxZ / (d / m_resolution))+1;//向上取整，计算出所需要的层数
	for (int k = 0; k < countLayer; k++)
	{
		Byte* buffNew;
		unsigned long long  totalsize = ((unsigned long long)m_maxX + 1)*
			((unsigned long long)m_maxY + 1)*((unsigned long long)m_maxZ + 1);
		buffNew = new Byte[totalsize];
		memset(buffNew, 0, totalsize);
		m_buff_LineCurvedLayer.push_back(buffNew);
	}

	for (int x = 0; x < m_maxX; x+=1)//生长线密度，体素精度的4倍.先让生长线足够密集
	{
		for (int y = 0; y < m_maxY; y+=1)
		{
			//获取每一个XY值所对应的生长线的交点
			//建立该XY值对应的生长线段vector数组
			vector<int > startPointZ;//起始点
			vector<int >endPointZ;//终止点

			//从下到上计数
			//for (int z = 1; z < m_maxZ-1; z++)
			//{
			//	//判断该点及前后两个点是否在实体内部
			//	if (m_buff[(z-1)*m_zCoefficient + y*m_yCoefficient + x] == 0 && m_buff[z*m_zCoefficient + y*m_yCoefficient + x] == 1 && m_buff[(z + 1)*m_zCoefficient + y*m_yCoefficient + x] == 1)//前一个点不在实体内，后一个点在实体内
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
			//	int numLayer = 0;//计数，计算当前点所在的层数
			//	for (int p = 0; p < startPointZ.size(); p ++)
			//	{
			//		int distanceAB = endPointZ.at(p) - startPointZ.at(p);//计算交线段的长度
			//		int D = d / m_resolution;
			//		for (int q = startPointZ.at(p); q < endPointZ.at(p); q+=D )
			//		{
			//			m_buff_LineCurvedLayer.at(numLayer)[q*m_zCoefficient + y*m_yCoefficient + x] = 1;
			//			numLayer++;
			//		}
			//		m_buff_LineCurvedLayer.at(numLayer)[endPointZ.at(p)*m_zCoefficient + y*m_yCoefficient + x] = 1;//把最后一个点加上
			//		numLayer++;
			//	}		
			//}

			//从上到下计数
			for (int z = m_maxZ-2; z > 0; z--)
			{
				//判断该点及前后两个点是否在实体内部
				if (m_buff[(z - 1)*m_zCoefficient + y*m_yCoefficient + x] == 0 && m_buff[z*m_zCoefficient + y*m_yCoefficient + x] == 1 && m_buff[(z + 1)*m_zCoefficient + y*m_yCoefficient + x] == 1)//前一个点不在实体内，后一个点在实体内
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
				int numLayer = 0;//计数，计算当前点所在的层数
				for (int p = 0; p < startPointZ.size(); p++)
				{
					int distanceAB = startPointZ.at(p) - endPointZ.at(p);//计算交线段的长度
					int D = d / m_resolution;
					for (int q = startPointZ.at(p); q > endPointZ.at(p); q -= D)
					{
						m_buff_LineCurvedLayer.at(numLayer)[q*m_zCoefficient + y*m_yCoefficient + x] = 1;
						numLayer++;
					}
					m_buff_LineCurvedLayer.at(numLayer)[endPointZ.at(p)*m_zCoefficient + y*m_yCoefficient + x] = 1;//把最后一个点加上
					numLayer++;
				}
			}


			startPointZ.clear();
			endPointZ.clear();
		}//结束当前生长线的遍历
	}
}

void CZ3DRepresentation::makeGrowthLineVirousCurveLayer(double dmin, double dmax, Buff_LineCurvedLayer &m_buff_LineCurvedLayer)
{
	//求交线段最大值
	int maxdistance = 0;
	for (int x = 0; x < m_maxX; x += 1)//生长线密度为1个体素精度，先让生长线足够密集。实际可以用4倍的体素精度试试。
	{
		for (int y = 0; y < m_maxY; y += 1)
		{
			vector<int > startPointZ;//起始点
			vector<int >endPointZ;//终止点
			for (int z = 1; z < m_maxZ - 1; z++)
			{
				if (m_buff[(z - 1)*m_zCoefficient + y*m_yCoefficient + x] == 0 && m_buff[z*m_zCoefficient + y*m_yCoefficient + x] == 1 && m_buff[(z + 1)*m_zCoefficient + y*m_yCoefficient + x] == 1)//前一个点不在实体内，后一个点在实体内
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
					int distanceAB = endPointZ.at(p) - startPointZ.at(p);//计算交线段的长度

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

	//把数据分成若干区间
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


	int countLayer = ceil((double)range.at(0) / dmax_voxel);//向上取整，计算出所需要的层数
	for (int k = 0; k < countLayer; k++)
	{
		Byte* buffNew;
		unsigned long long  totalsize = ((unsigned long long)m_maxX + 1)*
			((unsigned long long)m_maxY + 1)*((unsigned long long)m_maxZ + 1);
		buffNew = new Byte[totalsize];
		memset(buffNew, 0, totalsize);
		m_buff_LineCurvedLayer.push_back(buffNew);
	}

	for (int x = 0; x < m_maxX; x += 1)//生长线密度，体素精度的4倍.先让生长线足够密集
	{
		for (int y = 0; y < m_maxY; y += 1)
		{
			//获取每一个XY值所对应的生长线的交点
			//建立该XY值对应的生长线段vector数组
			vector<int > startPointZ;//起始点
			vector<int >endPointZ;//终止点

			 //从上到下计数
			for (int z = m_maxZ - 2; z > 0; z--)
			{
				//判断该点及前后两个点是否在实体内部
				if (m_buff[(z - 1)*m_zCoefficient + y*m_yCoefficient + x] == 0 && m_buff[z*m_zCoefficient + y*m_yCoefficient + x] == 1 && m_buff[(z + 1)*m_zCoefficient + y*m_yCoefficient + x] == 1)//前一个点不在实体内，后一个点在实体内
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
				int numLayer = 0;//计数，计算当前点所在的层数
				for (int p = 0; p < startPointZ.size(); p++)
				{
					int distanceAB = startPointZ.at(p) - endPointZ.at(p);//计算交线段的长度
					
					//找出该交线段的值所在的区间
					int layerCout;//该交线段的层厚
					for (int i = 0; i < range.size()-1; i++)
					{
						if (distanceAB <= range.at(i) && distanceAB > range.at(i+1))
						{
							layerCout = ceil((double)range.at(i) / dmax_voxel);
							break;//停止当前for循环
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
		}//结束当前生长线的遍历
	}

}

//生成最上层平面
void CZ3DRepresentation::makeTopFlatLayer(void)
{

}
//生成空体，将已经转化为最上层的体素变为空.每次生成一次最上层体素，规划完路径就把它变成空体
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

//接下来做刀具曲面---------------------------------------------------------------
//先做个最简单的底层刀具曲面
//这个刀具曲面存储的数组是一个int类型的二维普通数组，因为这个数组大小是已知的
//void makeToolSurface(CurveArray p_curveArray)
//{
//	CurveArray curveArray;
//	unsigned long long  total = ((unsigned long long)m_maxX + 1) *((unsigned long long)m_maxY + 1);//保证能取到0-max
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
//						//注意当前数组的下标是否正确
//						curveArray[(x-1)(m_maxY+1) + y] = z;
//						break;
//					}
//				}
//			}
//		}
//	}
//
//	//这里先进行阈值判断,去掉超出阈值的最上层曲面部分
//	//直接将该值与之前的若干值进行判断，若干是多少？这里设定一个规定条件。
//	//判断之后把合适的值留下，将其z值赋到数组元素
//	int interval = 2/m_resolution;
//	//判断X正方向
//	double m_XAngle = 0.0;
//	for (int x = 1; x < m_maxX; x++)
//	{
//		int num = 0;
//		for (int y = 1; y < m_maxY; y++)
//		{
//			if (curveArray[(x-1)(m_maxY+1) + y]>0)
//			{
//				num++;
//				if (num ==interval+1)//这个判断是为了去掉Xmin之前的最上层曲面
//				{
//					m_XAngle = atan( double( (curveArray[(x-1)(m_maxY+1) + y] - curveArray[(x-1)(m_maxY+1) + y - interval])/interval ));
//					if (m_XAngle < -3.14159265/4 || m_XAngle > 3.14159265/4)
//					{
//						//此时令1-interval+1这几个元素的值都为0
//						for (int i = 0; i < interval+1; i++)
//						{
//							curveArray[(x-1)(m_maxY+1) + y - i] =0;
//						}
//					}
//				}
//	
//				else if (num >interval+1)//继续判断x方向的的部分
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
//	//接下来处理实体外未被赋值的元素（即值为0的元素）
//	//已经想好了
//	for ()
//	{
//
//	}
//}


//形成支撑-----------------------------------------------------------------------
// p_type 支撑的类型, 
//p_diameter 支撑的直径或宽度, 
//p_interval 相邻支撑间的中心间距
//p_offset 支撑从边缘向内偏移的实际距离
void CZ3DRepresentation::makeSupportMatrix(CSupportType p_type, double p_diameter, double p_interval, 
		CPoint2d p_offset) 
{
	int row =(int) (m_cube.h()/p_interval);      //y轴方向支撑数目
	int arrange = (int)(m_cube.w()/p_interval);  //x轴方向支撑数目
	CPoint3i d;
	for(d.z=1; d.z<m_maxZ; d.z++)
	{
		for(int i=0; i<row; i++)          //y轴
		{
			for(int j=0; j<arrange; j++)  //x轴
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
								dot(d)&=0xfe;//设为非实体
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


//将体素模型输出到三角面片接收器中-----------------------------------------------
//p_exporter三角面片接收器, 
//bConverse是否反向三角化输出 
//输出实体体素的边界三角形面
void CZ3DRepresentation::exportFacets(CVFacetExporter& p_exporter, bool bConverse)
{
	CPoint3i d;
	for(d.x=1; d.x<m_maxX-1; d.x++)
		for(d.y=1; d.y<m_maxY-1; d.y++)
			for(d.z=1; d.z<m_maxZ; d.z++)
				if (dot(d)&dmSolid)
					for(int i=0; i<6; i++)//虽然一个点只能临3个面，但是这里考察6个面来统一情况处理
					if ((dot(d + dirID2offset(i))&dmSolid) == 0)//这里的+i是不是应该是+dirID2offset(i)，来获取邻接某个面的下个体素？
						{
							//dot(d+dirID2offset(i),表面上是将体素角点进行移动，但由于对一个体素任意角点来说，其任意的移动方向均存在一个垂直于移动方向的面；
							//因此这个角点移动方向就代表了一个面；

							//若邻接第i个面的邻接体素为空则输出第i个面的三角形；i代表了位于体素cCubeFacetDirs[i]方向上的面

							exportSubCubeFacet(p_exporter, d, i, bConverse);
						}

}


//体素模型转化成STL文件----------------------------------------------------------
//p_fn输出的文件.bConverse是否反向三角化
void CZ3DRepresentation::exportBinarySTL(const char* p_fn, bool bConverse)
{
	CBinarySTLExporter ex(p_fn);
	exportFacets(ex, bConverse);
}


//将边界的体素输出成三角面片-----------------------------------------------------
//p_exporter三角形接收器, 
//p_subCubeCord输入体素的坐标, 
//p_dirID体素的面的代号,
//bConverse 是否反向三角化  
//输出某个面上的三角形  
void CZ3DRepresentation::exportSubCubeFacet(CVFacetExporter& p_exporter, const CPoint3i& p_subCubeCord, int p_dirID, bool cConverse)
{
	
	//一个体素的8个角点分布：顶面逆时针分布0-3，对称在底面的位置分布4-7
	const CPoint3i shifts[8]=//8个角点
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

	//一个体素的6个面，其序号由角点来确定,
	//并且在这个二维数组中，每行代表的面，必须与最开始的cCubeFacetDirs二维数组中每个向量方向指向的面一致
	const int dotShiftID[6][4] =
	{
		{1, 2, 6, 5},//p_dirID=0，表示在体素角点x+1方向是一个空体素，必然是最右的面为边界面
		{4, 7, 3, 0},//p_dirID=1，表示在体素角点x-1方向是一个空体素，必然是最左的面为边界面
		{2, 3, 7, 6},//p_dirID=2，表示在体素角点y+1方向是一个空体素，必然是最后的面为边界面
		{1, 5, 4, 0},//p_dirID=3，表示在体素角点y-1方向是一个空体素，必然是最前的面为边界面
		{4, 5, 6, 7},//p_dirID=4，表示在体素角点z+1方向是一个空体素，必然是最上的面为边界面
		{0, 3, 2, 1}//p_dirID=5，表示在体素角点z-1方向是一个空体素，必然是最下的面为边界面
	};
	//二维数组元素代表顶点编号，二维数组的行号代表面编号
	//一个面上的点保存入行数组的顺序是：从左上顶点开始顺保存
	
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


//形成一个离散化的三角柱区域
//p_tri为三角形的三个顶点, p_orMask, p_andMask    运算掩码
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
	for3D2D(ring3d, ring2d, from3d2d);//3维点转化为2维点
    CPoint2d p1=ring2d[0];
	CPoint2d p2=ring2d[1];
	CPoint2d p3=ring2d[2];

	double radious;
    CPoint2d centpoint;
	circleCenter(p1, p2, p3, centpoint, radious);//计算三角形的内切圆的圆心和半径
    {
		double  angle1[3];
		angle1[0] = atan2(p3.y-p1.y, p3.x-p1.x)+ PI/2;
		angle1[1] = atan2(p1.y-p2.y, p1.x-p2.y)+ PI/2;
		angle1[2] = atan2(p2.y-p3.y, p2.x-p3.y)+ PI/2;
		double delt=m_resolution*cCoef;
		double hight;
		double offset=0.0;
		//三角形外的圆弧处理
		for(offset=m_compensate; offset>0.0; offset-=delt)//从最外面的偏置线开始处理，offset代表了圆弧半径
		{
			ring2d.setSize(0);
			hight=sqrt(sqr(m_compensate)-sqr(offset) );//计算那条放在偏置线上的垂直线段长度
			int numberPoint=int(fabs(angle1[1]-angle1[0])*offset/delt);//计算为了填充弧度要多少个点
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
			//处理一个环
		}

		// 三角形内的处理 [11/21/2002]
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
				// 处理环 [11/21/2002]
			}
			ring2d.setSize(0);
			ring2d.add(centpoint);
			// 处理一点 [11/21/2002]
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
// 			for3D2D(debug3d, debug2d, from3d2d);//3维点转化为2维点
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


//形成一个离散化的环-----------------------------------------------------------
//p_ring3d三维环,  
//p_normal空间直线的法矢向量, 
//p_hight 空间直线的高度, 
//p_orMask, p_andMask    运算掩码
void CZ3DRepresentation::makeRing(const CZArray<CPoint3d, false> &p_ring3d, const CPoint3d &p_normal, const double &p_hight, Byte p_orMask, Byte p_andMask)
{
	CPoint3d p1, p2;
	double delt=m_resolution*cCoef;
	if(p_hight<=delt)//如果进行扫描的空间线段长度太小，小于一个扫描步长，意味着这个环就只是一条线环
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
			if(distence<=delt)//如果还上两个点间距过小无法进行一次扫描，那么就对这两点进行垂直平面方向上的扫描
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
			for(int j=0; j<=allpoint; j++)//在两点之间进行垂直方向上的离散体素扫描
			{
				midpoint=d1+(j*addlet/distence)*(d2-d1);
				p1=midpoint+p_hight*p_normal;
				p2=midpoint+(-1)*p_hight*p_normal;
				make3DLine(p1, p2, p_orMask, p_andMask);
			}
			
		}

	}
}


//形成一个离散化的直线-----------------------------------------------------------
//p_orMask, p_andMask运算掩码
//要扫描两点之间的体素，置空！！！
inline void CZ3DRepresentation::make3DLine(const CPoint3d& p_p1, const CPoint3d& p_p2, Byte p_orMask, Byte p_andMask )//0,0xfe
{
	double len = dist(p_p1, p_p2);
	int  dotNum = (int)(len/(m_resolution*cCoef));
	double delt=m_resolution*cCoef;
	CPoint3d  pf, df;
	df = p_p2 - p_p1;
	CPoint3i pi;
	if(len<delt)//无法在两点间进行扫描，那么就直接检测两点所在体素
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


