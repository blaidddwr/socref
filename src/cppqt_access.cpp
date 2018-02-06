#include "cppqt_access.h"
#include "cppqt_blockfactory.h"
#include "exception.h"
#include "domelementreader.h"
#include "cppqt_function.h"



using namespace std;
using namespace CppQt;






Access::Access(Type type):
   _type(type)
{}






QString Access::name() const
{
   switch (_type)
   {
   case Type::Public: return "public:";
   case Type::Signals: return "signals:";
   case Type::PublicSlots: return "public slots:";
   case Type::Protected: return "protected:";
   case Type::ProtectedSlots: return "protected slots:";
   case Type::Private: return "private:";
   case Type::PrivateSlots: return "private slots:";
   default: return "public:";
   }
}






unique_ptr<AbstractBlock> Access::makeCopy() const
{
   unique_ptr<Access> ret {new Access};
   ret->_type = _type;
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
   else ret << BlockFactory::FunctionType << BlockFactory::VariableType;
   return ret;
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
   if ( isSlot(type) && ( hasFunctionsOrVariables() || hasSignals() ) )
   {
      Exception::InvalidArgument e;
      MARK_EXCEPTION(e);
      e.setDetails(tr("Slot access types cannot contain functions, variables, and/or signals."));
      throw e;
   }
   if ( type == Type::Signals && ( hasFunctionsOrVariables() || hasSlots() ) )
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






bool Access::hasFunctionsOrVariables() const
{
   return hasChildOfTypes(QList<int>() << BlockFactory::FunctionType << BlockFactory::VariableType);
}






bool Access::hasVirtual() const
{
   const QList<AbstractBlock*> list {children()};
   for (auto child : list)
   {
      if ( child->type() == BlockFactory::FunctionType )
      {
         if ( Function* func = qobject_cast<Function*>(child) )
         {
            if ( func->isVirtual() ) return true;
         }
      }
   }
   return false;
}






bool Access::hasAbstract() const
{
   const QList<AbstractBlock*> list {children()};
   for (auto child : list)
   {
      if ( child->type() == BlockFactory::FunctionType )
      {
         if ( Function* func = qobject_cast<Function*>(child) )
         {
            if ( func->isAbstract() ) return true;
         }
      }
   }
   return false;
}






bool Access::isSlot(Type type)
{
   return ( type == Type::PublicSlots || type == Type::ProtectedSlots || type == Type::PrivateSlots );
}






bool Access::isNormal(Type type)
{
   return ( type == Type::Public || type == Type::Protected || type == Type::Private );
}






void Access::readData(const QDomElement& data)
{
   DomElementReader reader(data);
   _type = static_cast<Type>(reader.attributeToInt("type",false));
}






QDomElement Access::writeData(QDomDocument& document) const
{
   QDomElement ret {document.createElement(factory().elementName(type()))};
   ret.setAttribute("type",static_cast<int>(_type));
   return ret;
}
