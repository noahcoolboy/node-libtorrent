#include <vector>
#include <utility>
#include <iostream>

struct A { virtual ~A() { } };
struct B : A {};
struct C : A {};
struct D : A {};
struct E : A {};

std::vector<std::pair<std::string, A>> classlist;





template<class...Types> void iterate(Types...args) {
        std::cout<<"iterating "<<(&args...).name()<<std::endl;
};

template <class T> bool isA(const A& p)
{
    return dynamic_cast<const T*>(&p) != nullptr;
}

template <class T1, class T2, class...Ts>  bool isA(const A& p)
{
    cout << ".";
    return isA<T1>(p) || isA<T2, Ts...>(p);
}

int main()
{
    classlist.push_back(std::make_pair(B, "class B"));

    iterate(collection);
        //isA<collection>(B()); cout<<endl<<endl;

  return 0;
}
