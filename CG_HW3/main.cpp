#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <vector>

#include <windows.h>
#include <GL/freeglut.h>

#include "ray.h"
#include "vec3.h"
#include "geo.h"

#define STBI_MSC_SECURE_CRT
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

float MAX(float a, float b) {
	return (a > b) ? a : b;
}
using namespace std;
int max_step = 5;

GLbyte* pImage = NULL, * image = NULL;
/* Change the width and height for the final result*/
GLint iWidth = 1280, iHeight = 720, iComponents;
GLenum eFormat;

void RenderScene() {

	glClear(GL_COLOR_BUFFER_BIT);
	glRasterPos2i(0, 0);
	if (image != NULL) {
		if (pImage == NULL)
			pImage = image;
		glDrawPixels(iWidth, iHeight, GL_RGB, GL_UNSIGNED_BYTE, pImage);
	}
	glutSwapBuffers();
}


void init(GLbyte* pixels) {
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	pImage = image = pixels;
}

void finish(void) {
	if (pImage != image) {
		free(pImage);
	}
	free(image);
}

void ChangeSize(int w, int h) {
	if (h == 0)
		h = 1;

	glViewport(0, 0, w, h);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	gluOrtho2D(0.0f, (GLfloat)w, 0.0, (GLfloat)h);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

vec3 skybox(const ray& r) {
	vec3 uni_direction = unit_vector(r.direction());
	float t = 0.5 * (uni_direction.y() + 1);
	return (1.0 - t) * vec3(1, 1, 1) + t * vec3(0.5, 0.7, 1.0);
}

vec3 shading(vec3 &lightsource, vec3 &intensity, hit_record ht, vec3 kd, const vector<sphere> &list) {
	/*
	To-do:
		define L, N by yourself
	*/

	vec3 L = unit_vector(lightsource - ht.p);
	vec3 N = unit_vector(ht.nv);

	ray shadowRay(ht.p, L);

	int intersect = -1;
	hit_record rec;
	float closest = FLT_MAX;
	float darkest = 1;
	/*
	To-do:
		To find whether the shadowRay hit other objects,
		you should run the function "hit" of all the hitable you created
	*/

	for (int i = 0; i < list.size(); i++) {
		if (list[i].hit(shadowRay, 0.01, 1000, rec)) {

			float t_near = rec.t;

			if (t_near < closest) {
				closest = t_near;
				intersect = i;
			}

			float darkness = (1 + dot(L, rec.nv)) / 2;

			if (darkness < darkest) {
				darkest = darkness;
			}
		}		
	}

	vec3 original_color = kd * intensity * MAX(0, dot(N, L));

	if (intersect == -1) {
		return original_color;
	} else {
		return original_color * darkest;
	}
}

vec3 trace(const ray&r, const vector<sphere> &list, int depth) {
	if (depth >= max_step) return skybox(r); // or return vec3(0,0,0);

	int intersect = -1;
	hit_record rec, closest_rec;
	float closest = FLT_MAX;

	/*
	To-do:
		To find the nearest object from the origin of the ray,
		you should run the function "hit" of all the hitable you created
	*/
	
	for (int i = 0; i < list.size(); i++) {
		if (list[i].hit(r, 0.01, 100, rec)) {

			float t_near = rec.t;

			if (t_near < closest) {
				closest = t_near;
				intersect = i;
				closest_rec = rec;
			}
		}
	}

	if (intersect != -1) {

		vec3 lightPosition = vec3(-10, 10, 0);
		vec3 lightIntensity = vec3(1, 1, 1);

		vec3 kd = list[intersect].kd;
		float w_r = list[intersect].w_r;
		float w_t = list[intersect].w_t;

		/*
		To-do:
			1.compute the diffuse color by shading function
			2.compute the reflected color by
				2.1 compute the reflected direction
				2.2 define a reflected ray by rec.p and the direction in 2.1
				2.3 run trace(reflected_ray,list,depth+1);
			3.compute the transmitted(refracted) color by
				3.1 compute the transmitted direction by Snell's law
				3.2 define a transmitted ray by rec.p and the direction in 3.1
				3.3 run trace( transmitted_ray, list, depth + 1 );
			4.return the color by the parameter w_r, w_t and the 3 color you computed.
		*/

		vec3 diffuse = shading(lightPosition, lightIntensity, closest_rec, kd, list);

		ray reflected_ray(closest_rec.p, reflect(r.direction(), closest_rec.nv));
		vec3 reflected = trace(reflected_ray, list, depth + 1);

		ray transmitted_ray(closest_rec.p, refract(r.direction(), closest_rec.nv, 2));
		vec3 transmitted = trace(transmitted_ray, list, depth + 1);

		vec3 color = ((1 - w_t) * ((1 - w_r) * diffuse + w_r * reflected)) + (w_t * transmitted);

		return color;

	} else {
		return skybox(r);
	}
}

int main(int argc, char* argv[]) {
	// You should change the resolution when you hand in your result
	int width = int(iWidth);
	int height = int(iHeight);
	GLbyte* data = new GLbyte[width * height * 3];
	uint8_t* pixels = new uint8_t[width * height * 3];
	srand(time(NULL));

	// camera and projection plane
	vec3 lower_left_corner(-4, -2.25, -1);
	vec3 origin(0, 0, 0);
	vec3 horizontal(8, 0, 0);
	vec3 vertical(0, 4.5, 0);

	vec3 colorlist[8] = {vec3(0.8, 0.3, 0.3), vec3(0.3, 0.8, 0.3), vec3(0.3, 0.3, 0.8),
		vec3(0.8, 0.8, 0.3), vec3(0.3, 0.8, 0.8), vec3(0.8, 0.3, 0.8),
		vec3(0.8, 0.8, 0.8), vec3(0.3, 0.3, 0.3)};

	//test scene with spheres
	vector<sphere> hitable_list;
	hitable_list.push_back(sphere(vec3(0, -100.5, -2), 100)); //ground default color (1.0, 1.0, 1.0)
	hitable_list.push_back(sphere(vec3(0, 0, -2), 0.5, vec3(1.0f, 1.0f, 1.0f), 0.0f, 0.9f)); // refracted ball
	hitable_list.push_back(sphere(vec3(1, 0, -1.75), 0.5, vec3(1.0f, 1.0f, 1.0f), 0.9f, 0.0f)); // reflected ball
	hitable_list.push_back(sphere(vec3(-1, 0, -2.25), 0.5, vec3(1.0f, 0.7f, 0.3f), 0.0f, 0.0f)); // diffuse ball

	for (int i = 0; i < 48; i++) {
		float xr = ((float)rand() / (float)(RAND_MAX)) * 3.0f - 1.5f;
		float zr = ((float)rand() / (float)(RAND_MAX)) * 3.0f - 1.5f;
		int cindex = rand() % 8;
		float rand_reflec = ((float)rand() / (float)(RAND_MAX));
		float rand_refrac = ((float)rand() / (float)(RAND_MAX));
		switch (rand() % 3) {
			case 0:
				hitable_list.push_back(sphere(vec3(xr, -0.4, zr - 2), 0.1, colorlist[cindex], rand_reflec, 0.0f)); // small balls is a reflected ray.
				break;
			case 1:
				hitable_list.push_back(sphere(vec3(xr, -0.4, zr - 2), 0.1, colorlist[cindex], 0.0f, rand_refrac)); // small ball is a refracted ray.
				break;
			case 2:
				hitable_list.push_back(sphere(vec3(xr, -0.4, zr - 2), 0.1, colorlist[cindex], 0.0f, 0.0f)); // small ball is neither reflected nor refracted.
				break;
		}
	}
	
	/*
		To-do:
			Save the result to ppm and bmp/jpg/png format
	*/
	
	fstream file;
	file.open("../output.ppm", ios::out);
	file << "P3\n" << width << " " << height << "\n255\n";

	for (int j = height - 1; j >= 0; j--) {
		for (int i = 0; i < width; i++) {
			float u = float(i) / float(width);
			float v = float(j) / float(height);

			ray r(origin, lower_left_corner + u * horizontal + v * vertical);
			vec3 color = trace(r, hitable_list, 0);

			/* Hint: Here to save each pixel after ray tracing  */

			file << int(color.r() * 255) << " " << int(color.g() * 255) << " " << int(color.b() * 255) << "\n";

			// for display window
			int index = ((j) * width + i) * 3;
			data[index + 0] = (GLbyte)(color.r() * 255);
			data[index + 1] = (GLbyte)(color.g() * 255);
			data[index + 2] = (GLbyte)(color.b() * 255);

			int index_for_pixels = ((height - j) * width + i) * 3;
			pixels[index_for_pixels + 0] = (uint8_t)(color.r() * 255);
			pixels[index_for_pixels + 1] = (uint8_t)(color.g() * 255);
			pixels[index_for_pixels + 2] = (uint8_t)(color.b() * 255);
		}
	}

	stbi_write_png("output.png", width, height, 3, pixels, width * 3);

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GL_DOUBLE);
	glutInitWindowSize(width, height);
	glutCreateWindow("Image Loading Test");

	glutReshapeFunc(ChangeSize);
	glutDisplayFunc(RenderScene);
	init(data);
	glutMainLoop();
	finish();

	return 0;
}