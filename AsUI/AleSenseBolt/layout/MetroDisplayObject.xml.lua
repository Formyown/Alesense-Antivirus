function OnInitControl(self)
	local TextObj = self:GetControlObject("TextObj")
	local TitleObjec = self:GetControlObject("TitleObj")
	local Attr = self:GetAttribute()
	TextObj:SetText(Attr.Text)
	TitleObjec:SetText(Attr.Title)
end