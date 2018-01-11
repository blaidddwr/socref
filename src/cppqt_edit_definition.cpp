#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QTableWidget>
#include <QSpinBox>
#include <QGroupBox>
#include <QGridLayout>
#include <QLabel>
#include <QLineEdit>
#include <QFormLayout>

#include "cppqt_edit_definition.h"



using namespace std;
using namespace CppQt::Edit;






unique_ptr<QLayout> Definition::createForm()
{
   unique_ptr<QLayout> ret{new QHBoxLayout};
   //QVBoxLayout* lala {new QVBoxLayout};
   QFormLayout* momo {new QFormLayout};
   //momo2->addWidget(new QLabel("Name:"));
   //momo2->addWidget(new QLineEdit);
   //momo2->addStretch();
   //QLabel* faaa {new QLabel("Size:")};
   //faaa->setAlignment(Qt::AlignRight);
   //faaa->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
   momo->addRow(new QLabel("Name:"),new QLineEdit);
   momo->addRow(new QLabel("Size:"),new QSpinBox);
   //QSpinBox* moo {new QSpinBox};
   //moo->setAlignment(Qt::AlignLeft);
   //moo->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
   QTableWidget* okok {new QTableWidget(10,1)};
   okok->setHorizontalHeaderLabels(QStringList() << "Names");
   //okok->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
   momo->addRow(new QLabel("Variants:"),okok);
   QGroupBox* far {new QGroupBox("Template Parameters")};
   far->setLayout(momo);
   ret->addWidget(far);
   return ret;
}
