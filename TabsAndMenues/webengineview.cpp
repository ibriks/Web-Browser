#include "browserwindow.h"
#include "webengineview.h"
#include <QWebEnginePage>
#include <QWebEngineView>
#include <QContextMenuEvent>
#include <QWebEngineContextMenuData>
#include <QVariant>
#include <QUrl>
#include <QMenu>
#include <QAction>
#include <QTabBar>

WebEngineView::WebEngineView(BrowserWindow *win)
{
    w = win;
}

void WebEngineView::contextMenuEvent(QContextMenuEvent *event)
{
    QMenu *menu = page()->createStandardContextMenu();
    const QList<QAction*> actions = menu->actions();

    auto it = std::find(actions.cbegin(), actions.cend(), page()->action(QWebEnginePage::OpenLinkInThisWindow));
    if (it != actions.cend()) {
        (*it)->setText(tr("Open Link in This Tab"));
        ++it;
        QAction *before(it == actions.cend() ? nullptr : *it);
        QAction *inNewTab;
        inNewTab = new QAction("Open Link in New Tab", this);
        inNewTab->setData(QVariant(page()->contextMenuData().linkUrl()));
        menu->insertAction(before, inNewTab);
        connect(inNewTab, &QAction::triggered, w, &BrowserWindow::openInNewTab);
        QAction *inNewWindow;
        inNewWindow = new QAction("Open Link in New Window", this);
        inNewWindow->setData(QVariant(page()->contextMenuData().linkUrl()));
        menu->insertAction(before, inNewWindow);
        connect(inNewWindow, &QAction::triggered, w, &BrowserWindow::openInNewWindow);
    }

    QAction *addTab;
    addTab = new QAction("Add Tab", this);
    menu->addAction(addTab);
    connect(addTab, &QAction::triggered, w, &BrowserWindow::handleNewTab);

    menu->removeAction(page()->action(QWebEnginePage::DownloadLinkToDisk));
    menu->removeAction(page()->action(QWebEnginePage::DownloadImageToDisk));

    menu->popup(event->globalPos());
}

