CONFIG -= qt

SOURCES = $$system("find -name '*.cpp'")
HEADERS = $$system("find -name '*.hpp'")

CONFIG(release) {
    QMAKE_LFLAGS_RELEASE += -flto
    QMAKE_CXXFLAGS_RELEASE += -flto
    QMAKE_POST_LINK += upx -9 $(TARGET)
}
CONFIG(debug) {
    DEFINES += RUN_TESTS
    QMAKE_CXXFLAGS_DEBUG += -O0
    QMAKE_LFLAGS_DEBUG += -O0
}

QMAKE_CXX = ccache g++

LIBS += -lcrypt -lpthread
LIBS += -lboost_signals -lboost_regex -lboost_system -lboost_thread
LIBS += -lwt
LIBS += -lwtdbo -lwtdbosqlite3 -lwtdbopostgres

