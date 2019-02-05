--[[window = Window.New(nil, 500, 500, 'title', 'mono')

window.slider = UISlider.New(window, 30, 300, "vertical", "uiwindow")
window.slider:SetYPos(30)
window.slider:SetKnobColor(1,1,1,1)
window.slider:SetSlideColor(0,0,0,1)
window.slider:AddOnDragged(function() 
   window.info:SetText(tostring(window.slider:GetValue() * 100))
end)

window.info = UILabel.New(window, "...", "vollkorn", 1,1,1,1)
window.info:SetXPos(30)
window.info:SetYPos(30)

window.tf = UITextField.New(window, 300, 60, "sans", "uiblank")
window.tf:SetXPos(50)
window.tf:SetYPos(50)

window.runBtn = UIButton.New(window, 'uiblank', 'Run', 'sans', 4)
window.runBtn:SetColor(0,0,0,0)
window.runBtn:SetBorderColor(1,1,1,1)
window.runBtn:SetBorderSize(1)
window.runBtn:SetXPos(window.tf:GetXPos() + window.tf:GetWidth())
window.runBtn:SetYPos(window.tf:GetYPos())
window.runBtn:AddOnClicked(function() 
   local f, er = load(window.tf:GetText())
   if f == nil then 
       print("Error: " .. er)
   else
       f() 
   end
end)

window.clearBtn = UIButton.New(window, 'uiblank', 'Clear', 'sans', 4)
window.clearBtn:SetColor(0,0,0,0)
window.clearBtn:SetBorderColor(1,1,1,1)
window.clearBtn:SetBorderSize(1)
window.clearBtn:SetXPos(window.runBtn:GetXPos())
window.clearBtn:SetYPos(window.runBtn:GetYPos() + window.runBtn:GetHeight())
window.clearBtn:AddOnClicked(function() 
    window.tf:SetText("")
end)
]]

print(GetScreenWidth(), GetScreenHeight())
window = Window.New(nil, GetScreenWidth() / 5, GetScreenHeight()-1, "CubeControlForm", "mono")

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

CreateSlideForm("redForm", "R", window.title, 1,0,0,1)
CreateSlideForm("greenForm", "G", window.redForm, 0,1,0,1)
CreateSlideForm("blueForm", "B", window.greenForm, 0,0,1,1)
CreateSlideForm("alphaForm", "A", window.blueForm, 1,1,1,1)
CreateSlideForm("rotationForm", "<>", window.alphaForm, 1,1,1,1)
window.rotationForm:SetYPos(window.rotationForm:GetYPos() + 10)

window.redForm.slider:AddOnDragged(function() TestGame_SetR(window.redForm.slider:GetValue()) end)
window.greenForm.slider:AddOnDragged(function() TestGame_SetG(window.greenForm.slider:GetValue()) end)
window.blueForm.slider:AddOnDragged(function() TestGame_SetB(window.blueForm.slider:GetValue()) end)
window.alphaForm.slider:AddOnDragged(function() TestGame_SetA(window.alphaForm.slider:GetValue()) end)
window.rotationForm.slider:AddOnDragged(function() TestGame_SetAngle(window.rotationForm.slider:GetValue()) end)