#ifndef __TEAC_DEF_H__
#define __TEAC_DEF_H__
#include "CZArray.h"
#include <iostream>
#include <fstream>
using namespace std;
/*
文件：TEACDef.h
用途：基本类型定义和常用工具例程之头文件
作者：张李超(teac)
日期：1999.9~
概要：
	 1。基本数据类型定义；
	 2。版本控制用宏；
	 3。常量定义；
	 4。异常处理类及相关例程；
	 5。类try--finally结构例程；
	 6。通用例程，包括：
		 1。最大值、最小值例程；
		 2。字符串例程；
		 3。布尔例程；
		 4。数学例程；
		 5。常规例程；
		 6。文件例程；
		 7。流例程；
		 8。字节压缩；
	 7。Win32 API例程；
	 8。MFC例程；
	 9。关键数据类型定义及相关例程：
		1。2维点；
		2。3维点；
		3。4*4矩阵；
		4。立方体；
		5。矩形类型；
	10。关键数据结构定义：
		1。动态数组类；
		2。堆栈类；
	11。高级工具类：
		* 内存/性能分析例程（CSpeedCalculator/CZVProfiler）；
		* 信息输出类（CVMsgOutput/CRichEditHelper）；
		* EscapeSever
		* 命令行解释工具类（CCommandParaser）；
	XX。其他例程；


*/

//
// Definiens & utilities of TEAC
// 1999
//

//
//基本数据类型定义
//
typedef unsigned __int8	Byte;
typedef unsigned __int16	Word;
typedef unsigned __int32	DWord;


//
//版本控制用宏
//test/debug version control------------------------------------------------------------------
//

//test:用于内部测试的代码
//#define __TEST__
#ifdef __TEST__
#define __test(a) a
#define __untest(a)
#define __testcase(a, b) a
#else
#define __test(a)
#define __untest(a) a
#define __testcase(a, b) b
#endif

//libAssert:用于自己例程库的ASSERT
//lib debug version control
#ifdef __TEST__
#define __LIB_DEBUG__
#endif //__TEST__

#ifdef __LIB_DEBUG__
#define libAssert(a) assert(a)
#else
#define libAssert(a)
#endif

//
//class 测试宏
//
#ifdef _DEBUG
#define _USE_SAFE_BASE
#endif

#ifdef _USE_SAFE_BASE
class CZSafeBase
{
public:
	CZSafeBase(void);
	virtual ~CZSafeBase(void);
};
#define _safe_class : public CZSafeBase
#else
#define _safe_class
#endif
//
//常量定义
//const---------------------------------------------------------------------------------------
//
const double PI = 3.14159265358979323846;


// 
// 异常处理类
// exception-----------------------------------------------------------------------------------
// 
// class CZException
// {
// 	const char* msg;
// public:
// 	CZException(const char* p_msg) {msg=p_msg;};
// 	const char* getMsg(void) const {return msg;};
// };
// 
// template <const char *MSG, class T=CZException>
// class CZCustomException : public T
// {
// public:
// 	CZCustomException(void) : CZException(MSG) {};
// };
// 
// const char
// csFileError[]="flie error",
// csMemoryError[]="memory error";
// 
// class CZFileException : public CZException
// {
// public:
// 	CZFileException(void) : CZException(csFileError) {};
// };
// 
// class CZMemoryException : public CZException
// {
// public:
// 	CZMemoryException(void) : CZException("memory error") {};
// };
// 
// exception check utility
// inline void check(ifstream stream) 
// { 
// 	if (!stream) throw CZFileException(); 
// }
// 
// inline void checkMem(void *p)
// {
// 	if (p==NULL) throw CZMemoryException();
// }


//
//类try--finally结构例程
//try--finally util---------------------------------------------------------------------------
//利用局部对象在退出块时自动运行析构函数的方法来实现类try--finally结构例程
//

// 自动切换变量值：
// template<class Type>
// class CZAutoReverse
// {
// public:
// 	CZAutoReverse(Type *p_pState, Type p_processingState, Type p_steadyState)
// 		: m_pState(p_pState), m_processingState(p_processingState), m_steadyState(p_steadyState)
// 		{ assert(*m_pState==p_steadyState); *m_pState=p_processingState; };
// 	~CZAutoReverse(void) { assert(*m_pState==m_processingState); *m_pState=m_steadyState; };
// protected:
// 	Type *m_pState, m_processingState, m_steadyState;
// };
// 
// 自动释放缓冲区
// template<class Type>
// class CZAutoFree
// {
// public:
// 	CZAutoFree(Type *&p_pointer) : m_pointer(&p_pointer) {};
// 	~CZAutoFree(void) { delete *m_pointer; *m_pointer=NULL; };
// protected:
// 	Type **m_pointer;
// };


//
//最大值、最小值例程
//min max utility-----------------------------------------------------------------------------
//
template <class T>
inline void setMinMax(T x, T &min, T &max)
{
	if (x>max) max=x;
	/*else*/ if(x<min) min=x;
	// Note! in some case, programmaer would like to write such code to init min, max value:
	// xMax=0; xMin=1e10; setMinMax(x, xMin, xMax)
	// so we can NOT use "else"
};

template <class T>
inline void setMax(T x, T &max)
{
	if (x>max) max=x;
};

template <class T>
inline void setMin(T x, T &min)
{
	if (x>min) min=x;
};

template <class T>
inline void maxBound(T &x, T boundMax)
{
	if (x>boundMax) x=boundMax;
};

template <class T>
inline void minBound(T &x, T boundMin)
{
	if (x<boundMin) x=boundMin;
};

template <class T>
inline void minMaxBound(T &x, T boundMin, T boundMax)
{
	if (x>boundMax) x=boundMax;
	else if (x<boundMin) x=boundMin;
};

template <class T>
inline void bound(T &x, T boundMin, T boundMax)
{
	minMaxBound(x, boundMin, boundMax);
};

template <class T>
inline const T& Max(const T& x1, const T& x2)
{
	return (x1>x2) ? x1 : x2;
};

template <class T>
inline const T& Min(const T& x1, const T& x2)
{
	return (x1<x2) ? x1 : x2;
};

template <class T>
inline T boundedValue(T x, T boundMin, T boundMax)//如果X在界限之内,返回x,否则,返回它超过的界限
{
	return max(boundMin, min(x, boundMax));
};

template <class T>
inline const T& Max(const T& x1, const T& x2, const T& x3)
{
	return Max( Max(x1, x2), x3);
};

template <class T>
inline const T& Min(const T& x1, const T& x2, const T& x3)
{
	return Min( Min(x1, x2), x3);
};

template <class T>
inline bool in(T a, T min, T max)	//return whether a in [min, max]
{
	return (a>=min) && (a<=max);
};

template <class T>
inline bool strictIn(T a, T min, T max)	//return whether a in (min, max)
{
	return (a>min) && (a<max);
};


//
//字符串例程
//string utility------------------------------------------------------------------------------
//
inline void strCpy( char *strDest, const char *strSource, size_t count )
//this function is safe even if the source string is longer than count. 
//It always add the \0 end tocken
{
	strncpy_s(strDest, count - 1, strSource, count - 1);
	//strncpy(strDest, strSource, count-1);
	strDest[count-1]='\0';
}


//
//布尔例程
//logical utility-----------------------------------------------------------------------------
//
//althrough there is: #define xor ^ [keyword in C++] 
//but ^ is bitwise xor operator
//for safe, we use:
inline bool xor(bool a, bool b)
{
	return (a && !b) || (b && !a);
}

//bit util
template <class T>
inline void reverse(T &x)
{
	x=!x;
};

template <class T>
inline void setBit(T &data, T mask, bool bSet)
{
	if (bSet) data|=mask;
	else data&=(!mask);
};


//
//数学例程
//math utility--------------------------------------------------------------------------------
//double  COMPARE_GATE=1e-5;

inline bool bEqual(float a, float b)
{
	return a==b;
}

inline bool bEqual(float a, float b, double compareGate)
{
	return fabs(a-b)<=compareGate;
}

template <class T>
inline T sqr(T x)
{
	return x*x;
};

inline double zFMod( double x, double y ) 
//return x mod y @ [0, y), compare to fmod
{
	double r=fmod(x,y);
	if (r<0) r+=y;
	return r;
}

inline int iMod( int x, int y ) //返回总是大于0的余数
//return x mod y @ [0, y), compare to operator %
{
	int r=x%y;
	if (r<0) r+=y;
	return r;
}

inline void round(double &x, double y)
//same as zFMod
{
	x=zFMod(x,y);
}

//Note:
//We can NOT use the template version of rad2deg and so on, since the parameter would
//look like integer and be caculated as integer which would bring the wrong value!
/*
template<class Number>
inline Number rad2deg(Number r)	{return Number(r*Number(360)/(2*PI));};

template<class Number>
inline Number deg2rad(Number d)	{return Number(2*PI*d/Number(360));};
*/
inline double rad2deg(double r)	{return double(r*double(360)/(2*PI));};

inline double deg2rad(double d)	{return double(2*PI*d/double(360));};

//PAY ATTENTION TO THE DIFFERENCE BETWEEN truncLarge and truncSmall!
inline int truncLarge(float f)	//return the smallest int which is larger than f
{
	int i=int(f);
	if ( (f>0) && (f!=i) ) return i+1;
	else return i;
}

inline int truncSmall(float f)	//return the largest int which is smaller than f
{
	int i=int(f);
	if ( (f<0) && (f!=i) ) return i-1;
	else return i;
}

int rnd(int range);


//
//常规例程
//general utility-----------------------------------------------------------------------------
//
template <class T>
inline void swapData(T &a, T &b)
{
	T temp=a;
	a=b;
	b=temp;
};

template <class T>
void safeDelete(T *&pointer)
{
	if (pointer)
	{
		delete pointer;
		pointer=NULL;
	}
};

//
//
//file utility--------------------------------------------------------------------------------
//
inline bool fileExist(const char* f)
{
	ifstream fs(f, ios::_Nocreate);
	return !fs;
}

bool fileHasExt(const char* f);
const char* getFileExt(const char* f);	//return NULL if no ext, w/0 dot
void addFileExt(char* f, const char* ext); //note: ext can have or not have '.'
bool getFilePath(const char* fileName, char* pathBuffer, int pathBufferLen);
	//return false for failure


//
//流例程
//stream utility------------------------------------------------------------------------------
//

//general type access for binary file functions
template <class T>
inline void binRead(istream &i, T &a) 
{
	i.read( (char*)(&a), sizeof a );
};

template <class T>
inline void binWrite(ostream &o, T a) 
{
	o.write( (char*)(&a), sizeof a );
};

//formating stream
ostream& science(ostream& os);
		
// struct CNumberFormat
// {
// 	int a,b;	//a/b refer to the number of digits before/after dot. (a.b)
// };
// IOMANIPdeclare(CNumberFormat);
// ostream& num(ostream& os, CNumberFormat f);
// OMANIP(CNumberFormat) num(int a, int b);


//
//字节压缩
// byte compress util-------------------------------------------------------------------------
//
//Note: int byte compress is only valid on intel architecture(small number)
/*	byteCompress/Decompress is used to (de)compress an int array in which every element is postive 
	and fairly small than 256^n (n=1,2,3), so the higher bytes(in intel small number architecture)
	are all zero, which can be removed to save disk space
*/
int getBytesCount(int p_int);	//return the min count of lower non-zero bytes of p_int
void byteComprass(int *p_base, int p_count, int p_srcElementSize, int p_destElementSize);
void byteDecomprass(void *p_base, int p_count, int p_srcElementSize, int p_destElementSize);

//
//Win32 API例程
//Win32 UTIL----------------------------------------------------------------------------------
//
// void getTempFileName(char* p_buff);
// 
// inline void enableDlgItem(HWND hDlg, int nIDDlgItem, BOOL b)
// { ::EnableWindow(::GetDlgItem(hDlg, nIDDlgItem), b); };
// 
// void setWinAlpha(HWND p_hWnd, BYTE p_alpha);
// void beep(void); 
// 
// bool isNT(void);
// bool isW2k(void);
// bool is9x(void);
// 
// inline int msgBox(LPCTSTR lpText, UINT uType=MB_OK | MB_ICONINFORMATION, LPCTSTR lpCaption="注意", HWND hWnd=NULL)
// {
// 	return MessageBox(hWnd, lpText, lpCaption, uType);
// }
// 
// inline bool bKeyDown(int p_vkKey)
// {
// 	return ::GetKeyState(p_vkKey)<0;
// }

//
//profile util
//
// #define __exchange(profile, variable) profile.exchange(#variable, variable)
// 
// enum CZSmartProfileStyle {spRead, spWrite, spAll};
// class CZSmartProfile
// {
// public:
// 	CZSmartProfile(LPCTSTR p_appName, LPCTSTR p_section, CZSmartProfileStyle p_style);
// 
// 	void exchange(LPCSTR p_entry, int &p_data);
// 	void exchange(LPCSTR p_entry, bool &p_data);
// 	void exchange(LPCSTR p_entry, float &p_data);
// 
// 	void getString(LPCSTR p_entry, LPSTR p_data, int p_size, LPCSTR p_defValue)
// 		{ GetPrivateProfileString(m_section, p_entry, p_defValue, p_data, p_size, m_appName); };
// 	void writeString(LPCSTR p_entry, LPCSTR p_data)
// 		{ WritePrivateProfileString(m_section, p_entry, p_data, m_appName); };
// 	int getInt(LPCSTR p_entry, int p_defValue)
// 		{ return GetPrivateProfileInt(m_section, p_entry, p_defValue, m_appName); };
// 	void writeInt(LPCSTR p_entry, int p_value)
// 		{ char s[128]; sprintf(s, "%i", p_value); WritePrivateProfileString(m_section, p_entry, s, m_appName); };
// protected:
// 	CZSmartProfileStyle m_style;
// 	char m_section[128];
// 	char m_appName[128];
// };
// 
// //
// //open/save file dlg
// //
// const char* getOpenFile(
// 	const char* p_filter=NULL, 
// 	const char* p_defExt=NULL, 
// 	const char* p_title=NULL,
// 	//char* p_initDir=NULL, 
// 	char* p_customFilterBuf=NULL, DWORD p_customFilterBufLen=0);
// 
// const char* getSaveFile(
// 	const char* p_filter=NULL, 
// 	const char* p_defExt=NULL, 
// 	const char* p_title=NULL,
// 	//char* p_initDir=NULL, 
// 	char* p_customFilterBuf=NULL, DWORD p_customFilterBufLen=0);

//
//MFC例程
//MFC util------------------------------------------------------------------------------------
//
#ifdef __AFXWIN_H__	//make sure mfc is used by the application

void DDX_CheckB(CDataExchange* pDX, int nIDC, bool& value);

void UpdateAllViewsNow(CDocument *p_doc);

CSize getClientSize(CWnd *w);

inline int length(const SIZE &s) {return (int)sqrt(sqr(s.cx)+sqr(s.cy));}

//
//smart file save dialog
//
BOOL DoPromptSaveFileName(CString& fileName, INT &nFilterIndex, UINT nIDSTitle, DWORD lFlags, LPCTSTR lpstrFilter);

class CZFileSaveDlg : public CFileDialog
{
	DECLARE_DYNAMIC(CZFileSaveDlg)

public:
	CZFileSaveDlg(BOOL bOpenFileDialog, // TRUE for FileOpen, FALSE for FileSaveAs
		LPCTSTR lpszDefExt = NULL,
		LPCTSTR lpszFileName = NULL,
		DWORD dwFlags = OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		LPCTSTR lpszFilter = NULL,
		CWnd* pParentWnd = NULL);

protected:
	//overrides
	virtual void OnTypeChange(void);
	virtual BOOL OnFileNameOK(void);

	bool checkFileName(void);

protected:
	//{{AFX_MSG(CZFileSaveDlg)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#endif __AFXWIN_H__ //make sure mfc is used by the application


//
//
//2维点、3维点类型定义及相关例程
//
//
/*
命名原则：
	CPoint<2|3><i|f|d>
	与OpenGL中点定义方式相同，2|3代表2/3维，i|f|d代表integer/float/double
*/

//
//CPoint3-------------------------------------------------------------------------------------
//
template<class Number>
struct CPoint3
{
	Number x,y,z;
};

typedef CPoint3<double> CPoint3d;
typedef CPoint3<float> CPoint3f;
typedef CPoint3<int> CPoint3i;

const CPoint3d zeroPoint3d={0,0,0};
const CPoint3f zeroPoint3f={0,0,0};
const CPoint3i zeroPoint3i={0,0,0};

template<class Number>
//val is a  to get the id%3 element of p_point.
inline Number val(const CPoint3<Number> &p_point, int id)
{
	return ((Number*)(&p_point))[id%3];
}

template<class Number>
inline CPoint3<Number> point3(Number x, Number y, Number z)
{
	CPoint3<Number> p={x, y, z};
	return p;
}

//operator
template<class Number>
inline CPoint3<Number> operator + (CPoint3<Number> p1, CPoint3<Number> p2)
{
	return point3(p1.x+p2.x, p1.y+p2.y, p1.z+p2.z);
}

template<class Number>
inline CPoint3<Number> operator - (CPoint3<Number> p)
{
	return point3(-p.x, -p.y, -p.z);
}

template<class Number>
inline CPoint3<Number> operator - (CPoint3<Number> p1, CPoint3<Number> p2)
{
	return p1+(-p2);
}

template<class Number>
inline CPoint3<Number> operator -=(CPoint3<Number> &p1, CPoint3<Number> p2)
{
	return (p1 = p1+(-p2));
}

template<class Number>
inline CPoint3<Number> operator * (Number v, CPoint3<Number> p)
{
	return point3(v*p.x, v*p.y, v*p.z);
}

template<class Number>
inline CPoint3<Number> operator / (CPoint3<Number> p, Number v)
{
	return point3(p.x/v, p.y/v, p.z/v);
}
// cjs-add [12/4/2002]
/*
template<class Number>
inline bool operator ==(CPoint3<Number> &p1, CPoint3<Number> p2)
{
	return p1.x==p2.x&&p1.y==p2.y&&p1.z==p2.z;
}
template<class Number>
inline bool operator !=(CPoint3<Number> &p1, CPoint3<Number> p2)
{
	return 
}
*/
// cjs-add
template<class Number>
inline ostream& operator << (ostream& os, CPoint3<Number> p)
{
	return os <<'(' <<p.x <<", " <<p.y <<", " <<p.z <<')' ;
}

template<class Number>
inline istream& operator >> (istream& is, CPoint3<Number> &p)
	//only for ascii file!
{
	return is >>p.x >>p.y >>p.z ;
}

//dist is distance for short
template<class Number>
Number dist(CPoint3<Number> p)
//距离
{
	return (Number)sqrt(sqr(p.x)+sqr(p.y)+sqr(p.z));
}

template<class Number>
inline Number dist(CPoint3<Number> p1, CPoint3<Number> p2)
{
	return dist(p1-p2);
}

//  char* const csDegradeTriangleNormalException="try to calc normal of a degrade triangle";
// class CZDegradeTrianglNormalException : public CZException
// {
// public:
// 	CZDegradeTrianglNormalException(void) : CZException(csDegradeTriangleNormalException) {};
// };

template<class Number>
void normalize(CPoint3<Number> &p) 
//归一化
{
	Number d=dist(p);
	//if (d==(Number)0) throw CZDegradeTrianglNormalException();

	p.x/=d;
	p.y/=d;
	p.z/=d;
}

template<class Number>
bool safeNormalize(CPoint3<Number> &p) //return true for success, do NOT throw exception
//归一化之安全版本
{
	Number d=dist(p);
	if (d==(Number)0) return false;
	p.x/=d;
	p.y/=d;
	p.z/=d;
	return true;
}

template<class Number>
void crossProd(CPoint3<Number> v1, CPoint3<Number> v2, CPoint3<Number> &out)
//叉乘
{
	out.x=v1.y*v2.z-v1.z*v2.y;
	out.y=v1.z*v2.x-v1.x*v2.z;
	out.z=v1.x*v2.y-v1.y*v2.x;
}

template<class Number>
inline void normCrossProd(CPoint3<Number> v1, CPoint3<Number> v2, CPoint3<Number> &out)
//归一化叉乘
{
	crossProd(v1, v2, out);
	normalize(out);
}

template<class Number>
inline Number dotProd(CPoint3<Number> v1, CPoint3<Number> v2)
//点乘
{
	return v1.x*v2.x + v1.y*v2.y + v1.z*v2.z;
}

//method
template<class Number>
inline void normal(CPoint3<Number> p1, CPoint3<Number> p2, CPoint3<Number> p3, CPoint3<Number> &out)
//求p1,p2,p3平面之归一化法矢
{
	normCrossProd(p1-p2, p2-p3, out);
}

template<class Number>
inline bool safeNormal
	(CPoint3<Number> p1, CPoint3<Number> p2, CPoint3<Number> p3, CPoint3<Number> &out)
	//return false for degrade triangle
//求p1,p2,p3平面之归一化法矢之安全版本
{
	crossProd(p1-p2, p2-p3, out);
	return safeNormalize(out);
}
// 
// //GL method
// inline void glNormal(CPoint3d p)
// {
// 	glNormal3dv((double*)&p);
// };
// 
// inline void glNormal(CPoint3f p)
// {
// 	glNormal3fv((float*)&p);
// };
// 
// inline void glNormal(CPoint3i p)
// {
// 	glNormal3iv((int*)&p);
// };
// 
// inline void glVertex(CPoint3d p)
// {
// 	glVertex3dv((double*)&p);
// };
// 
// inline void glVertex(CPoint3f p)
// {
// 	glVertex3fv((float*)&p);
// };
// 
// inline void glVertex(CPoint3i p)
// {
// 	glVertex3iv((int*)&p);
// };

//
//4*4矩阵
//CMatrix4------------------------------------------------------------------------------------
//
typedef float CMatrix4f[4][4];	//NOTE: WE SUPPORT THAT [4][4] IS [Y][X]

inline CPoint3f operator * (CMatrix4f m, CPoint3f p)
{
	float n=m[0][3]*p.x + m[1][3]*p.y + m[2][3]*p.z + m[3][3]*1.0f;
	if (n==0) return point3(1e10f, 1e10f, 1e10f);

	return point3(
		( m[0][0]*p.x + m[1][0]*p.y + m[2][0]*p.z + m[3][0]*1.0f ) / n,
		( m[0][1]*p.x + m[1][1]*p.y + m[2][1]*p.z + m[3][1]*1.0f ) / n,
		( m[0][2]*p.x + m[1][2]*p.y + m[2][2]*p.z + m[3][2]*1.0f ) / n );
}

//inline CMatrix4f& operator = (CMatrix4f &m1, const CMatrix4f &m2)
inline void assign(CMatrix4f dest, const CMatrix4f source)
{
	memcpy(dest, source, sizeof(float) *16 );
	//NOTE: WE CAN NOT USE SIZEOF DEST SINCE CMATRIX4F IS NOT A STRUCT.
	//		WE CAN NOT CALL SIZEOF(ARRAY), OPERATOR(ARRAY, ARRAY), PARAMETER &ARRAY!!!
}

const CMatrix4f cIdentityMatrix4f =
{	{1.0f, 0.0f, 0.0f, 0.0f},
	{0.0f, 1.0f, 0.0f, 0.0f},
	{0.0f, 0.0f, 1.0f, 0.0f},
	{0.0f, 0.0f, 0.0f, 1.0f}
};

inline void identify(CMatrix4f mat)
{
	/*
	for(int i=0; i<4; i++)
		for (int j=0; j<4; j++)
			mat[i][j] = (i==j) ? 1.0f : 0;
	*/
	assign(mat, cIdentityMatrix4f);
}


// cjs_add [10/31/2002]
typedef double CMatrix4d[4][4];	//NOTE: WE SUPPORT THAT [4][4] IS [Y][X]

inline CPoint3d operator * (CMatrix4d m, CPoint3d p)
{
	double n=m[0][3]*p.x + m[1][3]*p.y + m[2][3]*p.z + m[3][3]*1.0;
	if (n==0) return point3(1e10, 1e10, 1e10);

	return point3(
		( m[0][0]*p.x + m[1][0]*p.y + m[2][0]*p.z + m[3][0]*1.0 ) / n,
		( m[0][1]*p.x + m[1][1]*p.y + m[2][1]*p.z + m[3][1]*1.0 ) / n,
		( m[0][2]*p.x + m[1][2]*p.y + m[2][2]*p.z + m[3][2]*1.0 ) / n );
}

//inline CMatrix4f& operator = (CMatrix4f &m1, const CMatrix4f &m2)
inline void assign(CMatrix4d dest, const CMatrix4d source)
{
	memcpy(dest, source, sizeof(double) *16 );
	//NOTE: WE CAN NOT USE SIZEOF DEST SINCE CMATRIX4F IS NOT A STRUCT.
	//		WE CAN NOT CALL SIZEOF(ARRAY), OPERATOR(ARRAY, ARRAY), PARAMETER &ARRAY!!!
}

const CMatrix4d cIdentityMatrix4d =
{	{1.0, 0.0, 0.0, 0.0},
	{0.0, 1.0, 0.0, 0.0},
	{0.0, 0.0, 1.0, 0.0},
	{0.0, 0.0, 0.0, 1.0}
};

inline void identify(CMatrix4d mat)
{
	assign(mat, cIdentityMatrix4d);
}

template <class Number>
void definePlane(CPoint3<Number> p1, CPoint3<Number> p2, CPoint3<Number>p3, CPoint3<Number> &out, Number &t)
//求过原点、垂直于平面的直线与平面交点
{
	//直线方程 Q(t)=D+tE=t(p2p1Xp2p3) :过原点，D=0
	//平面方程 P(u,w)=A+uB+wC=p1+u(p2-p1)+w(p3-p2)
	//两个参数方程相等，t(p2p1Xp2p3)=A+uB+wC=p1+u(p2-p1)+w(p3-p2)
	//为求出t，则用向量B、C的法向量BXC乘在等式两方来消去w、u项
	//t=((BXC)A-(BXC)D)/(BXC)E=(BXC)A/(BXC)E

	//E 是p1-p2-p3组成平面的法线，即normal
	CPoint3<Number> point;
	CPoint3<Number> normal;
	crossProd(p3-p2, p1-p2, normal);   //normal=E
	crossProd(p2-p1, p3-p2, point);    //point=BXC 
	t=dotProd(point, p1)/dotProd(point, normal);
// 	TRACE("t=%f\n", (double)t);
// 	TRACE("normal (%f, %f, %f)\n", normal.x, normal.y, normal.z);
    if(t!=0)
		out=t*normal;
	else out=normal;
}



//从form到to 的旋转矩阵
template <class Number>
void rotateFromTo(CPoint3<Number> form, CPoint3<Number> to, CMatrix4d &out)
{
	normalize(form);
	normalize(to);
// 	TRACE("from=(%f, %f, %f)\n", form.x, form.y, form.z);
// 	TRACE("to=(%f, %f, %f)\n", to.x, to.y, to.z);

    CMatrix4d formto;
	identify(formto);
	if( (form.x!=to.x)||(form.y!=to.y)||(form.z!=to.z) )
	//如果两向量相等，则返回单位矩阵，否则计算其旋转矩阵，计算时特殊情况form = -to:则绕y轴转180度
	{	
		CPoint3<Number>  normal;
		CPoint3<Number>   temp;
		temp=form+to;
		if( (temp.x==0)&&(temp.y==0)&&(temp.z==0) )
		{
			normal.x=0;
			normal.y=1.0;
			normal.z=0;
		}
		else
			normCrossProd(form, to, normal);
		double dotend=dotProd(form,to);
		double degree=acos(dotend);
		double sin=(double) ::sin(degree);
		double cos=(double) ::cos(degree);
		Number n1,n2,n3;
		n1=normal.x;
		n2=normal.y;
		n3=normal.z;
/////公式：p108  (计算机图形学，华中科技大学出版社，第二版)	

		formto[0][0]=(double)(n1*n1+(1-n1*n1)*cos);
		formto[1][0]=(double)(n1*n2*(1-cos)-n3*sin);
		formto[2][0]=(double)(n1*n3*(1-cos)+n2*sin);

		formto[0][1]=(double)(n1*n2*(1-cos)+n3*sin);
		formto[1][1]=(double)(n2*n2+(1-n2*n2)*cos);
		formto[2][1]=(double)(n2*n3*(1-cos)-n1*sin);

		formto[0][2]=(double)(n1*n3*(1-cos)-n2*sin);
		formto[1][2]=(double)(n2*n3*(1-cos)+n1*sin);
		formto[2][2]=(double)(n3*n3+(1-n3*n3)*cos);
	}
	assign(out, formto);
}
//......................................cjs add ......................................

//
//CPoint2-------------------------------------------------------------------------------------
//
template<class Number>
struct CPoint2
{
	Number x,y;
};

typedef CPoint2<double> CPoint2d;
typedef CPoint2<float> CPoint2f;
typedef CPoint2<int> CPoint2i;







template<class Number>
inline Number val(const CPoint2<Number> &p_point, int id)
{
	return ((Number*)(&p_point))[id%2];
}

template<class Number>
inline CPoint2<Number> point2(Number x, Number y)
{
	CPoint2<Number> p={x, y};
	return p;
}

template<class Number>
inline CPoint2<Number> xyPoint(CPoint3<Number> p)
{
	CPoint2<Number> p2={p.x, p.y};
	return p2;
}

//operator
template<class Number>
inline CPoint2<Number> operator + (CPoint2<Number> p1, CPoint2<Number> p2)
{
	return point2(p1.x+p2.x, p1.y+p2.y);
}

template<class Number>
inline CPoint2<Number> operator - (CPoint2<Number> p)
{
	return point2(-p.x, -p.y);
}

template<class Number>
inline CPoint2<Number> operator - (CPoint2<Number> p1, CPoint2<Number> p2)
{
	return p1+(-p2);
}

template<class Number>
inline CPoint2<Number> operator * (Number v, CPoint2<Number> p)
{
	return point2(v*p.x, v*p.y);
}

template<class Number>
inline CPoint2<Number> operator / (CPoint2<Number> p, Number v)
{
	return point2(p.x/v, p.y/v);
}

template<class Number>
inline ostream& operator << (ostream& os, CPoint2<Number> p)
{
	return os <<'(' <<p.x <<", " <<p.y<<')' ;
}


template<class Number>
inline Number dotProd(CPoint2<Number> p1, CPoint2<Number> p2)
//点乘
{
	return p1.x*p2.x + p1.y*p2.y;
}

/*
template<class Number>
void crossProd(CPoint3<Number> v1, CPoint3<Number> v2, CPoint3<Number> &out)
//叉乘
{
	out.x=v1.y*v2.z-v1.z*v2.y;
	out.y=v1.z*v2.x-v1.x*v2.z;
	out.z=v1.x*v2.y-v1.y*v2.x;
}
*/

//??? Do we should return type Number instead of double?
template<class Number>
inline Number dist(const CPoint2<Number> &p)
//距离
{
	//return (Number)sqrt(sqr(p.x)+sqr(p.y));
	return (Number)hypot(p.x, p.y);
}
/* double version
inline double dist(const CPoint2<Number> &p)
//距离
{
	//return (Number)sqrt(sqr(p.x)+sqr(p.y));
	return hypot(p.x, p.y);
}
*/

template<class Number>
inline double dist(const CPoint2<Number> &p1, const CPoint2<Number> &p2)
{
	return dist(p1-p2);
}

template<class Number>
void normalize(CPoint2<Number> &p)
//归一化
{
	Number d = dist(p);
	//if (d==(Number)0) throw CZDegradeTrianglNormalException();

	p.x /= d;
	p.y /= d;
}

template<class Number>
bool safeNormalize(CPoint2<Number> &p) //return true for success, do NOT throw exception
//归一化之安全版本
{
	Number d = dist(p);
	if (d == (Number)0) return false;
	p.x /= d;
	p.y /= d;
	return true;
}


template<class Number>
double angle_total(const CPoint2<Number> &p1, const CPoint2<Number> &p2, const CPoint2<Number> &p3)

//return angle in (-2PI, 2PI)
{
	double r = atan2(p1.y - p2.y, p1.x - p2.x) - atan2(p3.y - p2.y, p3.x - p2.x);
	//r in (-2PI, 2PI) since atan2 return(-PI, PI]
	// 	if (r > PI) r -= 2 * PI;
	// 	else if (r <= -PI) r += 2 * PI;
	return r;
}
template<class Number>
double angle_total(const CPoint2<Number> &p1, const CPoint2<Number> &p3) //求顺时针方向从1到3的夹角
{
	return angle_total(p1, point2<Number>(0, 0), p3);
}


template<class Number>
double angle(const CPoint2<Number> &p1, const CPoint2<Number> &p2, const CPoint2<Number> &p3) 
//夹角
//return angle in rad
//counter clockwise from p2p1 to p2p3
//return angle in (-PI, PI]
{
	double r= atan2(p1.y-p2.y, p1.x-p2.x) - atan2(p3.y-p2.y, p3.x-p2.x);
				//r in (-2PI, 2PI) since atan2 return(-PI, PI]
	if (r>PI) r-=2*PI;
	else if (r<=-PI) r+=2*PI;
	return r;
}

template<class Number>
double angle(const CPoint2<Number> &p1, const CPoint2<Number> &p3) 
{
	return angle(p1, point2<Number>(0,0), p3);
}

template<class Number>
CPoint2<Number> getArcCenter(const CPoint2<Number> &p1, const CPoint2<Number> &p2, const CPoint2<Number> &p3)	
	//return center of arc(p1, p2, p3)
{
	/*
	(x1-x2)x+(y1-y2)y=( x1^2-x2^2 + y1^2-y2^2 ) / 2
	(x2-x3)x+(y2-y3)y=( x2^2-x3^2 + y2^2-y3^2 ) / 2

	as:	ax+by=c
		dx+ey=f
	*/
	double
		a=p1.x-p2.x,
		b=p1.y-p2.y,
		c=( sqr(p1.x)-sqr(p2.x) + sqr(p1.y)-sqr(p2.y) ) / 2,

		d=p2.x-p3.x,
		e=p2.y-p3.y,
		f=( sqr(p2.x)-sqr(p3.x) + sqr(p2.y)-sqr(p3.y) ) / 2,

		denominator = a*e - b*d;

	//assert(denominator!=0);
	if (denominator==0) return point2((Number)1e10, (Number)1e10); //?????

	return point2<Number>( Number((c*e - b*f) / denominator), Number((a*f - c*d) / denominator) );
}

template<class Number>
inline Number triangleArea(const CPoint2<Number> &p1, const CPoint2<Number> &p2, const CPoint2<Number> &p3)
//三角形面积
{
	return (Number)fabs(p1.x*p2.y - p1.x*p3.y + p1.y*p3.x - p1.y*p2.x + p2.x*p3.y - p2.y*p3.x) / 3 ;
}

template<class Number>
inline bool beAllLine(const CPoint2<Number> &p1, const CPoint2<Number> &p2, const CPoint2<Number> &p3)
//三点共线（不考虑三点顺序）
{
	return //fabs((p2.x-p1.x)*(p2.y-p3.y)) == fabs((p2.y-p1.y)*(p2.x-p3.x));
		triangleArea(p1, p2, p3)==0;
	//Note: we use fabs so that line p1-p2-p3 can be out of order such as p2-p1-p3
};

template<class Number>//如果▲的高小于门限,那么就返回1.返回的值表示是否共线:1表示共线
inline bool beAllLine(const CPoint2<Number> &p1, const CPoint2<Number> &p2, const CPoint2<Number> &p3, Number errorGate)
	//error gate refer to the maximize distance of p2 to line p1-p3
//三点共线（不考虑三点顺序）（带门限）
{
	return 2*triangleArea(p1, p2, p3)/dist(p1, p3)<errorGate;
};

template<class Number>
inline bool operator == (const CPoint2<Number> &p_point1, const CPoint2<Number> &p_point2)
{
	return ((p_point1.x - p_point2.x) <= 1e-7 && (p_point1.x - p_point2.x) >= -1e-7) ? 
		((p_point1.y - p_point2.y) <= 1e-7 && (p_point1.y - p_point2.y) >= -1e-7) : (false);
}

//
//gl util
// //
// inline void glVertex(CPoint2d p)//指定顶点
// {
// 	glVertex2dv((double*)&p);
// };
// 
// inline void glVertex(CPoint2f p)
// {
// 	glVertex2fv((float*)&p);
// };
// 
// inline void glVertex(CPoint2i p)
// {
// 	glVertex2iv((int*)&p);
// };

//
//立方体
//CCube---------------------------------------------------------------------------------------
//
template <class Number>
struct CCube
{
public:
	CPoint3<Number> min, max;

	void firstPoint(CPoint3<Number> p) {min=max=p;};
	void nextPoint(CPoint3<Number> p);//加入点

	inline Number w(void) const {return max.x-min.x;};
	inline Number h(void) const {return max.y-min.y;};
	inline Number t(void) const {return max.z-min.z;};//返回长宽高
	inline CPoint3<Number> dim(void) const {return point3(w(), h(), t());};//返回向量
	inline Number maxDim(void) const {return Max(w(), h(), t());};//返回最大值
	inline Number maxLength(void) const {return (Number) sqrt( sqr(w()) + sqr(h()) + sqr(t()) );};
//返回对角线
	inline Number midX(void) const {return (min.x+max.x)/2;};
	inline Number midY(void) const {return (min.y+max.y)/2;};
	inline Number midZ(void) const {return (min.z+max.z)/2;};//返回中点坐标
	inline CPoint3<Number> mid(void) const {return point3(midX(), midY(), midZ());};
//返回中点
	inline void zerolize(void) {min.x=min.y=min.z=max.x=max.y=max.z=(Number)0;};//归原点
};

template <class Number>
void CCube<Number>::nextPoint(CPoint3<Number> p)//可以加好多的点
{
	setMinMax(p.x, min.x, max.x);
	setMinMax(p.y, min.y, max.y);
	setMinMax(p.z, min.z, max.z);
}

typedef CCube<double> CCubed;
typedef CCube<float> CCubef;
typedef CCube<int> CCubei;

template <class Number>
inline ostream& operator << (ostream& os, CCube<Number> c)
{
	return os <<c.min <<'-' <<c.max;
}

//
//矩形
//rect----------------------------------------------------------------------------------------
//
template <class Number>
struct CZRect
{
	CPoint2<Number> min, max;

	void firstPoint(CPoint2<Number> p) {min=max=p;};
	void nextPoint(CPoint2<Number> p);

	inline Number w(void) const {return max.x-min.x;};
	inline Number h(void) const {return max.y-min.y;};
	inline CPoint2<Number> dim(void) const {return point2(w(), h());};
	inline Number maxDim(void) const {return Max(w(), h());};
	inline Number maxLength(void) const {return (Number) sqrt( sqr(w()) + sqr(h()) );};

	inline Number midX(void) const {return (min.x+max.x)/2;};
	inline Number midY(void) const {return (min.y+max.y)/2;};
	inline CPoint2<Number> mid(void) const {return point2(midX(), midY());};

	void inflateRatio(double p_ratio);
	void inflate(Number p_cx, Number p_cy) { min.x-=p_cx; min.y-=p_cy; max.x+=p_cx; max.y+=p_cy; };
	void inflate(CPoint2<Number> p_c) { inflate(p_c.x, p_c.y); };
	inline void inflate(Number p_c) { inflate(p_c, p_c); };

	void shift(CPoint2<Number> s) { min=min+s; max=max+s; };

	void normalize(void);
};

template<class Number>
inline CZRect<Number> rect(Number x0, Number y0, Number x1, Number y1)
{
	CZRect<Number> r={ {x0,y0}, {x1,y1} };
	return r;
}


template <class Number>
void CZRect<Number>::inflateRatio(double p_ratio)
{
	CZRect<Number> r=*this;
	r.inflate(Number(w()*p_ratio/2), Number(h()*p_ratio/2)); 
	if ( (r.w()<=0) || (r.h()<=0) ) return;
	*this=r;
};

template <class Number>
void CZRect<Number>::nextPoint(CPoint2<Number> p)
{
	setMinMax(p.x, min.x, max.x);
	setMinMax(p.y, min.y, max.y);
}

template <class Number>
void CZRect<Number>::normalize(void)
{
	if (min.x>max.x) swap(min.x, max.x);
	if (min.y>max.y) swap(min.y, max.y);
}

typedef CZRect<double> CZRectd;
typedef CZRect<float> CZRectf;
typedef CZRect<int> CZRecti;

void glRect(CZRectf r, bool p_bFilled=false);
void glRect(CZRectd r, bool p_bFilled=false);
void glRect(CZRecti r, bool p_bFilled=false);

template <class Number>
inline ostream& operator << (ostream& os, CZRect<Number> c)
{
	return os <<c.min <<'-' <<c.max;
}

template <class Number>
void cubeXY2rect(const CCube<Number> &c, CZRect<Number> &r)
{
	r.min.x=c.min.x;
	r.min.y=c.min.y;
	r.max.x=c.max.x;
	r.max.y=c.max.y;
}

template <class Number>
void inline pointZoomPoint(CPoint2<Number> &zoomp, CPoint2<Number> orgp, Number ratio)
{
	zoomp=orgp+ratio*(zoomp-orgp);
}

template <class Number>
void inline pointZoomRect(CZRect<Number> &zoomr, CPoint2<Number> orgp, Number ratio)
{
	pointZoomPoint(zoomr.min, orgp, ratio);
	pointZoomPoint(zoomr.max, orgp, ratio);
}


//
//动态数组
//CZArray-------------------------------------------------------------------------------------
//	--simply dyna array, but fast
// //
// enum CZArrayType{atEmpty, atSized, atFill0};
// template <class T, bool isObject=false>
// class CZArray
// {
// public:
// 	//consts
// 	enum CSetSizeAttr{saFlex, saOnlyGrowFlex, saExact};	
// 		//the order is from flex to exact, can NOT be changed!
// 	enum CCloneMethod{cmAuto, cmMemCpy, cmObjCpy};
// 
// public:
// 	CZArray(int p_memSize=0, int p_addSize=0, CZArrayType p_type=atEmpty) 
// 		{ initZero(); init(p_memSize, p_addSize, p_type); };
// //分配内存空间，再就是进行初始化（分三种情况，第一种不管，第二种调用构造函数，第三种还要给他们
// //赋0
// 	CZArray(int p_memSize, CZArrayType p_type) { initZero(); init(p_memSize, 0, p_type); };
// //这种方式使得m_addSize为0
// 	CZArray(const CZArray<T, isObject> &p_source) : m_data(NULL) { initZero(); cloneFrom(p_source); };
// //拷贝构造函数
// 	virtual ~CZArray(void);
// //继承的时候，在使用delete的时候使用的
// 	T& operator[](int i) { assert(i>=0 && i<m_size); return m_data[i]; };
// //返回第i+1个元素
// 	const T& operator[](int i) const { assert(i>=0 && i<m_size); return m_data[i]; };
// //即使是常量，也能使用
// 	//T* operator ()(void) { return m_data; };
// 
// 	void add(const T &p_t);
// //从尾部追加元素，追加完了之后并不返回什么数值
// 	void setAtGrow(int p_i, const T &p_t);
// //设置第i+1个元素的值
// 	T& add(void) { setSize(m_size+1); return top(); };
// //增加，增加的那个元素只是初始化，返回之后，让用户自己进行改变
// 	T& top(void) { assert(!bEmpty()); return m_data[m_size-1]; };
// //返回最尾部的数值
// 	const T& top(void) const { assert(!bEmpty()); return m_data[m_size-1]; };
// //返回最尾部的数值，并且都是常量
// 	void removeTop(int p_removeCount=1);
// //把从最后面起的p_removeCount个数值除去
// 	void removeAt(int p_id, int p_count=1);
// //把从p_id开始，往后数p_count个元素除去
// 	void insertEmptyAt(int p_id, int p_count=1);
// //在p_id之处插入p_count个元素，插入的时候只调用构造函数，其他不管
// 	void insert(int p_id, const T &p_t) { insertEmptyAt(p_id), m_data[p_id]=p_t; };
// //只是在p_id的位置插入元素
// 	void init(int p_memSize=0, int p_addSize=0, CZArrayType p_type=atEmpty);
// //初始化，把原来的记录都清除，在视处理type的不同而决定是否要初始化
// 	void init(int p_memSize, CZArrayType p_type) { init(p_memSize, 0, p_type); };
// //m_size=0的初始化，这时，增加的话一次就是至少100了
// 	void setSize(int p_size, CSetSizeAttr p_sizeAttr=saFlex);
// //主要的事就是如果是实体，就调用构造函数进行初始化的，再就是如果小了的话，就调用析构函数
// 	void setMemSize(int p_memSize, CSetSizeAttr p_sizeAttr=saFlex);
// //setMemSize作用：根据p_memSize的数值来安排空间。p_sizeAttr有三种模式，第一种表示增加空间的时候，
// //多增加一些，减少空间的时候，就不减少。第二种模式表示，增加空间的时候，多增加一些，减少空间的
// //时候，使适合。第三种模式表示，无论增加空间还是减少空间，都是采取最适合的方式。再就是，多增加的
// //时候，如果m_addSize小于0，就表示数目越大，增加的越多，大于0是只增加m_addSize
// 	void setAddSize(int p_addSize) { m_addSize=p_addSize; };
// //设置一次要增加的元素的数目
// 	void free(void) { setSize(0, saExact);};
// //因为是saExact,所以大小刚合适
// 	void pack(void) { setMemSize(m_size, saExact); };
// //使它刚好大小适合
// 	//void sort(void);
// 	void sort(int (__cdecl *p_compareFunc )(const void *elem1, const void *elem2 ));//排序
// 	void out(ostream &p_o, const char* p_spi);//每一个元素都多输出一个字符串
// 	void cloneFrom(const CZArray<T, isObject> &p_source, CCloneMethod p_cloneMethod=cmAuto);
// //把源数组的内容拷贝一分到目的数组，注意，m_memSize不一定相同
// 	void stealDataFrom(CZArray<T, isObject> &p_source);
// //实际上就是把源数组的内容转移到目的数组，原数组初始化	
// 	void export(ostream &p_os);
// //输出，带校验位的
// 	void import(istream &p_is);
// //带校验位的输入
// 	virtual void simpExport(ostream &p_os);
// //不带校验位的输出
// 	virtual void simpImport(istream &p_is);
// //不带校验位的输入
// 	void exportRawData(ostream &p_os);
// //输出T
// 	void importRawData(istream &p_is, int p_size, bool p_bReallocMemory=true);
// //输入T
// 	int size(void) const { return m_size; };
// //返回数组元素数目
// 	int topI(void) const { return m_size-1; };
// //返回最后一个的元素的索引号
// 	T& warp(int i) { return m_data[iMod(i, size())]; };
// //对于warp函数来说，i表示第几个字节数，也就是说，返回某字节对应的元素
// 	T* getData(void) { return m_data; };
// //返回数组首地址
// 	const T* getData(void) const { return m_data; };
// //常量型的数组首地址
// 	bool bEmpty(void) const { return m_size<=0; };//是空的就返回真，否则就返回假
// 
// protected:
// 	int m_size,//安全可用的元素的个数 
// 		m_memSize,//一共开辟的空间的最多包含的元素的个数
// 		m_addSize;//增加的数目
// 	T* m_data;
// 
// 	void inline initZero(void);
// //只是初始化为0
// 	enum{ValidateID=787889};
// };
// 
// template <class T>
// class  CZObjArray : public CZArray<T, true> {};//是实体就是了
// 
// template <class T, bool isObject>
// CZArray<T, isObject>::~CZArray(void) 
// {
// 	setSize(0, saFlex);//大小不变，只不过能使用的看成0
// 	::free(m_data); //释放它占用的空间
// };//最后在完全释放变量的空间
// 
// template <class T, bool isObject>
// void inline CZArray<T, isObject>::initZero(void)
// {
// 	//init member to init value
// 	m_data=NULL;//指向空
// 	m_size=m_memSize=m_addSize=0;//全为0
// };
// 
// template <class T, bool isObject>
// void CZArray<T, isObject>::init(int p_memSize, int p_addSize, CZArrayType p_type)
// //enum CZArrayType{atEmpty, atSized, atFill0};
// {
// 	assert(p_memSize>=0);
// 
// 	m_addSize=p_addSize;//给m_addSize赋值
// 	setSize(0);	//可用的空间数为0
// 	//even for non-atEmpty, this instruct should be called to clear the old object!
// 	setMemSize(p_memSize, saExact);//使用最后一个enum元素，恰好让他们大小相适合
// 	if (p_type!=atEmpty) setSize( p_memSize );
// //不为空，就在初始化的时候都调用构造函数进行初始化，使刚开辟的那些都能使用，之后，
// //p_memSize=m_size(但是，
// 	if (p_type==atFill0) memset(m_data, 0, m_size*sizeof m_data[0]);
// //因为atFill0不属于atEmpty，因此，前面就初始化了，后面的把初始化的部分再用0来填充
// 
// //void *memset( void *dest, int c, size_t count );作用：使用指定的字符充满指定的字符空间的指定的
// //前count个空间。参数含义：第一个参数表示buffer地址第二个参数表示要充满的数值，第三个参数表示要
// //充满的数目
// };
// 
// template <class T, bool isObject>
// void CZArray<T, isObject>::setSize(int p_size, CSetSizeAttr p_sizeAttr)
// //CSetSizeAttr{saFlex, saOnlyGrowFlex, saExact};
// //并不直接对m_memSize进行操作,因此，调用他的时候，也会释放空间，也会调用析构函数，这都是要看要
// //set的元素是否比m_memSize还要大	
// {
// 	assert(p_size>=0);
// 	if (p_size>m_size)//大于m_size,而不是大于m_memSize
// 	{
// 		setMemSize(p_size, p_sizeAttr);
// 		if (isObject)
// 			for (int i=m_size; i<p_size; i++)
// 				::new((void*)(&m_data[i])) T;//把它通过构造函数初始化，这些是能用的
// 		m_size=p_size;//m_size是恰好数组有多么的长
// 	}
// 	else if (p_size<m_size)
// 	{
// 		if (isObject)
// 			for (int i=p_size; i<m_size; i++)
// 				(&m_data[i])->~T();//把多余的析构
// 		m_size=p_size;//还是一样多
// 		setMemSize(p_size, p_sizeAttr);//这不就是重新分配内存空间吗？
// 	}
// };
// 
// template <class T, bool isObject>
// void CZArray<T, isObject>::setMemSize(int p_memSize, CSetSizeAttr p_sizeAttr) 
// //CSetSizeAttr{saFlex, saOnlyGrowFlex, saExact};	
// { 
// 	assert(p_memSize>=m_size);
// 
// 	int newMemSize;
// 	if (p_memSize>m_memSize)//需要增加尺寸的时候
// 	{
// 		if (m_addSize==0) m_addSize=-100;
// 		int addSize= (m_addSize>0) ? m_addSize : max(-m_addSize, p_memSize/8+1);
// //设置addSize,如果m_addSize大于0，不变，如果m_addsize值小于0，addSize的值取addSize与
// //p_memSize/8+1的最大值，addSize最起码多一个元素，随着元素的增多，addSize的数目也增多
// 		assert(addSize>0);
// 		newMemSize = (p_sizeAttr<=saOnlyGrowFlex) 
// //saFlex、saOnlyGrowFlexsh时，取前者，否则，取后者
// 						? ((p_memSize+addSize/2)/addSize+1)*addSize//p_memSize+3*addSize/2  
// 						: p_memSize;
// 	}
// 	else if (p_memSize<m_memSize)//需要减少尺寸的时候
// 		newMemSize = (p_sizeAttr<=saFlex) ? m_memSize : p_memSize;//后两者都必须要恰到好处
// 	else return;
// 
// 	if (m_memSize!=newMemSize)
// 	{
// 		m_memSize=newMemSize;//让他等于，newMemSize是新的要开辟的空间的大小，下一次还等于
// 		m_data=(T*)realloc(m_data, m_memSize*sizeof(T)); //指向新的地址
// 	}
// };
// 
// template <class T, bool isObject>
// void CZArray<T, isObject>::cloneFrom(const CZArray<T, isObject> &p_source, CCloneMethod p_cloneMethod)
// //也就是从原空间原封不动的拷贝到地址空间，相当于调用一次赋值构造函数
// {
// 	if (p_cloneMethod==cmAuto)//自动克隆
// 		p_cloneMethod= isObject ? cmObjCpy : cmMemCpy;
// 	init(p_source.m_memSize, p_source.m_addSize);
// 	setSize(p_source.m_size);//这都是exact型的，开辟好空间
// 
// 	//if (isObject)
// 		switch (p_cloneMethod)
// 		{
// 		case cmMemCpy:
// 			memcpy(m_data, p_source.m_data, p_source.m_size * sizeof T);//内存拷贝
// 			break;
// 		case cmObjCpy:
// 			for(int i=0; i<m_size; i++)
// 				//m_data[i]=p_source[i];
// 				::new(&m_data[i]) T(p_source.m_data[i]);//调用构造函数]
// 			break;
// 		}
// 	//else //for non-object array, always use mem copy
// 	//	memcpy(m_data, p_source.m_data, p_source.m_size * sizeof T);
// };
// 
// template <class T, bool isObject>
// void CZArray<T, isObject>::export(ostream &p_os)
// {
// 	assert(!isObject);
// 	binWrite(p_os, m_size);//先写大小
// 	binWrite(p_os, int(sizeof T));//再写数组的大小
// 	binWrite(p_os, ValidateID-m_size);//在协差值大小
// 	binWrite(p_os, m_addSize);//写入加上的值的大小
// 	p_os.write((char*)m_data, m_size*sizeof(T));//按位来写
// };
// 
// template <class T, bool isObject>
// void CZArray<T, isObject>::import(istream &p_is)
// {
// 	assert(!isObject);
// 	int s1, s2, st;
// 	binRead(p_is, s1);//s1表示有多少个元素
// 	binRead(p_is, st);//st表示每一个数组占用多少空间，这也是校验用的
// 	binRead(p_is, s2);//s2表示剩下多少个元素，这时校验用的
// 	if (s2!=(ValidateID-s1)) throw "CZArray import error: ValidateID not found";
// 	if (st!=sizeof(T)) throw "CZArray import error: sizeof T not equal";
// 	binRead(p_is, m_addSize);//把原数据直接读入m_addSize之中
// 	importRawData(p_is, s1);//默认是要realloc工作范围的
// };
// 
// template <class T, bool isObject>
// void CZArray<T, isObject>::simpExport(ostream &p_os)
// //没有了校验位
// {
// 	assert(!isObject);
// 	binWrite(p_os, m_size);
// 	p_os.write((char*)m_data, m_size*sizeof(T));//按位写
// };
// 
// template <class T, bool isObject>
// void CZArray<T, isObject>::simpImport(istream &p_is)
// //没有了校验位
// {
// 	assert(!isObject);
// 	int size;
// 	binRead(p_is, size);
// 	if (size<0) throw "CZArray import error: size less than zero!";
// 	importRawData(p_is, size);
// };
// 
// template <class T, bool isObject>
// inline void CZArray<T, isObject>::exportRawData(ostream &p_os)
// {
// 	assert(!isObject);
// 	p_os.write((char*)m_data, size()*sizeof(T));//写出来
// };
// 
// template <class T, bool isObject>
// void CZArray<T, isObject>::importRawData(istream &p_is, int p_size, bool p_bReallocMemory)
// {
// 	assert(!isObject);//必须是实体，否则，结果是1，出错
// 	if (p_bReallocMemory)
// 	{
// 		m_size=m_memSize=p_size;
// 		m_data=(T*)realloc(m_data, m_memSize*sizeof(T));
// 	}
// 	else
// 	{
// 		setSize(p_size);//默认是Exact型的
// 	}
// 	p_is.read((char*)m_data, size()*sizeof(T));//强制的转换
// };
// 
// template <class T, bool isObject>
// void CZArray<T, isObject>::stealDataFrom(CZArray<T, isObject> &p_source)
// //从源数组的内容都移植过来，源数组内容就没有了，只有一个空壳子了，注意，目的数组内容也是空的
// {
// 	setSize(0);
// 	::free(m_data);
// 
// 	m_data=p_source.m_data;
// 	m_size=p_source.m_size;
// 	m_memSize=p_source.m_memSize;
// 	//m_addSize=p_source.m_addSize;
// 
// 	p_source.m_data=NULL;
// 	p_source.m_size=p_source.m_memSize=0;
// }
// 
// template <class T, bool isObject>
// void CZArray<T, isObject>::removeAt(int p_id, int p_count)
// {
// 	assert(p_id>=0);//从id往后除去p_count个元素
// 	assert(p_count>0);
// 	assert(p_id+p_count<=m_size);
// 
// 	if (isObject)
// 		for (int i=p_id; i<p_id+p_count; i++)
// 			(&m_data[i])->~T();
// 
// //调用析构函数了，注意，人工的调用并不意味着往后不能使用，因为以后真正撤销的时候，
// //还可能调用一次（对于自动变量也是如此，即使调用了析构函数，函数结束的时候，还调用一次
// 	int moveCount = m_size - (p_id+p_count);
// 	if (moveCount)
// 		memcpy(&m_data[p_id], &m_data[p_id+p_count],moveCount * sizeof(T));//没有构造函数了
// 	m_size -= p_count;//减一下，并且后面的还没有调用析构函数
// };
// 
// template <class T, bool isObject>
// void CZArray<T, isObject>::insertEmptyAt(int p_id, int p_count)
// {
// 	assert(p_id>=0);
// 	assert(p_count>0);
// 	assert(p_id<=m_size);
// 	if (p_id==m_size)
// 	{
// 		setSize(m_size+p_count);
// 	}
// 	else
// 	{
// 		int newSize=m_size+p_count;
// 		setMemSize(newSize, saFlex);//只增不减
// 		memmove(&m_data[p_id+p_count], &m_data[p_id], (m_size-p_id) * sizeof T);
// 		//有没有调用构造函数，反正是最后还有一个强制类型转换么，但是，不安全
// 			//memmove is unsafe for object copy!!!!!  ????
// 		if (isObject)
// 		{
// 			for (int i=p_id; i<p_id+p_count; i++)
// 				::new((void*)(&m_data[i])) T;//又调用构造函数了，不过，因为没有了么
// 		}
// 		m_size = newSize;
// 	}
// };
// 
// template <class T, bool isObject>
// inline void CZArray<T, isObject>::setAtGrow(int p_i, const T &p_t)
// //把第p_i个参数设成p_t
// {
// 	assert(p_i>=0);
// 	if (p_i>=m_size) setSize(p_i+1);
// 	m_data[p_i]=p_t;
// };
// 
// template <class T, bool isObject>
// inline void CZArray<T, isObject>::add(const T &p_t)
// {
// 	setSize(m_size+1);
// 	m_data[m_size-1]=p_t;
// };
// 
// template <class T, bool isObject>
// inline void CZArray<T, isObject>::removeTop(int p_removeCount)
// //从最后面除去指定数量的数组
// {
// 	assert( (p_removeCount>=0) && (p_removeCount<=m_size) );
// 	setSize(m_size-p_removeCount);
// };
// 
// template <class T, bool isObject>
// int __cdecl compare( const void *i1, const void *i2 )
// //比较两个元素的大小，不是成员函数
// {
// 	const T &e1=*((T*)i1);
// 	const T &e2=*((T*)i2);
// 	return (e1>e2) ? 1 : ( (e1==e2) ? 0 : -1 );//比较地址大小？？
// }
// 
// /*
// template <class T, bool isObject>
// inline void CZArray<T, isObject>::sort(void)
// {
// 	qsort(m_data, m_size, sizeof(m_data[0]), compare<T, isObject>);
// };
// */
// 
// template <class T, bool isObject>
// inline void CZArray<T, isObject>::sort(int (__cdecl *p_compareFunc )(const void *elem1, const void *elem2 ))
// //以函数指针为参数的函数
// {
// 	qsort(m_data, m_size, sizeof(m_data[0]), p_compareFunc);
// //这个函数的目的：给数组进行排序，排序的函数就是那个函数指针，排序之后，重新写那个数组
// };
// 
// template <class T, bool isObject>
// inline void CZArray<T, isObject>::out(ostream &p_o, const char* p_spi)
// {
// 	for (int i=0; i<m_size; i++)
// 		p_o<<m_data[i]<<p_spi;//多输出一个字符串
// }

//
//SimpSet
//
// template <class T, bool isObject=false>
// class CZSimpSet : public CZArray<T, isObject>
// {
// public:
// 	CZSimpSet(int p_memSize=0, int p_addSize=0, CZArrayType p_type=atEmpty) 
// 		: CZArray<T, isObject>(p_memSize, p_addSize, p_type) {;}; 
// 	CZSimpSet(int p_memSize, CZArrayType p_type) 
// 		: CZArray<T, isObject>(p_memSize, p_type) {;}; 
// 	CZSimpSet(const CZArray<T, isObject> &p_source)
// 		: CZArray<T, isObject>(p_source) {;}; 
// 
// 	int iAdd(const T &p_t);	//return id for the added element
// 	void add(const T &p_t) { iAdd(p_t); };
// 
// protected:	//hide invalid meyhod for CZSimpSet
// 	void setAtGrow(int p_i, const T &p_t);
// 	T& add(void);
// 	void insertEmptyAt(int p_id, int p_count=1);
// 	void insert(int p_id, const T &p_t);
// 	void cloneFrom(const CZArray<T, isObject> &p_source, CCloneMethod p_cloneMethod=cmAuto);
// 	void stealDataFrom(CZArray<T, isObject> &p_source);
// };
// 
// template <class T, bool isObject>
// int CZSimpSet<T, isObject>::iAdd(const T &p_t)
// {
// 	for (int i=0; i<size(); i++)
// 		if (m_data[i]==p_t) return i;
// 	CZArray<T, isObject>::add(p_t);
// 	return topI();
// }

//
//SortArray	: using operator >; small to large
//
template <class T, bool isObject=false>
class CZSortArray : public CZArray<T, isObject>
{
public:
	CZSortArray(int p_memSize=0, int p_addSize=0, CZArrayType p_type=atEmpty) 
		: CZArray<T, isObject>(p_memSize, p_addSize, p_type) {;};
	CZSortArray(int p_memSize, CZArrayType p_type) 
		: CZArray<T, isObject>(p_memSize, p_type) {;}; 
	CZSortArray(const CZArray<T, isObject> &p_source)
		: CZArray<T, isObject>(p_source) {;}; 

	void add(const T &p_t);

protected:	//hide invalid meyhod for CZSortArray
	void setAtGrow(int p_i, const T &p_t);
	T& add(void);
	void insertEmptyAt(int p_id, int p_count=1);
	void insert(int p_id, const T &p_t);
	void cloneFrom(const CZArray<T, isObject> &p_source, CCloneMethod p_cloneMethod=cmAuto);
	void stealDataFrom(CZArray<T, isObject> &p_source);
};

template <class T, bool isObject>
void CZSortArray<T, isObject>::add(const T &p_t)
{
	int i;
	for (i=0; i<size(); i++)
		if (!(p_t>at(i))) break;
	CZArray<T, isObject>::insert(i, p_t);	
		//since CZSortArray::insert is hided(protected), 
		//we should call the base merhod(actually exactly the same
}

#endif
//
//堆栈
//CZSimpStack
// //
// template <class T, bool isObject=false>
// class CZSimpStack : public CZArray<T, isObject>
// {
// public:
// 	CZSimpStack(int p_memSize=0, int p_addSize=0)
// 		: CZArray<T, isObject>(p_memSize, p_addSize, atEmpty) {;}; 
// 	void push(const T &p_t) { add(p_t); };
// 	void pop(void) { removeTop(); };
// 
// protected:	//hide base class methods!
// 	void setAtGrow(int p_i, const T &p_t);
// 	T& add(void);
// 	void add(const T &p_t) { iAdd(p_t); };
// 	void insertEmptyAt(int p_id, int p_count=1);
// 	void insert(int p_id, const T &p_t);
// 	void cloneFrom(const CZArray<T, isObject> &p_source, CCloneMethod p_cloneMethod=cmAuto);
// 	void stealDataFrom(CZArray<T, isObject> &p_source);
// };


//
//内存分析例程
//test util-----------------------------------------------------------------------------------
//
//the heap function only functioned under NT!
// int heapState( int &totalUsed, int &totalFree );
// 
// ostream& heapDump(ostream& os);

/*
inline void getProcessMemoryInfo(PROCESS_MEMORY_COUNTERS &mc)
{
	::GetProcessMemoryInfo(::GetCurrentProcess(), &mc, sizeof mc);
}

ostream &operator>>(ostream &os, PROCESS_MEMORY_COUNTERS &mc)
{
    os	<<"Page fault count:\t" <<mc.PageFaultCount
		<<"Peak working set size:\t" <<mc.PeakWorkingSetSize
		<<"Working set size:\t" <<mc.WorkingSetSize
		<<"Peak paged pool usage:\t"	<<mc.QuotaPeakPagedPoolUsage
		<<"Paged pool usage:\t"	<<mc.QuotaPagedPoolUsage
		<<"Peak nonepaged pool usage:\t" <<mc.QuotaPeakNonPagedPoolUsage
		<<"Nonepaged pool usage:\t" <<mc.QuotaNonPagedPoolUsage
		<<"Pagefile usage:\t" <<mc.PagefileUsage
		<<"Peak pagefile usage:\t" <<mc.PeakPagefileUsage;
}

ostream &processMemoryInfo(ostream &os)
{
	PROCESS_MEMORY_COUNTERS mc;
	getProcessMemoryInfo(mc);
	os<<mc;
	return os;
}
*/

//
//性能分析例程
//profiler------------------------------------------------------------------------------------
//

//
//CTimeRecorder
//
// class CTimeRecorder
// {
// public:
// 	CTimeRecorder(void) {t0=GetTickCount();};
// 	~CTimeRecorder(void) {cout<<(GetTickCount()-t0)<<"ms used\n";}
// protected:
// 	int t0;
// };
// 
// class CTimeCalculator
// {
// public:
// 	CTimeCalculator(void) {t0=GetTickCount();};
// 	int elpTime(void) {return GetTickCount()-t0;}
// protected:
// 	int t0;
// };
// 
// //
// //SpeedCalculator
// //
// class CSpeedCalculator
// {
// public:
// 	CSpeedCalculator(void) { init(); };
// 	void init(void) { m_count=0; };
// 	bool bSampling(void);
// 	double getSpeed(void);
// 	ostream& speed(ostream& os); //it does not works as an operator, why???
// protected:
// 	int m_initTick, m_nowTick, m_count;
// };
// 
// //it does not works as an operator, why???
// ostream& CSpeedCalculator::speed(ostream& os); 
// 
// //
// //Profiler
// //
// enum //CProfilerAttrib
// {
// 	tfNone			=0, 
// 	tfSingleLine	=1, 
// 	tfAutoStart		=2, 
// 	tfAutoCOut		=4, 
// 	tfNotShowTitle	=8,
// 	tfMemprofiling	=16
// };
// 
// 
// class CZVProfiler
// {
// public:
// 	virtual void start(void) {};
// 	virtual void check(const char* /*msg*/=NULL) {;};
// 	virtual void over(void) {;};
// 	virtual const char* profile(void) { return "";};
// };
// 
// class CZProfiler : public CZVProfiler
// //主要是建立一个链表,链表有两个元素,一个是时间,一个是消息,在全部建立完后,以一定的方式输出内容.
// {
// public:
// 	CZProfiler(int p_attrib=tfNone, int p_lineWidtg=80, const char * p_title="---====== Profiler =====---");
// 	~CZProfiler(void);
// 
// 	void start(void);
// 	void check(const char* msg=NULL);
// 	void over(void);
// 	const char* profile(void) {if (!m_bOver) over(); return m_str.str();};//返回字符串的内容.
// protected:
// 	struct CCheckPoint
// 	{
// 		char msg[40];
// 		int checkPoint;//这是存放时间的数据
// 		//int globalMemory
// 	};
// 	CZArray<CCheckPoint> m_points;//message+整型的数据
// 	ostrstream m_str;
// 	bool m_bOver;
// 	int m_attrib;
// 	int m_lineWidth;//整型的
// 	const char* m_title;//只是在完整的格式输出的时候,才会有用.
// };
// 
// extern CZVProfiler fakeProfiler;
// 
// 
// //
// //信息输出类
// //msg output util-----------------------------------------------------------------------------
// //
// 
// //
// //CVMsgOutput
// //
// 
// //Since CVMsgOutput is platform-independent, 
// //it should deal the case that "windows.h" is not included.
// #ifndef _WINDEF_
// typedef DWORD   CColorRef;
// #else
// typedef COLORREF CColorRef;
// #endif	//ifndef _WINDEF_
// 
// #ifndef RGB
// #define RGB(r,g,b) ((COLORREF)(((BYTE)(r)|((WORD)((BYTE)(g))<<8))|(((DWORD)(BYTE)(b))<<16)))
// #endif
// 
// const CColorRef 
// 	crBlack	=RGB(0, 0, 0),
// 	crRed	=RGB(255, 0, 0);
// 
// enum CMOFormat { moRed=1, moBold=2 };
// 
// class CVMsgOutput//输出字符串的类
// //It's a interface class for MsgOutput. 
// //It can be used for a "null" msg out since it a non-abstract class
// {
// public:
// 	//constrator/destrator
// 	CVMsgOutput(void) { setDefault(); resetDefault(); };
// 	virtual ~CVMsgOutput(void) {};
// 
// 	//output message
// 	//Note! ever func that use p_format has a buffer only 16K long,
// 	//		user should not put big string to them!
// 	void puts(const char* p_format, ...);	
// 		//put a formatted string as CString::Format or printf, 
// 		//developer should refer to mfc\src\strex.cpp for the implement
// 	void puts(int/*CMOFormat mask*/ p_formatMask, const char* p_format, ...);	
// 	void putsl(int/*CMOFormat mask*/ p_formatMask, const char* p_msg);
// 		//putsl always add '\n' to the end os line
// 
// 	//setting
// 	//default attribte is black and normal(non-bold)
// 	virtual void setColor(CColorRef p_color) { m_color=p_color; };	
// 		//set color for the next ONLY ONE line to be output
// 	virtual void setBold(bool p_bBold=true) { m_bBold=p_bBold; };			
// 		//set bold font for the next ONLY ONE line
// 	virtual void setDefault(CColorRef p_color=crBlack, bool p_bBold=false)
// 		{ m_defaultColor=p_color; m_bDefaultBold=p_bBold; }
// 	virtual void puts1(const char* p_msg=NULL) { } ;		
// 		//put a message string to the arrigned rich edit, NULL for a new line
// 		//Note: all implement of overrides of puts1 should call "resetDefault()" at end
// 	void resetDefault(void) { setColor(m_defaultColor); setBold(m_bDefaultBold); };
// //protected:
// 	void puts1(const char* p_format, va_list argList) ;
// 
// protected:
// 	CColorRef m_color, m_defaultColor;
// 	bool m_bBold, m_bDefaultBold;
// };
// 
// extern CVMsgOutput fakeMsgOut;
// 
// //
// //CRichEditHelper
// //
// class CEscSever4RichHelper;
// class CRichEditHelper : public CVMsgOutput
// {
// public:
// 	//constrator/destrator
// 	CRichEditHelper(HWND p_hwndRich=NULL);
// 
// 	//initialization
// 	void attach(HWND p_hwndRich);
// 
// 	//output message
// 	void puts1(const char* p_msg=NULL);		
// 
// 	//setting
// 	void setAutoRedBeep(bool b) { m_bAutoRedBeep=b; };
// 	virtual void setDefault(CColorRef p_color=crBlack, bool p_bBold=false);
// 
// 	void setUserFontColor(CColorRef p_c);
// 	
// protected:
// 	HWND m_hwndRich;
// 	bool m_bAutoRedBeep;
// 
// 	friend class CEscSever4RichHelper;
// 	CEscSever4RichHelper *m_pEscSever;
// 
// 	CColorRef m_userFontColor;
// 	bool m_bUserFontSetted;
// };
// 
// //
// //class EscSever for RichHelper
// //
// char* const cNoEscapePrompt=NULL; 
// 	//using this string as parameter of p_msg
// 	//will cause EscSever do not prompt user while user break
// 
// class CEscSever4RichHelper
// {
// public:
// 	CEscSever4RichHelper(	CRichEditHelper* p_pRichHelper,
// 							const char* p_msg="你真的想退出吗？", 
// 							const char* p_escExceptionMsg="用户中断退出",
// 							const char* p_hintMsg="按Esc键中断退出");
// 	~CEscSever4RichHelper(void);
// 	virtual void setBreak(void);
// protected:
// 	const char* m_msg;
// 	const char* m_escExceptionMsg;
// 	CRichEditHelper* m_pRichHelper;
// 
// 	CWaitCursor m_wait;
// };
// 
// //
// //命令行解释工具类
// //command line paraser
// //
// enum { cpIgnoreCase=1, cpLowerPara=2 };
// class CCommandParaser
// {
// public:
// 	CCommandParaser(int argc, char * argv[], int p_attrib=cpIgnoreCase | cpLowerPara);
// 	~CCommandParaser(void) { free(); };
// 
// 	int getPara(const char* p_para);	//return id for para[], -1 for not found
// 	bool hasPara(const char* p_para) { return getPara(p_para)>=0; };
// 	bool hasParaPlus(const char* p_para);
// 	bool hasParaMinus(const char* p_para);
// 
// 	void free() { strs.free(); paras.free(); };
// 	
// 	enum { cMaxStrLen=512 };
// 	enum { cMaxParaLen=8 };
// 	struct CStr
// 	{
// 		char str[cMaxStrLen];
// 		int preParaID;
// 	};
// 	struct CPara
// 	{
// 		char para[cMaxParaLen];
// 		bool plusSigned;
// 		bool minusSigned;
// 	};
// 	CZArray<CStr> strs;
// 	CZArray<CPara> paras;
// protected:
// 	int m_attrib;
// };
// 
// //
// //Escape Sever
// //This util seems not useful
// //用于应用程序中运行一个长时间操作时中途检测用户是否要求
// //中途退出的工具接口类
// //
// class CVEscSever	//接口和伪实现类
// {
// public:
// 	CVEscSever(void) : m_msg(NULL), m_escExceptionMsg(NULL)/*, m_bRegistered(false)*/ {;};
// 	virtual bool bUserEscape(void) { /*assert(m_bRegistered);*/assert(m_msg); return false; };
// 	virtual void checkEscape(void);	
// 		//if break by user, this method will raise const char* exception
// 	virtual void registerEscape(const char* p_msg="你真的想退出吗？", 
// 						const char* p_escExceptionMsg="用户中断退出");
// 	//void unregisterEscape(void);
// protected:
// 	const char* m_msg;
// 	const char* m_escExceptionMsg;
// 	//bool m_bRegistered;
// };
// 
// extern CVEscSever fakeEscSever;
// 
// class CZEscSever : public CVEscSever
// {
// public:
// 	CZEscSever(void) : m_bBroken(false) {;};
// 	bool bUserEscape(void);
// 	void registerEscape(const char* p_msg="你真的想退出吗？", 
// 						const char* p_escExceptionMsg="用户中断退出")
// 		{ CVEscSever::registerEscape(p_msg, p_escExceptionMsg); m_bBroken=false;};
// 	void setBreak(void) {m_bBroken=true; };
// protected:
// 	bool m_bBroken;
// };
// 
// //
// //其他例程
// //Other Utility-------------------------------------------------------------------------------
// //
// //
// //FPS util
// //
// float getFPS(void);		//return 0 for fps not calcuated
//Note: since this func use function static valable, it must be used to calc ONLY ONE FPS!


