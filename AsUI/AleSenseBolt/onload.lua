local templateMananger = XLGetObject("Xunlei.UIEngine.TemplateManager")
local frameHostWndTemplate = templateMananger:GetTemplate("As.Wnd","HostWndTemplate")
if frameHostWndTemplate then  
	local frameHostWnd = frameHostWndTemplate:CreateInstance("MainFrame")
	if frameHostWnd then
		local objectTreeTemplate = templateMananger:GetTemplate("As.Tree","ObjectTreeTemplate")
		if objectTreeTemplate then
			local uiObjectTree = objectTreeTemplate:CreateInstance("MainObjectTree")
			if uiObjectTree then
				frameHostWnd:BindUIObjectTree(uiObjectTree)
				frameHostWnd:Create()
			end
		end
	end
end