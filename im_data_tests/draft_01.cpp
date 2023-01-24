#include <im_data/im_data.h>
#include "tests_common.h"

namespace im_data_test_draft_01 {
    template<class B, class T>
    void data(B& backend, T& data) {
        std::cout << "data<B,T>(B& backend, T& data)" << "\n";
    };

    struct Backend {
        template<class T>
        void data(T& data) {
            std::cout << "Backend::data<T>(T& data)" << "\n";
            im_data_test_draft_01::data<Backend,T>(*this, data);
        };
    };

} // namespace im_data_test_draft_01

struct Foo {
    int integer;
    float number;
    std::string name;
};

int draft_01(int argc, char** argv) {
    using namespace im_data_test_draft_01;
    Foo foo{1, 2.0f, "bob"};
    Backend be;
    be.data(foo);
    return 0;
}
