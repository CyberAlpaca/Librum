import QtQuick
import QtQuick.Layouts
import QtQuick.Controls


FocusScope
{
    id: root
    property string imagePath : "<path here>"
    property int imageWidth : 15
    property string textContent : "text here"
    property int textSpacing : 5
    signal clicked
    
    implicitWidth: 125
    implicitHeight: layout.implicitHeight
    
    
    RowLayout
    {
        id: layout
        anchors.fill: parent
        
        
        Image
        {
            id: syncIcon
            source: root.imagePath
            fillMode: Image.PreserveAspectFit
            sourceSize.width: root.imageWidth
        }
        
        Label
        {
            Layout.leftMargin: root.textSpacing
            text: root.textContent
            color: properties.colorLightText3
            font.family: properties.defaultFontFamily
            font.pointSize: 10.5
            font.weight: Font.Medium
        }
        
        MouseArea
        {
            Layout.fillWidth: true
            Layout.fillHeight: true
            
            onClicked: root.clicked()
        }
    }
}
