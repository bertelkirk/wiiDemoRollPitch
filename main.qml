import QtQuick 2.3
import QtQuick.Window 2.2
import QtQuick.Controls 1.3

import BKWiiLibrary  1.0

Window{
    id: mainWindowId
    width: 400
    height: 410
    visible: true
    // color: "transparent"

    // property int yaw   :0
    property int roll  :0
    property int pitch : 0



    Rectangle{
        id: instrumentHorizonOutlineId
        x:95
        y:75
        width: 200
        height: 200
        radius: 0
        border.width: 6
        border.color: "gray"
        color: "transparent"
        clip: true
        smooth: true

        Rectangle{
            color: "yellow"
            height:3
            width: 40
            anchors.horizontalCenterOffset: -35
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.verticalCenterOffset:  0
            anchors.verticalCenter: parent.verticalCenter
        }

        Rectangle{
            color: "yellow"
            height:6
            width: 6
            anchors.horizontalCenterOffset: 0
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.verticalCenterOffset:  0
            anchors.verticalCenter: parent.verticalCenter
        }

        Rectangle{
            color: "yellow"
            height:3
            width: 40
            anchors.horizontalCenterOffset: +35
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.verticalCenterOffset:  0
            anchors.verticalCenter: parent.verticalCenter
        }



    Rectangle{
        id: skyGroundCanvasId
        z:-1
        /*x:100
        y:100*/
        anchors.centerIn:parent
        width: 200
        height: 200
        //radius: 8
        //border.width: 10
        //border.color: "gray"
        color: "transparent"
        //clip: true
        //smooth: true
        smooth: true



        Rectangle{
            z:-2
            id: backgroundSkyGroundId
            x:-50
            y:-300 + pitch*400/90

            width:300
            height: 4*skyGroundCanvasId.height
            gradient: Gradient {
                GradientStop {
                    position: 0.986
                    color: "#ffffff"
                }

                GradientStop {
                    position: 0
                    color: "#000060"
                }

                GradientStop {
                    position: 0.296
                    color: "#0042ed"
                }
            }
            Rectangle{
                y: backgroundSkyGroundId.height / 2
                width:300
                height: 2*skyGroundCanvasId.height
                gradient: Gradient {
                    GradientStop {
                        position: 0
                        color: "#643700"
                    }

                    GradientStop {
                        position: 1
                        color: "#f8b35e"
                    }
                }
                smooth: true
            }
            smooth: true
           // transform: Rotation{ origin.x: 150 ; origin.y: (400 + pitch*400/90); axis { x: 0; y: 0; z: 1 } angle: roll  }


        //}
            Rectangle{
                width:2
                height:parent.height
                anchors.horizontalCenter: parent.horizontalCenter
                smooth: true
            }

            Rectangle{
                width:parent.width
                height:2
                anchors.verticalCenter: parent.verticalCenter
                smooth: true
            }
            Repeater{
                anchors.horizontalCenter: parent.horizontalCenter
                model: 9
                Rectangle{
                    id:aa
                    width:20
                    height:2
                    y: 400+(400/90)*10*(index+1)
                    anchors.horizontalCenter: parent.horizontalCenter
                    smooth: true
                }
            }
            Repeater{
                anchors.horizontalCenter: parent.horizontalCenter
                model: 9
                Rectangle{
                    id: bb
                    width:20
                    height:2
                    y: 400-(400/90)*10*(index+1)
                    anchors.horizontalCenterOffset: 0
                    anchors.horizontalCenter: parent.horizontalCenter
                    smooth: true
                    //visible:true
                }
            }
            Repeater{
                anchors.horizontalCenter: parent.horizontalCenter
                model: 9
                Text{
                    id:cc
                    y: 400-(400/90)*10*(index+1)
                    anchors.horizontalCenterOffset: 15
                    anchors.horizontalCenter: parent.horizontalCenter
                    color : "white"
                    text : (index+1)*10
                }
            }
            Repeater{
                anchors.horizontalCenter: parent.horizontalCenter
                model: 9
                Text{
                    id: dd
                    y: 400+(400/90)*10*(index+1)
                    anchors.horizontalCenterOffset: -15
                    anchors.horizontalCenter: parent.horizontalCenter
                    color : "white"
                    text : (index+1)*10
                }
            }
        }

        rotation: roll
    }
    }

    Slider {
        id: sliderPitch
        x: 12
        y: 20
        minimumValue: -180
        maximumValue: 180
        value: 0
        height: 300
        orientation: Qt.Vertical
        onValueChanged: {
            pitch=value
        }
        rotation: 180
    }

    Slider {
        id: sliderRoll
        x: 47
        y: 324
        width: 307
        height: 28
        value: 0
        minimumValue: -180
        maximumValue: 180
        onValueChanged: {
            roll=value
        }
    }

    WiiRemoteSensor{
        onSignalWiiStatusUpdate:{
           // parsed values : (quint16 buttons, double roll, double pitch, qint16 x,qint16 y,qint16 z)
           //console.log("signalWiiStateUpdate: bnts:"+ buttons + "  roll:"+roll+ "   pitch:"+pitch+" x:"+x +" y:"+y +" z:"+z)
           mainWindowId.pitch = pitch

            mainWindowId.roll  = roll // * ((pitch > 90)?-1:1)

        }
    }




}

