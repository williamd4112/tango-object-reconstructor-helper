#include <cstdio>
#include <cstdlib>
#include <string>
#include <iostream>
#include <vector>

using namespace std;

struct Point3f
{
	float x,y,z;
};

void writePCDHeader(FILE *fout, int count, int width, int height)
{
        fprintf(fout, "# .PCD v.7 - Point Cloud Data file format\n"
                "VERSION .7\n"
                "FIELDS x y z\n"
                "SIZE 4 4 4\n"
                "TYPE F F F\n"
                "COUNT 1 1 1\n"
                "WIDTH %d\n"
                "HEIGHT %d\n"
                "VIEWPOINT 0 0 0 1 0 0 0\n"
                "POINTS %d\n"
                "DATA ascii\n", width, height, count);
}


static unsigned int fread_chunk(char *buffer, FILE *fp, size_t begin, size_t end)
{
	return fread(buffer, end - begin, 1, fp);
}

static void parse_point3f(vector<Point3f> & point_buffer, char * src, size_t begin)
{
	char x[4] = {src[begin + 3], src[begin + 2], src[begin + 1], src[begin + 0]};
	char y[4] = {src[begin + 7], src[begin + 6], src[begin + 5], src[begin + 4]};
	char z[4] = {src[begin + 11], src[begin + 10], src[begin + 9], src[begin + 8]};
			
	float fx = *(float*)x;	
	float fy = *(float*)y;	
	float fz = *(float*)z;

	point_buffer.push_back(Point3f{fx, fy, fz});
}

static void parse_point3f_chunk(vector<Point3f> & point_buffer, char *src, size_t begin, size_t end)
{
	while (begin < end) {
		parse_point3f(point_buffer, src, begin);
		begin += sizeof(Point3f);
	}	
}

int main()
{
	vector<Point3f> points;
	FILE *fp = fopen("pointcloud.dat", "rb");
	if(fp != NULL)
	{
		printf("Read\n");
		char tmp[4 * 3];
		
		while(fread(tmp, 12, 1, fp))
		{
			char x[4] = {tmp[3], tmp[2], tmp[1], tmp[0]};
			char y[4] = {tmp[7], tmp[6], tmp[5], tmp[4]};
			char z[4] = {tmp[11], tmp[10], tmp[9], tmp[8]};
			
			float fx = *(float*)x;	
			float fy = *(float*)y;	
			float fz = *(float*)z;
			
			if(fx != 0 || fy != 0 || fz != 0)
			{
				printf("(%f, %f, %f)\n",fx,fy,fz);
				points.push_back(Point3f{fx, fy, fz});
			}
		}
	}
	
	
	FILE *fout = fopen("screenshot.pcd", "w");
	if(fout != NULL)
	{	
		writePCDHeader(fout, points.size(), points.size(), 1);
		for(Point3f p : points)
		{
			fprintf(fout, "%f %f %f\n", p.x, p.y, p.z);

		}
	}
	else
	{
		fprintf(stderr, "Write error");
	}
	return 0;
}
