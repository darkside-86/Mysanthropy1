-- callbacks
function OnSaveBtnClicked()
    local text = "res/tilemaps/" .. mainWindow.fileTf:GetText() .. ".bin"
    TileEditor_SetScriptPath(entWindow.scriptPathTf:GetText())
    TileEditor_SaveMap(text)
end

function UpdateMapName(name)
    mainWindow.fileTf:SetText(name)
end

function OnLoadBtnClicked()
    local text = "res/tilemaps/" .. mainWindow.fileTf:GetText() .. ".bin"
    TileEditor_LoadMap(text)
    entWindow.scriptPathTf:SetText(TileEditor_GetScriptPath())
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

function OnCollisionCBClicked()
    if mainWindow.collisionCB:GetText() == "[_]" then 
        mainWindow.collisionCB:SetText("[X]")
        TileEditor_SetCollisionLayer(true)
    else 
        mainWindow.collisionCB:SetText("[_]")
        TileEditor_SetCollisionLayer(false)
    end
end

function UpdateHoverData()
    -- g_hoverX, g_hoverY
    dataWindow.hoverTf:SetText("Tile " .. tostring(g_hoverX) .. ", " .. tostring(g_hoverY))
end

function OnPlaceBtnClicked()
    TileEditor_PlaceEntity(tonumber(entWindow.selectionTf:GetText()))
end

function OnRemoveBtnClicked()
    TileEditor_RemoveEntity(tonumber(entWindow.selectionTf:GetText()))
end

function OnGetBtnClicked()
    TileEditor_SelectEntity(true)
end

function UpdateEntitySelection(id)
    entWindow.selectionTf:SetText(tostring(id))
end

function MobOnPlaceBtnClicked()
    TileEditor_PlaceSpawner(tonumber(mobWindow.idTf:GetText()), tonumber(mobWindow.freqTf:GetText()),
        tonumber(mobWindow.chanceTf:GetText()))
end

function MobOnRemoveBtnClicked()
    TileEditor_RemoveSpawner(tonumber(mobWindow.idTf:GetText()))
end

function MobOnSelectBtnClicked()
    TileEditor_SelectSpawner()
end

function UpdateSpawnerSelection(id, freq, chance)
    mobWindow.idTf:SetText(tostring(id))
    mobWindow.freqTf:SetText(tostring(freq))
    mobWindow.chanceTf:SetText(tostring(chance))
end

-- main ui
mainWindow = Window.New(nil, 250, 450, "Options", "sans14")
mainWindow:SetXPos(GetScreenWidth() - mainWindow:GetWidth())
mainWindow:SetYPos(GetScreenHeight() - mainWindow:GetHeight())
mainWindow.closeBtn:SetVisible(false)

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
mainWindow.newBtn:SetTextColor(1,1,1,1)
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

mainWindow.collisionCB = UILabel.New(mainWindow, "[_]", "sans14", 1,1,1,1)
mainWindow.collisionCB:SetYPos(mainWindow.layerCB:GetYPos() + mainWindow.layerCB:GetHeight() + 5)
mainWindow.collisionCB:AddOnClicked(OnCollisionCBClicked)

mainWindow.label5 = UILabel.New(mainWindow, "Toggle collision layer", "sans14", 1,1,1,1)
mainWindow.label5:SetYPos(mainWindow.collisionCB:GetYPos())
mainWindow.label5:SetXPos(mainWindow.collisionCB:GetWidth() + 1)

dataWindow = Window.New(nil, 75, 75, "Data", "sans14")
dataWindow:SetXPos(mainWindow:GetXPos() - dataWindow:GetWidth())
dataWindow:SetYPos(mainWindow:GetYPos() + mainWindow:GetHeight() - dataWindow:GetHeight())
dataWindow.hoverTf = UILabel.New(dataWindow, "Tile _, _", "sans14", 1, 1, 1, 1)
dataWindow.hoverTf:SetYPos(dataWindow.closeBtn:GetHeight()+5)
dataWindow.closeBtn:SetVisible(false)

-- used to associate map script file with map and place entities
entWindow = Window.New(nil, 200, 150, "Entities", "sans14")
entWindow:SetXPos(0)
entWindow:SetYPos(mainWindow:GetYPos())
entWindow.closeBtn:SetVisible(false)

entWindow.label1 = UILabel.New(entWindow, "Script path", "sans14", 1,1,1,1)
entWindow.label1:SetYPos(entWindow.closeBtn:GetHeight())

entWindow.scriptPathTf = UITextField.New(entWindow, entWindow:GetWidth(), 20, "sans14", TEXTURE_UIBLANK)
entWindow.scriptPathTf:SetYPos(entWindow.label1:GetYPos() + entWindow.label1:GetHeight())
entWindow.scriptPathTf:SetText(TileEditor_GetScriptPath())

entWindow.label2 = UILabel.New(entWindow, "Selected entity: ", "sans14", 1,1,1,1)
entWindow.label2:SetYPos(entWindow.scriptPathTf:GetYPos() + entWindow.scriptPathTf:GetHeight())

entWindow.selectionTf = UITextField.New(entWindow, entWindow:GetWidth() - entWindow.label2:GetWidth(), 20,
                                        "sans14", TEXTURE_UIBLANK)
entWindow.selectionTf:SetXPos(entWindow.label2:GetWidth())
entWindow.selectionTf:SetYPos(entWindow.label2:GetYPos())

entWindow.placeBtn = UIButton.New(entWindow, TEXTURE_UIBLANK, "Place", "sans14", 5)
entWindow.placeBtn:SetYPos(entWindow.selectionTf:GetYPos()+entWindow.selectionTf:GetHeight())
entWindow.placeBtn:AddOnClicked(OnPlaceBtnClicked)

entWindow.removeBtn = UIButton.New(entWindow, TEXTURE_UIBLANK, "Remove", "sans14", 5)
entWindow.removeBtn:SetYPos(entWindow.placeBtn:GetYPos())
entWindow.removeBtn:SetXPos(entWindow.placeBtn:GetWidth() + 10)
entWindow.removeBtn:AddOnClicked(OnRemoveBtnClicked)

entWindow.getBtn = UIButton.New(entWindow, TEXTURE_UIBLANK, "Select", "sans14", 5)
entWindow.getBtn:SetYPos(entWindow.removeBtn:GetYPos())
entWindow.getBtn:SetXPos(entWindow.removeBtn:GetWidth() + entWindow.removeBtn:GetXPos() + 10)
entWindow.getBtn:AddOnClicked(OnGetBtnClicked)

-- mob spawner window
mobWindow = Window.New(nil, 200, 200, "Mob spawners", "sans14")
mobWindow:SetYPos(entWindow:GetYPos() + entWindow:GetHeight())
mobWindow.closeBtn:SetVisible(false)
mobWindow.label1 = UILabel.New(mobWindow, "ID:", "sans14", 1,1,1,1)
mobWindow.label1:SetYPos(mobWindow.closeBtn:GetHeight())
mobWindow.idTf = UITextField.New(mobWindow, 55, 20, "sans14", TEXTURE_UIBLANK)
mobWindow.idTf:SetXPos(mobWindow.label1:GetWidth())
mobWindow.idTf:SetYPos(mobWindow.label1:GetYPos())
mobWindow.idTf:SetWidth(mobWindow:GetWidth() / 2 - mobWindow.label1:GetWidth())
mobWindow.label2 = UILabel.New(mobWindow, "Freq:", "sans14", 1, 1, 1, 1)
mobWindow.label2:SetYPos(mobWindow.idTf:GetYPos() + mobWindow.idTf:GetHeight())
mobWindow.freqTf = UITextField.New(mobWindow, 55, 20, "sans14", TEXTURE_UIBLANK)
mobWindow.freqTf:SetXPos(mobWindow.label2:GetXPos() + mobWindow.label2:GetWidth())
mobWindow.freqTf:SetYPos(mobWindow.label2:GetYPos())
mobWindow.freqTf:SetWidth(mobWindow:GetWidth()/2 - mobWindow.label2:GetWidth())
mobWindow.label3 = UILabel.New(mobWindow, "Chance:", "sans14", 1, 1, 1, 1)
mobWindow.label3:SetYPos(mobWindow.freqTf:GetYPos() + mobWindow.freqTf:GetHeight())
mobWindow.chanceTf = UITextField.New(mobWindow, 55, 20, "sans14", TEXTURE_UIBLANK)
mobWindow.chanceTf:SetYPos(mobWindow.label3:GetYPos())
mobWindow.chanceTf:SetXPos(mobWindow.label3:GetWidth())
mobWindow.chanceTf:SetWidth(mobWindow:GetWidth() / 2 - mobWindow.label3:GetWidth())
mobWindow.placeBtn = UIButton.New(mobWindow, TEXTURE_UIBLANK, "Place", "sans14", 2)
mobWindow.placeBtn:SetXPos( mobWindow:GetWidth() / 2 )
mobWindow.placeBtn:SetYPos( mobWindow.closeBtn:GetHeight() )
mobWindow.placeBtn:SetBorderSize(1)
mobWindow.placeBtn:SetBorderColor(0,1,0,1)
mobWindow.placeBtn:AddOnClicked(MobOnPlaceBtnClicked)
mobWindow.removeBtn = UIButton.New(mobWindow, TEXTURE_UIBLANK, "Remove", "sans14", 2)
mobWindow.removeBtn:SetXPos( mobWindow.placeBtn:GetXPos() + mobWindow.placeBtn:GetWidth() )
mobWindow.removeBtn:SetYPos( mobWindow.placeBtn:GetYPos() )
mobWindow.removeBtn:SetBorderSize(1)
mobWindow.removeBtn:SetBorderColor(1,0,0,1)
mobWindow.removeBtn:AddOnClicked(MobOnRemoveBtnClicked)
mobWindow.selectBtn = UIButton.New(mobWindow, TEXTURE_UIBLANK, "Select", "sans14", 2)
mobWindow.selectBtn:SetXPos( mobWindow.placeBtn:GetXPos() )
mobWindow.selectBtn:SetYPos( mobWindow.placeBtn:GetYPos() + mobWindow.placeBtn:GetHeight() )
mobWindow.selectBtn:SetBorderSize(1)
mobWindow.selectBtn:SetBorderColor(0,0,1,1)
mobWindow.selectBtn:AddOnClicked(MobOnSelectBtnClicked)



