window = Window.New(nil, 500, 500, 'title', 'mono')

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