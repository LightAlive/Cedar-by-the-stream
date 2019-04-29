//3DRepresentation.h ��άʵ����ɢ���ı�ʾ��

//˵����
//STLʵ�壨���꣩��ʵ���壨���꣩��ʵ�壨���꣩����ʾ�ռ�ʵ�ʵ�������ʵ�壨���꣩
//��ɢ�����꣩����ɢ����ʵ�壨���꣩����ʾ������ɢ���ķ�������ʵ�壨���꣩
//
//��Ҫ��;��
//1��STL�ļ���¦��
//2��STL�ļ�����ƫ��
//3���γɼӹ������е�֧����
//4: ��ʧ���ȵĲ�������
//��Ҫ���ܣ�
//1����STLʵ���ʾ����ɢ��ʵ�塣
//2���γ������Ρ�Բ������ռ�
//3���γ������Ρ�Բ�Ĵ�����İ�����ռ�
//4: �γ�Բ���������֧��
//5�������������Ƭ����
//6�����������STL�ļ�
//7����������ʾ
//
#ifndef __Z3D_REPRESENTATION_H
#define __Z3D_REPRESENTATION_H
#include "TEACDef.h"
#include"SolidDef.h"
#include"stdafx.h"

//������������,���ʵ�����Ƭ��
typedef int CurveArray;//��������Ԫ���Ǵ��zֵ�ģ���������Ǹ���ά��
typedef vector<CurveArray> SolidArray;//�����һά�����飬�������ʵ�����Ƭ��

typedef std::vector<Byte*> Buff_LineCurvedLayer;//���Դ�Ÿ�������buff

struct CTrianglef //���ڴ��������Ƭ
{
	CPoint3f p[3];
};

struct CTriangled
{
	CPoint3d p[3];
};

const CPoint3i cCubeFacetDirs[6] =//ÿ��Ԫ�ر�ʾ��������ĳ�������ϵ���
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

class CVFacetExporter                                                   //�����Ƭ��
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
//class CFacets ������Ƭ�����飬��ҪΪ��¼�������������

class vtkPolyData;
class vtkUnstructuredGrid;


class CZ3DRepresentation                                         
{
public:
	#pragma pack(push, 2)//��������÷�����Ҫ�˽���Բ�һ��
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

	//���캯��������Ĺ��캯�����ĸ��������ͣ�ԭ�����壨Ҫ���ػ���ʵ��ģ�ͣ�����CCubef���͵ģ�Ҫȥ��һ�������
	//����ƫ�þ��룺���ƫ���������жϱ߽�ģ��������ɵ�����ʵ���ϱ�ԭ����ʵ�����һ�����ȵľ��롣����buff��ʱ���Ԫ��0��Ԫ��MAXȥ����
	//�ٽ���һ������ƫ�þ��룺���жϱ߽紦��㣨x+1,y,z��������ʵ���ڣ�������鲻����������ô�Ͳ����ڣ�x+1��y,z)����㡣
	CZ3DRepresentation(void);
	CZ3DRepresentation(CCubef p_orginalSolidCube, float p_compensate, float p_resolution);
	//virtual ~CZ3DRepresentation(void);



	


	int getMemoryEstimate(const CCubef &p_cube, float compenstae, float resolution);
	virtual void init(CCubef p_originalSolidCube, float p_compensate, float p_resolution);//��ʼ�������������ڴ�ռ䡣
	//void free(void) { assert(bInited()); delete []m_buff; m_buff=NULL; };//�ͷ��ڴ�ռ�
	bool bInited(void) { return m_buff!=NULL; };//����Ƿ��ʼ��
	
	//һЩget�������� 
	CCubed getOrginalSolidCube(void) { assert(bInited()); return m_orgCube; };//���ԭʼ������
	CCubed getCube(void);//��ò������������
	double getCompensate(void);//��ò���ֵ
	double getRessolution(void);//�����ɢ�ĵ�λ����
	int getMaxX(){ return m_maxX; };//�����ɢ���x�����ֵ
	int getMaxY(){ return m_maxY; };
	int getMaxZ(){ return m_maxZ; };
           
	//������Byte��ÿ��λ��ֵ
	enum CDotMask{	dmSolid=1, dmBoundary=2, dmFront=4,//x+1                                       
					dmBack=8/*x-1*/, dmLeft=16/*y+1*/, dmRight=32/*y-1*/,
					dmUP=64/*z+1*/, dmDown=128//z-1
				};//dmSolid,dmBoundary�ֱ����������ÿһ���ֽڵ�0λ��1λ,�ڶ�Ӧ��λ�ϸ�1
	
	//�γ���ɢ����ʵ���ʾ,p_sloid Ϊ�����ʵ��,p_orMsak,p_andMask �ֱ��ʾ�������롣�������������ִ��ʱҪ�õ�
	virtual void makeSolid(vtkPolyData *p_solid, Byte p_orMask, Byte p_andMask = 0xff);//p_andMask,p_orMask ��������                   
	void makeSphere(CPoint3d p_center, double p_radious, Byte p_orMask,                                  
		Byte p_andMask = 0xff);//�γ�һ����ɢ�����������򣬲�����������,p_center���������ʵ������,p_radious  ���������ʵ�ʰ뾶
	void makeTriangl(CPoint3f p_p1, CPoint3f p_p2, CPoint3f p_p3, float p_radious, //p_p1,p_p2,p_p3 �ֱ�Ϊ�����ε���������,p_radious  Բ�İ뾶
		Byte p_orMask, Byte p_andMask = 0xff);//�γ�һ����ɢ����������������һ����Բ����������������Ϊ�켣�γɵķ������,������������
	void makeHalfSphereDir(CPoint3f p_center, CPoint3f p_dir, float p_radious, 
		Byte p_orMask, Byte p_andMask = 0xff);//�γ�һ����ɢ���İ�������p_dir����ķ���,������Բ����ڷ���
	void makeTrianglDir(CPoint3f p_p1, CPoint3f p_p2, CPoint3f p_p3, bool p_bExterior, //p_p1,p_p2,p_p3��������, p_bExterior ���������ķ���
		float p_radious, Byte p_orMask, Byte p_andMask = 0xff);//�γ�һ����ɢ���İ�������������1/4Բ����������������Ϊ�켣�γɵķ������
	
	//֧�Ų���
	enum CSupportType{stCircle, stSquare};//֧�ŵ����� stCircle��ʾԲ����֧�ţ�stSquare��ʾ������֧��
	void makeSupportMatrix(CSupportType p_type, double p_diameter, double p_interval, //�γ�֧��, p_type ֧�ŵ�����, p_diameter ֧�ŵ�ֱ������
		CPoint2d p_offset) ;// p_interval ����֧�ż�����ļ�࣬p_offset ֧�Ŵӱ�Ե����ƫ�Ƶ�ʵ�ʾ���
        
	Byte getDot(CPoint3d p_point);//��ȡ�ռ�������ֵ,p_point �ռ�������
    bool bDotInSolid(CPoint3d p_point);//�жϿռ�һ���Ƿ���ʵ���ڲ�,p_point �ռ�������
	
	//��������Ƶ�stl�ļ�, p_fn �ļ���, p_smoothness    ����������
	void exportBinarySTL(const char* p_fn, float p_smoothness, vtkPolyData *p_solid, const int &p_hellowtype, const int &p_suppertype, const float &p_diameter, const float &p_interval);
        
	//ͼ�λ��ƣ���������VTK���л���
	virtual void glDraw(vtkSmartPointer<vtkPoints> ugpoints, vtkSmartPointer<vtkUnstructuredGrid> ug, vtkSmartPointer<vtkCellArray> cells);
	virtual void glDraw(vtkSmartPointer<vtkPoints> ugpoints, vtkSmartPointer<vtkUnstructuredGrid> ug, vtkSmartPointer<vtkCellArray> cells,double x,double y,double z);
	/*virtual void glDrawMain(vtkSmartPointer<vtkPoints> ugpoints, vtkSmartPointer<vtkUnstructuredGrid> ug, vtkSmartPointer<vtkCellArray> cells,float h);*/
	/*virtual void glDrawCurrent(vtkSmartPointer<vtkPoints> ugpoints, vtkSmartPointer<vtkUnstructuredGrid> ug, vtkSmartPointer<vtkCellArray> cells, float h);
	virtual void glDrawNext(vtkSmartPointer<vtkPoints> ugpoints, vtkSmartPointer<vtkUnstructuredGrid> ug, vtkSmartPointer<vtkCellArray> cells, float h);*/

	virtual bool bInRegionF(float p_z){ return in(double(p_z),m_cube.min.z,m_cube.max.z); }

	//��ȡ�ĵ������������������������Ҫ�ġ�
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

	//�ж��Ƿ�Ϊ�߽硢�Ƿ���ʵ���ڵ�
	bool bBoundary(int x, int y, int z)const;
	bool bDotInSolid(int p_x, int p_y, int p_z)const;
	bool bBoundaryAtZ(int x, int y, int z)const;
	bool bTopBoundary(int p_x, int p_y, int p_z);
	bool bDotInTopLayer(int p_x, int p_y, int p_z);
	bool bBottomBoundary(int p_x, int p_y, int p_z);

	Byte*getBuff() { return m_buff; }

	//ʵ������תΪ��������
	int toInt(double p_x) {
		int ret = (int)(ceil(p_x / m_resolution));
		double reserve = abs((double)ret - p_x / m_resolution);
		ret -= (reserve > m_resolution) ? 1 : 0;
		return ret + 1;
	}
	
	//��ά�����ϵ�����������ʵ�������໥ת�������������int���͵ģ�ʵ��������double���͵ģ�
	CPoint2d IntToDouble(CPoint2i val)//����תʵ������
	{
		CPoint2d ret;
		ret.x = (val.x )*m_resolution + m_cube.min.x;
		ret.y = (val.y )*m_resolution + m_cube.min.y;
		return ret;
	}
	CPoint2i DoubleToInt(CPoint2d val)//ʵ������ת��������
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

	//��������[�⼸���Ǻ���]-------------------------------------------------------1
	//��������ֲ�
	void makeTopCurvedLayer(void);
	void makeTopCurvedLayer2(Byte* a,int shift_x, int shift_y, int shift_z, int x1, int y1, int z1);//�û����Ƭ������
	void makeBottomCurvedLayer(void);
	void makeZCurvedLayer(int zFromToP);//�����top�������ΪzFromToP�����ص�����㣬��Ϊ�ò�ĵ���
	void makeZCurvedLayer2(int zFromBottom);//�������Ϸֲ�
	void getCurvedLayer(Byte *buff);

	//���������ߵ�����ֲ�
	void makeGrowthLineCurveLayer(double d, Buff_LineCurvedLayer &m_buff_LineCurvedLayer);
	void makeGrowthLineVirousCurveLayer(double dmin, double dmax, Buff_LineCurvedLayer &m_buff_LineCurvedLayer);

	//������������[�⼸���Ǻ���]-------------------------------------------------------1


	//�������ϲ�ƽ��
	void makeTopFlatLayer(void);
	//����ʣ��δ�滮·����ʵ��
	void makeAirSolid(void);
	
	//��˼·������һ���������棬�õ��������������ȥ�в�
	//����֮��ŵ�һ���µ�������ȥ
	//void makeToolSurface(CurveArray p_curveArray);


	//����ɢ�������굽ʵ������
	virtual CPoint3d toDouble(const CPoint3i &p_intCoordinates);    //���мӹ���ʱ����԰���ɢ���껻��ʵ�����꣬һ��ʵ�����������Ϊdouble��
	virtual double toDoubleX(int p_x);//�����任ĳһ�᷽�������ֵ
	virtual double toDoubleY(int p_y);
	virtual double toDoubleZ(int p_z);
	virtual CPoint3i toInt(const CPoint3d &p_doubleCoordinates);
	virtual CPoint3f toFloat(const CPoint3i &p_intCoordinates);
	



	Byte* m_buff_CurvedLayer;//��ʱ�洢������������������,��ΪҪ�����������������
	/*********************************************************************************************/

protected:
	
	//ɾ��С�Ŀ׶�
	void removeVerySmallPart();
	

	//ɨ��һ��ʵ���γ�һ����ά��ʾ��, p_z  ɨ����ʵ�ʸ߶�,p_slice ɨ����ʵ������������Ƭ��ʵ������
	virtual void scan(double p_z, vtkSmartPointer<vtkPolyData>p_slice, Byte p_orMask, Byte p_andMask = 0xff);
	//�ҳ��߽粢��ʾ
	void maskBoundary(void);


	//�����ɢ������ĳһ����������,  p_x, p_y, p_z ��ɢ������ĳһ���������������
	virtual Byte &dot(int p_x, int p_y, int p_z);
	virtual Byte dot(int p_x, int p_y, int p_z) const;
	virtual Byte &dot(const CPoint3i &p_coordinate);
	virtual Byte dot(const CPoint3i &p_coordinate) const;
	
	int m_maxX, m_maxY, m_maxZ;//��ɢ���������������󳤶�
        
	int m_yCoefficient, m_zCoefficient;//������λ�ü���ϵ��
	//m_yCoefficient = m_maxX + 1;
	//m_zCoefficient = (m_maxY + 1)*(m_maxX + 1);����������һά�ģ�ʵ����������ά�ģ���������άת��Ϊһά��
	
	int m_AccurateOfResolution;

	Byte* m_buff;//������ָ�룬�洢STL���ػ��������      
	
	//Byte* m_buff_CurvedLayer;//��ʱ�洢������������������
	Byte* m_buff_CurvedLayer2;//���ڴ洢��ǰ������������������


	//Ҫע�⣬��Ϊ��ת��Ϊ���ػ�����ʱ���Ǽ�ȥm_cube����С�㣬�������������С��ԭʼcube��С��ģ�
	virtual int toIntX(double p_x);//��ʵ�����굽��ɢ���������ת������
	virtual int toIntY(double p_y);
	virtual int toIntZ(double p_z);
	
	CCubed m_orgCube, m_cube;// m_orgCubeԭ����ʵ�ʷ��壬m_cube�����ķ���
        
	float m_compensate, m_resolution;// m_compensate�������루�߽��ж�ʱ��Ҫ���������ȥ���߽��жϵķ�������m_resolution��ɢ�ĵ�λ���� 

	//�����ⲿ����ʱ����
	void makeTriFacet(CTriangled& p_tri, Byte p_orMask, Byte p_andMask = 0xff);
	void make3DLine(const CPoint3d& p_p1, const CPoint3d& p_p2, Byte p_orMask, Byte p_andMask = 0xff);
	void makeRing(const CZArray<CPoint3d, false> &p_ring3d, const CPoint3d &p_normal, const double &p_hight, Byte p_orMask, Byte p_andMask = 0xff);
	void exportSubCubeFacet(CVFacetExporter& p_exporter, const CPoint3i& p_subCubeCord, int p_dirID, bool bConverse);

	void exportFacets(CVFacetExporter& p_exporter, bool bConverse);
	void exportBinarySTL(const char* p_fn, bool bConverse);
	
private:
	
};

//������������һЩ������ʵ�ֲ��֣���һ��������.cpp�ļ���
//���ô����޸ĵ���ԭʼ���ݣ����Ҳ������������Ч�ʸ���
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
	//in(p_z, m_cube.min.z, m_cube.max.z)���p_z�Ƿ��ڸ����������Сֵ�����ֵ֮��
	//assert�����������ж���������Ƿ���ȷ����ȷ�������������ֹͣ���С�
	assert(in(p_z, m_cube.min.z, m_cube.max.z) );
	
	//return (int)((p_z-m_cube.min.z)/m_resolution);
	
	//ceil�����������ش��ڻ���ڱ��ʽ����С����
	int ret = (int)(ceil(p_z / m_resolution));
	double reserve = abs((double)ret - p_z / m_resolution);
	ret -= (reserve > m_resolution) ? 1 : 0;
	return ret+1;

}

#endif //__Z3D_REPRESENTATION_H
