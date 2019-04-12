#include <iostream>
#include <float.h>

#include "camera.h"
#include "hitable.h"
#include "hitable_list.h"
#include "material.h"
#include "math.h"
#include "ray.h"
#include "sphere.h"
#include "util.h"
#include "vec3.h"

hitable* old_world() {
  hitable* list[5];
  list[0] = new sphere(vec3(0,0,-1), 0.5, new lambertian(vec3(0.1,0.2,0.5)));
  list[1] = new sphere(vec3(0,-100.5,-1), 100, new lambertian(vec3(0.8,0.8,0)));
  list[2] = new sphere(vec3(1,0,-1), 0.5, new metal(vec3(0.8,0.6,0.2), 0.3));
  list[3] = new sphere(vec3(-1,0,-1), 0.5, new dielectric(1.5));
  list[4] = new sphere(vec3(-1,0,-1), -0.45, new dielectric(1.5)); // negative radius = bubble
  return new hitable_list(list, 5);
}

hitable* random_scene() {
  int n =500;
  hitable** list = new hitable*[n+1];
  list[0] = new sphere(vec3(0,-1000,0), 1000, new lambertian(vec3(0.5,0.5,0.5)));
  int i = 1;
  for (int a = -11; a < 11; a++) {
    for (int b = -11; b < 11; b++) {
      float choose_mat = drand48();
      vec3 center(a+0.9*drand48(), 0.2, b+0.9*drand48());
      if ((center-vec3(4,0.2,0)).length() > 0.9) {
        if (choose_mat < 0.8) { // diffuse
          list[i++] = new sphere(center, 0.2, new lambertian(vec3(drand48()*drand48(), drand48()*drand48(), drand48()*drand48())));
        } else if (choose_mat < 0.95) { // metal
          vec3 albedo(0.5*(1+drand48()), 0.5*(1+drand48()), 0.5*(1+drand48()));
          float fuzz = 0.5*drand48();
          list[i++] = new sphere(center, 0.2, new metal(albedo, fuzz));
        } else { // glass
          list[i++] = new sphere(center, 0.2, new dielectric(1.5));
        }
      }
    }
  }

  list[i++] = new sphere(vec3(0,1,0), 1.0, new dielectric(1.5));
  list[i++] = new sphere(vec3(-4,1,0), 1.0, new lambertian(vec3(0.4,0.2,0.1)));
  list[i++] = new sphere(vec3(4,1,0), 1.0, new metal(vec3(0.7,0.6,0.5), 0.0));

  return new hitable_list(list, i);
}

vec3 color(const ray& r, hitable* world, int depth) {
  hit_record rec;
  if (world->hit(r, 0.001, FLT_MAX, rec)) {
    ray scattered;
    vec3 attentuation;
    if (depth < 50 && rec.mat_ptr->scatter(r, rec, attentuation, scattered)) {
      return attentuation*color(scattered, world, depth+1);
    } else {
      return vec3(0,0,0);
    }
  } else {
    vec3 unit_direction = unit_vector(r.direction());
    float t = 0.5*(unit_direction.y() + 1.0); // bring it into the range [0,1)
    return (1.0-t)*vec3(1.0,1.0,1.0) + t*vec3(0.5,0.7,1.0);
  }
}

int main() {
  int nx = 1200;
  int ny = 800;
  int ns = 100;
  std::cout << "P3\n" << nx << " " << ny << " " << "\n255\n";

  vec3 lookfrom(13,2,3);
  vec3 lookat(0,0,0);
  float dist_to_focus = 10.0;
  float aperture = 0.1;
  float vfov = 20.0;

  hitable* world = random_scene();

  camera cam(lookfrom, lookat, vec3(0,1,0), vfov, float(nx)/float(ny), aperture, dist_to_focus);

  for (int j = ny-1; j >= 0; j--) {
    for (int i = 0; i < nx; i++) {
      vec3 col(0,0,0);
      for (int s = 0; s < ns; s++) {
        float u = float(i + drand48()) / float(nx);
        float v = float(j + drand48()) / float(ny);
        ray r = cam.get_ray(u, v);
        col += color(r, world, 0);
      }
      col /= float(ns);
      col = vec3(sqrt(col[0]), sqrt(col[1]), sqrt(col[2]));
      int ir = int(255.99*col.r());
      int ig = int(255.99*col.g());
      int ib = int(255.99*col.b());
      std::cout << ir << " " << ig << " " << ib << "\n";
    }
  }
  return 0;
}