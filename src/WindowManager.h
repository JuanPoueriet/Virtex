#pragma once

#include <QObject>
#include <vector>
#include <QPoint>
#include <QtQml/qqmlregistration.h>

class QQuickWindow;
class TabSession;

class WindowManager : public QObject {
    Q_OBJECT
    QML_ELEMENT
    QML_SINGLETON
public:
    static WindowManager& instance();

    Q_INVOKABLE void createWindow();
    void removeWindow(QQuickWindow* window);

    Q_INVOKABLE void moveTab(TabSession* session, QObject* fromContainer, QObject* toContainer, int toIndex = -1);
    Q_INVOKABLE void detachTab(TabSession* session, QObject* fromContainer, const QPoint& globalPos);

    void notifyTabHostEmpty(QQuickWindow* window);

private:
    WindowManager(QObject* parent = nullptr);
    std::vector<QQuickWindow*> m_windows;
};
