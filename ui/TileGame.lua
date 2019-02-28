print("TileGame UI 0.1")

-- Create yellow FPS counter in bottom right corner
function UpdateFPS()
    fpsLbl:SetText("FPS: " .. tostring(GetFramesPerSecond()))
    fpsLbl:SetXPos(GetScreenWidth() - fpsLbl:GetWidth())
    fpsLbl:SetYPos(GetScreenHeight() - fpsLbl:GetHeight())
    fpsLbl:AddOnTimer(UpdateFPS, 1000)
end

fpsLbl = UILabel.New(nil, "FPS: ", "sans14", 1,1,0,1)
UpdateFPS()

-- Create console output box
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

-- test console
for i=1, consoleFrame.MAX_LINES * 2 do 
    WriteLineToConsole("Line " .. tostring(i), 1,1,1,1)
end
WriteLineToConsole("Hello console", 0,1,0,1)

-- Create cast bar
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


