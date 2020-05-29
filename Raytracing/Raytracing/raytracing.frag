#version 430

in vec3 interpolated_vertex;
out vec4 FragColor;

struct Camera
{
	vec3 position;
	vec3 view;
	vec3 up;
	vec3 side;
};

struct Ray
{
	vec3 origin;
	vec3 direction;
};

struct Sphere
{
	vec3 center;
	float radius;
	vec3 color;
};

struct Material
{
	float ambient;
	float diffuse;
	float specular;
	float specularPower;
};

struct Intersection
{
	float time;
	vec3 point;
	vec3 normal;
	vec3 color;
};

const int countSpheres = 3;

Material material = {0.4, 0.9, 0.9, 32.0};
vec3 lightPos = vec3(0.0, 0.0, -4.0);

uniform Camera camera;
uniform vec2 scale;
uniform Sphere spherein[countSpheres];

bool IntersectSphere (Sphere sphere, Ray ray, float start, float final, out float time)
{
	ray.origin -= sphere.center;
	float A = dot (ray.direction, ray.direction);
	float B = dot (ray.direction, ray.origin);
	float C = dot (ray.origin, ray.origin) - sphere.radius * sphere.radius;
	float D = B * B - A * C;
	if (D > 0.0)
	{
		D = sqrt(D);
		float t1 = (-B - D) / A;
		float t2 = (-B + D) / A;
		
		if (t1 < 0 && t2 < 0) 
			return false;
		
		if (min(t1, t2) < 0)
		{
			time = max(t1, t2);
			return true;
		}
		
		time = min(t1, t2);
		
		return true;
	}
	
	return false;
};

bool Intersect(Ray ray, float start, float final, inout Intersection intersect)
{
	bool result = false;
	float time = start;
	intersect.time = final;
	
	for (int i = 0; i < countSpheres; i++)
	{
		if (IntersectSphere(spherein[i], ray, start, final, time) && time < intersect.time)
		{
			intersect.time = time;
			intersect.point = ray.origin + ray.direction * time;
			intersect.normal = normalize(intersect.point - spherein[i].center);
			intersect.color = spherein[i].color;
			result = true;
		}
	}
	return result;
};

vec3 Phong(Intersection intersect, float shadow)
{
	vec3 light = normalize(lightPos - intersect.point);
	float diffuse = max(dot(light, intersect.normal), 0.0);
	vec3 view = normalize(camera.position - intersect.point);
	vec3 reflected = -reflect(view, intersect.normal);
	float specular = pow(max(dot(reflected, light), 0.0), material.specularPower);
	
	return material.ambient * intersect.color + shadow * 
		   (material.diffuse * diffuse * intersect.color +
		   material.specular * specular * vec3(1,1,1));
};

float Shadow(vec3 point)
{
	float light = 1.0;
	vec3 direction = normalize(lightPos - point);
	Ray shadowRay = Ray(point + direction * 0.001, direction);
	Intersection intersect;
	
	intersect.time = distance(lightPos, point);
	
	if (Intersect(shadowRay, 0, intersect.time, intersect))
		light = 0.0;
	
	return light;
};

vec4 Raytrace(Ray primary_ray)
{
	vec4 resultColor = vec4(0, 0, 0, 0);
	Ray ray = primary_ray;
	
	Intersection intersect;
	intersect.time = 10000.0;
	float start = 0;
	float final = 10000.0;
	
	if (Intersect(ray, start, final, intersect))
	{
		float shadowing = Shadow(intersect.point);
		resultColor += vec4(Phong(intersect, shadowing), 0.0);
	}
	return resultColor;
};

Ray GenerateRay(Camera camera)
{
	vec2 coords = interpolated_vertex.xy * normalize(scale);
	vec3 direction = camera.view + camera.side * coords.x + camera.up * coords.y;
	return Ray(camera.position, normalize(direction));
};

void main(void){
	Ray ray = GenerateRay(camera);
	FragColor = Raytrace(ray);
};