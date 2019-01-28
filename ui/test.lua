LoadTexture('uiblank', 'res/textures/uiblank.png')
LoadFont('sans', 'res/fonts/OpenSans-Regular.ttf', 12);
testFrame = UIFrame.New(nil, 200, 200, 100, 100, 'uiblank')
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
end)

closeButton = UIButton.New(testFrame, 'uiblank', 'X', 'sans', 3)
closeButton:SetXPos(testFrame:GetWidth() - closeButton:GetWidth() - 2)
closeButton:SetYPos(2)
closeButton:SetColor(1,0,0,1)
closeButton:SetTextColor(1,1,1,1)
closeButton:SetBorderColor(1,1,1,1)
closeButton:SetBorderSize(1)
closeButton:AddOnClicked(function() 
    testFrame:SetVisible(false)
    testFrame = nil
end)

function CreateWindow(width, height, title)
    local frame = UIFrame.New(nil, width, height, 0, 0, 'uiblank')
    frame:SetColor(0,0,0.5,1)
    frame:SetBorderSize(2)
    frame:SetBorderColor(0,0,0.9,1)
    local title = UILabel.New(frame, title, 'sans', 1,1,1,1)
    frame.title = title
    local closeButton = UIButton.New(frame, 'uiblank', 'X', 'sans', 1)
    closeButton:SetColor(0.5,0,0,1)
    closeButton:SetBorderSize(1)
    closeButton:SetBorderColor(1,0,0,1)
    closeButton:SetXPos(frame:GetWidth() - closeButton:GetWidth() - 2)
    closeButton:SetYPos(2)
    closeButton:AddOnClicked(function()
        frame:SetVisible(false)
    end)
    frame.closeBtn = closeButton
    return frame
end

myWindow = CreateWindow(300, 200, 'My window')
myWindow:SetXPos(300)
myWindow:SetYPos(200)
myOther = CreateWindow(50,80, 'Another one')

function test ()
    local x = 99
    local printX = function() print(x) end
    printX()
end

test()