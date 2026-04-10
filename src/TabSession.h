#pragma once

#include <QString>
#include <QIcon>
#include <QObject>
#include <QtQml/qqmlregistration.h>
#include "TabView.h"

class TabSession : public QObject {
    Q_OBJECT
    Q_PROPERTY(QString title READ title WRITE setTitle NOTIFY titleChanged)
    Q_PROPERTY(bool isDirty READ isDirty WRITE setDirty NOTIFY dirtyChanged)
    Q_PROPERTY(TabView* view READ view WRITE setView NOTIFY viewChanged)
    QML_ELEMENT

public:
    explicit TabSession(QObject* parent = nullptr);
    TabSession(const QString& title, QObject* parent = nullptr);

    QString title() const { return m_title; }
    void setTitle(const QString& title);

    QIcon icon() const { return m_icon; }
    void setIcon(const QIcon& icon);

    TabView* view() const { return m_view; }
    void setView(TabView* view);

    bool isDirty() const { return m_isDirty; }
    void setDirty(bool dirty);

signals:
    void titleChanged();
    void iconChanged();
    void dirtyChanged();
    void viewChanged();

private:
    QString m_title;
    QIcon m_icon;
    TabView* m_view = nullptr;
    bool m_isDirty = false;
};
