/********************************************************************
*
* =-----------------------------------------------------------------=
* =                                                                 =
* =             Copyright (c) Xunlei, Ltd. 2004-2009                =
* =                                                                 =
* =-----------------------------------------------------------------=
* 
*   FileName    :   XLUEHandle.h
*   Author      :   xlue group(xlue@xunlei.com)
*   Create      :   2009-12-18
*   LastChange  :   2009-12-18
*   History     :	
*
*   Description :   句柄的类型安全的定义
*
********************************************************************/ 
#ifndef __XLUE_HANDLE_H__
#define __XLUE_HANDLE_H__

#define DECLARE_XLUE_HANDLE(name) struct __XLUE_SAFE_HANDLE_##name { int unused; }; typedef struct __XLUE_SAFE_HANDLE_##name *name;

#ifdef __cplusplus
	#define DECLARE_XLUE_HANDLE_EX(name, base) struct __XLUE_SAFE_HANDLE_##name : public __XLUE_SAFE_HANDLE_##base { int n##name; }; typedef __XLUE_SAFE_HANDLE_##name *name;
#else
	#define DECLARE_XLUE_HANDLE_EX(name, base) DECLARE_XLUE_HANDLE(name)
#endif //__cplusplus 

// 句柄类型定义
#define LAYOUTOBJ_TYPECODE	1
#define UIOBJTREE_TYPECODE	2
#define ANIMATION_TYPECODE	3
#define HOSTWND_TYPECODE    4
#define TEMPLATE_TYPECODE   5

DECLARE_XLUE_HANDLE(XL_HANDLE)
DECLARE_XLUE_HANDLE_EX(XLUE_OBJTREE_HANDLE, XL_HANDLE)
DECLARE_XLUE_HANDLE_EX(XLUE_LAYOUTOBJ_HANDLE, XL_HANDLE)
DECLARE_XLUE_HANDLE_EX(XLUE_ANIMATION_HANDLE,XL_HANDLE)

DECLARE_XLUE_HANDLE_EX(XLUE_HOSTWND_HANDLE, XL_HANDLE)
DECLARE_XLUE_HANDLE_EX(XLUE_NORMAL_HOSTWND_HANDLE, XLUE_HOSTWND_HANDLE)
DECLARE_XLUE_HANDLE_EX(XLUE_MODAL_HOSTWND_HANDLE, XLUE_NORMAL_HOSTWND_HANDLE)
DECLARE_XLUE_HANDLE_EX(XLUE_FRAME_HOSTWND_HANDLE, XLUE_NORMAL_HOSTWND_HANDLE)
DECLARE_XLUE_HANDLE_EX(XLUE_TIPS_HOSTWND_HANDLE, XLUE_HOSTWND_HANDLE)
DECLARE_XLUE_HANDLE_EX(XLUE_MENU_HOSTWND_HANDLE, XLUE_HOSTWND_HANDLE)
DECLARE_XLUE_HANDLE_EX(XLUE_DRAGDROP_HOSTWND_HANDLE, XLUE_HOSTWND_HANDLE)

DECLARE_XLUE_HANDLE_EX(XLUE_TEMPLATE_HANDLE, XL_HANDLE)
DECLARE_XLUE_HANDLE_EX(XLUE_HOSTWND_TEMPLATE_HANDLE, XLUE_TEMPLATE_HANDLE)
DECLARE_XLUE_HANDLE_EX(XLUE_OBJECT_TEMPLATE_HANDLE, XLUE_TEMPLATE_HANDLE)
DECLARE_XLUE_HANDLE_EX(XLUE_OBJECTTREE_TEMPLATE_HANDLE, XLUE_TEMPLATE_HANDLE)
DECLARE_XLUE_HANDLE_EX(XLUE_ANIMATION_TEMPLATE_HANDLE, XLUE_TEMPLATE_HANDLE)

#define INVALID_XL_HANDLE NULL

#endif // __XLUE_HANDLE_H__