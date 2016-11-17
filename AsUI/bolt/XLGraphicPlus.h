/********************************************************************
*
* =-----------------------------------------------------------------=
* =                                                                 =
* =             Copyright (c) Xunlei, Ltd. 2004-2011              =
* =                                                                 =
* =-----------------------------------------------------------------=
* 
*   FileName    :   XLGraphicPlus
*   Author      :   xlue group(xlue@xunlei.com)
*   Create      :   2011-9-26 15:38
*   LastChange  :   2011-9-26 15:38
*   History     :	
*
*   Description :   XLGraphics附属库的头文件
*
********************************************************************/ 
#ifndef __XLGRAPHICPLUS_H__
#define __XLGRAPHICPLUS_H__

#ifdef WIN32
	#ifdef XLGRAPHICPLUS_EXPORTS
		#ifdef __cplusplus
			#define XLGRAPHICPLUS_API(x) extern "C" __declspec(dllexport) x __stdcall 
		#else
			#define XLGRAPHICPLUS_API(x) __declspec(dllexport) x __stdcall 
		#endif //__cplusplus
	#elif defined (XLUE_UNION)
		#ifdef __cplusplus
			#define XLGRAPHICPLUS_API(x) extern "C" x __stdcall 
		#else
			#define XLGRAPHICPLUS_API(x) x __stdcall 
		#endif //__cplusplus
	#else //XLGRAPHICPLUS_API
		#ifdef __cplusplus
			#define XLGRAPHICPLUS_API(x) extern "C" __declspec(dllimport) x __stdcall 
		#else
			#define XLGRAPHICPLUS_API(x) __declspec(dllimport) x __stdcall 
		#endif // __cplusplus
	#endif //XLGRAPHICPLUS_EXPORTS
#else
	#ifdef __cplusplus
		#define XLGRAPHICPLUS_API(x) extern "C" x 
	#else // __cplusplus
		#define XLGRAPHICPLUS_API(x) x 
	#endif // __cplusplus
#endif

#include <XLGraphic.h>
#include <XLLuaRuntime.h>


//定义了XGP库需要的各种错误码
#define XLGP_RESULT_SUCCESS			0
#define XLGP_RESULT_FAILED			1   

#define XLGP_RESULT_INVALIDPARAM	2	// 无效参数

#define XLGP_RESULT_OUT_OF_MEMORY   3   //内存申请不足


//定义了扩展库的基本数据结构
typedef void*	XLGP_ICON_HANDLE;
typedef void*	XLGP_GIF_HANDLE;
typedef void*	XLGP_CURSOR_HANDLE;

typedef int  TCurveData[256];

////初始化相关数据结构和接口
typedef struct tagXLGraphicPlusParam
{
	BOOL bInitLua;	// 是否初始化lua，默认为TRUE
	// 增加初始化字段
}XLGraphicPlusParam;

XLGRAPHICPLUS_API(int)	XLGP_InitGraphicPlus(const XLGraphicPlusParam* lpInitParam);
XLGRAPHICPLUS_API(int)	XLGP_UnInitGraphicPlus();
XLGRAPHICPLUS_API(BOOL) XLGP_PrepareGraphicPlusParam(XLGraphicPlusParam* lpInitParam);

XLGRAPHICPLUS_API(BOOL) XLGP_RegisterLuaHost();


//////HBITMAP和XL_BITMAP_HANDLE互转

//转换成设备无关位图，其中destBitCount指定了目标位图的像素位数，可以取值32、24、16和8
XLGRAPHICPLUS_API(HBITMAP) XLGP_ConvertXLBitmapToDIB(XL_BITMAP_HANDLE hBitmap, unsigned short destBitCount);
//转换成设备相关位图，其中hDC指定了目标设备dc
XLGRAPHICPLUS_API(HBITMAP) XLGP_ConvertXLBitmapToDDB(XL_BITMAP_HANDLE hBitmap, HDC hDC);

//转换成XLGraphic支持的位图格式，其中destColorType指定了目标位图的格式，目前支持XLGRAPHIC_CT_ARGB32、XLGRAPHIC_CT_RGB24和XLGRAPHIC_CT_GRAY
//需要注意，如果DIB本身就是32位的，那么转换成XLBitmap后，alpha通道会保留原DIB的，如果原DIB的alpha通道无意义，那么请使用XL_ResetAlphaChannel对结果进行alpha修正
XLGRAPHICPLUS_API(XL_BITMAP_HANDLE) XLGP_ConvertDIBToXLBitmap(HBITMAP hBitmap, unsigned long destColorType);
XLGRAPHICPLUS_API(XL_BITMAP_HANDLE) XLGP_ConvertDDBToXLBitmap(HDC hDC, HBITMAP hBitmap, unsigned long destColorType);

//设备相关位图转设备无关位图，目标位图均是32bit; 如果源位图已经是DIB，那么返回null
XLGRAPHICPLUS_API(HBITMAP) XLGP_ConvertDDBToDIB(HDC hDC, HBITMAP hBitmap);

//////jpeg相关接口
XLGRAPHICPLUS_API(XL_BITMAP_HANDLE) XLGP_LoadJpegFromStream(XLFS_STREAM_HANDLE hStream);
XLGRAPHICPLUS_API(XL_BITMAP_HANDLE) XLGP_LoadJpegFromFile(const wchar_t* lpFileName);

//////加载bmp的接口
XLGRAPHICPLUS_API(XL_BITMAP_HANDLE) XLGP_LoadBmpFromStream(XLFS_STREAM_HANDLE hStream);
XLGRAPHICPLUS_API(XL_BITMAP_HANDLE) XLGP_LoadBmpFromFile(const wchar_t* lpFileName);
XLGRAPHICPLUS_API(XL_BITMAP_HANDLE) XLGP_LoadOriginBmpFromStream(XLFS_STREAM_HANDLE hStream);
XLGRAPHICPLUS_API(XL_BITMAP_HANDLE) XLGP_LoadOriginBmpFromFile(const wchar_t* lpFileName);

//////icon相关接口
XLGRAPHICPLUS_API(XLGP_ICON_HANDLE) XLGP_CreateIconFromBitmap(XL_BITMAP_HANDLE hBitmap);
XLGRAPHICPLUS_API(XLGP_ICON_HANDLE) XLGP_CreateIconFromHandle(HICON hIcon);

//从文件或者流加载图标
XLGRAPHICPLUS_API(XLGP_ICON_HANDLE) XLGP_LoadIconFromStream(XLFS_STREAM_HANDLE hStream, int cxDesired, int cyDesired);
XLGRAPHICPLUS_API(XLGP_ICON_HANDLE) XLGP_LoadIconFromFile(const wchar_t* lpFileName, int cxDesired, int cyDesired);

// 从dll或者exe文件提取指定索引的指定大小的图标，iconindex从0开始
XLGRAPHICPLUS_API(XLGP_ICON_HANDLE) XLGP_LoadIconFromModuleFile(const wchar_t* lpExeFileName, int iconIndex, int cxDesired, int cyDesired);
// 从dll或者exe模块提取指定索引的指定大小的图标，iconindex从0开始
XLGRAPHICPLUS_API(XLGP_ICON_HANDLE) XLGP_LoadIconFromModule(HMODULE hModule, int iconIndex, int cxDesired, int cyDesired);

// 从指定的文件类型获取指定大小的图标
XLGRAPHICPLUS_API(XLGP_ICON_HANDLE) XLGP_LoadIconFromFileExt(const wchar_t* lpExt, int cxDesired, int cyDesired);

XLGRAPHICPLUS_API(long) XLGP_SaveIconToIcoFile(XLGP_ICON_HANDLE hIcon, const wchar_t* lpFileName);

XLGRAPHICPLUS_API(unsigned long) XLGP_AddRefIcon(XLGP_ICON_HANDLE hIcon);
XLGRAPHICPLUS_API(unsigned long) XLGP_ReleaseIcon(XLGP_ICON_HANDLE hIcon);

//获取的系统icon句柄不可销毁
XLGRAPHICPLUS_API(HICON) XLGP_IconGetHandle(XLGP_ICON_HANDLE hIcon);
XLGRAPHICPLUS_API(BOOL) XLGP_IconGetSize(XLGP_ICON_HANDLE hIcon, int* lpWidth, int* lpHeight);

// 这里获取的bitmap是经过预乘的，可以直接设置到xlue里面作为bitmap资源使用
XLGRAPHICPLUS_API(XL_BITMAP_HANDLE) XLGP_IconGetBitmap(XLGP_ICON_HANDLE hIcon);

// 获取没有预乘的bitmap，该bitmap不可进入xlue的渲染体系
XLGRAPHICPLUS_API(XL_BITMAP_HANDLE) XLGP_IconGetOriginBitmap(XLGP_ICON_HANDLE hIcon);

// cursor相关接口
XLGRAPHICPLUS_API(XLGP_CURSOR_HANDLE) XLGP_CreateCursorFromHandle(HCURSOR hCursor, bool aniCursor);

//从文件或者流加载图标
XLGRAPHICPLUS_API(XLGP_CURSOR_HANDLE) XLGP_LoadCursorFromStream(XLFS_STREAM_HANDLE hStream, int cxDesired, int cyDesired);
XLGRAPHICPLUS_API(XLGP_CURSOR_HANDLE) XLGP_LoadCursorFromStreamEx(XLFS_STREAM_HANDLE hStream, int cxDesired, int cyDesired, BOOL aniCursor);
XLGRAPHICPLUS_API(XLGP_CURSOR_HANDLE) XLGP_LoadCursorFromFile(const wchar_t* lpFileName, int cxDesired, int cyDesired);


XLGRAPHICPLUS_API(unsigned long) XLGP_AddRefCursor(XLGP_CURSOR_HANDLE hCursor);
XLGRAPHICPLUS_API(unsigned long) XLGP_ReleaseCursor(XLGP_CURSOR_HANDLE hCursor);

//获取的系统cursor句柄，注意不可销毁
XLGRAPHICPLUS_API(HCURSOR) XLGP_CursorGetHandle(XLGP_CURSOR_HANDLE hCursor);


//gif相关接口
XLGRAPHICPLUS_API(XLGP_GIF_HANDLE) XLGP_LoadGifFromFile(const wchar_t* lpFileName);
XLGRAPHICPLUS_API(XLGP_GIF_HANDLE) XLGP_LoadGifFromStream(XLFS_STREAM_HANDLE hStream);
XLGRAPHICPLUS_API(unsigned long) XLGP_AddRefGif(XLGP_GIF_HANDLE hGif);
XLGRAPHICPLUS_API(unsigned long) XLGP_ReleaseGif(XLGP_GIF_HANDLE hGif);

XLGRAPHICPLUS_API(BOOL) XLGP_GifGetSize(XLGP_GIF_HANDLE hGif, int* lpWidth, int* lpHeight);

XLGRAPHICPLUS_API(unsigned int) XLGP_GifGetFrameCount(XLGP_GIF_HANDLE hGif);

//获取第uFrameIndex到其下一帧的时间间隔
XLGRAPHICPLUS_API(unsigned long) XLGP_GifGetIntervalAfterFrame(XLGP_GIF_HANDLE hGif, unsigned int uFrameIndex);

//获取第uFrameIndex帧的XLBitmap，从0开始，
XLGRAPHICPLUS_API(XL_BITMAP_HANDLE) XLGP_GifGetFrame(XLGP_GIF_HANDLE hGif, unsigned int uFrameIndex);

XLGRAPHICPLUS_API(XL_BITMAP_HANDLE) XLGP_GifGetFirstFrame(XLGP_GIF_HANDLE hGif);
XLGRAPHICPLUS_API(XL_BITMAP_HANDLE) XLGP_GifGetNextFrame(XLGP_GIF_HANDLE hGif);

//保存文件接口
//保存xlbmp为jpg文件。quality取值从0-100，0-压缩率最高，质量最低，100-质量最好
XLGRAPHICPLUS_API(BOOL) XLGP_SaveXLBitmapToJpegFile(XL_BITMAP_HANDLE hBmp, const wchar_t* lpFilePath, unsigned long quality);
XLGRAPHICPLUS_API(BOOL) XLGP_SaveXLBitmapToBmpFile(XL_BITMAP_HANDLE hBmp, const wchar_t* lpFilePath, unsigned long quality);
XLGRAPHICPLUS_API(BOOL) XLGP_SaveXLBitmapToPngFile(XL_BITMAP_HANDLE hBmp, const wchar_t* lpFilePath, unsigned long quality);


// lua相关接口，在指定版本有效

//下面这两组函数和xlue.dll导出的函数完全一致
//在句柄为null或者push失败的情况下，会push一个nil到栈顶
XLGRAPHICPLUS_API(BOOL) XLGP_PushBitmap(lua_State* luaState, XL_BITMAP_HANDLE hBitmap);
XLGRAPHICPLUS_API(BOOL) XLGP_PushMask(lua_State* luaState, XL_MASK_HANDLE hMask);
XLGRAPHICPLUS_API(BOOL) XLGP_PushTexture(lua_State* luaState, XL_TEXTURE_HANDLE hTexture);
XLGRAPHICPLUS_API(BOOL) XLGP_PushFont(lua_State* luaState, XL_FONT_HANDLE hFont);
XLGRAPHICPLUS_API(BOOL) XLGP_PushColor(lua_State* luaState, XL_Color* lpColor);
XLGRAPHICPLUS_API(BOOL) XLGP_PushColor2(lua_State* luaState, XL_Color color);
XLGRAPHICPLUS_API(BOOL) XLGP_PushPen(lua_State* luaState, XL_PEN_HANDLE hPen);
XLGRAPHICPLUS_API(BOOL) XLGP_PushBrush(lua_State* luaState, XL_BRUSH_HANDLE hBrush);
XLGRAPHICPLUS_API(BOOL) XLGP_PushImageList(lua_State* luaState, XL_IMAGELIST_HANDLE hImageList);
XLGRAPHICPLUS_API(BOOL) XLGP_PushImageSeq(lua_State* luaState, XL_IMAGESEQ_HANDLE hImageSeq);
XLGRAPHICPLUS_API(BOOL) XLGP_PushCurve(lua_State* luaState, XL_CURVE_HANDLE hCurve);
XLGRAPHICPLUS_API(BOOL) XLGP_PushIcon(lua_State* luaState, XLGP_ICON_HANDLE hIcon);
XLGRAPHICPLUS_API(BOOL) XLGP_PushGif(lua_State* luaState, XLGP_GIF_HANDLE hGif);
XLGRAPHICPLUS_API(BOOL) XLGP_PushCursor(lua_State* luaState, XLGP_CURSOR_HANDLE hCursor);

// 下面几个check函数，如果句柄不为空，那么在用完之后，需要对句柄调用相应的Release(color除外)
XLGRAPHICPLUS_API(BOOL) XLGP_CheckBitmap(lua_State* luaState, int index, XL_BITMAP_HANDLE *lpBitmap);
XLGRAPHICPLUS_API(BOOL) XLGP_CheckMask(lua_State* luaState, int index, XL_MASK_HANDLE *lpMask);
XLGRAPHICPLUS_API(BOOL) XLGP_CheckTexture(lua_State* luaState, int index, XL_TEXTURE_HANDLE *lpTexture);
XLGRAPHICPLUS_API(BOOL) XLGP_CheckFont(lua_State* luaState, int index, XL_FONT_HANDLE *lpFont);
XLGRAPHICPLUS_API(BOOL) XLGP_CheckColor(lua_State* luaState, int index, XL_Color** lplpColor);
XLGRAPHICPLUS_API(BOOL) XLGP_CheckColor2(lua_State* luaState, int index, XL_Color* lpColor);
XLGRAPHICPLUS_API(BOOL) XLGP_CheckPen(lua_State* luaState, int index, XL_PEN_HANDLE* lpPen);
XLGRAPHICPLUS_API(BOOL) XLGP_CheckBrush(lua_State* luaState, int index, XL_BRUSH_HANDLE* lpBrush);
XLGRAPHICPLUS_API(BOOL) XLGP_CheckImageList(lua_State* luaState, int index, XL_IMAGELIST_HANDLE* lpImageList);
XLGRAPHICPLUS_API(BOOL) XLGP_CheckImageSeq(lua_State* luaState, int index, XL_IMAGESEQ_HANDLE* lpImageSeq);
XLGRAPHICPLUS_API(BOOL) XLGP_CheckCurve(lua_State* luaState, int index, XL_CURVE_HANDLE* lpCurve);
XLGRAPHICPLUS_API(BOOL) XLGP_CheckIcon(lua_State* luaState, int index, XLGP_ICON_HANDLE* lpIcon);
XLGRAPHICPLUS_API(BOOL) XLGP_CheckGif(lua_State* luaState, int index, XLGP_GIF_HANDLE* lpGif);
XLGRAPHICPLUS_API(BOOL) XLGP_CheckCursor(lua_State* luaState, int index, XLGP_CURSOR_HANDLE* lpCursor);

#endif //__XLGRAPHICPLUS_H__