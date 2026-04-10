#include "WindowManager.h"
#include "TabSession.h"
#include <algorithm>
#include <QGuiApplication>
#include <QQuickWindow>
#include <QQmlApplicationEngine>
#include <QDebug>

WindowManager::WindowManager(QObject* parent) : QObject(parent) {
}

WindowManager& WindowManager::instance() {
    static WindowManager manager;
    return manager;
}

void WindowManager::createWindow() {
    QQmlApplicationEngine* engine = qobject_cast<QQmlApplicationEngine*>(QGuiApplication::instance()->property("engine").value<QObject*>());
    if (!engine) return;

    QQmlComponent component(engine, QUrl(u"qrc:/app/src/Main.qml"_qs));
    QObject* obj = component.create();
    QQuickWindow* window = qobject_cast<QQuickWindow*>(obj);
    if (window) {
        m_windows.push_back(window);
        connect(window, &QQuickWindow::closing, this, [this, window]() {
            removeWindow(window);
        });
        window->show();
    } else {
        qWarning() << "Failed to create window:" << component.errorString();
    }
}

void WindowManager::removeWindow(QQuickWindow* window) {
    auto it = std::find(m_windows.begin(), m_windows.end(), window);
    if (it != m_windows.end()) {
        m_windows.erase(it);
    }
    if (m_windows.empty()) {
        QGuiApplication::quit();
    }
}

void WindowManager::moveTab(TabSession* session, QObject* fromContainer, QObject* toContainer, int toIndex) {
    if (!session || !fromContainer || !toContainer) return;

    QMetaObject::invokeMethod(fromContainer, "removeTabBySession", Q_ARG(TabSession*, session));
    QMetaObject::invokeMethod(toContainer, "insertTab", Q_ARG(int, toIndex), Q_ARG(TabSession*, session));
}

void WindowManager::detachTab(TabSession* session, QObject* fromContainer, const QPoint& globalPos) {
    if (!session || !fromContainer) return;

    QMetaObject::invokeMethod(fromContainer, "removeTabBySession", Q_ARG(TabSession*, session));

    QQmlApplicationEngine* engine = qobject_cast<QQmlApplicationEngine*>(QGuiApplication::instance()->property("engine").value<QObject*>());
    if (!engine) return;

    QQmlComponent component(engine, QUrl(u"qrc:/app/src/Main.qml"_qs));
    QObject* obj = component.create();
    QQuickWindow* window = qobject_cast<QQuickWindow*>(obj);
    if (window) {
        m_windows.push_back(window);
        connect(window, &QQuickWindow::closing, this, [this, window]() {
            removeWindow(window);
        });

        window->setX(globalPos.x());
        window->setY(globalPos.y());

        QObject* tabContainer = window->findChild<QObject*>("tabContainer");
        if (tabContainer) {
            QMetaObject::invokeMethod(tabContainer, "addTab", Q_ARG(TabSession*, session));
        }

        window->show();
    }
}

void WindowManager::notifyTabHostEmpty(QQuickWindow* window) {
    if (window) {
        window->close();
    }
}
