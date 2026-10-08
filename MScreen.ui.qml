import QtQuick
import QtQuick.Controls
// import QtCharts
import QtGraphs
import QtQuick.Layouts
import CTsoftware

Rectangle {
    // Datalink {}
    id: rectangle1
    implicitWidth: 640
    implicitHeight: 480
    // width: 640
    // height: 480
    // property real chartr: 0.8
    // property real dialr: 0.3
    Connections {
        Component.onCompleted: {
            //         chartndial.SplitView.preferredHeight = target.height * 0.8
            //         dials.SplitView.preferredWidth = target.width * 0.3
            tobeupdated.text = Datalink.intializeserial()
            // console.log(Datalink.getseries(0))
            console.log(dataseriess.objectAt(0))
            for (var i = 0; i < 10; i++) {
                chartv.addSeries(dataseriess.objectAt(i))
            }
            dataseriess.objectAt(0).replace(Datalink.getseries(0))
        }
        //     function onWidthChanged() {
        //         dials.SplitView.preferredWidth = rectangle1.width * rectangle1.dialr
        //     }
        //     function onHeightChanged() {
        //         chartndial.SplitView.preferredHeight = rectangle1.chartr * rectangle1.height
        //     }
    }
    ColumnLayout {
        id: splitView
        anchors.fill: parent
        // orientation: "Vertical"
        // handle: Item {
        //     id: handleDelegate
        //     implicitWidth: 0
        //     implicitHeight: 0
        //     containmentMask: Item {
        //         y: -height / 2
        //         height: 6
        //         width: splitView.width
        //     }
        // }
        // Connections {
        //     function onResizingChanged() {
        //         if (!target.resizing) {
        //             chartr = chartndial.height / rectangle1.height
        //         }
        //     }
        // }
        // SplitView {
        //     id: hsplitView
        //     anchors.fill: parent
        //     handle: Item {
        //         id: hhandleDelegate
        //         implicitWidth: 0
        //         implicitHeight: 0
        //         containmentMask: Item {
        //             x: -width / 2
        //             width: 6
        //             height: splitView.height
        //         }
        //     }
        // Connections {
        //     function onResizingChanged() {
        //         if (!target.resizing) {
        //             dialr = dials.width / rectangle1.width
        //             // console.log(chartndial.preferredHeight)
        //         }
        //     }
        // }
        Item {
            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.verticalStretchFactor: 4
            // Layout.horizontalStretchFactor: 1
            implicitHeight: 480 * 0.8
            z: 10
            RowLayout {
                anchors.fill: parent
                Rectangle {
                    id: dials
                    color: "transparent"
                    gradient: Gradient {
                        GradientStop {
                            position: 0
                            color: "#2af598"
                        }

                        GradientStop {
                            position: 1
                            color: "#009efd"
                        }
                        orientation: Gradient.Vertical
                    }
                    implicitWidth: 640 * 0.2
                    Layout.horizontalStretchFactor: 1
                    Layout.maximumWidth: 640 * 0.2
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    z: 10
                    ColumnLayout {
                        anchors.fill: parent

                        MDial {
                            id: dial
                            Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
                            // anchors.centerIn: parent
                            // width: parent.width
                            // height: parent.width
                            Layout.fillWidth: true
                            Layout.fillHeight: false
                            // background: Item {}
                            ToolTip.visible: hovered ? true : false
                            MouseArea {
                                anchors.fill: parent
                                acceptedButtons: "NoButton"
                                cursorShape: parent.pressed ? "ClosedHandCursor" : "OpenHandCursor"
                            }
                            ToolTip.text: "hello"
                            ToolTip.delay: 500
                            dtext: "V range"
                            from: 1
                            to: 4
                            model: [1, 5, 10, 20]
                        }

                        MDial {
                            id: dial1
                            Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
                            Layout.fillWidth: true
                            Layout.fillHeight: false
                            dtext: "I range"
                            MouseArea {
                                anchors.fill: parent
                                acceptedButtons: "NoButton"
                                cursorShape: parent.pressed ? "ClosedHandCursor" : "OpenHandCursor"
                            }
                            ToolTip.visible: hovered ? true : false
                            ToolTip.text: "hello"
                            ToolTip.delay: 500
                        }

                        MDial {
                            id: dial2
                            Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
                            Layout.fillWidth: true
                            Layout.fillHeight: false
                            dtext: "I count"
                            MouseArea {
                                anchors.fill: parent
                                acceptedButtons: "NoButton"
                                cursorShape: parent.pressed ? "ClosedHandCursor" : "OpenHandCursor"
                            }
                            ToolTip.visible: hovered ? true : false
                            ToolTip.text: "hello"
                            ToolTip.delay: 500
                        }
                    }
                }

                GraphsView {
                    id: chartv
                    // color: "#ffffff"
                    // text: Datalink.showserials()
                    axisYSmoothing: aA.checked ? 1 : 0
                    axisXSmoothing: aA.checked ? 1 : 0
                    gridSmoothing: aA.checked ? 1 : 0
                    marginBottom: 0
                    marginLeft: 0
                    marginRight: 0
                    marginTop: 0
                    // implicitWidth: 640 * 0.8
                    Layout.verticalStretchFactor: 1
                    Layout.horizontalStretchFactor: 5
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    // elide: Text.ElideRight
                    // wrapMode: "WordWrap"
                    anchors.margins: 0
                    // font.weight: Font.Black
                    SplitView.fillWidth: true
                    SplitView.fillHeight: true

                    // axisX :xax
                    // backgroundColor: "#002222"
                    // theme:
                    Timer {
                        id: timer
                        interval: pause.checkState ? 0 : freq.checkState ? 500 : 1000
                        repeat: true
                        running: true
                    }
                    Connections {
                        target: timer
                        function onTriggered() {
                            Datalink.getinput()
                            for (var i = 0; i < 10; i++) {
                                dataseriess.objectAt(i).replace(
                                            Datalink.getseries(i))
                            }
                        }
                    }
                    axisX: ValueAxis {
                        min: 0
                        max: 128
                    }
                    axisY: ValueAxis {
                        min: 0
                        max: 256
                    }
                    Instantiator {
                        id: dataseriess
                        model: 10
                        delegate: LineSeries {
                            selectable: true
                            XYPoint {
                                x: 2
                                y: 2
                            }
                        }
                    }
                }
                // ChartView {
                //     id: chartv
                //     // color: "#ffffff"
                //     // text: Datalink.showserials()
                //     antialiasing: aA.checked
                //     // implicitWidth: 640 * 0.8
                //     Layout.verticalStretchFactor: 1
                //     Layout.horizontalStretchFactor: 5
                //     Layout.fillWidth: true
                //     Layout.fillHeight: true
                //     // elide: Text.ElideRight
                //     // wrapMode: "WordWrap"
                //     margins {
                //         left: 0
                //         top: 0
                //         right: 0
                //         bottom: 0
                //     }
                //     anchors.margins: 0
                //     // font.weight: Font.Black
                //     SplitView.fillWidth: true
                //     SplitView.fillHeight: true
                //     legend.visible: false
                //     backgroundRoundness: 0
                //     // axisX :xax
                //     // backgroundColor: "#002222"
                //     theme: ChartView.ChartThemeDark
                //     Timer {
                //         id: timer
                //         property int t: 0
                //         interval: pause.checkState ? 0 : freq.checkState ? 500 : 1000
                //         repeat: true
                //         running: true
                //     }
                //     Connections {
                //         target: timer
                //         function onTriggered() {
                //             console.log("hello:" + target.t + " " + target.interval)
                //             target.t++
                //         }
                //     }
                //     ValueAxis {
                //         id: xax
                //         min: 0
                //         max: 5
                //         tickCount: 10
                //         minorTickCount: 5
                //         minorGridLineColor: color.lighter(
                //                                 chartv.backgroundColor)
                //     }

                //     LineSeries {
                //         id: awd
                //         name: "LineSeries"
                //         Connections {
                //             function onClicked() {
                //                 replace(0, 0, rectangle1.height / 300)
                //             }
                //         }
                //         // onHovered :replace(0,0,rectangle1.height / 300)
                //         // color: hoverr.hovered ? "#ff0000" : "#00ff00"
                //         axisX: xax
                //         XYPoint {
                //             x: 0
                //             y: rectangle1.height / 100
                //         }

                //         XYPoint {
                //             x: 1
                //             y: dial1.value
                //         }

                //         XYPoint {
                //             x: 2
                //             y: dial2.value
                //         }

                //         XYPoint {
                //             x: 4
                //             y: 2.1
                //         }
                //         XYPoint {
                //             x: 5
                //             y: 0
                //         }
                //     }
                //     LineSeries {
                //         id: dummy
                //         name: "dummy"
                //         axisX: xax
                //         XYPoint {
                //             x: 0
                //             y: 5
                //         }
                //     }
                // }
            }
        }
        // }
        Rectangle {
            id: buttnpad
            implicitHeight: 480 * 0.2
            Layout.maximumHeight: 480 * 0.2
            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.verticalStretchFactor: 1
            GridLayout {
                anchors.fill: parent
                rows: 2
                columns: 5

                CheckBox {
                    id: aA
                    text: qsTr("Anti aliasing")
                    Connections {
                        function onPressed() {// console.log("data thread:" + Datalink.pthread())
                            // console.log("gui thread:" + rectangle1.thread)
                            // chartv.text = Datalink.showserials()
                            // dial.children[0].children[0].transform[1].angle += -10
                            // awd.append(4.5, 0)
                            // awd.replace(4, Qt.point(5, dial.value))
                            // chartv.update()
                            // awd.replace(5, 5, dial.value)
                            // for (var i = 0; i < 256; i++) {
                            //     Datalink.write(String.fromCharCode(i))
                            // }
                        }
                    }
                    ToolTip.visible: hovered ? true : false
                    ToolTip.text: "hello"
                    ToolTip.delay: 500
                }

                CheckBox {
                    id: pause
                    text: qsTr("Pause")
                    ToolTip.visible: hovered ? true : false
                    ToolTip.text: "hello"
                    ToolTip.delay: 500
                    // Connections {
                    //     function onPressed() {
                    //         Datalink.getinput()
                    //         dataseries.replace(Datalink.getseries(0))
                    //         dataseries.update()
                    //     }
                    // }
                }

                CheckBox {
                    id: fet
                    text: qsTr("FET(?)")
                    ToolTip.visible: hovered ? true : false
                    ToolTip.text: "hello"
                    ToolTip.delay: 500
                    // Connections {
                    //     function onPressed() {
                    //         Datalink.getinput()
                    //         for (var i = 0; i < dataseries.count; i++) {
                    //             console.log(dataseries.at(i))
                    //         }
                    //     }
                    // }
                }

                Slider {
                    id: interpolate
                    from : 5
                    to : 20
                    stepSize: 15
                    snapMode: "SnapOnRelease"
                    //text: qsTr("Interpolate")
                    ToolTip.visible: hovered ? true : false
                    ToolTip.text: "hello"
                    ToolTip.delay: 500
                }

                CheckBox {
                    id: polarity
                    text: qsTr("Polarity(?)")
                    ToolTip.visible: hovered ? true : false
                    ToolTip.text: "hello"
                    ToolTip.delay: 500
                }

                CheckBox {
                    id: bipolarity
                    text: qsTr("Bipolarity(?)")
                    ToolTip.visible: hovered ? true : false
                    ToolTip.text: "hello"
                    ToolTip.delay: 500
                }

                CheckBox {
                    id: manuallimit
                    text: qsTr("Manual Y limit")
                    ToolTip.visible: hovered ? true : false
                    ToolTip.text: "hello"
                    ToolTip.delay: 500
                }

                CheckBox {
                    id: lines
                    text: qsTr("Lines")
                    ToolTip.visible: hovered ? true : false
                    ToolTip.text: "hello"
                    ToolTip.delay: 500
                }

                CheckBox {
                    id: freq
                    text: qsTr("1Hz/2Hz(example)")
                    ToolTip.visible: hovered ? true : false
                    ToolTip.text: "hello"
                    ToolTip.delay: 500
                    // Connections {
                    //     function onPressed() {
                    //         tobeupdated.text = "data:" + Datalink.read(1)
                    //     }
                    // }
                }

                TextField {
                    id: tobeupdated
                    placeholderText: qsTr("Text Field")
                    ToolTip.visible: hovered ? true : false
                    ToolTip.text: "hello"
                    ToolTip.delay: 500
                }
            }
            // Layout.horizontalStretchFactor: 1
            color: "#e47da9ff"
        }
    }
}
