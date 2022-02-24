// Copyright 2018-present Network Optix, Inc. Licensed under MPL 2.0: www.mozilla.org/MPL/2.0/

#include "webview_window.h"
#include "webview_controller.h"

#include <QtWidgets/QVBoxLayout>
#include <QtQuick/QQuickItem>
#include <QtQuickWidgets/QQuickWidget>
#include <QtCore/QVariant>

#include <client_core/client_core_module.h>

namespace nx::vms::client::desktop {

struct WebViewWindow::Private
{
    QScopedPointer<QQuickWidget> quickWidget;
    QScopedPointer<WebViewController> controller;

    Private(WebViewWindow* webViewWindow);
};

WebViewWindow::Private::Private(WebViewWindow* webViewWindow):
    quickWidget(new QQuickWidget(qnClientCoreModule->mainQmlEngine(), webViewWindow)),
    controller(new WebViewController(webViewWindow))
{
}

namespace {

static constexpr Qt::WindowFlags kCommonFlags =
    Qt::WindowSystemMenuHint
    | Qt::WindowTitleHint
    | Qt::WindowMinimizeButtonHint
    | Qt::WindowMaximizeButtonHint
    | Qt::WindowCloseButtonHint
    | Qt::Dialog;

} // namespace

WebViewWindow::WebViewWindow(QWidget* parent):
    base_type(parent, kCommonFlags),
    d(new Private(this))
{
    // QML should fill the whole window.
    const auto layout = new QVBoxLayout();
    layout->setContentsMargins({});
    layout->addWidget(d->quickWidget.get());
    setLayout(layout);

    setMinimumSize({214, 0}); //< Leave some space for window controls.

    connect(d->controller.get(), &WebViewController::rootReady, this, &WebViewWindow::rootReady);
}

void WebViewWindow::loadAndShow()
{
    d->controller->loadIn(d->quickWidget.get());
    show();
}

void WebViewWindow::setWindowGeometry(const QRect& geometry)
{
    setGeometry(geometry.marginsRemoved(windowHandle()->frameMargins()));
}

void WebViewWindow::setWindowSize(int width, int height)
{
    resize(width, height);
}

WebViewWindow::~WebViewWindow()
{
}

WebViewController* WebViewWindow::controller() const
{
    return d->controller.get();
}

} // nx::vms::client::desktop
