/*
    SPDX-FileCopyrightText: 2026 Konsole Developers

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "../widgets/TabColorStyle.h"

#include <KColorUtils>
#include <QTest>

using namespace Konsole;

namespace Konsole
{
class TabColorStyleTest : public QObject
{
    Q_OBJECT

private Q_SLOTS:
    void testActiveTabIsFilledWithTabColor();
    void testLightTabColorGetsDarkText();
    void testInactiveTabIsMutedTowardsBackground_data();
    void testInactiveTabIsMutedTowardsBackground();
};
}

void TabColorStyleTest::testActiveTabIsFilledWithTabColor()
{
    const QColor navy(0x1a, 0x23, 0x7e);
    const FullTabColors colors = fullTabColors(navy, true, QColor(Qt::white));

    QCOMPARE(colors.fill, navy);
    QCOMPARE(colors.text, QColor(Qt::white));
}

void TabColorStyleTest::testLightTabColorGetsDarkText()
{
    const QColor yellow(0xfd, 0xbc, 0x4b);
    const FullTabColors colors = fullTabColors(yellow, true, QColor(0x20, 0x20, 0x20));

    QCOMPARE(colors.fill, yellow);
    QCOMPARE(colors.text, QColor(Qt::black));
}

void TabColorStyleTest::testInactiveTabIsMutedTowardsBackground_data()
{
    QTest::addColumn<QColor>("tabColor");
    QTest::addColumn<QColor>("background");

    QTest::newRow("red on light theme") << QColor(0xda, 0x44, 0x53) << QColor(0xef, 0xf0, 0xf1);
    QTest::newRow("red on dark theme") << QColor(0xda, 0x44, 0x53) << QColor(0x20, 0x23, 0x26);
    QTest::newRow("yellow on dark theme") << QColor(0xfd, 0xbc, 0x4b) << QColor(0x20, 0x23, 0x26);
}

void TabColorStyleTest::testInactiveTabIsMutedTowardsBackground()
{
    QFETCH(QColor, tabColor);
    QFETCH(QColor, background);

    const FullTabColors colors = fullTabColors(tabColor, false, background);

    // The fill sits strictly between the Tab Color and the background
    QVERIFY(colors.fill != tabColor);
    QVERIFY(colors.fill != background);
    QVERIFY(qMin(tabColor.red(), background.red()) <= colors.fill.red() && colors.fill.red() <= qMax(tabColor.red(), background.red()));
    QVERIFY(qMin(tabColor.green(), background.green()) <= colors.fill.green() && colors.fill.green() <= qMax(tabColor.green(), background.green()));
    QVERIFY(qMin(tabColor.blue(), background.blue()) <= colors.fill.blue() && colors.fill.blue() <= qMax(tabColor.blue(), background.blue()));

    // WCAG AA contrast for normal text
    QVERIFY(KColorUtils::contrastRatio(colors.text, colors.fill) >= 4.5);
}

QTEST_GUILESS_MAIN(Konsole::TabColorStyleTest)

#include "TabColorStyleTest.moc"
