#ifndef __MODEL_H__
#define __MODEL_H__
#include <vector>
#include <string>

#include "tgaimage.hh"

#include "Types.hh"

class Model {
private:
	std::vector<Types::Vec3f> verts_;
	std::vector<std::vector<int> > faces_;
public:
	Model(const char *filename);
	~Model();
	int nverts();
	int nfaces();
	Types::Vec3f vert(int i);
	std::vector<int> face(int idx);
};

#endif //__MODEL_H__