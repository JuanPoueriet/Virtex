#include "TabView.h"

TabView::TabView(QObject* parent) : QObject(parent) {
}

void TabView::setContent(const QString& content) {
    if (m_content != content) {
        m_content = content;
        emit contentChanged();
    }
}
