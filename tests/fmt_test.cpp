#include <fmt/core.h>
#include <cstdio>

class FmtOutputGuard
{
public:
    // Flush stdout and redirect to null
    FmtOutputGuard()
    {
        std::fflush(stdout);
#ifdef _WIN32
        null_file = freopen("NUL", "w", stdout);
#else
        null_file = freopen("/dev/null", "w", stdout);
#endif
    }

    // Restore stdout
    ~FmtOutputGuard()
    {
        std::fflush(stdout);
#ifdef _WIN32
        if (null_file)
        {
            freopen("CON", "w", stdout);  // Windows console device
        }
#else
        if (null_file)
        {
            freopen("/dev/tty", "w", stdout);  // Linux/macOS console device
        }
#endif
    }

private:
    FILE *null_file = nullptr;
};

void test_fn()
{
    fmt::print("This should NOT appear.\n");
}

int main()
{
    fmt::print("Before benchmark\n");

        FmtOutputGuard guard;  // Disable fmt output
    {
        for (uint32_t i = 0; i < 5; ++i)
        {
            test_fn();
        }
    }

    fmt::print("After benchmark\n");

    return 0;
}
