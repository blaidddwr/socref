
SOURCES += \
    glsl_settings.cpp \
    glsl_settings_dialog.cpp \
    glsl_factory.cpp \
    glsl_base.cpp \
    glsl_shader.cpp \
    glsl_shader_view.cpp \
    glsl_variable.cpp \
    glsl_variable_view.cpp \
    glsl_function.cpp \
    glsl_function_view.cpp

HEADERS += \
    glsl.h \
    glsl_settings.h \
    glsl_settings_dialog.h \
    glsl_factory.h \
    glsl_base.h \
    glsl_namespace.h \
    glsl_shader.h \
    glsl_shader_view.h \
    glsl_variable.h \
    glsl_variable_view.h \
    glsl_struct.h \
    glsl_function.h \
    glsl_function_view.h

include(GLSL_Parse.pri)
