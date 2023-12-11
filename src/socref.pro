QT += core gui widgets testlib
CONFIG += c++17

SOURCES +=  Global.cpp \
            BlockAbstract.cpp \
            BlockCppBase.cpp \
            BlockCppClass.cpp \
            BlockCppEnumeration.cpp \
            BlockCppNamespace.cpp \
            BlockTestNode.cpp \
            ExceptionBase.cpp \
            FactoryLanguage.cpp \
            LanguageAbstract.cpp \
            LanguageCpp.cpp \
            LanguageCppQt.cpp \
            LanguageTest.cpp \
            ModelMetaBlock.cpp \
            ModelMetaLanguage.cpp \
            ModelProject.cpp \
            Test.cpp \
            TestBlockAbstract.cpp \
            TestBlockCppClass.cpp \
            TestBlockCppEnumeration.cpp \
            TestBlockCppNamespace.cpp \
            TestFactoryLanguage.cpp \
            TestLanguageAbstract.cpp \
            TestLanguageCpp.cpp \
            TestLanguageCppQt.cpp \
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
            BlockTest.h \
            BlockTestNode.h \
            Exception.h \
            ExceptionBase.h \
            ExceptionBlock.h \
            ExceptionBlockLogical.h \
            ExceptionBlockRead.h \
            ExceptionProject.h \
            ExceptionProjectRead.h \
            ExceptionSystem.h \
            ExceptionSystemFile.h \
            Factory.h \
            FactoryLanguage.h \
            Language.h \
            LanguageAbstract.h \
            LanguageCpp.h \
            LanguageCppQt.h \
            LanguageTest.h \
            Model.h \
            ModelMeta.h \
            ModelMetaBlock.h \
            ModelMetaLanguage.h \
            ModelProject.h \
            Test.h \
            TestBlock.h \
            TestBlockAbstract.h \
            TestBlockCpp.h \
            TestBlockCppClass.h \
            TestBlockCppEnumeration.h \
            TestBlockCppNamespace.h \
            TestFactory.h \
            TestFactoryLanguage.h \
            TestLanguage.h \
            TestLanguageAbstract.h \
            TestLanguageCpp.h \
            TestLanguageCppQt.h \
            TestModel.h \
            TestModelProject.h \
            Widget.h \
            WidgetBlock.h \
            WidgetBlockAbstract.h

RESOURCES += socref.qrc
