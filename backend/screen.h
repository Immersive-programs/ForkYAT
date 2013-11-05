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

#ifndef TERMINALSCREEN_H
#define TERMINALSCREEN_H

#include <QObject>

#include "color_palette.h"
#include "parser.h"
#include "yat_pty.h"
#include "text_style.h"

#include <QtCore/QPoint>
#include <QtCore/QSize>
#include <QtCore/QStack>
#include <QtCore/QElapsedTimer>

class Block;
class Cursor;
class Text;
class ScreenData;

class Screen : public QObject
{
    Q_OBJECT

    Q_PROPERTY(int height READ height WRITE setHeight NOTIFY heightChanged)
    Q_PROPERTY(int width READ width WRITE setWidth NOTIFY widthChanged)
    Q_PROPERTY(int contentHeight READ contentHeight NOTIFY contentHeightChanged)
    Q_PROPERTY(QString title READ title WRITE setTitle NOTIFY screenTitleChanged)
    Q_PROPERTY(bool selectionEnabled READ selectionEnabled NOTIFY selectionEnabledChanged)
    Q_PROPERTY(QPointF selectionAreaStart READ selectionAreaStart WRITE setSelectionAreaStart NOTIFY selectionAreaStartChanged)
    Q_PROPERTY(QPointF selectionAreaEnd READ selectionAreaEnd WRITE setSelectionAreaEnd NOTIFY selectionAreaEndChanged)
    Q_PROPERTY(QColor defaultBackgroundColor READ defaultBackgroundColor NOTIFY defaultBackgroundColorChanged)

public:
    explicit Screen(QObject *parent = 0);
    ~Screen();

    void emitRequestHeight(int newHeight);
    void setHeight(int height);
    int height() const;
    int contentHeight() const;

    void emitRequestWidth(int newWidth);
    void setWidth(int width);
    int width() const;

    ScreenData *currentScreenData() const { return m_current_data; }
    void useAlternateScreenBuffer();
    void useNormalScreenBuffer();

    Cursor *currentCursor() const { return  m_cursor_stack.last(); }
    void saveCursor();
    void restoreCursor();

    TextStyle defaultTextStyle() const;

    QColor defaultForgroundColor() const;
    QColor defaultBackgroundColor() const;

    ColorPalette *colorPalette() const;

    void clearScreen();

    void fill(const QChar character);
    void clear();

    void setFastScroll(bool fast);
    bool fastScroll() const;

    QPointF selectionAreaStart() const;
    void setSelectionAreaStart(const QPointF &start);
    QPointF selectionAreaEnd() const;
    void setSelectionAreaEnd(const QPointF &end);

    bool selectionEnabled() const;

    Q_INVOKABLE void setSelectionEnabled(bool enabled);
    Q_INVOKABLE void sendSelectionToClipboard() const;
    Q_INVOKABLE void sendSelectionToSelection() const;
    Q_INVOKABLE void pasteFromSelection();
    Q_INVOKABLE void pasteFromClipboard();

    Q_INVOKABLE void doubleClicked(const QPointF &clicked);

    void setTitle(const QString &title);
    QString title() const;

    void scheduleFlash();

    Q_INVOKABLE void printScreen() const;

    void scheduleEventDispatch();
    void dispatchChanges();

    void sendPrimaryDA();
    void sendSecondaryDA();

    void setApplicationCursorKeysMode(bool enable);
    bool applicationCursorKeyMode() const;

    Q_INVOKABLE void sendKey(const QString &text, Qt::Key key, Qt::KeyboardModifiers modifiers);

    YatPty *pty();

    Q_INVOKABLE void ensureVisiblePages(int top_line);
    Text *createTextSegment(const TextStyleLine &style_line);
    void releaseTextSegment(Text *text);

public slots:
    void readData(const QByteArray &data);
    void paletteChanged();

signals:
    void reset();

    void flash();

    void dispatchLineChanges();
    void dispatchTextSegmentChanges();

    void selectionAreaStartChanged();
    void selectionAreaEndChanged();
    void selectionEnabledChanged();

    void screenTitleChanged();

    void textCreated(Text *text);
    void cursorCreated(Cursor *cursor);

    void requestHeightChange(int newHeight);
    void heightAboutToChange(int height, int currentCursorLine, int currentScrollBackHeight);
    void heightChanged();
    void contentHeightChanged();

    void requestWidthChange(int newWidth);
    void widthAboutToChange(int width);
    void widthChanged();

    void defaultBackgroundColorChanged();
protected:
    void timerEvent(QTimerEvent *);

private:
    void setSelectionValidity();

    ColorPalette *m_palette;
    YatPty m_pty;
    Parser m_parser;
    QElapsedTimer m_time_since_parsed;
    QElapsedTimer m_time_since_initiated;

    int m_timer_event_id;
    int m_width;
    int m_height;

    ScreenData *m_primary_data;
    ScreenData *m_alternate_data;
    ScreenData *m_current_data;
    ScreenData *m_old_current_data;

    QVector<Cursor *> m_cursor_stack;
    QVector<Cursor *> m_new_cursors;
    QVector<Cursor *> m_delete_cursors;

    QString m_title;

    bool m_selection_valid;
    bool m_selection_moved;
    QPointF m_selection_start;
    QPointF m_selection_end;

    bool m_flash;
    bool m_cursor_changed;
    bool m_application_cursor_key_mode;
    bool m_fast_scroll;

    QVector<Text *> m_to_delete;

    QColor m_default_background;

    friend class ScreenData;
};

#endif // TERMINALSCREEN_H
