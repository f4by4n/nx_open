// Copyright 2018-present Network Optix, Inc. Licensed under MPL 2.0: www.mozilla.org/MPL/2.0/

import QtQuick 2.11
import QtQuick.Controls 2.15
import QtQuick.Window 2.15
import QtQuick.Layouts 1.15

import Nx 1.0
import Nx.Core 1.0
import Nx.Controls 1.0
import Nx.Models 1.0

import nx.client.desktop 1.0
import nx.vms.client.core 1.0
import nx.vms.client.desktop 1.0

import ".."
import "Tabs"
import "Components"

DialogWithState
{
    id: dialog
    objectName: "groupEditDialog" //< For autotesting.

    modality: Qt.NonModal

    width: minimumWidth
    height: minimumHeight

    minimumWidth: 765
    minimumHeight: 672

    color: ColorTheme.colors.dark7

    // State properties.
    property alias groupId: membersModel.groupId
    property alias name: generalSettings.name
    property bool isLdap: false
    property bool isPredefined: false
    property alias description: generalSettings.description
    property alias parentGroups: membersModel.parentGroups
    property alias groups: membersModel.groups
    property alias users: membersModel.users
    property alias globalPermissions: globalPermissionsModel.globalPermissions
    property alias sharedResources: membersModel.sharedResources
    property bool parentGroupsEditable
    property bool membersEditable
    property bool permissionsEditable

    // Mapped to dialog property.
    property alias tabIndex: tabControl.currentTabIndex
    property bool isSaving: false
    property alias editingContext: membersModel.editingContext
    property var self

    signal deleteRequested()
    signal groupClicked(var id)

    title: qsTr("Group - %1").arg(name)

    validateFunc: () =>
    {
        if (generalSettings.validate())
            return true

        dialog.tabIndex = 0
        return false
    }

    MembersModel
    {
        id: membersModel
    }

    DialogTabControl
    {
        id: tabControl

        anchors.fill: parent
        anchors.bottomMargin: buttonBox.height

        dialogLeftPadding: dialog.leftPadding
        dialogRightPadding: dialog.rightPadding

        Tab
        {
            button: DialogTabButton
            {
                text: qsTr("General")
            }

            page: GroupGeneralTab
            {
                id: generalSettings
                anchors.fill: parent

                self: dialog.self

                enabled: !dialog.isSaving

                nameEditable: !dialog.isLdap && !dialog.isPredefined && !dialog.isSaving
                isLdap: dialog.isLdap
                isPredefined: dialog.isPredefined
                groups: dialog.parentGroups
                userCount: membersModel.users.length
                groupCount: membersModel.groups.length
                deleteAvailable: !dialog.isLdap && !dialog.isPredefined && !dialog.isSaving
                editingContext: membersModel.editingContext
                globalPermissions: dialog.globalPermissions
                sharedResources: dialog.sharedResources

                onDeleteRequested: dialog.deleteRequested()
                onGroupClicked: (id) => { dialog.groupClicked(id) }
                onMoreGroupsClicked: dialog.tabIndex = 1
            }
        }
        Tab
        {
            button: DialogTabButton
            {
                text: qsTr("Groups")
            }

            visible: !dialog.isPredefined

            page: ParentGroupsTab
            {
                id: groupsSettings
                anchors.fill: parent

                model: membersModel
                editable: dialog.parentGroupsEditable
                enabled: !dialog.isSaving

                onAddGroupClicked: dialog.groupClicked(null)
            }
        }

        Tab
        {
            button: DialogTabButton
            {
                text: qsTr("Resources")
            }

            page: PermissionsTab
            {
                id: permissionSettings
                anchors.fill: parent

                enabled: !dialog.isPredefined && dialog.permissionsEditable && !dialog.isSaving

                buttonBox: buttonBox
                editingContext: membersModel.editingContext
            }
        }

        Tab
        {
            button: DialogTabButton
            {
                text: qsTr("Global Permissions")
            }

            page: GlobalPermissionsTab
            {
                id: globalPermissionSettings
                anchors.fill: parent

                enabled: !dialog.isPredefined && dialog.permissionsEditable && !dialog.isSaving

                model: GlobalPermissionsModel
                {
                    id: globalPermissionsModel
                    context: membersModel.editingContext
                }
            }
        }

        Tab
        {
            button: DialogTabButton
            {
                text: qsTr("Members")
            }

            page: GroupMembersTab
            {
                id: membersSettings
                anchors.fill: parent

                model: membersModel
                editable: !dialog.isLdap && dialog.membersEditable
                enabled: !dialog.isSaving
            }
        }
    }

    buttonBox: DialogButtonBoxWithPreloader
    {
        id: buttonBox

        modified: dialog.modified
        preloaderButton: dialog.isSaving
            ? (dialog.self.isOkClicked()
                ? DialogButtonBox.Ok
                : DialogButtonBox.Apply)
            : undefined
    }
}
