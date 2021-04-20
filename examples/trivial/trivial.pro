TARGET      	= trivial

SOURCES		= trivial.cpp

win32 {
   RC_FILE		= trivial.rc
   DEFINES += _CRT_SECURE_NO_WARNINGS
} else:mac {
   QMAKE_INFO_PLIST = Info.plist
   REZ_FILES += trivial.r
   rsrc_files.files = trivial.rsrc
   rsrc_files.path = Contents/Resources
   QMAKE_BUNDLE_DATA += rsrc_files
} else {
   QMAKE_CXXFLAGS += -Wno-format-zero-length
}

include(../../src/qtbrowserplugin.pri)
