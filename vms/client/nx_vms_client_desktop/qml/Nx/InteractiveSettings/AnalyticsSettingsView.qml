// Copyright 2018-present Network Optix, Inc. Licensed under MPL 2.0: www.mozilla.org/MPL/2.0/

import QtQuick
import QtQuick.Layouts

import "private"

StackLayout
{
    required property var viewModel

    property alias apiIntegrations: apiIntegrations
    property alias settingsView: settingsView
    property alias settingsViewHeader: header
    property alias settingsViewPlaceholder: placeholder

    currentIndex:
        Array.from(children).findIndex((widget) => widget.itemType === viewModel.currentItemType)

    Plugins
    {
        id: plugins

        property string itemType: "Plugins"
    }

    ApiIntegrations
    {
        id: apiIntegrations

        property string itemType: "ApiIntegrations"

        requestsModel: viewModel.requestsModel
    }

    SettingsView
    {
        id: settingsView

        property string itemType: "Engine"
        property var engineId: null

        headerItem: IntegrationPanel
        {
            id: header
            engineInfo: viewModel.currentEngineInfo
            checked:
                checkable && viewModel.enabledEngines.indexOf(viewModel.currentEngineId) !== -1
        }

        placeholderItem: SettingsPlaceholder
        {
            id: placeholder
        }

        Connections
        {
            target: viewModel

            function onCurrentSectionPathChanged()
            {
                settingsView.selectSection(viewModel.currentSectionPath)
            }

            function onSettingsViewStateChanged(model, initialValues)
            {
                const restoreScrollPosition = viewModel.currentEngineId === settingsView.engineId
                settingsView.engineId = viewModel.currentEngineId

                settingsView.loadModel(
                    model ?? {},
                    initialValues ?? {},
                    viewModel.currentSectionPath,
                    restoreScrollPosition)
            }

            function onSettingsViewValuesChanged(values)
            {
                settingsView.setValues(values ?? {})
            }

            function onCurrentSettingsErrorsChanged()
            {
                settingsView.setErrors(viewModel.currentSettingsErrors)
            }
        }
    }
}
