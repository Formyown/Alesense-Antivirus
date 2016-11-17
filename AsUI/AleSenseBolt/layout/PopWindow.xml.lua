function PopWindow(X,Y,type,id,defaultBtnText,otherBtnText,isRightDefault,isTick,text1,text2,text3,text4)
	local templateManager = XLGetObject("Xunlei.UIEngine.TemplateManager")
	local PopWndTemplate = templateManager:GetTemplate("As.PopWin", "HostWndTemplate")
	if not PopWndTemplate then
		return
	end
	local PopWnd = PopWndTemplate:CreateInstance("As.Popwin.WndInstl")
	if not PopWnd then
		return
	end
	local objectTreeTemplate = templateManager:GetTemplate("As.Popwin.Tree", "ObjectTreeTemplate")
	if not objectTreeTemplate then
		return
	end
	local objectTree = objectTreeTemplate:CreateInstance("As.Popwin.TreeInstl")
	if objectTree then
		PopWnd:BindUIObjectTree(objectTree)
		PopWnd:Create()
		--InitMessageBox(PopWnd, notice, title, type, icontype)
		this = PopWnd
		InitPopWin(X,Y,PopWnd,type,id,defaultBtnText,otherBtnText,isRightDefault,isTick,text1,text2,text3,text4)
	end
end
function GetShortPathForTextObj(path,textobj)
	CalcTextObj:SetText(path)
 		local iTextWidth, iTextHeight = CalcTextObj:GetTextExtent()	
    	if iTextWidth > 370 then
    		local l 
    		l,_ = string.find(path,'\\')
    		local str1 = string.sub(path,1,l)
    		local str2 = string.match(path, ".+(\\.-%..+)")  -- 最后文件名
    		if str2 == nil or str1 == nil then
    			textobj:SetText(path)
    			return
    		end
    		CalcTextObj:SetText(str1.."..."..str2)
 			iTextWidth, iTextHeight = CalcTextObj:GetTextExtent()	
    		while iTextWidth < 350 do
    			l = l + 3
    			str1 = string.sub(path,1,l)
    			CalcTextObj:SetText(str1.."..."..str2)
    			iTextWidth, iTextHeight = CalcTextObj:GetTextExtent()	
    		end
    			textobj:SetText(str1.."..."..str2);
        else
        		textobj:SetText(path)
    	end
end
function InitPopWin(X,Y,wnd,type,id,defaultBtnText,otherBtnText,isRightDefault,isTick,text1,text2,text3,text4)
	local tree = wnd:GetBindUIObjectTree()
	this:Move(X, Y, 460,260)
	Id = id

	local BkgImg = tree:GetUIObject("BackGroundImg")
	local TextObj1 = tree:GetUIObject("TextObj1")
	local TextObj2 = tree:GetUIObject("TextObj2")
	local TextObj3 = tree:GetUIObject("TextObj3")
	local TextObj4 = tree:GetUIObject("TextObj4")
	local LeftBtn = tree:GetUIObject("LeftBtn")
	local RightBtn = tree:GetUIObject("RightBtn")
	GIsRightDefault = isRightDefault
	CalcTextObj = tree:GetUIObject("CalcTextObj")
	if type == "warning" then
		BkgImg:SetResID("Res.PopWin.Bkg.Warning")
	else
		BkgImg:SetResID("Res.PopWin.Bkg.Info")
	end

	if text1 ~= nil then
	TextObj1:SetText(text1)
	end
	if text2 ~= nil then
	GetShortPathForTextObj(text2,TextObj2)
	end
	if text3 ~= nil then
	GetShortPathForTextObj(text3,TextObj3)
	end
	if text4 ~= nil then
	TextObj4:SetText(text4)
	end
	if isRightDefault == true then
		RightBtn:SetText(defaultBtnText)
		RightBtn:SetDefaultButton(false)
		if otherBtnText ~= nil then
			LeftBtn:SetText(otherBtnText)
			LeftBtn:SetDefaultButton(false)
		else
			LeftBtn:SetChildrenVisible(false)

		end
		if isTick == true then
			StartTick(RightBtn)
		end
	
	else
		LeftBtn:SetText(defaultBtnText)
		RightBtn:SetText(otherBtnText)
		if isTick == true then
			StartTick(LeftBtn)

		end
	end

end
function StartTick(btnobj)
	local BtnText = btnobj:GetText()
	local AsControl = XLGetObject("As.Control")
	local TimerManager = XLGetObject("Xunlei.UIEngine.TimerManager")
	local Time = 15
	
	Timer = TimerManager:SetTimer(
		function ()
			if Time > 0 then
				Time = Time - 1
				btnobj:SetText(BtnText.."("..Time.."s)")
			else
				TimerManager:KillTimer(Timer)
				AsControl:InformControl(20,Id,true)

				this:Destroy()
			end
			
		end,1000)
end
function OnLeftButtonClick(button)
	local AsControl = XLGetObject("As.Control")

	if GIsRightDefault == true then
		AsControl:InformControl(20,Id,false)
	else
		AsControl:InformControl(20,Id,true)
	end
	this:Destroy()
end
function OnRightButtonClick(button)
	local AsControl = XLGetObject("As.Control")
	
	if GIsRightDefault == true then
		AsControl:InformControl(20,Id,true)
	else
		AsControl:InformControl(20,Id,false)
	end
	this:Destroy()
end
function OnDestroy(wnd)
	local TimerManager = XLGetObject("Xunlei.UIEngine.TimerManager")
	TimerManager:KillTimer(Timer)
	local hostwndManager = XLGetObject("Xunlei.UIEngine.HostWndManager")
	local objtreeManager = XLGetObject("Xunlei.UIEngine.TreeManager")
	local tree = wnd:UnbindUIObjectTree()
	objtreeManager:DestroyTree(tree:GetID())
	hostwndManager:RemoveHostWnd(wnd:GetID())
end