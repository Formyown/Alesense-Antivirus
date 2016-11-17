local tipHostWnd_OnCreate_Cookie = nil
local tipHostWnd_OnCreate_Cookie_CtrlID = nil
local function GetTipHostWnd(BtnObj)
	local btnAttr = BtnObj:GetAttribute()
	-- 给的tipTemplate一定要有一个TextObject,ID "Text"
	local function OnCreate(tipWnd)
		local tipTree = tipWnd:GetBindUIObjectTree()
		if not tipTree then
			return 
		end
		local textObj = tipTree:GetUIObject("text")
		if textObj and btnAttr.TipText then
			textObj:SetText(btnAttr.TipText)
		end
	end
	
	local hostWndManager = XLGetObject("Xunlei.UIEngine.HostWndManager")
	local tipsHostWnd = hostWndManager:GetHostWnd("UIBtn.TipWnd.Instance")
	if not tipsHostWnd then
		local templateMananger = XLGetObject("Xunlei.UIEngine.TemplateManager")
		local tipsHostWndTemplate = templateMananger:GetTemplate("UIBtn.TipWnd","HostWndTemplate")
		tipsHostWnd = tipsHostWndTemplate:CreateInstance("UIBtn.TipWnd.Instance")	
	end
	
	if not tipHostWnd_OnCreate_Cookie_CtrlID then
	    tipHostWnd_OnCreate_Cookie = tipsHostWnd:AttachListener("OnCreate", true, OnCreate)
	    tipHostWnd_OnCreate_Cookie_CtrlID = BtnObj:GetID()
	else
	    if tipHostWnd_OnCreate_Cookie_CtrlID ~= BtnObj:GetID() then
	         tipsHostWnd:RemoveListener("OnCreate", tipHostWnd_OnCreate_Cookie)
	         tipHostWnd_OnCreate_Cookie = tipsHostWnd:AttachListener("OnCreate", true, OnCreate)
	         tipHostWnd_OnCreate_Cookie_CtrlID = BtnObj:GetID()
	    end
	end
	
	if not tipsHostWnd:GetParent() then
		local tree = BtnObj:GetOwner()
		local hostwnd = tree:GetBindHostWnd();
		if hostwnd then
			--tipsHostWnd:SetParent(hostwnd:GetWndHandle())
		end
	end
	
	return tipsHostWnd
end

-- 移除默认按钮动画
local function RemoveDefaultAnimation(BtnObj)
    local btnAttr = BtnObj:GetAttribute()
    if btnAttr.DefaultBtnAni then
        btnAttr.DefaultBtnAni:Stop()
		btnAttr.DefaultBtnAni = nil
    end
end

-- 默认按钮动画
local function SetDefaultAnimation(BtnObj)
	local btnAttr = BtnObj:GetAttribute()
	if btnAttr.IsDefaultBtn and btnAttr.NowState == "normal" and BtnObj:GetVisible() then
		-- 设置焦点,当然如果在同一层有多少btn,你最好只设置其中一个为默认
		BtnObj:SetFocus(true)
		if btnAttr.DefaultBkgNormal then
			local BkgObj = BtnObj:GetControlObject("bkg")
			BkgObj:SetTextureID(btnAttr.DefaultBkgNormal)
		end
		if not btnAttr.DefaultBtnAni then
			local tree = BtnObj:GetOwner()
			local aniFactory = XLGetObject("Xunlei.UIEngine.AnimationFactory")
			local AniObj = aniFactory:CreateAnimation("ComAni.Button.DefaultChange")
			AniObj:BindObj(BtnObj)
			tree:AddAnimation(AniObj)
			btnAttr.DefaultBtnAni = AniObj
			
			AniObj:AttachListener(true,
				function (AniObj, oldState, newState)
					if newState == 4 then
						btnAttr.DefaultBtnAni = nil
					end
				end)
			
			AniObj:Resume()
		end
	else
		BtnObj:SetFocus(false)
		if btnAttr.DefaultBtnAni then
            RemoveDefaultAnimation(BtnObj)
        end
	end
end
--调整子控件的位置
local function AdjustBtnChildPos(BtnObj)
	local btnAttr = BtnObj:GetAttribute()
	local btnLeft, btnTop, btnRight, btnBottom = BtnObj:GetObjPos()
	local btnWidth, btnHeight = btnRight - btnLeft, btnBottom - btnTop
	
	if btnAttr.TextObj then
		local textLeft, textTop, textWidth, textHeight = btnAttr.TextLeftPos, btnAttr.TextTopPos, btnAttr.TextWidth, btnAttr.TextHeight
		if not textWidth then
			textWidth = btnWidth - textLeft
		end
		
		if not textHeight then
			textHeight = btnHeight - textTop
		end
		
		if btnAttr.IsDownModifyPos and btnAttr.NowState == "down" then 
			textLeft = textLeft + 2
			textTop = textTop + 2
		end
		
		btnAttr.TextObj:SetObjPos2(textLeft, textTop, textWidth, textHeight)
	end
	
	if btnAttr.IconObj then
		local iconLeft, iconTop = btnAttr.IconLeftPos, btnAttr.IconTopPos
		local iconWidth, iconHeight = btnAttr.IconWidth, btnAttr.IconHeight
		if not iconWidth then
			iconWidth = btnWidth
		end
		if not iconHeight then
			iconHeight = btnHeight
		end
		btnAttr.IconObj:SetObjPos2( iconLeft, iconTop, iconWidth, iconHeight )
	end
	
	if btnAttr.FocusRectObj then
		local focusRectLeft, focusRectTop, focusRectWidth, focusRectHeight = btnAttr.FocusRectLeftPos, btnAttr.FocusRectTopPos, btnAttr.FocusRectWidth, btnAttr.FocusRectHeight
		if not focusRectWidth then
			focusRectWidth = btnWidth - focusRectLeft
		end
		
		if not focusRectHeight then
			focusRectHeight = btnHeight - textTop
		end
		btnAttr.FocusRectObj:SetObjPos2(focusRectLeft, focusRectTop, focusRectWidth, focusRectHeight)
	end
end
-- 创建并初始化Text
local function UIButton_CreateText(BtnObj)
	local btnAttr = BtnObj:GetAttribute()
	local BkgObj = BtnObj:GetControlObject("bkg")
	
	if not btnAttr.TextObj then
		local objFactory = XLGetObject("Xunlei.UIEngine.ObjectFactory")
		btnAttr.TextObj = objFactory:CreateUIObject("text", "TextObject")
		BkgObj:AddChild(btnAttr.TextObj)

		btnAttr.TextObj:SetHAlign(btnAttr.TextHAlign)
		btnAttr.TextObj:SetVAlign(btnAttr.TextVAlign)
	end
end

-- 创建并初始化Text
local function UIButton_CreateIcon(BtnObj)
	local btnAttr = BtnObj:GetAttribute()
	local BkgObj = BtnObj:GetControlObject("bkg")
	
	if not btnAttr.IconObj then
		local objFactory = XLGetObject("Xunlei.UIEngine.ObjectFactory")
		btnAttr.IconObj = objFactory:CreateUIObject("icon", "ImageObject")
		BkgObj:AddChild(btnAttr.IconObj)

		btnAttr.IconObj:SetHAlign(btnAttr.IconHAlign)
		btnAttr.IconObj:SetVAlign(btnAttr.IconVAlign)
	end
end

-- 创建并初始化FocusRect
local function UIButton_CreateFocusRect(BtnObj)
	local btnAttr = BtnObj:GetAttribute()
	local BkgObj = BtnObj:GetControlObject("bkg")
	
	if not btnAttr.FocusRectObj then
		local objFactory = XLGetObject("Xunlei.UIEngine.ObjectFactory")
		btnAttr.FocusRectObj = objFactory:CreateUIObject("text", "TextObject")
		BkgObj:AddChild(btnAttr.FocusRectObj)
		-- 设置FocusRectObj的z序，可以比text高，要不低，但不要相等(100),而且一定要比oldbkg高(oldbkg 我设成10)
		-- 当然如果你用oldbkg作为它的父就可以不管 oldbkg的zorder了，
		btnAttr.FocusRectObj:SetZorder(150)
		btnAttr.FocusRectObj:SetLinePenResID("pen.dot")
		btnAttr.FocusRectObj:SetLineColorResID("system.black")
		btnAttr.FocusRectObj:SetRectangleBrushResID("brush.null")
		btnAttr.FocusRectObj:SetRectanglePoint(0, 0, "father.width", "father.height")
	end	
end
-- 初始化控件，一般在重置控件属性时调用
local function InitCtrlByAttr(BtnObj)
	local btnAttr = BtnObj:GetAttribute()
	
	if btnAttr.TipTemplate then
		BtnObj:SetTipTemplate(btnAttr.TipTemplate)
	end
	
	if btnAttr.Text then
		if not btnAttr.TextObj then
			UIButton_CreateText(BtnObj)
		end
		-- 不要用下面的判断，因为自身的可视性会受到父控件的影响(判断可见时没问题，但不可见时会有问题)
		--if not btnAttr.TextObj:GetVisible() then
		btnAttr.TextObj:SetVisible(true)
	else
		-- 不要用下面的判断，因为自身的可视性会受到父控件的影响(判断可见时没问题，但不可见时会有问题)
		if btnAttr.TextObj then --and  btnAttr.TextObj:GetVisible() then
			btnAttr.TextObj:SetVisible(false)
		end
	end
	--设置TextObj相关的自定义属性，防止控件重用时没有初始化
	if btnAttr.TextObj then
		btnAttr.TextObj:SetText(btnAttr.Text)
		btnAttr.TextObj:SetHAlign(btnAttr.TextHAlign)
		btnAttr.TextObj:SetVAlign(btnAttr.TextVAlign)
	end
	
	if btnAttr.IconNormalImageID then
		if not btnAttr.IconObj then
			UIButton_CreateIcon(BtnObj)
		end
		btnAttr.IconObj:SetVisible(true)
	else
		if btnAttr.IconObj then
			btnAttr.IconObj:SetVisible(false)
		end
	end
	--设置IconObj相关的自定义属性，防止控件重用时没有初始化
	if btnAttr.IconObj then
		btnAttr.IconObj:SetHAlign(btnAttr.TextHAlign)
		btnAttr.IconObj:SetVAlign(btnAttr.TextVAlign)
	end
	
	if btnAttr.BShowFocusRect then
		if not btnAttr.FocusRectObj then
			UIButton_CreateFocusRect(BtnObj)
		end
		--可视性同text
		btnAttr.FocusRectObj:SetVisible(true)
	else
		--可视性同text
		if btnAttr.FocusRectObj then
			btnAttr.FocusRectObj:SetVisible(false)
		end
	end
	
	--因为没有定义FocusRectObj相关的自定义属性，所以这里也就不用初始化了，而text有相关属性
	--要加的话就取消下面的注释
	--[[btnAttr.FocusRectObj:SetLinePenResID("pen.dot")
	btnAttr.FocusRectObj:SetLineColorResID("system.black")
	btnAttr.FocusRectObj:SetRectangleBrushResID("brush.null")
	btnAttr.FocusRectObj:SetRectanglePoint(0, 0, "father.width", "father.height")--]]
	
	AdjustBtnChildPos(BtnObj);
	UIButton_SetState(BtnObj, btnAttr.NowState, true, true)
end
------------------------------------------外部事件------------------------------
-- 设置控件状态
-- newState:新状态
-- bForce:是否强制更新状态
-- noAni:没有切换动画
function UIButton_SetState(BtnObj, newState, bForce, noAni)
	local btnAttr = BtnObj:GetAttribute()
	
	if bForce or newState ~= btnAttr.NowState then
		local bkgTexture, textFont, textColor, iconimage
		
		if btnAttr.NowState ~= newState then
			btnAttr.NowState = newState
		end
		
		if newState == "hover" then
			bkgTexture = btnAttr.BkgHoverTextureID
			textFont = btnAttr.TextHoverFontID
			textColor = btnAttr.TextHoverColorID
			iconimage = btnAttr.IconHoverImageID
			
		elseif newState == "down" then
			bkgTexture = btnAttr.BkgDownTextureID
			textFont = btnAttr.TextDownFontID
			textColor = btnAttr.TextDownColorID
			iconimage = btnAttr.IconDownImageID
			
		elseif newState == "disable" then
			bkgTexture = btnAttr.BkgDisableTextureID
			textFont = btnAttr.TextDisableFontID
			textColor = btnAttr.TextDisableColorID
			iconimage = btnAttr.IconDisalbeImageID
		
		else
			if btnAttr.NowState ~= "normal" then
				btnAttr.NowState = "normal"
			end
			bkgTexture = btnAttr.BkgNormalTextureID
			textFont = btnAttr.TextNormalFontID
			textColor = btnAttr.TextNormalColorID
			iconimage = btnAttr.IconNormalImageID

		end
	
		if btnAttr.TextObj then
			if textFont then
				btnAttr.TextObj:SetTextFontResID(textFont)
			end
			
			if textColor then
				btnAttr.TextObj:SetTextColorResID(textColor)

			end		
			
			if btnAttr.IsDownModifyPos then
				local textLeft, textTop, textRight, textBottom = btnAttr.TextObj:GetObjPos()
				local textWidth, textHeight = textRight - textLeft, textBottom - textTop
				if newState ~= "down" then
					if (textLeft ~= btnAttr.TextLeftPos) or (textTop ~= btnAttr.TextTopPos) then
						btnAttr.TextObj:SetObjPos2(btnAttr.TextLeftPos, btnAttr.TextTopPos, textWidth, textHeight)
					end
				else
					if (textLeft ~= btnAttr.TextLeftPos + 2) or (textTop ~= btnAttr.TextTopPos + 2) then
						btnAttr.TextObj:SetObjPos2(btnAttr.TextLeftPos + 2, btnAttr.TextTopPos + 2, textWidth, textHeight)
					end
				end
			end
		end
		
		if btnAttr.IconObj and iconimage then
			btnAttr.IconObj:SetResID(iconimage)
		end
		
		if bkgTexture then
			local BkgObj = BtnObj:GetControlObject("bkg")
			local OldBkgObj = BtnObj:GetControlObject("oldbkg")
			local oldBkgTexture = BkgObj:GetTextureID()
			BkgObj:SetTextureID(bkgTexture)
			
			if (oldBkgTexture ~= bkgTexture) and (not noAni) then
				
				local aniFactory = XLGetObject("Xunlei.UIEngine.AnimationFactory")
				if btnAttr.AniAlpha then
					OldBkgObj:SetAlpha(0)
					btnAttr.AniAlpha:Stop()
					btnAttr.AniAlpha = nil
				end	
				OldBkgObj:SetTextureID(oldBkgTexture)
				OldBkgObj:SetAlpha(255)
				
				local aniAlpha = aniFactory:CreateAnimation("AlphaChangeAnimation")
				aniAlpha:BindObj(OldBkgObj)
				aniAlpha:SetTotalTime(200)
				aniAlpha:SetKeyFrameAlpha(255,0)
				
				local tree = BtnObj:GetOwner()
				tree:AddAnimation(aniAlpha)
				
				btnAttr.AniAlpha = aniAlpha
				aniAlpha:AttachListener(true,
					function (aniAlpha, oldState, newState)
						if newState == 4 then
							btnAttr.AniAlpha = nil
						end
					end)
				btnAttr.AniAlpha:Resume()
				
				--return 
			end
		end
		
		if btnAttr.IsDefaultBtn then
			--默认按钮动画和透明动画同时存在影响不大，当然你介意的话自己改下 206的return 不注释在透明动画结束的时候开始默认动画
			SetDefaultAnimation(BtnObj)
		end
	end
end
-- 得到控件状态
function UIButton_GetState(BtnObj)
	local btnAttr = BtnObj:GetAttribute()
	
	return btnAttr.NowState
end
-- 控件复位后最好是把控件用在和以前一样的环境里，不然可能出问题（建议就不要复用控件了）
function UIButton_Reset(BtnObj)
	local btnAttr = BtnObj:GetAttribute()
	
	local initAttr = btnAttr.InitAttr
	--因为TextObj和FocusRectObj是动态生成，为了控件重用所以要先保存控件
	local TextObj = btnAttr.TextObj
	local FocusRectObj = btnAttr.FocusRectObj
	local IconObj = btnAttr.IconObj
	
	for key, value in pairs(btnAttr) do
		if not initAttr[key] then
			btnAttr[key] = nil
		else
			btnAttr[key] = initAttr[key]
		end
	end
	
	btnAttr.TextObj = TextObj
	btnAttr.FocusRectObj = FocusRectObj
	btnAttr.IconObj = IconObj
	
	btnAttr.InitAttr = initAttr
	
	BtnObj:SetVisible(btnAttr.BVisible)
	BtnObj:SetEnable(btnAttr.BEnable)
	InitCtrlByAttr(BtnObj)
end

function UIButton_SetDefaultButton(BtnObj, bDefault)
	local btnAttr = BtnObj:GetAttribute()
	UIButton_SetState(BtnObj,"normal",true,true)
	if bDefault == true then
		UIButton_SetState(BtnObj,"hover",true,true)
	end
	if btnAttr.IsDefaultBtn == bDefault then
		return 
	end
	
	btnAttr.IsDefaultBtn = bDefault
	SetDefaultAnimation(BtnObj)
end

function UIButton_SetText(BtnObj, text)
	if not text then
		return 
	end
	local btnAttr = BtnObj:GetAttribute()
	if not btnAttr.TextObj then
		UIButton_CreateText(BtnObj)
		AdjustBtnChildPos(BtnObj)
	end
	btnAttr.Text = text
		
	btnAttr.TextObj:SetText(text)
end

function UIButton_GetText(BtnObj)
	local btnAttr = BtnObj:GetAttribute()
	
	return btnAttr.Text
end
-- 这个因为我使用的是不等于就赋值，所以如果只想改变四个中的某一个值，
-- 要先使用GetBkgResID得到四态ID后再改
function UIButton_SetBkgResID(BtnObj, normal, hover, down, disable)
	local btnAttr = BtnObj:GetAttribute()
	
	if normal ~= btnAttr.BkgNormalTextureID then
		btnAttr.BkgNormalTextureID = normal
	end
	
	if hover ~= btnAttr.BkgHoverTextureID then
		btnAttr.BkgHoverTextureID = hover
	end
	
	if down ~= btnAttr.BkgDownTextureID then
		btnAttr.BkgDownTextureID = down
	end
	
	if disable ~= btnAttr.BkgDisableTextureID then
		btnAttr.BkgDisableTextureID = disable
	end
	
	BtnObj:SetState(btnAttr.NowState, true, true)
end

function UIButton_GetBkgResID(BtnObj)
	local btnAttr = BtnObj:GetAttribute()
	
	return btnAttr.BkgNormalTextureID, btnAttr.BkgHoverTextureID,
			btnAttr.BkgDownTextureID, btnAttr.BkgDisableTextureID
end
-- 这个因为我使用的是不等于就赋值，所以如果只想改变四个中的某一个值，
-- 要先使用UIButton_GetTextFont得到四态ID后再改
function UIButton_SetTextFont(BtnObj, normal, hover, down, disable)
	local btnAttr = BtnObj:GetAttribute()
	
	if normal ~= btnAttr.TextNormalFontID then
		btnAttr.TextNormalFontID = normal
	end
	
	if hover ~= btnAttr.TextHoverFontID then
		btnAttr.TextHoverFontID = hover
	end
	
	if down ~= btnAttr.TextDownFontID then
		btnAttr.TextDownFontID = down
	end
	
	if disable ~= btnAttr.TextDisableFontID then
		btnAttr.TextDisableFontID = disable
	end
	
	BtnObj:SetState(btnAttr.NowState, true, true)
end

function UIButton_GetTextFont(BtnObj)
	local btnAttr = BtnObj:GetAttribute()
	
	return btnAttr.TextNormalFontID, btnAttr.TextHoverFontID,
			btnAttr.TextDownFontID, btnAttr.TextDisableFontID
end
-- 这个因为我使用的是不等于就赋值，所以如果只想改变四个中的某一个值，
-- 要先使用UIButton_GetTextColor得到四态ID后再改
function UIButton_SetTextColor(BtnObj, normal, hover, down, disable)
	local btnAttr = BtnObj:GetAttribute()
	
	if normal ~= btnAttr.TextNormalColorID then
		btnAttr.TextNormalColorID = normal
	end
	
	if hover ~= btnAttr.TextHoverColorID then
		btnAttr.TextHoverColorID = hover
	end
	
	if down ~= btnAttr.TextDownColorID then
		btnAttr.TextDownColorID = down
	end
	
	if disable ~= btnAttr.TextDisableColorID then
		btnAttr.TextDisableColorID = disable
	end
	
	BtnObj:SetState(btnAttr.NowState, true, true)
end

function UIButton_GetTextColor(BtnObj)
	local btnAttr = BtnObj:GetAttribute()
	
	return btnAttr.TextNormalColorID, btnAttr.TextHoverColorID,
			btnAttr.TextDownColorID, btnAttr.TextDisableColorID
end

function UIButton_GetTextExtent(BtnObj)
	local btnAttr = BtnObj:GetAttribute()
	if btnAttr.TextObj then
		return btnAttr.TextObj:GetTextExtent()
	end
end
-- left, top我上面没有作处理，所以限制不能为nil,width,height可以
-- 			
function UIButton_SetTextPos(BtnObj, left, top, width, height)
	local btnAttr = BtnObj:GetAttribute()
	
	if left and left ~= btnAttr.TextLeftPos then
		btnAttr.TextLeftPos = left
	end
	
	if top and top ~= btnAttr.TextTopPos then
		btnAttr.TextTopPos = top
	end
	
	if width ~= btnAttr.TextWidth then
		btnAttr.TextWidth = width
	end
	
	if height ~= btnAttr.TextHeight then
		btnAttr.TextHeight = height
	end
	
	--用adujst更新，因为它会考虑到IsDownModifyPos属性
	AdjustBtnChildPos(BtnObj)
	--btnAttr.TextObj:SetObjPos2(left, top, width, height)
end

function UIButton_SetTipText(BtnObj, text)	
	local btnAttr = BtnObj:GetAttribute()
	btnAttr.TipText = text
end

function UIButton_GetTipText(BtnObj)
	local btnAttr = BtnObj:GetAttribute()
	
	return btnAttr.TipText
end

function UIButton_SetTipTemplate(BtnObj, template)
	local btnAttr = BtnObj:GetAttribute()
	btnAttr.TipTemplate = template
	
	local tipHostWnd = GetTipHostWnd(BtnObj)
	tipHostWnd:SetTipTemplate(btnAttr.TipTemplate)
end

function UIButton_GetTipTemplate(BtnObj)
	local btnAttr = BtnObj:GetAttribute()
	
	return btnAttr.TipTemplate
end
------------------------------------------------按钮事件--------------------------------
function UIButton_OnBind(BtnObj)
	local btnAttr = BtnObj:GetAttribute()
	-- 保存控件初始化时属性，
	local initAttr = {}
	for key, value in pairs(btnAttr) do
		initAttr[key] = value
	end
	-- 保存控件可视性与有效性，
	initAttr.BVisible = BtnObj:GetVisible()
	initAttr.BEnable = BtnObj:GetEnable()
	
	btnAttr.InitAttr = initAttr
end

function UIButton_OnInitControl(BtnObj)
	InitCtrlByAttr(BtnObj)

	--控件第一次Init并不会激发visiblechange, enablechange事件，所以为了同步控件内visible,enable要在init调用一次
	UIButton_OnVisibleChange(BtnObj, BtnObj:GetVisible())
	UIButton_OnEnableChange(BtnObj, BtnObj:GetEnable())

	local btnAttr = BtnObj:GetAttribute()
	UIButton_SetState(BtnObj, "hover", true, true)
end

function UIButton_OnVisibleChange(BtnObj, visible)
	
	SetDefaultAnimation(BtnObj)
	BtnObj:SetChildrenVisible(visible)
end

function UIButton_OnEnableChange(BtnObj, enable)
	local btnAttr = BtnObj:GetAttribute()
	
	if enable then
		BtnObj:SetState("normal")
	else
		if btnAttr.BShowFocusRect and btnAttr.FocusRectObj then
			btnAttr.FocusRectObj:SetVisible(false)
		end
		BtnObj:SetState("disable")
	end
	
	--SetDefaultAnimation(BtnObj) 在设置状态时会设置
	BtnObj:SetChildrenEnable(enable)
end

function UIButton_OnPosChange(BtnObj, oldLeft, oldTop, oldRight, oldBottom, newLeft, newTop, newRight, newBottom)
	if (newRight - newLeft) ~= (oldRight - oldLeft) or (newBottom - newTop) ~= (oldBottom - oldTop) then
		AdjustBtnChildPos(BtnObj)
	end
end

function UIButton_OnFocusChange(BtnObj, focus)
	local btnAttr = BtnObj:GetAttribute()
	
	if btnAttr.NowState == "disable" then
		return 
	end
	
	if btnAttr.FocusRectObj and btnAttr.BShowFocusRect then
		btnAttr.FocusRectObj:SetVisible(focus)
	end
	
	if btnAttr.BFocusShowDown then
		if focus then
			BtnObj:SetState("down")
		else
			BtnObj:SetState("normal")
		end
	end	
end

function UIButton_OnLButtonDown(BtnObj, x, y, flags)
	local btnAttr = BtnObj:GetAttribute()
	
	if btnAttr.NowState ~= "disable" then
		BtnObj:SetState("down")
		BtnObj:SetCaptureMouse(true)
		
		if btnAttr.FireClickOnDown then
			BtnObj:FireExtEvent("OnClick")
		end
	end
end

function UIButton_OnLButtonUp(BtnObj, x, y, flags)
	local btnAttr = BtnObj:GetAttribute()
	
	if btnAttr.NowState == "down" then
		local btnLeft, btnTop, btnRight, btnBottom = BtnObj:GetObjPos()
		local btnWidth, btnHeight = btnRight - btnLeft, btnBottom - btnTop
		if x > btnWidth or  y > btnHeight then
			BtnObj:SetState("normal")
		else
			BtnObj:SetState("hover")
		end
		
		if not btnAttr.FireClickOnDown then
			BtnObj:FireExtEvent("OnClick")
		end
	end
	
	BtnObj:SetCaptureMouse(false)
end

function UIButton_OnLButtonDbClick(BtnObj, x, y, flags)
	local btnAttr = BtnObj:GetAttribute()
	
	if btnAttr.NowState ~= "disable" and btnAttr.ProcDbClickEqSClick then
		BtnObj:SetState("down")
		BtnObj:SetCaptureMouse(true)

		if btnAttr.FireClickOnDown then
			BtnObj:FireExtEvent("OnClick")
		end
	end
end

function UIButton_OnMouseEnter(BtnObj, x, y)
	local btnAttr = BtnObj:GetAttribute()
	
	if btnAttr.NowState == "normal" then
		BtnObj:SetState("hover")
	end
	
	if btnAttr.BHoverHand and btnAttr.NowState ~= "disable" then
		if "IDC_HAND" ~= BtnObj:GetCursorID() then
			BtnObj:SetCursorID("IDC_HAND")
		end
	end
	
	if btnAttr.TipText and btnAttr.TipText ~= "" and btnAttr.TipTemplate then
		local tipHostWnd = GetTipHostWnd(BtnObj)
		if tipHostWnd then
			local btnLeft, btnTop, btnRight, btnBottom = BtnObj:GetObjPos()
			local btnWidth, btnHeight = btnRight - btnLeft, btnBottom - btnTop
			tipHostWnd:DelayPopup(500)
			tipHostWnd:SetPositionByObject(btnWidth / 2 - 18 / 2 - 1000 , btnHeight + 5, BtnObj)
		end
	end
end

function UIButton_OnMouseMove(BtnObj, x, y, flags)
	local btnAttr = BtnObj:GetAttribute()
	
	if btnAttr.NowState == "normal" then
		BtnObj:SetState("hover")
	end
	
	if btnAttr.BHoverHand and btnAttr.NowState ~= "disable" then
		if "IDC_HAND" ~= BtnObj:GetCursorID() then
			BtnObj:SetCursorID("IDC_HAND")
		end
	end
end

function UIButton_OnMouseLeave(BtnObj, x, y)
	local btnAttr = BtnObj:GetAttribute()
	
	if btnAttr.NowState ~= "disalbe"  then
		if BtnObj:GetFocus() and btnAttr.NowState == "down" and btnAttr.BFocusShowDown then
		else
			BtnObj:SetState("normal")
		end
	end
	
	local tipHostWnd = GetTipHostWnd(BtnObj)
	if tipHostWnd then
		tipHostWnd:Cancel()
	end
	-- 好像在鼠标离开它树的范围时有产生离开事件这时可以使用下面的
	BtnObj:SetCaptureMouse(false)
end

function UIButton_OnMouseWheel(BtnObj)

	BtnObj:RouteToFather()
end

function UIButton_OnKeyDown(BtnObj ,uKey, uRepeatCount, uFlags)
	if uKey == 13 or uKey == 32 then -- 空格和回车
		BtnObj:FireExtEvent("OnClick")
	end
	
	BtnObj:RouteToFather()
end

function UIButton_OnKeyUp(BtnObj, uKey, uRepeatCount, uFlags)

	BtnObj:RouteToFather()
end

--------------------------------tiphostwnd----------------------------
function UIBtnTipText_OnPosChange(TextObj)
	local tree = TextObj:GetOwner()
	local textLeft, textTop, textRight, textBottom = TextObj:GetObjPos()
	local textWidth, textHight = textRight - textLeft, textBottom - textTop
	
	local bkgObj = tree:GetUIObject("bkg")
	local bkgLeft, bkgTop, bkgRight, bkgBottom = bkgObj:GetObjPos()
	
	local newBkgWidth, newBkgHeight = textWidth + 10, textHight + 10
	bkgLeft = 1000
	bkgObj:SetObjPos(bkgLeft, bkgTop, bkgLeft + newBkgWidth, bkgTop + newBkgHeight)
end

function UIBtnTipWnd_OnCreate(self)

end

function UIBtnTipWnd_OnDelayPopup(self)

end

function UIBtnTipWnd_OnDelayCancel(self)

end
---------------------------------DefaultBtnAni-------------------------
function DefaultButton_BindObj(AniObj, ButtonObj)
	local aniAttr = AniObj:GetAttribute()
	if not ButtonObj then
		return
	end
	
	aniAttr.BindObj = ButtonObj
    local btnAttr = ButtonObj:GetAttribute()
    local objFactory = XLGetObject("Xunlei.UIEngine.ObjectFactory")
	if not aniAttr.MaskImageObj then
		aniAttr.MaskImageObj = objFactory:CreateUIObject("masktexture", "TextureObject")
		ButtonObj:AddChild(aniAttr.MaskImageObj)
	end
	aniAttr.MaskImageObj:SetTextureID(aniAttr.AniTexture)
	aniAttr.MaskImageObj:SetObjPos(0,0,"father.width", "father.height")
	
	if aniAttr.ListenerCookie then
		AniObj:RemoveListener(aniAttr.ListenerCookie)
		aniAttr.ListenerCookie = nil
	end
	
	aniAttr.ListenerCookie = AniObj:AttachListener(true,
		function (AniObj, oldState, newState)
			if newState == 4 then
				ButtonObj:RemoveChild(aniAttr.MaskImageObj)
				aniAttr.MaskImageObj = nil
				--XLMessageBox("in")
				-- 及时回收内存，避免内存泄漏 
				collectgarbage()
			end
		end)
		
	AniObj:SetTotalTime(aniAttr.TotalTime)
end

function DefaultButton_GetBindObj(AniObj)
	local aniAttr = AniObj:GetAttribute()
	
	return aniAttr.BindObj
end

function DefaultButton_Action(AniObj)
    local aniAttr = AniObj:GetAttribute()
	local aniCurve = AniObj:GetCurve()
	local aniTotalTime = AniObj:GetTotalTime()
	local aniRunningTime = AniObj:GetRuningTime()
	
	local trueTime = aniRunningTime % aniAttr.FrameTime
	local singleFrameTime = aniAttr.FrameTime / 2
	local progTime = trueTime % singleFrameTime
	local progress =  aniCurve:GetProgress(progTime / singleFrameTime)
	if trueTime >= 0 and trueTime < singleFrameTime then
	    aniAttr.MaskImageObj:SetAlpha(255 * progress)
	else
	    aniAttr.MaskImageObj:SetAlpha(255 * (1 - progress))
	end
end

function DefaultButton_Stop(AniObj)
	local aniAttr = AniObj:GetAttribute()
	AniObj:SetState(4)
	if aniAttr.MaskImageObj then
		aniAttr.BindObj:RemoveChild(aniAttr.MaskImageObj)
		aniAttr.MaskImageObj = nil
	end
	-- 及时回收内存，避免内存泄漏 
	collectgarbage()
end