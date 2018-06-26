#ifndef __HTML_H__
#define __HTML_H__

const char joy_text[] PROGMEM = R"=====(
<!DOCTYPE html>
<html>
<head>
    <meta charset="utf-8">
    <meta name="viewport" content="width=device-width,height=device-height, user-scalable=no, minimum-scale=1.0, maximum-scale=1.0">
    <meta name="apple-mobile-web-app-capable" content="yes" />
    <meta name="apple-mobile-web-app-status-bar-style" content="black" />
    <meta name="format-detection" content="telephone=no" />
    <style>
        * {
            margin: 0;
            padding: 0;
            user-select: none;
            -moz-user-select: none;
            -webkit-user-select: none;
            -ms-user-select: none;
            overflow: hidden;
            overscroll-behavior: none;
        }
        html,body {
            width: 100%;
            height: 100%;
        }
        #fullBlack {
            width: 100%;
            height: 100%;
            background-color: #000000;
            text-align: center;
        }
    </style>
</head>

<body>
    <div id="fullBlack">
        <span style="color: #0000FF; ">Virtual Joystick</span>
        <br>
        <span id="msg1" style="color: #FF0000; "></span>
        <br>
        <span id="msg2" style="color: #FF0000; "></span>
        <br>
        <span id="msg3" style="color: #FF0000; "></span>
        <br>
        <span id="msg4" style="color: #FF0000; "></span>
        <br>
        <span id="msg5" style="color: #FF0000; "></span>
        <br>
        <span id="msg6" style="color: #FF0000; "></span>
        <br>
        <span id="msg7" style="color: #FF0000; "></span>
        <br>
        <span id="msg8" style="color: #FF0000; "></span>
        <br>
        <span id="msg9" style="color: #FF0000; "></span>
        <br>
        <span id="msg10" style="color: #FF0000; "></span>
    </div>
    <script>
        var VirtualJoystick = function (opts) {
            opts = opts || {};
            this._container = opts.container || document.body;
            this._strokeStyle = opts.strokeStyle || 'cyan';
            this._stickEl = opts.stickElement || this._buildJoystickStick();
            this._baseEl = opts.baseElement || this._buildJoystickBase();
            this._mouseSupport = opts.mouseSupport !== undefined ? opts.mouseSupport : false;
            this._stationaryBase = opts.stationaryBase || false;
            this._baseX = this._stickX = opts.baseX || 0
            this._baseY = this._stickY = opts.baseY || 0
            this._limitStickTravel = opts.limitStickTravel || false
            this._stickRadius = opts.stickRadius !== undefined ? opts.stickRadius : 100
            this._useCssTransform = opts.useCssTransform !== undefined ? opts.useCssTransform : false

            this._container.style.position = "relative"

            this._container.appendChild(this._baseEl)
            this._baseEl.style.position = "absolute"
            this._baseEl.style.display = "none"
            this._container.appendChild(this._stickEl)
            this._stickEl.style.position = "absolute"
            this._stickEl.style.display = "none"

            this._pressed = false;
            this._touchIdx = null;

            if (this._stationaryBase === true) {
                this._baseEl.style.display = "";
                this._baseEl.style.left = (this._baseX - this._baseEl.width / 2) + "px";
                this._baseEl.style.top = (this._baseY - this._baseEl.height / 2) + "px";
            }

            this._transform = this._useCssTransform ? this._getTransformProperty() : false;
            this._has3d = this._check3D();

            var __bind = function (fn, me) { return function () { return fn.apply(me, arguments); }; };
            this._$onTouchStart = __bind(this._onTouchStart, this);
            this._$onTouchEnd = __bind(this._onTouchEnd, this);
            this._$onTouchMove = __bind(this._onTouchMove, this);
            this._container.addEventListener('touchstart', this._$onTouchStart, false);
            this._container.addEventListener('touchend', this._$onTouchEnd, false);
            this._container.addEventListener('touchmove', this._$onTouchMove, false);
            if (this._mouseSupport) {
                this._$onMouseDown = __bind(this._onMouseDown, this);
                this._$onMouseUp = __bind(this._onMouseUp, this);
                this._$onMouseMove = __bind(this._onMouseMove, this);
                this._container.addEventListener('mousedown', this._$onMouseDown, false);
                this._container.addEventListener('mouseup', this._$onMouseUp, false);
                this._container.addEventListener('mousemove', this._$onMouseMove, false);
            }
        }
        VirtualJoystick.prototype.destroy = function () {
            this._container.removeChild(this._baseEl);
            this._container.removeChild(this._stickEl);

            this._container.removeEventListener('touchstart', this._$onTouchStart, false);
            this._container.removeEventListener('touchend', this._$onTouchEnd, false);
            this._container.removeEventListener('touchmove', this._$onTouchMove, false);
            if (this._mouseSupport) {
                this._container.removeEventListener('mouseup', this._$onMouseUp, false);
                this._container.removeEventListener('mousedown', this._$onMouseDown, false);
                this._container.removeEventListener('mousemove', this._$onMouseMove, false);
            }
        }
        /**
         * @returns {Boolean} true if touchscreen is currently available, false otherwise
        */
        VirtualJoystick.touchScreenAvailable = function () {
            return 'createTouch' in document ? true : false;
        }
            /**
             * microevents.js - https://github.com/jeromeetienne/microevent.js
            */
            ; (function (destObj) {
                destObj.addEventListener = function (event, fct) {
                    if (this._events === undefined) this._events = {};
                    this._events[event] = this._events[event] || [];
                    this._events[event].push(fct);
                    return fct;
                };
                destObj.removeEventListener = function (event, fct) {
                    if (this._events === undefined) this._events = {};
                    if (event in this._events === false) return;
                    this._events[event].splice(this._events[event].indexOf(fct), 1);
                };
                destObj.dispatchEvent = function (event /* , args... */) {
                    if (this._events === undefined) this._events = {};
                    if (this._events[event] === undefined) return;
                    var tmpArray = this._events[event].slice();
                    for (var i = 0; i < tmpArray.length; i++) {
                        var result = tmpArray[i].apply(this, Array.prototype.slice.call(arguments, 1))
                        if (result !== undefined) return result;
                    }
                    return undefined
                };
            })(VirtualJoystick.prototype);

        VirtualJoystick.prototype.deltaX = function () { return this._stickX - this._baseX; }
        VirtualJoystick.prototype.deltaY = function () { return this._stickY - this._baseY; }

        VirtualJoystick.prototype.up = function () {
            if (this._pressed === false) return false;
            var deltaX = this.deltaX();
            var deltaY = this.deltaY();
            if (deltaY >= 0) return false;
            if (Math.abs(deltaX) > 2 * Math.abs(deltaY)) return false;
            return true;
        }
        VirtualJoystick.prototype.down = function () {
            if (this._pressed === false) return false;
            var deltaX = this.deltaX();
            var deltaY = this.deltaY();
            if (deltaY <= 0) return false;
            if (Math.abs(deltaX) > 2 * Math.abs(deltaY)) return false;
            return true;
        }
        VirtualJoystick.prototype.right = function () {
            if (this._pressed === false) return false;
            var deltaX = this.deltaX();
            var deltaY = this.deltaY();
            if (deltaX <= 0) return false;
            if (Math.abs(deltaY) > 2 * Math.abs(deltaX)) return false;
            return true;
        }
        VirtualJoystick.prototype.left = function () {
            if (this._pressed === false) return false;
            var deltaX = this.deltaX();
            var deltaY = this.deltaY();
            if (deltaX >= 0) return false;
            if (Math.abs(deltaY) > 2 * Math.abs(deltaX)) return false;
            return true;
        }
        VirtualJoystick.prototype._onUp = function () {
            this._pressed = false;
            this._stickEl.style.display = "none";

            if (this._stationaryBase == false) {
                this._baseEl.style.display = "none";

                this._baseX = this._baseY = 0;
                this._stickX = this._stickY = 0;
            }
        }
        VirtualJoystick.prototype._onDown = function (x, y) {
            this._pressed = true;
            if (this._stationaryBase == false) {
                this._baseX = x;
                this._baseY = y;
                this._baseEl.style.display = "";
                this._move(this._baseEl.style, (this._baseX - this._baseEl.width / 2), (this._baseY - this._baseEl.height / 2));
            }

            this._stickX = x;
            this._stickY = y;

            if (this._limitStickTravel === true) {
                var deltaX = this.deltaX();
                var deltaY = this.deltaY();
                var stickDistance = Math.sqrt((deltaX * deltaX) + (deltaY * deltaY));
                if (stickDistance > this._stickRadius) {
                    var stickNormalizedX = deltaX / stickDistance;
                    var stickNormalizedY = deltaY / stickDistance;

                    this._stickX = stickNormalizedX * this._stickRadius + this._baseX;
                    this._stickY = stickNormalizedY * this._stickRadius + this._baseY;
                }
            }

            this._stickEl.style.display = "";
            this._move(this._stickEl.style, (this._stickX - this._stickEl.width / 2), (this._stickY - this._stickEl.height / 2));
        }

        VirtualJoystick.prototype._onMove = function (x, y) {
            if (this._pressed === true) {
                this._stickX = x;
                this._stickY = y;

                if (this._limitStickTravel === true) {
                    var deltaX = this.deltaX();
                    var deltaY = this.deltaY();
                    var stickDistance = Math.sqrt((deltaX * deltaX) + (deltaY * deltaY));
                    if (stickDistance > this._stickRadius) {
                        var stickNormalizedX = deltaX / stickDistance;
                        var stickNormalizedY = deltaY / stickDistance;

                        this._stickX = stickNormalizedX * this._stickRadius + this._baseX;
                        this._stickY = stickNormalizedY * this._stickRadius + this._baseY;
                    }
                }

                this._move(this._stickEl.style, (this._stickX - this._stickEl.width / 2), (this._stickY - this._stickEl.height / 2));
            }
        }
        VirtualJoystick.prototype._onMouseUp = function (event) {
            return this._onUp();
        }

        VirtualJoystick.prototype._onMouseDown = function (event) {
            event.preventDefault();
            var x = event.clientX;
            var y = event.clientY;
            return this._onDown(x, y);
        }

        VirtualJoystick.prototype._onMouseMove = function (event) {
            var x = event.clientX;
            var y = event.clientY;
            return this._onMove(x, y);
        }
        VirtualJoystick.prototype._onTouchStart = function (event) {
            if (this._touchIdx !== null) return;
            var isValid = this.dispatchEvent('touchStartValidation', event);
            if (isValid === false) return;
            this.dispatchEvent('touchStart', event);

            event.preventDefault();
            var touch = event.changedTouches[0];
            this._touchIdx = touch.identifier;
            var x = touch.pageX;
            var y = touch.pageY;
            return this._onDown(x, y)
        }

        VirtualJoystick.prototype._onTouchEnd = function (event) {

            if (this._touchIdx === null) return;

            this.dispatchEvent('touchEnd', event);

            var touchList = event.changedTouches;
            for (var i = 0; i < touchList.length && touchList[i].identifier !== this._touchIdx; i++);

            if (i === touchList.length) return;


            this._touchIdx = null;

            event.preventDefault();

            return this._onUp()
        }

        VirtualJoystick.prototype._onTouchMove = function (event) {

            if (this._touchIdx === null) return;

            var touchList = event.changedTouches;
            for (var i = 0; i < touchList.length && touchList[i].identifier !== this._touchIdx; i++);

            if (i === touchList.length) return;
            var touch = touchList[i];

            event.preventDefault();

            var x = touch.pageX;
            var y = touch.pageY;
            return this._onMove(x, y)
        }

        VirtualJoystick.prototype._buildJoystickBase = function () {
            var canvas = document.createElement('canvas');
            canvas.width = 126;
            canvas.height = 126;

            var ctx = canvas.getContext('2d');
            ctx.beginPath();
            ctx.strokeStyle = this._strokeStyle;
            ctx.lineWidth = 6;
            ctx.arc(canvas.width / 2, canvas.width / 2, 40, 0, Math.PI * 2, true);
            ctx.stroke();

            ctx.beginPath();
            ctx.strokeStyle = this._strokeStyle;
            ctx.lineWidth = 2;
            ctx.arc(canvas.width / 2, canvas.width / 2, 60, 0, Math.PI * 2, true);
            ctx.stroke();

            return canvas;
        }

        VirtualJoystick.prototype._buildJoystickStick = function () {
            var canvas = document.createElement('canvas');
            canvas.width = 86;
            canvas.height = 86;
            var ctx = canvas.getContext('2d');
            ctx.beginPath();
            ctx.strokeStyle = this._strokeStyle;
            ctx.lineWidth = 6;
            ctx.arc(canvas.width / 2, canvas.width / 2, 40, 0, Math.PI * 2, true);
            ctx.stroke();
            return canvas;
        }

        VirtualJoystick.prototype._move = function (style, x, y) {
            if (this._transform) {
                if (this._has3d) {
                    style[this._transform] = 'translate3d(' + x + 'px,' + y + 'px, 0)';
                } else {
                    style[this._transform] = 'translate(' + x + 'px,' + y + 'px)';
                }
            } else {
                style.left = x + 'px';
                style.top = y + 'px';
            }
        }

        VirtualJoystick.prototype._getTransformProperty = function () {
            var styles = [
                'webkitTransform',
                'MozTransform',
                'msTransform',
                'OTransform',
                'transform'
            ];

            var el = document.createElement('p');
            var style;

            for (var i = 0; i < styles.length; i++) {
                style = styles[i];
                if (null != el.style[style]) {
                    return style;
                }
            }
        }

        VirtualJoystick.prototype._check3D = function () {
            var prop = this._getTransformProperty();

            if (!prop || !window.getComputedStyle) return module.exports = false;

            var map = {
                webkitTransform: '-webkit-transform',
                OTransform: '-o-transform',
                msTransform: '-ms-transform',
                MozTransform: '-moz-transform',
                transform: 'transform'
            };

            var el = document.createElement('div');
            el.style[prop] = 'translate3d(1px,1px,1px)';
            document.body.insertBefore(el, null);
            var val = getComputedStyle(el).getPropertyValue(map[prop]);
            document.body.removeChild(el);
            var exports = null != val && val.length && 'none' != val;
            return exports;
        }

        console.log('touchscreen is', VirtualJoystick.touchScreenAvailable() ? 'available' : 'not available');
		var radius=75;
        var joystickXY = new VirtualJoystick({
            container: document.body,
            mouseSupport: true,
            strokeStyle: 'cyan',
            limitStickTravel: true,
            stickRadius: radius
        });
        joystickXY.addEventListener('touchStartValidation', function (event) {
            var touch = event.changedTouches[0];
            if (touch.pageX < window.innerWidth / 2) return true;
            return false;
        });
        var joystickUV = new VirtualJoystick({
            container: document.body,
            strokeStyle: 'orange',
            limitStickTravel: true,
            stickRadius: radius
        });
        joystickUV.addEventListener('touchStartValidation', function (event) {
            var touch = event.changedTouches[0];
            if (touch.pageX >= window.innerWidth / 2) return true;
            return false
        });
        var ip_addr = document.location.hostname;
        var socket = new WebSocket('ws://' + ip_addr + ':81/', ['arduino']);
        socket.onopen = function (event) {
            socket.send('x0y0u0v0');
        }

        function newline() {
            for (var i = 1; i < 10; i++) {
                var firow = document.getElementById('msg' + i.toString());
                var secRow = document.getElementById('msg' + (i + 1).toString());
                firow.innerHTML = secRow.innerHTML;
            }
            document.getElementById('msg10').innerHTML = '';
        }
        socket.onmessage = function (event) {
            var reader = new FileReader();
            reader.onload = function (e) {
                var rxdata = reader.result;
                var outputMSG = document.getElementById('msg10');
                var toShow = '';
                for (var i = 0; i < rxdata.length; i++) {
                    if (rxdata[i] == '\n') {
                        outputMSG.innerHTML += toShow;
                        newline();
                        toShow = '';
                    } else {
                        toShow += rxdata[i];
                    }
                }
                outputMSG.innerHTML += toShow;
            }
            reader.readAsText(event.data);
        };
        socket.onclose = function (event) {
            console.log('Client notified socket has closed', event);
            for (var i = 1; i <= 10; i++) {
                var outputMSG = document.getElementById('msg' + i.toString());
                outputMSG.innerHTML = 'Lost connection!';
            }
        };
        socket.onerror = function (event) {
            console.log('WebSocketError!');
            for (var i = 1; i <= 10; i++) {
                var outputMSG = document.getElementById('msg' + i.toString());
                outputMSG.innerHTML = 'Lost connection!';
            }
        };
        setInterval(function () {
            var tosend = 'u' + Math.round(joystickXY.deltaX()*255/radius)
                + 'v' + Math.round(-joystickXY.deltaY()*255/radius)
                + 'w' + Math.round(joystickUV.deltaX()*255/radius)
                + 'a' + Math.round(-joystickUV.deltaY()*255/radius)+'\r\n';
            if (socket.readyState == 1) socket.send(tosend);
        }, 1000 / 20);
        document.body.addEventListener('touchmove', function (e) { e.preventDefault();}, {passive: false});
    </script>
</body>
</html>
)=====";

const char index_text[] PROGMEM = R"=====(
<!DOCTYPE html>
<html lang="en" xmlns="http://www.w3.org/1999/xhtml">
<head>
    <meta charset="utf-8" />
    <title>OSEPP Esp8266 Uploder for Uno</title>
    <style type="text/css">
        .select-editable {
            position: relative;
            background-color: white;
            border: solid grey 1px;
            width: 120px;
            height: 18px;
        }
            .select-editable select {
                position: absolute;
                top: 0px;
                left: 0px;
                font-size: 14px;
                border: none;
                width: 120px;
                margin: 0;
            }
            .select-editable input {
                position: absolute;
                top: 0px;
                left: 0px;
                width: 100px;
                padding: 1px;
                font-size: 12px;
                border: none;
            }
                .select-editable select:focus, .select-editable input:focus {
                    outline: none;
                }
    </style>
</head>
<body>
    <form action="setap" method="post">
        <table>
            <tr>
                <td style="width: 150px">AP SSID</td>
                <td>
                    <input name="ssid" type="text" value="%s" /></td>
            </tr>
            <tr>
                <td>AP PSWD</td>
                <td>
                    <input name="pswd" type="password" placeholder="new password" /></td>
            </tr>
            <tr>
                <td>&nbsp;</td>
                <td>
                    <input type="submit" value="update" /></td>
            </tr>
        </table>
    </form>
    <br />
    <form action="setwf" method="post">
        <table>
            <tr>
                <td style="width: 150px">WIFI SSID</td>
                <td>
                    <div class="select-editable">
                        <select onchange="document.getElementById('wifissid').value=this.value">
                            %s
                        </select>
                        <input id="wifissid" name="ssid" type="text" value="%s" />
                    </div>
                </td>
            </tr>
            <tr>
                <td>WIFI Password</td>
                <td>
                    <input name="pswd" type="password" placeholder="Your WIFI Password" /></td>
            </tr>
            <tr>
                <td>&nbsp;</td>
                <td>
                    <input type="submit" value="update" /></td>
            </tr>
        </table>
    </form>
    <br />
    <table>
        <tr>
            <td style="width: 150px">Local IP</td>
            <td>%s</td>
        </tr>
        <tr>
            <td>subnetMask</td>
            <td>%s</td>
        </tr>
        <tr>
            <td>gateway</td>
            <td>%s</td>
        </tr>
        <tr>
            <td>dns</td>
            <td>%s</td>
        </tr>
    </table>
</body>
</html>

)=====";

const char redir_text[] PROGMEM = R"=====(
<!DOCTYPE HTML>
<html lang="en-US">
<head>
<meta http-equiv="refresh" content="10;url=netstat"> 
</head>
    <body>ok[<span id="totalSecond">0</span>]
<script language="javascript" type="text/javascript"> 
var second = totalSecond.innerText; 
setInterval("redirect()", 1000); 
function redirect(){ 
totalSecond.innerText=++second; 
if(second==11) location.href='netstat'; 
} 
</script>
    </body>
</html>)=====";


const char serial_text[] PROGMEM = R"=====(
<!DOCTYPE html>
<html lang="en" xmlns="http://www.w3.org/1999/xhtml">
<head>
    <meta charset="utf-8" />
    <meta http-equiv="pragma" content="no-cache" />
    <meta http-equiv="cache-control" content="no-cache" />
    <meta http-equiv="expires" content="0" />
    <title>Serial Monitor</title>

    <style type="text/css">
        * {
            margin: 0;
            padding: 0;
        }

        html, body {
            width: 100%;
            height: 100%;
            overflow: hidden;
        }

        .top, .main {
            width: 100%;
            position: absolute;
            overflow: hidden;
        }

        .top {
            top: 0;
            height: 40px;
        }

        .main {
            top: 40px;
            bottom: 0px;
            overflow: auto;
        }

        #msgBar {
            position: absolute;
            width: 100%;
            background-color: cadetblue;
        }

            #msgBar * {
                user-select: none;
                -moz-user-select: none;
                -webkit-user-select: none;
                -ms-user-select: none;
            }

            #msgBar ul {
                display: block;
                margin-left: 10px;
                margin-right: 10px;
            }

                #msgBar ul li {
                    display: inline;
                    line-height: 40px;
                }

            #msgBar li > * {
                display: inline-block;
            }

        .svg_icon {
            cursor: pointer;
            width: 40px;
            height: 40px;
            transition: fill-opacity 0.5s;
            -moz-transition: fill-opacity 0.5s; /* Firefox 4 */
            -webkit-transition: fill-opacity 0.5s; /* Safari ºÍ Chrome */
            -o-transition: fill-opacity 0.5s; /* Opera */
            vertical-align: middle;
            fill: #006461;
            fill-opacity: 0.7;
        }

            .svg_icon:hover {
                fill-opacity: 1;
            }

        .select-editable {
            position: relative;
            width: 120px;
            height: 19px;
        }

            .select-editable * {
                position: absolute;
                top: 5px;
                left: 0px;
            }

            .select-editable select {
                font-size: 14px;
                border: none;
                width: 120px;
                margin: 0;
                vertical-align: middle;
                white-space: pre;
            }

            .select-editable input {
                width: 100px;
                padding: 1px;
                font-size: 12px;
                border: none;
                vertical-align: middle;
            }

                .select-editable select:focus,
                .select-editable input:focus {
                    outline: none;
                }
    </style>
</head>
<body>
    <svg xmlns="http://www.w3.org/2000/svg" version="1.1" style="display: none">
        <symbol id="svg_sendtext" viewBox="0 0 512 512">
            <path d="M224 387.814v124.186l-192-192 192-192v126.912c223.375 5.24 213.794-151.896 156.931-254.912 140.355 151.707 110.55 394.785-156.931 387.814z"></path>
        </symbol>
        <symbol id="svg_resetuno" viewBox="0 0 512 512">
            <path d="M444.84 83.16c-46.804-51.108-114.077-83.16-188.84-83.16-141.385 0-256 114.615-256 256h48c0-114.875 93.125-208 208-208 61.51 0 116.771 26.709 154.848 69.153l-74.848 74.847h176v-176l-67.16 67.16z"></path>
            <path d="M464 256c0 114.875-93.125 208-208 208-61.51 0-116.771-26.709-154.847-69.153l74.847-74.847h-176v176l67.16-67.16c46.804 51.108 114.077 83.16 188.84 83.16 141.385 0 256-114.615 256-256h-48z"></path>
        </symbol>
    </svg>
    <div id="msgBar" class="top">
        <ul style="float: right">
            <li>
                <select id="line_ending">
                    <option>No line ending</option>
                    <option>Newline</option>
                    <option>Carriage return</option>
                    <option>Both NL&CR</option>
                </select>
            </li>
            <li title="send text to uno">
                <svg id="send" class="svg_icon" onclick="sendmsg()">
                    <use xlink:href="#svg_sendtext" width="30px" height="30px" transform="translate(0,5)"></use>
                </svg>
            </li>
            <li title="reset uno" onclick="resetUno()">
                <svg class="svg_icon" id="reset">
                    <use xlink:href="#svg_resetuno" width="26px" height="26px" transform="translate(0,7)"></use>
                </svg>
            </li>
        </ul>
        <ul style="float: left">
            <li>Baudrate:</li>
            <li>
                <span class="select-editable">
                    <select id="BandrateList">
                        <option>300</option>
                        <option>1200</option>
                        <option>2400</option>
                        <option>4800</option>
                        <option>9600</option>
                        <option>14400</option>
                        <option>19200</option>
                        <option>28800</option>
                        <option>38400</option>
                        <option>57600</option>
                        <option>74880</option>
                        <option selected="selected">115200</option>
                        <option>128000</option>
                        <option>230400</option>
                        <option>250000</option>
                        <option>1000000</option>
                        <option>2000000</option>
                    </select>
                    <input id="SelectBandrate" type="text" />
                </span>
            </li>
            <li>Message:</li>
        </ul>
        <ul style="overflow: hidden; display: block">
            <li>
                <input id="sendtext" type="text" style="width: 100%" onkeypress="sendIfEnter(event)" />
            </li>
        </ul>

    </div>
    <div id="msgDiv" class="main" style="background-color: darkgray">
    </div>
    <script>
        var updateBaudrate = function (baudrate) {
            var getBaudrate = new XMLHttpRequest();

            getBaudrate.onload = function () {
                document.getElementById("SelectBandrate").value = this.responseText;
            };
            var hostString = '/getbaudrate';
            if (baudrate) parseInt(baudrate, 10);
            if (baudrate) {
                hostString += "?baudrate=" + baudrate;
            }
            getBaudrate.open('GET', hostString);
            getBaudrate.send();
        }

        document.getElementById("SelectBandrate").onchange = function () {
            updateBaudrate(this.value);
        }

        document.getElementById("BandrateList").onchange = function () {
            if (document.getElementById("SelectBandrate").value != this.value) {
                document.getElementById("SelectBandrate").value = this.value;
                document.getElementById("SelectBandrate").onchange();
            }
        };
        document.getElementById("BandrateList").onclick = function () {
            document.getElementById("BandrateList").selectedIndex = -1;
        };

        updateBaudrate();

        var resetUno = function () {
            var xhr = new XMLHttpRequest();
            xhr.open('POST', '/log/reset', false);
            xhr.send();
        };
        var ip_addr = document.location.hostname;
        var socket = new WebSocket('ws://' + ip_addr + ':81/', ['arduino']);
        socket.onopen = function (event) {
            document.getElementById("msgDiv").style.cssText = "";
            document.title = "Serial Monitor at " + document.location.hostname;
        };
        socket.onmessage = function (event) {
            var reader = new FileReader();
            reader.onload = function (e) {
                var rxdata = reader.result;
                var el = document.getElementById("msgDiv");
                var scroll = el.scrollHeight - el.clientHeight - el.scrollTop;
                var chile = document.createElement("span");
                chile.innerText = rxdata;
                el.appendChild(chile);
                if (scroll < chile.offsetHeight * 1.5) {
                    if (el.childNodes.length > 300) {
                        while (el.childNodes.length > 200) el.removeChild(el.childNodes[0]);
                    }
                    el.scrollTop = el.scrollHeight;
                };
            }
            reader.readAsText(event.data);
        };
        socket.onclose = function (event) {
            document.getElementById("msgDiv").style.cssText = "background-color:darkgray";
            document.title = "Serial Monitor Disconnected";
        };
        socket.onerror = function (event) {
            document.getElementById("msgDiv").style.cssText = "background-color:darkgray";
            document.title = "Serial Monitor Disconnected";
        };
        var sendmsg = function () {
            var text = document.getElementById("sendtext").value;
            var line_ending = document.getElementById("line_ending").value;
            if (!text) return;
            switch (line_ending) {
                case "Newline":
                    text += "\n";
                    break;
                case "Carriage return":
                    text += "\r";
                    break;
                case "Both NL&CR":
                    text += "\n\r";
            }
            if (socket.readyState == 1) socket.send(text);
            document.getElementById("sendtext").focus();
            document.getElementById("sendtext").select();
        };
        var sendIfEnter = function (event) {
            if (event.keyCode == 13) {
                sendmsg();
            }
        };
    </script>
</body>
</html>
)=====";
#endif