#ifndef WEBENGINEVIEW_H
#define WEBENGINEVIEW_H

#include "browserwindow.h"
#include <QMainWindow>
#include <QWebEngineView>
#include <QWebEnginePage>
#include "ui_browserwindow.h"

class BrowserWindow;

class WebEngineView : public QWebEngineView
{
    Q_OBJECT

private:
    BrowserWindow *w;

public:
    WebEngineView(BrowserWindow *w);

protected:
    void contextMenuEvent(QContextMenuEvent *event) override;
};

#endif // WEBENGINEVIEW_H
