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

-- todo: organize unit frames into a Lua class

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

-- create target unit frame ---------------------------------------------------
-------------------------------------------------------------------------------
targetUnitFrame = UIFrame.New(nil, 175, 75, 10, 10, TEXTURE_UIBLANK)
targetUnitFrame:SetColor(0.15, 0.15, 0.15, 1)
targetUnitFrame:SetXPos(playerUnitFrame:GetWidth() + playerUnitFrame:GetXPos() + 1)
targetUnitFrame:SetYPos(playerUnitFrame:GetYPos())
-- create elements first then fill in data
targetUnitFrame.topLabel = UILabel.New(targetUnitFrame, "???, Level ?", "sans14", 1, 1, 1, 1)
function TargetUnitFrame_SetNameAndLevel(name, level)
    targetUnitFrame.topLabel:SetText(tostring(name) .. ", Level " .. tostring(level))
end
targetUnitFrame.healthBar = UIFrame.New(targetUnitFrame, targetUnitFrame:GetWidth(), 20, 0, 0, TEXTURE_UIBLANK)
targetUnitFrame.healthBar:SetYPos(targetUnitFrame.topLabel:GetHeight() + targetUnitFrame.topLabel:GetYPos())
targetUnitFrame.healthBar:SetColor( 0.4, 0.4, 0.4, 1) -- color will depend on hostility of target
targetUnitFrame.healthBar.dataLbl = UILabel.New(targetUnitFrame.healthBar, "Health ? / ?", "sans14", 1, 1, 1, 1)
function TargetUnitFrame_SetHealth(current, max, hostility)
    local percentage = current / max 
    -- set text label first
    targetUnitFrame.healthBar.dataLbl:SetText("Health " .. tostring(current) .. " / " .. tostring(max))
    -- then width indicator
    targetUnitFrame.healthBar:SetWidth(percentage * targetUnitFrame:GetWidth())
    -- set color based on... hostile=red, neutral=yellow, friendly=green
    if hostility == "hostile" then 
        targetUnitFrame.healthBar:SetColor(0.75, 0, 0, 1)
    elseif hostility == "neutral" then 
        targetUnitFrame.healthBar:SetColor(0.7, 0.7, 0, 1)
    elseif hostility == "friendly" then 
        targetUnitFrame.healthBar:SetColor(0, 0.75, 0, 1)
    end
end
function TargetUnitFrame_Toggle(show)
    targetUnitFrame:SetVisible(show)
end
TargetUnitFrame_Toggle(false) -- hide until C++ code shows it

-- Build the action bar -------------------------------------------------------
-------------------------------------------------------------------------------
actionBar1Panel = UIFrame.New(nil, 10, 10, 10, 10, TEXTURE_UIBLANK)
-- set the dimensions now
actionBar1Panel:SetXPos(consoleFrame:GetXPos() + consoleFrame:GetWidth()+8)
actionBar1Panel:SetWidth(12 * 32)
actionBar1Panel:SetHeight(32)
actionBar1Panel:SetYPos(experienceBar:GetYPos() - actionBar1Panel:GetHeight() - 8)
actionBar1Panel:SetColor(0.5, 0.5, 0.5, 0.2)

--- Build the left and right hand CD indicators -------------------------------
-------------------------------------------------------------------------------
leftHandFrame = UIFrame.New(nil, 16, 32, 0, 0, TEXTURE_UIBLANK)
leftHandFrame:SetXPos(actionBar1Panel:GetXPos() - leftHandFrame:GetWidth() )
leftHandFrame:SetYPos(actionBar1Panel:GetYPos())
leftHandFrame:SetColor(0.1, 0.1, 0.1, 1)
leftHandFrame.fill = UIFrame.New(leftHandFrame, 16, 32, 0, 0, TEXTURE_UIBLANK)
leftHandFrame.fill:SetColor(0.7, 0.7, 0.1, 1)
function LeftHandFrame_SetValue(value)
    leftHandFrame.fill:SetHeight(value * leftHandFrame:GetHeight())
    leftHandFrame.fill:SetYPos(leftHandFrame:GetHeight() - leftHandFrame.fill:GetHeight())
end

rightHandFrame = UIFrame.New(nil, 16, 32, 0, 0, TEXTURE_UIBLANK)
rightHandFrame:SetXPos(actionBar1Panel:GetXPos() + actionBar1Panel:GetWidth())
rightHandFrame:SetYPos(actionBar1Panel:GetYPos())
rightHandFrame:SetColor(0.1, 0.1, 0.1, 1)
rightHandFrame.fill = UIFrame.New(rightHandFrame, 16, 32, 0, 0, TEXTURE_UIBLANK)
rightHandFrame.fill:SetColor(0.7, 0.7, 0.1, 1)
function RightHandFrame_SetValue(value)
    rightHandFrame.fill:SetHeight(value * rightHandFrame:GetHeight())
    rightHandFrame.fill:SetYPos(rightHandFrame:GetHeight() - rightHandFrame.fill:GetHeight())
end

-- Setup crafting window ------------------------------------------------------
-------------------------------------------------------------------------------
craftingWindow = UIFrame.New(nil, screenWidth / 1.5, screenHeight / 1.5, 0, 0, TEXTURE_UIBLANK)
craftingWindow:SetXPos(screenWidth / 2 - craftingWindow:GetWidth() / 2)
craftingWindow:SetYPos(screenHeight / 2 - craftingWindow:GetHeight() / 2)
craftingWindow:SetColor(0.2, 0.2, 0.2, 0.85)
craftingWindow:SetBorderColor(0.6, 0.6, 0.6, 0.85)
craftingWindow:SetBorderSize(1)
craftingWindow:SetVisible(false)
function CraftingWindow_Toggle(show)
    craftingWindow:SetVisible(show)
end
craftingWindow.title = UILabel.New(craftingWindow, "Crafting", "sans14", 1, 1, 1, 1)
craftingWindow.optionPanel = UIFrame.New(craftingWindow, craftingWindow:GetWidth(), 25, 0, 0, TEXTURE_UIBLANK)
craftingWindow.optionPanel:SetYPos(craftingWindow.title:GetHeight())
craftingWindow.optionPanel:SetColor(0.4, 0.5, 0.4, 0.2)
craftingWindow.optionPanel.craftBtn = UIButton.New(craftingWindow.optionPanel, TEXTURE_UIBLANK, "Craft", 
    "sans14", 5)
craftingWindow.optionPanel.craftBtn:SetColor(0,0,1,0.5)
craftingWindow.optionPanel.craftBtn:SetBorderSize(1)
craftingWindow.optionPanel.craftBtn:SetBorderColor(1,1,1,0.75)
craftingWindow.optionPanel:SetHeight(craftingWindow.optionPanel.craftBtn:GetHeight())
craftingWindow.listPanel = UIFrame.New(craftingWindow, craftingWindow:GetWidth(), 1, 0, 0, TEXTURE_UIBLANK)
craftingWindow.listPanel:SetYPos(craftingWindow.optionPanel:GetYPos() + craftingWindow.optionPanel:GetHeight())
craftingWindow.listPanel:SetHeight(craftingWindow:GetHeight() - craftingWindow.listPanel:GetYPos())
craftingWindow.listPanel:SetColor(0.5, 0.4, 0.4, 0.1)
-- TODO: Game_GetCraftables() to get craftable item list and populate it
-- TODO: Game_GetItemTexture(itemName) to get icons for each craftable entry
-- test out our Game_GetCraftables() function
craftDB = Game_GetCraftables()    
craftingWindow.listPanel.elements = {}
ycounter = 0
for i,v in ipairs(craftDB) do
    local element = UIFrame.New(craftingWindow.listPanel, craftingWindow.listPanel:GetWidth(), 48, 0, 0,
        TEXTURE_UIBLANK)
    element:SetYPos(ycounter)
    element:SetColor(0,0,0,0)
    element.data = v
    element.nameIcon = UITexture.New(element, Game_GetItemTexture(v.name), 48, 48)
    element.nameLbl = UILabel.New(element, v.name, "sans14", 1, 1, 1, 1)
    element.eqLabel = UILabel.New(element, "=", "sans14", 1, 1, 1, 1)
    element.eqLabel:SetXPos(element.nameIcon:GetWidth() + element.nameIcon:GetXPos())
    element.eqLabel:SetYPos(element.nameIcon:GetHeight() / 2 - element.eqLabel:GetHeight() / 2)
    element.requirements = {}
    xcounter = element.eqLabel:GetWidth() + element.eqLabel:GetXPos()
    for j, reqs in ipairs(v.required) do 
        element.requirements[j] = UITexture.New(element, Game_GetItemTexture(reqs.name), 48, 48)
        element.requirements[j]:SetXPos(xcounter)
        element.requirements[j].countLbl = UILabel.New(element.requirements[j], tostring(reqs.count),
            "sans14", 1, 1, 1, 1)
        xcounter = xcounter + element.requirements[j]:GetWidth()
    end
    ycounter = ycounter + element:GetHeight()
    table.insert(craftingWindow.listPanel.elements, element)
end


