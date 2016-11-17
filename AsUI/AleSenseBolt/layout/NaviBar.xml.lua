function OnInitControl(self)
	AniFactory = XLGetObject("Xunlei.UIEngine.AnimationFactory")
	OwnerTree = self:GetOwner()	
	Control = self

	ButtonImgObj = Control:GetControlObject("BkgImg")
	Background = Control:GetControlObject("BackLayout")

	OrgLeft = 0
	OrgTop = 0
	OrgRight = 780
	OrgBottom = 50

	Buttons = 
	{
		Control = Control:GetControlObject("ControlCenter"),
		Security = Control:GetControlObject("SecurityCenter"),
		Update = Control:GetControlObject("Update"),
		Home = Control:GetControlObject("Home"),
		Scan = Control:GetControlObject("Scan")
	}
	Buttons[1] = Buttons.Scan
	Buttons[0] = Buttons.Home
	Buttons[3] = Buttons.Security
	Buttons[2] = Buttons.Update
end



Hidden = false
function Hide(self,bool)

	if bool ~= Hidden then
		local PosAni = AniFactory:CreateAnimation("PosChangeAnimation")
		PosAni:SetTotalTime(250)
		if  bool == true then
			Hidden = bool
			PosAni:SetKeyFrameRect(OrgLeft,OrgTop,OrgRight,OrgBottom,OrgLeft,OrgTop - 50,OrgRight,OrgBottom - 50)
		end
		if bool == false then
			Hidden = bool
			PosAni:SetKeyFrameRect(OrgLeft,OrgTop - 50,OrgLeft,OrgBottom - 50,OrgLeft,OrgTop,OrgRight,OrgBottom)
		end
		PosAni:BindCurveID("Curve.Jmp")
		PosAni:BindObj(Background)
		OwnerTree:AddAnimation(PosAni)
		PosAni:Resume()
	end
end

function OnMouseEnter(self)
	
	self:SetCursorID ("IDC_HAND")	
end

function OnMouseLeave(self)
	--self:SetCursorID ("IDC_ARROW")
	
end
Current = 0
FunctionTable = 
{
	Home = 0,
	Scan = 1,
	Update = 2,
	SecurityCenter = 3,
	ControlCenter = 4
}

IsTrueNavi = true
function Navi(self,index)
	if Buttons[index] ~= nil then
		IsTrueNavi = false
		OnMouseClick(Buttons[index])
	end
end

function OnMouseClick(self)
	if FunctionTable[self:GetID()]  ~= Current then
		Current = FunctionTable[self:GetID()]
		local Left,Top,Right,Bottom = self:GetObjPos()
		local PosAni = AniFactory:CreateAnimation("PosChangeAnimation")
		local OrgLeft,OrgTop,OrgRight,OrgBottom = ButtonImgObj:GetObjPos()


		PosAni:SetTotalTime(170)
		PosAni:SetKeyFrameRect(OrgLeft,OrgTop,OrgRight,OrgBottom,Left - 5 ,OrgTop,Right + 2,OrgBottom)
		PosAni:BindObj(ButtonImgObj)
		PosAni:BindCurveID("bezier")
		OwnerTree:AddAnimation(PosAni)
		PosAni:Resume()


		Buttons.Scan:SetTextColorResID("Color.Gray")
		Buttons.Home:SetTextColorResID("Color.Gray")
		Buttons.Security:SetTextColorResID("Color.Gray")
		Buttons.Update:SetTextColorResID("Color.Gray")
		Buttons.Control:SetTextColorResID("Color.Gray")
		self:SetTextColorResID("Color.White")
		if IsTrueNavi == true then
			Control:FireExtEvent("OnNavi",Current)
		end
		IsTrueNavi = true
	end
end