class Test
{
//enumerations:
public:
    enum Foo
    {
        One
        ,Two
    };
//properties:
public:
    int foo() const;
    void setFoo(int value);
//body:
public:
    Test::Test(int foo);
    void lookAtFoo();
};
