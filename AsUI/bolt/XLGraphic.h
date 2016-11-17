/********************************************************************
*
* =-----------------------------------------------------------------=
* =                                                                 =
* =             Copyright (c) Xunlei, Ltd. 2004-2011                =
* =                                                                 =
* =-----------------------------------------------------------------=
* 
*   FileName    :   XLGraphic.h 
*   Author      :   xlue group(xlue@xunlei.com)
*   Create      :   2007年4月3日
*   LastChange  :   
*   History     :	
*
*   Description :   XLGraphic.dll的头文件，定义了相关的接口
*
********************************************************************/ 

#ifndef _XUNLEI_GRAPHIC_H_
#define _XUNLEI_GRAPHIC_H_

// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the XLGRAPHIC_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// XLGRAPHIC_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.

#ifdef WIN32
	#ifdef XLGRAPHIC_EXPORTS
		#ifdef __cplusplus
			#define XLGRAPHIC_API(x) extern "C" __declspec(dllexport) x __stdcall 
		#else
			#define XLGRAPHIC_API(x) __declspec(dllexport) x __stdcall 
		#endif //__cplusplus
	#elif defined (XLUE_UNION)
		#ifdef __cplusplus
			#define XLGRAPHIC_API(x) extern "C" x __stdcall 
		#else
			#define XLGRAPHIC_API(x) x __stdcall 
		#endif //__cplusplus
	#else//XLGRAPHIC_EXPORTS
		#ifdef __cplusplus
			#define XLGRAPHIC_API(x) extern "C" __declspec(dllimport) x __stdcall 
		#else
			#define XLGRAPHIC_API(x) __declspec(dllimport) x __stdcall 
		#endif // __cplusplus
	#endif//XLGRAPHIC_EXPORTS
#else
	#ifdef __cplusplus
		#define XLGRAPHIC_API(x) extern "C" x 
	#else
		#define XLGRAPHIC_API(x) x 
	#endif
#endif

#ifndef __XLFS_H__
	#include "XLFS.h"
#endif //__XLFS_H__

//////////////////常量///////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
#define XLGRAPHIC_INVALID_HANDLE		NULL
// 填充类型
#define XLGRAPHIC_FILL_SINGLE_COLOR		0	//单色填充
#define XLGRAPHIC_FILL_LINE_GRADIENT	1	//直线渐变填充
#define XLGRAPHIC_FILL_CIRCLE_GRADIENT	2	//圆弧渐变填充
// 颜色类型定义
#define	XLGRAPHIC_CT_ARGB32		0
#define XLGRAPHIC_CT_RGB24		1
#define XLGRAPHIC_CT_RGB565		2
#define XLGRAPHIC_CT_RGB555		3
#define XLGRAPHIC_CT_GRAY		4
#define XLGRAPHIC_CT_HSV655		5
#define XLGRAPHIC_CT_PARGB32	6
#define XLGRAPHIC_CT_HSV978		7
#define XLGRAPHIC_CT_HSL978		8
#define XLGRAPHIC_CT_UNKNOWN	0xCDCDCDCD

// 渲染类型
#define XLGRAPHIC_BLEND_CONST_ALPHA		0
#define XLGRAPHIC_BLEND_SRC_ALPHA		1
#define XLGRAPHIC_BLEND_DIRECTOVERLAP	2
#define XLGRAPHIC_BLEND_KEYVALUEOVERLAP	3

// 文本类型
#define XLTEXT_TYPE_GDI				0
#define XLTEXT_TYPE_FREETYPE		1
#define XLTEXT_TYPE_FIXEDGDI		2

// 字体的hint属性，仅对freetype引擎有效
#define XLTEXT_FONTHINT_DEFAULT				0
#define XLTEXT_FONTHINT_NOHINT				1
#define XLTEXT_FONTHINT_FORCEAUTOHINT		2
#define XLTEXT_FONTHINT_DISABLEAUTOHINT		3

//字体效果，仅对freetype引擎有效
#define XLTEXT_EFFECT_NONE			0
#define XLTEXT_EFFECT_BRIGHT		1
#define XLTEXT_EFFECT_BORDER		2

// 格式控制
#define			XLTEXT_DT_TOP			0x00000000
#define			XLTEXT_DT_VCENTER		0x00000004
#define			XLTEXT_DT_BOTTOM		0x00000008

#define			XLTEXT_DT_LEFT			0x00000000
#define			XLTEXT_DT_CENTER		0x00000001
#define			XLTEXT_DT_RIGHT			0x00000002

#define			XLTEXT_DT_WORD_ELLIPSIS	0x00000040
#define         XLTEXT_DT_END_ELLIPSIS  0x00000080
#define			XLTEXT_DT_WORDBREAK		0x00000100

// 获取整行的全部宽度，用于显示而不是排版
#define			XLTEXT_DT_VISIBLEWIDTH	0x00000200

// 逻辑字体的描述结构
#define XLTEXT_LF_FACESIZE 32

// 字体的反走样定义，默认为XLAntiAliasMode_Default
// 此时内部使用的是LCD和MONO的混合策略
#define XLTEXT_ANTIALIASMODE_DEFAULT	0
#define XLTEXT_ANTIALIASMODE_MONO		1
#define XLTEXT_ANTIALIASMODE_LIGHT		2
#define XLTEXT_ANTIALIASMODE_LCD		3


// 字符集定义
#define XLTEXT_CHAR_SET_ALL			0		//全字符
#define XLTEXT_CHAR_SET_UNCH		1		//西文字
#define XLTEXT_CHAR_SET_CH			2		//中文字

// XLTextEnv的背景模式
#define		XLTEXT_BK_TRANSPARENT	0x0000
#define		XLTEXT_BK_OPAQUE		0x0001

#define XLADJUST_MODE_NORMAL			0x00000001
#define XLADJUST_MODE_BEPAINT			0x00000002
#define XLADJUST_MODE_SPLITTERPOINT		0x00000004

// transform
#define XLTRANS_MODE_NORMAL				1
#define XLTRANS_MODE_ANTIALIAS			2
#define XLTRANS_MODE_SHADOW				4

// region limit defination
#define XLGRAPHIC_INTEX_NORMAL		0x00000000		
#define XLGRAPHIC_INTEX_INFINITY	0x00000001
#define XLGRAPHIC_INTEX_NINFINITY	0x00000002

// mask type
#define XLMASK_SOURCE_NULL		0
#define XLMASK_SOURCE_BITMAP	1
#define XLMASK_SOURCE_TEXTURE	2
#define XLMASK_SOURCE_FILL		3

//instance chanel 
#define XLGRAPHIC_COLORCHANEL_NULL	0
#define XLGRAPHIC_COLORCHANEL_B		0
#define XLGRAPHIC_COLORCHANEL_G		1
#define XLGRAPHIC_COLORCHANEL_R		2
#define XLGRAPHIC_COLORCHANEL_A		3

//mask blend type
#define XLMASK_BLEND_INSTEAD		1
#define XLMASK_BLEND_ATTENUNATE		2

// 纹理类型
#define XLTEXTURE_TYPE_STRETCH			0
#define XLTEXTURE_TYPE_NORMAL			1
#define XLTEXTURE_TYPE_NINEINONE		2
#define XLTEXTURE_TYPE_THREEINONEH		3
#define XLTEXTURE_TYPE_THREEINONEV		4
#define XLTEXTURE_TYPE_FIVEINONEH		5
#define XLTEXTURE_TYPE_FIVEINONEV		6
#define XLTEXTURE_TYPE_TILE				7

// 对于三合一纹理，中间部分拉伸
#define XLTEXTURE_CENTERSTRETCH    0   
// 对于三合一纹理，边缘部分拉伸
#define XLTEXTURE_SIDESTRETCH      1 


//////////////////类型///////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
// 内部颜色值，bgra的字节顺序，有别于COLORREF的字节顺序
typedef DWORD           XL_Color;

// 句柄类型定义
typedef void* XL_BITMAP_HANDLE;
typedef void* XL_BITMAPLIST_HANDLE;
typedef void* XL_TEXTURE_HANDLE;
typedef void* XL_TEXTENV_HANDLE;
typedef void* XL_FONT_HANDLE; 
typedef void* XL_IMAGELIST_HANDLE;
typedef void* XL_CURVE_HANDLE;
typedef void* XL_MASK_HANDLE;
typedef void* XL_IMAGESEQ_HANDLE;
typedef void* XL_TRANSFORM_HANDLE;
typedef void* XL_BRUSH_HANDLE;
typedef void* XL_PEN_HANDLE;

// 位图结构
typedef struct tagXLBitmapInfo
{
	DWORD ColorType;
	unsigned long Width;
	unsigned long Height;
	long ScanLineLength; //如果scan line是负数，表示位图是倒序存储的

}XLBitmapInfo;

typedef struct tagXLGraphicHint
{
	LPCRECT pClipRect;
	XL_MASK_HANDLE Mask;
	LPCRECT pMaskRect;
	DWORD MaskBlendType;
}XLGraphicHint;

//----------------初始化库-------------------------------
typedef struct tagXLGraphicParam
{
	// 指定需要的文本渲染引擎，包括gdi和freetype两种
	DWORD			textType;
	// 以下配置字段只对freetype文本渲染有效
	unsigned int	uCacheMaxFaces;
	unsigned int	uCacheMaxSize;
	unsigned int	uCacheMaxBytes;
	DWORD			fontHint;
	BOOL			bUseKerning;
	int				nItalicSlant;
	int				nXPixelsPerInch;
	int				nYPixelsPerInch;
}XLGraphicParam;

XLGRAPHIC_API(int) XL_InitGraphicLib(void* initParam);
XLGRAPHIC_API(int) XL_UnInitGraphicLib();
XLGRAPHIC_API(BOOL) XL_PrepareGraphicParam(XLGraphicParam* lpParam);
XLGRAPHIC_API(XLGraphicHint) XL_DefaultGraphicHint();

////////颜色空间转换  XL_Color解析
//RGBA  PRGBA
#define XLCOLOR_BGR(b,g,r)          ((XL_Color)(((BYTE)(b)|((WORD)((BYTE)(g))<<8))|(((DWORD)(BYTE)(r))<<16)))
#define XLCOLOR_BGRA(b,g,r,a)		((XL_Color)(((BYTE)(b)|((WORD)((BYTE)(g))<<8))|(((DWORD)((BYTE)(r)|((WORD)((BYTE)(a))<<8))<<16))))
#define XLCOLOR_BGRA_A(bgra)		((BYTE)((bgra)>>24))
#define XLCOLOR_BGRA_R(bgra)		((BYTE)((bgra)>>16))
#define XLCOLOR_BGRA_G(bgra)		((BYTE)((bgra)>>8))
#define XLCOLOR_BGRA_B(bgra)		((BYTE)(bgra))
XLGRAPHIC_API(XL_Color) XL_ARGB2PARGB(const XL_Color clr);
XLGRAPHIC_API(XL_Color) XL_PARGB2ARGB(const XL_Color clr); 

// rgba(colorref)  bgra
#define XLColorToCOLORREF(bgra)     (XLCOLOR_BGRA(XLCOLOR_BGRA_R(bgra), XLCOLOR_BGRA_G(bgra), \
	XLCOLOR_BGRA_B(bgra),XLCOLOR_BGRA_A(bgra)))

//HSL  HSV
#define XLCOLOR_HSV(h,s,v) \
	(XL_Color)((((DWORD)(h)) << 23) | (((DWORD)(s)) << 16) | ((WORD)(v)) << 8)
#define XLCOLOR_HSV_H(clr) ((short)((clr) >> 23))
#define XLCOLOR_HSV_S(clr) ((unsigned char)(((clr) << 9) >> 25))
#define XLCOLOR_HSV_V(clr) ((unsigned char)(((clr) << 16) >> 24))
XLGRAPHIC_API(XL_Color) XL_RGB2HSV(const XL_Color clrSource);
XLGRAPHIC_API(XL_Color) XL_HSV2RGB(const XL_Color hsv,const unsigned char alpha);

#define XLCOLOR_HSL(h,s,l) \
	(XL_Color)((((DWORD)(h)) << 23) | (((DWORD)(s)) << 16) | ((WORD)(l)) << 8)
#define XLCOLOR_HSL_H(clr) ((short)((clr) >> 23) )
#define XLCOLOR_HSL_S(clr) ((unsigned char)(((clr) << 9) >> 25))
#define XLCOLOR_HSL_L(clr) ((unsigned char)(((clr) << 16) >> 24))

XLGRAPHIC_API(XL_Color) XL_HSL2RGB(const XL_Color hsl,const unsigned char alpha);
XLGRAPHIC_API(XL_Color) XL_RGB2HSL(const XL_Color clrSource);

//XYZ  Lab
#define XLCOLOR_XYZ(x,y,z) \
	(XL_Color)(((DWORD)(x)) << 24 | ((DWORD)(y)) << 16 | ((WORD)(z)) <<8)
#define XLCOLOR_XYZ_X(clr) ((unsigned char)((clr) >> 24))
#define XLCOLOR_XYZ_Y(clr) ((unsigned char)(((clr) << 8) >> 24))
#define XLCOLOR_XYZ_Z(clr) ((unsigned char)(((clr) << 16) >> 24))

//L： 0 ～ 120 
//a： -120 ～ 120 
//b： -120 ～ 120
#define XLCOLOR_LAB(l,a,b) \
	(XL_Color)(((DWORD)(x)) << 24 | ((DWORD)(y)) << 16 | ((WORD)(z)) <<8)
#define XLCOLOR_LAB_L(clr) ((unsigned char)((clr) >> 24))
#define XLCOLOR_LAB_A(clr) ((char)(((clr) << 8) >> 24))
#define XLCOLOR_LAB_B(clr) ((char)(((clr) << 16) >> 24))

//-----------Bitmap数据结构定义---------------------------
XLGRAPHIC_API(XL_BITMAP_HANDLE) XL_CreateBitmap(DWORD BitmapColorType,unsigned long Width,unsigned long Height);
XLGRAPHIC_API(XL_BITMAP_HANDLE) XL_CreateBindBitmap(XLBitmapInfo* pBitmapInfo,BYTE* pBuffer);

XLGRAPHIC_API(unsigned long) XL_AddRefBitmap(XL_BITMAP_HANDLE hBitmap);
XLGRAPHIC_API(unsigned long) XL_ReleaseBitmap(XL_BITMAP_HANDLE hBitmap);

// 得到一个BindBuffer的新位图,该位图的有效性取决于hSrcBitmap的有效性
XLGRAPHIC_API(XL_BITMAP_HANDLE) XL_ClipSubBindBitmap(XL_BITMAP_HANDLE hSrcBitmap,LPCRECT pClipRect);

// 得到一个OwnerBuffer的新位图
XLGRAPHIC_API(XL_BITMAP_HANDLE) XL_CloneBitmap(XL_BITMAP_HANDLE hSrcBitmap);

// 用指定颜色来填充位图
XLGRAPHIC_API(void) XL_FillBitmap(XL_BITMAP_HANDLE hBitmap,DWORD newColor);

// 得到Bitmap的信息
XLGRAPHIC_API(int) XL_GetBitmapInfo(XL_BITMAP_HANDLE hBitmap,XLBitmapInfo* pResult);

// 获取图片的像素字节数
XLGRAPHIC_API(unsigned char) XL_GetBitmapPixelBytes(XL_BITMAP_HANDLE hBitmap);

// 判断图片是原始图，还是剪裁得来的图
XLGRAPHIC_API(BOOL) XL_IsBitmapOwnerBuffer(XL_BITMAP_HANDLE hBitmap);

// 获取图片的内存缓冲，需要格外小心操作，防止溢出！
XLGRAPHIC_API(BYTE*) XL_GetBitmapBuffer(XL_BITMAP_HANDLE hBitmap,unsigned long x,unsigned long y);

// 判断一张图片是否经过有损clip操作，也即通过XL_ClipSubBindBitmap剪裁了一个较小的图片出来
XLGRAPHIC_API(BOOL) XL_IsBitmapLossyCliped(XL_BITMAP_HANDLE hBitmap);

// 判断一张图片是否经过预乘处理
XLGRAPHIC_API(BOOL) XL_IsBitmapPreMultiplied(XL_BITMAP_HANDLE hBitmap);

// 对图片进行预处理，该过程不可逆；进入渲染体系之前，所有32位的bitmap都要进行预处理
XLGRAPHIC_API(BOOL) XL_PreMultiplyBitmap(XL_BITMAP_HANDLE hBitmap);

// 重新设置alpha通道，目标位图须是32位
XLGRAPHIC_API(BOOL) XL_ResetAlphaChannel(XL_BITMAP_HANDLE hBitmap, unsigned char alpha);

// 获取图片主色调
XLGRAPHIC_API(XL_Color) XL_GetBitmapMainColor(XL_BITMAP_HANDLE hSrcBmp,LPCRECT lpRegions,
											  unsigned int nRegionCnt);

// 对图片色深进行转换，如果源位图色深已经和目标位图色深一致，那么会直接返回
// 需要返回句柄只要不为null，就需要release
// 目前支持32位和24位互转、32位转8位，24位转8位
XLGRAPHIC_API(XL_BITMAP_HANDLE) XL_ConvertBitmap(XL_BITMAP_HANDLE hBitmap, DWORD destColorType);

// 加载pgn格式的图片，图片不经过预乘
XLGRAPHIC_API(XL_BITMAP_HANDLE) XL_LoadPngFromStream(XLFS_STREAM_HANDLE hStream);
XLGRAPHIC_API(XL_BITMAP_HANDLE) XL_LoadPngFromFile(const wchar_t* lpFile);

//----------Bitmap的一些Blend操作
// Blend操作
typedef struct tagXLBlendHint
{
	XLGraphicHint BaseHint;
	DWORD BlendType;
	union 
	{                 
		unsigned char ConstAlpha;	// alpha值
		unsigned long KeyValue;		// 关键色的值
	};   
}XLBlendHint;

// ClipRect相对DestBitmap的0,0点，MaskRect相对于ClipRect的0,0点
XLGRAPHIC_API(int) XL_Blend(XL_BITMAP_HANDLE hDesBitmap,long DstX,long DstY,XL_BITMAP_HANDLE hSrcBitmap, 
							const XLBlendHint* pHint);

typedef struct tagXLFillBlendHint
{
	XLBlendHint BaseHint;
	DWORD FillType;				//类型
	union 
	{
		XL_Color SingleColor;
		struct {
			XL_Color SrcColor;	//起点颜色
			XL_Color DstColor;	//目标点颜色
			long SrcX;			//起点X坐标值
			long SrcY;			//起点Y坐标值
			long DstX;			//目标点X坐标值
			long DstY;			//目标点Y坐标值
		}Gradient;
	};
}XLFillBlendHint;

XLGRAPHIC_API(int) XL_FillBlend(XL_BITMAP_HANDLE hDesBitmap, const XLFillBlendHint* pHint);


//新的Blend函数，支持拉伸的同时混合
//如果pSrcRect为NULL,则SrcRect=src的大小,如果pDestRect=NULL,则pDestRect=(0,0,srcRect.width,srcRect.height)
//StretchBlend结构
#define XLGRAPHIC_STRETCHTYPE_MULTISAMPLE_1X	0x00
#define XLGRAPHIC_STRETCHTYPE_MULTISAMPLE_2X	0x01
typedef struct tagXLStretchBlendHint
{
	XLBlendHint BaseHint;
	unsigned char StretchType;
}XLStretchBlendHint;

XLGRAPHIC_API(int) XL_StretchBlend(XL_BITMAP_HANDLE hDestBitmap,const RECT* pDestRect,
								   XL_BITMAP_HANDLE hSrcBitmap,const XLStretchBlendHint* pHint);
//---------Bitmap的拉伸操作
XLGRAPHIC_API(XL_BITMAP_HANDLE) XL_StretchBitmap(XL_BITMAP_HANDLE hBitmap,unsigned long newWidth,unsigned long newHeight);

//---------Bitmap loader 这一块接口会修改,使用开源通用的流定义方式
XLGRAPHIC_API(XL_BITMAP_HANDLE) XL_LoadBitmapFromMemory(const void *buffer, long size, unsigned long colorType);
XLGRAPHIC_API(XL_BITMAP_HANDLE) XL_LoadBitmapFromMemoryEx(const void *buffer, long size, unsigned long colorType, const wchar_t* lpFileName);
XLGRAPHIC_API(XL_BITMAP_HANDLE) XL_LoadBitmapFromFile(const wchar_t* filePath,unsigned long colorType);


//-----颜色调整-------
typedef struct tagHSLAdjust
{
	short HAdjust; //-180--180
	short SAdjust; //-100--100
	short LAdjust; //-255--255

	DWORD Mode;
	XL_Color KeyColor;
}XL_HSLAdjust;
XLGRAPHIC_API(BOOL) XL_AdjustBitmapColor(XL_BITMAP_HANDLE hSrcBmp,XL_HSLAdjust* pModify);
XLGRAPHIC_API(XL_Color) XL_AdjustColor(XL_Color clrSource,XL_HSLAdjust* pModify);
XLGRAPHIC_API(XL_HSLAdjust) XL_GetHSVModify(const XL_Color clrSource,DWORD mode);

///////分析
XLGRAPHIC_API(XL_HSLAdjust) XL_GetHSLAdjust(const XL_Color clrSource,DWORD mode);
XLGRAPHIC_API(int) XL_GetColorVariance(const XL_Color clr1,const XL_Color clr2);


//////////变换////////////////////////
XLGRAPHIC_API(unsigned long) XL_AddRefTrans(XL_TRANSFORM_HANDLE hTrans);
XLGRAPHIC_API(unsigned long) XL_ReleaseTrans(XL_TRANSFORM_HANDLE hTrans);
XLGRAPHIC_API(int) XL_GetSubTransCount(XL_TRANSFORM_HANDLE hTrans);
XLGRAPHIC_API(XL_TRANSFORM_HANDLE) XL_GetSubTrans(XL_TRANSFORM_HANDLE hTrans,int nIndex);
XLGRAPHIC_API(BOOL) XL_AddSubTrans(XL_TRANSFORM_HANDLE hTrans,XL_TRANSFORM_HANDLE hSubTrans);
XLGRAPHIC_API(XL_TRANSFORM_HANDLE) XL_CloneTrans(XL_TRANSFORM_HANDLE hLNT);
XLGRAPHIC_API(XL_BITMAP_HANDLE) XL_DoTrans(XL_BITMAP_HANDLE hSrc,XL_BITMAP_HANDLE hDest,
										   XL_TRANSFORM_HANDLE hTrans,DWORD flag);
XLGRAPHIC_API(void) XL_TransPoint(XL_TRANSFORM_HANDLE hTrans,const POINT *lpSrcPt,POINT* lpDstPt);
XLGRAPHIC_API(void) XL_TransRect(XL_TRANSFORM_HANDLE hTrans,const RECT* lpSrcRect,RECT* lpDstRect);
XLGRAPHIC_API(void) XL_TransGetDestBmpSize(XL_TRANSFORM_HANDLE hTrans,XL_BITMAP_HANDLE hSrc,
										   int*pWidth,int* pHeight);
XLGRAPHIC_API(void) XL_TransKeepCentreRect(XL_TRANSFORM_HANDLE hTrans,const RECT* lpSrcRect,
										   RECT* lpDstRect,const POINT* lpCentre);

typedef struct tagXLIntEx
{	
	DWORD m_flag;
	int m_value;
}XL_IntEx;

typedef struct tagIntLimit
{
	XL_IntEx m_min;
	XL_IntEx m_max;
}XL_IntLimit;

typedef struct tagRectRegionLimit
{
	//DWORD m_sign;
	XL_IntLimit m_limits[3];
}XL_RectRegion;

// bogus 3d linearity transform
typedef struct tagTransInfo
{
	double a[3][3];
	double b[3][1];
	int m_nRgnCnt;
	XL_RectRegion m_rgns[1];
}XLTransInfo;

XLGRAPHIC_API(BOOL) XL_GetTransInfo(XL_TRANSFORM_HANDLE hLNT,XLTransInfo* pInfo);
XLGRAPHIC_API(XL_TRANSFORM_HANDLE) XL_CreateTrans();
XLGRAPHIC_API(XL_TRANSFORM_HANDLE) XL_CreateRotateTrans(double dAnglex,double dAngley,double dAnglez);
XLGRAPHIC_API(XL_TRANSFORM_HANDLE) XL_CreateTransferTrans(int nDeltax,int nDeltay,int nDeltaz);
XLGRAPHIC_API(XL_TRANSFORM_HANDLE) XL_CreateStretchTrans(double dRatex,double dRatey,double dRatez);
XLGRAPHIC_API(BOOL) XL_SetTransParam(XL_TRANSFORM_HANDLE hLNT,double a[3][3],double b[3][1]);
XLGRAPHIC_API(BOOL) XL_SetTransDistDisp2obs(XL_TRANSFORM_HANDLE hLNT,int newDist);
XLGRAPHIC_API(BOOL) XL_SetTransRegion(XL_TRANSFORM_HANDLE hLNT,XL_RectRegion* pRegions,int nRegionCnt);


/////////////////////////////////////////////////////
XLGRAPHIC_API(XL_BITMAP_HANDLE) XL_GetBitmapChanel(XL_BITMAP_HANDLE hSrcBitmap,DWORD chanel,LPCRECT pRegion);
XLGRAPHIC_API(unsigned long) XL_AddRefMask(XL_MASK_HANDLE hMask);
XLGRAPHIC_API(unsigned long) XL_ReleaseMask(XL_MASK_HANDLE hMask);
XLGRAPHIC_API(XL_MASK_HANDLE) XL_NewMask();
XLGRAPHIC_API(XL_MASK_HANDLE) XL_CreateSubMask(XL_MASK_HANDLE hMask,LPCRECT lprcRect);
XLGRAPHIC_API(DWORD) XL_GetMaskSourceType(XL_MASK_HANDLE hMask);
XLGRAPHIC_API(void*) XL_GetMaskSource(XL_MASK_HANDLE hMask);
XLGRAPHIC_API(void) XL_BindMaskSource(XL_MASK_HANDLE hMask,DWORD type,void* pInstance,BOOL cache,DWORD chanel);
XLGRAPHIC_API(BOOL) XL_BuildMaskCache(XL_MASK_HANDLE hMask,int nWidth,int nHeight,LPCRECT pRegion,BOOL useDuelSize);
XLGRAPHIC_API(BOOL) XL_SetMaskSize(XL_MASK_HANDLE hMask,int nWidth,int nHeight,LPCRECT lpRegion);
XLGRAPHIC_API(void) XL_GetMaskSize(XL_MASK_HANDLE pMask,int *pWidth,int* pHeight,LPRECT lpRegion);
XLGRAPHIC_API(DWORD) XL_GetMaskSourceChanel(XL_MASK_HANDLE hMask);
XLGRAPHIC_API(XL_MASK_HANDLE) XL_CloneMask(XL_MASK_HANDLE hMask);
XLGRAPHIC_API(void) XL_SetMaskBmpStretch(XL_MASK_HANDLE hMask,BOOL bmpStretch);
XLGRAPHIC_API(void) XL_ClearMaskCache(XL_MASK_HANDLE hMask);


//---基础文本服务接口----

#ifdef WIN32
//-------------- 一些和Windows有关的OS函数--------------
XLGRAPHIC_API(int) XL_PaintBitmap(HDC hDC,XL_BITMAP_HANDLE hBitmap,const RECT* pDestRect,const RECT* pSrcRect);
XLGRAPHIC_API(int) XL_AlphaPaintBitmap(HDC hdcDest, int nXOriginDest, int nYOriginDest,
									   int nWidthDest, int nHeightDest,
									   XL_BITMAP_HANDLE hSrcBitmap, int nXOriginSrc, int nYOriginSrc,
									   int nWidthSrc, int nHeightSrc,
									   BLENDFUNCTION blendFunction);
#endif


// --------------字体的相关结构和函数定义------------------------------------------------------

// 字体的反走样定义，默认为XLAntiAliasMode_Normal
typedef struct tagXLLogFontInfo
{
	LONG  lfHeight;
	LONG  lfWidth;
	LONG  lfEscapement;
	LONG  lfOrientation;
	LONG  lfWeight;
	BYTE  lfItalic;
	BYTE  lfUnderline;
	BYTE  lfStrikeOut;
	BYTE  lfCharSet;
	BYTE  lfOutPrecision;
	BYTE  lfClipPrecision;
	BYTE  lfQuality;
	BYTE  lfPitchAndFamily;
	WCHAR lfFaceName[XLTEXT_LF_FACESIZE];

	BOOL    bUseCache;

	// 反走样相关参数
	DWORD antialiasMode;
	BOOL bIsBGROrder;        // 对LCD反走样模式有效，标识所在LCD屏幕的RGB排列顺序，默认是RGB排列

	BOOL bBorder;

}XLLogFontInfo, *LPXLLogFontInfo;

XLGRAPHIC_API(XL_FONT_HANDLE) XL_CreateFont(const LPXLLogFontInfo lpLogFontInfo);
XLGRAPHIC_API(BOOL) XL_GetLogFont(XL_FONT_HANDLE hFont, LPXLLogFontInfo lpLogFontInfo);
XLGRAPHIC_API(BOOL) XL_DeleteFont(XL_FONT_HANDLE hFont);
XLGRAPHIC_API(unsigned long) XL_AddRefFont(XL_FONT_HANDLE hFont);
XLGRAPHIC_API(unsigned long) XL_ReleaseFont(XL_FONT_HANDLE hFont);

// ----------------文本的相关结构和函数定义-----------------------------------------------------
//----------- 文本环境的创建和销毁----------------------------------

// 创建一个新的TextEnv并返回句柄，hSrc不为空，则新的TextEnv和hSrc相同，否则采用默认值
XLGRAPHIC_API(XL_TEXTENV_HANDLE) XL_CreateTextEnv(XL_TEXTENV_HANDLE hSrc);

// 创建指定文本渲染引擎的textenv
XLGRAPHIC_API(XL_TEXTENV_HANDLE) XL_CreateTextEnv2(DWORD type);

// 删除一个现有的TextEnv，若删除成功，返回true；否则返回false，可能该TextEnv还在使用中，引用计数大于1
XLGRAPHIC_API(BOOL) XL_DeleteTextEvn(XL_TEXTENV_HANDLE hTextEnv);

XLGRAPHIC_API(unsigned long) XL_AddRefTextEnv(XL_TEXTENV_HANDLE hTextEnv);
XLGRAPHIC_API(unsigned long) XL_ReleaseTextEnv(XL_TEXTENV_HANDLE hTextEnv);

//------------ 对文本环境的配置和读取 ---------------------

// 获得默认的facename
XLGRAPHIC_API(BOOL) XL_GetDefaultFaceName(wchar_t* szFaceName, UINT nLen);
XLGRAPHIC_API(BOOL) XL_IsSupportFont(const wchar_t* szFaceName);
XLGRAPHIC_API(BOOL) XL_SetConfigFontName(const wchar_t* szFaceName);

XLGRAPHIC_API(BOOL) XL_SetFreeTypeEnabled(BOOL bEnabled);
XLGRAPHIC_API(BOOL) XL_IsFreeTypeEnabled();

// 设定LCD模式下的三级抖动因子，gdi和freetype可独立设置
XLGRAPHIC_API(BOOL) XL_SetLCDModeFactor(BOOL gdi, double prim, double second, double tert);

//设置文本每行左右的间隔
XLGRAPHIC_API(int) XL_SetTextLRGap(XL_TEXTENV_HANDLE hTextEnv, int nGap);
XLGRAPHIC_API(int) XL_GetTextLRGap(XL_TEXTENV_HANDLE hTextEnv);

XLGRAPHIC_API(BOOL) XL_EnableRenderMode(BOOL bEnabled);
XLGRAPHIC_API(BOOL) XL_IsRenderModeEnabled();
XLGRAPHIC_API(BOOL) XL_SetLoadNoBitmap(BOOL bLoadNoBitmap);
XLGRAPHIC_API(DWORD) XL_SetHintMode(DWORD dwHintMode);
XLGRAPHIC_API(DWORD) XL_SetAntialiasMode(DWORD dwAntialiasMode);

XLGRAPHIC_API(XL_FONT_HANDLE) XL_GetDefaultFont();

XLGRAPHIC_API(BOOL)  XL_SetTextType(XL_TEXTENV_HANDLE hTextEnv, DWORD type);
XLGRAPHIC_API(DWORD)  XL_GetTextType(XL_TEXTENV_HANDLE hTextEnv);

XLGRAPHIC_API(XL_Color) XL_SetTextColor(XL_TEXTENV_HANDLE hTextEnv, XL_Color crText);
XLGRAPHIC_API(XL_Color) XL_GetTextColor(XL_TEXTENV_HANDLE hTextEnv);

XLGRAPHIC_API(XL_Color) XL_GetTextEffectColor(XL_TEXTENV_HANDLE hTextEnv);
XLGRAPHIC_API(XL_Color) XL_SetTextEffectColor(XL_TEXTENV_HANDLE hTextEnv, XL_Color crEffect);

XLGRAPHIC_API(DWORD) XL_SetTextEffectType(XL_TEXTENV_HANDLE hTextEnv, DWORD dwEffectType);
XLGRAPHIC_API(DWORD) XL_GetTextEffectType(XL_TEXTENV_HANDLE hTextEnv);

XLGRAPHIC_API(float) XL_SetTextBorderWidth(XL_TEXTENV_HANDLE hTextEnv,float fWidth);
XLGRAPHIC_API(float) XL_GetTextBorderWidth(XL_TEXTENV_HANDLE hTextEnv);

XLGRAPHIC_API(XL_Color) XL_SetBknColor(XL_TEXTENV_HANDLE hTextEnv, XL_Color crBkn);
XLGRAPHIC_API(XL_Color) XL_GetBknColor(XL_TEXTENV_HANDLE hTextEnv);

XLGRAPHIC_API(int)	XL_SetBknMode(XL_TEXTENV_HANDLE hTextEnv,int nMode);
XLGRAPHIC_API(int)	XL_GetBknMode(XL_TEXTENV_HANDLE hTextEnv);

XLGRAPHIC_API(XL_FONT_HANDLE) XL_SetTextFont(XL_TEXTENV_HANDLE hTextEnv, XL_FONT_HANDLE hFont);
XLGRAPHIC_API(XL_FONT_HANDLE) XL_GetTextFont(XL_TEXTENV_HANDLE hTextEnv);

XLGRAPHIC_API(unsigned long) XL_SetTextAlignment(XL_TEXTENV_HANDLE hTextEnv, unsigned long ulTextAlignment);
XLGRAPHIC_API(unsigned long) XL_GetTextAlignment(XL_TEXTENV_HANDLE hTextEnv);

XLGRAPHIC_API(int) XL_SetTextCharacterExtra(XL_TEXTENV_HANDLE hTextEnv, int nTextCharacterExtra);
XLGRAPHIC_API(int) XL_GetTextCharacterExtra(XL_TEXTENV_HANDLE hTextEnv);

XLGRAPHIC_API(BOOL) XL_SetTextForceUnderline(XL_TEXTENV_HANDLE hTextEnv, BOOL bForceUnderline);
XLGRAPHIC_API(BOOL) XL_GetTextForceUnderline(XL_TEXTENV_HANDLE hTextEnv);

XLGRAPHIC_API(int) XL_SetLineGap(XL_TEXTENV_HANDLE hTextEnv, int nLineGap);
XLGRAPHIC_API(int) XL_GetLineGap(XL_TEXTENV_HANDLE hTextEnv);

XLGRAPHIC_API(BOOL) XL_EnableShadow(XL_TEXTENV_HANDLE hTextEnv, BOOL bEnable);
XLGRAPHIC_API(XL_Color) XL_SetShadowColor(XL_TEXTENV_HANDLE hTextEnv, XL_Color crShadow);
XLGRAPHIC_API(BOOL) XL_SetShadowOffset(XL_TEXTENV_HANDLE hTextEnv, char xOffset, char yOffset);


// 获取指定长度nCount文本lpcstrText的高度和宽度信息
XLGRAPHIC_API(BOOL) XL_GetSinglelineTextExtent(XL_TEXTENV_HANDLE hTextEnv, const wchar_t* lpcstrText,INT nCount, LPSIZE lpSize);

//为richEdit写的一个，同时获得最低点到baseline的距离
XLGRAPHIC_API(BOOL) XL_GetTextExtentAndBaseLine(XL_TEXTENV_HANDLE hTextEnv, const wchar_t* lpcstrText,
												INT nCount, LPSIZE lpSize, int* lpnFontDescender);

XLGRAPHIC_API(BOOL) XL_GetMultilineTextExtent(XL_TEXTENV_HANDLE hTextEnv, const wchar_t* lpcstrText,INT nCount,INT nWidthLimit, LPSIZE lpSize);

// 按照指定格式输出文本，最后一个参数表示特效类型
XLGRAPHIC_API(int)	XL_DrawSinglelineText(XL_TEXTENV_HANDLE hTextEnv, XL_BITMAP_HANDLE hBmp, const wchar_t* lpcstrText,INT nCount, LPCRECT lpcRect, DWORD dwFormat, XLGraphicHint* pHint);
XLGRAPHIC_API(int)	XL_DrawMultilineText(XL_TEXTENV_HANDLE hTextEnv, XL_BITMAP_HANDLE hBmp, const wchar_t* lpcstrText,INT nCount, LPCRECT lpcRect, DWORD dwFormat, XLGraphicHint* pHint);

XLGRAPHIC_API(BOOL) XL_TabbedTextOut(XL_TEXTENV_HANDLE hTextEnv, XL_BITMAP_HANDLE hBmp, const wchar_t* lpcstrText, INT nCount, 
										INT nXPos, INT nYPos, INT nTabPostions, const INT* lpnTabPostions, INT nTabOrigin, LPSIZE lpTextDimension, XLGraphicHint* pHint);

//-------------对曲线对象的支持---------------------
#define XLCURVE_BEZIER		0 //贝塞尔
#define XLCURVE_BSPLINE     1 //B样条曲线


typedef struct tagXLAniPoint 
{
	float t;
	float s;
}XLAniPoint;

typedef struct tagXLAniCurveInfo
{
	DWORD type;
	unsigned long nsize;//控制点数量
    XLAniPoint* lpPoint;
}XLAniCurveInfo;

XLGRAPHIC_API(XL_CURVE_HANDLE) XL_CreateCurve(DWORD type);
XLGRAPHIC_API(unsigned long) XL_AddRefCurve(XL_CURVE_HANDLE hCurve);
XLGRAPHIC_API(unsigned long) XL_ReleaseCurve(XL_CURVE_HANDLE hCurve);
XLGRAPHIC_API(unsigned long) XL_AddCurveControlPoint(XL_CURVE_HANDLE hCurve, float t, float s);
XLGRAPHIC_API(BOOL) XL_RemoveCurveControlPoint(XL_CURVE_HANDLE hCurve, long lControlPoint);
XLGRAPHIC_API(unsigned long) XL_GetCurveControlPointCount(XL_CURVE_HANDLE hCurve);
XLGRAPHIC_API(BOOL) XL_GetCurveInfo(XL_CURVE_HANDLE hCurve, XLAniCurveInfo* info);
XLGRAPHIC_API(BOOL) XL_GetCurveControlPoint(XL_CURVE_HANDLE hCurve, float* point, int nIndex);
XLGRAPHIC_API(DWORD) XL_GetCurveType(XL_CURVE_HANDLE hCurve);
XLGRAPHIC_API(float) XL_GetCurveProgress(XL_CURVE_HANDLE hCurve, float t);


//------------ 对纹理对象的支持 ---------------------
XLGRAPHIC_API(XL_TEXTURE_HANDLE) XL_CreateTexture(DWORD type);
XLGRAPHIC_API(unsigned long) XL_AddRefTexture(XL_TEXTURE_HANDLE hTexture);
XLGRAPHIC_API(unsigned long) XL_ReleaseTexture(XL_TEXTURE_HANDLE hTexture);

XLGRAPHIC_API(BOOL) XL_SetTextureBitmap(XL_TEXTURE_HANDLE hTexture, XL_BITMAP_HANDLE hBitmap, unsigned long uFlags);
XLGRAPHIC_API(BOOL) XL_SetTextureRect(XL_TEXTURE_HANDLE hTexture, LPCRECT lpRect);
XLGRAPHIC_API(BOOL) XL_SetTextureOrigin(XL_TEXTURE_HANDLE hTexture, const char* lpstrX, const char* lpstrY);

XLGRAPHIC_API(XL_BITMAP_HANDLE) XL_GetTextureBitmap(XL_TEXTURE_HANDLE hTexture);
XLGRAPHIC_API(DWORD) XL_GetTextureType(XL_TEXTURE_HANDLE hTexture);

XLGRAPHIC_API(long) XL_AddTextureBlock(XL_TEXTURE_HANDLE hTexture, LPCRECT lpSrc, 
									   const char* lpstrDestLeft, const char* lpstrDestTop, 
									   const char* lpstrDestWidth, const char* lpstrDestHeight);
XLGRAPHIC_API(BOOL) XL_RemoveTextureBlock(XL_TEXTURE_HANDLE hTexture, long lBlock);

XLGRAPHIC_API(BOOL) XL_PaintTexture(XL_TEXTURE_HANDLE hTexture, long DstX, long DstY,
									XL_BITMAP_HANDLE hBitmap, const XLBlendHint* pHint);

//--------------对ImageList的支持-------------------------

#define IMAGELIST_FLAG_HOR  0       // ImageList是水平的
#define IMAGELIST_FLAG_VER  1       // ImageList是垂直的

#define IMAGELIST_FLAG_NOSPLITTER   0   // ImageList里面没有分隔条
#define IMAGELIST_FLAG_SPLITTER     2   // ImageList里面有分隔条

#define IMAGELIST_FLAG_EUQALBLOCK   0   // ImageList里面每个Block的size是相等的
#define IMAGELIST_FLAG_VARYBLOCK    4   // ImageList里面每个Block的size是不等的，需要用分隔条来确定


XLGRAPHIC_API(XL_IMAGELIST_HANDLE) XL_CreateImageList();
XLGRAPHIC_API(unsigned long) XL_AddRefImageList(XL_IMAGELIST_HANDLE hImageList);
XLGRAPHIC_API(unsigned long) XL_ReleaseImageList(XL_IMAGELIST_HANDLE hImageList);

XLGRAPHIC_API(BOOL) XL_SetImageListBitmap(XL_IMAGELIST_HANDLE hImageList, XL_BITMAP_HANDLE hBitmap, unsigned long uFlags, int nBlockSize, XL_Color crSplitter);
XLGRAPHIC_API(XL_BITMAP_HANDLE) XL_GetImageListBitmap(XL_IMAGELIST_HANDLE hImageList);

XLGRAPHIC_API(BOOL) XL_PaintImageList(XL_IMAGELIST_HANDLE hImageList, XL_BITMAP_HANDLE hBitmap, int nIndex, LPCRECT lprcDest, unsigned char uAlpha);
XLGRAPHIC_API(int) XL_GetImageCount(XL_IMAGELIST_HANDLE hImageList);
XLGRAPHIC_API(XL_BITMAP_HANDLE) XL_ExtractBitmap(XL_IMAGELIST_HANDLE hImageList, int nIndex);

//画笔对象
/* Pen Styles */
#define XLVECTOR_PS_SOLID            0
#define XLVECTOR_PS_DASH             1       /* -------  */
#define XLVECTOR_PS_DOT              2       /* .......  */
#define XLVECTOR_PS_DASHDOT          3       /* _._._._  */
#define XLVECTOR_PS_DASHDOTDOT       4       /* _.._.._  */
#define XLVECTOR_PS_NULL             5
#define XLVECTOR_PS_INSIDEFRAME      6

typedef struct tagXLLogPen
{
	int nPenStyle;
	int nWidth;
	XL_Color crColor;
}XLLOGPEN, *LPXLLOGPEN;

XLGRAPHIC_API(XL_PEN_HANDLE) XL_CreatePen(const LPXLLOGPEN lpLogPen);
XLGRAPHIC_API(BOOL) XL_GetLogPen(XL_PEN_HANDLE hPen, LPXLLOGPEN lpLogPen);
XLGRAPHIC_API(void) XL_SetPenColor(XL_PEN_HANDLE hPen, XL_Color color);
XLGRAPHIC_API(unsigned long) XL_AddRefPen(XL_PEN_HANDLE hPen);
XLGRAPHIC_API(unsigned long) XL_ReleasePen(XL_PEN_HANDLE hPen);

// 画刷对象
/* Brush Styles */
#define XLVECTOR_BS_SOLID            0
// 下面的暂不支持
#define XLVECTOR_BS_NULL             1
#define XLVECTOR_BS_HOLLOW           XLVECTOR_BS_NULL
#define XLVECTOR_BS_HATCHED          2
#define XLVECTOR_BS_PATTERN          3
#define XLVECTOR_BS_INDEXED          4
#define XLVECTOR_BS_DIBPATTERN       5
#define XLVECTOR_BS_DIBPATTERNPT     6
#define XLVECTOR_BS_PATTERN8X8       7

/* DIB color table identifiers */
#define XLVECTOR_DIB_RGB_COLORS      0 /* color table in RGBs */
#define XLVECTOR_DIB_PAL_COLORS      1 /* color table in palette indices */

/* Hatch Styles */
#define XLVECTOR_HS_HORIZONTAL       0       /* ----- */
#define XLVECTOR_HS_VERTICAL         1       /* ||||| */
#define XLVECTOR_HS_FDIAGONAL        2       /* \\\\\ */
#define XLVECTOR_HS_BDIAGONAL        3       /* ///// */
#define XLVECTOR_HS_CROSS            4       /* +++++ */
#define XLVECTOR_HS_DIAGCROSS        5       /* xxxxx */

typedef struct tagXLLOGBRUSH { 
	UINT     lbStyle; 
	XL_Color lbColor; 
	LONG     lbHatch; 
} XLLOGBRUSH, *LPXLLOGBRUSH;

XLGRAPHIC_API(XL_BRUSH_HANDLE) XL_CreateBrush(const LPXLLOGBRUSH lpLogBrush);
XLGRAPHIC_API(BOOL) XL_GetLogBrush(XL_BRUSH_HANDLE hBrush, LPXLLOGBRUSH lpLogBrush);
XLGRAPHIC_API(void) XL_SetBrushColor(XL_BRUSH_HANDLE hBrush, XL_Color color);
XLGRAPHIC_API(unsigned long) XL_AddRefBrush(XL_BRUSH_HANDLE hBrush);
XLGRAPHIC_API(unsigned long) XL_ReleaseBrush(XL_BRUSH_HANDLE hBrush);


XLGRAPHIC_API(BOOL) XL_DrawLine(XL_BITMAP_HANDLE hBmp, XL_PEN_HANDLE hPen, int nX1, int nY1,int nX2, int nY2, const XLGraphicHint* pHint);
XLGRAPHIC_API(BOOL) XL_DrawRectangle(XL_BITMAP_HANDLE hBmp, XL_BRUSH_HANDLE hBrush, XL_PEN_HANDLE hPen, int nX, int nY,int nWidth,  int nHeight, const XLGraphicHint* pHint);
XLGRAPHIC_API(BOOL) XL_DrawRectangle2(XL_BITMAP_HANDLE hBmp, XL_BRUSH_HANDLE hBrush, XL_PEN_HANDLE hPen, int nX, int nY,int nWidth,  int nHeight, const XLBlendHint* pBlendHint);
XLGRAPHIC_API(BOOL) XL_DrawPolygon(XL_BITMAP_HANDLE hBmp, XL_BRUSH_HANDLE hBrush,  XL_PEN_HANDLE hPen, POINT *lpPt, int nCount, const XLGraphicHint* pHint);
XLGRAPHIC_API(BOOL) XL_DrawEllipse(XL_BITMAP_HANDLE hBmp, XL_BRUSH_HANDLE hBrush, XL_PEN_HANDLE hPen, int nX, int nY,int nWidth, int nHeight, const XLGraphicHint* pHint);						   
XLGRAPHIC_API(BOOL) XL_DrawCurve(XL_BITMAP_HANDLE hBmp, XL_PEN_HANDLE hPen, int nX,  int nY,XL_CURVE_HANDLE hCurve, const XLGraphicHint* pHint);

XLGRAPHIC_API(XL_IMAGESEQ_HANDLE) XL_CreateImageSeq(int nCapacity);
XLGRAPHIC_API(unsigned long) XL_AddRefImageSeq(XL_IMAGESEQ_HANDLE hImageSeq);
XLGRAPHIC_API(unsigned long) XL_ReleaseImageSeq(XL_IMAGESEQ_HANDLE hImageSeq);

XLGRAPHIC_API(BOOL) XL_ImageSeqBeginAddImage(XL_IMAGESEQ_HANDLE hImageSeq);
XLGRAPHIC_API(BOOL) XL_ImageSeqEndAddImage(XL_IMAGESEQ_HANDLE hImageSeq);
XLGRAPHIC_API(BOOL) XL_ImageSeqAddImage(XL_IMAGESEQ_HANDLE hImageSeq, XL_BITMAP_HANDLE hBitmap, unsigned long long pos);

XLGRAPHIC_API(XL_BITMAP_HANDLE) XL_ImageSeqGetImage(XL_IMAGESEQ_HANDLE hImageSeq, unsigned long long pos);

XLGRAPHIC_API(int) XL_ImageSeqGetCount(XL_IMAGESEQ_HANDLE hImageSeq);
XLGRAPHIC_API(XL_BITMAP_HANDLE) XL_ImageSeqGetImageByIndex(XL_IMAGESEQ_HANDLE hImageSeq, int nIndex);

XLGRAPHIC_API(long) XL_StatObject(long lType);

// 垃圾回收接口，定期调用可以减少内存占用
XLGRAPHIC_API(long) XL_GC(void* lpReserved);


/////image loader扩展

// 注册loader需要的回调函数
typedef XL_BITMAP_HANDLE (*LPFNLOADIMAGEFROMSTREAM)(XLFS_STREAM_HANDLE hStream, unsigned long resultColorType);
typedef BOOL (*LPFNCHECKIMAGEFILEPATH)(const wchar_t* filePath, const wchar_t* extName);

XLGRAPHIC_API(int) XL_RegisterImageLoader(LPFNCHECKIMAGEFILEPATH lpFileCheck, LPFNLOADIMAGEFROMSTREAM lpLoaderFunction);
XLGRAPHIC_API(BOOL) XL_RemoveImageloader(int cookie);

#endif // _XUNLEI_GRAPHIC_H_

