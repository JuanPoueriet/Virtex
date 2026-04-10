import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import ChromeTabsQt

ApplicationWindow {
    id: root
    visible: true
    width: 800
    height: 600
    title: "ChromeTabs QML"

    TabContainer {
        id: tabContainer
        objectName: "tabContainer"
        anchors.fill: parent
    }
}
