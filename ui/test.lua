LoadTexture('uiblank', 'res/textures/uiblank.png')
print('lol')
s = UIFrame.New(nil,100,100,100,100,'uiblank')
t = UIFrame.New(s, 25,30,1,1, 'uiblank')
s:SetColor(1,0,0,1)
t:SetColor(0.5,0,0.5,1)
s.foo = 'Foo'
print(s.foo)
print(s)
s:AddOnClicked(function(x,y,btn) 
    t:SetXPos(t:GetXPos() + 1);
end)