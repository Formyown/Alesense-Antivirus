local Vlaue = 0
local MaxValue = 100
function SetValue(self,value)
	left,top,right,buttom = Fore:GetObjPos()
	bleft,btop,bright,bbuttom = Bkg:GetObjPos()
	bright = bright - 4
	if value == 0 or value == MaxValue then
		Fore:SetObjPos(left,top,left + ((bright - bleft) / MaxValue * value),buttom)
	else
		local PosAni = AniFactory:CreateAnimation("PosChangeAnimation")
		PosAni:SetTotalTime(300)
		PosAni:SetKeyFrameRect(left,top,right,buttom,left,top,left + ((bright - bleft) / MaxValue * value),buttom)
		PosAni:BindObj(Fore)
		--PosAni:BindCurveID("Curve.Jmp")

		OwnerTree:AddAnimation(PosAni)
		PosAni:Resume()
	end
	
	--Fore:SetObjPos(left,top,left + ((bright - bleft) / MaxValue * value),buttom
end

function SetMaxValue(self,value)
	MaxValue = value
end


function OnInitControl(self)
	Fore = self:GetControlObject("Fore")
	Bkg = self:GetControlObject("Bkg")
	OwnerTree = self:GetOwner()		
	AniFactory = XLGetObject("Xunlei.UIEngine.AnimationFactory")
	local attr = self:GetAttribute()
	SetMaxValue(self,attr.maxvalue)
	SetValue(self,attr.value)
end