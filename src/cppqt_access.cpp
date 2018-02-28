#include "cppqt_access.h"
#include "cppqt_view_access.h"
#include "cppqt_edit_access.h"
#include "cppqt_blockfactory.h"
#include "exception.h"
#include "domelementreader.h"
#include "cppqt_function.h"



using namespace std;
using namespace Gui;
using namespace CppQt;
const QStringList Access::_typeNames
{
   "public:"
   ,"protected:"
   ,"private:"
   ,"signals:"
   ,"public slots:"
   ,"protected slots:"
   ,"private slots:"
};
const char* Access::_typeTag {"type"};






Access::Access(Type type):
   _type(type)
{}






QString Access::name() const
{
   QString ret {_typeNames.at(static_cast<int>(_type))};
   ret.append(" (").append(QString::number(childrenSize())).append(")");
   return ret;
}






int Access::type() const
{
   return BlockFactory::AccessType;
}






const AbstractBlockFactory& Access::factory() const
{
   return BlockFactory::instance();
}






QIcon Access::icon() const
{
   static bool isLoaded {false};
   static QIcon public_;
   static QIcon signals_;
   static QIcon publicSlots;
   static QIcon protected_;
   static QIcon protectedSlots;
   static QIcon private_;
   static QIcon privateSlots;
   if ( !isLoaded )
   {
      public_ = QIcon(":/icons/public.svg");
      signals_ = QIcon(":/icons/signals.svg");
      publicSlots = QIcon(":/icons/pubslots.svg");
      protected_ = QIcon(":/icons/protected.svg");
      protectedSlots = QIcon(":/icons/proslots.svg");
      private_ = QIcon(":/icons/private.svg");
      privateSlots = QIcon(":/icons/prislots.svg");
      isLoaded = true;
   }
   switch (_type)
   {
   case Type::Public: return public_;
   case Type::Signals: return signals_;
   case Type::PublicSlots: return publicSlots;
   case Type::Protected: return protected_;
   case Type::ProtectedSlots: return protectedSlots;
   case Type::Private: return private_;
   case Type::PrivateSlots: return privateSlots;
   default: return public_;
   }
}






QList<int> Access::buildList() const
{
   QList<int> ret;
   if ( isSlot(_type) ) ret << BlockFactory::SlotType;
   else if ( _type == Type::Signals ) ret << BlockFactory::SignalType;
   else
   {
      ret << BlockFactory::FunctionType;
      ret << BlockFactory::OperatorType;
      ret << BlockFactory::ConstructorType;
      ret << BlockFactory::DestructorType;
      ret << BlockFactory::VariableType;
      ret << BlockFactory::EnumerationType;
      ret << BlockFactory::ClassType;
   }
   return ret;
}






unique_ptr<QWidget> Access::makeView() const
{
   return unique_ptr<QWidget>(new View::Access(this));
}






unique_ptr<AbstractEdit> Access::makeEdit()
{
   return unique_ptr<AbstractEdit>(new Edit::Access(this));
}






Access::Type Access::accessType()
{
   return _type;
}






void Access::setAccessType(Type type)
{
   if ( isNormal(type) && hasSignalsOrSlots() )
   {
      Exception::InvalidArgument e;
      MARK_EXCEPTION(e);
      e.setDetails(tr("Normal access types cannot contain signals and/or slots."));
      throw e;
   }
   if ( isSlot(type) && ( hasRegularMembers() || hasSignals() ) )
   {
      Exception::InvalidArgument e;
      MARK_EXCEPTION(e);
      e.setDetails(tr("Slot access types cannot contain functions, variables, and/or signals."));
      throw e;
   }
   if ( type == Type::Signals && ( hasRegularMembers() || hasSlots() ) )
   {
      Exception::InvalidArgument e;
      MARK_EXCEPTION(e);
      e.setDetails(tr("Signals access type cannot contain functions, variables, and/or slots."));
      throw e;
   }
   if ( _type != type )
   {
      _type = type;
      notifyOfNameChange();
      emit modified();
   }
}






bool Access::hasSignals() const
{
   return hasChildOfType(BlockFactory::SignalType);
}






bool Access::hasSlots() const
{
   return hasChildOfType(BlockFactory::SlotType);
}






bool Access::hasSignalsOrSlots() const
{
   return hasChildOfTypes(QList<int>() << BlockFactory::SignalType << BlockFactory::SlotType);
}






bool Access::hasRegularMembers() const
{
   QList<int> types
   {
      BlockFactory::FunctionType
      ,BlockFactory::OperatorType
      ,BlockFactory::ConstructorType
      ,BlockFactory::DestructorType
      ,BlockFactory::VariableType
      ,BlockFactory::EnumerationType
      ,BlockFactory::ClassType
   };
   return hasChildOfTypes(types);
}






bool Access::hasVirtual() const
{
   for (auto child : children())
   {
      if ( Function* func = qobject_cast<Function*>(child) )
      {
         if ( func->isVirtual() ) return true;
      }
   }
   return false;
}






bool Access::hasAbstract() const
{
   for (auto child : children())
   {
      if ( Function* func = qobject_cast<Function*>(child) )
      {
         if ( func->isAbstract() ) return true;
      }
   }
   return false;
}






bool Access::isSlot(Type type)
{
   return ( type == Type::PublicSlots
            || type == Type::ProtectedSlots
            || type == Type::PrivateSlots );
}






bool Access::isNormal(Type type)
{
   return ( type == Type::Public || type == Type::Protected || type == Type::Private );
}






void Access::childAdded(AbstractBlock* child)
{
   Q_UNUSED(child)
   notifyOfNameChange();
   emit bodyChanged();
}






void Access::childRemoved(AbstractBlock* child)
{
   Q_UNUSED(child)
   notifyOfNameChange();
   emit bodyChanged();
}






void Access::readData(const QDomElement& data)
{
   DomElementReader reader(data);
   _type = static_cast<Type>(_typeNames.indexOf(reader.attribute(_typeTag)));
}






QDomElement Access::writeData(QDomDocument& document) const
{
   QDomElement ret {document.createElement("na")};
   ret.setAttribute(_typeTag,_typeNames.at(static_cast<int>(_type)));
   return ret;
}






unique_ptr<AbstractBlock> Access::makeBlank() const
{
   return unique_ptr<AbstractBlock>(new Access);
}






void Access::copyDataFrom(const AbstractBlock* object)
{
   if ( const Access* object_ = qobject_cast<const Access*>(object) )
   {
      _type = object_->_type;
   }
   else
   {
      Exception::LogicError e;
      MARK_EXCEPTION(e);
      e.setDetails("Block object given to copy is not correct type");
      throw e;
   }
}
