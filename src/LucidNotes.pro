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
    ClassUtils.cpp \
    ClassWorkspace.cpp \
    DialogOptions.cpp \
    WidgetTextEdit.cpp \
    aes256.cpp \
    main.cpp \
    MainWindow.cpp

HEADERS += \
    CTextDocument.h \
    CTextEdit.h \
    ClassDatabase.h \
    ClassEnex.h \
    ClassNote.h \
    ClassUtils.h \
    ClassWorkspace.h \
    DialogOptions.h \
    MainWindow.h \
    WidgetTextEdit.h \
    aes256.h

FORMS += \
    DialogOptions.ui \
    MainWindow.ui \
    WidgetTextEdit.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
