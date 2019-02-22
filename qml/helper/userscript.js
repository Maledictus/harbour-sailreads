function elementContainedInBox(element, box) {
    var rect = element.getBoundingClientRect();
    return ((box.left <= rect.left) && (box.right >= rect.right) &&
            (box.top <= rect.top) && (box.bottom >= rect.bottom));
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
