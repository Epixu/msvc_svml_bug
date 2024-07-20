# msvc_svml_bug
Isolating a bug in MSVC's SVML implementation involving `_mm_pow_pd`, see [TestPow.cpp](https://github.com/Epixu/msvc_svml_bug/blob/main/TestPow.cpp) for reproduction

The bug is reproducible only on 32-bit builds with MSVC and SSE/SSE2/AVX/AVX2 enabled, and very big numbers.
The reproduction compares against `std::pow` as a control.

Check out the [Actions tab](https://github.com/Epixu/msvc_svml_bug/actions) for the CI infrastructure to see build and test logs.
These logs are temporary, but you can always fork and rerun the CI for yourself.
