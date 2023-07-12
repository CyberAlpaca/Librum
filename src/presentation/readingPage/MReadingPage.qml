import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import QtQuick.Window 2.15
import CustomComponents 1.0
import Librum.style 1.0
import Librum.elements 1.0
import Librum.controllers 1.0
import Librum.globals 1.0
import "readingToolbar"
import "readingSearchbar"


Page
{
    id: root
    background: Rectangle { anchors.fill: parent; color: Style.colorPageBackground }
    
    Component.onCompleted: root.forceActiveFocus()
    Component.onDestruction: internal.saveCurrentPage()
    
    Shortcut
    {
        id: zoomIn
        sequences: [SettingsController.shortcuts.ZoomIn]
        onActivated: documentView.changeZoomBy(1.13)
    }
    
    Shortcut
    {
        id: zoomOut
        sequences: [SettingsController.shortcuts.ZoomOut]
        onActivated: documentView.changeZoomBy(0.87)
    }
    
    Shortcut
    {
        id: flickUp
        sequences: [SettingsController.shortcuts.MoveUp]
        onActivated: documentView.flick("up")
    }
    
    Shortcut
    {
        id: flickDown
        sequences: [SettingsController.shortcuts.MoveDown]
        onActivated: documentView.flick("down")
    }
    
    Shortcut
    {
        id: nextPage
        sequences: [SettingsController.shortcuts.NextPage]
        onActivated: documentView.nextPage()
    }
    
    Shortcut
    {
        id: previousPage
        sequences: [SettingsController.shortcuts.PreviousPage]
        onActivated: documentView.previousPage()
    }
    
    Shortcut
    {
        id: stopFullScreenMode
        sequences: [SettingsController.shortcuts.ExitFullScreenMode]
        onActivated: internal.stopFullScreenMode()
        enabled: internal.fullScreen
    }
    
    Shortcut
    {
        id: startOfDocument
        sequences: [SettingsController.shortcuts.StartOfDocument]
        onActivated: internal.goToStart()
    }
    
    Shortcut
    {
        id: endOfDocument
        sequences: [SettingsController.shortcuts.EndOfDocument]
        onActivated: internal.goToEnd()
    }
    
    
    DocumentItem
    {
        id: documentItem
        filePath: Globals.selectedBook.filePath
    }
    
    /*
      An invisible component at the top of the screen, when hovering over it
      while the fullscreen mode is enabled, the fullscreen mode will be disabled.
      */
    Item
    {
        id: toolbarReactivationContainer
        width: parent.width
        height: toolbar.height
        z: 1
        
        HoverHandler
        {
            id: toolbarReactivationArea
            enabled: true
            onHoveredChanged: if(hovered) internal.stopFullScreenMode();
        }
    }
    
    
    ColumnLayout
    {
        id: layout
        anchors.fill: parent
        spacing: 0
        
        
        MReadingToolBar
        {
            id: toolbar            
            Layout.fillWidth: true
            currentPage: documentItem.currentPage
            pageCount: documentItem.pageCount
            document: documentItem
            bookTitle: Globals.selectedBook.title
            
            onBackButtonClicked:
            {
                // Save current page before leaving
                internal.saveCurrentPage();
                loadPage(homePage, sidebar.homeItem, false);
            }
            
            onChapterButtonClicked:
            {
                if(chapterSidebar.active)
                {
                    chapterSidebar.close();
                    return;
                }
                
                if(bookmarksSidebar.active)
                    bookmarksSidebar.close();
                
                chapterSidebar.open();
            }
            
            onBookMarkButtonClicked:
            {
                if(bookmarksSidebar.active)
                {
                    bookmarksSidebar.close();
                    return;
                }
                
                if(chapterSidebar.active)
                    chapterSidebar.close();
                
                bookmarksSidebar.open();
            }
            
            onFullScreenButtonClicked:
            {
                if(internal.fullScreen)
                    internal.stopFullScreenMode();
                else
                    internal.startFullScreenMode();
            }
            
            onSearchButtonClicked:
            {
                searchbar.visible = !searchbar.visible;
            }
            
            onOptionsPopupVisibileChanged:
            {
                optionsButton.active = !optionsButton.active
            }
            
            
            PropertyAnimation
            {
                id: hideToolbar
                target: toolbar
                property: "opacity"
                to: 0
                duration: 100
                
                onFinished: toolbar.visible = false
            }
            
            PropertyAnimation
            {
                id: showToolbar
                target: toolbar
                property: "opacity"
                to: 1
                duration: 200
                
                onStarted: toolbar.visible = true
            }
        }
        
        
        /*
          Contains the reizeable sidebars, e.g. chapter or book-sidebar
          */
        SplitView
        {
            id: sidebarSplitView
            Layout.fillHeight: true
            Layout.fillWidth: true
            orientation: Qt.Horizontal
            padding: 0
            spacing: 10
            handle: Rectangle { implicitWidth: 8; color: "transparent" }
            smooth: true
            
            /*
              Need to combine sidebar items into one item, else rezising doesn't
              work properly, since the SplitView thinks they are 3 different items
              */
            Item
            {
                id: sidebarItem
                visible: chapterSidebar.active || bookmarksSidebar.active
                // Load in the last width of the correct sidebar
                SplitView.preferredWidth: chapterSidebar.visible ? chapterSidebar.lastWidth 
                                                                 : bookmarksSidebar.visible ? bookmarksSidebar.lastWidth : 0
                SplitView.minimumWidth: chapterSidebar.visible || bookmarksSidebar.visible ? 140 : 0
                SplitView.maximumWidth: 480
                
                
                MChapterSidebar
                {
                    id: chapterSidebar
                    property int lastWidth: 370
                    property bool active: false
//                    chapterModel: documentItem.tableOfContents
                    anchors.fill: parent
                    visible: false
                    
                    // Save the last width to restore it if re-enabled
                    onVisibleChanged: if(!visible) lastWidth = width
//                    onSwitchPage: (pageNumber) => documentView.setPage(...)
                     
                    
                    Rectangle
                    {
                        id: rightChaptersBorder
                        width: 1
                        height: parent.height
                        color: Style.colorDarkSeparator
                        anchors.right: parent.right
                    }
                    
                    
                    function open()
                    {
                        chapterSidebar.active = true;
                        chapterSidebar.visible = true;
                        toolbar.chapterButton.active = true;
                    }
                    
                    function close()
                    {
                        chapterSidebar.active = false;
                        chapterSidebar.visible = false;
                        toolbar.chapterButton.active = false;
                        documentView.forceActiveFocus();
                    }
                }
                
                MBookmarksSidebar
                {
                    id: bookmarksSidebar
                    property int lastWidth: 300
                    property bool active: false
                    
                    anchors.fill: parent
                    visible: false
                    
                    // Save the last width to restore it if re-enabled
                    onVisibleChanged: if(!visible) lastWidth = width
                    
                    
                    Rectangle
                    {
                        id: rightBookmarksBorder
                        width: 1
                        height: parent.height
                        color: Style.colorDarkSeparator
                        anchors.right: parent.right
                    }
                    
                    
                    function open()
                    {
                        bookmarksSidebar.active = true;
                        bookmarksSidebar.visible = true;
                        toolbar.bookmarksButton.active = true;
                    }
                    
                    function close()
                    {
                        bookmarksSidebar.active = false;
                        bookmarksSidebar.visible = false;
                        toolbar.bookmarksButton.active = false;
                    }
                }
            }
            
            
            RowLayout
            {
                id: documentLayout
                SplitView.fillWidth: true
                SplitView.fillHeight: true
                spacing: 0
                clip: true
                
                
                MDocumentView
                {
                    id: documentView
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    visible: true
                    document: documentItem
                }
            }
        }
        
        MReadingSearchbar
        {
            id: searchbar
            visible: false
            Layout.fillWidth: true
            
            onVisibleChanged: toolbar.searchButton.active = visible;
        }
    }
    
    QtObject
    {
        id: internal
        property bool fullScreen: false
        
        
        function startFullScreenMode()
        {
            if(internal.fullScreen)
                return;
            
            internal.fullScreen = true;
            hideToolbar.start();
        }
        
        function stopFullScreenMode()
        {
            if(!internal.fullScreen)
                return;
            
            internal.fullScreen = false;
            showToolbar.start();
        }
        
        function goToStart()
        {
            documentView.setPage(0);
        }

        function goToEnd()
        {
            documentView.setPage(documentItem.pageCount)
        }
        
        function saveCurrentPage()
        {
            var operationsMap = {};
            operationsMap[BookController.MetaProperty.CurrentPage] = 1;
            BookController.updateBook(Globals.selectedBook.uuid, operationsMap);
        }
    }
}