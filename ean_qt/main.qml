import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Dialogs.qml 1.0
import QtQuick.Controls 2.3
import QtQuick.Layouts 1.3

Window {
    id: rectangle
    width: 640
    height: 500
    visible: true
    color: "#98e3e6"
    title: qsTr("Newton Algorithm")
    property int cnt: 0
    property int lim: 3
    property var res: "0"
    property int max_iter: 10
    property int steps: 0
    property double eps: 1e-5
    property double fguess: 1.0
    property double fguessr: 1.0
    property bool intv_arth: false
    property var p:0

    Connections {
        target: rectangle
        onSceneGraphInitialized: {
            ean.set_len(lim)
            textLEFT.text = ean.get_poli()
            textID.text = ean.get_list()
        }
    }

    Text {
        id: textTitle
        x: 212
        y: 26
        text: qsTr("Newton Algorithm")
        font.pixelSize: 24
        horizontalAlignment: Text.AlignHCenter

        font.family: "Verdana"
        minimumPixelSize: 23
        textFormat: Text.RichText
    }

    RadioButton {
        id: radioButtonFP
        x: 78
        y: 83
        text: qsTr("Floating point arithmetic")
        anchors.horizontalCenter: textX.horizontalCenter
        checked: true
        font.pointSize: 11

        Connections {
            target: radioButtonFP
            onToggled: {textFieldRight.visible = 0
                textRIGHT.visible = 0
                buttonAdd.visible = 0
                intv_arth = false
                ean.gen(lim)
                textStep.visible = true
                textLEFT.text = ean.get_poli()
                cnt = 0
            }
        }
    }

    RadioButton {
        id: radioButtonI
        x: 346
        y: 83
        text: qsTr("Interval arithmetic")
        anchors.top: radioButtonFP.top
        anchors.bottom: radioButtonFP.bottom
        checked: false
        font.pointSize: 11
        anchors.bottomMargin: 0
        anchors.topMargin: 0

        Connections {
            target: radioButtonI
            onToggled: {
                textFieldRight.visible = 1
                textRIGHT.visible = 1
                buttonAdd.visible = 1
                intv_arth = true
                ean.gen_intv(lim)
                textLEFT.text = ean.get_left()
                textRIGHT.text = ean.get_right()
                textStep.visible = false
                cnt = 0
            }
        }
    }

    TextField {
        id: textFieldDeg
        x: 120
        y: 169
        width: 139
        height: 40
        text: "3"
        color: "#000000"
        selectByMouse: true
        anchors.left: radioButtonFP.left
        anchors.right: textDegTitle.right
        hoverEnabled: true
        anchors.leftMargin: 0
        anchors.rightMargin: -53
        placeholderText: qsTr("Degree")

        Connections {
            target: textFieldDeg
            onEditingFinished: {
                if(ean.correction_test(textFieldDeg.text, 1) == true)
                {
                    textFieldDeg.color = "#000000"
                    textFieldDeg.focus = 0
                    lim = ean.translate(textFieldDeg.text)
                    ean.set_len(lim)


                    if(intv_arth == false)
                    {
                        ean.gen(lim)
                        textLEFT.text = ean.get_poli()
                    }
                    else{
                        ean.gen_intv(lim)
                        textLEFT.text = ean.get_left()
                        textRIGHT.text = ean.get_right()
                    }
                    textID.text = ean.get_list()


                    cnt = 0
                    console.log(cnt)
                    textFieldLeft.text = 0
                    textFieldLeft.color = "#000"
                    textFieldRight.text = 0
                    textFieldRight.color = "#000"
                }
                else
                {
                    textFieldDeg.color = "#FF0099"
                }


            }
        }
    }

    Text {
        id: textDegTitle
        x: 124
        y: 145
        text: qsTr("Degree of Polynomial")
        anchors.left: radioButtonFP.left
        anchors.bottom: textFieldDeg.top
        font.pixelSize: 15
        anchors.leftMargin: 0
        anchors.bottomMargin: 6
    }

    TextField {
        id: textFieldLeft
        x: 78
        y: 279
        height: 40
        text: "0"
        anchors.left: radioButtonFP.left
        anchors.right: radioButtonFP.right
        clip: false
        anchors.rightMargin: 15
        placeholderText: qsTr("coefficient")
        selectByMouse: true
        Connections {
            target: textFieldLeft
            onEditingFinished: {
                if (ean.correction_test(textFieldLeft.text, 0) == 1)
                {
                    textFieldLeft.color = "#000000"
                    if (cnt <= lim)
                    {
                        console.log(textFieldLeft.text)
                        if(intv_arth == false)
                        {
                            ean.add(cnt, Number(textFieldLeft.text))
                            textLEFT.text = ean.get_poli()
                            textFieldLeft.text = 0
                            cnt +=1
                        }


                    }
                    if(lim-cnt == -1){
                        textFieldLeft.color = "#000000"
                        textFieldLeft.text = qsTr("No more coefficient")
                        textFieldRight.text = qsTr("No more coefficient")
                        textFieldLeft.focus = 0

                    }
                }
                else
                {
                    textFieldLeft.color = "#FF0099"
                }


            }
        }
    }

    TextField {
        id: textFieldRight
        x: 124
        y: 332
        height: 40
        text: "0"
        visible: false
        anchors.left: radioButtonFP.left
        anchors.right: radioButtonFP.right
        anchors.rightMargin: 15
        anchors.leftMargin: 0
        placeholderText: qsTr("coefficient")
        selectByMouse: true
        Connections {
            target: textFieldRight
            onEditingFinished: {
                if (ean.correction_test(textFieldRight.text, 0) == 1)
                {
                    textFieldRight.color = "#000000"
                    if (cnt <= lim)
                    {
                        console.log(textFieldRight.text)

                    }
                    if(lim-cnt == -1){
                        textFieldRight.color = "#000000"
                        textFieldRight.text = qsTr("No more coefficient")
                        textFieldRight.focus = 0

                    }
                }
                else
                {
                    textFieldRight.color = "#FF0099"
                }
            }
        }
    }

    Button {
        id: buttonAdd
        x: 280
        y: 303
        width: 65
        height: 40
        visible: false
        text: qsTr("Add")
        Connections {
            target: buttonAdd
            onClicked: {
                if(ean.correction_test(textFieldLeft.text, 0) && ean.correction_test(textFieldRight.text, 0) == 1)
                {
                    if(ean.correction_compare(Number(textFieldLeft.text), Number(textFieldRight.text)) == 1)
                    {
                        if (cnt <= lim)
                        {
                            console.log(textFieldLeft.text)
                            ean.add_intv(textFieldLeft.text, textFieldRight.text, cnt)
                            textFieldLeft.text = 0
                            textFieldRight.text = 0
                            cnt +=1
                        }
                        if(lim-cnt == -1){
                            textFieldLeft.color = "#F09"
                            textFieldRight.color = "#F09"
                            textFieldLeft.text = qsTr("No more coefficient")
                            textFieldRight.text = qsTr("No more coefficient")
                            textFieldLeft.focus = 0
                            textFieldRight.focus = 0

                        }
                    }
                    else{
                        textFieldLeft.color = "#F09"
                        textFieldRight.color = "#F09"
                    }

                }
                textLEFT.text = ean.get_left()
                textRIGHT.text = ean.get_right()
            }
        }
    }


    Text {
        id: textCoef
        x: 124
        y: 259
        width: 54
        height: 14
        text: qsTr("Coefficent")
        anchors.left: radioButtonFP.left
        anchors.bottom: textFieldLeft.top
        font.pixelSize: 15
        anchors.leftMargin: 0
        anchors.bottomMargin: 6
    }

    ScrollView {
        id: scrollView
        x: 380
        y: 145
        width: 227
        height: 242
        contentHeight: textID.height
        clip: true

        Text {
            id: textID
            x: 0
            y: 0
            text: qsTr("")
            font.pixelSize: 15
            clip: true
        }
        Text {
            id: textLEFT
            x: 69
            y: 0
            font.pixelSize: 15
            clip: true
        }
        Text {
            id: textRIGHT
            x: 154
            y: 0
            visible: false
            text: qsTr("")
            font.pixelSize: 15
            clip: true
        }
    }

    Text {
        id: textRelVal
        x: 346
        y: 182
        visible: false
        text: textFieldDeg.text
        anchors.verticalCenter: textFieldDeg.verticalCenter
        font.pixelSize: 12
        anchors.bottomMargin: 0
        anchors.leftMargin: 6
        anchors.topMargin: 0
    }



    Button {
        id: buttonEXE
        x: 124
        y: 393
        text: qsTr("Execute")
        anchors.left: textCoef.left
        anchors.leftMargin: 0

        Connections {
            target: buttonEXE
            onClicked: {
                if(intv_arth == false)
                {
                    res = ean.newton(fguess, eps, max_iter)
                }
                else
                {
                    p = ean.convert_to_tab()
                    res = ean.newton_i(fguess, fguessr, eps, max_iter, p)
                }
                steps = ean.get_step()
                if(steps == max_iter)
                {
                    textStep.color = "#F09"
                }
                else{
                    textStep.color = "#000"
                }
                textStep.text = qsTr("Steps: %1").arg(steps)
                textEditRes.text = res
            }

        }
    }

    Text {
        id: textX
        x: 165
        y: 259

        text: qsTr("x^%1").arg(lim - cnt)
        anchors.verticalCenter: textCoef.verticalCenter
        font.pixelSize: 15
        anchors.bottomMargin: 0
        anchors.topMargin: 0
        rotation: 1.202
    }





    TextEdit {
        id: textEditRes
        x: 91
        y: 455
        width: 244
        height: 20
        text: res
        font.pixelSize: 17
        selectByKeyboard: true
        selectByMouse: true
        readOnly: true
    }

    Text {
        id: textRes
        x: 32
        y: 455
        text: qsTr("Result:")
        font.pixelSize: 17
    }

    Button {
        id: buttonAddParam
        x: 371
        y: 393
        width: 129
        height: 40
        text: qsTr("Additional Parameters")
        clip: false
        font.pointSize: 8

        Connections {
            target: buttonAddParam
            onClicked: {
                pane.visible = 1

            }
        }
    }

    Text {
        id: textStep
        x: 174
        y: 406
        width: 99
        height: 15
        text: qsTr("Steps: %1").arg(steps)
        visible: true
        font.pixelSize: 15
    }

    Button {
        id: buttonEdit
        x: 506
        y: 393
        text: qsTr("Reset")
        font.pointSize: 8

        Connections {
            target: buttonEdit
            onClicked: {
                cnt = 0
                lim = 3
                ean.set_len(lim)
                textFieldLeft.text = 0
                textFieldLeft.color = "#000"
                textFieldRight.text = 0
                textFieldRight.color = "#000"
                textFieldDeg.text = lim
                res =0
                textEditRes.text = 0
                steps = 0
                textStep.color = "#000"
                if(intv_arth == false)
                {
                    ean.gen(lim)
                    textID.text = ean.get_list()
                    textLEFT.text = ean.get_poli()
                }
                else{
                    ean.gen_intv(lim)
                    textID.text = ean.get_list()
                    textLEFT.text = ean.get_left()
                    textRIGHT.text = ean.get_right()
                }
            }
        }
    }


    Pane {
        id: pane
        x: 220
        y: 90
        width: 200
        height: 339
        visible: false
        Text {
            id: textAddParamTitle
            x: 10
            y: 15
            visible: pane.visible
            color: "#113fb7"
            text: qsTr("Additonal parameters")
            font.pixelSize: 16
        }

        Text {
            id: textMaxIter
            x: 0
            y: 50
            visible: pane.visible
            text: qsTr("MAX iteration")
            font.pixelSize: 12
        }

        Text {
            id: textFirstGuess
            x: 0
            y: 100
            width: 58
            height: 0
            visible: pane.visible
            text: qsTr("First guess")
            font.pixelSize: 12
        }

        Text {
            id: textEps
            x: 0
            y: 194
            visible: pane.visible
            text: qsTr("EPS")
            font.pixelSize: 12
        }

        TextField {
            id: textFieldMIter
            x: 0
            y: 65
            width: 163
            height: 29
            visible: pane.visible
            placeholderText: qsTr("maximum iterations")
            text: max_iter
            selectByMouse: true
            Connections {
                target: textFieldMIter
                onEditingFinished:{
                    if(ean.correction_test(textFieldMIter.text, 1) == 1)
                    {
                        textFieldMIter.color = "#000"
                        max_iter = Number(textFieldMIter.text)
                        textFieldMIter.focus = 0
                    }
                    else{
                        textFieldMIter.color = "#F09"
                    }


                }
            }
        }

        TextField {
            id: textFieldFGuess
            x: 0
            y: 120
            width: 163
            height: 29
            visible: pane.visible
            placeholderText: qsTr("first guess")
            text: fguess
            selectByMouse: true
            Connections {
                target: textFieldFGuess
                onEditingFinished:{
                    if(ean.correction_test(textFieldFGuess.text, 0) == 1)
                    {
                        textFieldFGuess.color = "#000"
                        fguess = Number(textFieldFGuess.text)
                        textFieldFGuess.focus = 0
                    }
                    else{
                        textFieldFGuess.color = "#F09"
                    }

                }
            }
        }

        TextField {
            id: textFieldEPS
            x: 0
            y: 215
            width: 163
            height: 29
            visible: pane.visible
            placeholderText: qsTr("eps")
            text: eps
            selectByMouse: true

            onEditingFinished:{

                if(ean.correction_test(textFieldEPS.text, 0) == 1)
                {
                    textFieldEPS.color = "#000"
                    eps = Number(textFieldEPS.text)
                    textFieldEPS.focus = 0
                }
                else{
                    textFieldEPS.color = "#F09"
                }

            }
        }

        TextField {
            id: textFieldFGuessR
            x: 0
            y: 150
            width: 163
            height: 29
            visible: (pane.visible && intv_arth)
            text: qsTr("1")
            selectByMouse: true
            placeholderText: "first guess - right"
            Connections {
                target: textFieldFGuessR
                onEditingFinished:{
                    if(ean.correction_test(textFieldFGuessR.text, 0) == 1)
                    {
                        textFieldFGuessR.color = "#000"
                        fguessr = Number(textFieldFGuessR.text)
                        textFieldFGuessR.focus = 0
                    }
                    else{
                        textFieldFGuessR.color = "#F09"
                    }

                }
            }
        }

        Button {
            id: buttonDone
            x: 32
            y: 259
            width: 100
            height: 31
            visible: pane.visible
            text: qsTr("Done")

            Connections {
                target: buttonDone
                onClicked:
                {
                    if(ean.correction_test(textFieldMIter.text, 1) == 1 && ean.correction_test(textFieldEPS.text, 0) == 1)
                    {
                        if (intv_arth == false)
                        {
                            if(ean.correction_test(textFieldFGuess.text, 0) == 1)
                            {
                                pane.visible = 0
                            }
                        }
                        else{

                            if(ean.correction_test(textFieldFGuess.text, 0) == 1 && ean.correction_test(textFieldFGuessR.text, 0) == 1)
                            {
                                if(ean.correction_compare(Number(textFieldFGuess.text),Number(textFieldFGuessR.text))==1)
                                {
                                    textFieldFGuess.color = "#000"
                                    textFieldFGuessR.color = "#000"
                                    pane.visible = 0
                                }
                                else{
                                    textFieldFGuess.color = "#F09"
                                    textFieldFGuessR.color = "#F09"
                                }
                            }

                        }
                    }


                }


            }
        }

    }

    Text {
        id: element
        x: 458
        y: 474
        text: qsTr("Coded by Karol Wesołowski")
        font.pixelSize: 12
    }







}


