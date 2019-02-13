print("Testing...")

window = Window.New(nil, 200, 200, 'Test')
window.btn = UIButton.New(window, TEXTURE_UIBLANK, "Hello", "sans", 5)
window.btn:SetYPos(50)