print("Testing...")

window = Window.New(nil, 200, 200, 'Test')
window.btn = UIButton.New(window, TEXTURE_UIBLANK, "Hello", "sans", 5)
window.btn:SetYPos(50)
window.btn:AddOnClicked(function() 
    local m = GetViewMatrix()
    print(m)
    for i,t in ipairs(m) do 
        for j,v in ipairs(t) do 
            io.write(tostring(v) .. " ")
        end
        print("")
    end
end)

function window.Tick()
    print("Window ticking every second")
    window:AddOnTimer(window.Tick, 1000)
end

window:Tick()
