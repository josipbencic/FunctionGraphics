#ifndef INCLUDED_REAL_FUNCTIONS_H
#define INCLUDED_REAL_FUNCTIONS_H

#include <vector>

namespace func {

  struct Polynomial {
    std::vector<float> a;

    inline float operator() (float x) const {
      float t = 1;
      float ret = 0.0f;
      for (unsigned i = 0; i < deg(); i++) {
        ret += a[i] * t;
        t *= x;
      }
      return ret;
    }

    inline unsigned deg() const {
      return a.size();
    }
  };

  template <class Poly>
  inline unsigned deg(Poly p) { return p.deg(); }

  using Poly = Polynomial;
}

#endif
