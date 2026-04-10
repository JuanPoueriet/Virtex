#pragma once

#include <QObject>
#include <QString>
#include <QtQml/qqmlregistration.h>

class TabView : public QObject {
    Q_OBJECT
    Q_PROPERTY(QString content READ content WRITE setContent NOTIFY contentChanged)
    QML_ELEMENT
public:
    explicit TabView(QObject* parent = nullptr);

    QString content() const { return m_content; }
    void setContent(const QString& content);

signals:
    void contentChanged();

private:
    QString m_content;
};
