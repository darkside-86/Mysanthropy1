
-- create the main form
window = Window.New(nil, GetScreenWidth() / 5, GetScreenHeight()-1, "CubeControlForm", "mono")

-- creates a slider with text next to it
local function CreateSlideForm(name, txt, anchor, r,g,b,a)
    window[name] = UIFrame.New(window, window:GetWidth(), window:GetHeight()/10, 0,
            anchor:GetYPos()+anchor:GetHeight()+10, "uiblank")
    window[name]:SetColor(0,0,0,0)
    window[name].label = UILabel.New(window[name], txt, "vollkorn", r,g,b,a)
    window[name].slider = UISlider.New(window[name], window[name]:GetWidth() - window[name].label:GetWidth(),
            window[name].label:GetHeight(), "horizontal", "uiblank")
    window[name].slider:SetXPos(window[name].label:GetWidth())
    window[name]:SetHeight(window[name].slider:GetHeight())
end

-- create each slider control
CreateSlideForm("redForm", "R", window.title, 1,0,0,1)
CreateSlideForm("greenForm", "G", window.redForm, 0,1,0,1)
CreateSlideForm("blueForm", "B", window.greenForm, 0,0,1,1)
CreateSlideForm("alphaForm", "A", window.blueForm, 1,1,1,1)
CreateSlideForm("rotationForm", "<>", window.alphaForm, 1,1,1,1)
window.rotationForm:SetYPos(window.rotationForm:GetYPos() + 10)

-- set value controls via TestGame bindings
window.redForm.slider:AddOnDragged(function() TestGame_SetR(window.redForm.slider:GetValue()) end)
window.greenForm.slider:AddOnDragged(function() TestGame_SetG(window.greenForm.slider:GetValue()) end)
window.blueForm.slider:AddOnDragged(function() TestGame_SetB(window.blueForm.slider:GetValue()) end)
window.alphaForm.slider:AddOnDragged(function() TestGame_SetA(window.alphaForm.slider:GetValue()) end)
window.rotationForm.slider:AddOnDragged(function() TestGame_SetAngle(window.rotationForm.slider:GetValue()) end)

-- disable close button for form
window.closeBtn:SetVisible(false)
window.closeBtn = nil