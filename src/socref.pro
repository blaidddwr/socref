QT += core gui widgets testlib
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
            LanguageCppQt.cpp \
            ModelMetaBlock.cpp \
            ModelMetaLanguage.cpp \
            ModelProject.cpp \
            Test.cpp \
            TestLanguageCpp.cpp \
            TestModelProject.cpp \
            WidgetBlockAbstract.cpp

HEADERS +=  Global.h \
            Block.h \
            BlockAbstract.h \
            BlockCpp.h \
            BlockCppBase.h \
            BlockCppClass.h \
            BlockCppEnumeration.h \
            BlockCppNamespace.h \
            BlockCppQt.h \
            Factory.h \
            FactoryLanguage.h \
            Language.h \
            LanguageAbstract.h \
            LanguageCpp.h \
            LanguageCppQt.h \
            Model.h \
            ModelMeta.h \
            ModelMetaBlock.h \
            ModelMetaLanguage.h \
            ModelProject.h \
            Test.h \
            TestLanguage.h \
            TestLanguageCpp.h \
            TestModel.h \
            TestModelProject.h \
            Widget.h \
            WidgetBlock.h \
            WidgetBlockAbstract.h

RESOURCES += socref.qrc
