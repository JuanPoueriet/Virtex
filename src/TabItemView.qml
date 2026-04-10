import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import ChromeTabsQt

Rectangle {
    property TabSession session

    color: "#f8f9fa"

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 20
        spacing: 15

        Label {
            text: "Tab Content - " + (session ? session.title : "")
            font.pixelSize: 18
            font.bold: true
            color: "#3c4043"
        }

        ScrollView {
            Layout.fillWidth: true
            Layout.fillHeight: true

            TextArea {
                text: session && session.view ? session.view.content : ""
                placeholderText: "Type something here..."
                font.pixelSize: 14
                wrapMode: TextArea.Wrap
                background: Rectangle {
                    color: "white"
                    border.color: "#dadce0"
                    radius: 8
                }
                onTextChanged: {
                    if (session && session.view) {
                        session.view.content = text
                    }
                }
            }
        }

        RowLayout {
            Layout.fillWidth: true
            Label {
                text: "Status: " + (session && session.isDirty ? "Modified" : "Saved")
                font.pixelSize: 12
                color: "#70757a"
            }
        }
    }
}
