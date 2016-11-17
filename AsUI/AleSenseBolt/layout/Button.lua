--lua文件必须是UTF-8编码的(最好无BOM头)

function SetState(self,newState) 		
	local attr = self:GetAttribute()
	

	if newState ~= attr.NowState then
		local ownerTree = self:GetOwner()
		

		local oldBkg = nil
		local bkg = nil
		local graphic = XLGetObject("Xunlei.XLGraphic.Factory.Object")
		local xar = XLGetObject("Xunlei.UIEngine.XARManager")
		local attr = self:GetAttribute()
		if attr.UseBitmap == true then
			oldBkg = self:GetControlObject("oldBkg_t")
			bkg = self:GetControlObject("bkg_t")
		else
			oldBkg = self:GetControlObject("oldBkg")
			bkg = self:GetControlObject("bkg")
		end

		if attr.UseBitmap == false then
			oldBkg:SetTextureID (bkg:GetResID ())
			oldBkg:SetAlpha(255)
			if newState == 0 then
				bkg:SetTextureID (attr.NormalBkgID)
			elseif newState == 1 then
				bkg:SetTextureID (attr.DownBkgID)
			elseif newState == 2 then
				bkg:SetTextureID (attr.DisableBkgID)
			elseif newState == 3 then
				bkg:SetTextureID (attr.HoverBkgID)
			end
		else
			oldBkg:SetResID (bkg:GetResID ())
			oldBkg:SetAlpha(255)
			if newState == 0 then
				bkg:SetResID (attr.NormalBkgID)
			elseif newState == 1 then
				bkg:SetResID (attr.DownBkgID)
			elseif newState == 2 then
				bkg:SetResID (attr.DisableBkgID)
			elseif newState == 3 then
				bkg:SetResID (attr.HoverBkgID)
			end
		end
		
	local aniFactory = XLGetObject("Xunlei.UIEngine.AnimationFactory")	
	local aniAlpha = aniFactory:CreateAnimation("AlphaChangeAnimation")
	aniAlpha:BindRenderObj(oldBkg)
	aniAlpha:SetTotalTime(300)
	aniAlpha:SetKeyFrameAlpha(255,0)
	ownerTree:AddAnimation(aniAlpha)
	aniAlpha:Resume()
	attr.NowState = newState

						
	end


end

function SetText(self,newText)
	local textObj = self:GetControlObject("text")
	textObj:SetText(newText)
end
This = {}
function OnLButtonDown(self)
	local attr = self:GetAttribute()
	This[self:GetID()].IsButtonDown = true
	if attr.Enable then
		self:SetState(1)

		self:SetCaptureMouse(true)
	end
end

function OnLButtonUp(self)
	local attr = self:GetAttribute()
	
	if attr.Enable then
		if attr.NowState==1 then
			self:SetState(0)
			if This[self:GetID()].IsButtonDown == true then
			   This[self:GetID()].IsButtonDown = false
				self:FireExtEvent("OnClick")

			end
			
		end

		self:SetCaptureMouse(false)
	end
end

function OnMouseMove(self)
	local attr = self:GetAttribute()
	if attr.Enable then
		if attr.NowState == 0 then
			self:SetState(3)
		end
	end
end

function OnMouseLeave(self)
	local attr = self:GetAttribute()
	This[self:GetID()].IsButtonDown = false
	self:SetState(0)
end
function OnBind(self)
	This[self:GetID()] = {}
	This[self:GetID()].IsButtonDown = false

	graphic = XLGetObject("Xunlei.XLGraphic.Factory.Object")
	xar = XLGetObject("Xunlei.UIEngine.XARManager")
	local attr = self:GetAttribute()
	self:SetText(attr.Text)
	attr.NowState = 0
	
	if attr.UseBitmap == true then
		local bkg = self:GetControlObject("bkg_t")
		bkg:SetResID (attr.NormalBkgID)
	else
		local bkg = self:GetControlObject("bkg")
		bkg:SetTextureID (attr.NormalBkgID)
	end
end