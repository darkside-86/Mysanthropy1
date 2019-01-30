RunFile("ui/window.lua")

LoadTexture('uiblank', 'res/textures/uiblank.png')
LoadFont('sans', 'res/fonts/OpenSans-Regular.ttf', 12);

math.randomseed(os.time())

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
            if character ~= '\n' and character ~= '\r' then  
                frame.label:SetText( frame.label:GetText() .. character )
            end
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

function CreateHSlider(parent, width, height)
    local frame = UIFrame.New(parent, width, height, 0, 0, 'uiblank')
    frame:SetColor(0.9,0.9,0.9,0.1)
    frame.slide = UIFrame.New(frame, width, height / 10, 0, 0, 'uiblank')
    frame.slide:SetYPos(frame:GetHeight() / 2 - frame.slide:GetHeight() / 2)
    frame.slide:SetColor(0.2,0.2,0.2,1)
    frame.knob = UIFrame.New(frame, width / 25, height, 0, 0, 'uiblank')
    frame.knob:SetColor(0.7,0.7,0.7,1)
    frame.onUpdate = function(value) end
    frame.getValue = function() 
        return frame.knob:GetXPos() / (frame.slide:GetWidth() - frame.knob:GetWidth())
    end
    frame:AddOnDragged(function(x,y,dx,dy) 
        frame.knob:SetXPos(frame.knob:GetXPos() + dx)
        if frame.knob:GetXPos() < 0 then
            frame.knob:SetXPos(0)
        elseif frame.knob:GetXPos() > frame.slide:GetWidth() - frame.knob:GetWidth() then 
            frame.knob:SetXPos(frame.slide:GetWidth() - frame.knob:GetWidth())
        end
        frame.onUpdate(frame.getValue())
    end)
    return frame
end

function CreateVSlider(parent, width, height)
    local frame = UIFrame.New(parent, width, height, 0, 0, 'uiblank')
    frame:SetColor(0.9,0.9,0.9,0.1)
    frame.slide = UIFrame.New(frame, width / 10, height, 0, 0, 'uiblank')
    frame.slide:SetXPos(frame:GetWidth() / 2 - frame.slide:GetWidth() / 2)
    frame.slide:SetColor(0.2,0.2,0.2,1)
    frame.knob = UIFrame.New(frame, width, height / 25, 0, 0, 'uiblank')
    frame.knob:SetColor(0.7,0.7,0.7,1)
    frame.onUpdate = function(value) end
    frame.getValue = function() 
        return frame.knob:GetYPos() / (frame.slide:GetHeight() - frame.knob:GetHeight())
    end
    frame:AddOnDragged(function(x,y,dx,dy) 
        frame.knob:SetYPos(frame.knob:GetYPos() + dy)
        if frame.knob:GetYPos() < 0 then
            frame.knob:SetYPos(0)
        elseif frame.knob:GetYPos() > frame.slide:GetHeight() - frame.knob:GetHeight() then 
            frame.knob:SetYPos(frame.slide:GetHeight() - frame.knob:GetHeight())
        end
        frame.onUpdate(frame.getValue())
    end)
    return frame
end

myWindow = Window.New(nil, 500, 400, 'My window', 'sans')
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
myWindow.textField = CreateTextField(myWindow, 400, "")
myWindow.textField:SetXPos(5)
myWindow.textField:SetYPos(30)
myWindow.textField:AddOnKeypressed(function(keyCode, scanCode, mod, rep) 
    if keyCode == SDLK_RETURN then
        local f, er = load(myWindow.textField.label:GetText())
        if f == nil then 
            print("Error: " .. er)
        else
            f() 
        end
    end
end)
myWindow.slider = CreateHSlider(myWindow, 200, 30)
myWindow.slider:SetYPos(60)
myWindow.slider.onUpdate = function(value)
    print(value * 100)
    local r,g,b,a = myWindow:GetColor()
    myWindow:SetColor(r,g,b,value)
end

myWindow.vslider = CreateVSlider(myWindow, 30, 200)
myWindow.vslider:SetXPos(myWindow:GetWidth() - myWindow.vslider:GetWidth() - 5)
myWindow.vslider:SetYPos(33)
