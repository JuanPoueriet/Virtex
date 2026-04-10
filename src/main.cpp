#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include "WindowManager.h"
#include "TabSession.h"
#include "TabView.h"

int main(int argc, char *argv[]) {
    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;

    // Register types
    qmlRegisterType<TabSession>("ChromeTabsQt", 1, 0, "TabSession");
    qmlRegisterType<TabView>("ChromeTabsQt", 1, 0, "TabView");
    qmlRegisterSingletonInstance("ChromeTabsQt", 1, 0, "WindowManager", &WindowManager::instance());

    // Expose engine to WindowManager
    app.setProperty("engine", QVariant::fromValue<QObject*>(&engine));

    const QUrl url(u"qrc:/app/src/Main.qml"_qs);
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);

    // Initial data setup
    auto& wm = WindowManager::instance();

    // We create the first window through QML engine
    engine.load(url);

    // Setup some initial tabs in the first window
    if (!engine.rootObjects().isEmpty()) {
        QObject* root = engine.rootObjects().first();
        QObject* tabContainer = root->findChild<QObject*>("tabContainer");
        if (tabContainer) {
            for (int i = 1; i <= 3; ++i) {
                TabSession* session = new TabSession(QString("Tab %1").arg(i), tabContainer);
                TabView* view = new TabView(session);
                view->setContent(QString("This is the content of tab %1.\nNow in beautiful QML!").arg(i));
                session->setView(view);
                QMetaObject::invokeMethod(tabContainer, "addTab", Q_ARG(TabSession*, session));
            }
        }
    }

    return app.exec();
}
