#include "cppqt_factory.h"
#include "projectfactory.h"
#include "cppqt_namespace.h"
#include "cppqt_enumeration.h"
#include "cppqt_enumvalue.h"
#include "cppqt_variable.h"
#include "cppqt_template.h"
#include "cppqt_function.h"
#include "cppqt_struct.h"
#include "cppqt_union.h"
#include "cppqt_class.h"
#include "cppqt_access.h"
#include "cppqt_parent.h"
#include "cppqt_declaration.h"



namespace CppQt
{






/*!
 * Implements _Abstract::BlockFactory_ interface.
 *
 * @return See interface docs.
 */
int Factory::type() const
{
   return ProjectFactory::CppQt;
}






/*!
 * Implements _Abstract::BlockFactory_ interface.
 *
 * @param type See interface docs.
 *
 * @return See interface docs.
 */
QString Factory::elementName(int type) const
{
   // Based off the given block type return its element name, throwing an exception
   // if the given type is unknown.
   switch (type)
   {
   case Type::Namespace: return QStringLiteral("namespace");
   case Type::Enumeration: return QStringLiteral("enumeration");
   case Type::EnumValue: return QStringLiteral("enumvalue");
   case Type::Variable: return QStringLiteral("variable");
   case Type::Function: return QStringLiteral("function");
   case Type::Template: return QStringLiteral("template");
   case Type::Struct: return QStringLiteral("struct");
   case Type::Union: return QStringLiteral("union");
   case Type::Class: return QStringLiteral("class");
   case Type::Access: return QStringLiteral("access");
   case Type::Parent: return QStringLiteral("parent");
   case Type::Declaration: return QStringLiteral("declaration");
   default:
      Q_ASSERT(false);
   }
}






/*!
 * Constructs a new C++/Qt block factory.
 */
Factory::Factory()
   :
   Basic::BlockFactory(QStringLiteral(":/cppqt/defs.xml"))
{
}






/*!
 * Implements _Basic::BlockFactory_ interface.
 *
 * @param type See interface docs.
 *
 * @return See interface docs.
 */
Soc::Ut::QPtr<Basic::Block> Factory::createBasicBlock(int type) const
{
   // Based off the given block type return a new instance of that type or a null
   // pointer if that type does not have an implementation.
   switch (type)
   {
   case Type::Namespace: return Soc::Ut::QPtr<Basic::Block>(new CppQt::Namespace);
   case Type::Enumeration: return Soc::Ut::QPtr<Basic::Block>(new CppQt::Enumeration);
   case Type::EnumValue: return Soc::Ut::QPtr<Basic::Block>(new CppQt::EnumValue);
   case Type::Variable: return Soc::Ut::QPtr<Basic::Block>(new CppQt::Variable);
   case Type::Template: return Soc::Ut::QPtr<Basic::Block>(new CppQt::Template);
   case Type::Function: return Soc::Ut::QPtr<Basic::Block>(new CppQt::Function);
   case Type::Struct: return Soc::Ut::QPtr<Basic::Block>(new CppQt::Struct);
   case Type::Union: return Soc::Ut::QPtr<Basic::Block>(new CppQt::Union);
   case Type::Class: return Soc::Ut::QPtr<Basic::Block>(new CppQt::Class);
   case Type::Access: return Soc::Ut::QPtr<Basic::Block>(new CppQt::Access);
   case Type::Parent: return Soc::Ut::QPtr<Basic::Block>(new CppQt::Parent);
   case Type::Declaration: return Soc::Ut::QPtr<Basic::Block>(new CppQt::Declaration);
   default: return nullptr;
   }
}

}
