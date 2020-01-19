#include "model.hh"

Model::Model(const char *filename)
    : pos_()    // v
    , uv_()     // vt
    , normal_() // vn
    , faces_()  // f
    , diffuse_map_() {
    std::ifstream in;
    in.open(filename, std::ifstream::in);
    if (in.fail())
        return;
    
    std::string line;
    while (!in.eof()) {
        std::getline(in, line);
        std::istringstream iss(line.c_str());
        char trash;
        if (!line.compare(0, 2, "v ")) {
            iss >> trash;
            Vec3f v;
            for (int i = 0; i < 3; ++i)
                iss >> v[i];
            pos_.push_back(v);
        } else if (!line.compare(0, 3, "vt ")) {
            iss >> trash >> trash;
            Vec2f vt;
            for (int i = 0; i < 2; ++i)
                iss >> vt[i];
            uv_.push_back(vt);
        } else if (!line.compare(0, 3, "vn ")) {
            iss >> trash >> trash;
            Vec3f vn;
            for (int i = 0; i < 3; ++i)
                iss >> vn[i];
            normal_.push_back(vn);
        } else if (!line.compare(0, 2, "f ")) {
            std::vector<Vec3i> f;
            Vec3i tmp;
            iss >> trash;
            while (iss >> tmp[0] >> trash >> tmp[1] >> trash >> tmp[2]) {
                for (int i = 0; i < 3; ++i)
                    tmp.raw[i]--; // in wavefront obj all indices start at 1, not zero
                f.push_back(tmp);
            }
            faces_.push_back(f);
        }
    }

    std::cerr << "# v# " << pos_.size()
              << " f# "  << faces_.size()
              << " vt# " << uv_.size()
              << " vn# " << normal_.size() << std::endl;
    load_texture(filename, "_diffuse.tga", diffuse_map_);
}

Model::~Model() { }

void Model::load_texture(std::string filename, const char *suffix, 
                         TGAImage &image) {
    std::string texfile(filename);
    size_t dot = texfile.find_last_of(".");
    if (dot != std::string::npos) {
        texfile = texfile.substr(0, dot) + std::string(suffix);
        std::cerr << "texture file " << texfile << " loading "
                  << (image.read_tga_file(texfile.c_str()) ? "ok" : "failed") << std::endl;
        image.flip_vertically();
    }
}

int Model::n_of_vertices() {
    return static_cast<int>(pos_.size());
}

int Model::n_of_faces() {
    return static_cast<int>(faces_.size());
}

Vec3f Model::vertex(int i) {
    return pos_[i];
}

Vec3f Model::vertex(int iface, int nthvert) {
    int idx = faces_[iface][nthvert].ivertex;
    return pos_[idx];
}

Vec2f Model::uv(int iface, int nthvert) {
    int idx = faces_[iface][nthvert].iuv;
    return Vec2f(uv_[idx].x * diffuse_map_.get_width(),
                 uv_[idx].y * diffuse_map_.get_height());
}

Vec3f Model::normal(int iface, int nthvert) {
    int idx = faces_[iface][nthvert].inormal;
    return normal_[idx].normalize();
}

// Vec3f Model::normal(Vec2f uv) {
//     uv.x *= normal_map_.get_width();
//     uv.y *= normal_map_.get_height();
//     TGAColor color = normal_map_.get(uv.x, uv.y);
//     return 2 * (Vec3f(color[2], color[1], color[0]) / 255) - 1;
// }

std::vector<int> Model::face(int idx) {
    std::vector<int> face;
    for (int i = 0; i < static_cast<int>(faces_[idx].size()); ++i)
        face.push_back(faces_[idx][i].ivertex);
    return face;
}

TGAColor Model::diffuse(Vec2f uv) {
    uv.x *= diffuse_map_.get_width();
    uv.y *= diffuse_map_.get_height();
    return diffuse_map_.get(uv.x, uv.y);
}