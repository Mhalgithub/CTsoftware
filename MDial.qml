import QtQuick
import QtQuick.Controls
    // Item{
        // property alias dtext : text1.text
        // property alias from : dial.from
        // property alias to : dial.to
        // property alias model : rep.model
        // property alias value : dial.value
        // property alias hovered:dial.hovered
        // property alias pressed:dial.pressed
        Dial {
            // anchors.centerIn: parent
            property alias model:rep.model
            property alias dtext : text1.text
        id: dial
        height: 80
        width: 80
        antialiasing: true
        // width: parent.width*0.7
        // height: parent.height*0.7
        // property alias rottrans : handle
        property real lol :-140
        property real off
        startAngle: -140
        endAngle: 140
        // lol : angle
        onAngleChanged: {
            lol=angle}
        onLolChanged: {
            handle.transform[1].angle = lol}
        Component.onCompleted: {
        handle.transform[1].angle = lol
        off=handle.transform[0].y
        }
        hoverEnabled: true
        snapMode: Dial.SnapAlways
        live: false
        from:1
        to:10
        stepSize: 1
        Behavior on lol {
        SpringAnimation {spring: 1.4
        damping: 0.15}}
        Repeater{
            id : rep
            // model:parent.to-parent.from+1
            model : [1,3,5,7]
            anchors.fill : parent
        Text{
            antialiasing: true
            color : "#ffffff"
            width : 0
            height : 0
            // property int index: 1
            // anchors.centerIn: dial.handle
            x:dial.width/2
            y: dial.height/2
            font.pixelSize: 18
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            text: ""+ modelData
            transform : [
            Translate{
                    y:dial.off*1.8
                },
            Rotation{angle: -140
                            + index * 280/(rep.count-1)
                    // origin.x:y
                    // origin.y : x
                }
            ]
        }
        }
        // }

    Text {
        id: text1
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: parent.bottom
        color: "#ffffff"
        text: qsTr("Text")
        font.pixelSize: 20
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
    }
}
