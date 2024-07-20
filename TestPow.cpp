#include <catch2/catch.hpp>
#include <immintrin.h>

/*#include <simde/x86/sse4.2.h>
#include <simde/x86/sse4.1.h>
#include <simde/x86/ssse3.h>
#include <simde/x86/sse3.h>
#include <simde/x86/sse2.h>
#include <simde/x86/sse.h>
#include <simde/x86/avx.h>
#include <simde/x86/avx2.h>

#include <simde/x86/svml.h>*/


// Comparing against std::pow                                                    
// This always gives consistent results, unless built on a 32bit MSVC build with 
// SSE/SSE2/AVX/AVX2                                                             
template<class T, size_t C>
void ControlPow(const T(&l)[C], const T(&r)[C], T(&out)[C]) noexcept {
   for (int i = 0; i < C; ++i)
      out[i] = ::std::pow(l[i], r[i]);
}

// Test float/double vectors                                                     
SCENARIO("Power on vectors", "[power]") {
   GIVEN("pow(x, y) = r using 4 floats") {
      float x[4] {1.1f, 2.2f, 3.3f, 42.f};
      float y[4] {1.0f, 2.0f, 3.0f, 59.f};
      float r[4];

      ControlPow(x, y, r);

      WHEN("Raised to a power via SIMD") {
         auto x2 = _mm_setr_ps(x[0], x[1], x[2], x[3]);
         auto y2 = _mm_setr_ps(y[0], y[1], y[2], y[3]);
         auto r2 = _mm_pow_ps(x2, y2);

         alignas(16) float test[4];
         _mm_store_ps(test, r2);

         for (int i = 0; i < 4; ++i)
            REQUIRE(test[i] == r[i]);
      }
   }

   GIVEN("pow(x, y) = r using 2 doubles") {
      double x[2] {1.1, 42.0};
      double y[2] {1.0, 59.f};
      double r[2];

      ControlPow(x, y, r);

      WHEN("Raised to a power via SIMD") {
         auto x2 = _mm_setr_pd(x[0], x[1]);
         auto y2 = _mm_setr_pd(y[0], y[1]);
         auto r2 = _mm_pow_pd(x2, y2);

         alignas(16) double test[2];
         _mm_store_pd(test, r2);

         for (int i = 0; i < 2; ++i)
            REQUIRE(test[i] == r[i]); // second element fails if built as 32bit MSVC build with SSE/SSE2/AVX/AVX2
      }
   }

   GIVEN("pow(x, y) = r using 4 doubles") {
      double x[4] {1.1, 2.2, 3.3, 42.0};
      double y[4] {1.0, 2.0, 3.0, 59.f};
      double r[4];

      ControlPow(x, y, r);

      WHEN("Raised to a power via SIMD") {
         auto x2 = _mm256_setr_pd(x[0], x[1], x[2], x[3]);
         auto y2 = _mm256_setr_pd(y[0], y[1], y[2], y[3]);
         auto r2 = _mm256_pow_pd(x2, y2);

         alignas(32) double test[4];
         _mm256_store_pd(test, r2);

         for (int i = 0; i < 4; ++i)
            REQUIRE(test[i] == r[i]); // fourth element fails if built as 32bit MSVC build with SSE/SSE2/AVX/AVX2
      }
   }
}