
#define yyUseCustomFmt
#define yyExposeAliases
#define yyEnableTesting
#define yyEnableBenchmarking
#include "../src/y.hpp"

int main() {
    y::Benchmark B;
    usize executions = 5;
    B.set_align_column(42);

    B.run("StdCout", executions, [] {});
}
