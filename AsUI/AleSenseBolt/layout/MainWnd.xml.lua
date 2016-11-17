MetroLeft = 70
MetroTop = 140
MetroWidth = 680
MetroHeight = 340
MetroRight = MetroLeft + MetroWidth
MetroBottom = MetroTop + MetroHeight
MetroHiddenTop = 560
MetroHiddenBottom = 560 + 560

AsControl = XLGetObject("As.Control")

function Navi_OnCloseButtonClick(self)
	AsControl:InformControl(18)
end
function Navi_OnMiniButtonClick( self )
	AsControl:InformControl(19)
end

--主Metro的导航事件
function MetroButtonClick(self,name,index)
	SwitchMetro(index)
end
--------------------------------------
Metro = {}
CurrentMetro = nil
OldMetro = nil
function  OnInitControl(self)


	AniFactory = XLGetObject("Xunlei.UIEngine.AnimationFactory")
	------------------------------------------------------
	ObjectTree = self:GetBindUIObjectTree()	
	------------------------------------------------------
	NaviBar = ObjectTree:GetUIObject("NaviBar")

	Metro.Home = ObjectTree:GetUIObject("Metro.Home")
	Metro[0] = Metro.Home

	Metro.Scanning = ObjectTree:GetUIObject("Metro.Scanning")
	Metro[1] = Metro.Scanning 

	Metro.Update = ObjectTree:GetUIObject("Metro.Update")
	Metro[2] = Metro.Update 
	--Metro.Home:SetChildrenVisible (false)
	Metro.Scanning:SetChildrenVisible (false)
	Metro.Scanning:SetObjPos(MetroLeft,MetroHiddenTop,MetroRight,MetroHiddenBottom)
	Metro.Update:SetChildrenVisible (false)
	Metro.Update:SetObjPos(MetroLeft,MetroHiddenTop,MetroRight,MetroHiddenBottom)
	CurrentMetro = Metro.Home


	--local path = __document
	--local index = string.find(path, "/[^/]*$")
	--local layoutDir = string.sub(path,1,index)
	--XLLoadModule(layoutDir.."PopWindow.xml.lua").PopWindow("warning",0,"确定","取消",true,false,"有程序正试图注入其他进程.","危险来源:[PID 162] amvirus.exe","受害目标:[PID 3645] firefox.exe","这个动作很危险，建议您立即阻止！")
	--function PopWindow(type,defaultBtnText,otherBtnText,isRightDefault,isTick,text1,text2,text3,text4)
	--UI OnInit
	--reg callback
	AsControl:AttachInfoListener(UIMessageHandler)
	MsgHandler[0] = function (Type,Msg) Metro.Home:HandleMsg(Type,Msg) end
	
	
end
function PopWindow(X,Y,type,id,defaultBtnText,otherBtnText,isRightDefault,isTick,text1,text2,text3,text4)
	local path = __document
	local index = string.find(path, "/[^/]*$")
	local layoutDir = string.sub(path,1,index)
	XLLoadModule(layoutDir.."PopWindow.xml.lua").PopWindow(X,Y,type,id,defaultBtnText,otherBtnText,isRightDefault,isTick,text1,text2,text3,text4)
end
local PopWindowLocation={X = 0,Y = 0}
MsgHandler = 
{
	--0 通知主界面的“运行程序地址”
	
}
function UIMessageHandler(Type,Msg,A,B,C,D,E,F,G,H,I)
	if Type == 20 or Type == 21 then
		
		PopWindow(PopWindowLocation.X,PopWindowLocation.Y,Msg,A,B,C,D,E,F,G,H,I)

		return
	end
	Metro.Home:HandleMsg(Type,Msg) 
	Metro.Scanning:HandleMsg(Type,Msg,A,B,C) 
	if Type == 19 then
		PopWindowLocation.X = Msg
		PopWindowLocation.Y = A
	end
end

CurrentIndex = 0
function SwitchMetro(MetroIndex)
	local new = Metro[MetroIndex] 
	if new == nil then
		return
	end

	if CurrentIndex == MetroIndex then 
		return 
	end
	CurrentIndex = MetroIndex 

	NaviBar:Navi(MetroIndex)

	new:SetChildrenVisible (true)
	OldMetro = CurrentMetro
	CurrentMetro = Metro[MetroIndex]
	local PosAni = AniFactory:CreateAnimation("PosChangeAnimation")
	PosAni:SetTotalTime(300)
	PosAni:SetKeyFrameRect(MetroLeft,MetroHiddenTop,MetroRight,MetroHiddenBottom,MetroLeft,MetroTop,MetroRight,MetroBottom)
	PosAni:BindObj(new)
	PosAni:AttachListener(true,AniFinish)
	PosAni:BindCurveID("Curve.Metro")
	ObjectTree:AddAnimation(PosAni)
	PosAni:Resume()

	local PosAni2 = AniFactory:CreateAnimation("PosChangeAnimation")
	PosAni2:SetTotalTime(100)
	PosAni2:SetKeyFrameRect(MetroLeft,MetroTop,MetroRight,MetroBottom,MetroLeft,MetroHiddenTop,MetroRight,MetroHiddenBottom)
	PosAni2:BindObj(OldMetro)
	PosAni2:AttachListener(true,AniFinish)
	ObjectTree:AddAnimation(PosAni2)
	PosAni2:Resume()

	local AlphaAni = AniFactory:CreateAnimation("AlphaChangeAnimation")
	AlphaAni:BindRenderObj(OldMetro)
	AlphaAni:SetTotalTime(100)
	AlphaAni:SetKeyFrameAlpha(255,0)
	ObjectTree:AddAnimation(AlphaAni)
	AlphaAni:Resume()

	local AlphaAni2 = AniFactory:CreateAnimation("AlphaChangeAnimation")
	AlphaAni:BindRenderObj(new)
	AlphaAni:SetTotalTime(100)
	AlphaAni:SetKeyFrameAlpha(100,255)
	ObjectTree:AddAnimation(AlphaAni)
	AlphaAni:Resume()
end

function AniFinish(self,oldState,newState)
	if newState == 4 then
		OldMetro:SetChildrenVisible (false)
		OldMetro:SetObjPos(MetroLeft,MetroHiddenTop,MetroRight,MetroHiddenBottom)
		collectgarbage()
	end
end

