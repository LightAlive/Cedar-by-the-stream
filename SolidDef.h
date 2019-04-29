#ifndef __SOLID_DEF_H__
#define __SOLID_DEF_H__

#include <assert.h>
#include "stdafx.h"

/*
�ļ���TEACDef.h
��;��HRPʵ����ػ�����ԭ�ζ���ͷ�ļ�
���ߣ����(teac)
���ڣ�1999.9~
��ʷ��	1999.9~		�����棻
		2000.11~	���ӿڽ�һ����ǿ�����ԣ������ڸ���RP����
��Ҫ��������CZRing����CZSlice��Ƭ��CSolidʵ�壬CGrid����
			Solid Manager ʵ���������HRP Virtual Machine �����
			�Ȼ���HRP��ض�����
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
// CZRing ��
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
// 	//���ص���������Ӧ�ĵ�,�������ɻ���
// 	const CSPoint2 &dot(int p_id) const { return m_data[p_id%m_size]; };
// 
// 	//ring creating method
// 	void addPoint(const CSPoint2 &p_newPoint);//���ӵ�,ͬʱ,������(����)����
// 	void combine(const CZRing &p_ring1, bool p_bSameOrder);
// 	//��һ����׷�ӵ���һ������,��ͬ���뷴������
// 	void over(bool p_bRinged);//�ѻ��Ӻ�֮��,�ڴ�����β����
// 		//when all addPoint is done call this method with parameter true to check 
// 		//	whether the head/tail point be line
// 		//	false to indicate to be hole tracking
// 
// 	void addArc(CPoint2f p_centerPos, float p_radious, float p_radStart=0, float p_radEnd=float(2*PI));
// 	//��Բ����ɢ�ɻ�
// 
// 	//inflate the ring by p_radious with ���ַ���
// 	void inflate(float p_radious);//���ڻ���������,ȥ������,ƫ�ø����ȱ��.
// 
// 	//general method
// 	void glDraw(void) const;
// 	void glDrawSharpDots(void) const;
// 	void gDump(ostream &p_os) const;	//dump G code to p_os//����G����
// 
// 	//operators
// 	void shift(CSPoint2 p_shift);//ƫ�ƻ�
// 
// 	//performance test functions
// 	__test(void dumpRemoveDots(void);)
// 
// protected:
// 	//data member
// 	enum CRingState {csAdding, csNotRinged, csRinged} m_state;
// 
// 	//util func
// 	bool beAllLine(int p1, int p2, int p3); //�ǲ��ǹ���
// };
// 
// 
// CZSlice ��Ƭ
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
// 	void inflate(float p_radious);//����ÿһ������ƫ�ú���©.
// 
// 	//operators
// 	void shift(CSPoint2 p_shift);//����Ƭ��ÿһ������ƽ��.
// 	void combine(const CZSlice& p_additionalSlice);//ֻ��β������,������bool����
// 	void addFrame(const CZRectf& p_rect, CS2dNumber p_round, bool p_bArced=false);
// ���⻷����ʽ���ӿ��,ע�����,��һ��������ʾ��ܵľ���,�Ķ���������ʾ��ܵĵ���(��Բ)�뾶,
// ������������ʾ����ǲ���Ҫ��Բ,�����TRUE�Ļ�,���ǵ�Բ.ע��,������������Զ������Ӵ�������Ŀ
// ���Ǹ�������ֵ,�û�Ҫ�Լ�����.
// 		//Note: addFrame will never increase m_nFrameRingsCount automaticly.
// 		//		User should use setFrameCount manully.
// 	void addCircle(CPoint2f p_centerPos, float p_radious);
// 	//ֻ׷��Բ,��Բ��ɢ�ɻ�����ʽ,�����й���Ĵ���.
// 	void setFrameCount(int p_nFrameRingsCount) //�����������(���Ըı�Frame�ĸ���)
// 		{ assert(p_nFrameRingsCount>=0); m_nFrameRingsCount=p_nFrameRingsCount; };
// 	int getFrameCount(void) { return m_nFrameRingsCount; };//����Frame�ĸ���
// 	int getTrueRingsCount(void) const { return Max(0, size()-m_nFrameRingsCount); };
// 	//���������Ļ��ĸ���.ע��,Frame�����������ָһ��һ���Ŀ��,��һ�����ε���ʽ�ӽ�����.��������
// 	//һ����,���Ǳ�ʾ�߽�,����б�Ҫ�����뻷���ֿ���.
// protected:
// 	int m_nFrameRingsCount;	//��ʾ�����ܵĸ���.
// 		//some HRP VM add frames as rings on the end of cut slice . This data member 
// 		//refer to the count.
// };
// 
// 
// CVSolid ����ʵ��
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
// 	virtual const int getFaceCount(void) const //��֤�ǲ�������ȡ������Ϣ��.
// 		{ assert(!(getCapabilityWord()&canGetFace)); return 0; };
// 	virtual const char* getSimpInfo(void) const {return "";};//��õļ򵥵���Ϣ.
// 		//simp info(less than 20chars) used to be displayed on the panel of status bar
// 
// 	//file operation
// 	virtual void init(const char *fn)=0;	//read file fn
// 	virtual const char* getSupportedReadFileFilters(void)=0;
// 	virtual const char* getSupportedWriteFileFilters(void)//д���ļ������ǿո�
// 				{ assert(!(getCapabilityWord()&canSave)); return ""; };
// 	virtual void save(const char* /*fn*/, int /*filterIndex*/=-1) 
// 				{ assert(!(getCapabilityWord()&canSave));};//��֤������֮��û�п��Դ��̵�
// 	virtual int getFileType(void)	//file type is also file save filterIndex(zero based)
// 				{ assert(!(getCapabilityWord()&canSave)); return 0; };//����ļ�����
// 
// 	//methods
// 	virtual void glDraw(void) const { assert(!(getCapabilityWord()&canDraw));};
// 	virtual void cutSlice(float p_z, CZSlice &p_slice) /*const?*/ = 0;//��Ƭ
// 	virtual void transform(CMatrix4f p_transformMatrix)	{ assert(!(getCapabilityWord()&canTransform));};
// 	//�ı���״
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
// 	CVMsgOutput *m_pMsgOut;//ר������ַ�������.
// 	CZVProfiler *m_pProfiler;//��¼��Ϣ��ʱ����Լ�ʱ��Ƭ����Ϣ,��������ļ��ķ�ʽ�������.
// 	CZSolidManager *m_pManager;//��ʵ����б任�������.
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
	CRingLineDealType ringLineDealing;//����ˮƽ�ߣ�������λ��ɨ������
							//clDefault: scan half of ring lines that combine to scan lines
							//clGo: scan all scan lines
							//clSkip: do not scan ring lines that combine to scan line
	int nIgnoredFrame;		//ignore the last n(IgnoredFrame) ring(frame) of the slice
	float intver;			//step of basic scan line
	CZRectf rect;			//rect of grid region, alway the same as xy plane of solid cube
	
	
		//X Multi-Scan
	int multiScanPower;		//2^n x multiscan lines  �ǲ�����û����ɨ����֮���һ�����ɨ���ߣ�
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
//grid ����
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
// 	float	cutSpeed,//�ٶ�
// 			accuracy,//����
// 			maxAccelerate,//���ٶ�
// 			DDAPeriod;//
// };
// 
// void dealFI(const CZRing& p_inRing, CFIRing* p_pOutRing, CFIOption p_option);
// //Note:
// //currently, dealFI is implemented by LiuFI.cpp
// 
// //
// //Solid Manager ʵ�������
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
// class CZSolidManager//�ʣ�GLenum�Ǹ�ʲô�õģ�����
// {
// public: 
// 	//constructor & destructor
// 	CZSolidManager(CVSolid *p_pSolid=NULL);
// ////����:���˳�ʼ����������,�����ù����������һ��ָ���������ָ��ָ�򴴽��Ĺ�����,�ɼ�,������
// //ֻ�ܴ���һ��.��Document����ĺ���֮��,���Ե��ù������Ĺ����ӿ�.
// 	~CZSolidManager(void);
// 
// 	//operators
// 	void attachSolid(CVSolid *p_pSolid);
// //��ԭ����ָ���ʵ��ɾ��,Ȼ��,��ָ��ʵ��ı���ָ�����Ĳ���.ͬʱ,��ָ���ʵ�����ҲҪָ�����
// //ʵ�������
// 		//IMPORTANT NOTICE: p_pSolid MUST BE A POINTER TO A DYNAMIC ALLOCED SOLID OBJECT
// 		//					AND SHOULD BE DELETED BY SOLID MANAGER ONLY!
// 	void deattachSolid(void) { attachSolid(NULL); };
// //��ʵ��ֿ�,��ʵ���þ��ǰ�ָ���ʵ��ɾ��,Ȼ���ٰ�ָ��ʵ���ָ�븳ΪNULL
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
// 	CProjectionMode getProjectionMode(void) { return m_projectionMode;};//��m_projectionMode����
// 
// //polygon mode for drawing solid
// 	void setSolidPolygonMode(GLenum p_mode)	
// 		{ assert( (p_mode==GL_POINT)||(p_mode==GL_LINE)||(p_mode==GL_FILL)); m_glPolygonMode=p_mode; };
// //����:����ʾģʽ���ó���
// 	GLenum getSolidPolygonMode(void) { return m_glPolygonMode; };
// //����:������ʾģʽ
// 	//restore solid view state
// 	void adjustSolidView(void);
// 
// 	//solid pos
// 	//const CPoint3d &getPos(void) { return m_pos; };
// 	//void setPos(const CPoint3d &p_pos) { m_pos=p_pos; };
// 	CPoint3d &solidPos(void) { return m_pos; };//����λ�õ������
// 
// 	//solid fove in perspective mode
// 	double getFovy(void) { return m_fovy; };//��m_fovy����
// 	void setFovy(double p_fovy) { m_fovy=p_fovy; bound(m_fovy, 0.1, 175.9); };
// //����m_fovy��ֵ,����֤������0.1��175.9֮��
// 	//solid in ortho mode
// 	const COrthoMap& getSolidOrthoMap(void) { return m_solidOrthoMap; };
// //��ʵ��OpenGL orthoӳ����������Ķ��󷵻�
// 	CZRectf &solidOrthoMapRect(void) { return m_solidOrthoMapRect; };
// //����ӳ�����
// 	float solidZOrthoWin2gl(int z) { return m_solidOrthoMap.win2gl(CPoint(0, z)).y+cube().midZ()-float(m_pos.y); }
// 	int solidZOrthoGl2win(float z) { return int(m_solidOrthoMap.gl2win(point2(0.0f, z-cube().midZ()+float(m_pos.y))).y); }
// 	//ʵ�������봰�ڵ�������ת��
// 	//angle for rotate solid at Z axis in ortho mode
// 	float solidOrthoZangle;
// 
// 	//
// 	//event handler to m_pSolid
// 	//
// 	virtual int onNotify(CSolidMsgID p_msgID, const void* p_buff=NULL);
// //����������Ϣ˵����ʵ��ı�����,��ʱ,���ȵ����������ʵ��ı�ĺ���
// 	//attributes
// 	CVSolid& solid(void) { assert(m_pSolid!=NULL); return *m_pSolid;};
// //���ع�����ָ���ʵ��
// 	const CSTLCube& cube(void) { return solid().cube(); };
// //����:���ر�ָ���ʵ��İ�Χ������.
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
// 	CPoint3d m_pos;//˫�����͵���ά��
// 	double	m_fovy,
// 			m_dim;
// 	COrthoMap m_solidOrthoMap;//ʵ��OpenGL orthoӳ����������
// 	CZRectf m_solidOrthoMapRect;//�������/ʵ������OPenGL�ļ��о���
// 	CProjectionMode  m_projectionMode;//��ʾӳ�䷽ʽ�Ĺ�����
// //enum CProjectionMode {mPerspective, mOrtho};
// 	bool m_bDrawAxises;//�Ƿ�Ҫ����������
// 	GLenum m_glPolygonMode;//GLenum��OpenGL��,���ڱ�־��ͼ��ʽ�ı�������
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
// 	void attach(CVMsgOutput &p_msgOut) {m_msgOut=p_msgOut;};//������Ϣ��ȣ�Ҳ����λ����
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
// 	float x,y;//һ����
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