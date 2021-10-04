--[[
    Sync data via websocket - client side

    This script will keep running for a while if the dialog window 
    is closed with an "x" button :(

    If that happens, close the sprite and open again (Shift + Ctrl + T)
]]

local ws -- websocket created later
local dlg = Dialog()
local spr = app.activeSprite

--[[
Set up an image buffer for two reasons:
    a) the active cel might not be the same size as the sprite
    b) the sprite might not be in RGBA mode, and it's easier to use ase 
       than do conversions on the other side.
]]
local buf = Image(spr.width, spr.height, ColorMode.RGB)

-- magic number, not strictly required in this example
local IMAGE_ID = string.byte("I")


-- clean up and exit
local function finish()
    if ws ~= nil then ws:close() end
    if dlg ~= nil then dlg:close() end
    spr.onChange = nil
    app.site.onChange = nil
    spr = nil
    dlg = nil
end


local function sendImage()
    if buf.width ~= spr.width or buf.height ~= spr.height then
        buf:resize(spr.width, spr.height)
    end

    buf:clear()
    buf:drawSprite(spr, app.activeFrame.frameNumber)

    ws:sendBinary(string.pack("<LLL", IMAGE_ID, buf.width, buf.height), buf.bytes)
end


-- close connection and ui if the sprite is closed
local frame = -1
local function onSiteChange()
    if app.activeSprite ~= spr then
        -- quit if the sprite was closed
        for _,s in ipairs(app.sprites) do
            if s == spr then 
                -- the sprite is open in an inactive tab
                break
            end
        end

        finish()
    else
        -- update the view after the frame changes
        if app.activeFrame.frameNumber ~= frame then
            frame = app.activeFrame.frameNumber
            sendImage()
        end
    end
end


-- t is for type, there's already a lua function
local function receive(t, message)
    if t == MessageType.Open then
        dlg:modify{id="status", text="Sync ON"}
        spr.onChange = sendImage
        app.site.onChange = onSiteChange
        sendImage()

    elseif t == MessageType.Close and dlg ~= nil then
        dlg:modify{id="status", text="No connection"}
        spr.onChange = nil
        app.site.onChange = nil
    end
end


-- set up a websocket
ws = WebSocket{ url="http://127.0.0.1:34613", onreceive=receive, deflate=false }

-- create an UI
dlg:label{ id="status", text="Connecting..." }
dlg:button{ text="Cancel", onclick=finish}

-- let's go!
ws:connect()
dlg:show{ wait=false }