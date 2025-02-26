class Test
{
    Q_OBJECT
    Q_PROPERTY(int foo READ foo WRITE setFoo NOTIFY fooChanged)
//enums:
public:
    enum Foo
    {
        One
        ,Two
    };
//properties:
signals:
    void fooChanged(int value);
public:
    int foo() const;
    void setFoo(int value);
//body:
public:
    Test::Test(int foo);
    void lookAtFoo();
//footer:
private:
    int _foo;
};
