CONFIG -= qt

SOURCES += \
    main.cpp \
    game.cpp \
    scene.cpp \
    opengl.cpp \
    light.cpp \
    shader.cpp \
    material.cpp \
    level.cpp \
    renderer.cpp \
    resource_manager.cpp \
    entity.cpp \
    player.cpp \
    animator.cpp \
    model.cpp \
    utils.cpp \
    camera.cpp \
    settings.cpp \
    glew/src/glew.c \
    overlay.cpp \
    input_handler.cpp \
    fbo.cpp \
    state.cpp \
    texture.cpp \
    render_context.cpp \
    grid.cpp \
    tga_image.cpp \
    physics.cpp \
    marching_cubes.cpp \
    parser.cpp \
    obj_loader.cpp

HEADERS += \
    game.hpp \
    scene.hpp \
    opengl.hpp \
    light.hpp \
    shader.hpp \
    material.hpp \
    level.hpp \
    renderer.hpp \
    resource_manager.hpp \
    entity.hpp \
    player.hpp \
    animator.hpp \
    model.hpp \
    utils.hpp \
    camera.hpp \
    settings.hpp \
    forward.hpp \
    glew/include/GL/glew.h \
    overlay.hpp \
    input_handler.hpp \
    zb_tr1.hpp \
    fbo.hpp \
    state.hpp \
    texture.hpp \
    render_context.hpp \
    grid.hpp \
    tga_image.h \
    physics.hpp \
    marching_cubes.hpp \
    math.hpp \
    parser.hpp \
    obj_loader.hpp

OTHER_FILES += ao.fs ao.vs blob.fs blob.vs blurx.fs blury.fs level.fs level.vs postproc.fs \
    debug.fs

CONFIG += link_pkgconfig

INCLUDEPATH += $$PWD/glew/include

DEFINES += GLEW_STATIC

win32 {
  LIBS += -lopengl32 -lglu32
  CONFIG += console
}
!win32 {
  LIBS += -lGL -lGLU
}

CONFIG(release, debug|release) {
  DEFINES += NDEBUG
}

CONFIG(debug, debug|release) {
  QMAKE_CXXFLAGS += -O0
  QMAKE_CFLAGS += -O0
}

#QMAKE_CXXFLAGS += -pg -g
#QMAKE_CFLAGS += -pg -g


# linux-* {
#   exists(/usr/local/include/bullet/) {
#     INCLUDEPATH += /usr/local/include/bullet/
#     LIBS += -Wl,-rpath,/usr/local/lib
#   }
#   !exists(/usr/local/include/bullet/) {
#     INCLUDEPATH += /usr/include/bullet/
#   }
# }
# LIBS += -lLinearMath -lBulletDynamics -lBulletCollision -lBulletSoftBody

exists($$SDL_PATH) {
  INCLUDEPATH += $$SDL_PATH/include
  LIBS += -L$$SDL_PATH/lib -lSDL -lSDLmain
}
!exists($$SDL_PATH) {
  PKGCONFIG += sdl
}

include(bullet/bullet.pri)
