class Simple
{
};

class Forward1;
class Forward2;
class Forward3;

class AnotherOne
{
    int a;
    int b;
};

class Parents:
    public Simple
    ,private AnotherOne
{
    int c;
    int d;
//footer:
    int e;
    int f;
};

template<class A<int*>, class B = C&> class Templated
{
};
