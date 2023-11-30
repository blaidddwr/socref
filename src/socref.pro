QT += core gui widgets
CONFIG += c++17

SOURCES +=  Global.cpp \
            BlockAbstract.cpp \
            BlockCppBase.cpp \
            BlockCppClass.cpp \
            BlockCppEnumeration.cpp \
            BlockCppNamespace.cpp \
            FactoryLanguage.cpp \
            LanguageAbstract.cpp \
            LanguageCpp.cpp \
            ModelMetaBlock.cpp \
            ModelMetaLanguage.cpp \
            ModelProject.cpp \
            WidgetBlockAbstract.cpp

HEADERS +=  Global.h \
            Block.h \
            BlockAbstract.h \
            BlockCpp.h \
            BlockCppBase.h \
            BlockCppClass.h \
            BlockCppEnumeration.h \
            BlockCppNamespace.h \
            Factory.h \
            FactoryLanguage.h \
            Language.h \
            LanguageAbstract.h \
            LanguageCpp.h \
            Model.h \
            ModelMeta.h \
            ModelMetaBlock.h \
            ModelMetaLanguage.h \
            ModelProject.h \
            Widget.h \
            WidgetBlock.h \
            WidgetBlockAbstract.h

RESOURCES += socref.qrc
