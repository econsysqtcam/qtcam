/*
 * aboutview.qml -- display about QtCam information
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

//Added by Dhurka - 19th Oct 2016
import QtQuick 2.0
import QtQuick.Controls 1.1
import QtQuick.Controls.Styles 1.1
import QtQuick.Dialogs 1.1

Item {

    id: aboutViewItem
    property variant aboutWindow
    property variant statusBarHeight

    Action {
        id: aboutAction
        onTriggered: {
            aboutWindow.show();
        }
    }

    Action {
        id: exitAction
        onTriggered: {
            exitDialog.visible = true
        }
    }

    Button {
        id: about
        x: 20
        y: root.height - statusBarHeight - 35
        opacity: 1
        action: aboutAction
        activeFocusOnPress : true
        style: ButtonStyle {
            background: Rectangle {
                border.width: control.activeFocus ? 1 :0
                color: "#222021"
                border.color: control.activeFocus ? "#ffffff" : "#222021"
            }
            label: Image {
                source: "images/about.png"
            }
        }
        Keys.onReturnPressed:  {
            aboutWindow.show();
        }
    }

    Button {
        id: exit
        x: 192
        y: root.height - statusBarHeight - 35
        opacity: 1
        action: exitAction
        activeFocusOnPress : true
        style: ButtonStyle {
            background: Rectangle {
                border.width: control.activeFocus ? 1 :0
                color: "#222021"
                border.color: control.activeFocus ? "#ffffff" : "#222021"
            }
            label: Image {
                source: "images/exit.png"
            }
        }
        Keys.onReturnPressed: {
            exitDialog.visible = true
        }
    }

    MessageDialog {
        id: exitDialog
        title: "Exit Application"
        icon: StandardIcon.Question
        text: qsTr("Do you want to really close the application?")
        standardButtons:  StandardButton.No | StandardButton.Yes
        onYes: Qt.quit()
        onNo: close()
    }

    Connections
    {
        target: root
        onSidebarVisibleStatus:
        {
            aboutViewItem.visible = status;
        }
    }

    Component.onCompleted:
    {
        var aboutComponent = Qt.createComponent("../about/about.qml")
        aboutWindow = aboutComponent.createObject(root);
    }

    Component.onDestruction:
    {
        exitDialog.visible = false
    }
}
