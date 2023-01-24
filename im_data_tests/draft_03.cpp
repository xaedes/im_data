#include <im_data/im_data.h>
#include <type_traits>
#include <sstream>
#include "tests_common.h"


namespace im_data_test_draft_03 {

    template <class T, class U=std::false_type> struct TypeHolder {};
    template <class T> struct is_type_holder : public std::false_type {};
    template <class T> struct is_type_holder<TypeHolder<T>> : public std::true_type {};
    template <class T> using enable_if_type_holder = std::enable_if_t<is_type_holder<T>::value, bool>;


    template<class B, class T, class H>
    struct Data {
        static_assert(is_type_holder<H>::value == true, "is_type_holder<H>::value == true");
        template<class... Args>
        static void def(B& be, T& ut, const H& th, Args&&... args) { 
            std::cout << "Data<B,T,H>::def(B& be, T& ut, const H& th, Args&&... args) " << "\n";
        }
    };
    
    template<class B, class T, class H>
    struct Fundamental {
        static_assert(is_type_holder<H>::value == true, "is_type_holder<H>::value == true");
        template<class... Args>
        static void def(B& be, T& ut, const H& th, Args&&... args) { 
            std::cout << "Fundamental<B,T,H>::def(B& be, T& ut, const H& th, Args&&... args) " << "\n";
        }
    };

    template<class B>
    struct Fundamental<B,int,TypeHolder<int>> {
        static void def(B& b, int& d, const TypeHolder<int>& th) {
            std::cout << "Fundamental<B,int,TypeHolder<int>>::def(B& b, int& d, TypeHolder<int>& th)" << "\n";
            b.value(d);
        };
    };
    template<class B>
    struct Fundamental<B,float,TypeHolder<float>> {
        static void def(B& b, float& d, const TypeHolder<float>& th) {
            std::cout << "Fundamental<B,float,TypeHolder<float>>::def(B& b, float& d, TypeHolder<float>& th)" << "\n";
            b.value(d);
        };
    };
    template<class B>
    struct Fundamental<B,std::string,TypeHolder<std::string>> {
        static void def(B& b, std::string& d, const TypeHolder<std::string>& th) {
            std::cout << "Fundamental<B,std::string,TypeHolder<std::string>>::def(B& b, std::string& d, TypeHolder<std::string>& th)" << "\n";
            b.value(d);
        };
    };

    struct Backend {
        std::stringstream ss;

        template<class T, class H, enable_if_type_holder<H> = true>
        void data(T& v, const H& h) { 
            ss << "< ";
            Data<CtxData,T,H>::def(ctx_data, v, h); 
            ss << ">";
        }

        struct CtxData {
            template<class T, class H, enable_if_type_holder<H> = true>
            void fundamental(T& v, const H& h) { 
                Fundamental<CtxFundamental,T,H>::def(base.ctx_fundamental, v, h); 
                base.ss << " ";
            } 

            template<class T> void fundamental(T& v) { 
                fundamental(v, TypeHolder<T>()); 
            }

            CtxData(Backend& base) : base(base) {}
        protected:
            Backend& base;
        };

        struct CtxFundamental {
            void value(int& v) { base.ss << v; }
            void value(float& v) { base.ss << v; }
            void value(std::string& v) { base.ss << "'" << v << "'"; }

            CtxFundamental(Backend& base) : base(base) {}
        protected:
            Backend& base;
        };

        Backend() 
        : ctx_data(*this)
        , ctx_fundamental(*this) 
        {} 

    protected:
        CtxData ctx_data;
        CtxFundamental ctx_fundamental;
    };

} // namespace im_data_test_draft_03

struct JustAnInteger {
    int value;
};

struct Foo {
    int integer;
    float number;
    std::string name;
    JustAnInteger just_int = JustAnInteger{3};
};

namespace im_data_test_draft_03 {
    template<class B>
    struct Data<B,Foo,TypeHolder<Foo>> {
        static void def(B& b, Foo& d, const TypeHolder<Foo>& th) {
            std::cout << "Data<B,Foo,TypeHolder<Foo>>::def(B& b, Foo& d, TypeHolder<Foo>& th)" << "\n";
            b.fundamental(d.integer);
            b.fundamental(d.number);
            b.fundamental(d.name); 
            b.fundamental(d.just_int); 
        };
    };

    template<class B>
    struct Fundamental<B,JustAnInteger,TypeHolder<JustAnInteger>> {
        static void def(B& b, JustAnInteger& d, const TypeHolder<JustAnInteger>& th) {
            std::cout << "Fundamental<B,JustAnInteger,TypeHolder<JustAnInteger>>::def(B& b, JustAnInteger& d, TypeHolder<JustAnInteger>& th)" << "\n";
            b.value(d.value);
        };
    };
} // namespace im_data_test_draft_03

int draft_03(int argc, char** argv) {
    using namespace im_data_test_draft_03;
    Foo foo{1, 2.0f, "bob"};
    Backend be;
    be.data(foo, TypeHolder<Foo>());
    std::cout << be.ss.str() << "\n";
    // < 1 2 'bob' 3 >
    return 0;
}
