// Modified by Network Optix, Inc. Original copyright notice follows.

/****************************************************************************
**
** Copyright (C) 1992-2005 Trolltech AS. All rights reserved.
**
** This file is part of the QtGui module of the Qt Toolkit.
**
** This file may be used under the terms of the GNU General Public
** License version 2.0 as published by the Free Software Foundation
** and appearing in the file LICENSE.GPL included in the packaging of
** this file.  Please review the following information to ensure GNU
** General Public Licensing requirements will be met:
** http://www.trolltech.com/products/qt/opensource.html
**
** If you are unsure which license is appropriate for your use, please
** review the following information:
** http://www.trolltech.com/products/qt/licensing.html or contact the
** sales department at sales@trolltech.com.
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/

#include "graphics_label.h"
#include "graphics_label_p.h"

#include <QtGui/QFontMetricsF>

#include <nx/utils/math/fuzzy.h>
#include <nx/vms/client/core/utils/geometry.h>
#include <utils/common/scoped_painter_rollback.h>

#include <ui/common/text_pixmap_cache.h>
#include <nx/vms/client/desktop/style/skin.h>
#include <ui/workaround/sharp_pixmap_painting.h>

using nx::vms::client::core::Geometry;

// -------------------------------------------------------------------------- //
// GraphicsLabelPrivate
// -------------------------------------------------------------------------- //
void GraphicsLabelPrivate::init() {
    Q_Q(GraphicsLabel);

    performanceHint = GraphicsLabel::NoCaching;
    alignment = Qt::AlignTop | Qt::AlignLeft;
    q->setSizePolicy(QSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred, QSizePolicy::Label));
}

void GraphicsLabelPrivate::updateSizeHint() {
    Q_Q(GraphicsLabel);

    QRectF newRect;
    if (text.isEmpty()) {
        newRect = QRectF();
        pixmap = QPixmap();
    } else {
        QFontMetricsF metrics(q->font());
        newRect = metrics.boundingRect(QRectF(0.0, 0.0, 0.0, 0.0), Qt::AlignCenter, text);
        newRect.moveTopLeft(QPointF(0.0, 0.0)); /* Italicized fonts may result in negative left border. */
    }

    if (!qFuzzyEquals(newRect, rect)) {
        rect = newRect;

        q->updateGeometry();
    }
}

void GraphicsLabelPrivate::ensurePixmaps() {
    Q_Q(GraphicsLabel);

    if(!pixmapDirty)
        return;

    pixmap = QnTextPixmapCache::instance()->pixmap(text, q->font(), q->palette().color(QPalette::WindowText));
    pixmapDirty = false;
}

void GraphicsLabelPrivate::ensureStaticText() {
    if(!staticTextDirty)
        return;

    staticText.setText(text);
    staticTextDirty = false;
}

QColor GraphicsLabelPrivate::textColor() const {
    Q_Q(const GraphicsLabel);

    return q->palette().color(q->isEnabled() ? QPalette::Normal : QPalette::Disabled, QPalette::WindowText);
}


// -------------------------------------------------------------------------- //
// GraphicsLabel
// -------------------------------------------------------------------------- //
GraphicsLabel::GraphicsLabel(QGraphicsItem* parent, Qt::WindowFlags flags):
    GraphicsFrame(*new GraphicsLabelPrivate, parent, flags)
{
    Q_D(GraphicsLabel);
    d->init();
}

GraphicsLabel::GraphicsLabel(const QString& text, QGraphicsItem* parent, Qt::WindowFlags flags):
    GraphicsFrame(*new GraphicsLabelPrivate, parent, flags)
{
    Q_D(GraphicsLabel);
    d->init();
    setText(text);
}

GraphicsLabel::~GraphicsLabel() {
    return;
}

QString GraphicsLabel::text() const {
    Q_D(const GraphicsLabel);

    return d->text;
}

void GraphicsLabel::setText(const QString &text) {
    Q_D(GraphicsLabel);
    if (d->text == text)
        return;

    d->text = text;
    d->updateSizeHint();
    d->pixmapDirty = d->staticTextDirty = true;
    update();
}

void GraphicsLabel::clear() {
    setText(QString());
}

Qt::Alignment GraphicsLabel::alignment() const {
    return d_func()->alignment;
}

void GraphicsLabel::setAlignment(Qt::Alignment alignment) {
    Q_D(GraphicsLabel);

    if(d->alignment == alignment)
        return;

    d->alignment = alignment;

    update();
}

GraphicsLabel::PerformanceHint GraphicsLabel::performanceHint() const {
    return d_func()->performanceHint;
}

void GraphicsLabel::setPerformanceHint(PerformanceHint performanceHint) {
    Q_D(GraphicsLabel);

    d->performanceHint = performanceHint;

    switch (performanceHint) {
    case ModerateCaching:
        d->staticText.setPerformanceHint(QStaticText::ModerateCaching);
        break;
    case AggressiveCaching:
        d->staticText.setPerformanceHint(QStaticText::AggressiveCaching);
        break;
    default:
        break;
    }
}

QSizeF GraphicsLabel::sizeHint(Qt::SizeHint which, const QSizeF &constraint) const {
    if(which == Qt::MinimumSize || which == Qt::PreferredSize)
        return d_func()->rect.size();

    return base_type::sizeHint(which, constraint);
}

void GraphicsLabel::changeEvent(QEvent *event) {
    Q_D(GraphicsLabel);

    switch(event->type()) {
    case QEvent::FontChange:
        d->updateSizeHint();
        d->staticTextDirty = d->pixmapDirty = true;
        break;
    case QEvent::PaletteChange:
    case QEvent::EnabledChange:
        d->pixmapDirty = true;
        break;
    default:
        break;
    }

    base_type::changeEvent(event);
}

void GraphicsLabel::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
    Q_D(GraphicsLabel);
    base_type::paint(painter, option, widget);

    if (d->performanceHint == PixmapCaching)
    {
        d->ensurePixmaps();
        const auto dpiPixmapSize = (d->pixmap.size() / d->pixmap.devicePixelRatio());
        const QPointF position = Geometry::aligned(dpiPixmapSize, rect(), d->alignment).topLeft();
        paintPixmapSharp(painter, d->pixmap, position);
    }
    else
    {
        QnScopedPainterPenRollback penRollback(painter, d->textColor());
        QnScopedPainterFontRollback fontRollback(painter, font());

        if (d->performanceHint == NoCaching)
        {
            painter->drawText(rect(), d->alignment, d->text);
        }
        else
        {
            d->ensureStaticText();
            const QPointF position = Geometry::aligned(d->staticText.size(), rect(), d->alignment).topLeft();
            painter->drawStaticText(position, d->staticText);
        }
    }
}

