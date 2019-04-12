#ifndef CAMERA_H
#define CAMERA_H

#include "ray.h"

class camera {
public:
  // vfov is top to bottom in degrees
  // vup = view up
  camera(const vec3& lookfrom, const vec3& lookat, const vec3& vup, float vfov, float aspect_ratio) {
    float theta = vfov*M_PI/180;
    float half_height = tan(theta/2);
    float half_width = aspect_ratio*half_height;
    origin = lookfrom;
    vec3 w = unit_vector(lookfrom-lookat); // the camera faces in the direction -w
    vec3 u = unit_vector(cross(vup, w));
    vec3 v = cross(w, u);
    lower_left_corner = origin - half_width*u - half_height*v - w;
    horizontal = 2*half_width*u;
    vertical = 2*half_height*v;
  }

  ray get_ray(float u, float v) {
    return ray(origin, lower_left_corner + u*horizontal + v*vertical - origin);
  }

  vec3 lower_left_corner; // of the screen
  vec3 horizontal;
  vec3 vertical;
  vec3 origin;
};

#endif