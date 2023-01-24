#include <im_data/im_data.h>
#include <type_traits>
#include "tests_common.h"

namespace im_data_test_draft_02 {

    template <class T, class U=std::false_type> struct TypeHolder {};

    template <class T>
    struct is_type_holder : public std::false_type {};

    template <class T>
    struct is_type_holder<TypeHolder<T>> : public std::true_type {};

    template <class T> 
    using enable_if_type_holder = std::enable_if_t<is_type_holder<T>::value, bool>;

    // Custom<B,T,H> can be specialized for T & H by users
    template<class B, class T, class H>
    struct Custom
    {
        static_assert(is_type_holder<H>::value == true, "is_type_holder<H>::value == true");
        template<class... Args>
        static void data(B& be, T& ut, const H& th, Args&&... args) 
        { 
            std::cout << "Custom<B,T,H>::data(B& be, T& ut, const H& th, Args&&... args) " << "\n";
            /* empty */ 
        }
    };
    
    struct Backend {
        template<class T>
        void data(T& data) {
            std::cout << "Backend::data<T>(T& data)" << "\n";
            std::cout << "data: " << data << "\n";
        };

        template<class T, class H, enable_if_type_holder<H> = true>
        void data(T& data, const H& th) {
            std::cout << "Backend::data<T>(T& data, const H& th)" << "\n";
            std::cout << "<" << "\n";
            Custom<Backend,T,H>::data(*this, data, th);
            std::cout << ">" << "\n";
        };
    };

} // namespace im_data_test_draft_02

struct Foo {
    int integer;
    float number;
    std::string name;
};

namespace im_data_test_draft_02 {
    template<class B>
    struct Custom<B,Foo,TypeHolder<Foo>> {
        static void data(B& b, Foo& d, const TypeHolder<Foo>& th) {
            std::cout << "Custom<B,Foo,TypeHolder<Foo>>::data(B& b, Foo& d, TypeHolder<Foo>& th)" << "\n";
            b.data(d.integer);
            b.data(d.number);
            b.data(d.name);
        };
    };
} // namespace im_data_test_draft_02

int draft_02(int argc, char** argv) {
    using namespace im_data_test_draft_02;
    Foo foo{1, 2.0f, "bob"};
    Backend be;
    be.data(foo, TypeHolder<Foo>());
    return 0;
}
