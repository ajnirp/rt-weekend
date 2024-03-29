#ifndef SPHERE_H
#define SPHERE_H

#include "material.h"

class sphere : public hitable {
public:
  sphere() {}
  sphere(const vec3& cen, float r, material* m_ptr) : center(cen), radius(r), mat_ptr(m_ptr) {}
  virtual bool hit(const ray& r, float tmin, float tmax, hit_record& rec) const;
  vec3 center;
  float radius;
  material* mat_ptr;
};

bool sphere::hit(const ray& r, float tmin, float tmax, hit_record& rec) const {
  vec3 oc = r.origin() - center;
  float a = dot(r.direction(), r.direction());
  float b = dot(oc, r.direction());
  float c = dot(oc, oc) - radius*radius;
  float discriminant = b*b - a*c;
  if (discriminant > 0) {
    float temp = (-b - sqrt(discriminant)) / a;
    if (temp < tmax && temp > tmin) {
      rec.t = temp;
      rec.p = r.point_at_parameter(rec.t);
      rec.normal = (rec.p - center) / radius;
      // Record the sphere's material into the hit record object.
      rec.mat_ptr = mat_ptr;
      return true;
    }
    temp = (-b + sqrt(discriminant)) / a;
    if (temp < tmax && temp > tmin) {
      rec.t = temp;
      rec.p = r.point_at_parameter(rec.t);
      rec.normal = (rec.p - center) / radius;
      // Record the sphere's material into the hit record object.
      rec.mat_ptr = mat_ptr;
      return true;
    }
  }
  return false;
}

#endif