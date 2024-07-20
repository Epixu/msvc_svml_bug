#include "Common.hpp"


// Comparing against std::pow                                                    
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
         auto x2 = simde_mm_setr_ps(x[0], x[1], x[2], x[3]);
         auto y2 = simde_mm_setr_ps(y[0], y[1], y[2], y[3]);
         auto r2 = simde_mm_pow_ps(x2, y2);

         alignas(16) float test[4];
         simde_mm_store_ps(test, r2);

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
         auto x2 = simde_mm_setr_pd(x[0], x[1]);
         auto y2 = simde_mm_setr_pd(y[0], y[1]);
         auto r2 = simde_mm_pow_pd(x2, y2);

         alignas(16) double test[2];
         simde_mm_store_pd(test, r2);

         for (int i = 0; i < 2; ++i)
            REQUIRE(test[i] == r[i]);
      }
   }

   GIVEN("pow(x, y) = r using 4 doubles") {
      double x[4] {1.1, 2.2, 3.3, 42.0};
      double y[4] {1.0, 2.0, 3.0, 59.f};
      double r[4];

      ControlPow(x, y, r);

      WHEN("Raised to a power via SIMD") {
         auto x2 = simde_mm256_setr_pd(x[0], x[1], x[2], x[3]);
         auto y2 = simde_mm256_setr_pd(y[0], y[1], y[2], y[3]);
         auto r2 = simde_mm256_pow_pd(x2, y2);

         alignas(32) double test[4];
         simde_mm256_store_pd(test, r2);

         for (int i = 0; i < 4; ++i)
            REQUIRE(test[i] == r[i]);
      }
   }
}