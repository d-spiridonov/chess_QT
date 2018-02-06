import QtQuick 2.4
import QtQuick.Controls 1.3
import QtQuick.Layouts 1.1
import QtQuick.Dialogs 1.2

ApplicationWindow {
    id: mainWindow
    title: qsTr("Chess")
    visible: true
    minimumWidth: logic.boardSize * squareSize + squareSize * 4
    minimumHeight: logic.boardSize * squareSize

    
    property int squareSize: 70

    property var images: [
        {'imgPath' : "/images/black_pawn.svg"},
        {'imgPath' : "/images/white_pawn.svg"},
        {'imgPath' : "/images/black_rook.svg"},
        {'imgPath' : "/images/white_rook.svg"},
        {'imgPath' : "/images/black_knight.svg"},
        {'imgPath' : "/images/white_knight.svg"},
        {'imgPath' : "/images/black_bishop.svg"},
        {'imgPath' : "/images/white_bishop.svg"},
        {'imgPath' : "/images/black_queen.svg"},
        {'imgPath' : "/images/white_queen.svg"},
        {'imgPath' : "/images/black_king.svg"},
        {'imgPath' : "/images/white_king.svg"},
    ]
    color: "#ffd9a9"

    Item {
        id: gameBoard
        x: 0
        y: 0
        width : logic.boardSize * squareSize
        height: logic.boardSize * squareSize

        Image {
            source: "/images/chess_board.jpg"
            height: gameBoard.height
            width: gameBoard.width
        }
    }

    Button {
        id: startButton
        y: 7
        anchors.left: gameBoard.right
        anchors.right: parent.right
        anchors.leftMargin: 10
        anchors.rightMargin: 10

        text: "Start"

        onClicked: {
            logic.clear();
            logic.newGame();
            screen2.show();
            mainWindow.hide();
        }
    }

    Button {
        id: loadGame
        x: 10
        y: 35
        text: "Load game"
        anchors.leftMargin: 10
        anchors.right: parent.right
        anchors.left: gameBoard.right
        anchors.rightMargin: 10
        enabled: false

        onClicked: {
            logic.loadFile();
            screen3.show();
            mainWindow.hide();
        }
    }
    ApplicationWindow {
        id: screen2
        title: qsTr("Chess")
        visible: false
        minimumWidth: logic.boardSize * squareSize + squareSize * 4
        minimumHeight: logic.boardSize * squareSize
        color: "#ffd9a9"

        property int squareSize: 70

        Item {
            id: gameBoard2
            x: 0
            y: 0
            width : logic.boardSize * squareSize
            height: logic.boardSize * squareSize

            Image {
                source: "/images/chess_board.jpg"
                height: gameBoard.height
                width: gameBoard.width
                Loader {sourceComponent: chessPlacement}
            }

            Component{
                id: chessPlacement
                Item{
                    Repeater {
                        model: logic
                        Image {
                            height: squareSize
                            width : squareSize

                            x: squareSize * positionX
                            y: squareSize * positionY

                            source: images[type].imgPath
                            MouseArea {
                                anchors.fill: parent
                                drag.target: parent

                                property int startX: 0
                                property int startY: 0

                                onPressed: {
                                    startX = parent.x;
                                    startY = parent.y;
                                }

                                onReleased: {
                                    var fromX = (startX / squareSize);
                                    var fromY = (startY / squareSize);
                                    var toX   = ((parent.x + mouseX) / squareSize);
                                    var toY   = ((parent.y + mouseY) / squareSize);
                                    white_turn.visible = logic.getCurrentTurn() === true ? true : false
                                    black_turn.visible = logic.getCurrentTurn() === false ? true : false
                                    if (!logic.move(fromX, fromY, toX, toY, type)) {
                                        parent.x = startX;
                                        parent.y = startY;
                                        white_turn.visible = logic.getCurrentTurn() === false ? true : false
                                        black_turn.visible = logic.getCurrentTurn() === true ? true : false
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
        Text {
            id: currentTurnStr
            text: "Current turn:"
            color: "black"
            font.pixelSize: 25
            font.bold: true
            anchors.top: stop.bottom
            anchors.left: gameBoard2.right
            anchors.topMargin: 75
            anchors.leftMargin: 70
        }
        Text {
            id: white_turn
            text: "white"
            color: "white"
            font.pixelSize: 25
            font.bold: true
            y: 135
            anchors.left: gameBoard2.right
            anchors.leftMargin: 100
            visible: true
        }
        Text {
            id: black_turn
            text: "black"
            color: "black"
            font.pixelSize: 25
            font.bold: true
            y: 135
            anchors.left: gameBoard2.right
            anchors.leftMargin: 100
            visible: false
        }


        Button {
            id: stop
            y: 7
            anchors.left: gameBoard2.right
            anchors.right: parent.right
            anchors.leftMargin: 10
            anchors.rightMargin: 10
            text: "Stop"
            onClicked: {
                logic.clear();
                mainWindow.show();
                screen2.close();
            }
        }

        Button {
            id: save
            x: 10
            y: 35
            text: "Save"
            anchors.leftMargin: 10
            anchors.right: parent.right
            anchors.left: gameBoard2.right
            anchors.rightMargin: 10

            onClicked: {
                logic.saveFile();
                loadGame.enabled = true;
            }
        }
    }
    ApplicationWindow {
        id: screen3
        title: qsTr("Chess")
        visible: false
        minimumWidth: logic.boardSize * squareSize + squareSize * 4
        minimumHeight: logic.boardSize * squareSize
        color: "#ffd9a9"

        property int squareSize: 70

        Item {
            id: gameBoard3
            x: 0
            y: 0
            width : logic.boardSize * squareSize
            height: logic.boardSize * squareSize

            Image {
                source: "/images/chess_board.jpg"
                height: gameBoard.height
                width: gameBoard.width
                Loader {sourceComponent: chessPlacement2}
            }
        }
        Button {
            id: next
            y: 7
            text: "next"
            anchors.leftMargin: 10
            anchors.right: parent.right
            anchors.left: gameBoard3.right
            anchors.rightMargin: 10

            onClicked: {
                logic.nextMove();
            }
        }

        Button {
            id: prev
            x: 10
            y: 35
            text: "prev"
            anchors.leftMargin: 10
            anchors.right: parent.right
            anchors.left: gameBoard3.right
            anchors.rightMargin: 10

            onClicked: {
                logic.prevMove();
            }
        }

        Button {
            id: load
            x: 10
            y: 63
            text: "load game"
            anchors.leftMargin: 10
            anchors.right: parent.right
            anchors.left: gameBoard3.right
            anchors.rightMargin: 10
            onClicked: {
                logic.clear();
                logic.loadFromHistory();
                screen2.show();
                screen3.hide();
            }
        }

        Component{
            id: chessPlacement2
            Item{
                Repeater {
                    model: logic
                    Image {
                        height: squareSize
                        width : squareSize
                        x: squareSize * positionX
                        y: squareSize * positionY
                        source: images[type].imgPath
                    }
                }
            }
        }
    }
    Text {
        anchors.bottom: parent.bottom
        anchors.right: parent.right
        text: qsTr("by dspiridonov");
    }
}
