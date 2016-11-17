function OnInitControl(self)
	AsControl = XLGetObject("As.Control")
	
	AniFactory = XLGetObject("Xunlei.UIEngine.AnimationFactory")
	TimerManager = XLGetObject("Xunlei.UIEngine.TimerManager")
	

	OwnerTree = self:GetOwner()		
	ChoiceItemBackground = self:GetControlObject("ItemBkg")

	ScanningLayout = self:GetControlObject("ScanningLayout")
	DefaultLayout = self:GetControlObject("DefaultLayout")
	ResultLayout = self:GetControlObject("ResultLayout")
	ScanningLayout:SetChildrenVisible(false)
	ResultLayout:SetChildrenVisible(false)

	ScanTypeTitle = self:GetControlObject("ScanTypeTitle")
	ScanTypeText = self:GetControlObject("ScanTypeText")

	ScanningType = self:GetControlObject("ScanningType")

	ScanningButton = self:GetControlObject("ScanButton")
	ProgressBar = self:GetControlObject("ProgressBar")

	PauseButton = self:GetControlObject("PauseButton")
	StopButton = self:GetControlObject("StopButton")
	ScanningVelocity = self:GetControlObject("ScanningVelocity")
	ScanedNumber = self:GetControlObject("ScanedNumber")
	FoundNumber = self:GetControlObject("FoundNumber")
	ScanningTime = self:GetControlObject("ScanningTime")
	ScanningPath = self:GetControlObject("ScanningPath")
	ResultList = self:GetControlObject("ResultList")
	List = self:GetControlObject("List")
----
	CalcTextObj = self:GetControlObject("CalcTextObj")
----

	ResultTitle = self:GetControlObject("ResultTitle")
	ResultText = self:GetControlObject("ResultText")

end
Details = 
{
	CustomScanBackground = {"自定义扫描","扫描选定的文件、文件夹或驱动器，时间取决于项目的数量",2},
	QuickScanBackground = {"快速扫描","扫描系统的关键位置、敏感位置，不会耗费太多时间",1},
	TotalScanBackground = {"全盘扫描","扫描计算机上的所有文件和文件夹，可能需要较长时间",0}
}
FoundCount = 0
                            --Name
function GetShortPathForTextObj(path,textobj)
	CalcTextObj:SetText(path)
 		local iTextWidth, iTextHeight = CalcTextObj:GetTextExtent()	
    	if iTextWidth > 330 then
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
    		while iTextWidth < 300 do
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
function HandleMsg(self,Type,Msg, Path,CType,EngineName)
	if Type == 5 then --ScanningProgress
		ProgressBar:SetValue(Msg)
	end
	if Type == 6 then --ScanningCount
		ScanedNumber:SetText("已扫描:"..Msg.."个")
		ScanningCount = Msg
	end
	if Type == 7 then --FoundCount
		FoundNumber:SetText("发现:"..Msg.."个")
		FoundCount = Msg
	end
	if Type == 8 then --Time
		ScanningTime:SetText("用时:"..Msg)
	end
	if Type == 9 then --StartCustomScanning
		CustomScan()
	end
	if Type == 10 then --Path
		GetShortPathForTextObj(Msg,ScanningPath)

	end
	if Type == 11 then --Next

		List:AddNextItem(Msg,Path,CType,EngineName)
		
	end
	if Type == 12 then --Prev
		List:AddPrevItem(Msg,Path,CType,EngineName)
	end
	if Type == 13 then --SetCount
		List:SetTrueTotal(Msg)
	end

	if Type == 14 then --ScanningVelocity
		ScanningVelocity:SetText("速度:"..Msg.." 个每秒")
	end

	if Type == 15 then --Completed
		ScanningCompleted()
	end

	if Type == 16 then --HandleCompleted
		ShowResult(1)
	end

	--if Type == 17 then
end
function OnListGetPrev(count)
	AsControl:InformControl(7)
end
function OnListGetNext(count)
	AsControl:InformControl(6)
end
ScanningTypeCode = 1
IsScanning = false
Completed = false
OnAni = false
function OnMouseDown(self)
	if OnAni == true then return end
	local OldLeft,OldTop,OldRight,OldBottom = ChoiceItemBackground:GetObjPos()
	local Left,Top,Right,Bottom = self:GetObjPos()
	local PosAni = AniFactory:CreateAnimation("PosChangeAnimation")
	PosAni:SetTotalTime(180)
	PosAni:SetKeyFrameRect(OldLeft,OldTop,OldRight,OldBottom,Left,Top,Right, Top + (OldBottom - OldTop))
	PosAni:BindObj(ChoiceItemBackground)
	--PosAni:BindCurveID("Curve.Jmp")
	OwnerTree:AddAnimation(PosAni)
	PosAni:Resume()


	local aniAlpha = AniFactory:CreateAnimation("AlphaChangeAnimation")
	aniAlpha:BindRenderObj(ChoiceItemBackground)
	aniAlpha:SetTotalTime(180)
	aniAlpha:SetKeyFrameAlpha(255,50)
	OwnerTree:AddAnimation(aniAlpha)
	aniAlpha:BindCurveID("Curve.Alpha")
	aniAlpha:Resume()



	ScanTypeTitle:SetText(Details[self:GetID()][1])
	ScanTypeText:SetText(Details[self:GetID()][2])
	ScanningTypeCode = Details[self:GetID()][3]
end
function OnMouseUp( ... )

end
function OnMouseLeave( ... )

end

function OnClickReturnDeafult()
	ShowDefault()
end

IsPause = false
function OnClickPause(self)
	--PauseScanning = 3,
	--StopScanning = 4,
	--ContinueScanning = 5
	if Completed == false then
	if IsPause == false then
		IsPause = true
		AsControl:InformControl(3)
		PauseButton:SetText("继续")
	else
		IsPause = false
		AsControl:InformControl(5)
		PauseButton:SetText("暂停")
	end
	else
		AsControl:InformControl(3)
		local path = __document
		local index = string.find(path, "/[^/]*$")
		local layoutDir = string.sub(path,1,index)
		local ret = XLLoadModule(layoutDir.."MessageBox.xml.lua").MessageBox("确定要放弃处理这些威胁吗？", "侦锐", 6)
		if 1 == ret then
		
		PauseButton:SetEnable(false)
		StopButton:SetEnable(false)
		AsControl:InformControl(8)
		end
	end
end
function OnClickStop(self)
	if Completed == false then
		--AsControl:InformControl(4)
		ShowQueryMessage()
	else
		
		AsControl:InformControl(9)
		PauseButton:SetEnable(false)
		StopButton:SetEnable(false)
	end
	
end
function CustomScan()
	if IsScanning == false then
			ScanningType:SetText(ScanTypeTitle:GetText())
			OnAni = true
			IsScanning = true
			Completed = false
			PauseButton:SetAlpha(1,true)
			StopButton:SetAlpha(1,true)
			FoundNumber:SetAlpha(0,false)
			ScanedNumber:SetAlpha(0,false)
			ScanningTime:SetAlpha(0,false)
			ResultList:SetAlpha(0,true)
			ScanningVelocity:SetAlpha(0,true)
			ProgressBar:SetAlpha(255,true)
			SwitchToScanningAniTimer = TimerManager:SetTimer(SwitchToScanningTimerTick,5)
	end
end
function OnClickScanning(self)
	if OnAni == false then
		if IsScanning == false then
			AsControl:InformControl(ScanningTypeCode)
			if ScanningTypeCode ~= 2 then
			ScanningType:SetText(ScanTypeTitle:GetText())
			OnAni = true
			--ScanningButton:SetState(2)
			IsScanning = true
			Completed = false
			ScanningButton:SetEnable(false)
			ScanningButton:SetState(2)
			PauseButton:SetText("暂停")
			StopButton:SetText("终止")

			ProgressBar:SetValue(0)

			ScanedNumber:SetText("已扫描:0 个")
			ScanningCount = 0
			FoundNumber:SetText("发现:0 个")
			FoundCount = 0
			ScanningVelocity:SetText("速度:0 个每秒")
			ScanningTime:SetText("用时:0 秒")
			ScanningPath:SetText("")

			PauseButton:SetAlpha(1,true)
			StopButton:SetAlpha(1,true)
			FoundNumber:SetAlpha(0,false)
			ScanedNumber:SetAlpha(0,false)
			ScanningTime:SetAlpha(0,false)
			ResultList:SetAlpha(0,true)
			ScanningVelocity:SetAlpha(0,true)
			ProgressBar:SetAlpha(255,true)
			SwitchToScanningAniTimer = TimerManager:SetTimer(SwitchToScanningTimerTick,5)					
			end
			-------------------------------
			end
		--else
			--OnAni = true
			--SwitchToDefaultAniTimer = TimerManager:SetTimer(SwitchToDefaultTimerTick,5)

			
			
			--ScanningType:SetAlpha(0,false)
			
		--end
	end
end

DefaultLayoutAlpha = 255
--默认界面消失
function SwitchToScanningTimerTick(self)
	if DefaultLayoutAlpha > 0 then
		DefaultLayoutAlpha = DefaultLayoutAlpha - 40
		DefaultLayout:SetAlpha(DefaultLayoutAlpha,true)
	else
		TimerManager:KillTimer(SwitchToScanningAniTimer)
		ScanningLayout:SetChildrenVisible(true)
		DefaultLayout:SetChildrenVisible(false)

		local AlphaAni2 = AniFactory:CreateAnimation("AlphaChangeAnimation")
		AlphaAni2:BindRenderObj(ScanningType)
		AlphaAni2:SetTotalTime(500)
		AlphaAni2:SetKeyFrameAlpha(0,255)
		
		AlphaAni2:AttachListener(true,
			function (AlphaAni2, oldState, newState)
				if newState == 4 then

					SwitchToScanningAniTimer2 = TimerManager:SetTimer(SwitchToScanning2TimerTick,5)
					
					

				end
				end)
		OwnerTree:AddAnimation(AlphaAni2)
		AlphaAni2:Resume()


		local PosAni = AniFactory:CreateAnimation("PosChangeAnimation")
		PosAni:SetTotalTime(500)
		PosAni:SetKeyFrameRect(290,35,390,61,0,35,680, 61)
		PosAni:BindObj(ProgressBar)
		OwnerTree:AddAnimation(PosAni)
		PosAni:Resume()
	end
end

ScanningUIControlsAlpha = 0
--扫描界面渐变元素
function SwitchToScanning2TimerTick(self)
	if ScanningUIControlsAlpha < 255 then
		ScanningUIControlsAlpha = ScanningUIControlsAlpha + 15
		PauseButton:SetAlpha(ScanningUIControlsAlpha,true)
		StopButton:SetAlpha(ScanningUIControlsAlpha,true)
		ScanningTime:SetAlpha(ScanningUIControlsAlpha,false)
		FoundNumber:SetAlpha(ScanningUIControlsAlpha,false)
		ScanedNumber:SetAlpha(ScanningUIControlsAlpha,false)
		ScanningVelocity:SetAlpha(ScanningUIControlsAlpha,false)
		ResultList:SetAlpha(ScanningUIControlsAlpha,true)

	else
		TimerManager:KillTimer(SwitchToScanningAniTimer2)
		ScanningUIControlsAlpha = 0
		OnAni = false
		
	end
end


function SwitchToDefaultTimerTick(self)
	if ScanningUIControlsAlpha > 0 then
		ScanningUIControlsAlpha = ScanningUIControlsAlpha - 40
		
		ScanningLayout:SetAlpha(ScanningUIControlsAlpha,true)
	else
		TimerManager:KillTimer(SwitchToDefaultAniTimer)
		ScanningLayout:SetChildrenVisible(false)
		DefaultLayout:SetChildrenVisible(true)
		SwitchToDefaultAniTimer2 = TimerManager:SetTimer(SwitchToDefault2TimerTick,5)
	end
end

function SwitchToDefault2TimerTick(self)
	if DefaultLayoutAlpha < 255 then
		DefaultLayoutAlpha = DefaultLayoutAlpha + 25
		
		DefaultLayout:SetAlpha(DefaultLayoutAlpha,true)
	else
		TimerManager:KillTimer(SwitchToDefaultAniTimer2)
		OnAni = false
		IsScanning = false
	end
end


function ScanningCompleted()

	IsScanning = false
	if FoundCount == 0 then
	
	ShowResult(true)

	else
		Completed = true
		PauseButton:SetEnable(true)
		StopButton:SetEnable(true)
		ScanningPath:SetText("")
		PauseButton:SetText("暂不处理")
		StopButton:SetText("立即处理")
	end
end

function ShowResult(completed)
	local ResultTitle = 1
	List:DeleteAllItems(ResultList)
	if completed == 1 then --正常扫描完成

	else if completed == 0 then


		else
		end

	end

	ResultLayout:SetChildrenVisible(true)
	ScanningLayout:SetChildrenVisible(false)

	
end
function ShowDefault()
	Completed = true
	local TurnAni = AniFactory:CreateAnimation("TurnObjectAnimation")
	TurnAni:BindRenderObj(ChoiceItemBackground)
	TurnAni:SetTotalTime(300)
	TurnAni:BindRenderObj(ResultLayout,DefaultLayout)
	OwnerTree:AddAnimation(TurnAni)
	TurnAni:Resume()

	--DefaultLayout:SetChildrenVisible(true)
	--ResultLayout:SetChildrenVisible(false)
	DefaultLayout:SetAlpha(255,true)
	ScanningButton:SetEnable(true)
	ScanningButton:SetState(0)

	PauseButton:SetEnable(true)
	StopButton:SetEnable(true)
	ScanningButton:SetState(0)
	ScanningButton:SetState(0)
end

function ShowQueryMessage()
	--PauseScanning = 3,
	--StopScanning = 4,
	AsControl:InformControl(3)
	local path = __document
	local index = string.find(path, "/[^/]*$")
	local layoutDir = string.sub(path,1,index)
	local ret = XLLoadModule(layoutDir.."MessageBox.xml.lua").MessageBox("扫描还未完成，您确定要终止吗？", "侦锐", 6)
	if 1 == ret then
		--XLMessageBox("确定")
		PauseButton:SetEnable(false)
		StopButton:SetEnable(false)
		AsControl:InformControl(4)

	else
		--XLMessageBox("取消")
		AsControl:InformControl(5)

	end
end