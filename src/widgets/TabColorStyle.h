/*
    SPDX-FileCopyrightText: 2026 Konsole Developers

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef TABCOLORSTYLE_H
#define TABCOLORSTYLE_H

#include <QColor>
#include <QProxyStyle>

#include "konsoleprivate_export.h"

namespace Konsole
{
/** The colors of a tab drawn in the "Full tab" Tab Color style */
struct FullTabColors {
    QColor fill;
    QColor text;
};

/**
 * Returns the fill and text colors for a tab filled with @p tabColor.
 * Inactive tabs are muted towards @p background, and the text is black or
 * white, whichever is more readable on the fill.
 */
KONSOLEPRIVATE_EXPORT FullTabColors fullTabColors(const QColor &tabColor, bool active, const QColor &background);

class DetachableTabBar;

/**
 * Paints colored tabs of a DetachableTabBar in the "Full tab" Tab Color style:
 * the tab is filled with its color, and activity is shown as bold text.
 * Everything else is left to the base style.
 */
class TabColorStyle : public QProxyStyle
{
    Q_OBJECT
public:
    explicit TabColorStyle(DetachableTabBar *tabBar);

    void drawControl(ControlElement element, const QStyleOption *option, QPainter *painter, const QWidget *widget = nullptr) const override;

private:
    DetachableTabBar *_tabBar;
};
}

#endif
