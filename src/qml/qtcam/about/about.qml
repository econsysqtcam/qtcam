/*
 * about.qml -- Display the about window
 * Copyright © 2015  e-con Systems India Pvt. Limited
 *
 * This file is part of Qtcam.
 *
 * Qtcam is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3, or (at your option)
 * any later version.
 *
 * Qtcam is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Qtcam. If not, see <http://www.gnu.org/licenses/>.
 */


import QtQuick 2.0
import QtQuick.Window 2.0
import QtQuick.Controls 1.1
import QtQuick.Controls.Styles 1.0
import econ.camera.about 1.0
Window {
    //Item {
    width:800
    height:480
    modality: Qt.WindowModal
    flags: Qt.FramelessWindowHint
    id: aboutRoot
    //    anchors.centerIn: parent
    Action {
        id: windowClose
        onTriggered: {
            aboutRoot.close()
        }
    }
    Image {
        id: bg
        source: "images/bg.png"
        x: 0
        y: 0
        opacity: 1
    }
    Text {
        id: product_name
        font.pixelSize: 18
        font.family: "Ubuntu"
        color: "#ffffff"
        smooth: true
        //anchors.centerIn: parent
        x: 380
        y: 226.5
        opacity: 1
    }
    Rectangle {
        id: linkRect
        x: 344
        y: 363.5
        width: 110
        height: 20
        color: "#222021"
        Text {
            id: linkText
            text: "<a href=\"http://e-consystems.com/\">e-consystems</a>"
            font.pixelSize: 18
            font.family: "Ubuntu"
            color: "#ffffff"
            smooth: true
            opacity: 1
        }
        MouseArea {
            anchors.fill: parent
            hoverEnabled: true
            cursorShape: (containsMouse ? Qt.PointingHandCursor : Qt.ArrowCursor)
            onClicked: Qt.openUrlExternally("http://e-consystems.com")
        }
    }
    Text {
        id: version
        font.pixelSize: 18
        font.family: "Ubuntu"
        color: "#ffffff"
        smooth: true
        x: 382
        y: 256.5
        opacity: 1
    }

    Text {
        id: copyrights
        text: "Copyright © 2015  e-con Systems India Pvt. Limited"
        font.pixelSize: 15
        font.family: "Ubuntu Light"
        color: "#ffffff"
        smooth: true
        x: 238
        y: 399.25
        opacity: 1

    }
    Button {
        id: close_selected
        x: 670
        y: 418
        opacity: 1
        focus: true
        action: windowClose
        style: ButtonStyle {
            background: Rectangle {
                border.width: control.activeFocus ? 3 :0
                color: "#222021"
                border.color: control.activeFocus ? "#ffffff" : "#222021"
                radius: 5
            }
            label: Image {
                horizontalAlignment: Image.AlignHCenter
                verticalAlignment: Image.AlignVCenter
                fillMode: Image.PreserveAspectFit
                source: "images/close_selected.png"
            }
        }
        Keys.onReturnPressed: {
            aboutRoot.close()
        }
    }
    ScrollView {
        id: scrollId
        x: 94
        y: 290
        width: parent.width - 100
        height: 70
        style: ScrollViewStyle {
            scrollToClickedPosition: true
            handle: Image {
                id: scrollhandle
                source: "../Views/images/scroller.png"
            }
            scrollBarBackground: Image {
                id: scrollStyle
                source: "../Views/images/Scroller_bg.png"
            }
            incrementControl: Image {
                id: increment
                source: "../Views/images/down_arrow.png"
            }
            decrementControl: Image {
                id: decrement
                source: "../Views/images/up_arrow.png"
            }
        }

        Text {
            id: content
            font.pixelSize: 15
            font.family: "Ubuntu Light"
            color: "#ffffff"
            smooth: true
            opacity: 1
            onLinkActivated: Qt.openUrlExternally(link)
        }
    }
    Image {
        id: logo
        fillMode: Image.PreserveAspectFit
        source: "images/logo.png"
        x: -50
        y: -180
        opacity: 1
    }
    AboutPage {
        id: aboutPage
    }
    Component.onCompleted:  {
        product_name.text = aboutPage.getApplicationName().toString()
        version.text = aboutPage.getApplicationVersionNumber().toString()
        content.text = aboutPage.getCopyRight().toString()
    }
}
