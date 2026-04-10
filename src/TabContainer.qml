import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import ChromeTabsQt

Item {
    id: container

    property list<TabSession> sessions
    property int currentIndex: 0

    function addTab(session) {
        sessions.push(session)
        currentIndex = sessions.length - 1
    }

    function insertTab(index, session) {
        if (index < 0 || index > sessions.length) index = sessions.length
        sessions.splice(index, 0, session)
        currentIndex = index
    }

    function removeTab(index) {
        if (index >= 0 && index < sessions.length) {
            sessions.splice(index, 1)
            if (currentIndex >= sessions.length) currentIndex = sessions.length - 1
            if (sessions.length === 0) {
                WindowManager.notifyTabHostEmpty(root)
            }
        }
    }

    function removeTabBySession(session) {
        for (let i = 0; i < sessions.length; i++) {
            if (sessions[i] === session) {
                removeTab(i)
                return
            }
        }
    }

    ColumnLayout {
        anchors.fill: parent
        spacing: 0

        // Tab Bar
        Rectangle {
            Layout.fillWidth: true
            Layout.preferredHeight: 40
            color: "#dee1e6"

            Row {
                id: tabRow
                anchors.fill: parent
                spacing: 1

                Repeater {
                    model: container.sessions
                    delegate: Rectangle {
                        width: 150
                        height: 40
                        color: index === container.currentIndex ? "#ffffff" : "#dee1e6"
                        border.color: "#bdc1c6"
                        border.width: 1

                        Text {
                            anchors.centerIn: parent
                            text: modelData.title
                            font.pixelSize: 13
                        }

                        MouseArea {
                            anchors.fill: parent
                            onClicked: container.currentIndex = index

                            drag.target: dragDummy

                            property bool dragActive: false

                            onPressed: {
                                dragActive = false
                            }

                            onPositionChanged: {
                                if (!dragActive && drag.active) {
                                    dragActive = true
                                }
                            }

                            onReleased: {
                                if (dragActive) {
                                    let globalPos = mapToGlobal(mouseX, mouseY)
                                    if (mouseY < -50 || mouseY > height + 50) {
                                        WindowManager.detachTab(modelData, container, globalPos)
                                    }
                                }
                            }
                        }

                        Item { id: dragDummy }

                        Button {
                            anchors.right: parent.right
                            anchors.verticalCenter: parent.verticalCenter
                            anchors.rightMargin: 5
                            width: 16
                            height: 16
                            text: "x"
                            onClicked: container.removeTab(index)
                        }
                    }
                }

                Button {
                    text: "+"
                    width: 30
                    height: 30
                    Layout.alignment: Qt.AlignVCenter
                    onClicked: {
                        let session = Qt.createQmlObject('import ChromeTabsQt; TabSession { title: "New Tab" }', container)
                        let view = Qt.createQmlObject('import ChromeTabsQt; TabView { content: "New Content" }', session)
                        session.view = view
                        container.addTab(session)
                    }
                }
            }
        }

        // Content Area
        StackLayout {
            Layout.fillWidth: true
            Layout.fillHeight: true
            currentIndex: container.currentIndex

            Repeater {
                model: container.sessions
                delegate: TabItemView {
                    session: modelData
                }
            }
        }
    }
}
