// Copyright 2018-present Network Optix, Inc. Licensed under MPL 2.0: www.mozilla.org/MPL/2.0/

import QtQuick
import QtQuick.Layouts

import Nx.Controls 1.0
import Nx.Core 1.0

Rectangle
{
    id: control

    enum Style { Info, Warning, Error }

    property int style: DialogBanner.Style.Info
    readonly property var styleData:
    {
        switch (style)
        {
            case DialogBanner.Style.Error:
                return {
                    "background": ColorTheme.colors.attention.red_bg,
                    "border": ColorTheme.colors.attention.red_dark,
                    "icon": "image://svg/skin/banners/error.svg"
                }

            case DialogBanner.Style.Warning:
                return {
                    "background": ColorTheme.colors.attention.yellow_bg,
                    "border": ColorTheme.colors.attention.yellow_dark,
                    "icon": "image://svg/skin/banners/warning.svg"
                }

            case DialogBanner.Style.Info:
            default:
                return {
                    "background": ColorTheme.colors.attention.blue_bg,
                    "border": ColorTheme.colors.attention.blue_dark,
                    "icon": "image://svg/skin/banners/info.svg"
                }
        }
    }

    property alias text: bannerText.text
    property alias buttonText: bannerButton.text
    property alias buttonIcon: bannerButton.icon.source
    property alias closeVisible: closeButton.visible

    signal buttonClicked()
    signal closeClicked()

    color: styleData.background
    border.color: styleData.border
    border.width: 1

    implicitHeight: bannerLayout.height + 20

    RowLayout
    {
        id: bannerLayout

        anchors
        {
            left: parent.left
            leftMargin: 16
            top: parent.top
            topMargin: 10
            right: parent.right
            rightMargin: 16
        }

        spacing: 8

        Image
        {
            id: icon

            Layout.alignment: Qt.AlignTop | Qt.AlignLeft

            source: control.styleData.icon
            sourceSize: Qt.size(20, 20)
        }

        ColumnLayout
        {
            spacing: 8

            Layout.alignment: Qt.AlignTop
            Layout.fillWidth: true

            Text
            {
                id: bannerText

                Layout.alignment: Qt.AlignTop
                Layout.fillWidth: true

                topPadding: 2

                wrapMode: Text.WordWrap

                color: ColorTheme.colors.light4
                font: Qt.font({pixelSize: 14, weight: Font.Medium})
            }

            Button
            {
                id: bannerButton

                Layout.alignment: Qt.AlignLeft
                visible: !!text

                leftPadding: 6
                rightPadding: 6
                spacing: 4

                textColor: ColorTheme.colors.light4

                background: Rectangle
                {
                    radius: 4
                    color: (!bannerButton.pressed && bannerButton.hovered)
                        ? Qt.rgba(1, 1, 1, 0.12) //< Hovered.
                        : bannerButton.pressed
                            ? Qt.rgba(1, 1, 1, 0.08) //< Pressed.
                            : Qt.rgba(1, 1, 1, 0.1) //< Default.
                }

                onClicked: control.buttonClicked()
            }

        }

        ImageButton
        {
            id: closeButton

            visible: false

            Layout.alignment: Qt.AlignTop | Qt.AlignRight

            icon.source: "image://svg/skin/banners/close.svg"
            icon.width: 20
            icon.height: 20
            icon.color: ColorTheme.colors.light4

            onClicked: control.closeClicked()
        }
    }
}
