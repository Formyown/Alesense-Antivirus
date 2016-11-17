function OnInitControl(self)
	--AsControl = XLGetObject("As.Control")
	Protect = self:GetControlObject("Protect")
	Update = self:GetControlObject("Update")
	Status = self:GetControlObject("Status")

	StartProgramPath = Protect:GetControlObject("StartProgramPath")
    ScanedCount = Protect:GetControlObject("ScanedCount")
    VirusCount = Protect:GetControlObject("VirusCount")
    ProtectValue = Protect:GetControlObject("ProtectValue")

	Version = Update:GetControlObject("Version")
	DataBaseVersion = Update:GetControlObject("DataBaseVersion")

	PerformanceLevel = self:GetControlObject("Status"):GetControlObject("PerformanceLevel")
	ProtectLevel =	   Protect:GetControlObject("ProtectLevel")
 	ProtectInfo = 	   Protect:GetControlObject("ProtectInfo")

end
ScannedNum = 0
FoundNum = 0
function HandleMsg(self,Type,Msg)
	if Type == 0 then --CreatePath
		StartProgramPath:SetText(Msg)
	end
	if Type == 1 then --CreatedCount
		ScannedNum = Msg
		ScanedCount:SetText("今日检测应用程序：".. Msg .."个")
		local value = 100 * (FoundNum/ScannedNum)
		ProtectValue:SetText(string.format("%2.0f",value).."%")
		if value >= 40 then
			ProtectValue:SetTextColorResID ("Color.Red")
			ProtectInfo:SetText ("防护压力较大，建议及时\n备份资料并进行全盘扫描")
		else
			if value >= 15 then
				ProtectValue:SetTextColorResID ("Color.Yellow")
				ProtectInfo:SetText("防护压力中等，建议及时\n备份资料并进行快速扫描")
			else
				ProtectValue:SetTextColorResID ("Color.MetroGreen")
				ProtectInfo:SetText("防护压力较小\n继续保持健康使用习惯")
			end
		end
		
	end
	if Type == 2 then --FoundCount
		FoundNum = Msg
		VirusCount:SetText("发现：" .. Msg .."个病毒")
	end
	if Type == 3 then --Version
		Version:SetText("版本: " .. Msg)
	end
	if Type == 4 then --DataBaseVersion
		DataBaseVersion:SetText("数据库: " .. Msg)
	end
end