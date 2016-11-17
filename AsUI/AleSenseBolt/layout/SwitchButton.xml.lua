This = {}
function OnInitControl(self)
	local SelfID = self:GetID()
	local Handle = self:GetControlObject("Handle")
	local Bkg = self:GetControlObject("Background3")
	local Left,Top,Right,Bottom = self:GetObjPos()

	This[SelfID]=
	{	
		Clicked = false,
		IsMouseDown = false,
		Handle = self:GetControlObject("Handle"),
		Left,Top,Right,Bottom = self:GetObjPos(),
		ControlWidth = Right - Left,
		Attr = self:GetAttribute(),
		Handle = Handle,
		Bkg = Bkg
	}
	This[SelfID].OldLeft,This[SelfID].OldTop,This[SelfID].OldRight,This[SelfID].OldBottom = Handle:GetObjPos()
	local OldLeft,OldTop,OldRight,OldBottom = Handle:GetObjPos()
	local Handle = This[SelfID].Handle
	local ControlWidth = This[SelfID].ControlWidth
	local OldLeft,OldTop,OldRight,OldBottom = Handle:GetObjPos()
	local Bkg = This[SelfID].Bkg
	local Attr = This[SelfID].Attr

	This[SelfID].Clicked = Attr.switch
	if Attr.switch == true then
		Handle:SetObjPos(OldLeft + ControlWidth - 15,OldTop,OldRight + ControlWidth - 15,OldBottom)
		Bkg:SetSrcColor("Color.Blue")
	else
		Handle:SetObjPos(OldBottom,OldLeft,OldTop,OldRight,OldBottom)
		Bkg:SetSrcColor("Color.Gray")
	end

	
end

function OnLButtonDown(self)
	This[self:GetID()].IsMouseDown = true
end
function OnLButtonUp(self)
	local AniFactory = XLGetObject("Xunlei.UIEngine.AnimationFactory")
	local OwnerTree = self:GetOwner()	

	local SelfID = self:GetID()
	local Handle = This[SelfID].Handle
	local Bkg = This[SelfID].Bkg
	local OldLeft = This[SelfID].OldLeft
	local OldTop = This[SelfID].OldTop
	local OldRight = This[SelfID].OldRight
	local OldBottom = This[SelfID].OldBottom
	
	local ControlWidth = This[SelfID].ControlWidth

	if This[SelfID].IsMouseDown == true then
		This[SelfID].Clicked = not This[SelfID].Clicked
		if This[SelfID].Clicked == true then
			This[SelfID].IsMouseDown = false
			local PosAni = AniFactory:CreateAnimation("PosChangeAnimation")
			PosAni:SetTotalTime(150)
			PosAni:SetKeyFrameRect(OldLeft,OldTop,OldRight,OldBottom,OldLeft + ControlWidth - 15,OldTop,OldRight + ControlWidth - 15,OldBottom)
			PosAni:BindObj(Handle)
			--PosAni:AttachListener(true,PosAni_OldFinish)
			OwnerTree:AddAnimation(PosAni)
			--Handle:SetObjPos(OldLeft + ControlWidth - 15,OldTop,OldRight + ControlWidth - 15,OldBottom)
			PosAni:Resume()
			Bkg:SetSrcColor("Color.Blue")
		else
			local PosAni = AniFactory:CreateAnimation("PosChangeAnimation")
			PosAni:SetTotalTime(150)
			PosAni:SetKeyFrameRect(OldLeft + ControlWidth - 15,OldTop,OldRight + ControlWidth - 15,OldBottom,OldLeft,OldTop,OldRight,OldBottom)
			PosAni:BindObj(Handle)
			--PosAni:AttachListener(true,PosAni_OldFinish)
			OwnerTree:AddAnimation(PosAni)
			--Handle:SetObjPos(OldLeft + ControlWidth - 15,OldTop,OldRight + ControlWidth - 15,OldBottom)
			PosAni:Resume()
			Handle:SetObjPos(OldLeft,OldTop,OldRight,OldBottom)
			Bkg:SetSrcColor("Color.Gray")
		end
	end
end

function GetSwitch(self)
	return This[self:GetID()].Clicked
end

function SetSwitch(self,switch)
	local SelfID = self:GetID()
	This[SelfID].Clicked = switch
	local Handle = This[SelfID].Handle
	local Bkg = This[SelfID].Bkg
	if switch == true then
		Handle:SetObjPos(OldLeft + ControlWidth - 15,OldTop,OldRight + ControlWidth - 15,OldBottom)
		Bkg:SetSrcColor("Color.Blue")
	else
		Handle:SetObjPos(OldBottom,OldLeft,OldTop,OldRight,OldBottom)
		Bkg:SetSrcColor("Color.Gray")
	end
end