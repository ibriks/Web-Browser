#include "browserwindow.h"
#include "browser.h"
#include "webengineview.h"
#include "ui_browserwindow.h"
#include <QtWebEngineWidgets>
#include <QWebEnginePage>
#include <QWebEngineView>
#include <QContextMenuEvent>
#include <QFile>
#include <QTextStream>
#include <QUrl>
#include <QMenu>
#include <QMenuBar>
#include <QAction>
#include <QTabBar>
#include <QCloseEvent>

WebEngineView* BrowserWindow::currentTab()
{
    return qobject_cast<WebEngineView*>(tabWidget->widget(tabWidget->currentIndex()));
}

BrowserWindow::BrowserWindow(Browser *browser, QWebEngineProfile *profile)
    : QMainWindow(),
      Ui::BrowserWindow(),
      m_browser(browser),
      m_profile(profile)
{
    setupUi(this);

    QPalette pal = palette();
    pal.setColor(QPalette::Background, Qt::lightGray);
    this->setAutoFillBackground(true);
    this->setPalette(pal);

    QFile inputFile(":/bookmarks.txt");
    if (inputFile.open(QIODevice::ReadOnly | QIODevice::Text))
    {
       QTextStream in(&inputFile);
       while (!in.atEnd())
       {
          QString line = in.readLine();
          WebEngineView *bookmark = new WebEngineView(this);
          QWebEnginePage *page = new QWebEnginePage(m_profile, bookmark);
          bookmark->setPage(page);
          bookmark->load(QUrl(line));
          connect(bookmark, &WebEngineView::iconChanged, this, &BrowserWindow::handleBookmarks);
       }
       inputFile.close();
    }

    QMenu *menu = new QMenu(tr("Window") );
    menu->addAction(tr("New Window"), this, &BrowserWindow::handleNewWindowTriggered, QKeySequence::Open);
    menu->addAction(tr("New Incognito Window"), this, &BrowserWindow::handleNewIncognitoWindowTriggered);
    menu->addAction(tr("New Tab"), this, &BrowserWindow::handleNewTab, QKeySequence::New);
    menuBar()->addMenu(menu);

    QTabBar *tabBar = tabWidget->tabBar();
    tabBar->setTabsClosable(true);
    tabBar->setSelectionBehaviorOnRemove(QTabBar::SelectPreviousTab);
    tabBar->setMovable(true);

    connect(tabBar, &QTabBar::tabBarDoubleClicked, [this](int index) {
        if (index == -1)
            this->handleNewTab();
    });

    tabWidget->setDocumentMode(true);
    tabWidget->setElideMode(Qt::ElideRight);
    pal.setColor(QPalette::Background, Qt::darkCyan);
    tabWidget->setAutoFillBackground(true);
    tabWidget->setPalette(pal);

    connect(goButton, &QPushButton::clicked, this, &BrowserWindow::handleUrlClicked);
    connect(urlEdit, &QLineEdit::returnPressed, this, &BrowserWindow::handleUrlClicked);

    connect(backButton, &QPushButton::clicked, this, &BrowserWindow::handleUrlBack);
    connect(refreshButton, &QPushButton::clicked, this, &BrowserWindow::handleUrlRefresh);
    connect(forwardButton, &QPushButton::clicked, this, &BrowserWindow::handleUrlForward);
    connect(tabButton, &QPushButton::clicked, this, &BrowserWindow::handleNewTab);

    connect(tabWidget->tabBar(), &QTabBar::tabCloseRequested, [=](int index) {
        if (tabWidget->count() == 1) {
            this->handleNewTab();
            tabWidget->widget(index)->deleteLater();
        }
        else {
            tabWidget->widget(index)->deleteLater();
        }
    });

    connect(tabWidget, &QTabWidget::currentChanged, tabWidget, &QTabWidget::setCurrentIndex);
    connect(tabWidget, &QTabWidget::currentChanged, this , &BrowserWindow::handleUrlChanged);
}

void BrowserWindow::handleBookmarks()
{
    WebEngineView *bookmark = qobject_cast<WebEngineView *>(sender());
    QPushButton* link = new QPushButton(bookmark->icon(), bookmark->title());
    Bookmarks->layout()->addWidget(link);

    connect(link, &QPushButton::clicked,     [=](){
        WebEngineView *m_webview = new WebEngineView(this);
        QWebEnginePage *page = new QWebEnginePage(m_profile, m_webview);
        m_webview->setPage(page);

        urlEdit->setText(bookmark->url().toString());
        m_webview->setUrl(bookmark->url());

        connect(m_webview, &WebEngineView::titleChanged, [this](const QString &title) {
            int index = tabWidget->currentIndex();
            tabWidget->tabBar()->setTabText(index, title);
            if (this->windowTitle() != "Incognito") this->setWindowTitle(title);
        });
        connect(m_webview, &WebEngineView::iconChanged, [this](const QIcon &icon) {
            int index = tabWidget->currentIndex();
            tabWidget->tabBar()->setTabIcon(index, icon);
        });

        tabWidget->addTab(m_webview, m_webview->title());
        tabWidget->setCurrentIndex(tabWidget->count()-1);
        connect(currentTab(), &WebEngineView::urlChanged, this, &BrowserWindow::handleUrlChanged);
    });
}

void BrowserWindow::handleNewTab()
{
    WebEngineView *m_webview = new WebEngineView(this);
    QWebEnginePage *page = new QWebEnginePage(m_profile, m_webview);
    m_webview->setPage(page);

    urlEdit->setText("https://www.google.com");
    QUrl url;
    url.setUrl(urlEdit->text());
    m_webview->setUrl(url);

    connect(m_webview, &WebEngineView::titleChanged, [this](const QString &title) {
        int index = tabWidget->currentIndex();
        tabWidget->tabBar()->setTabText(index, title);
        if (this->windowTitle() != "Incognito") this->setWindowTitle(title);
    });
    connect(m_webview, &WebEngineView::iconChanged, [this](const QIcon &icon) {
        int index = tabWidget->currentIndex();
        tabWidget->tabBar()->setTabIcon(index, icon);
    });

    tabWidget->addTab(m_webview, m_webview->title());
    tabWidget->setCurrentIndex(tabWidget->count()-1);
    connect(currentTab(), &WebEngineView::urlChanged, this, &BrowserWindow::handleUrlChanged);
}

void BrowserWindow::openInNewTab()
{
    QAction *act = qobject_cast<QAction *>(sender());
    QVariant var = act->data();
    QUrl url = var.value<QUrl>();

    WebEngineView *m_webview = new WebEngineView(this);
    QWebEnginePage *page = new QWebEnginePage(m_profile, m_webview);
    m_webview->setPage(page);

    urlEdit->setText(url.toString());
    m_webview->setUrl(url);

    connect(m_webview, &WebEngineView::titleChanged, [this](const QString &title) {
        int index = tabWidget->currentIndex();
        tabWidget->tabBar()->setTabText(index, title);
        if (this->windowTitle() != "Incognito") this->setWindowTitle(title);
    });
    connect(m_webview, &WebEngineView::iconChanged, [this](const QIcon &icon) {
        int index = tabWidget->currentIndex();
        tabWidget->tabBar()->setTabIcon(index, icon);
    });

    tabWidget->addTab(m_webview, m_webview->title());
    tabWidget->setCurrentIndex(tabWidget->count()-1);
    connect(currentTab(), &WebEngineView::urlChanged, this, &BrowserWindow::handleUrlChanged);
}

void BrowserWindow::openInNewWindow()
{
    QAction *act = qobject_cast<QAction *>(sender());
    QVariant var = act->data();
    QUrl url = var.value<QUrl>();

    BrowserWindow *window = m_browser->createWindow();
    window->show();

    WebEngineView *new_webview = new WebEngineView(window);
    QWebEnginePage *page = new QWebEnginePage(window->m_profile, new_webview);
    new_webview->setPage(page);

    window->urlEdit->setText(url.toString());
    new_webview->setUrl(url);

    connect(new_webview, &WebEngineView::titleChanged, [window](const QString &title) {
        int index = window->tabWidget->currentIndex();
        window->tabWidget->tabBar()->setTabText(index, title);
        if (window->windowTitle() != "Incognito") window->setWindowTitle(title);

    });
    connect(new_webview, &WebEngineView::iconChanged, [window](const QIcon &icon) {
        int index = window->tabWidget->currentIndex();
        window->tabWidget->tabBar()->setTabIcon(index, icon);
    });

    window->tabWidget->addTab(new_webview, new_webview->title());
    window->tabWidget->setCurrentIndex(window->tabWidget->count()-1);
    connect(window->currentTab(), &WebEngineView::urlChanged, window, &BrowserWindow::handleUrlChanged);
}

void BrowserWindow::handleUrlClicked()
{
    QUrl url = QUrl(urlEdit->text());
    url.setScheme("https");
    currentTab()->setUrl(url);
}

void BrowserWindow::handleUrlBack()
{
    currentTab()->back();
}

void BrowserWindow::handleUrlRefresh()
{
    currentTab()->reload();
}

void BrowserWindow::handleUrlForward()
{
    currentTab()->forward();
}

void BrowserWindow::handleUrlChanged()
{
    urlEdit->setText(currentTab()->url().toString());
    if (this->windowTitle() != "Incognito") this->setWindowTitle(currentTab()->title());
}

void BrowserWindow::handleNewWindowTriggered()
{
    BrowserWindow *window = m_browser->createWindow(false);
    window->handleNewTab();
}

void BrowserWindow::handleNewIncognitoWindowTriggered()
{
    BrowserWindow *window = m_browser->createWindow(true);
    window->handleNewTab();
}

void BrowserWindow::closeEvent(QCloseEvent *event)
{
    this->deleteLater();
    event->accept();
}


