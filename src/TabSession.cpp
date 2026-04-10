#include "TabSession.h"
#include "TabView.h"

TabSession::TabSession(QObject* parent)
    : QObject(parent) {
}

TabSession::TabSession(const QString& title, QObject* parent)
    : QObject(parent), m_title(title) {
}

void TabSession::setTitle(const QString& title) {
    if (m_title != title) {
        m_title = title;
        emit titleChanged();
    }
}

void TabSession::setIcon(const QIcon& icon) {
    m_icon = icon;
    emit iconChanged();
}

void TabSession::setView(TabView* view) {
    if (m_view != view) {
        m_view = view;
        emit viewChanged();
    }
}

void TabSession::setDirty(bool dirty) {
    if (m_isDirty != dirty) {
        m_isDirty = dirty;
        emit dirtyChanged();
    }
}
