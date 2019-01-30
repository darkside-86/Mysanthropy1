
--[[ testFrame = UIFrame.New(nil, 200, 200, 100, 100, 'uiblank')
testFrame:SetColor(0.4,0.3,0.2,1)
testFrame:SetBorderColor(0.2,1,1,1)
testFrame:SetBorderSize(2)
testLabel = UILabel.New(testFrame, 'Test frame', 'sans', 0,0,0,1)
testButton = UIButton.New(testFrame, 'uiblank', 'Click', 'sans', 4)
testButton:SetColor(0.5,0.4,0.3,1)
testButton:SetTextColor(1,0.4,0.4,1)
print( testFrame:GetWidth() / 2 - testButton:GetWidth() / 2)
print( testFrame:GetHeight() / 2 - testButton:GetHeight() / 2)
testButton:SetBorderSize(1)
testButton:SetBorderColor(0,0,0.3,1)
testButton:SetXPos( testFrame:GetWidth() / 2 - testButton:GetWidth() / 2)
testButton:SetYPos( testFrame:GetHeight() / 2 - testButton:GetHeight() / 2)
testButton:AddOnClicked(function() 
    testLabel:SetText("Button was clicked!")
end)
testLabel:AddOnHover(function(x,y,sx,sy,over)
    if over then 
        testLabel:SetText("Hover on")
    else
        testLabel:SetText("Hover off")
    end
end)]]

--[[closeButton = UIButton.New(testFrame, 'uiblank', 'X', 'sans', 3)
closeButton:SetXPos(testFrame:GetWidth() - closeButton:GetWidth() - 2)
closeButton:SetYPos(2)
closeButton:SetColor(1,0,0,1)
closeButton:SetTextColor(1,1,1,1)
closeButton:SetBorderColor(1,1,1,1)
closeButton:SetBorderSize(1)
closeButton:AddOnClicked(function() 
    testFrame:SetVisible(false)
    testFrame = nil
end)]]

LoadTexture('uiblank', 'res/textures/uiblank.png')
LoadFont('sans', 'res/fonts/OpenSans-Regular.ttf', 12);

math.randomseed(os.time())

function CreateWindow(width, height, title)
    local frame = UIFrame.New(nil, width, height, 0, 0, 'uiblank')
    frame:SetColor(0,0,0.5,0.6)
    frame:SetBorderSize(2)
    frame:SetBorderColor(0,0,0.9,1)
    frame.title = UILabel.New(frame, title, 'sans', 1,1,1,1)
    closeButton = UIButton.New(frame, 'uiblank', 'X', 'sans', 3)
    closeButton:SetColor(0.5,0,0,1)
    closeButton:SetTextColor(1,1,1,1)
    closeButton:SetBorderSize(1)
    closeButton:SetBorderColor(1,0,0,1)
    closeButton:SetXPos(frame:GetWidth() - closeButton:GetWidth() - 2)
    closeButton:SetYPos(2)
    closeButton:AddOnClicked(function()
        frame:SetVisible(false)
    end)
    frame.closeBtn = closeButton
    frame:AddOnDragged(function(x,y,dx,dy) 
        frame:SetXPos(frame:GetXPos() + dx)
        frame:SetYPos(frame:GetYPos() + dy)
    end)    
    return frame
end

function CreateTextField(parent, width, text)
    text = text or ""
    local SHIFT_TABLE = {
        ['`'] = '~', ['1'] = '!', ['2'] = '@', ['3'] = '#', ['4'] = '$', ['5'] = '%',
        ['6'] = '^', ['7'] = '&', ['8'] = '*', ['9'] = '(', ['0'] = ')', ['-'] = '_',
        ['='] = '+', ['['] = '{', [']'] = '}', ['\\']= '|', [';'] = ':', ["'"] = '"',
        [','] = '<', ['.'] = '>', ['/'] = '?'
    }
    local frame = UIFrame.New(parent, width, 20, 0, 0, 'uiblank')
    frame:SetColor(0.7,0.7,0.3,1)
    frame:SetBorderSize(1)
    frame:SetBorderColor(0,0,0,1)
    frame.label = UILabel.New(frame, text, 'sans', 0,0,0.5,1)
    frame:AddOnKeypressed(function(keyCode, scanCode, mod, rep)
        print(keyCode)
        local character = ''
        if keyCode >= 10 and keyCode <= 126 then 
            character = string.char(keyCode)
            if mod & ( KMOD_LSHIFT | KMOD_RSHIFT) ~= 0 then
                local n = string.byte(character)
                if n >= 97 and n <= 122 then
                    character = string.char(string.byte(character) - 32)
                else 
                    character = SHIFT_TABLE[character]
                    if character == nil then 
                        character = '?'
                    end
                end
            end
            frame.label:SetText( frame.label:GetText() .. character )
        elseif keyCode == SDLK_BACKSPACE then 
            local text = frame.label:GetText()
            if text:len() > 0 then 
                text = text:sub(1,text:len()-1)
            end
            frame.label:SetText(text)
        end
    end)
    return frame
end

myWindow = CreateWindow(500, 400, 'My window')
myWindow:SetXPos(GetScreenWidth() / 2 - myWindow:GetWidth() / 2)
myWindow:SetYPos(GetScreenHeight() / 2 - myWindow:GetHeight() / 2)
myWindow.colorBtn = UIButton.New(myWindow, 'uiblank', 'Colors', 'sans', 5)
myWindow.colorBtn:SetXPos(myWindow:GetWidth() / 2 - myWindow.colorBtn:GetWidth() / 2)
myWindow.colorBtn:SetYPos(myWindow:GetHeight() / 2 -  myWindow.colorBtn:GetHeight() / 2)
myWindow.colorBtn:SetColor(0,1,0,1)
myWindow.colorBtn:SetTextColor(0,0,0,1)
myWindow.colorBtn:SetBorderColor(0.2,0.2,0.8,1)
myWindow.colorBtn:SetBorderSize(1)
myWindow.colorBtn:AddOnClicked(function() 
    myWindow:SetColor(math.random(),math.random(),math.random(),math.random())
end)
myWindow.colorBtn:AddOnHover(function(x,y,xrel,yrel,over)
    if over then
        myWindow.colorBtn:SetColor(1,0,1,1)
    else
        myWindow.colorBtn:SetColor(0,1,0,1)
    end
end)
myWindow:AddOnKeypressed(function(keycode, scancode, mod, rep)
    print(keycode,scancode,mod,rep)
end)
myWindow.textField = CreateTextField(myWindow, 200, "")
myWindow.textField:SetXPos(5)
myWindow.textField:SetYPos(30)

print(SDLK_a)
