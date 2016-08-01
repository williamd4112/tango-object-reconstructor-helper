#include <cstdio>
#include <cstdlib>
#include <string>
#include <iostream>
#include <vector>

using namespace std;

struct Point3f
{
	float x,y,z;
	
	Point3f(float x, float y, float z) : x(x), y(y), z(z) {}
	Point3f() {}
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


static unsigned int fread_chunk(char *buffer, FILE *fp, size_t chunk_size)
{
	return fread(buffer, chunk_size, 1, fp);
}

static void parse_point3f(vector<Point3f> & point_buffer, char * src, size_t begin)
{
	char x[4] = {src[begin + 3], src[begin + 2], src[begin + 1], src[begin + 0]};
	char y[4] = {src[begin + 7], src[begin + 6], src[begin + 5], src[begin + 4]};
	char z[4] = {src[begin + 11], src[begin + 10], src[begin + 9], src[begin + 8]};
			
	float fx = *(float*)x;	
	float fy = *(float*)y;	
	float fz = *(float*)z;
	
	if (fx != 0 || fy != 0 || fz != 0)
		point_buffer.push_back(Point3f(fx, fy, fz));
}

static void parse_point3f_chunk(vector<Point3f> & point_buffer, char * src, size_t begin, size_t end)
{
	while (begin < end) {
		parse_point3f(point_buffer, src, begin);
		begin += sizeof(Point3f);
	}
}

static bool read_raw_pointcloud(
	const char *filename, 
	vector<Point3f> & points, 
	size_t chunk_size, 
	unsigned int chunk_begin, 
	unsigned int chunk_end,
	unsigned int chunk_offset_begin,
	unsigned int chunk_offset_end
	)
{
	FILE * fin = fopen(filename, "rb");
	if (fin != NULL) {
		char *chunk_buffer = new char[chunk_size];
		memset(chunk_buffer, 0, chunk_size);

		// How many chunk read
		unsigned int chunk = 0;
		
		// Seek to start row
		fseek(fin, chunk_begin * chunk_size, SEEK_SET);
		while (chunk < chunk_end && fread_chunk(chunk_buffer, fin, chunk_size)) {
			parse_point3f_chunk(points, chunk_buffer, chunk_offset_begin, chunk_offset_end);
			chunk++;
		}
		fclose(fin);
		return true;
	}
	else {
		fprintf(stderr, "Read error\n");
		return false;
	}
}

static void write_pcd(const char *filename, vector<Point3f> & points)
{
	FILE *fout = fopen(filename, "w");
	if(fout != NULL)
	{	
		writePCDHeader(fout, points.size(), points.size(), 1);
		for(int i = 0; i < points.size(); i++)
		{
			Point3f & p = points[i];
			fprintf(fout, "%f %f %f\n", p.x, p.y, p.z);
		}
		fclose(fout);
	}
	else
	{
		fprintf(stderr, "Write error");
	}
}

int main(int argc, char *argv[])
{
	vector<Point3f> points;
	
	unsigned int w, h;
	sscanf(argv[2], "%u", &w);
	sscanf(argv[3], "%u", &h);

	// (x1, y1) : left top
	// (x2, y2) : right bottom
	unsigned int x1, y1, x2, y2;
	sscanf(argv[4], "%u", &x1);
	sscanf(argv[5], "%u", &y1);
	sscanf(argv[6], "%u", &x2);
	sscanf(argv[7], "%u", &y2);
	
	// chunk_size : a row of pointcloud data
	// chunk_offset_begin/end : col offset
	// chunk_begin/end : row offset
	size_t chunk_size = w * sizeof(Point3f);
	unsigned int chunk_begin = y1;
	unsigned int chunk_end = y2;
	unsigned int chunk_offset_begin = x1 * sizeof(Point3f);
	unsigned int chunk_offset_end = x2 * sizeof(Point3f);
	
	read_raw_pointcloud(argv[1], points, chunk_size, chunk_begin, chunk_end, chunk_offset_begin, chunk_offset_end);
	write_pcd(argv[8], points);
	

	return 0;
}
