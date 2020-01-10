#ifndef __MODEL_HH__
#define __MODEL_HH__

#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <iostream>

#include "geometry.hh"

class Model {
    private:
        std::vector<Vec3f> verts_;
        std::vector<std::vector<int>> faces_;

    public:
        Model(const char *filename);
        ~Model();
        
        int nverts();
        int nfaces();
        
        Vec3f vert(int i);
        std::vector<int> face(int idx);
};

#endif //__MODEL_HH__