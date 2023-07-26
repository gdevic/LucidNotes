QT += core gui widgets sql

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    CTextDocument.cpp \
    CTextEdit.cpp \
    ClassDatabase.cpp \
    ClassEnex.cpp \
    ClassNote.cpp \
    ClassWorkspace.cpp \
    DialogOptions.cpp \
    EditWindow.cpp \
    WidgetTableView.cpp \
    WidgetTextEdit.cpp \
    WidgetTreeView.cpp \
    aes256.cpp \
    main.cpp \
    MainWindow.cpp

HEADERS += \
    CTextDocument.h \
    CTextEdit.h \
    ClassDatabase.h \
    ClassEnex.h \
    ClassNote.h \
    ClassWorkspace.h \
    DialogOptions.h \
    EditWindow.h \
    MainWindow.h \
    Utils.h \
    WidgetTableView.h \
    WidgetTextEdit.h \
    WidgetTreeView.h \
    aes256.h

FORMS += \
    DialogOptions.ui \
    EditWindow.ui \
    MainWindow.ui \
    WidgetTableView.ui \
    WidgetTextEdit.ui \
    WidgetTreeView.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resources.qrc
