#ifndef DPOINT_H
#define DPOINT_H

#include <stdint.h>
#include <limits>
#include <cmath>
#include <vector>
#include <ostream>

#ifndef INLINE
#define INLINE static inline
#endif

//定义一个double类型的带有方向的Point
struct DoubleNormalPoint
{
	double X;
	double Y;
	double Z;
	double A;//绕X轴旋转
	double B;//绕Y轴旋转
	double C;//绕Z轴旋转

	DoubleNormalPoint(double x = 0.0, double y = 0.0, double z = 0.0, double a = 0.0, double b = 0.0, double c = 0.0):X(x),Y(y),Z(z),A(a),B(b),C(c){}
	DoubleNormalPoint P(){return DoubleNormalPoint(X,Y,Z,A,B,C);}

	friend inline bool operator ==(const DoubleNormalPoint& a,const DoubleNormalPoint& b)
	{
		return a.X == b.X && a.Y == b.Y && a.Z == b.Z && a.A == b.A && a.B == b.B && a.C == b.C;
	}
	friend inline bool operator !=(const DoubleNormalPoint& a,const DoubleNormalPoint&b)
	{
		return a.X != b.X || a.Y != b.Y || a.Z != b.Z || a.A != b.A || a.B != b.B || a.C != b.C;
	}
};

typedef DoubleNormalPoint NormalPoint;

INLINE NormalPoint operator - (const NormalPoint& p0) { return NormalPoint(-p0.X,-p0.Y,-p0.Z, -p0.A, -p0.B, -p0.C); }
INLINE NormalPoint operator + (const NormalPoint& p0,const NormalPoint &p1) { return NormalPoint(p0.X+p1.X, p0.Y+p1.Y, p0.Z+p1.Z, p0.A+p1.A, p0.B+p1.B, p0.C+p1.C); }
INLINE NormalPoint operator - (const NormalPoint& p0,const NormalPoint &p1) { return NormalPoint(p0.X-p1.X, p0.Y-p1.Y, p0.Z-p1.Z, p0.A-p1.A, p0.B-p1.B, p0.C-p1.C); }
INLINE NormalPoint operator * (const NormalPoint& p0,const int32_t i) { return NormalPoint(p0.X*i, p0.Y*i, p0.Z*i, p0.A*i, p0.B*i, p0.C*i); }
INLINE NormalPoint operator * (const int32_t i,const NormalPoint &p0) { return p0*i; }
INLINE NormalPoint operator / (const NormalPoint& p0,const int32_t i) { return NormalPoint(p0.X/i, p0.Y/i, p0.Z/i, p0.A/i, p0.B/i, p0.C/i); }

INLINE NormalPoint& operator += (NormalPoint& p0,const NormalPoint &p1) { p0.X += p1.X; p0.Y += p1.Y; p0.Z += p1.Z; p0.A += p1.A; p0.B += p1.B; p0.C += p1.C; return p0; }
INLINE NormalPoint& operator -= (NormalPoint& p0,const NormalPoint &p1) { p0.X -= p1.X; p0.Y -= p1.Y; p0.Z -= p1.Z; p0.A -= p1.A; p0.B -= p1.B; p0.C -= p1.C; return p0; }


//构建三个层次的数组
typedef std::vector<NormalPoint> CurvePath;
typedef std::vector<CurvePath> SurfacePath;
typedef std::vector<SurfacePath> SolidPath;

//定义两种操作符“《”为把元素放进数组，push_back表示进栈（调用时使用出栈函数即可，可以指定出哪个元素，只要给出位置指示）
inline CurvePath& operator << (CurvePath& poly, const NormalPoint& p) {poly.push_back(p); return poly;}
inline SurfacePath& operator << (SurfacePath& polys, const CurvePath& p) {polys.push_back(p); return polys;}
inline SolidPath& operator << (SolidPath& polys, const SurfacePath& p) {polys.push_back(p); return polys;}


//重载操作运算符“《”，表示把数组写入输出文件中
std::ostream& operator << (std::ostream &s, const NormalPoint &p);
std::ostream& operator << (std::ostream &s, const CurvePath &p);
std::ostream& operator << (std::ostream &s, const SurfacePath &p);
std::ostream& operator << (std::ostream &s, const SolidPath &p);

#endif 