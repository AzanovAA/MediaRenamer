QT += widgets
CONFIG += c++11

TARGET = "Visual Renamer"

DEFINES += "PREVIEW_SIZE=\"QSize(30, 24)\""

SOURCES += \
    main.cpp \
    GUI/mainwindow.cpp \
    GUI/fileslistmodel.cpp \
    GUI/fileslistwidget.cpp \
    fileslist.cpp \
    GUI/filepreview.cpp \
    VideoHandler/videohandler.cpp \
    VideoHandler/videodata.cpp \
    VideoPlayer/vlcerror.cpp \
    VideoPlayer/vlcmediaplayer.cpp \
    VideoPlayer/vlcvideo.cpp \
    VideoPlayer/vlcmedia.cpp \
    VideoPlayer/vlcinstance.cpp \
    VideoPlayer/vlcconfig.cpp \
    VideoPlayer/vlcaudio.cpp \
    GUI/videowidget.cpp \
    GUI/player.cpp \
    GUI/fixedheaderview.cpp \
    app.cpp \
    GUI/incrementsettingswindow.cpp \
    Utils/jpegthumbnailfetcher.cpp \
    aboutdialog.cpp \
    GUI/showlogdialog.cpp

HEADERS += \
    GUI/mainwindow.h \
    GUI/fileslistmodel.h \
    GUI/fileslistwidget.h \
    fileslist.h \
    GUI/filepreview.h \
    VideoHandler/videodata.h \
    VideoHandler/videohandler.h \
    VideoPlayer/vlcerror.h \
    VideoPlayer/videowidget.h \
    VideoPlayer/videodelegate.h \
    VideoPlayer/vlcmediaplayer.h \
    VideoPlayer/vlcvideo.h \
    VideoPlayer/vlcmedia.h \
    VideoPlayer/vlcinstance.h \
    VideoPlayer/vlcconfig.h \
    VideoPlayer/vlcaudio.h \
    GUI/videowidget.h \
    GUI/player.h \
    GUI/fixedheaderview.h \
    GUI/custombuttoneventer.h \
    app.h \
    settings.h \
    GUI/incrementsettingswindow.h \
    Utils/jpegthumbnailfetcher.h \
    aboutdialog.h \
    GUI/showlogdialog.h

RESOURCES += \
    resources.qrc

macx: {
    DEFINES += "MACOSX"
    QMAKE_MACOSX_DEPLOYMENT_TARGET = 10.7

    QMAKE_CFLAGS += -gdwarf-2
    QMAKE_CXXFLAGS += -gdwarf-2

    INCLUDEPATH += /usr/local/include
#    LIBS+= -L/opt/local/lib/ -lavutil -lavcodec -lavdevice -lavfilter -lavformat -lavresample -lswscale
    LIBS+= -L/usr/local/lib/ -lavutil -lavcodec -lavfilter -lavformat -lswscale


    #INCLUDEPATH += /Applications/'Visual Renamer.app'/Contents /Users/admin/Documents/vlc/build/vlc_install_dir/include
    #LIBS +=  /Users/admin/Documents/vlc/build/vlc_install_dir/lib/libvlc.5.dylib

    INCLUDEPATH += /Applications/VLC.app/Contents/MacOS/Include
    LIBS += /Applications/VLC.app/Contents/MacOS/lib/libvlc.5.dylib
    #LIBS += /Applications/'Visual Renamer.app'/Contents/Frameworks/libvlc.5.dylib

    #LIBS += -L/Users/admin/Documents/vlc/build/lib/ -lvlc

    #LIBS +=  /Applications/VLC.app/Contents/MacOS/Lib/libvlc.5.dylib

#    LIBS += /usr/lib/libvlc-qt.dylib /usr/lib/libvlc-qt-widgets.dylib
#    LIBS+= -L/usr/local/lib -lQtAV
    ##ICON = /Users/Paul_V/Projects/VideoEncoder/VideoEncoderGUI/resources/icon.icns
    QMAKE_INFO_PLIST = Info.plist
    ICON = icon.icns
}

win32: {
    INCLUDEPATH += C:/Dev/MinGW/libav/include
    LIBS += -LC:/Dev/MinGW/libav/lib

    INCLUDEPATH += C:/Dev/MinGW/vlc/include
    LIBS += -LC:/Dev/MinGW/vlc/lib

    LIBS += -lvlc
    LIBS += -lavutil -lavcodec -lavdevice -lavfilter -lavformat -lavresample -lswscale
}

FORMS += \
    aboutdialog.ui \
    GUI/showlogdialog.ui
