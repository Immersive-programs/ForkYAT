/**************************************************************************************************
* Copyright (c) 2012 Jørgen Lind
*
* Permission is hereby granted, free of charge, to any person obtaining a copy of this software and
* associated documentation files (the "Software"), to deal in the Software without restriction,
* including without limitation the rights to use, copy, modify, merge, publish, distribute,
* sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in all copies or
* substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT
* NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
* NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
* DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT
* OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*
***************************************************************************************************/

#include "terminal_item.h"

TerminalItem::TerminalItem(QQuickItem *parent)
    : QQuickItem(parent)
    , m_state(new TerminalState)
{
    connect(m_state->screen(), &TerminalScreen::geometryChanged,
            this, &TerminalItem::screenGeometryChanged);

    connect(this,&TerminalItem::widthChanged,
            this, &TerminalItem::onWidthChanged);
    connect(this, &TerminalItem::heightChanged,
            this, &TerminalItem::onHeightChanged);
}

TerminalScreen *TerminalItem::terminalScreen() const
{
    return m_state->screen();
}

TerminalState *TerminalItem::terminalState() const
{
    return m_state;
}


void TerminalItem::onWidthChanged()
{
    m_state->setWidth(width());
}

void TerminalItem::onHeightChanged()
{
    m_state->setHeight(height());
}
