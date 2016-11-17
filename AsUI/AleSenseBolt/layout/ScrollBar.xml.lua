function RouteToFather__OnChar(self, char)
    if char == 9 then
        self:RouteToFather()
    end
end

function OnBtnFocusChange(self, focus)
    local ctrl = self:GetOwnerControl()
    ctrl:FireExtEvent("OnScrollBarFocusChange", focus)
end

function SetState(self, state)
    local attr = self:GetAttribute()
    if attr.state == state then
        return
    end
    local bkg = self:GetControlObject("scrollbar.bkn")
    if state == 0 then
        bkg:SetTextureID(attr.NormalBkn)
    elseif state == 1 then
        bkg:SetTextureID(attr.HoverBkn)
    end
end

function SetEnable(self, enable)
    local attr = self:GetAttribute()
    attr.Enable = enable
    local thumbtn = self:GetControlObject("scrollbar.thumbbtn")
    local leftbtn = self:GetControlObject("scrollbar.leftbtn")
    local rightbtn = self:GetControlObject("scrollbar.rightbtn")
    if enable then
        self:UpdateThumbPos()
    else
        thumbtn:SetVisible(enable, true)
        leftbtn:SetEnable(enable)
        rightbtn:SetEnable(enable)
    end
end

function GetEnable(self)
    local attr = self:GetAttribute()
    if attr.Enable == nil then
        attr.Enable = true
    end
    return attr.Enable
end

function IsShow(self)
    local attr = self:GetAttribute()
    if attr.Visible == nil then
        attr.Visible = true
    end
    return attr.Visible
end

function Show(self, visible)
    local attr = self:GetAttribute()
    attr.Visible = visible
    local bkg = self:GetControlObject("scrollbar.bkn")
    bkg:SetChildrenVisible(visible)
    bkg:SetVisible(visible)
    self:SetVisible(visible)
    self:FireExtEvent("OnVisibleChanged", visible)
end

function UpdateThumbPos(self)
    local attr = self:GetAttribute()
    local type = attr.Type;
    self:Show(true)
    local thumbBtn = self:GetControlObject("scrollbar.thumbbtn")
    local leftbtn = self:GetControlObject("scrollbar.leftbtn")
    local rightbtn = self:GetControlObject("scrollbar.rightbtn")
    

    local left,top,right,bottom = self:GetObjPos()
    local width = right - left
    local height = bottom - top
    

    local trackLen;
    if type == 0 then
        trackLen = width - attr.SideBtnLength * 2;
    else
        trackLen = height - attr.SideBtnLength * 2;
    end
    
    if attr.PageSize == 0 then
        self:Show(false)
        return
    end
    
    local range = attr.RangeEnd - attr.RangeBegin;
    if range <= 0 then
        self:Show(false)
        return
    end
            
    local thumbLen = trackLen * attr.PageSize / (range + attr.PageSize)
    if thumbLen < attr.ThumbBtnLength then
        thumbLen = attr.ThumbBtnLength
    end
    

    if trackLen <= thumbLen then
        thumbBtn:SetVisible(false, true)
        return
    end
    
    thumbBtn:SetVisible(true, true)
    leftbtn:SetEnable(true)
    rightbtn:SetEnable(true)
    local thumbPos = attr.SideBtnLength + (attr.ScrollPos - attr.RangeBegin)*(trackLen - thumbLen)/(range)
    
    if type == 0 then
        thumbBtn:SetObjPos(thumbPos, height - attr.ThumbRightMagrin - attr.ThumbBtnWidth, thumbPos + thumbLen, height - attr.ThumbRightMagrin)
    else
        thumbBtn:SetObjPos(width - attr.ThumbRightMagrin - attr.ThumbBtnWidth, thumbPos, width - attr.ThumbRightMagrin, thumbPos + thumbLen)
    end
    
    if attr.ScrollPos == attr.RangeBegin then
        leftbtn:SetEnable(false)
    elseif attr.ScrollPos == attr.RangeEnd then
        rightbtn:SetEnable(false)
    end
end

function UpdatePos(self)              
    local attr = self:GetAttribute()
    local type = attr.Type
                
    local left,top,right,bottom = self:GetObjPos()
    local width = right - left
    local height = bottom - top
    
    local bkn = self:GetControlObject("scrollbar.bkn")
    bkn:SetObjPos(0,0,width, height)
    bkn:SetVisible(true)
    local leftBtn = self:GetControlObject("scrollbar.leftbtn")
    local rightBtn = self:GetControlObject("scrollbar.rightbtn")

    if type == 0 then
        leftBtn:SetObjPos(0, 0, attr.SideBtnLength, height)
    else 
        leftBtn:SetObjPos(0, 0, width, attr.SideBtnLength)
    end
                
    if type == 0 then
        rightBtn:SetObjPos(width - attr.SideBtnLength, 0, width, height)
    else 
        rightBtn:SetObjPos(0, height - attr.SideBtnLength, width, height)
    end
    
    self:UpdateThumbPos()			
end

function GetThumbPos(self)
    local attr = self:GetAttribute()
    local thumbBtn = self:GetControlObject("scrollbar.thumbbtn")
    local thumbLeft, thumbTop, thumbRight, thumbBottom = thumbBtn:GetObjPos()
    if attr.Type == 0 then
        return thumbLeft
    else
        return thumbTop
    end
end

function SetThumbPos(self, newPos)
    local attr = self:GetAttribute()
    local thumbBtn = self:GetControlObject("scrollbar.thumbbtn")
    local thumbLeft, thumbTop, thumbRight, thumbBottom = thumbBtn:GetObjPos()
    local thumbWidth = thumbRight - thumbLeft
    local thumbHeight = thumbBottom - thumbTop
    local left,top,right,bottom = self:GetObjPos()
    local thumbLength
    
    if attr.Type == 0 then
        if newPos < attr.SideBtnLength then
            newPos = attr.SideBtnLength
        elseif newPos > right -left - attr.SideBtnLength - thumbWidth then
            newPos = right - left - attr.SideBtnLength - thumbWidth
        end
        
        thumbLeft = newPos
        thumbRight = thumbLeft + thumbWidth
        trackLength = right - left - attr.SideBtnLength * 2
        thumbLength = thumbWidth
    else
        if newPos < attr.SideBtnLength then
            newPos = attr.SideBtnLength
        elseif newPos > bottom -top - attr.SideBtnLength - thumbHeight then
            newPos = bottom - top - attr.SideBtnLength - thumbHeight
        end
        
        thumbTop = newPos
        thumbBottom = thumbTop + thumbHeight
        trackLength = bottom - top - attr.SideBtnLength * 2
        thumbLength = thumbHeight
    end
    
    local newScrollPos = (newPos - attr.SideBtnLength) * (attr.RangeEnd - attr.RangeBegin) / (trackLength - thumbLength) + attr.RangeBegin
    thumbBtn:SetObjPos(thumbLeft, thumbTop, thumbRight, thumbBottom)
    attr.ScrollPos = newScrollPos
    if attr.Type == 0 then
        self:FireExtEvent("OnHScroll", 4, newScrollPos)
    else
        self:FireExtEvent("OnVScroll", 4, newScrollPos)
    end
end

function SetScrollPos(self, newPos, update)
    local attr = self:GetAttribute()
    
    if newPos < attr.RangeBegin then
        newPos = attr.RangeBegin
    elseif newPos > attr.RangeEnd then
        newPos = attr.RangeEnd
    end
    
    attr.ScrollPos = newPos
    if update then
        self:UpdateThumbPos()
    end
end

function SetScrollRange(self, rangeBegin, rangeEnd, update)
    local attr = self:GetAttribute()
    
    if rangeBegin > rangeEnd then
        return
    end
    
    attr.RangeBegin = rangeBegin
    attr.RangeEnd = rangeEnd
    
    if attr.ScrollPos > attr.RangeEnd then
        attr.ScrollPos = attr.RangeEnd
    end
    
    if update then
        self:UpdateThumbPos()
    end
end

function GetScrollPos(self)
    local attr = self:GetAttribute()
    return attr.ScrollPos
end

function GetScrollRange(self)
    local attr = self:GetAttribute()
    return attr.RangeBegin, attr.RangeEnd
end

function GetPageSize(self)
    local attr = self:GetAttribute()
    return attr.PageSize
end

function SetPageSize(self ,pagesize, update)
    local attr = self:GetAttribute()
    attr.PageSize = pagesize
    if update then
        self:UpdateThumbPos()
    end
end

function LBtn_OnLeftBtnDown(self)
    local sb = self:GetOwnerControl()
    local attr = sb:GetAttribute()
    if not attr.Enable then
        return 0, true
    end
    if attr.Type == 0 then
        sb:FireExtEvent("OnHScroll", 1,0)
    else
        sb:FireExtEvent("OnVScroll", 1,0)
    end
    return 0, true
end

function RBtn_OnLeftBtnDown(self)   
    local sb = self:GetOwnerControl()
    local attr = sb:GetAttribute()
    if not attr.Enable then
        return 0, true
    end
    if attr.Type == 0 then
        sb:FireExtEvent("OnHScroll", 2,0)
    else
        sb:FireExtEvent("OnVScroll", 2,0)
    end
    return 0, true
end

function ThumbBtn_OnMouseMove(self, x, y, flag)   
    local sb = self:GetOwnerControl()
    local attr = sb:GetAttribute()
    
    if not attr.TrackMouse then
        return 0, true
    end
    
    if BitAnd(flag, 1) == 0 then
        return 0, true
    end
    
    --[[if flag == 0 then
        return 0, true
    end]]
    
    local left,top,right,bottom = self:GetObjPos()

    local offset
    if attr.Type == 0 then
        x = left + x
        offset = x - attr.TrackMousePos
        attr.TrackMousePos = x
    else
        y = top + y
        offset = y - attr.TrackMousePos
        attr.TrackMousePos = y
    end
    
    if offset == 0 then
        return 0, true
    elseif offset < 0 then
        if attr.ScrollPos <= attr.RangeBegin then
            return 0, true
        end
    elseif attr.ScrollPos >= attr.RangeEnd then
        return 0, true
    end

    if attr.Type == 0 then
        sb:SetThumbPos(left + offset)
    else
        sb:SetThumbPos(top + offset)
    end

    return 0, true
end

function ThumbBtn_OnLeftBtnDown(self, x, y)   
    local sb = self:GetOwnerControl()
    local attr = sb:GetAttribute()
    local left, top, right, bottom = self:GetObjPos()
    attr.TrackMouse = true
    self:SetCaptureMouse(true)
    if attr.Type == 0 then
        attr.TrackMousePos = x + left
    else
        attr.TrackMousePos = y + top
    end
    return 0, true
end

function ThumbBtn_OnLeftBtnUp(self)    
    local sb = self:GetOwnerControl()
    local attr =sb:GetAttribute()
    self:SetCaptureMouse(false)
    attr.TrackMouse = false
    attr.TrackMousePos = 0
    return 0, true
end

function OnBind(self)
    local attr = self:GetAttribute()
    attr.TrackMousePos = 0
    attr.OldTop = 0
    --ʨ׃ѳް
    local bkn = self:GetControlObject("scrollbar.bkn")
    bkn:SetTextureID(attr.NormalBkn)
    local leftBtn = self:GetControlObject("scrollbar.leftbtn")
    local rightBtn = self:GetControlObject("scrollbar.rightbtn")
    --ʨ׃سӠݽͷ
    local leftBtnAttr = leftBtn:GetAttribute()
    leftBtnAttr.NormalBkgID = attr.LeftBtn_normal;
    leftBtnAttr.DownBkgID = attr.LeftBtn_down;
    leftBtnAttr.HoverBkgID = attr.LeftBtn_hover;
    leftBtnAttr.DisableBkgID = attr.LeftBtn_normal;

    --ʨ׃ԒӠݽͷ
    local rightBtnAttr = rightBtn:GetAttribute()
    rightBtnAttr.NormalBkgID = attr.RightBtn_normal;
    rightBtnAttr.DownBkgID = attr.RightBtn_down;
    rightBtnAttr.HoverBkgID = attr.RightBtn_hover;
    rightBtnAttr.DisableBkgID = attr.RightBtn_normal;
    if attr.SideBtnLength ~= 0 then
        leftBtn:AttachListener("OnLButtonDown", false, self.LBtn_OnLeftBtnDown)
        rightBtn:AttachListener("OnLButtonDown",false, self.RBtn_OnLeftBtnDown)
    end
    
    --ʨ׃Ώ֯дť
    local thumbBtn = self:GetControlObject("scrollbar.thumbbtn")
    local thumbBtnAttr = thumbBtn:GetAttribute()
    thumbBtnAttr.NormalBkgID = attr.ThumbBtn_normal;
    thumbBtnAttr.DownBkgID = attr.ThumbBtn_down;
    thumbBtnAttr.HoverBkgID = attr.ThumbBtn_hover;
    thumbBtnAttr.DisableBkgID = attr.ThumbBtn_normal;
    
    self:UpdatePos()
    
    local left,top,right,bottom = self:GetObjPos()
    self:PushDirtyRect(left, top, right, bottom)
    
    thumbBtn:AttachListener("OnMouseMove",false, self.ThumbBtn_OnMouseMove)
    thumbBtn:AttachListener("OnLButtonDown",false, self.ThumbBtn_OnLeftBtnDown)
    thumbBtn:AttachListener("OnLButtonUp",false, self.ThumbBtn_OnLeftBtnUp)
end

function OnPosChange(self)   
    if not self:GetOwner() then
        return
    end
    self:UpdatePos()
end

function OnLButtonDown(self, x, y)
    local attr = self:GetAttribute()
    if not attr.Enable then
        return 0, true
    end
    local thumbBtn = self:GetControlObject("scrollbar.thumbbtn")
    local left, top, right, bottom = thumbBtn:GetObjPos();
    local upordown
    local attr = self:GetAttribute()
    if attr.Type == 0 then
        if x < left then
            self:FireExtEvent("OnHScroll", 3, -1)
        elseif x > right then
            self:FireExtEvent("OnHScroll", 3, 1)
        end
    else
        if y < top then
            self:FireExtEvent("OnVScroll", 3, -1)
        elseif y > bottom then
            self:FireExtEvent("OnVScroll", 3, 1)
        end
    end
end

function OnMouseMove(self)
    self:SetState(1)
end

function OnMouseLeave(self, x, y)
    local left, top, right, bottom = self:GetObjPos()
    local width, height = right - left, bottom - top
    if x < 0 or x > width or y <= 0 or y >= height then
        self:SetState(0)
    end
end

function OnButtonMouseMove(self, name, x, y)
    local scrollbar = self:GetOwnerControl()
    scrollbar:SetState(1)
end

function OnButtonMouseLeave(self, name, x, y)
    local scrollbar = self:GetOwnerControl()
    local attr = scrollbar:GetAttribute()
    local btnleft, btntop, btnright, btnbottom = self:GetObjPos()
    local sbleft, sbtop, sbright, sbbottom = scrollbar:GetObjPos()
    if attr.Type == 1 then
        if x + btnleft <= sbleft or x + btnleft >= sbright then
            scrollbar:SetState(0)
        end
    else
        if y + btntop <= sbtop or y + btntop >= sbbottom then
            scrollbar:SetState(0)
        end
    end
end

function OnInitControl(self)
    local attr = self:GetAttribute()
    local leftBtn = self:GetControlObject("scrollbar.leftbtn")
    local rightBtn = self:GetControlObject("scrollbar.rightbtn")
    if attr.Visible ~= nil then
        self:Show(attr.Visible)
    end
    if attr.Enable == nil then
        attr.Enable = true
    end
    self:SetEnable(attr.Enable)
end

function OnMouseWheel(self, x, y, distance)
    self:FireExtEvent("OnScrollBarMouseWheel", x, y, distance)
end

function ThumbBtn__OnMouseWheel(self, x, y, distance)
    local sb = self:GetOwnerControl()
    sb:OnMouseWheel(x, y, distance)
end