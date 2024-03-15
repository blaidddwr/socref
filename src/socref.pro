QT += core gui widgets testlib
CONFIG += c++17

SOURCES +=  Global.cpp \
            BlockAbstract.cpp \
            BlockCppBase.cpp \
            BlockCppClass.cpp \
            BlockCppEnumeration.cpp \
            BlockCppException.cpp \
            BlockCppFunction.cpp \
            BlockCppNamespace.cpp \
            BlockCppProperty.cpp \
            BlockCppQtFunction.cpp \
            BlockCppUnion.cpp \
            BlockCppVariable.cpp \
            CommandProjectAbstract.cpp \
            CommandProjectInsert.cpp \
            CommandProjectMove.cpp \
            CommandProjectRemove.cpp \
            CommandProjectSet.cpp \
            ExceptionBase.cpp \
            FactoryLanguage.cpp \
            LanguageAbstract.cpp \
            LanguageCpp.cpp \
            LanguageCppQt.cpp \
            ModelMetaBlock.cpp \
            ModelMetaLanguage.cpp \
            ModelProject.cpp \
            StreamBlockDir.cpp \
            StreamBlockXml.cpp \
            StreamProjectDir.cpp \
            StreamProjectXml.cpp \
            Test.cpp \
            TestBlockAbstract.cpp \
            TestBlockCppClass.cpp \
            TestBlockCppEnumeration.cpp \
            TestBlockCppExceptionItem.cpp \
            TestBlockCppFunction.cpp \
            TestBlockCppNamespace.cpp \
            TestBlockCppProperty.cpp \
            TestBlockCppQtFunction.cpp \
            TestBlockCppVariable.cpp \
            TestFactoryLanguage.cpp \
            TestLanguageAbstract.cpp \
            TestLanguageCpp.cpp \
            TestLanguageCppQt.cpp \
            TestModelProject.cpp \
            TestUnitModel.cpp \
            WidgetBlockAbstract.cpp \
            WidgetBlockCppBase.cpp \
            WidgetBlockCppClass.cpp \
            WidgetBlockCppEnumeration.cpp \
            WidgetBlockCppException.cpp \
            WidgetBlockCppFunction.cpp \
            WidgetBlockCppNamespace.cpp \
            WidgetBlockCppProperty.cpp \
            WidgetBlockCppUnion.cpp \
            WidgetBlockCppVariable.cpp \
            WidgetDialogAbout.cpp \
            WidgetDialogOrphanFiles.cpp \
            WidgetDialogProject.cpp \
            WidgetDialogUnitTests.cpp \
            WidgetProject.cpp \
            WidgetStringList.cpp \
            WidgetTreeView.cpp \
            WidgetWindowMain.cpp

HEADERS +=  Global.h \
            Block.h \
            BlockAbstract.h \
            BlockCpp.h \
            BlockCppBase.h \
            BlockCppClass.h \
            BlockCppEnumeration.h \
            BlockCppException.h \
            BlockCppFunction.h \
            BlockCppNamespace.h \
            BlockCppProperty.h \
            BlockCppQt.h \
            BlockCppQtFunction.h \
            BlockCppUnion.h \
            BlockCppVariable.h \
            Command.h \
            CommandProject.h \
            CommandProjectAbstract.h \
            CommandProjectInsert.h \
            CommandProjectMove.h \
            CommandProjectRemove.h \
            CommandProjectSet.h \
            Exception.h \
            ExceptionBase.h \
            ExceptionBlock.h \
            ExceptionBlockLogical.h \
            ExceptionBlockRead.h \
            ExceptionBlockWrite.h \
            ExceptionProject.h \
            ExceptionProjectLogical.h \
            ExceptionProjectRead.h \
            ExceptionProjectWrite.h \
            ExceptionSystem.h \
            ExceptionSystemFile.h \
            ExceptionSystemRun.h \
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
            Stream.h \
            StreamBlockDir.h \
            StreamBlockXml.h \
            StreamProjectDir.h \
            StreamProjectXml.h \
            Test.h \
            TestBlock.h \
            TestBlockAbstract.h \
            TestBlockCpp.h \
            TestBlockCppClass.h \
            TestBlockCppEnumeration.h \
            TestBlockCppExceptionItem.h \
            TestBlockCppFunction.h \
            TestBlockCppNamespace.h \
            TestBlockCppProperty.h \
            TestBlockCppQt.h \
            TestBlockCppQtFunction.h \
            TestBlockCppVariable.h \
            TestDummy.h \
            TestDummyBlock.h \
            TestDummyLanguage.h \
            TestFactory.h \
            TestFactoryLanguage.h \
            TestLanguage.h \
            TestLanguageAbstract.h \
            TestLanguageCpp.h \
            TestLanguageCppQt.h \
            TestModel.h \
            TestModelProject.h \
            TestUnitModel.h \
            Widget.h \
            WidgetBlock.h \
            WidgetBlockAbstract.h \
            WidgetBlockAbstract.t.h \
            WidgetBlockCpp.h \
            WidgetBlockCppBase.h \
            WidgetBlockCppClass.h \
            WidgetBlockCppEnumeration.h \
            WidgetBlockCppException.h \
            WidgetBlockCppFunction.h \
            WidgetBlockCppNamespace.h \
            WidgetBlockCppProperty.h \
            WidgetBlockCppUnion.h \
            WidgetBlockCppVariable.h \
            WidgetDialog.h \
            WidgetDialogAbout.h \
            WidgetDialogOrphanFiles.h \
            WidgetDialogProject.h \
            WidgetDialogUnitTests.h \
            WidgetProject.h \
            WidgetStringList.h \
            WidgetTreeView.h \
            WidgetWindow.h \
            WidgetWindowMain.h \
            gassert.h

RESOURCES += socref.qrc
