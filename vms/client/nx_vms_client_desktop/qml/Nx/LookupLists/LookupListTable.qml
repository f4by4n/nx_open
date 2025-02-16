// Copyright 2018-present Network Optix, Inc. Licensed under MPL 2.0: www.mozilla.org/MPL/2.0/

import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQml
import Qt.labs.qmlmodels

import Nx
import Nx.Core
import Nx.Controls
import Nx.Dialogs

import nx.vms.client.desktop

TableView
{
    id: control

    function getColumnWidth(columnIndex)
    {
        return Math.max(124, getButtonItem(columnIndex).implicitWidth)
    }

    columnSpacing: 0
    rowSpacing: 0

    horizontalHeaderVisible: true

    columnWidthProvider: function (columnIndex)
    {
        if (isCheckboxColumn(control.model, columnIndex))
            return Math.min(getButtonItem(columnIndex).implicitWidth, 124)

        // Stretch the last column to the entire free width.
        if (columnIndex === columns - 1)
        {
            let occupiedSpace = 0
            for (let i = 0; i < columns - 2; ++i)
                occupiedSpace += getColumnWidth(columnIndex)

            return width - occupiedSpace
        }

        return getColumnWidth(columnIndex)
    }

    DelegateChooser
    {
        id: chooser

        role: "type"

        DelegateChoice
        {
            roleValue: "checkbox"

            Rectangle
            {
                implicitWidth: 28
                implicitHeight: 28
                color: ColorTheme.colors.dark7

                CheckBox
                {
                    x: 8
                    y: 6
                    anchors.verticalCenter: parent.verticalCenter
                    // checked: control.model["checked"]
                }
            }
        }

        DelegateChoice
        {
            roleValue: "text"

            Rectangle
            {
                implicitWidth: Math.max(124, title.implicitWidth)
                implicitHeight: 28
                color: ColorTheme.colors.dark7

                required property bool selected

                Text
                {
                    id: title

                    x: 8
                    y: 6

                    text: display || qsTr("NONE")
                    color: display ? ColorTheme.light : ColorTheme.colors.dark17
                }
            }
        }
    }

    delegate: chooser
}
