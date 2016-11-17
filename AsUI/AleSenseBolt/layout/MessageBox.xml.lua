--messagebox的类型
local MSGBOX_TYPE_OK = 1
local MSGBOX_TYPE_YESNO = 2
local MSGBOX_TYPE_ABORTRETRYIGNORE = 3
local MSGBOX_TYPE_YESNOCANCEL = 4
local MSGBOX_TYPE_RETRYCANCEL = 5
local MSGBOX_TYPE_OKCANCEL = 6
--messagebox的返回值
local MSGBOX_RET_NOVAILD = -1
local MSGBOX_RET_IDOK = 1
local MSGBOX_RET_IDCANCEL = 2
local MSGBOX_RET_IDABORT = 3
local MSGBOX_RET_IDRETRY = 4
local MSGBOX_RET_IDIGNORE = 5
local MSGBOX_RET_IDYES = 6
local MSGBOX_RET_IDNO = 7

function MessageBox(notice, title, type, icontype)
	local ret = -1
	local templateManager = XLGetObject("Xunlei.UIEngine.TemplateManager")
	local MsgBoxWndTemplate = templateManager:GetTemplate("MessageBox.Wnd", "HostWndTemplate")
	if not MsgBoxWndTemplate then
		return ret
	end
	local MsgBoxWnd = MsgBoxWndTemplate:CreateInstance("MessageBox.WndInstl")
	if not MsgBoxWnd then
		return ret
	end
	local objectTreeTemplate = templateManager:GetTemplate("MessageBox.Tree", "ObjectTreeTemplate")
	if not objectTreeTemplate then
		return ret
	end
	local objectTree = objectTreeTemplate:CreateInstance("MessageBox.TreeInstl")
	if objectTree then
		MsgBoxWnd:BindUIObjectTree(objectTree)
		InitMessageBox(MsgBoxWnd, notice, title, type, icontype)
		ret = MsgBoxWnd:DoModal()
	end
	return ret
end

function InitMessageBox(wnd, notice, title, type, icontype)
	local tree = wnd:GetBindUIObjectTree()
	if notice then
		local NoticeTextObj = tree:GetUIObject("noticetext")
		NoticeTextObj:SetText(notice)
	end
	
	if title then
		local TitleTextObj = tree:GetUIObject("titletext")
		TitleTextObj:SetText(title)
	end
	
	local BtnObj1 = tree:GetUIObject("button1")
	local BtnObj2 = tree:GetUIObject("button2")
	local BtnObj3 = tree:GetUIObject("button3")
	
	if type == MSGBOX_TYPE_OK then
		BtnObj1:SetVisible(true)
		BtnObj1:SetText("确定")
		BtnObj1:SetDefaultButton(true)
		
	elseif type == MSGBOX_TYPE_YESNO then
		BtnObj1:SetVisible(true)
		BtnObj1:SetText("否")
		BtnObj2:SetVisible(true)
		BtnObj2:SetText("是")
		BtnObj2:SetDefaultButton(true)
	
	elseif type == MSGBOX_TYPE_ABORTRETRYIGNORE then
		BtnObj1:SetVisible(true)
		BtnObj1:SetText("忽略")
		BtnObj2:SetVisible(true)
		BtnObj2:SetText("重试")
		BtnObj3:SetVisible(true)
		BtnObj3:SetText("中止")
		--:SetDefaultButton(true)
	
	elseif type == MSGBOX_TYPE_YESNOCANCEL then
		BtnObj1:SetVisible(true)
		BtnObj1:SetText("取消")
		BtnObj2:SetVisible(true)
		BtnObj2:SetText("否")
		BtnObj3:SetVisible(true)
		BtnObj3:SetText("是")
		--:SetDefaultButton(true)
	
	elseif type == MSGBOX_TYPE_RETRYCANCEL then
		BtnObj1:SetVisible(true)
		BtnObj1:SetText("取消")
		BtnObj2:SetVisible(true)
		BtnObj2:SetText("重试")
		--BtnObj2:SetDefaultButton(true)
	
	elseif type == MSGBOX_TYPE_OKCANCEL then
		BtnObj1:SetVisible(true)
		BtnObj1:SetText("取消")
		BtnObj2:SetVisible(true)
		BtnObj2:SetText("确定")
		BtnObj1:SetDefaultButton(true)
		BtnObj2:SetDefaultButton(false)
	end

	local CloseButton =  tree:GetUIObject("closebtn")
	CloseButton:SetDefaultButton(false)
end

function Button_OnClick(BtnObj, eventname)
	local text = BtnObj:GetText()
	local tree = BtnObj:GetOwner()
	local HostWnd = tree:GetBindHostWnd()
	if text == "确定" then
		return EndMessageBox(HostWnd, MSGBOX_RET_IDOK)
		
	elseif text == "取消" then
		return EndMessageBox(HostWnd, MSGBOX_RET_IDCANCEL)
	
	elseif text == "重试" then
		return EndMessageBox(HostWnd, MSGBOX_RET_IDRETRY)
	
	elseif text == "是" then
		return EndMessageBox(HostWnd, MSGBOX_RET_IDYES)
	
	elseif text == "否" then
		return EndMessageBox(HostWnd, MSGBOX_RET_IDNO)
	
	elseif text == "中止" then
		return EndMessageBox(HostWnd, MSGBOX_RET_IDABORT)
	
	elseif text == "忽略" then
		return EndMessageBox(HostWnd, MSGBOX_RET_IDIGNORE)
	end
	
	return EndMessageBox(HostWnd, MSGBOX_RET_NOVAILD)
end

------------------------------------------------------------
----------窗口函数
function OnCreate(wnd)
	wnd:Center()
end

function OnDestroy(wnd)
	local hostwndManager = XLGetObject("Xunlei.UIEngine.HostWndManager")
	local objtreeManager = XLGetObject("Xunlei.UIEngine.TreeManager")
	local tree = wnd:UnbindUIObjectTree()
	objtreeManager:DestroyTree(tree:GetID())
	hostwndManager:RemoveHostWnd(wnd:GetID())
end

function OnShowWindow(wnd, bshow)
	local tree = wnd:GetBindUIObjectTree()
	if bshow then
		local RootObj = tree:GetRootObject()
		local xlgraphic = XLGetObject("Xunlei.XLGraphic.Factory.Object")
		local render = XLGetObject("Xunlei.UIEngine.RenderFactory")
		local left, top, right, bottom = RootObj:GetObjPos()
		local width, height = right - left, bottom - top
		local theBitmap = xlgraphic:CreateBitmap("ARGB32", width, height)
		render:RenderObject(RootObj, theBitmap)
		
		local ShadowObj = tree:GetUIObject("background")
		ShadowObj:SetVisible(false)
		ShadowObj:SetChildrenVisible(false)
		
		local objFactory = XLGetObject("Xunlei.UIEngine.ObjectFactory")
		local AniBitmapObj = objFactory:CreateUIObject("", "ImageObject")
		RootObj:AddChild(AniBitmapObj)
		AniBitmapObj:SetObjPos(0, 0, width, height)
		AniBitmapObj:SetBitmap(theBitmap)
		-- 一定要是拉伸的
		AniBitmapObj:SetDrawMode(1)
		
		local AniFactory = XLGetObject("Xunlei.UIEngine.AnimationFactory")
        local AlphaAni = AniFactory:CreateAnimation("AlphaChangeAnimation")
		AlphaAni:BindObj(AniBitmapObj)
		AlphaAni:SetTotalTime(300)
		AlphaAni:SetKeyFrameAlpha(0, 255)
		tree:AddAnimation(AlphaAni)
		
		local PosAni = AniFactory:CreateAnimation("PosChangeAnimation")
		PosAni:BindObj(AniBitmapObj)
		PosAni:SetTotalTime(300)
		PosAni:SetKeyFrameRect(10, 5, width - 10, height - 5, -10, -5, width + 10, height + 5)
		tree:AddAnimation(PosAni)
		
		local cookie = AlphaAni:AttachListener(true, 
			function (ani, oldState, newState)
				if newState == 4 then
					RootObj:RemoveChild(AniBitmapObj)
					ShadowObj:SetVisible(true)
					ShadowObj:SetChildrenVisible(true)
				end
			end)
		PosAni:Resume()
		AlphaAni:Resume()
	end
end

function CloseBtn_OnClick(BtnObj)
	local tree = BtnObj:GetOwner()
	local wnd = tree:GetBindHostWnd()
	EndMessageBox(wnd, MSGBOX_RET_NOVAILD)
end

function EndMessageBox(wnd, ret)
	local tree = wnd:GetBindUIObjectTree()
	local RootObj = tree:GetRootObject()
	local xlgraphic = XLGetObject("Xunlei.XLGraphic.Factory.Object")
	local render = XLGetObject("Xunlei.UIEngine.RenderFactory")
	local left, top, right, bottom = RootObj:GetObjPos()
	local width, height = right - left, bottom - top
	local theBitmap = xlgraphic:CreateBitmap("ARGB32", width, height)
	render:RenderObject(RootObj, theBitmap)
	
	local ShadowObj = tree:GetUIObject("background")
	ShadowObj:SetVisible(false)
	ShadowObj:SetChildrenVisible(false)
	
	local objFactory = XLGetObject("Xunlei.UIEngine.ObjectFactory")
	local AniBitmapObj = objFactory:CreateUIObject("", "ImageObject")
	RootObj:AddChild(AniBitmapObj)
	AniBitmapObj:SetObjPos(0, 0, width, height)
	AniBitmapObj:SetBitmap(theBitmap)
	-- 一定要是拉伸的
	AniBitmapObj:SetDrawMode(1)
	
	local AniFactory = XLGetObject("Xunlei.UIEngine.AnimationFactory")
	local AlphaAni = AniFactory:CreateAnimation("AlphaChangeAnimation")
	AlphaAni:BindObj(AniBitmapObj)
	AlphaAni:SetTotalTime(300)
	AlphaAni:SetKeyFrameAlpha(255, 0)
	tree:AddAnimation(AlphaAni)
	
	local PosAni = AniFactory:CreateAnimation("PosChangeAnimation")
	PosAni:BindObj(AniBitmapObj)
	PosAni:SetTotalTime(300)
	--PosAni:SetKeyFrameRect(10, 5, width - 10, height - 5, -10, -5, width + 10, height + 5)
	PosAni:SetKeyFrameRect(-10, -5, width + 10, height + 5, 10, 5, width - 10, height - 5)
	tree:AddAnimation(PosAni)
	
	local cookie = AlphaAni:AttachListener(true, 
		function (ani, oldState, newState)
			if newState == 4 then
				RootObj:RemoveChild(AniBitmapObj)
				ShadowObj:SetVisible(true)
				ShadowObj:SetChildrenVisible(true)
				wnd:EndDialog(ret)
			end
		end)
	PosAni:Resume()
	AlphaAni:Resume()
end

------窗口对象树事件
function OnInitControl(rootObj)

end

function OnKeyDown(root ,uKey, uRepeatCount, uFlags)
	local tree = root:GetOwner()
	local wnd = tree:GetBindHostWnd()
	
	if uKey == 27 then --ESC按键
		EndMessageBox(wnd, MSGBOX_RET_NOVAILD)
	end
end