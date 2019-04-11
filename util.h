#ifndef UTIL_H
#define UTIL_H

vec3 random_in_unit_sphere() {
  vec3 p;
  do {
    // random vec (x,y,z) such each component lies in [-1,1)
    p = 2.0*vec3(drand48(), drand48(), drand48()) - vec3(1,1,1);
  } while (p.squared_length() >= 1.0);
  return p;
}

#endif