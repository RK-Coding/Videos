//Author: RK Coding
//Description: A raytracer created for a video on the RK Coding Youtube Channel
//License: Contact Me

//Standard Libs
#include <iostream>
#include <math.h>
#include <vector>
#include <random>
#include <algorithm>

//SDL
#include <SDL.h>

//Globals
#define WIDTH 800
#define HEIGHT 600

#define SAMPLE_SIZE 32

struct Vec3 // Vector3 structure
{
public:
	float x, y, z;

	Vec3(float x, float y, float z) : x(x), y(y), z(z) {}

	Vec3 operator + (Vec3 o) { return Vec3(x+o.x, y+o.y, z+o.z); }
	Vec3 operator - (Vec3 o) { return Vec3(x-o.x, y-o.y, z-o.z); }
	Vec3 operator * (float d) { return Vec3(x*d, y*d, z*d); }
	Vec3 operator / (float d) { return Vec3(x/d, y/d, z/d); }
	float DotProduct(Vec3 o) { return (x*o.x+y*o.y+z*o.z); }
	Vec3 Normalize()
	{
		float mag = sqrtf(x*x+y*y+z*z);
		return Vec3(x/mag, y/mag, z/mag);
	}
	Vec3 ToColor()
	{
		float r, g, b;

		r = x;
		g = y;
		b = z;

		if (x < 0.0f) { r = 0.0f; }
		if (x > 255.0f) { r = 255.0f; }
		if (y < 0.0f) { g = 0.0f; }
		if (y > 255.0f) { g = 255.0f; }
		if (z < 0.0f) { b = 0.0f; }
		if (z > 255.0f) { b = 255.0f; }

		return Vec3(r, g, b);
	}
};

struct Light 
{
	Vec3 pos, color;

	Light(Vec3 pos, Vec3 color) : pos(pos), color(color) {}
};

struct Ray
{
public:
	Vec3 origin, dir;

	Ray(Vec3 origin, Vec3 dir) : origin(origin), dir(dir) {}
};

class Sphere
{
public:
	Vec3 center;
	float radius;
	Vec3 color;
	float ambient, diffuse, specular;

	Sphere(Vec3 center, float radius, Vec3 color, float ambient, float diffuse, float specular) : center(center), radius(radius), color(color), ambient(ambient), diffuse(diffuse), specular(specular) {}

	float Intersects(Ray ray)
	{
		Vec3 sphere_to_ray = ray.origin - center;
		float b = 2 * ray.dir.DotProduct(sphere_to_ray);
		float c = sphere_to_ray.DotProduct(sphere_to_ray) - radius * radius;

		float disc = b * b - 4 * c;

		if (disc >= 0)
		{
			float dist = (-b - sqrtf(disc)) / 2;

			if (dist > 0)
			{
				return dist;
			}
		}

		return -1;
	}

	Vec3 NormalAt(Vec3 hit_point) { return (hit_point-center).Normalize(); }
};

class Window //Window Class
{
public:
	bool running = false;
	SDL_Event event;

	SDL_Window* window;
	SDL_Renderer* renderer;
	
	SDL_Texture* texture;
	Uint32* pixels;

	int width, height;
	
	Window(int width, int height, const char* title) : width(width), height(height)
	{
		running = true;

		window = SDL_CreateWindow(title, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, 0);
		renderer = SDL_CreateRenderer(window, -1, 0);
		
		texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STATIC, width, height);
		pixels = new Uint32[width * height];
	}

	void Update()
	{
		SDL_UpdateTexture(texture, NULL, pixels, width * sizeof(Uint32));

		SDL_WaitEvent(&event);

		switch (event.type)
		{
		case SDL_QUIT:
			running = false;
			break;
		}

		SDL_RenderClear(renderer);
		SDL_RenderCopy(renderer, texture, NULL, NULL);
		SDL_RenderPresent(renderer);
	}

	void Destroy()
	{
		delete[] pixels;
		SDL_DestroyWindow(window);
		SDL_DestroyRenderer(renderer);
		SDL_DestroyTexture(texture);
		SDL_Quit;
	}

	void SetPixel(int x, int y, Vec3 color_vec)
	{
		Vec3 to_color = color_vec.ToColor();
		Uint32 color = ((int)to_color.x << 16) + ((int)to_color.y << 8) + ((int)to_color.z);
		pixels[y * width + x] = color;
	}
};

int main(int argc, char *argv[])
{
	//Data
	SDL_Init(SDL_INIT_EVERYTHING);
	
	Window win(WIDTH, HEIGHT, "Raytracer by RK Coding");
	std::vector<Sphere> objects = {
		//Walls
		Sphere(Vec3(-1100.0f, 300.0f, 1000.0f), 1300.0f, Vec3(255.0f, 0.0f, 0.0f), 0.5f, 0.5f, 0.0f),
		Sphere(Vec3(400.0f, -900.0f, 1500.0f), 1300.0f, Vec3(0.0f, 255.0f, 0.0f), 0.5f, 0.5f, 0.0f),
		Sphere(Vec3(400.0f, 300.0f, 2000.0f), 1300.0f, Vec3(40.0f, 40.0f, 40.0f), 0.5f, 0.5f, 0.0f),
		Sphere(Vec3(1900.0f, 300.0f, 1000.0f), 1300.0f, Vec3(0.0f, 0.0f, 255.0f), 0.5f, 0.5f, 0.0f),
		Sphere(Vec3(400.0f, 1500.0f, 1500.0f), 1300.0f, Vec3(0.0f, 255.0f, 0.0f), 0.5f, 0.5f, 0.0f),
	
		//Sphere
		Sphere(Vec3(250.0f, 300.0f, 150.0f), 120.0f, Vec3(255.0f, 0.0f, 0.0f), 0.5f, 0.5f, 0.0f)
	};

	std::vector<Light> lights = { Light(Vec3(250.0f, 250.0f, -500.0f), Vec3(255.0f, 255.0f, 255.0f)) };

	std::default_random_engine generator;
	std::uniform_real_distribution<float> distribution(0.0, 1.0);

	for (int x = 0; x < win.width; x++) //Loop through every pixel
	{
		for (int y = 0; y < win.height; y++)
		{
			Vec3 pixel_color(0.0f, 0.0f, 0.0f);
			std::vector<Vec3> colors;

			for (int i = 0; i < SAMPLE_SIZE; i++) //Use 32 samples
			{
				float distance = 10000.0f;
				Sphere selected_object(Vec3(0.0f, 0.0f, 0.0f), 0.0f, Vec3(0.0f, 0.0f, 0.0f), 0.0f, 0.0f, 0.0f);
				bool object_hit = false;

				//Ray offset by uniform
				Ray ray(Vec3(x + distribution(generator), y + distribution(generator), 0.0f), Vec3(0.0f, 0.0f, 1.0f));

				for (Sphere object : objects) //Test intersections
				{
					if (object.Intersects(ray) > 0)
					{
						if (object.Intersects(ray) < distance)
						{
							distance = object.Intersects(ray);
							selected_object = object;
							object_hit = true;
						}
					}
				}

				if (object_hit)
				{
					//Ambient
					Vec3 object_color = selected_object.color * selected_object.ambient;
					Vec3 hit_pos = ray.origin + ray.dir * distance;
					Vec3 hit_normal = selected_object.NormalAt(hit_pos);
					Vec3 to_cam = Vec3(0.0f, 0.0f, 0.0f) - hit_pos;

					for (Light light : lights)
					{
						//Diffuse
						object_color = object_color + (selected_object.color * selected_object.diffuse * std::max(hit_normal.DotProduct((light.pos - hit_pos).Normalize()), 0.0f));
					
						//Specular
						Vec3 half_vec = ((light.pos - hit_pos).Normalize() + to_cam).Normalize();
						object_color = object_color + (light.color * selected_object.specular * pow(std::max(hit_normal.DotProduct(half_vec), 0.0f), 50));
					}
					
					//object_color = selected_object.color * selected_object.ambient / (distance * 0.02); //(RK Coding Shading Method!!, Thought of by me!!)
					colors.push_back(object_color);
				}
			}

			for (Vec3 color : colors)
			{
				pixel_color = pixel_color + color;
			}

			pixel_color = pixel_color / SAMPLE_SIZE;

			win.SetPixel(x, y, pixel_color);
		}
	}

	while (win.running)
	{
		win.Update();
	}

	win.Destroy();

	return 0;
}