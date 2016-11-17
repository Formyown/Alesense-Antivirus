/********************************************************************
*
* =-----------------------------------------------------------------=
* =                                                                 =
* =             Copyright (c) Xunlei, Ltd. 2004-2009                =
* =                                                                 =
* =-----------------------------------------------------------------=
* 
*   FileName    :   XLUE.h
*   Author      :   xlue group(xlue@xunlei.com)
*   Create      :   2009-12-04
*   LastChange  :   
*   History     :	
*
*   Description :  XLUE.dll的头文件，定义了相关的接口
*
********************************************************************/ 
#ifndef _XUNLEI_XLUE_API_H_
#define _XUNLEI_XLUE_API_H_

#include "./XLUEHandle.h"
#include "XLGraphic.h"

#ifdef __cplusplus
extern "C"{
#endif // __cplusplus

#include "lua/lua.h"

#ifdef __cplusplus
}
#endif // __cplusplus

#define XLUE_USE_ANIMATION
#define XLUE_USE_LOADER
#define XLUE_USE_XAR

#ifdef XLUE_EXPORTS
	#ifdef __cplusplus 
		#define XLUE_API(x) extern "C" __declspec(dllexport) x __stdcall 
	#else
		#define XLUE_API(x) __declspec(dllexport) x __stdcall 
	#endif //__cplusplus
#elif defined (XLUE_UNION)
	#ifdef __cplusplus 
		#define XLUE_API(x) extern "C" x __stdcall 
	#else
		#define XLUE_API(x) x __stdcall 
	#endif //__cplusplus
#else // not XLUE_EXPORTS
	#ifdef __cplusplus
		#define XLUE_API(x) extern "C" __declspec(dllimport) x __stdcall
	#else
		#define XLUE_API(x) __declspec(dllimport) x __stdcall
	#endif //__cplusplus
#endif //XLUE_EXPORTS

//Kernel

XLUE_API(long) XLUE_SetDC(const void* lpBuffer, size_t size);

// 校验XAR
#define XARVERIFY_RESULT_SUCCESS		0
#define XARVERIFY_RESULT_INVALIDXAR		1
#define XARVERIFY_RESULT_LOADXARFAILED	2
#define XARVERIFY_RESULT_INVALIDKEY		3
#define XARVERIFY_RESULT_XARNOSIGN		4
#define XARVERIFY_RESULT_DECRYPTFAILED	5
#define XARVERIFY_RESULT_INVALIDDATA	6
#define XARVERIFY_RESULT_INVALIDXARSIGN	7
#define XARVERIFY_RESULT_INVALIDSIGN	8

XLUE_API(long) XLUE_VerifyXAR(const wchar_t* lpXARFile);
XLUE_API(long) XLUE_VerifyXAR2(const wchar_t* lpXARFile, const char* lpKey);

XLUE_API(long) XLUE_Init(void*);
XLUE_API(long) XLUE_InitLoader(void*);
XLUE_API(long) XLUE_InitLuaHost(void*);

XLUE_API(long) XLUE_RegisterStandardObjects(void*);
XLUE_API(long) XLUE_RegisterAnimation(void*);

XLUE_API(long) XLUE_Uninit(void*);
XLUE_API(long) XLUE_UninitLuaHost(void*);
XLUE_API(long) XLUE_UninitLoader(void*);
XLUE_API(long) XLUE_UninitHandleMap(void*);

XLUE_API(long) XLUE_ClearLuaObj(void*);

// 设置textobject使用的默认文本渲染引擎
XLUE_API(unsigned long) XLUE_SetTextObjectDefaultTextType(unsigned long textType);

XLUE_API(XLUE_OBJTREE_HANDLE) XLUE_CreateObjTree(const char* id);
XLUE_API(long) XLUE_DestroyObjTree(XLUE_OBJTREE_HANDLE hObjTree);
XLUE_API(XLUE_OBJTREE_HANDLE) XLUE_GetObjTree(const char* id);
XLUE_API(void) XLUE_SetTreeMaxClosure(XLUE_OBJTREE_HANDLE hObjTree,LPCRECT pClosure);
XLUE_API(const char*) XLUE_GetObjTreeID(XLUE_OBJTREE_HANDLE hObjTree);
XLUE_API(XLUE_LAYOUTOBJ_HANDLE) XLUE_GetUIObject(XLUE_OBJTREE_HANDLE hObjTree,const char* id);
XLUE_API(XLUE_LAYOUTOBJ_HANDLE) XLUE_GetObjTreeRootObj(XLUE_OBJTREE_HANDLE hObjTree);
XLUE_API(XLUE_LAYOUTOBJ_HANDLE) XLUE_GetObjTreeFocusObj(XLUE_OBJTREE_HANDLE hObjTree);
XLUE_API(XLUE_LAYOUTOBJ_HANDLE) XLUE_GetObjTreeCaptureMouseObj(XLUE_OBJTREE_HANDLE hObjTree);
XLUE_API(long) XLUE_SetObjTreeRootObj(XLUE_OBJTREE_HANDLE hObjTree,XLUE_LAYOUTOBJ_HANDLE hRootObj);
XLUE_API(XLUE_HOSTWND_HANDLE) XLUE_GetObjTreeBindHostWnd(XLUE_OBJTREE_HANDLE hObjTree);
XLUE_API(XLUE_HOSTWND_HANDLE) XLUE_GetHostWndByHandle(HWND hWnd);
XLUE_API(XLUE_LAYOUTOBJ_HANDLE) XLUE_HitTest(XLUE_OBJTREE_HANDLE hObjTree, long x, long y);

XLUE_API(XLUE_LAYOUTOBJ_HANDLE) XLUE_CreateObj(const char* id, const char* className, const char* package);
XLUE_API(long) XLUE_DestoryObj(XLUE_LAYOUTOBJ_HANDLE hObj);

XLUE_API(const char*) XLUE_GetObjID(XLUE_LAYOUTOBJ_HANDLE hObj);
//SetID在大部分时候都是不应调用的!
XLUE_API(BOOL) XLUE_SetObjID(XLUE_LAYOUTOBJ_HANDLE hObj,const char* id);
XLUE_API(const char*) XLUE_GetObjClassName(XLUE_LAYOUTOBJ_HANDLE hObj);

XLUE_API(XLUE_OBJTREE_HANDLE) XLUE_GetObjOwner(XLUE_LAYOUTOBJ_HANDLE hObj);
XLUE_API(XLUE_LAYOUTOBJ_HANDLE) XLUE_GetObjFather(XLUE_LAYOUTOBJ_HANDLE hObj);
//不检查循环应用以及去重，请使用者保证
XLUE_API(long) XLUE_AddObjChild(XLUE_LAYOUTOBJ_HANDLE hObj,XLUE_LAYOUTOBJ_HANDLE hChild);
XLUE_API(long) XLUE_RemoveObjChild(XLUE_LAYOUTOBJ_HANDLE hObj,XLUE_LAYOUTOBJ_HANDLE hChild);
XLUE_API(size_t) XLUE_GetObjChildCount(XLUE_LAYOUTOBJ_HANDLE hObj);
XLUE_API(XLUE_LAYOUTOBJ_HANDLE) XLUE_GetObjChild(XLUE_LAYOUTOBJ_HANDLE hObj,size_t index);
XLUE_API(BOOL) XLUE_IsChild(XLUE_LAYOUTOBJ_HANDLE hObj, XLUE_LAYOUTOBJ_HANDLE hChild);

//只有自己是control有意义
XLUE_API(XLUE_LAYOUTOBJ_HANDLE) XLUE_GetControlObject(XLUE_LAYOUTOBJ_HANDLE hObj,const char* id);
XLUE_API(XLUE_LAYOUTOBJ_HANDLE) XLUE_GetOwnerControl(XLUE_LAYOUTOBJ_HANDLE hObj);
XLUE_API(BOOL) XLUE_IsControl(XLUE_LAYOUTOBJ_HANDLE hObj);

XLUE_API(const RECT*) XLUE_GetObjPos(XLUE_LAYOUTOBJ_HANDLE hObj);
XLUE_API(long) XLUE_SetObjPos(XLUE_LAYOUTOBJ_HANDLE hObj,RECT* pNewPos);
XLUE_API(const RECT*) XLUE_GetObjAbsPos(XLUE_LAYOUTOBJ_HANDLE hObj);
XLUE_API(BOOL) XLUE_IsObjLimitChild(XLUE_LAYOUTOBJ_HANDLE hObj);
XLUE_API(void) XLUE_SetObjLimitChild(XLUE_LAYOUTOBJ_HANDLE hObj,BOOL isLimit);
XLUE_API(void) XLUE_ObjPushDirtyRect(XLUE_LAYOUTOBJ_HANDLE hObj,RECT* pDirty);
XLUE_API(void) XLUE_EnableObjInputTarget(XLUE_LAYOUTOBJ_HANDLE hObj,BOOL isEnable);
XLUE_API(void) XLUE_SetObjCaptureMouse(XLUE_LAYOUTOBJ_HANDLE hObj,BOOL isCapture);
XLUE_API(void) XLUE_SetObjFocus(XLUE_LAYOUTOBJ_HANDLE hObj,BOOL isFocus);
XLUE_API(long) XLUE_GetObjZorder(XLUE_LAYOUTOBJ_HANDLE hObj);
XLUE_API(void) XLUE_SetObjZorder(XLUE_LAYOUTOBJ_HANDLE hObj,long zorder);
XLUE_API(long) XLUE_GetObjTabOrder(XLUE_LAYOUTOBJ_HANDLE hObj);
XLUE_API(void) XLUE_SetObjTabOrder(XLUE_LAYOUTOBJ_HANDLE hObj,long newTabOrder);
XLUE_API(void) XLUE_ObjRouteToFather(XLUE_LAYOUTOBJ_HANDLE hObj);
XLUE_API(void) XLUE_SetObjCursorID(XLUE_LAYOUTOBJ_HANDLE hObj,const char* pid);
XLUE_API(const char*) XLUE_GetObjCursorID(XLUE_LAYOUTOBJ_HANDLE hObj, long x, long y);

XLUE_API(BOOL) XLUE_GetObjVisible(XLUE_LAYOUTOBJ_HANDLE hObj);
XLUE_API(void) XLUE_SetObjVisible(XLUE_LAYOUTOBJ_HANDLE hObj,BOOL isVisible,BOOL isRecursive);
XLUE_API(BOOL) XLUE_GetObjChildrenVisible(XLUE_LAYOUTOBJ_HANDLE hObj);
XLUE_API(void) XLUE_SetObjChildrenVisible(XLUE_LAYOUTOBJ_HANDLE hObj,BOOL isVisible);

XLUE_API(BOOL) XLUE_GetObjFatherVisible(XLUE_LAYOUTOBJ_HANDLE hObj);
XLUE_API(BOOL) XLUE_GetObjPrivateVisible(XLUE_LAYOUTOBJ_HANDLE hObj);

XLUE_API(BOOL) XLUE_GetObjEnable(XLUE_LAYOUTOBJ_HANDLE hObj);
XLUE_API(void) XLUE_SetObjEnable(XLUE_LAYOUTOBJ_HANDLE hObj,BOOL bEnable);
XLUE_API(BOOL) XLUE_GetObjChildrenEnable(XLUE_LAYOUTOBJ_HANDLE hObj);
XLUE_API(void) XLUE_SetObjChildrenEnable(XLUE_LAYOUTOBJ_HANDLE hObj,BOOL bEnable);

XLUE_API(BOOL) XLUE_GetObjFatherEnable(XLUE_LAYOUTOBJ_HANDLE hObj);
XLUE_API(BOOL) XLUE_GetObjPrivateEnable(XLUE_LAYOUTOBJ_HANDLE hObj);

XLUE_API(unsigned char) XLUE_GetRenderObjAlpha(XLUE_LAYOUTOBJ_HANDLE hObj);
XLUE_API(void) XLUE_SetRenderObjAlpha(XLUE_LAYOUTOBJ_HANDLE hObj,unsigned char alpha);

XLUE_API(BOOL) XLUE_GetObjInheritMask(XLUE_LAYOUTOBJ_HANDLE hObj);
XLUE_API(void) XLUE_SetObjInheritMask(XLUE_LAYOUTOBJ_HANDLE hObj,BOOL isVisible,BOOL isRecursive);

XLUE_API(XL_BITMAP_HANDLE) XLUE_RenderTree(XLUE_OBJTREE_HANDLE hTree,LPCRECT pViewRect);
XLUE_API(BOOL) XLUE_RenderObject(XLUE_LAYOUTOBJ_HANDLE hObj,XL_BITMAP_HANDLE hDstBmp,BOOL isRenderChild,LPCRECT pViewRect);

#ifdef XLUE_USE_ANIMATION
XLUE_API(XLUE_ANIMATION_HANDLE) XLUE_CreateAnimation(const char* className,const char* lpID, const char* package);
XLUE_API(long) XLUE_AddRefAnimation(XLUE_ANIMATION_HANDLE hAnimation);
XLUE_API(long) XLUE_ReleaseAnimation(XLUE_ANIMATION_HANDLE hAnimation);
XLUE_API(const char*) XLUE_GetAnimationClassName(XLUE_ANIMATION_HANDLE hAnimation);
XLUE_API(long) XLUE_GetAnimationState(XLUE_ANIMATION_HANDLE hAnimation);
XLUE_API(long) XLUE_ResumeAnimation(XLUE_ANIMATION_HANDLE hAnimation);
XLUE_API(long) XLUE_StopAnimation(XLUE_ANIMATION_HANDLE hAnimation);
XLUE_API(long) XLUE_UpdateAnimationRunningTime(XLUE_ANIMATION_HANDLE hAnimation);
XLUE_API(long) XLUE_GetAnimationRunningTime(XLUE_ANIMATION_HANDLE hAnimation);

XLUE_API(long) XLUE_SetKeyAnimationForceStop(XLUE_ANIMATION_HANDLE hAnimation);
XLUE_API(long) XLUE_SetKeyAnimationTotalTime(XLUE_ANIMATION_HANDLE hAnimation,unsigned long totalTime);
XLUE_API(unsigned long) XLUE_GetKeyAnimationTotalTime(XLUE_ANIMATION_HANDLE hAnimation);
XLUE_API(long) XLUE_SetKeyAnimationLoop(XLUE_ANIMATION_HANDLE hAnimation, BOOL bloop);
XLUE_API(long) XLUE_PosChangAniBindLayoutObj(XLUE_ANIMATION_HANDLE hAnimation,XLUE_LAYOUTOBJ_HANDLE hObj, BOOL isRender/* = FALSE*/);
XLUE_API(long) XLUE_PosChangeAniSetKeyPos(XLUE_ANIMATION_HANDLE hAnimation,long startLeft,long startTop,long endLeft,long endTop);
XLUE_API(long) XLUE_PosChangeAniSetKeyRect(XLUE_ANIMATION_HANDLE hAnimation,const RECT* pStart,const RECT* pEnd);

XLUE_API(long) XLUE_SetKeyAnimationBindObj(XLUE_ANIMATION_HANDLE hAnimation,XLUE_LAYOUTOBJ_HANDLE hObj, BOOL isRender/* = FALSE*/);

//增加了曲线计算的两个方法
XLUE_API(long) XLUE_AniBindCurveID(XLUE_ANIMATION_HANDLE hAnimation, const char* id);
XLUE_API(long) XLUE_AniBindCurve(XLUE_ANIMATION_HANDLE hAnimation, XL_CURVE_HANDLE hCurve);
XLUE_API(const char*) XLUE_AniGetCurveID(XLUE_ANIMATION_HANDLE hAnimation);
XLUE_API(XL_CURVE_HANDLE) XLUE_AniGetCurve(XLUE_ANIMATION_HANDLE hAnimation);

XLUE_API(long) XLUE_AlphaChangAniBindRenderObj(XLUE_ANIMATION_HANDLE hAnimation,XLUE_LAYOUTOBJ_HANDLE hObj, BOOL isRender/* = FALSE*/);
XLUE_API(long) XLUE_AlphaChagneAniSetKeyAlpha(XLUE_ANIMATION_HANDLE hAnimatio,unsigned char startAlpha,unsigned char endAlpha);

XLUE_API(long) XLUE_MaskChangeAniBindMaskObj(XLUE_ANIMATION_HANDLE hAnimation, XLUE_LAYOUTOBJ_HANDLE hObj);
XLUE_API(XLUE_LAYOUTOBJ_HANDLE) XLUE_MaskChangeAniGetBindMaskObj(XLUE_ANIMATION_HANDLE hAnimation);
XLUE_API(long) XLUE_MaskChangeAniSetMaskKeyFrame(XLUE_ANIMATION_HANDLE hAnimation, 
                                                 POINT beginOrg, SIZE beginSize,POINT endOrg, SIZE endSize);
//centre mode
typedef enum tagAngleChangeAniCentreMode
{	
	LeftTopCoordinate = 0,//以左上为0,0的绝对坐标
	WidthHeightRate = 1,//以与长宽的比例表示中心

}AngleChangeAniCentreMode;

//position mode
typedef enum tagAngleChangeAniPositionMode
{
	CentreStay = 0,

}AngleChangeAniPositionMode;

//size limit mode
typedef enum tagAngleChangeAniSizeLimitMode
{
	NoLimit = 0,
	SourceObjectSize = 1,

}AngleChangeAniSizeLimitMode;

XLUE_API(long) XLUE_B3DAniSetCentrePoint(XLUE_ANIMATION_HANDLE hAnimation,POINT centre);
XLUE_API(long) XLUE_B3DAniSetZPlane(XLUE_ANIMATION_HANDLE hAnimation,int ZPlaneCoordinate);
XLUE_API(long) XLUE_B3DAniSetCentrePointMode(XLUE_ANIMATION_HANDLE hAnimation,AngleChangeAniCentreMode mode);
XLUE_API(long) XLUE_B3DAniSetDistanceDisp2Obs(XLUE_ANIMATION_HANDLE hAnimation,int distDisp2obs);
XLUE_API(long) XLUE_B3DAniSetPositionMode(XLUE_ANIMATION_HANDLE hAnimation,AngleChangeAniPositionMode mode);
XLUE_API(long) XLUE_B3DAniSetSizeLimitMode(XLUE_ANIMATION_HANDLE hAnimation,AngleChangeAniSizeLimitMode mode);
XLUE_API(long) XLUE_B3DAniSetBlendMode(XLUE_ANIMATION_HANDLE hAnimation,DWORD mode);

XLUE_API(long) XLUE_AngleChangeAniBindRenderObj(XLUE_ANIMATION_HANDLE hAnimation,XLUE_LAYOUTOBJ_HANDLE hObj);
XLUE_API(long) XLUE_AngleChangeAniSetKeyAngle(XLUE_ANIMATION_HANDLE hAnimation,double startAngle[3],double endAngle[3]);
XLUE_API(long) XLUE_AngleChangeAniSetKeyRange(XLUE_ANIMATION_HANDLE hAnimation,double startRange[3],double endRange[3]);

//turn object flag
typedef enum tagTurnObjectFlag
{
	RoundX = 0,
	RoundY = 1,
	
	Deasil = 10,
	Widdershins = 11,

	EffectNormal = 21,
	EffectBlur = 22,

}TurnObjectFlag;

XLUE_API(long) XLUE_TurnObjectAniBindRenderObj(XLUE_ANIMATION_HANDLE hAnimation,XLUE_LAYOUTOBJ_HANDLE hFrontObj,XLUE_LAYOUTOBJ_HANDLE hBackObj);
XLUE_API(long) XLUE_TurnObjectAniBindFrontObj(XLUE_ANIMATION_HANDLE hAnimation,XLUE_LAYOUTOBJ_HANDLE hFrontObj,LPCRECT pFrontView);
XLUE_API(long) XLUE_TurnObjectAniBindBackObj(XLUE_ANIMATION_HANDLE hAnimation,XLUE_LAYOUTOBJ_HANDLE hBackObj);
XLUE_API(long) XLUE_TurnObjectAniSetFlag(XLUE_ANIMATION_HANDLE hAnimation,TurnObjectFlag flag);
XLUE_API(XLUE_HOSTWND_HANDLE) XLUE_GetHostWndByID(const char* id);

#endif //XLUE_USE_ANIMATION

// hostwnd
typedef void * OS_HOSTWND_HANDLE;

typedef enum tagCmdShow
{
	CS_SW_HIDE = 0,
	CS_SW_SHOWNORMAL = 1,
	CS_SW_SHOWMINIMIZED = 2,
	CS_SW_SHOWMAXIMIZED = 3,
	CS_SW_SHOWNOACTIVATE = 4,
	CS_SW_SHOW = 5,
	CS_SW_MINIMIZE = 6,
	CS_SW_SHOWMINNOACTIVE = 7,
	CS_SW_SHOWNA = 8,
	CS_SW_RESTORE = 9,
	CS_SW_SHOWDEFAULT = 10,
	CS_SW_FORCEMINIMIZE = 11
}CMDSHOW, *LPCMDSHOW;

XLUE_API(const char*) XLUE_GetHostWndID(XLUE_HOSTWND_HANDLE hHostWnd);
XLUE_API(BOOL) XLUE_BindUIObjectTree(XLUE_HOSTWND_HANDLE hHostWnd, XLUE_OBJTREE_HANDLE hObjTree);
XLUE_API(XLUE_OBJTREE_HANDLE) XLUE_UnbindUIObjectTree(XLUE_HOSTWND_HANDLE hHostWnd);
XLUE_API(XLUE_OBJTREE_HANDLE) XLUE_GetBindUIObjectTree(XLUE_HOSTWND_HANDLE hHostWnd);
XLUE_API(BOOL) XLUE_SetUpdateFPS(XLUE_HOSTWND_HANDLE hHostWnd, unsigned long ulFPS);
XLUE_API(unsigned long) XLUE_GetUpdateFPS(XLUE_HOSTWND_HANDLE hHostWnd);
XLUE_API(BOOL) XLUE_SetHostWndVisible(XLUE_HOSTWND_HANDLE hHostWnd, BOOL bVisible);
XLUE_API(BOOL) XLUE_GetHostWndVisible(XLUE_HOSTWND_HANDLE hHostWnd);
XLUE_API(BOOL) XLUE_SetHostWndLayered(XLUE_HOSTWND_HANDLE hHostWnd, BOOL bLayered);
XLUE_API(BOOL) XLUE_GetHostWndLayered(XLUE_HOSTWND_HANDLE hHostWnd);
XLUE_API(BOOL) XLUE_MoveHostWnd(XLUE_HOSTWND_HANDLE hHostWnd, LPCRECT lpRect);
XLUE_API(BOOL) XLUE_GetHostWndRect(XLUE_HOSTWND_HANDLE hHostWnd, LPRECT lpRect);
XLUE_API(BOOL) XLUE_SetHostWndCacheRect(XLUE_HOSTWND_HANDLE hHostWnd, LPCRECT lpRect);
XLUE_API(BOOL) XLUE_GetHostWndCacheRect(XLUE_HOSTWND_HANDLE hHostWnd, LPRECT lpRect);
XLUE_API(BOOL) XLUE_SetHostWndParent(XLUE_HOSTWND_HANDLE hHostWnd, OS_HOSTWND_HANDLE hParentHostWnd);
XLUE_API(OS_HOSTWND_HANDLE) XLUE_GetHostWndParent(XLUE_HOSTWND_HANDLE hHostWnd);
XLUE_API(OS_HOSTWND_HANDLE) XLUE_GetHostWndWindowHandle(XLUE_HOSTWND_HANDLE hHostWnd);
XLUE_API(const char*) XLUE_GetHostWndClassName(XLUE_HOSTWND_HANDLE hHostWnd);
XLUE_API(const char*) XLUE_GetHostWndCursorID(XLUE_HOSTWND_HANDLE hHostWnd);
XLUE_API(void) XLUE_SetHostWndCursorID(XLUE_HOSTWND_HANDLE hHostWnd,const char* pid);
XLUE_API(BOOL) XLUE_SetHostWndTitle(XLUE_HOSTWND_HANDLE hHostWnd, const char* lpTitle);
XLUE_API(const char*) XLUE_GetHostWndTitle(XLUE_HOSTWND_HANDLE hHostWnd);
XLUE_API(BOOL) XLUE_SetHostWndEnable(XLUE_HOSTWND_HANDLE hHostWnd, BOOL bEndable);
XLUE_API(BOOL) XLUE_GetHostWndEnable(XLUE_HOSTWND_HANDLE hHostWnd);
XLUE_API(BOOL) XLUE_SetHostWndTopMost(XLUE_HOSTWND_HANDLE hHostWnd, BOOL bTopMost);
XLUE_API(BOOL) XLUE_GetHostWndTopMost(XLUE_HOSTWND_HANDLE hHostWnd);
XLUE_API(XL_BITMAP_HANDLE) XLUE_GetWindowBitmap(XLUE_HOSTWND_HANDLE hHostWnd);

XLUE_API(BOOL) XLUE_ShowHostWnd(XLUE_NORMAL_HOSTWND_HANDLE hHostWnd, CMDSHOW cmdShow);
XLUE_API(BOOL) XLUE_SetHostWndAppWindow(XLUE_NORMAL_HOSTWND_HANDLE hHostWnd, BOOL bAppWindow);
XLUE_API(BOOL) XLUE_GetHostWndAppWindow(XLUE_NORMAL_HOSTWND_HANDLE hHostWnd);
XLUE_API(BOOL) XLUE_MaxHostWnd(XLUE_NORMAL_HOSTWND_HANDLE hHostWnd);
XLUE_API(BOOL) XLUE_MinHostWnd(XLUE_NORMAL_HOSTWND_HANDLE hHostWnd);
XLUE_API(BOOL) XLUE_RestoreHostWnd(XLUE_NORMAL_HOSTWND_HANDLE hHostWnd);
XLUE_API(BOOL) XLUE_CenterHostWnd(XLUE_NORMAL_HOSTWND_HANDLE hHostWnd, XLUE_HOSTWND_HANDLE hWndCenter);
XLUE_API(BOOL) XLUE_SetHostWndMinTrackSize(XLUE_NORMAL_HOSTWND_HANDLE hHostWnd, const SIZE* lpSize);
XLUE_API(BOOL) XLUE_SetHostWndMaxTrackSize(XLUE_NORMAL_HOSTWND_HANDLE hHostWnd, const SIZE* lpSize);
XLUE_API(BOOL) XLUE_GetHostWndMinTrackSize(XLUE_NORMAL_HOSTWND_HANDLE hHostWnd, LPSIZE lpSize);
XLUE_API(BOOL) XLUE_GetHostWndMaxTrackSize(XLUE_NORMAL_HOSTWND_HANDLE hHostWnd, LPSIZE lpSize);

XLUE_API(int) XLUE_HostWndDoModal(XLUE_MODAL_HOSTWND_HANDLE hHostWnd, OS_HOSTWND_HANDLE hParent);
XLUE_API(BOOL) XLUE_HostWndEndDialog(XLUE_MODAL_HOSTWND_HANDLE hHostWnd, int nRetCode);

XLUE_API(BOOL) XLUE_HostWndCreateWnd(XLUE_FRAME_HOSTWND_HANDLE hHostWnd, OS_HOSTWND_HANDLE hParentWnd);
XLUE_API(BOOL) XLUE_HostWndDestroyWnd(XLUE_FRAME_HOSTWND_HANDLE hHostWnd);

XLUE_API(BOOL) XLUE_DeleteHostWnd(XLUE_HOSTWND_HANDLE hHostWnd);
XLUE_API(XLUE_HOSTWND_HANDLE) XLUE_CreateHostWnd(const char* lpID, const char* lpType);

XLUE_API(BOOL) XLUE_BeginEnumHostWnd();
XLUE_API(XLUE_HOSTWND_HANDLE) XLUE_GetNextHostWnd();

#ifdef XLUE_USE_XAR

typedef void (__stdcall* ASYNLOADXARCALLBACKPROC)(char const*, BOOL);

#define XLUE_LOADSTRATEGY_DOM	0
#define XLUE_LOADSTRATEGY_SAX	1

XLUE_API(long) XLUE_SetLoadStrategy(long strategy);

XLUE_API(long) XLUE_LoadXAR(const char* xarName);
XLUE_API(long) XLUE_AsynLoadXAR(const char* xarName, ASYNLOADXARCALLBACKPROC lpCallBack, int flag);

XLUE_API(long) XLUE_IsXARLoaded(const char* xarName);
XLUE_API(long) XLUE_IsXARInAsynLoading(const char* xarName);

XLUE_API(BOOL) XLUE_XARExist(const char* xarName);
XLUE_API(long) XLUE_UnloadAllXAR();
XLUE_API(long) XLUE_AddXARSearchPath(const wchar_t* xarSearhPath);
XLUE_API(long) XLUE_RemoveXARSearchPath(const wchar_t* xarSearhPath);
XLUE_API(long) XLUE_ClearAllXARSearchPath();
XLUE_API(long) XLUE_LoadXLUEApp(const wchar_t* xlueAppXMLPath);

XLUE_API(long) XLUE_GetBitmap(const char *id, XL_BITMAP_HANDLE *phBitmap);
XLUE_API(long) XLUE_GetFont(const char *id, XL_FONT_HANDLE *phFont);
XLUE_API(long) XLUE_GetTexture(const char *id, XL_TEXTURE_HANDLE *phTexture);
XLUE_API(long) XLUE_GetImageList(const char *id, XL_IMAGELIST_HANDLE *phImageList);
XLUE_API(long) XLUE_GetColor(const char *id, XL_Color *pColor);
XLUE_API(long) XLUE_GetCurve(const char* id, XL_CURVE_HANDLE* phCurve);
XLUE_API(long) XLUE_GetImageSeq(const char* id, XL_IMAGESEQ_HANDLE* lpImageSeq);
XLUE_API(long) XLUE_GetPen(const char *id, XL_PEN_HANDLE *phPen);
XLUE_API(long) XLUE_GetBrush(const char *id, XL_BRUSH_HANDLE *phBrush);

XLUE_API(long) XLUE_SelectResPackage(const char *resPackageName);
#endif //XLUE_USE_XAR

XLUE_API(long) XLUE_GC(void* lpvReserved);
XLUE_API(long) XLUE_Stat(long lType);
XLUE_API(long) XLUE_GetLuaStack(lua_State* luaState,char* lpStackBuffer, int bufferSize);

// hook
// hook type define
#define XLUE_HOOK_TEXTCHANGE 0x01

typedef struct tagXLUE_HOOK_DEF
{
	void* userData;
	XL_HANDLE obj;
	const char* lpObjType;
	const char* lpEventName;

}XLUE_HOOK_DESC;

typedef long (__stdcall* LPFNTEXTCHANGEHOOK)(const XLUE_HOOK_DESC* lpHookDesc,	// 关于该次hook的附加信息
											 wchar_t* lpstrTextBuffer,			// 文本buffer，外部可以动态修改
											 long textLen,						// 文本的长度，也即字符个数
											 long* lpTextBufferLen,				// 文本缓冲区的大小，也即字节数
											 BOOL* lpCallNext);					// 是否调用下一个hook，默认为true

XLUE_API(long) XLUE_SetGlobalHook(unsigned long type, void* lpHookProc, void* userData, unsigned long flags, BOOL hookBack);
XLUE_API(BOOL) XLUE_RemoveGlobalHook(long cookie, void** lplpUserData);

// 在句柄为null或者push失败的情况下，会push一个nil到栈顶
// 如果push成功，对应的句柄会自动增持引用计数
XLUE_API(BOOL) XLUE_PushBitmap(lua_State* luaState, XL_BITMAP_HANDLE hBitmap);
XLUE_API(BOOL) XLUE_PushMask(lua_State* luaState, XL_MASK_HANDLE hMask);
XLUE_API(BOOL) XLUE_PushTexture(lua_State* luaState, XL_TEXTURE_HANDLE hTexture);
XLUE_API(BOOL) XLUE_PushFont(lua_State* luaState, XL_FONT_HANDLE hFont);
XLUE_API(BOOL) XLUE_PushColor(lua_State* luaState, XL_Color* lpColor);
XLUE_API(BOOL) XLUE_PushColor2(lua_State* luaState, XL_Color color);
XLUE_API(BOOL) XLUE_PushPen(lua_State* luaState, XL_PEN_HANDLE hPen);
XLUE_API(BOOL) XLUE_PushBrush(lua_State* luaState, XL_BRUSH_HANDLE hBrush);
XLUE_API(BOOL) XLUE_PushImageList(lua_State* luaState, XL_IMAGELIST_HANDLE hImageList);
XLUE_API(BOOL) XLUE_PushImageSeq(lua_State* luaState, XL_IMAGESEQ_HANDLE hImageSeq);
XLUE_API(BOOL) XLUE_PushCurve(lua_State* luaState, XL_CURVE_HANDLE hCurve);

// 下面几个check函数，如果句柄不为空，那么在用完之后，需要对句柄调用相应的Release(color除外)
XLUE_API(BOOL) XLUE_CheckBitmap(lua_State* luaState, int index, XL_BITMAP_HANDLE *lpBitmap);
XLUE_API(BOOL) XLUE_CheckMask(lua_State* luaState, int index, XL_MASK_HANDLE *lpMask);
XLUE_API(BOOL) XLUE_CheckTexture(lua_State* luaState, int index, XL_TEXTURE_HANDLE *lpTexture);
XLUE_API(BOOL) XLUE_CheckFont(lua_State* luaState, int index, XL_FONT_HANDLE *lpFont);
XLUE_API(BOOL) XLUE_CheckColor(lua_State* luaState, int index, XL_Color** lplpColor);
XLUE_API(BOOL) XLUE_CheckColor2(lua_State* luaState, int index, XL_Color* lpColor);
XLUE_API(BOOL) XLUE_CheckPen(lua_State* luaState, int index, XL_PEN_HANDLE* lpPen);
XLUE_API(BOOL) XLUE_CheckBrush(lua_State* luaState, int index, XL_BRUSH_HANDLE* lpBrush);
XLUE_API(BOOL) XLUE_CheckImageList(lua_State* luaState, int index, XL_IMAGELIST_HANDLE* lpImageList);
XLUE_API(BOOL) XLUE_CheckImageSeq(lua_State* luaState, int index, XL_IMAGESEQ_HANDLE* lpImageSeq);
XLUE_API(BOOL) XLUE_CheckCurve(lua_State* luaState, int index, XL_CURVE_HANDLE* lpCurve);

#endif //_XUNLEI_XLUE_API_H_
