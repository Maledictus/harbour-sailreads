// Attempt to fix news.google.com not scrolling
//var hs = document.getElementsByTagName('header');
//for (var i=0, max = hs.length; i < max; i++) {
//    if (hs[i].role == "banner")
//        hs[i].parentNode.removeChild(hs[i]);
//    //hs[i].setAttribute('style',"-webkit-overflow-scrolling: auto; overflow: scroll");
//    hs[i].parentNode.style.webkitOverflowScrolling = "auto";
//    hs[i].parentNode.style.overflow = "scroll";
//}

function getImgFullUri(uri) {
//    if ((uri.slice(0, 7) === 'http://') ||
//        (uri.slice(0, 8) === 'https://') ||
//        (uri.slice(0, 7) === 'file://')) {
//        return uri;
//    } else if (uri.slice(0, 1) === '/') {
//        var docuri = document.documentURI;
//        var firstcolon = docuri.indexOf('://');
//        var protocol = 'http://';
//        if (firstcolon !== -1) {
//            protocol = docuri.slice(0, firstcolon + 3);
//        }
//        return protocol + document.domain + uri;
//    } else {
//        var base = document.baseURI;
//        var lastslash = base.lastIndexOf('/');
//        if (lastslash === -1) {
//            return base + '/' + uri;
//        } else {
//            return base.slice(0, lastslash + 1) + uri;
//        }
//    }
    // Sometimes life can be made a lot easier
    var a;
    if (!a) a = document.createElement('a');
    a.href = uri;

    return a.href;
}

function elementContainedInBox(element, box) {
    var rect = element.getBoundingClientRect();
    return ((box.left <= rect.left) && (box.right >= rect.right) &&
            (box.top <= rect.top) && (box.bottom >= rect.bottom));
}

function getSelectedData(element) {
    var node = element;
    var data = new Object();

    var nodeName = node.nodeName.toLowerCase();
    if (nodeName === 'img') {
        data.img = getImgFullUri(node.getAttribute('src'));
    } if (nodeName === 'a') {
        data.href = node.href;
        data.title = node.title;
    }

    // If the parent tag is a hyperlink, we want it too.
    var parent = node.parentNode;
    if ((nodeName !== 'a') && parent && (parent.nodeName.toLowerCase() === 'a')) {
        data.href = parent.href;
        data.title = parent.title;
        node = parent;
    }
    if ((nodeName !== 'img') && parent && (parent.nodeName.toLowerCase() === 'img')) {
        data.img = getImgFullUri(node.getAttribute('src'));
        node = parent;
    }

    return data;
}

function adjustSelection(selection) {
    // FIXME: allow selecting two consecutive blocks, instead of
    // interpolating to the containing block.

    var data = new Object();

    //console.debug("[userscript.js] Jumped into adjustSelection")

    var centerX = (selection.left + selection.right) / 2;
    var centerY = (selection.top + selection.bottom) / 2;
    var element = document.elementFromPoint(centerX, centerY);
    var parent = element;
    while (elementContainedInBox(parent, selection)) {
        parent = parent.parentNode;
    }
    element = parent;

    node = element.cloneNode(true);
    // filter out script nodes
    var scripts = node.getElementsByTagName('script');
    while (scripts.length > 0) {
        var scriptNode = scripts[0];
        if (scriptNode.parentNode) {
            scriptNode.parentNode.removeChild(scriptNode);
        }
    }
    data.html = node.outerHTML;
    data.nodeName = node.nodeName.toLowerCase();
    // FIXME: extract the text and images in the order they appear in the block,
    // so that this order is respected when the data is pushed to the clipboard.
    data.text = node.textContent;
    var images = [];
    var imgs = node.getElementsByTagName('img');
    for (var i = 0; i < imgs.length; i++) {
        images.push(getImgFullUri(imgs[i].getAttribute('src')));
    }
    if (images.length > 0) {
        data.img = images;
    }

    return data
}

function checkNode(e, node) {
    // hook for Open in New Tab (link with target)
    if (node.tagName === 'A') {
        var link = new Object({'type':'link', 'pageX': e.pageX, 'pageY': e.pageY})
        if (node.hasAttribute('target'))
            link.target = node.getAttribute('target');
        link.href = node.href //node.getAttribute('href'); // We want always the absolute link
        navigator.qt.postMessage( JSON.stringify(link) );
    }
}

// Catch window open events as normal links
window.open = function (url, windowName, windowFeatures) {
    var link = new Object({'type':'link', 'target':'_blank', 'href':url});
    navigator.qt.postMessage( JSON.stringify(link) );
}

//window.onerror = function (errorMsg, url, lineNumber, column, errorObj) {
//    var err = new Object({'type':'error', 'msg': errorMsg, 'url' : url, 'line': lineNumber, 'strace' : errorObj});
//    navigator.qt.postMessage( JSON.stringify(err) );
//}

// virtual keyboard hook
window.document.addEventListener('click', (function(e) {
    if (e.srcElement.tagName === ('INPUT'||'TEXTAREA'||'FORM')) {
        var inputContext = new Object({'type':'input', 'state':'show'})
        navigator.qt.postMessage(JSON.stringify(inputContext))
    }
}), true);
window.document.addEventListener('focus', (function(e) {
    if (e.srcElement.tagName === ('INPUT'||'TEXTAREA'||'FORM') ||
            document.activeElement && document.activeElement.tagName.toLowerCase() == 'input' &&
            document.activeElement.type == 'text' || document.activeElement.tagName.toLowerCase() == 'textarea') {
        var inputContext = new Object({'type':'input', 'state':'show'})
        navigator.qt.postMessage(JSON.stringify(inputContext))
    }
}), true);
window.document.addEventListener('blur', (function(e) {
    if (e.srcElement.tagName === ('INPUT'||'TEXTAREA'||'FORM')) {
        var inputContext = new Object({'type':'input', 'state':'hide'})
        navigator.qt.postMessage(JSON.stringify(inputContext))
    }
}), true);

document.documentElement.addEventListener('click', (function(e) {
    var node = e.target;
    while(node) {
        checkNode(e, node);
        node = node.parentNode;
    }
}), true);

//var frames = document.documentElement.getElementsByTagName('iframe');

//for (var i=0; i<frames.length; i++) {
//    frames[i].onload = function() {
//        var link = new Object({'type':'link', 'pageX': 0, 'pageY': 0, 'target': '_blank'})
//        if (this.contentWindow.location != "") link.href = this.contentWindow.location
//        navigator.qt.postMessage( JSON.stringify(link) );
//    }
//}

//Attach to window.onload instead of overwriting it
window.addEventListener("load", function(){

    if (document.activeElement && document.activeElement.tagName.toLowerCase() == 'input' &&
            document.activeElement.type == 'text' || document.activeElement.tagName.toLowerCase() == 'textarea') {
        var inputContext = new Object({'type':'input', 'state':'show'})
        navigator.qt.postMessage(JSON.stringify(inputContext))
    }

    var inputs = document.getElementsByTagName('INPUT');
    var textareas = document.getElementsByTagName('TEXTAREA');

    for(var i = 0; i < inputs.length; i++) {
        var elem = inputs[i];

        if(elem.type == 'text' || elem.type == 'password') {
            elem.onfocus = function() {
                var inputContext = new Object({'type':'input', 'state':'show'})
                navigator.qt.postMessage(JSON.stringify(inputContext))
            }
            elem.onblur = function() {
                var inputContext = new Object({'type':'input', 'state':'hide'})
                navigator.qt.postMessage(JSON.stringify(inputContext))
            }
        }
    }

    for(var j = 0; j < textareas.length; j++) {
        var telem = textareas[j];

        telem.onfocus = function() {
            var inputContext = new Object({'type':'input', 'state':'show'})
            navigator.qt.postMessage(JSON.stringify(inputContext))
        }
        telem.onblur = function() {
            var inputContext = new Object({'type':'input', 'state':'hide'})
            navigator.qt.postMessage(JSON.stringify(inputContext))
        }
    }
}, true);

window.onunload = function(e) {
    // Don't do anything simply be there
    console.debug("I don't do anything but making window.onload work after back or forward navigation");
}

navigator.qt.onmessage = function(ev) {
    //console.debug("[userscript.js] message received")
    var data = JSON.parse(ev.data)
    if (data.type === 'readability') {

        readStyle='style-novel';
        readSize='size-large';
        readMargin='margin-wide';

        _readability_script = document.createElement('SCRIPT');
        _readability_script.type = 'text/javascript';
        _readability_script.text = data.content;
        document.getElementsByTagName('head')[0].appendChild(_readability_script);
    }
    else if (data.type === 'adjustselection') {
        //console.debug("[userscript.js] 'query' received")
        var selection = adjustSelection(data);
        selection.type = 'selectionadjusted';
        navigator.qt.postMessage(JSON.stringify(selection));

    }
    else if (data.type === "search") {
        findString(data.searchTerm)
    }
    else if (data.type === "setInput") {
        setInputText(data.elem, data.text);
    }
    else if (data.type === "getInput") {
        getInputText(data.elem);
    }
}

function findTag(element, tagN) {
    var currelement = element

    while(currelement) {
        if(currelement.tagName === "BODY")
            return null;

        if(currelement.tagName === tagN)
            break;

        currelement = currelement.parentNode;
    }

    return currelement;
}

// FIXME: experiementing on tap and hold
var hold;
var longpressDetected = false;
var currentTouch = null;

function longPressed(x, y, element) {
    longpressDetected = true;
    //var element = document.elementFromPoint(x, y);

    // FIXME: should travel nodes to find links
    var data = new Object({'type': 'longpress', 'pageX': x, 'pageY': y})
    data.href = 'CANT FIND LINK'
    var anchors = findTag(element,"A");
    var images = findTag(element,"IMG");
    var videos = findTag(element,"VIDEO");
    var audios = findTag(element,"AUDIO");
    var input = findTag(element,"INPUT");
    var form = findTag(element, "FORM");
    var textarea = findTag(element, "TEXTAREA");
    if (anchors) {
        data.href = anchors.href //getAttribute('href'); // We always want the absolute link
    }
    if (images) {
        data.img = getImgFullUri(images.getAttribute('src'));
    }
    if (videos) {
        data.video = videos.hasChildNodes();
        var children = videos.childNodes;
        for (var i = 0; i < children.length; i++) {
            if (children[i].tagName === 'SOURCE') {
                data.video = getImgFullUri(children[i].getAttribute('src'));
                break;
            }
        }
        if (element.tagName.getAttribute('src') != "") {
            data.video = getImgFullUri(videos.tagName.getAttribute('src'));
        }
    }
    if (audios) {
        data.video = audios.hasChildNodes();
        var children = audios.childNodes;
        for (var i = 0; i < children.length; i++) {
            if (children[i].tagName === 'SOURCE') {
                data.video = getImgFullUri(children[i].getAttribute('src'));
                break;
            }
        }
        if (element.tagName.getAttribute('src') != "") {
            data.video = getImgFullUri(audios.tagName.getAttribute('src'));
        }
    }

    var boundingRect = element.getBoundingClientRect();
    data.left = boundingRect.left;
    data.top = Math.round(boundingRect.top);
    data.width = boundingRect.width;
    data.height = boundingRect.height;

    node = element.cloneNode(true);
    // filter out script nodes
    var scripts = node.getElementsByTagName('script');
    while (scripts.length > 0) {
        var scriptNode = scripts[0];
        if (scriptNode.parentNode) {
            scriptNode.parentNode.removeChild(scriptNode);
        }
    }
    data.html = node.outerHTML;
    data.nodeName = node.nodeName.toLowerCase();
    // FIXME: extract the text and images in the order they appear in the block,
    // so that this order is respected when the data is pushed to the clipboard.
    if (data.nodeName == "input" || data.nodeName == "form" || data.nodeName == "textarea") {
        fakeKeyUpEvent(node);
        if (node.value != "") data.input = node.value
        else data.input = " "
        data.id = node.id
    }

    data.text = node.textContent;
//    var images = [];
//    var imgs = node.getElementsByTagName('img');
//    for (var i = 0; i < imgs.length; i++) {
//        images.push(getImgFullUri(imgs[i].getAttribute('src')));
//    }
//    if (images.length > 0) {
//        data.images = images;
//    }

    navigator.qt.postMessage( JSON.stringify(data) );
}

document.addEventListener('touchstart', (function(event) {
    if (event.touches.length > 1) {
        event.preventDefault();
        return;
    }
    else if (event.touches.length == 1) {
        currentTouch = event.touches[0];
        hold = setTimeout(longPressed, 800, currentTouch.clientX, currentTouch.clientY, event.target);
    }
}), true);

document.addEventListener('touchend', (function(event) {
    if (longpressDetected) {
        longpressDetected = false
        event.preventDefault();
    }
    currentTouch = null;
    clearTimeout(hold);
}), true);


function distance(touch1, touch2) {
    return Math.sqrt(Math.pow(touch2.clientX - touch1.clientX, 2) +
                     Math.pow(touch2.clientY - touch1.clientY, 2));
}

document.addEventListener('touchmove', (function(event) {
    if ((event.changedTouches.length > 1) || (distance(event.changedTouches[0], currentTouch) > 3)) {
        clearTimeout(hold);
        currentTouch = null;
    }
}), true);

function findString(str) {
 //if (parseInt(navigator.appVersion)<4) return;
 var strFound;
 if (window.find) {
  strFound=self.find(str);
  if (!strFound) {
   strFound=self.find(str,0,1);
   while (self.find(str,0,1)) continue;
  }
 }
 if (!strFound) {
     var data = new Object({'type': 'search', 'errorMsg': "String '" + str + "' not found!"})
     navigator.qt.postMessage( JSON.stringify(data) );
     //alert ("String '"+str+"' not found!")
 }
 return;
}

function fakeKeyUpEvent(target) {
    var event = document.createEvent("KeyboardEvent");
    event.initKeyboardEvent('keyup', true, false, window, '', 0, false, false, false, false, false);
    target.dispatchEvent(event);
}

function getInputText(id) {
    var inElemValue = document.getElementById(id).value;
    var data = new Object({'type': 'input', 'input': inElemValue});
    navigator.qt.postMessage( JSON.stringify(data) );
}

function setInputText(id, text) {
    var inElem = document.getElementById(id);
    inElem.value = text;
    fakeKeyUpEvent(inElem);
}

//canvg();
