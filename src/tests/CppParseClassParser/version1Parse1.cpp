class Simple
{
};

class AnotherOne
{
    int a;
    int b;
};

class Parents:
    public Simple
    ,private AnotherOne
{
    int a;
    int b;
//footer:
    int c;
    int d;
};

template<class A<int*>, class B = C&> class Templated
{
};
