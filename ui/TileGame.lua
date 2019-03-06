print("Welcome to Despacito Island user interface 0.1")
local screenWidth = GetScreenWidth()
local screenHeight = GetScreenHeight()

-- Create yellow FPS counter in bottom right corner ---------------------------
-------------------------------------------------------------------------------
function UpdateFPS()
    fpsLbl:SetText("FPS: " .. tostring(GetFramesPerSecond()))
    fpsLbl:SetXPos(GetScreenWidth() - fpsLbl:GetWidth())
    fpsLbl:SetYPos(GetScreenHeight() - fpsLbl:GetHeight())
    fpsLbl:AddOnTimer(UpdateFPS, 1000)
end

fpsLbl = UILabel.New(nil, "FPS: ", "sans14", 1,1,0,1)
UpdateFPS()

-- Create console output box --------------------------------------------------
-------------------------------------------------------------------------------
consoleFrame = UIFrame.New(nil, GetScreenWidth() / 3, GetScreenHeight() / 4, 0, 0, TEXTURE_UIBLANK)
consoleFrame:SetColor(0,0,0,0.5)
consoleFrame:SetYPos(GetScreenHeight() - consoleFrame:GetHeight() - (consoleFrame:GetHeight()/5))
consoleFrame.lines = {} -- array of UILabels
consoleFrame.MAX_LINES = math.floor(consoleFrame:GetHeight() / 10)
consoleFrame:SetHeight(consoleFrame:GetHeight() + 10)

function WriteLineToConsole(text, r,g,b,a)
    print("[CONSOLE] " .. text) -- for debugging
    local newLine = UILabel.New(consoleFrame, text, "mono10", r,g,b,a)
    -- do we need to move up existing lines?
    if #consoleFrame.lines >= consoleFrame.MAX_LINES then 
        consoleFrame.lines[1]:SetVisible(false)
        table.remove( consoleFrame.lines, 1 )
        for i,v in ipairs(consoleFrame.lines) do
            v:SetYPos(v:GetYPos() - v:GetHeight())
        end
        newLine:SetYPos(consoleFrame.lines[#consoleFrame.lines]:GetYPos() + 10)
        table.insert( consoleFrame.lines, newLine )
    else -- just need to append it to the table
        table.insert( consoleFrame.lines, newLine )
        -- and set YPos relative to length-1 line
        if consoleFrame.lines[#consoleFrame.lines - 1] ~= nil then
            consoleFrame.lines[#consoleFrame.lines]:SetYPos(
                consoleFrame.lines[#consoleFrame.lines-1]:GetYPos() +
                consoleFrame.lines[#consoleFrame.lines-1]:GetHeight())
        end
    end
    -- fix line size distortion glitch by table.removing anything less than y=0
    local n = #consoleFrame.lines
    for j=1, n do 
        if consoleFrame.lines[j] ~= nil and consoleFrame.lines[j]:GetYPos() < 0 then 
            table.remove(consoleFrame.lines, j)
        end
    end
end

-- Create cast bar ------------------------------------------------------------
-------------------------------------------------------------------------------
castbar = UIFrame.New(nil, 250, 25, 0, 0, TEXTURE_UIBLANK)
castbar:SetXPos(GetScreenWidth() / 2 - castbar:GetWidth() / 2)
castbar:SetYPos(GetScreenHeight() - castbar:GetHeight() * 5)
castbar:SetColor(0,0,0.5,0.5)
castbar.filler = UIFrame.New(castbar, castbar:GetWidth(), castbar:GetHeight(), 0, 0, TEXTURE_UIBLANK)
castbar.filler:SetColor(1,1,0,0.5)
function SetCastBarValue(value)
    local w = value * castbar:GetWidth()
    castbar.filler:SetWidth(w)
end
function ToggleCastBar(show)
    castbar:SetVisible(show)
end
SetCastBarValue(0)
castbar:SetVisible(false) -- hide until we use it

-- Create experience bars (half of screen width) ------------------------------
-------------------------------------------------------------------------------
local halfScrW = GetScreenWidth() / 2
experienceBar = UIFrame.New(nil, halfScrW, 15, 0, 0, TEXTURE_UIBLANK)
experienceBar:SetColor(0.2,0.2,0.2,0.5)
experienceBar:SetXPos(halfScrW - experienceBar:GetWidth() / 2)
experienceBar:SetYPos(GetScreenHeight() - experienceBar:GetHeight())
experienceBar.fill = UIFrame.New(experienceBar, experienceBar:GetWidth()-2, experienceBar:GetHeight(), 
    0, 0, TEXTURE_UIBLANK)
experienceBar.fill:SetColor(0.5,0,0.5,0.8)
experienceBar.fill:SetWidth(0)
experienceBar.segments = {}
local NUM_EXP_SEGMENTS = 10
for i=1, NUM_EXP_SEGMENTS do 
    local w = experienceBar:GetWidth() / NUM_EXP_SEGMENTS
    local x = w * (i-1)
    experienceBar.segments[i] = UIFrame.New(experienceBar, w, experienceBar:GetHeight(), x, 0, TEXTURE_UIBLANK)
    experienceBar.segments[i]:SetBorderSize(1)
    experienceBar.segments[i]:SetBorderColor(0.8,0.8,0.8,1)
    experienceBar.segments[i]:SetColor(0.2,0.2,0.2,0.0)
end
function SetExperienceBar(value)
    experienceBar.fill:SetWidth( value * (experienceBar:GetWidth()))
end

-- Inventory backpack frame ---------------------------------------------------
-------------------------------------------------------------------------------
inventoryFrame = UIFrame.New(nil, screenWidth / 2.4, screenHeight / 2.0, 0, 0, TEXTURE_UIBLANK)
inventoryFrame:SetXPos(screenWidth - inventoryFrame:GetWidth())
inventoryFrame:SetYPos(screenHeight - inventoryFrame:GetHeight())
inventoryFrame:SetColor(0.1, 0.1, 0.1, 0.7)
inventoryFrame:SetBorderColor(0.5,0.5,0.5,0.7)
inventoryFrame:SetBorderSize(2)
inventoryFrame:SetVisible(false)
function ShowInventory(show)
    inventoryFrame:SetVisible(show)
end

inventoryFrame.convertBtn = UIButton.New(inventoryFrame, TEXTURE_UIBLANK, 
    "Convert food to foodstuff", "sans14", 2)
inventoryFrame.convertBtn:SetColor(0.2,0.2,1,0.7)
inventoryFrame.convertBtn:SetBorderSize(1)
inventoryFrame.convertBtn:SetBorderColor(1,1,1,0.7)
inventoryFrame.convertingFood = false
inventoryFrame.convertBtn:AddOnClicked(function() 
    if inventoryFrame.convertingFood == false then 
        inventoryFrame.convertingFood = true 
        inventoryFrame.convertBtn:SetColor(1,0.2,0.2,0.7)
    else
        inventoryFrame.convertingFood = false 
        inventoryFrame.convertBtn:SetColor(0.2,0.2,1,0.7)
    end
end)

inventoryFrame.panel = UIFrame.New(inventoryFrame, inventoryFrame:GetWidth(), 
    inventoryFrame:GetHeight() - inventoryFrame.convertBtn:GetHeight(), 0,
    inventoryFrame.convertBtn:GetHeight() + inventoryFrame.convertBtn:GetYPos(), TEXTURE_UIBLANK)
inventoryFrame.panel:SetColor(0,0,0,0)
inventoryFrame.panel:AddOnClicked(function(x,y) 
    local clicked = nil
    if inventoryFrame.convertingFood == false then 
        return 
    end 
    for k,v in pairs(inventoryFrame.panel.elements) do
        -- determine which item was clicked
        if x >= v:GetXPos() and x <= v:GetXPos() + v:GetWidth() and 
          y >= v:GetYPos() and y <= v:GetYPos() + v:GetHeight() then 
            clicked = v
            break
        end
    end
    if clicked ~= nil then 
        local keepGoing = Game_ConvertItemToFoodstuff(clicked.name, 1)
        if keepGoing == false then 
            inventoryFrame.convertingFood = false 
            inventoryFrame.convertBtn:SetColor(0.2,0.2,1,0.7)
        else 
            -- update the number on success
            clicked.count = clicked.count - 1
            clicked.countLbl:SetText(clicked.count)
        end
        SetFoodstuffBarData(Game_GetFoodstuffCount())
    end
end)
-- build the inventory item list
function BuildInventory()
    local inventoryData = Game_GetInventory()
    if inventoryFrame.panel.elements ~= nil then 
        for k,v in pairs(inventoryFrame.panel.elements) do 
            v:SetVisible(false)
        end
    end
    inventoryFrame.panel.elements = {}
    local x = 0
    local y = 0
    for i,item in ipairs(inventoryData) do 
        inventoryFrame.panel.elements[item.name] = UITexture.New(
            inventoryFrame.panel, item.texture, 64, 64)
        inventoryFrame.panel.elements[item.name]:SetXPos(x)
        inventoryFrame.panel.elements[item.name]:SetYPos(y)
        inventoryFrame.panel.elements[item.name].name = item.name
        inventoryFrame.panel.elements[item.name].count = item.count
        inventoryFrame.panel.elements[item.name].countLbl = UILabel.New(
            inventoryFrame.panel.elements[item.name], tostring(item.count), "sans14", 1,0.5,0,1)
        x = x + 64
        if x > inventoryFrame.panel:GetWidth() - 64 then 
            x = 0
            y = y + 64
        end
    end
end
-- "scroll effect"
inventoryFrame.panel:AddOnDragged(function(x,y,dx,dy) 
    for k,v in pairs(inventoryFrame.panel.elements) do 
        v:SetYPos(v:GetYPos() + dy)
    end
end)
BuildInventory() -- initial call

-- food stuff bar at bottom left ----------------------------------------------
-------------------------------------------------------------------------------
foodstuffBar = UIFrame.New(nil, experienceBar:GetXPos() - 1, screenHeight 
    - (consoleFrame:GetHeight() + consoleFrame:GetYPos()), 0, 0, TEXTURE_UIBLANK)
foodstuffBar:SetYPos(consoleFrame:GetYPos() + consoleFrame:GetHeight())
foodstuffBar:SetColor(0.09,0.09,0.09,0.9)
foodstuffBar.fsIcon = UITexture.New(foodstuffBar, "res/textures/items/foodstuff.png", 16, 16)
foodstuffBar.label1 = UILabel.New(foodstuffBar, "Foodstuff: ", "sans14", 1,1,1,1)
foodstuffBar.label1:SetXPos( foodstuffBar.fsIcon:GetXPos() + foodstuffBar.fsIcon:GetWidth() )
foodstuffBar.label2 = UILabel.New(foodstuffBar, "0", "sans14", 1,1,0,1)
foodstuffBar.label2:SetXPos( foodstuffBar.label1:GetXPos() + foodstuffBar.label1:GetWidth() )
function SetFoodstuffBarData(num)
    foodstuffBar.label2:SetText(tostring(num))
end
-- Initial data
SetFoodstuffBarData(Game_GetFoodstuffCount())

-- Return to main menu popup --------------------------------------------------
-------------------------------------------------------------------------------
returnMM = UIFrame.New(nil, screenWidth / 5, screenHeight / 5, 0, 0, TEXTURE_UIBLANK )
returnMM:SetXPos( screenWidth / 2 - returnMM:GetWidth() / 2)
returnMM:SetYPos( screenHeight / 2 - returnMM:GetHeight() / 2)
returnMM:SetColor(0.2, 0.2, 0.8, 0.8)
returnMM:SetVisible(false)
returnMM.label1 = UILabel.New(returnMM, "Return to main menu?", "sans14", 1, 1, 1, 1)
returnMM.confirmBtn = UIButton.New(returnMM, TEXTURE_UIBLANK, "Confirm", "sans14", 10)
returnMM.confirmBtn:SetXPos(0)
returnMM.confirmBtn:SetYPos(returnMM.label1:GetYPos() + returnMM.label1:GetHeight())
returnMM.confirmBtn:SetColor(0,0,0,0)
returnMM.confirmBtn:SetBorderColor(1,1,1,1)
returnMM.confirmBtn:SetBorderSize(1)
returnMM.confirmBtn:AddOnClicked(function() 
    Game_ReturnToMainMenu()
    returnMM:SetVisible(false)
end)
returnMM.cancelBtn = UIButton.New(returnMM, TEXTURE_UIBLANK, "Cancel", "sans14", 10)
returnMM.cancelBtn:SetXPos(returnMM:GetWidth() - returnMM.cancelBtn:GetWidth())
returnMM.cancelBtn:SetYPos(returnMM.confirmBtn:GetYPos())
returnMM.cancelBtn:SetColor(0, 0, 0, 0)
returnMM.cancelBtn:SetBorderColor(1, 1, 1, 1)
returnMM.cancelBtn:SetBorderSize(1)
returnMM.cancelBtn:AddOnClicked(function() 
    returnMM:SetVisible(false)
end)
function ShowMMPopup()
    returnMM:SetVisible(true)
end
function ToggleMMPopup(show)
    returnMM:SetVisible(show)
end

-- create player unit frame ---------------------------------------------------
-------------------------------------------------------------------------------
playerUnitFrame = UIFrame.New(nil, 175, 75, 10, 10, TEXTURE_UIBLANK)
playerUnitFrame:SetColor(0.15, 0.15, 0.15, 1)
-- create the elements first then fill their data
playerUnitFrame.topLabel = UILabel.New(playerUnitFrame, "???, Level ?", "sans14", 1,1,1,1)
function PlayerUnitFrame_SetNameAndLevel(name, level)
    playerUnitFrame.topLabel:SetText(tostring(name) .. ", Level " .. tostring(level))
end
playerUnitFrame.healthBar = UIFrame.New(playerUnitFrame, playerUnitFrame:GetWidth(), 20, 0, 0, TEXTURE_UIBLANK)
playerUnitFrame.healthBar:SetYPos(playerUnitFrame.topLabel:GetHeight() + playerUnitFrame.topLabel:GetYPos())
playerUnitFrame.healthBar:SetColor(0, 0.75, 0, 1)
playerUnitFrame.healthBar.dataLbl = UILabel.New(playerUnitFrame.healthBar, "Health ? / ?", "sans14", 1, 1, 1, 1)
function PlayerUnitFrame_SetHealth(current, max)
    local percentage = current / max
    -- set text label first
    playerUnitFrame.healthBar.dataLbl:SetText("Health " .. tostring(current) .. " / " .. tostring(max))
    playerUnitFrame.healthBar:SetWidth(percentage * playerUnitFrame:GetWidth())
end






