import QtQuick 2.9
import QtQuick.Window 2.3
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.2
import QtQuick.Controls.Material 2.2

ApplicationWindow {
    Item {
        id: toggleLogin
        property bool on: true

        function toggle() {
            on = !on
        }
    }

    Item {
        id: toggleRegister
        property bool on: false

        function toggle() {
            on = !on
        }
    }

    Item {
        id: toggleMainWindow
        property bool on:false

        function toggle() {
            on = !on
        }
    }

    visible: true
    id: mainWindow

    Material.theme: Material.Dark
    Material.accent: Material.DeepOrange

    width: 400
    height: 200

    title: qsTr("Regexp Explainer")

    ColumnLayout {
        id: loginWindowLayout
        x: 10
        y: 10
        width: 380
        height: 180

        GridLayout {
            columns: 2
            rowSpacing: 10
            Label {
                text: qsTr("Login")
            }

            TextField {
                id: login
                Layout.fillWidth: true
                Layout.rightMargin: 25
            }

            Label {
                text: qsTr("Password")
            }

            TextField {
                id: password
                Layout.fillWidth: true
                Layout.rightMargin: 25
                echoMode: TextInput.Password
            }
            columnSpacing: 10
            anchors.margins: 10
            anchors.fill: parent
        }

        RowLayout {
            width: 100
            height: 100
            spacing: 25
            Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter

            Button {
                id: loginButton
                text: qsTr("Login")
                onClicked: {
                    toggleLogin.toggle();
                    toggleMainWindow.toggle();
                }
            }

            Button {
                id: registerButton
                text: qsTr("Register")
                onClicked: {
                    toggleLogin.toggle();
                    toggleRegister.toggle();
                }
            }
        }

        states: [
            State {
                name: "enabled"
                when: toggleLogin.on
                PropertyChanges {
                    target: loginWindowLayout; visible: true
                }
            },
            State {
                name: "disabled"
                when: !toggleLogin.on
                PropertyChanges {
                    target: loginWindowLayout; visible: false
                }
            }
        ]
    }

    ColumnLayout {
        visible: false
        id: registerWidgetLayout
        x: 10
        y: 10
        width: 380
        height: 180

        GridLayout {
            columns: 2

            height: 100

            Label {
                text: qsTr("Username")
            }
            TextField {
                id: usernameField
                Layout.fillWidth: true
                Layout.rightMargin: 25
            }

            Label {
                text: qsTr("Email")
            }
            TextField {
                id: emailField
                Layout.fillWidth: true
                Layout.rightMargin: 25
            }

            Label {
                text: qsTr("Password")
            }
            TextField {
                id: passwordField
                Layout.fillWidth: true
                Layout.rightMargin: 25
                echoMode: TextField.Password
            }
        }

        RowLayout {
            width: 100
            height: 80
            spacing: 25
            Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter

            Button {
                id: registerButton_2
                text: qsTr("Register")
                onClicked: {
                    toggleLogin.toggle();
                    toggleRegister.toggle();
                }
            }

            Button {
                id: backButton
                text: qsTr("Back")
                onClicked: {
                    toggleLogin.toggle();
                    toggleRegister.toggle();
                }
            }
        }

        states: [
            State {
                name: "enable"
                when: toggleRegister.on
                PropertyChanges {
                    target: registerWidgetLayout; visible: true
                }
            },
            State {
                name: "disable"
                when: !toggleRegister.on
                PropertyChanges {
                    target: registerWidgetLayout; visible: false
                }
            }
        ]
    }

    RowLayout {
        id: mainLayout
        x: 10
        y: 5
        width: 600
        height: 75
        visible: false

        TextField {
            id: searchField
            Layout.fillWidth: true
            Layout.rightMargin: 15
        }
        Button {
            id: searchSubmitButton
            text: qsTr("Search")
            Layout.rightMargin: 25
        }

        states: [
            State {
                name: "enable"
                when: toggleMainWindow.on
                PropertyChanges {
                    target: mainWindow; width: 800; height: 400; x: Screen.width / 2 - width / 2; y: Screen.height / 2 - height / 2;
                }
                PropertyChanges {
                    target: mainLayout; visible: true
                }
            },
            State {
                name: "disable"
                when: !toggleMainWindow.on
                PropertyChanges {
                    target: mainLayout; visible: false
                }
            }
        ]
    }
    ColumnLayout {
        id: mainListLayout
        x: 10
        y: 80
        width: 600
        height: 300
        visible: false

        states: [
            State {
                name: "enable"
                when: toggleMainWindow.on
                PropertyChanges {
                    target: mainListLayout; visible: true

                }
            },
            State {
                name: "disable"
                when: !toggleMainWindow.on
                PropertyChanges {
                    target: mainListLayout; visible: false

                }
            }
        ]
    }
}
