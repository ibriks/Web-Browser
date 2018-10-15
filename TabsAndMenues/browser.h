#ifndef BROWSER_H
#define BROWSER_H

#include <QVector>
#include <QWebEngineProfile>

class BrowserWindow;

class Browser
{
public:
    Browser();
    BrowserWindow *createWindow(bool offTheRecord = false);

private:
    QVector<BrowserWindow*> m_windows;
    QWebEngineProfile m_otrProfile;
};

#endif // BROWSER_H
