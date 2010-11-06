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
    physics.cpp

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
    physics.hpp

CONFIG += link_pkgconfig
PKGCONFIG += sdl

INCLUDEPATH += $$PWD/glew/include

DEFINES += GLEW_STATIC

LIBS += -lGL -lGLU

linux-* {
  exists(/usr/local/include/bullet/) {
    INCLUDEPATH += /usr/local/include/bullet/
    LIBS += -Wl,-rpath,/usr/local/lib
  }
  !exists(/usr/local/include/bullet/) {
    INCLUDEPATH += /usr/include/bullet/
  }
}
LIBS += -lLinearMath -lBulletDynamics -lBulletCollision -lBulletSoftBody
