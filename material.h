#ifndef MATERIAL_H
#define MATERIAL_H

#include "util.h"

class material {
public:
  virtual bool scatter(const ray& r_in, const hit_record& rec, vec3& attenutation, ray& scattered) const = 0;
};

class lambertian : public material {
public:
  lambertian(const vec3& a) : albedo(a) {}
  virtual bool scatter(const ray& r_in, const hit_record& rec, vec3& attenutation, ray& scattered) const {
    vec3 target = rec.p + rec.normal + random_in_unit_sphere();
    scattered = ray(rec.p, target - rec.p);
    attenutation = albedo;
    return true;
  }
  vec3 albedo;
};

vec3 reflect(const vec3& v, const vec3& n) {
  return v - 2*dot(v,n)*n;
}

class metal : public material {
public:
  metal(const vec3& a) : albedo(a) {}
  virtual bool scatter(const ray& r_in, const hit_record& rec, vec3& attenutation, ray& scattered) const {
    vec3 reflected = reflect(unit_vector(r_in.direction()), rec.normal);
    scattered = ray(rec.p, reflected);
    attenutation = albedo;
    return dot(scattered.direction(), rec.normal) > 0;
  }
  vec3 albedo;
};

#endif