#ifndef __SOLID_DEF_H__
#define __SOLID_DEF_H__

#include <assert.h>
#include "stdafx.h"

/*
文件：TEACDef.h
用途：HRP实体相关基本类原形定义头文件
作者：张李超(teac)
日期：1999.9~
历史：	1999.9~		：初版；
		2000.11~	：接口进一步增强抽象性，适用于各型RP机；
概要：包括：CZRing环，CZSlice切片，CSolid实体，CGrid网格，
			Solid Manager 实体管理器，HRP Virtual Machine 虚拟机
			等基本HRP相关对象定义
*/

//
// STL general number type definion
// 
// typedef float CS2dNumber;	//here S refer to STL
// typedef CPoint2<CS2dNumber> CSPoint2;
// 
// typedef CCube<float> CSTLCube;
// 
// 
// CZRing 环
// 
// CZRing is a class to create/store/draw the ring.
// It's based on CZArray<CSPoint2> which store the ring node data.
// 
// class CZRing : public CZArray<CSPoint2, false> 
// {
// public:
// 	//constructor&destructor
// 	CZRing(void) : m_state(csAdding) {};
// 	~CZRing(void);
// 
// 	//attributes
// 		//attribute dot(i) return the No.i(based on zero) dot of the ring, if i is larger
// 		//than the size of the ring, it's wraped to the heard of the ring.
// 	CSPoint2 &dot(int p_id) { return m_data[iMod(p_id, m_size)]; };
// 	//返回的是余数对应的点,把他看成环了
// 	const CSPoint2 &dot(int p_id) const { return m_data[p_id%m_size]; };
// 
// 	//ring creating method
// 	void addPoint(const CSPoint2 &p_newPoint);//增加点,同时,处理共线(共点)问题
// 	void combine(const CZRing &p_ring1, bool p_bSameOrder);
// 	//把一个环追加到另一个环上,有同向与反向两种
// 	void over(bool p_bRinged);//把环加好之后,在处理首尾两点
// 		//when all addPoint is done call this method with parameter true to check 
// 		//	whether the head/tail point be line
// 		//	false to indicate to be hole tracking
// 
// 	void addArc(CPoint2f p_centerPos, float p_radious, float p_radStart=0, float p_radEnd=float(2*PI));
// 	//把圆弧离散成环
// 
// 	//inflate the ring by p_radious with 右手法则
// 	void inflate(float p_radious);//对于环进行修整,去掉共线,偏置干涉等缺陷.
// 
// 	//general method
// 	void glDraw(void) const;
// 	void glDrawSharpDots(void) const;
// 	void gDump(ostream &p_os) const;	//dump G code to p_os//生成G代码
// 
// 	//operators
// 	void shift(CSPoint2 p_shift);//偏移环
// 
// 	//performance test functions
// 	__test(void dumpRemoveDots(void);)
// 
// protected:
// 	//data member
// 	enum CRingState {csAdding, csNotRinged, csRinged} m_state;
// 
// 	//util func
// 	bool beAllLine(int p1, int p2, int p3); //是不是共线
// };
// 
// 
// CZSlice 切片
// 
// class CZSlice : public CZArray<CZRing, true>
// {
// public:
// 	//constructor&destructor
// 	CZSlice(void) : m_nFrameRingsCount(0) {;};
// 	~CZSlice(void) {;};
// 
// 	//general method
// 	void gDump(ostream &p_os) const;	//dump G code to p_os
// 	void glDrawSharpDots(bool p_bIgnoredFrame) const;
// 	void glDraw(bool p_bIgnoredFrame) const;
// 	void glDrawSharpDots(void);
// 	void inflate(float p_radious);//修整每一个环在偏置后的纰漏.
// 
// 	//operators
// 	void shift(CSPoint2 p_shift);//把切片的每一个环都平移.
// 	void combine(const CZSlice& p_additionalSlice);//只在尾部增加,不进行bool运算
// 	void addFrame(const CZRectf& p_rect, CS2dNumber p_round, bool p_bArced=false);
// 以外环的形式增加框架,注意的是,第一个参数表示框架的矩形,的二个参数表示框架的导角(导圆)半径,
// 第三个参数表示框架是不是要导圆,如果是TRUE的话,就是导圆.注意,这个函数不会自动的增加代表框架数目
// 的那个变量的值,用户要自己增加.
// 		//Note: addFrame will never increase m_nFrameRingsCount automaticly.
// 		//		User should use setFrameCount manully.
// 	void addCircle(CPoint2f p_centerPos, float p_radious);
// 	//只追加圆,把圆离散成环的形式,还进行共点的处理.
// 	void setFrameCount(int p_nFrameRingsCount) //处理个数问题(可以改变Frame的个数)
// 		{ assert(p_nFrameRingsCount>=0); m_nFrameRingsCount=p_nFrameRingsCount; };
// 	int getFrameCount(void) { return m_nFrameRingsCount; };//返回Frame的个数
// 	int getTrueRingsCount(void) const { return Max(0, size()-m_nFrameRingsCount); };
// 	//看看真正的环的个数.注意,Frame的意义可能是指一个一个的框架,是一个矩形的形式加进来的.他并不是
// 	//一个环,而是表示边界,因此有必要把它与环区分开来.
// protected:
// 	int m_nFrameRingsCount;	//表示这个框架的个数.
// 		//some HRP VM add frames as rings on the end of cut slice . This data member 
// 		//refer to the count.
// };
// 
// 
// CVSolid 虚拟实体
// CVSolid is a virtual solid definon class which define the virtual interface suce as
// read, write, draw, cut, transform, query state and so on.
// 
// class CZSolidManager;
// enum CSolidMsgID 
// { 
// 	smiDimChanged,
// 	smiSliceChanged,
// 	smiUserIDStart=10000
// };
// 
// class CVSolid
// {
// public:
// 	//constructor&destructor
// 	CVSolid(	CZSolidManager *p_manager=NULL, 
// 				CVMsgOutput *p_msgOut=NULL, 
// 				CZVProfiler *p_profiler=NULL) 
// 		{ attach(p_manager); attach(p_msgOut, p_profiler); };
// 	virtual ~CVSolid(void) {;};
// 	
// 	//init methods
// 	void attach(CZSolidManager *p_pManager);
// 	void attach(CVMsgOutput *p_msgOut=NULL, CZVProfiler *p_profiler=NULL);
// 
// 	//attributes
// 	virtual bool bOK(void) const =0;	//whether this object has raed a solid file successfully
// 	virtual const CSTLCube& cube(void) const =0;
// typedef CCube<float> CSTLCube;
// 	virtual const int getFaceCount(void) const //保证是不可以提取表面信息的.
// 		{ assert(!(getCapabilityWord()&canGetFace)); return 0; };
// 	virtual const char* getSimpInfo(void) const {return "";};//获得的简单的信息.
// 		//simp info(less than 20chars) used to be displayed on the panel of status bar
// 
// 	//file operation
// 	virtual void init(const char *fn)=0;	//read file fn
// 	virtual const char* getSupportedReadFileFilters(void)=0;
// 	virtual const char* getSupportedWriteFileFilters(void)//写的文件过滤是空格
// 				{ assert(!(getCapabilityWord()&canSave)); return ""; };
// 	virtual void save(const char* /*fn*/, int /*filterIndex*/=-1) 
// 				{ assert(!(getCapabilityWord()&canSave));};//保证性质字之中没有可以存盘的
// 	virtual int getFileType(void)	//file type is also file save filterIndex(zero based)
// 				{ assert(!(getCapabilityWord()&canSave)); return 0; };//获得文件类型
// 
// 	//methods
// 	virtual void glDraw(void) const { assert(!(getCapabilityWord()&canDraw));};
// 	virtual void cutSlice(float p_z, CZSlice &p_slice) /*const?*/ = 0;//切片
// 	virtual void transform(CMatrix4f p_transformMatrix)	{ assert(!(getCapabilityWord()&canTransform));};
// 	//改变形状
// 	//add by cjs
// 	virtual void getTri(int id, CPoint3f &p_p1, CPoint3f &p_p2, CPoint3f &p_p3)=0;
// 	//end add by cjs 
// 	//capability definion and attributes
// 	enum
// 	{
// 		canGetFace	= 1,	//also face count
// 		canDraw		= 2,	//gl draw
// 		canSave		= 4,
// 		canTransform= 8
// 	};
// 	virtual int getCapabilityWord(void) const = 0;
// 	
// protected:
// 	int notify(CSolidMsgID p_msgID, const void* p_buff=NULL);
// 		//sender message to manager(if exist);
// 
// 	CVMsgOutput *m_pMsgOut;//专管输出字符串的类.
// 	CZVProfiler *m_pProfiler;//记录消息与时间点以及时间片的信息,并以输出文件的方式输出的类.
// 	CZSolidManager *m_pManager;//对实体进行变换管理的类.
// };

//
//Grid class and its sub-member classes:
//Grid---X ScanLines--n*ScanLine--(y, x SubCords)
//     --Y ScanLines--...
//

//
//line scan type and const definiens
//

//RP Machine types
enum CRPMType 
{ 
	rtLOM=0, 
	rtSLS,
	rtFack,
	rtMaxWarp //this const must be put at the bottom
};

//how to deal with scan lines that are superposed with gridlines
enum CRingLineDealType 
{ 
	clDefault, 
	clGo, 
	clSkip 
};

//scanX/Y(0 and 1) are alwayes used as the array suffix
enum CZScanLineType 
{ 
	scanX=0,
	scanY=1, 
	scanUnknown, 
	scanBoth
};

struct CZLineScanSetting
{
		//General Setting
	CRPMType RPMType;
	CZScanLineType scanType;//scanX or scanY
	bool bBiDirection;		//bidirectional scan(always for LOM)
	CRingLineDealType ringLineDealing;//处理水平线，且正好位于扫描线上
							//clDefault: scan half of ring lines that combine to scan lines
							//clGo: scan all scan lines
							//clSkip: do not scan ring lines that combine to scan line
	int nIgnoredFrame;		//ignore the last n(IgnoredFrame) ring(frame) of the slice
	float intver;			//step of basic scan line
	CZRectf rect;			//rect of grid region, alway the same as xy plane of solid cube
	
	
		//X Multi-Scan
	int multiScanPower;		//2^n x multiscan lines  是不是在没两条扫描线之间进一步填充扫描线？
							//0 for no multi scan(include xy multiscan, EXCLUDE Z MULTISCAN)
	float multGate;			//minimum gate of x multiscan, used when multiScanPower is greated than 0 
	
		//XY Multi-Scan	
	bool bXYMultiGrid;		//x/y multi-grid

		//gap multi
	bool bGapMultiGrid;
	float gapRatio;
	
		//Z Multi-Scan
	bool bZMultiGrid;		//weather to use z multi-grid
	float zMultiGridSharpGate;
							//minimum gate of sharp z chage layer gap
};

//
//scan line
//
typedef CZArray<float> CZSubCords;
const int cEstScanLineEdgesCount=12;

class CZScanLine
{
public:
	//init
	CZScanLine(void) 
		:	subCords(cEstScanLineEdgesCount, -cEstScanLineEdgesCount*2), 
			m_mainCord(-1e10) {;};	
		//-1e10 make sure this object has not been inited
		//Note: esti is abbreviation of estimate

	void init (CZScanLineType p_scanType, float p_mainCord)
		{
			assert( (p_scanType==scanX) || (p_scanType==scanY) );
			m_scanType=p_scanType;
			m_mainCord=p_mainCord;
		}

	//attibutes
	int pointsCount(void) const { return subCords.size()/2*2; };
	float getMainCord(void) const { return m_mainCord; };
	const CPoint2f point(int p_i) const
		{
			assert (m_mainCord>-1e5);	//scan line has been inited;
			assert (p_i<pointsCount());
			return (m_scanType==scanX) 
				? point2(subCords[p_i], m_mainCord) : point2(m_mainCord, subCords[p_i]);
		}
		//if scanType is scanX, point is (subCords[i], mainCord)
		//if scanType is scanY, point is (mainCord, subCords[i])

public:
	//public data member
	CZSubCords subCords;

protected:
	float m_mainCord;	
	CZScanLineType m_scanType;
};

class CZScanLines : public CZArray<CZScanLine, true>
{
public:
	CZScanLines(CZScanLineType p_type=scanUnknown) : m_type(p_type) {;};

// 	void gDump(ostream &p_os) const;	//dump G code to p_os
// 	void glDraw(void) const;

	void setType(CZScanLineType p_type) { m_type=p_type; };	//set x or y scan
	CZScanLineType type(void) const {return m_type;};

protected:
	CZScanLineType m_type;
};

//
//line scan algorithm
//

//util functions for CZLineScanSetting
inline bool bNeedMultiSlice(const CZLineScanSetting &s)
{
	return (s.multGate>0) && s.bZMultiGrid && (s.zMultiGridSharpGate>0);
}

inline bool bNeedXYMultiGrid(const CZLineScanSetting &s)
{
	return (s.multGate>0) && s.bXYMultiGrid;
}

//
//algorithm line scan
//	scan one direction (scanX or scanY once a time)
//
class vtkPolyData;
class ILineScan
{
public:
	virtual void exec(
		const CZLineScanSetting &p_setting, 
		vtkSmartPointer<vtkPolyData>p_slice,
		CZScanLines &p_lines) = 0;
};

//
//algorithm grid scan
//  scan both scanX and scanY and use 3 slice(down, base and up) to make Z multi-scan.
//
struct CZGrid;
enum CSliceID { sDown=0, sBase, sUp };
class IGridScan
{
public:
	virtual void exec(
		const CZLineScanSetting &p_setting, 
		vtkPolyData *p_slices[3],
		CZGrid &p_grid
		) = 0;
};

//
//grid 网格
//
struct CZGrid
{
public:
	CZGrid(void);

	//make grid by two line scan(no Z multi scan)
	void make(	ILineScan* algor,				//pointer to algorithm object ILineScan
				const CZLineScanSetting &p_setting,	 
				vtkSmartPointer<vtkPolyData>p_slice);		//reference to current slice

	//make grid by one grid scan, 
	//give 3 slice(up, current and down slice) to make Z mului-scan
// 	void makeEx(IGridScan* algor,				//pointer to algorithm object IGridScan
// 				const CZLineScanSetting &p_setting,
// 				vtkPolyData /***/p_slices[3]			//array of slice pointers([down, base and up])
// 			   );
// 
// 	void gDump(ostream &p_os) const;	//dump G code to p_os
// 	void glDraw(void);
	void free(void) { lines[scanX].free(); lines[scanY].free(); };

	CZScanLines lines[scanY+1];
};


//
//FI fit&interpolation
//
// typedef CZArray<CPoint2f, false> CFIRing;
// typedef CZArray<CFIRing, true> CFISlice;
// 
// struct CFIOption
// {
// 	float	cutSpeed,//速度
// 			accuracy,//精度
// 			maxAccelerate,//加速度
// 			DDAPeriod;//
// };
// 
// void dealFI(const CZRing& p_inRing, CFIRing* p_pOutRing, CFIOption p_option);
// //Note:
// //currently, dealFI is implemented by LiuFI.cpp
// 
// //
// //Solid Manager 实体管理器
// //
// enum CProjectionMode {mPerspective, mOrtho};
// class CVHrpVM;
// 
// //
// //CZSolidManager
// //
// /*
// Solid Manager has 3 major functions as the following describes:
// 1. manage the dynamatic created solid object: attatch, deattatch;
// 2. draw solid view
// 3. handle notifify msg from solid object: onNotify
// */
// class CZSolidManager//问：GLenum是干什么用的？？？
// {
// public: 
// 	//constructor & destructor
// 	CZSolidManager(CVSolid *p_pSolid=NULL);
// ////作用:除了初始化管理器外,就是让管理器以外的一个指向管理器的指针指向创建的管理器,可见,管理器
// //只能创建一次.在Document以外的函数之中,可以调用管理器的公共接口.
// 	~CZSolidManager(void);
// 
// 	//operators
// 	void attachSolid(CVSolid *p_pSolid);
// //把原来的指向的实体删除,然后,把指向实体的变量指向函数的参数.同时,被指向的实体变量也要指向这个
// //实体管理器
// 		//IMPORTANT NOTICE: p_pSolid MUST BE A POINTER TO A DYNAMIC ALLOCED SOLID OBJECT
// 		//					AND SHOULD BE DELETED BY SOLID MANAGER ONLY!
// 	void deattachSolid(void) { attachSolid(NULL); };
// //与实体分开,其实作用就是把指向的实体删除,然后再把指向实体的指针赋为NULL
// 
// 	//
// 	//funcs for drawing solid
// 	//
// 	//draw
// 	void glDrawSolid(
// 		double p_aspect=1, 
// 		GLfloat p_matRot[][4]=NULL,	//matrix to rotate(transform) the solid while vieming 
// 		bool p_bMatRotFirst=false,	//on translate oreview mode, you should set bMatRotFirst to true
// 		double p_jitter_dx=0,
// 		double p_jitter_dy=0		//non-zero value of this two parameter refer to accume antilias
// 		);
// 
// 	//projection mode(ortho or perspective)
// 	CProjectionMode getProjectionMode(void) { return m_projectionMode;};//把m_projectionMode返回
// 
// //polygon mode for drawing solid
// 	void setSolidPolygonMode(GLenum p_mode)	
// 		{ assert( (p_mode==GL_POINT)||(p_mode==GL_LINE)||(p_mode==GL_FILL)); m_glPolygonMode=p_mode; };
// //作用:把显示模式设置出来
// 	GLenum getSolidPolygonMode(void) { return m_glPolygonMode; };
// //作用:返回显示模式
// 	//restore solid view state
// 	void adjustSolidView(void);
// 
// 	//solid pos
// 	//const CPoint3d &getPos(void) { return m_pos; };
// 	//void setPos(const CPoint3d &p_pos) { m_pos=p_pos; };
// 	CPoint3d &solidPos(void) { return m_pos; };//返回位置点的坐标
// 
// 	//solid fove in perspective mode
// 	double getFovy(void) { return m_fovy; };//把m_fovy返回
// 	void setFovy(double p_fovy) { m_fovy=p_fovy; bound(m_fovy, 0.1, 175.9); };
// //设置m_fovy的值,并保证它落在0.1与175.9之间
// 	//solid in ortho mode
// 	const COrthoMap& getSolidOrthoMap(void) { return m_solidOrthoMap; };
// //把实现OpenGL ortho映射管理根据类的对象返回
// 	CZRectf &solidOrthoMapRect(void) { return m_solidOrthoMapRect; };
// //返回映射矩形
// 	float solidZOrthoWin2gl(int z) { return m_solidOrthoMap.win2gl(CPoint(0, z)).y+cube().midZ()-float(m_pos.y); }
// 	int solidZOrthoGl2win(float z) { return int(m_solidOrthoMap.gl2win(point2(0.0f, z-cube().midZ()+float(m_pos.y))).y); }
// 	//实现坐标与窗口的坐标差的转换
// 	//angle for rotate solid at Z axis in ortho mode
// 	float solidOrthoZangle;
// 
// 	//
// 	//event handler to m_pSolid
// 	//
// 	virtual int onNotify(CSolidMsgID p_msgID, const void* p_buff=NULL);
// //用来处理当消息说明是实体改变的情况,这时,首先调用虚拟机的实体改变的函数
// 	//attributes
// 	CVSolid& solid(void) { assert(m_pSolid!=NULL); return *m_pSolid;};
// //返回管理器指向的实体
// 	const CSTLCube& cube(void) { return solid().cube(); };
// //作用:返回被指向的实体的包围立方体.
// 
// protected:
// 	//
// 	//data members
// 	//
// 	CVSolid *m_pSolid;
// 
// 	//
// 	//solid view attributes;
// 	//
// 	CPoint3d m_pos;//双精度型的三维点
// 	double	m_fovy,
// 			m_dim;
// 	COrthoMap m_solidOrthoMap;//实现OpenGL ortho映射管理根据类
// 	CZRectf m_solidOrthoMapRect;//浮点矩形/实际上是OPenGL的剪切矩形
// 	CProjectionMode  m_projectionMode;//表示映射方式的共用体
// //enum CProjectionMode {mPerspective, mOrtho};
// 	bool m_bDrawAxises;//是否要绘制坐标轴
// 	GLenum m_glPolygonMode;//GLenum是OpenGL中,用于标志绘图方式的变量类型
// };
// 
// /*
// class CVHRPVM
// {
// public:
// 	virtual void glDrawCurrentLayer(void);
// 
// 	float getCutZ(void) {return m_cutZ;};
// 	bool bSliceOK(void) {return m_cutZ<1e9;};
// 	bool bCutZinSolid(void) {return in(m_cutZ, cube().min.z, cube().max.z);};
// 
// 	const CZSlice getSlice(void) const { return m_slice; };
// 
// 		//cut z
// 	void cut(float p_z);
// 	bool bZinSolid(float p_z) { return in(p_z, cube().min.z, cube().max.z); };
// 
// 	CFISlice m_FISlice;
// 	CZGrid	m_drawGrid; //used to draw LayersView, see drawALayer
// 	float	m_cutZ;
// */
// 
// //
// //exception for CVHrpVM
// //
// /* replaced by const char * now
// enum CHrpErrCode {hecUndefined};
// class EHrpVM
// {
// public:
// 	//constructor/destructor
// 	EHrpVM(const char* p_msg, CHrpErrCode p_errCode=hecUndefined)
// 		: m_errCode(p_errCode) { strncpy(m_msg, p_msg, sizeof m_msg); };
// 	EHrpVM(CHrpErrCode p_errCode) : m_errCode(p_errCode) {m_msg[0]='\0';};
// 	
// 	//attribs
// 	CHrpErrCode errCode(void) {return m_errCode;};
// 	const char* msg(void) {return m_msg;};
// protected:
// 	char m_msg[128];
// 	CHrpErrCode m_errCode;	
// };
// */
// 
// //
// //HRP Virtual Machine
// //
// CVSolid& getSolid(void);
// 
// class CVHrpVM //_safe_class
// {
// public:
// 	//constructor/destructor
// 	CVHrpVM(void) : m_msgOut(fakeMsgOut) {};
// 	virtual ~CVHrpVM(void) {};
// 	
// 	//init
// 	void attach(CVMsgOutput &p_msgOut) {m_msgOut=p_msgOut;};//两个消息相等，也就是位拷贝
// 
// 	//
// 	//virtual interface
// 	//
// 
// 	//VM starting and ending
// 	virtual void init(void) {};
// 	virtual void close(void) {};
// 
// 	//basic actions of VM
// 	virtual void doManufacture(void) {};
// 	virtual bool doSetting(void) { return false; }; 
// 		//return true refer to setting has really been changed
// 	virtual void doCheck(void) {};
// 
// 	//cut actions
// 	virtual void cut(float p_cutZ)=0;
// 		//Note: every implementation must call 	getSolidManager().onNotify(smiSliceChanged)
// 		//		at the end
// 	virtual float getCutZ(void)=0;
// 	//virtual bool bCutInSolid(void)=0;
// 	virtual bool bZinSolid(float p_z) { return in(p_z, cube().min.z, cube().max.z); };
// 
// 	//additional actions
// 	virtual void gDump(ostream &p_os) const {};	//dump G code to p_os
// 
// 	//layer view methods
// 	virtual CZRectf getLayerRect(void)=0;
// 	virtual bool bLayerOK(void)=0;
// 	//current layer view
// 	virtual void glDrawCurrentLayer(void)=0;
// 	//layers view
// 	virtual void glSetupLayersView(double p_aspect=1)=0;
// 	virtual void glDrawALayer(void)=0;
// 
// 	//attributes
// 	virtual const char* getMachineName(void)=0;
// 	virtual CRPMType getRPMType(void)=0;
// 
// 	//event handlers
// 	//3rd progemer's Note: while override the following event handlers, you should run the
// 	//						handler of base class firstly or finally!
// 	virtual void onSystemStart(void) {;};
// 	virtual void onASolidOpened(void) {;};
// 		//when a solid if opened by the app, this event will be fired.
// 		//VM can use this event to write hint to msgout for user
// 	virtual void onSolidDimChanged(void) {;};
// 
// 	//dyna menu support
// 	virtual const char* onGetMenuStr(int p_id) { return NULL; };
// 	virtual void onClickMenu(int p_id) {;};
// 	virtual bool onUpdateMenu(int p_id) { return true; };
// 
// protected:
// 	CVMsgOutput &m_msgOut;
// 
// 	//util funcs
// 	CVSolid& solid(void) { return getSolid(); };
// 	const CSTLCube& cube() { return solid().cube(); };//typedef CCube<float> CSTLCube
// };
// 
// //temp
// CVHrpVM& getVM(void);	//now in lomvm.cpp
// 
// 
// //
// //G code stream out utility------------------------------------------------------------------------------
// //
// ostream& gnumber(ostream& os);
// 		
// struct CGDot	//g dot number
// {
// 	float x,y;//一个点
// };
// IOMANIPdeclare(CGDot);
// ostream& num(ostream& os, CGDot dot);
// OMANIP(CGDot) gdot(float x, float y);
// OMANIP(CGDot) gdot(CPoint2f point);
// 
// //
// //temp usage
// //
// CVSolid* getNewSolid(void);

#endif