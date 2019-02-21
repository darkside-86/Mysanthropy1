-- callbacks
function OnSaveBtnClicked()
    local text = "res/tilemaps/" .. mainWindow.fileTf:GetText() .. ".bin"
    TileEditor_SaveMap(text)
end

function OnLoadBtnClicked()
    local text = "res/tilemaps/" .. mainWindow.fileTf:GetText() .. ".bin"
    TileEditor_LoadMap(text)
end

-- main ui
mainWindow = Window.New(nil, 250, 450, "Options", "sans14")
mainWindow:SetXPos(GetScreenWidth() - mainWindow:GetWidth())
mainWindow:SetYPos(GetScreenHeight() - mainWindow:GetHeight())

mainWindow.fileTf = UITextField.New(mainWindow, mainWindow:GetWidth(), 20, "sans14", TEXTURE_UIBLANK)
mainWindow.fileTf:SetYPos(mainWindow.closeBtn:GetYPos() + mainWindow.closeBtn:GetHeight() + 5)

mainWindow.saveBtn = UIButton.New(mainWindow, TEXTURE_UIBLANK, "Save", "sans14", 5)
mainWindow.saveBtn:SetTextColor(1,1,1,1)
mainWindow.saveBtn:SetWidth(mainWindow:GetWidth() / 2)
mainWindow.saveBtn:SetYPos(mainWindow.fileTf:GetYPos() + mainWindow.fileTf:GetHeight())
mainWindow.saveBtn:SetBorderSize(1)
mainWindow.saveBtn:SetBorderColor(1,1,1,1)
mainWindow.saveBtn:SetColor(0,0,1,0)
mainWindow.saveBtn:AddOnClicked(OnSaveBtnClicked)

mainWindow.loadBtn = UIButton.New(mainWindow, TEXTURE_UIBLANK, "Load", "sans14", 5)
mainWindow.loadBtn:SetTextColor(1,1,1,1)
mainWindow.loadBtn:SetWidth(mainWindow:GetWidth() / 2)
mainWindow.loadBtn:SetXPos(mainWindow.saveBtn:GetXPos() + mainWindow.saveBtn:GetWidth())
mainWindow.loadBtn:SetYPos(mainWindow.saveBtn:GetYPos())
mainWindow.loadBtn:SetBorderSize(1)
mainWindow.loadBtn:SetBorderColor(1,1,1,1)
mainWindow.loadBtn:SetColor(0,0,1,0)
mainWindow.loadBtn:AddOnClicked(OnLoadBtnClicked)

