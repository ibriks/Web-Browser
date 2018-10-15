#ifndef BROWSERWINDOW_H
#define BROWSERWINDOW_H

#include <QMainWindow>
#include <QWebEngineView>
#include <QWebEnginePage>
#include <QWebEngineProfile>
#include "ui_browserwindow.h"

class Browser;
class WebEngineView;

class BrowserWindow : public QMainWindow, public Ui::BrowserWindow
{
    Q_OBJECT

public:
    BrowserWindow(Browser *browser, QWebEngineProfile *profile);
    WebEngineView* currentTab();

protected:
    void closeEvent(QCloseEvent *event) override;

public slots:
    void handleUrlClicked();
    void handleUrlBack();
    void handleUrlRefresh();
    void handleUrlForward();
    void handleUrlChanged();
    void handleNewTab();
    void openInNewTab();
    void openInNewWindow();
    void handleBookmarks();
    void handleNewWindowTriggered();
    void handleNewIncognitoWindowTriggered();

private:
    Browser *m_browser;
    QWebEngineProfile *m_profile;
};

#endif // BROWSERWINDOW_H
