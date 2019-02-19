fpsFrame = UIFrame.New(nil, 75, 25, 0, 0, TEXTURE_UIBLANK)
fpsFrame:SetYPos(GetScreenHeight() - fpsFrame:GetHeight())
fpsFrame:SetColor(0,0,0,0)
fpsFrame.text = UILabel.New(fpsFrame, "FPS: ?", "sans", 1,1,0,1)

function UpdateFPS()
    fpsFrame.text:SetText("FPS: " .. tostring(GetFramesPerSecond()))
    fpsFrame:AddOnTimer(UpdateFPS, 1000)
end

UpdateFPS()
