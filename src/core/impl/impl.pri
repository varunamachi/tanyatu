HEADERS += \
        impl/StdAudioLibrary.h \
        impl/StdHistoryManager.h \
        impl/StdPlaylistManager.h \
        impl/StdPlayQueue.h \
        impl/StdTrackInformationManager.h \
        impl/SettingsManager.h \
        impl/StdSettingsReaderWriter.h \
        impl/MMAudioEngine.h \
	
SOURCES += \
        impl/StdAudioLibrary.cpp \
        impl/StdHistoryManager.cpp \
        impl/StdPlaylistManager.cpp \
        impl/StdPlayQueue.cpp \
        impl/StdTrackInformationManager.cpp \
        impl/MMAudioEngine.cpp \


# linux-g++ {
    HEADERS += impl/PhononAudioEngine.h
    SOURCES += impl/PhononAudioEngine.cpp
# }
