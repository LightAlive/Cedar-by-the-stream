//3DRepresentation.h 三维实体离散化的表示类

//说明：
//STL实体（座标），实物体（座标），实体（座标）都表示空间实际的连续的实体（座标）
//离散（座标），离散化的实体（座标）都表示经过离散化的非连续的实体（座标）
//
//主要用途：
//1：STL文件的娄空
//2：STL文件表面偏置
//3：形成加工过程中的支撑体
//4: 损失精度的布尔运算
//主要功能：
//1：由STL实体表示成离散的实体。
//2：形成三角形、圆的区域空间
//3：形成三角形、圆的带方向的半区域空间
//4: 形成圆、长方体的支撑
//5：输出处理后的切片轮廓
//6：输出处理后的STL文件
//7：处理后的显示
//
#ifndef __Z3D_REPRESENTATION_H
#define __Z3D_REPRESENTATION_H
#include "TEACDef.h"
#include"SolidDef.h"
#include"stdafx.h"

//用来构建数组,存放实体的切片层
typedef int CurveArray;//这个数组的元素是存放z值的，这个数组是个二维的
typedef vector<CurveArray> SolidArray;//这个是一维的数组，用来存放实体的切片层

typedef std::vector<Byte*> Buff_LineCurvedLayer;//用以存放各曲面层的buff

struct CTrianglef //用于存放三角面片
{
	CPoint3f p[3];
};

struct CTriangled
{
	CPoint3d p[3];
};

const CPoint3i cCubeFacetDirs[6] =//每个元素表示了体素在某个方向上的面
{
	{  1,  0,  0 },  //x+1
	{ -1,  0,  0 },  //x-1
	{  0,  1,  0 },  //y+1 
	{  0, -1,  0 },  //y-1
	{  0,  0,  1 },  //z+1
	{  0,  0, -1 }   //z-1
};
inline const CPoint3i &dirID2offset(int p_dirID)
{
	assert(in(p_dirID, 0, 5));
	return cCubeFacetDirs[p_dirID];
}

class CVFacetExporter                                                   //输出面片类
{
public:
	//interface
	virtual void exporter(CPoint3f p_p0, CPoint3f p_p1, CPoint3f p_p2)=0;
	virtual ~CVFacetExporter(void) {;};
	void exporter(const CTrianglef& p_tri) { exporter(p_tri.p[0], p_tri.p[1], p_tri.p[2]); };
};

class CBinarySTLExporter: public CVFacetExporter
{
public:
	CBinarySTLExporter(const char *fn);
	~CBinarySTLExporter(void);
	void exporter(CPoint3f p_p0, CPoint3f p_p1, CPoint3f p_p2);
protected:
	ofstream m_ostream;
	int m_triNum;
};

class CFacets; //to do
//class CFacets 三角面片的数组，主要为记录各三角面的数据

class vtkPolyData;
class vtkUnstructuredGrid;


class CZ3DRepresentation                                         
{
public:
	#pragma pack(push, 2)//不懂这个用法，想要了解可以查一下
	struct  CFace
	{
		CPoint3f  normal;
		CPoint3f p[3];
		__int16 attrib;
	};
	
	#pragma pack(pop)
	class CFacets : public CZArray<CFace, false>
	{

	};
	struct CIntLine
	{
		CPoint2i start;
		CPoint2i end;
	};

	//构造函数，该类的构造函数的四个参数解释：原立方体（要体素化的实体模型），是CCubef类型的，要去看一下这个类
	//内外偏置距离：这个偏置是用来判断边界的，所以生成的数组实际上比原来的实体大了一个精度的距离。在用buff的时候把元素0和元素MAX去掉。
	//再解释一下内外偏置距离：在判断边界处如点（x+1,y,z）不属于实体内，如果数组不进行扩大，那么就不存在（x+1，y,z)这个点。
	CZ3DRepresentation(void);
	CZ3DRepresentation(CCubef p_orginalSolidCube, float p_compensate, float p_resolution);
	//virtual ~CZ3DRepresentation(void);



	


	int getMemoryEstimate(const CCubef &p_cube, float compenstae, float resolution);
	virtual void init(CCubef p_originalSolidCube, float p_compensate, float p_resolution);//初始化函数，申请内存空间。
	//void free(void) { assert(bInited()); delete []m_buff; m_buff=NULL; };//释放内存空间
	bool bInited(void) { return m_buff!=NULL; };//检测是否初始化
	
	//一些get（）函数 
	CCubed getOrginalSolidCube(void) { assert(bInited()); return m_orgCube; };//获得原始立方体
	CCubed getCube(void);//获得补偿后的立方体
	double getCompensate(void);//获得补偿值
	double getRessolution(void);//获得离散的单位长度
	int getMaxX(){ return m_maxX; };//获得离散后的x的最大值
	int getMaxY(){ return m_maxY; };
	int getMaxZ(){ return m_maxZ; };
           
	//用来给Byte的每个位赋值
	enum CDotMask{	dmSolid=1, dmBoundary=2, dmFront=4,//x+1                                       
					dmBack=8/*x-1*/, dmLeft=16/*y+1*/, dmRight=32/*y-1*/,
					dmUP=64/*z+1*/, dmDown=128//z-1
				};//dmSolid,dmBoundary分别代表缓冲区中每一个字节的0位和1位,在对应的位上赋1
	
	//形成离散化的实体表示,p_sloid 为输入的实体,p_orMsak,p_andMask 分别表示或，与掩码。看下这个函数，执行时要用到
	virtual void makeSolid(vtkPolyData *p_solid, Byte p_orMask, Byte p_andMask = 0xff);//p_andMask,p_orMask 运算掩码                   
	void makeSphere(CPoint3d p_center, double p_radious, Byte p_orMask,                                  
		Byte p_andMask = 0xff);//形成一个离散化的球形区域，并与掩码运算,p_center球形区域的实际中心,p_radious  球形区域的实际半径
	void makeTriangl(CPoint3f p_p1, CPoint3f p_p2, CPoint3f p_p3, float p_radious, //p_p1,p_p2,p_p3 分别为三角形的三个顶点,p_radious  圆的半径
		Byte p_orMask, Byte p_andMask = 0xff);//形成一个离散化的三角柱区域（是一个半圆弧的中心以三角形为轨迹形成的封闭区域）,并与掩码运算
	void makeHalfSphereDir(CPoint3f p_center, CPoint3f p_dir, float p_radious, 
		Byte p_orMask, Byte p_andMask = 0xff);//形成一个离散化的半球区域，p_dir半球的方向,即半球圆面的内法线
	void makeTrianglDir(CPoint3f p_p1, CPoint3f p_p2, CPoint3f p_p3, bool p_bExterior, //p_p1,p_p2,p_p3三个顶点, p_bExterior 半三角柱的方向
		float p_radious, Byte p_orMask, Byte p_andMask = 0xff);//形成一个离散化的半三角柱区域（是1/4圆弧的中心以三角形为轨迹形成的封闭区域）
	
	//支撑部分
	enum CSupportType{stCircle, stSquare};//支撑的类型 stCircle表示圆柱形支撑，stSquare表示立方柱支撑
	void makeSupportMatrix(CSupportType p_type, double p_diameter, double p_interval, //形成支撑, p_type 支撑的类型, p_diameter 支撑的直径或宽度
		CPoint2d p_offset) ;// p_interval 相邻支撑间的中心间距，p_offset 支撑从边缘向内偏移的实际距离
        
	Byte getDot(CPoint3d p_point);//获取空间点的坐标值,p_point 空间点的座标
    bool bDotInSolid(CPoint3d p_point);//判断空间一点是否在实体内部,p_point 空间点的座标
	
	//输出二进制的stl文件, p_fn 文件名, p_smoothness    最大允许误差
	void exportBinarySTL(const char* p_fn, float p_smoothness, vtkPolyData *p_solid, const int &p_hellowtype, const int &p_suppertype, const float &p_diameter, const float &p_interval);
        
	//图形绘制，这里是用VTK进行绘制
	virtual void glDraw(vtkSmartPointer<vtkPoints> ugpoints, vtkSmartPointer<vtkUnstructuredGrid> ug, vtkSmartPointer<vtkCellArray> cells);
	virtual void glDraw(vtkSmartPointer<vtkPoints> ugpoints, vtkSmartPointer<vtkUnstructuredGrid> ug, vtkSmartPointer<vtkCellArray> cells,double x,double y,double z);
	/*virtual void glDrawMain(vtkSmartPointer<vtkPoints> ugpoints, vtkSmartPointer<vtkUnstructuredGrid> ug, vtkSmartPointer<vtkCellArray> cells,float h);*/
	/*virtual void glDrawCurrent(vtkSmartPointer<vtkPoints> ugpoints, vtkSmartPointer<vtkUnstructuredGrid> ug, vtkSmartPointer<vtkCellArray> cells, float h);
	virtual void glDrawNext(vtkSmartPointer<vtkPoints> ugpoints, vtkSmartPointer<vtkUnstructuredGrid> ug, vtkSmartPointer<vtkCellArray> cells, float h);*/

	virtual bool bInRegionF(float p_z){ return in(double(p_z),m_cube.min.z,m_cube.max.z); }

	//获取改点的体素索引【体素索引是重要的】
	virtual CPoint3i getVoxel3DIndex(CPoint3d &point_3d)                    
	{
		return toInt(point_3d);
	}
	virtual CPoint3i getVoxel3DIndex(double x, double y, double z)
	{
		CPoint3i ret;
		ret.x = toIntX(x);
		ret.y = toIntX(y);
		ret.z = toIntX(z);
		return ret;
	}

	//判断是否为边界、是否在实体内等
	bool bBoundary(int x, int y, int z)const;
	bool bDotInSolid(int p_x, int p_y, int p_z)const;
	bool bBoundaryAtZ(int x, int y, int z)const;
	bool bTopBoundary(int p_x, int p_y, int p_z);
	bool bDotInTopLayer(int p_x, int p_y, int p_z);
	bool bBottomBoundary(int p_x, int p_y, int p_z);

	Byte*getBuff() { return m_buff; }

	//实际坐标转为体素坐标
	int toInt(double p_x) {
		int ret = (int)(ceil(p_x / m_resolution));
		double reserve = abs((double)ret - p_x / m_resolution);
		ret -= (reserve > m_resolution) ? 1 : 0;
		return ret + 1;
	}
	
	//二维层面上的体素坐标与实际坐标相互转化（体素左边是int类型的，实际坐标是double类型的）
	CPoint2d IntToDouble(CPoint2i val)//体素转实际坐标
	{
		CPoint2d ret;
		ret.x = (val.x )*m_resolution + m_cube.min.x;
		ret.y = (val.y )*m_resolution + m_cube.min.y;
		return ret;
	}
	CPoint2i DoubleToInt(CPoint2d val)//实际坐标转体素坐标
	{
		CPoint2i ret;

		int retx = (int)(ceil(val.x / m_resolution));
		double reserve = abs((double)retx - val.x / m_resolution);
		retx -= (reserve > m_resolution) ? 1 : 0;
		
		int rety = (int)(ceil(val.y / m_resolution));
		reserve = abs((double)rety - val.y / m_resolution);
		rety -= (reserve > m_resolution) ? 1 : 0;
		ret.x = retx + 1;
		ret.y = rety + 1;
		return ret;
	}

	//生成曲面[这几块是核心]-------------------------------------------------------1
	//刀具曲面分层
	void makeTopCurvedLayer(void);
	void makeTopCurvedLayer2(Byte* a,int shift_x, int shift_y, int shift_z, int x1, int y1, int z1);//用基板底片做刀具
	void makeBottomCurvedLayer(void);
	void makeZCurvedLayer(int zFromToP);//获得离top曲面距离为zFromToP个体素的曲面层，作为该层的刀具
	void makeZCurvedLayer2(int zFromBottom);//从下往上分层
	void getCurvedLayer(Byte *buff);

	//基于生长线的曲面分层
	void makeGrowthLineCurveLayer(double d, Buff_LineCurvedLayer &m_buff_LineCurvedLayer);
	void makeGrowthLineVirousCurveLayer(double dmin, double dmax, Buff_LineCurvedLayer &m_buff_LineCurvedLayer);

	//结束生成曲面[这几块是核心]-------------------------------------------------------1


	//生成最上层平面
	void makeTopFlatLayer(void);
	//生成剩下未规划路径的实体
	void makeAirSolid(void);
	
	//新思路：生成一个刀具曲面，用刀具曲面从上往下去切层
	//切完之后放到一个新的数组里去
	//void makeToolSurface(CurveArray p_curveArray);


	//由离散体素坐标到实际坐标
	virtual CPoint3d toDouble(const CPoint3i &p_intCoordinates);    //进行加工的时候可以把离散坐标换到实际坐标，一般实际坐标的类型为double。
	virtual double toDoubleX(int p_x);//单独变换某一轴方向的坐标值
	virtual double toDoubleY(int p_y);
	virtual double toDoubleZ(int p_z);
	virtual CPoint3i toInt(const CPoint3d &p_doubleCoordinates);
	virtual CPoint3f toFloat(const CPoint3i &p_intCoordinates);
	



	Byte* m_buff_CurvedLayer;//临时存储刀具曲面层的体素数据,因为要在类层面调用这个数组
	/*********************************************************************************************/

protected:
	
	//删除小的孔洞
	void removeVerySmallPart();
	

	//扫描一层实体形成一层三维表示层, p_z  扫描层的实际高度,p_slice 扫描层的实际轮廓，即切片的实际数据
	virtual void scan(double p_z, vtkSmartPointer<vtkPolyData>p_slice, Byte p_orMask, Byte p_andMask = 0xff);
	//找出边界并标示
	void maskBoundary(void);


	//获得离散方体内某一座标点的属性,  p_x, p_y, p_z 离散方体内某一点三个方向的座标
	virtual Byte &dot(int p_x, int p_y, int p_z);
	virtual Byte dot(int p_x, int p_y, int p_z) const;
	virtual Byte &dot(const CPoint3i &p_coordinate);
	virtual Byte dot(const CPoint3i &p_coordinate) const;
	
	int m_maxX, m_maxY, m_maxZ;//离散方体三个方向的最大长度
        
	int m_yCoefficient, m_zCoefficient;//缓冲区位置计算系数
	//m_yCoefficient = m_maxX + 1;
	//m_zCoefficient = (m_maxY + 1)*(m_maxX + 1);【缓冲区是一维的，实体坐标是三维的，这里由三维转化为一维】
	
	int m_AccurateOfResolution;

	Byte* m_buff;//缓冲区指针，存储STL体素化后的体素      
	
	//Byte* m_buff_CurvedLayer;//临时存储刀具曲面层的体素数据
	Byte* m_buff_CurvedLayer2;//用于存储当前刀具曲面层的体素数据


	//要注意，因为在转换为体素化坐标时，是减去m_cube的最小点，但是这个坐标是小于原始cube最小点的；
	virtual int toIntX(double p_x);//由实际座标到离散体素座标的转化函数
	virtual int toIntY(double p_y);
	virtual int toIntZ(double p_z);
	
	CCubed m_orgCube, m_cube;// m_orgCube原来的实际方体，m_cube补偿的方体
        
	float m_compensate, m_resolution;// m_compensate补偿距离（边界判断时需要，具体可以去理解边界判断的方法），m_resolution离散的单位长度 

	//下面这部分暂时不用
	void makeTriFacet(CTriangled& p_tri, Byte p_orMask, Byte p_andMask = 0xff);
	void make3DLine(const CPoint3d& p_p1, const CPoint3d& p_p2, Byte p_orMask, Byte p_andMask = 0xff);
	void makeRing(const CZArray<CPoint3d, false> &p_ring3d, const CPoint3d &p_normal, const double &p_hight, Byte p_orMask, Byte p_andMask = 0xff);
	void exportSubCubeFacet(CVFacetExporter& p_exporter, const CPoint3i& p_subCubeCord, int p_dirID, bool bConverse);

	void exportFacets(CVFacetExporter& p_exporter, bool bConverse);
	void exportBinarySTL(const char* p_fn, bool bConverse);
	
private:
	
};

//以下是这个类的一些函数的实现部分，另一部分是在.cpp文件中
//引用传参修改的是原始数据，而且不会产生副本，效率更高
inline Byte &CZ3DRepresentation::dot(int p_x, int p_y, int p_z)           
{
	//assert(in(p_x, 0, m_maxX)) ;
	//assert(in(p_y, 0, m_maxY)) ;
	//assert(in(p_z, 0, m_maxZ)) ;
	return m_buff[p_z*m_zCoefficient + p_y*m_yCoefficient + p_x];
};

inline Byte CZ3DRepresentation::dot(int p_x, int p_y, int p_z) const
{
	assert(in(p_x, 0, m_maxX)) ;
	assert(in(p_y, 0, m_maxY)) ;
	assert(in(p_z, 0, m_maxZ)) ;
	return m_buff[p_z*m_zCoefficient + p_y*m_yCoefficient + p_x];
};

inline CCubed CZ3DRepresentation::getCube(void)
{
	assert(bInited());
	return m_cube;
}

inline double CZ3DRepresentation::getCompensate(void)
{
	assert(bInited());
	return m_compensate;
}

inline double CZ3DRepresentation::getRessolution(void)
{ 
	assert(bInited());
	return m_resolution;
}

inline Byte CZ3DRepresentation::getDot(CPoint3d p_point)
{
	CPoint3i p=toInt(p_point);
	return dot(p);
}

inline bool CZ3DRepresentation::bDotInSolid(CPoint3d p_point)
{
	Byte byteInsolid=getDot(p_point);
	return byteInsolid&dmSolid;
}

inline bool CZ3DRepresentation::bDotInTopLayer(int p_x, int p_y, int p_z)
{
	Byte byteInTopLayer =dot(p_x, p_y, p_z);
	return byteInTopLayer&dmFront;
}

inline bool CZ3DRepresentation::bDotInSolid(int p_x, int p_y, int p_z) const
{
	Byte byteInsolid =dot(p_x, p_y, p_z);
	return byteInsolid&dmSolid;
}

inline Byte & CZ3DRepresentation::dot(const CPoint3i &p_coordinate)
{
	return dot(p_coordinate.x, p_coordinate.y, p_coordinate.z);
}

inline Byte  CZ3DRepresentation::dot(const CPoint3i &p_coordinate)const
{
	return dot(p_coordinate.x, p_coordinate.y, p_coordinate.z);
}

inline CPoint3f CZ3DRepresentation::toFloat(const CPoint3i &p_intCoordinates)
{
	assert(in(p_intCoordinates.x, 0, m_maxX)&&in(p_intCoordinates.y, 0, m_maxY)&&in(p_intCoordinates.z, 0, m_maxZ));
	double r=m_resolution;
	const CPoint3i &i=p_intCoordinates;
	CPoint3d p3f=point3(
		r*(double(i.x))+m_cube.min.x, 
		r*(double(i.y))+m_cube.min.y, 
		r*(double(i.z))+m_cube.min.z);
	return point3(float(p3f.x), float(p3f.y), float(p3f.z));
}

inline CPoint3d CZ3DRepresentation::toDouble(const CPoint3i &p_intCoordinates)
{
	assert(in(p_intCoordinates.x, 0, m_maxX)&&in(p_intCoordinates.y, 0, m_maxY)&&in(p_intCoordinates.z, 0, m_maxZ));
	double r=m_resolution;
	const CPoint3i &i=p_intCoordinates;
	CPoint3d p3f=point3(
		r*(double(i.x))+double(m_cube.min.x), 
		r*(double(i.y))+double(m_cube.min.z), 
		r*(double(i.z))+double(m_cube.min.z));
	return p3f;
}

inline double  CZ3DRepresentation::toDoubleX(int p_x)
{
	return m_resolution*(double(p_x))+m_cube.min.x;
}

inline double CZ3DRepresentation::toDoubleY(int p_y)
{
	return m_resolution*(double(p_y))+m_cube.min.y;
}

inline double CZ3DRepresentation::toDoubleZ(int p_z)
{
	return m_resolution*(double(p_z))+m_cube.min.z;
}

inline CPoint3i CZ3DRepresentation::toInt(const CPoint3d &p_doubleCoordinates)
{
	const CPoint3d &p = p_doubleCoordinates;
	assert(in(p.x, m_cube.min.x, m_cube.max.x)&&in(p.y, m_cube.min.y, m_cube.max.y)&&in(p.z, m_cube.min.z, m_cube.max.z));    
	CPoint3i p3i;
	p3i.x = toIntX(p.x);
	p3i.y = toIntX(p.y);
	p3i.z = toIntX(p.z);
	return p3i;
}

inline int CZ3DRepresentation::toIntX(double p_x)
{
	//assert(in(p_x, m_cube.min.x, m_cube.max.x) );
	//return (int) ((p_x-m_cube.min.x)/m_resolution);
	int ret = (int)(ceil(p_x / m_resolution));
	double reserve = abs((double)ret - p_x / m_resolution);
	ret -= (reserve > m_resolution) ? 1 : 0;
	return ret+1;
}

inline int CZ3DRepresentation::toIntY(double p_y)
{
	//assert(in(p_y, m_cube.min.y, m_cube.max.y) );
	//return (int) ((p_y-m_cube.min.y)/m_resolution);
	int ret = (int)(ceil(p_y / m_resolution));
	double reserve = abs((double)ret - p_y / m_resolution);
	ret -= (reserve > m_resolution) ? 1 : 0;
	return ret+1;
}

inline int CZ3DRepresentation::toIntZ(double p_z)
{
	//in(p_z, m_cube.min.z, m_cube.max.z)检查p_z是否在该立方体的最小值和最大值之间
	//assert（）函数是判断上述检查是否正确，正确则继续，错误则停止运行。
	assert(in(p_z, m_cube.min.z, m_cube.max.z) );
	
	//return (int)((p_z-m_cube.min.z)/m_resolution);
	
	//ceil（）函数返回大于或等于表达式的最小整数
	int ret = (int)(ceil(p_z / m_resolution));
	double reserve = abs((double)ret - p_z / m_resolution);
	ret -= (reserve > m_resolution) ? 1 : 0;
	return ret+1;

}

#endif //__Z3D_REPRESENTATION_H
