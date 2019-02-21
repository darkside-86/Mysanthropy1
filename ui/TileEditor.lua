-- callbacks
function OnSaveBtnClicked()
    local text = "res/tilemaps/" .. mainWindow.fileTf:GetText() .. ".bin"
    TileEditor_SaveMap(text)
end

function OnLoadBtnClicked()
    local text = "res/tilemaps/" .. mainWindow.fileTf:GetText() .. ".bin"
    TileEditor_LoadMap(text)
end

function OnNewBtnClicked()
    local tilesetPath = "res/textures/tilesets/" .. mainWindow.tilesetTf:GetText() .. ".png"
    local tileWidth = tonumber(mainWindow.tileWidthTf:GetText())
    local tileHeight = tonumber(mainWindow.tileHeightTf:GetText())
    local mapWidth = tonumber(mainWindow.mapWidthTf:GetText())
    local mapHeight = tonumber(mainWindow.mapHeightTf:GetText())
    TileEditor_NewMap(tileWidth, tileHeight, tilesetPath, mapWidth, mapHeight)
end

function OnFillBtnClicked()
    TileEditor_FillWithSelection()
end

function OnLayerCBClicked()
    if mainWindow.layerCB:GetText() == "[_]" then 
        mainWindow.layerCB:SetText("[X]")
        TileEditor_SetSelectedLayer(1)
    else 
        mainWindow.layerCB:SetText("[_]")
        TileEditor_SetSelectedLayer(0)
    end
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

mainWindow.label1 = UILabel.New(mainWindow, "Tileset", "sans14", 1,1,1,1)
mainWindow.label1:SetYPos(mainWindow.loadBtn:GetYPos() + mainWindow.loadBtn:GetHeight())

mainWindow.tilesetTf = UITextField.New(mainWindow, mainWindow:GetWidth(), 20, "sans14", TEXTURE_UIBLANK)
mainWindow.tilesetTf:SetYPos(mainWindow.label1:GetYPos() + mainWindow.label1:GetHeight())

mainWindow.label2 = UILabel.New(mainWindow, "Tile width and height", "sans14", 1,1,1,1)
mainWindow.label2:SetYPos(mainWindow.tilesetTf:GetYPos() + mainWindow.tilesetTf:GetHeight())

mainWindow.tileWidthTf = UITextField.New(mainWindow, mainWindow:GetWidth() / 2 - 5, 20, "sans14", TEXTURE_UIBLANK)
mainWindow.tileWidthTf:SetYPos(mainWindow.label2:GetYPos() + mainWindow.label2:GetHeight())

mainWindow.tileHeightTf = UITextField.New(mainWindow, mainWindow:GetWidth() / 2 - 5, 20, "sans14", TEXTURE_UIBLANK)
mainWindow.tileHeightTf:SetYPos(mainWindow.tileWidthTf:GetYPos())
mainWindow.tileHeightTf:SetXPos(mainWindow:GetWidth() / 2)

mainWindow.label3 = UILabel.New(mainWindow, "Map width and height", "sans14", 1,1,1,1)
mainWindow.label3:SetYPos(mainWindow.tileHeightTf:GetYPos() + mainWindow.tileHeightTf:GetHeight())

mainWindow.mapWidthTf = UITextField.New(mainWindow, mainWindow:GetWidth() / 2 - 6, 20, "sans14", TEXTURE_UIBLANK)
mainWindow.mapWidthTf:SetYPos(mainWindow.label3:GetYPos() + mainWindow.label3:GetHeight())

mainWindow.mapHeightTf = UITextField.New(mainWindow, mainWindow:GetWidth() / 2 - 6, 20, "sans14", TEXTURE_UIBLANK)
mainWindow.mapHeightTf:SetYPos(mainWindow.mapWidthTf:GetYPos())
mainWindow.mapHeightTf:SetXPos(mainWindow:GetWidth() / 2)

mainWindow.newBtn = UIButton.New(mainWindow, TEXTURE_UIBLANK, "New", "sans14", 5)
mainWindow.newBtn:SetYPos(mainWindow.mapHeightTf:GetYPos() + mainWindow.mapHeightTf:GetHeight())
mainWindow.newBtn:SetWidth(mainWindow:GetWidth())
mainWindow.newBtn:SetColor(0,0,1,0)
mainWindow.newBtn:SetBorderSize(1)
mainWindow.newBtn:SetBorderColor(1,1,1,1)
mainWindow.newBtn:AddOnClicked(OnNewBtnClicked)

mainWindow.fillBtn = UIButton.New(mainWindow, TEXTURE_UIBLANK, "Fill with selection", "sans14", 5)
mainWindow.fillBtn:SetYPos(mainWindow.newBtn:GetYPos() + mainWindow.newBtn:GetHeight())
mainWindow.fillBtn:SetColor(1,0,0,1)
mainWindow.fillBtn:SetTextColor(1,1,1,1)
mainWindow.fillBtn:SetXPos(mainWindow:GetWidth() - mainWindow.fillBtn:GetWidth())
mainWindow.fillBtn:AddOnClicked(OnFillBtnClicked)

mainWindow.layerCB = UILabel.New(mainWindow, "[_]", "sans14", 1,1,1,1)
mainWindow.layerCB:SetYPos(mainWindow.fillBtn:GetYPos() + mainWindow.fillBtn:GetHeight())
mainWindow.layerCB:AddOnClicked(OnLayerCBClicked)

mainWindow.label4 = UILabel.New(mainWindow, "Toggle layer 1", "sans14", 1,1,1,1)
mainWindow.label4:SetYPos(mainWindow.layerCB:GetYPos())
mainWindow.label4:SetXPos(mainWindow.layerCB:GetXPos() + mainWindow.layerCB:GetWidth() + 1)