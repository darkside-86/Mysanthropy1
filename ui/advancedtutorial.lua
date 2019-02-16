print("Testing...")

window = Window.New(nil, 500, 220, 'Test')

function CreateMatrixFrame(name, x, y)
    window[name] = UIFrame.New(window, 200, 150, x, y, TEXTURE_UIBLANK)
    window[name]:SetColor(1,1,1,0.3)
    window[name]:SetBorderSize(1)
    window[name]:SetBorderColor(1,1,1,0.5)
    window[name].labels = { {}, {}, {}, {} }
    for i=1,4 do 
        for j=1, 4 do 
            window[name].labels[i][j] = UILabel.New(window[name], '?', 'sans', 1,1,1,1)
            local xPos = (i - 1) * 45
            local yPos = (j - 1) * 30
            window[name].labels[i][j]:SetXPos(xPos)
            window[name].labels[i][j]:SetYPos(yPos)
        end
    end
end

CreateMatrixFrame("projectionFrame", 10, 20)
CreateMatrixFrame("viewFrame", window.projectionFrame:GetXPos() + window.projectionFrame:GetWidth() + 3, 20)

testMatrix = {
    {1,2,3,4}, {5,6,7,8}, 
    {9,10,11,12}, {13,14,15,16}
}

function DisplayMatrix(frame, matrix)
    for i=1,4 do 
        for j=1, 4 do 
            frame.labels[i][j]:SetText(string.format("%.3f",matrix[i][j]))
        end
    end
end

DisplayMatrix(window.projectionFrame, testMatrix)

g_viewMatrix = nil
g_projectionMatrix = nil
function UpdateMatrixFrames()
    DisplayMatrix(window.viewFrame, g_viewMatrix)
    DisplayMatrix(window.projectionFrame, g_projectionMatrix)
end
