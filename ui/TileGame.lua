print("TileGame UI 0.1")

function UpdateFPS()
    fpsLbl:SetText("FPS: " .. tostring(GetFramesPerSecond()))
    fpsLbl:SetXPos(GetScreenWidth() - fpsLbl:GetWidth())
    fpsLbl:SetYPos(GetScreenHeight() - fpsLbl:GetHeight())
    fpsLbl:AddOnTimer(UpdateFPS, 1000)
end

fpsLbl = UILabel.New(nil, "FPS: ", "sans14", 1,1,0,1)
UpdateFPS()
