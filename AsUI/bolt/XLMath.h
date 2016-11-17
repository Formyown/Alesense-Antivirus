/********************************************************************
*
* =-----------------------------------------------------------------=
* =                                                                 =
* =             Copyright (c) Xunlei, Ltd. 2004-2009                =
* =                                                                 =
* =-----------------------------------------------------------------=
* 
*   FileName    :   XLMath.h
*   Author      :   xlue group(xlue@xunlei.com)
*   Create      :   2009-9-3
*   LastChange  :   
*   History     :	
*
*   Description :   图形库的数学相关函数的头文件
*
********************************************************************/ 
#ifndef __XUNLEI_MATH_H__
#define __XUNLEI_MATH_H__

#ifdef WIN32
	#ifdef XLGRAPHIC_EXPORTS
		#ifdef __cplusplus 
			#define XLMATH_API(x) extern "C" __declspec(dllexport) x __stdcall 
		#else //__cplusplus
			#define XLMATH_API(x)  __declspec(dllexport) x __stdcall 
		#endif //__cplusplus
	#elif defined (XLUE_UNION)
		#ifdef __cplusplus 
			#define XLMATH_API(x) extern "C" x __stdcall 
		#else //__cplusplus
			#define XLMATH_API(x) x __stdcall 
		#endif //__cplusplus
	#else //XLGRAPHIC_EXPORTS
		#ifdef __cplusplus 
			#define XLMATH_API(x) extern "C" __declspec(dllimport) x __stdcall 
		#else //__cplusplus
			#define XLMATH_API(x) __declspec(dllimport) x __stdcall 
		#endif //__cplusplus
	#endif
#else
	#ifdef _cplusplus
		#define XLMATH_API(x) extern "C" x
	#else //__cplusplus
		#define XLMATH_API(x) x
	#endif //__cplusplus
#endif

#define XL_RECT_NOT_INTERSECT 0
#define XL_RECT_INTERSET      1
#define XL_RECT_INCLUDE_RECT  2
#define XL_RECT_IN_RECT       3
//********************* 矩形相关算法 ****************************
//***矩形相交
XLMATH_API(BOOL) XL_SetRect(LPRECT lprc,int xLeft,int yTop,int xRight,int yBottom);
XLMATH_API(BOOL) XL_OffsetRect(LPRECT lprc,int dx,int dy);
XLMATH_API(BOOL) XL_SetRectEmpty(LPRECT lprc);
XLMATH_API(BOOL) XL_EqualRect(const RECT* lprc1,const RECT* lprc2);
XLMATH_API(BOOL) XL_CopyRect(LPRECT lprcDst,const RECT* lprsSrc);
XLMATH_API(BOOL) XL_IsRectEmpty(const RECT* lprc);
XLMATH_API(BOOL) XL_PtInRect(const RECT *lprc,POINT pt);

XLMATH_API(long) XL_IsRectIntersect(const RECT* pRect1,const RECT* pRect2);
XLMATH_API(BOOL) XL_IntersectRect(RECT* pResult,const RECT* pSrc1,const RECT* pSrc2);
//对一组矩形求交
XLMATH_API(BOOL) XL_IntersectRectEx(RECT* pResult,const RECT* pRectList,size_t listSize);

//***矩形合并
//得到一个包含pSrc1,pSrc2的最小矩形
XLMATH_API(BOOL) XL_UnionRect(RECT* pResult,const RECT* pSrc1,const RECT* pSrc2);
//将输入矩形集合合并成结果矩形集合,结果矩形集合包含输入矩形集合，并且结果矩形集合里没有相交的矩形 
XLMATH_API(long) XL_UnionRectEx(const RECT* pInputRectList,size_t listSize,RECT* pOutputRectList,size_t outlistSize);


//***********************表达式相关算法************************************

typedef DWORD* XL_EXP_HANDLE;

typedef LPCRECT (*XL_ExpBindProc)(void* lpObject, const char* lpcstrObject);

XLMATH_API(XL_EXP_HANDLE) XL_CreateExp();
XLMATH_API(BOOL) XL_DeleteExp(XL_EXP_HANDLE hExp);
XLMATH_API(XL_EXP_HANDLE) XL_CloneEXP(XL_EXP_HANDLE hSrcExp);

XLMATH_API(BOOL) XL_SetExp(XL_EXP_HANDLE hExp, const char* lpcstrExp, BOOL* lpbPureNum);
XLMATH_API(BOOL) XL_BindExpObject(XL_EXP_HANDLE hExp, void* lpObject, XL_ExpBindProc lpBindProc);
XLMATH_API(BOOL) XL_BindExpRect(XL_EXP_HANDLE hExp, LPCRECT lpRect);

XLMATH_API(BOOL) XL_CaclExp(XL_EXP_HANDLE hExp);
XLMATH_API(long) XL_GetExpValue(XL_EXP_HANDLE hExp);
XLMATH_API(long) XL_GetExpString(XL_EXP_HANDLE hExp, char* lpBuffer, long len);

// 矩形表达式
typedef DWORD* XL_RECTEXP_HANDLE;

XLMATH_API(XL_RECTEXP_HANDLE) XL_CreateRectExp();
XLMATH_API(BOOL) XL_DeleteRectExp(XL_RECTEXP_HANDLE hExp);

XLMATH_API(BOOL) XL_CaclRectExp(XL_RECTEXP_HANDLE hExp);
XLMATH_API(BOOL) XL_BindRectExpRect(XL_RECTEXP_HANDLE hExp, LPCRECT lpRect);

XLMATH_API(long) XL_GetRectExpLeft(XL_RECTEXP_HANDLE hExp);
XLMATH_API(long) XL_GetRectExpTop(XL_RECTEXP_HANDLE hExp);
XLMATH_API(long) XL_GetRectExpWidth(XL_RECTEXP_HANDLE hExp);
XLMATH_API(long) XL_GetRectExpHeight(XL_RECTEXP_HANDLE hExp);
XLMATH_API(BOOL) XL_GetRectExpRect(XL_RECTEXP_HANDLE hExp, LPRECT lpRect);

XLMATH_API(BOOL) XL_SetRectExpLeft(XL_RECTEXP_HANDLE hExp, LPCSTR lpcstrLeft);
XLMATH_API(BOOL) XL_SetRectExpTop(XL_RECTEXP_HANDLE hExp, LPCSTR lpcstrTop);
XLMATH_API(BOOL) XL_SetRectExpWidth(XL_RECTEXP_HANDLE hExp, LPCSTR lpcstrWidth);
XLMATH_API(BOOL) XL_SetRectExpHeight(XL_RECTEXP_HANDLE hExp, LPCSTR lpcstrHeight);

//*********************曲线拟合的相关算法*******************************************************
//曲线
//typedef DWORD* XL_ANICURVE_HANDLE;


//由于时间是递增的，B样条曲线中的部分点就必须舍弃掉，定义了一个坏点值9999
//若返回值为9999，则该点是坏点，应舍弃

#define XLMATH_BSPLINE_ERROR      9999

#define	XLMATH_BEZIER   0//贝塞尔
#define	XLMATH_BSPLINE  1//B样条

typedef struct ParamControlPoint 
{
	float t;
	float s;
}_ControlPoint_;

//lpControl -- 控制点数据
//nSize -- 控制点数量
//type -- 曲线类型
//t -- 时间值（0~1）

XLMATH_API(float) XL_GetCurvePosition(_ControlPoint_* lpControl, int nSize, int type, float t);


#ifdef WIN32
	#ifdef XLGRAPHIC_THREADSAFE
		#define XL_INCREMENT InterlockedIncrement	
		#define XL_DECREMENT InterlockedDecrement
	#else  //XLGRAPHIC_THREADSAFE
		#define	XL_INCREMENT(x) (++*(x))
		#define XL_DECREMENT(x) (--*(x))
	#endif //XLGRAPHIC_THREADSAFE
#else 
	#define	XL_INCREMENT(x) (++*(x))
	#define XL_DECREMENT(x) (--*(x))
#endif // WIN32

#endif // __XUNLEI_MATH_H__

