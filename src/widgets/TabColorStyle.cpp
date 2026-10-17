/*
    SPDX-FileCopyrightText: 2026 Konsole Developers

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "widgets/TabColorStyle.h"

#include <QPainter>
#include <QStyleOptionTab>

#include <KColorUtils>

#include "widgets/DetachableTabBar.h"

namespace Konsole
{
FullTabColors fullTabColors(const QColor &tabColor, bool active, const QColor &background)
{
    // Inactive tabs sit 60% of the way towards the background, so the active tab stands out
    const QColor fill = active ? tabColor : KColorUtils::mix(tabColor, background, 0.6);
    const QColor black(Qt::black);
    const QColor white(Qt::white);
    const QColor text = KColorUtils::contrastRatio(fill, black) >= KColorUtils::contrastRatio(fill, white) ? black : white;
    return {fill, text};
}

void paintFullTabFill(QPainter *painter, const QRect &rect, const QColor &fill)
{
    painter->save();
    painter->setRenderHint(QPainter::Antialiasing);
    painter->setPen(Qt::NoPen);
    painter->setBrush(fill);
    painter->drawRoundedRect(QRectF(rect).adjusted(1, 2, -1, -1), 3, 3);
    painter->restore();
}

TabColorStyle::TabColorStyle(DetachableTabBar *tabBar)
    : _tabBar(tabBar)
{
    setParent(tabBar);
}

void TabColorStyle::drawControl(ControlElement element, const QStyleOption *option, QPainter *painter, const QWidget *widget) const
{
    const auto *tabOption = qstyleoption_cast<const QStyleOptionTab *>(option);
    if (tabOption == nullptr || widget != _tabBar || (element != CE_TabBarTabShape && element != CE_TabBarTabLabel)) {
        QProxyStyle::drawControl(element, option, painter, widget);
        return;
    }

#if QT_VERSION >= QT_VERSION_CHECK(6, 6, 0)
    const int index = tabOption->tabIndex;
#else
    const int index = _tabBar->tabAt(tabOption->rect.center());
#endif
    const QColor tabColor = _tabBar->fullTabColor(index);
    if (!tabColor.isValid()) {
        QProxyStyle::drawControl(element, option, painter, widget);
        return;
    }

    const bool active = tabOption->state & State_Selected;
    FullTabColors colors = fullTabColors(tabColor, active, tabOption->palette.window().color());

    if (element == CE_TabBarTabShape) {
        if (!active && (tabOption->state & State_MouseOver)) {
            colors.fill = KColorUtils::mix(colors.fill, tabColor, 0.5);
        }
        paintFullTabFill(painter, tabOption->rect, colors.fill);
        return;
    }

    // CE_TabBarTabLabel: DetachableTabBar already set the text color, show activity as bold text
    QStyleOptionTab label(*tabOption);

    painter->save();
    if (_tabBar->tabTextColor(index).isValid()) {
        QFont font = painter->font();
        font.setBold(true);
        painter->setFont(font);
        label.fontMetrics = QFontMetrics(font);
        // QTabBar elided the text for the regular font, so elide it again for the wider bold one
        const int textWidth = subElementRect(SE_TabBarTabText, &label, widget).width();
        label.text = label.fontMetrics.elidedText(_tabBar->tabText(index), _tabBar->elideMode(), textWidth, Qt::TextShowMnemonic);
    }
    QProxyStyle::drawControl(element, &label, painter, widget);
    painter->restore();
}
}

#include "moc_TabColorStyle.cpp"
