var tool = Qt.createComponent("MyToolTip.qml");
var tooltip;
var fadeInDelay;
var fadeOutDelay;
var toolTipXPosition;
var toolTipYPosition;
var tip;
function show() {
    tooltip = tool.createObject(root);
    tooltip.text = tip;
    tooltip.toolTipXPosition = toolTipXPosition;
    tooltip.toolTipYPosition = toolTipYPosition;
    tooltip.fadeInDelay = fadeInDelay;
    tooltip.fadeOutDelay = fadeOutDelay;
    tooltip.state = "poppedUp";
}

function close() {
    tooltip.state = "poppedDown";
}
