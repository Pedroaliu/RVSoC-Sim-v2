#ifndef ARCHLAB_TEST_FRAMEWORK_H
#define ARCHLAB_TEST_FRAMEWORK_H

#include <exception>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

namespace archlab::test {

using TestFunction = void (*)();

struct TestCase {
    std::string name;
    TestFunction function;
};

class Registry {
public:
    static Registry& instance() {
        static Registry registry;
        return registry;
    }

    void add(std::string name, TestFunction function) {
        tests_.push_back(TestCase{
            .name = std::move(name),
            .function = function,
        });
    }

    int run_all() const {
        std::size_t passed = 0;
        std::size_t failed = 0;

        for (const auto& test : tests_) {
            try {
                test.function();
                ++passed;
                std::cout << "[PASS] " << test.name << '\n';
            } catch (const std::exception& error) {
                ++failed;
                std::cerr << "[FAIL] " << test.name << "\n  " << error.what() << '\n';
            } catch (...) {
                ++failed;
                std::cerr << "[FAIL] " << test.name << "\n  unknown exception\n";
            }
        }

        std::cout << "\n" << passed << " passed, " << failed << " failed, "
                  << tests_.size() << " total\n";

        return failed == 0 ? 0 : 1;
    }

private:
    std::vector<TestCase> tests_;
};

class Registrar {
public:
    Registrar(const char* name, TestFunction function) {
        Registry::instance().add(name, function);
    }
};

[[noreturn]] inline void fail_check(const char* expression, const char* file, int line) {
    std::ostringstream message;
    message << "CHECK failed: " << expression << " at " << file << ':' << line;
    throw std::runtime_error(message.str());
}

template <typename Exception, typename Callable>
void check_throws_as(Callable&& callable,
                     const char* expression,
                     const char* exception_name,
                     const char* file,
                     int line) {
    try {
        std::forward<Callable>(callable)();
    } catch (const Exception&) {
        return;
    } catch (...) {
        std::ostringstream message;
        message << "CHECK_THROWS_AS failed: " << expression << " threw the wrong exception; expected "
                << exception_name << " at " << file << ':' << line;
        throw std::runtime_error(message.str());
    }

    std::ostringstream message;
    message << "CHECK_THROWS_AS failed: " << expression << " did not throw " << exception_name
            << " at " << file << ':' << line;
    throw std::runtime_error(message.str());
}

} // namespace archlab::test

#define ARCHLAB_TEST_CONCAT_IMPL(left, right) left##right
#define ARCHLAB_TEST_CONCAT(left, right) ARCHLAB_TEST_CONCAT_IMPL(left, right)

#define ARCHLAB_TEST(name)                                                                         \
    static void ARCHLAB_TEST_CONCAT(archlab_test_function_, __LINE__)();                           \
    static const ::archlab::test::Registrar                                                        \
        ARCHLAB_TEST_CONCAT(archlab_test_registrar_, __LINE__)(                                    \
            name, &ARCHLAB_TEST_CONCAT(archlab_test_function_, __LINE__));                          \
    static void ARCHLAB_TEST_CONCAT(archlab_test_function_, __LINE__)()

#define CHECK(...)                                                                                 \
    do {                                                                                           \
        if (!(__VA_ARGS__)) {                                                                      \
            ::archlab::test::fail_check(#__VA_ARGS__, __FILE__, __LINE__);                         \
        }                                                                                          \
    } while (false)

#define CHECK_THROWS_AS(expression, exception_type)                                                \
    ::archlab::test::check_throws_as<exception_type>(                                              \
        [&] { static_cast<void>(expression); }, #expression, #exception_type, __FILE__, __LINE__)

#endif // ARCHLAB_TEST_FRAMEWORK_H
