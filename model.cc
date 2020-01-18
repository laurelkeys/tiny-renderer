#include "model.hh"

Model::Model(const char *filename)
    : verts_()
    , faces_()
    , norms_()
    , uv_()
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
                iss >> v.raw[i];
            verts_.push_back(v);
        } else if (!line.compare(0, 3, "vn ")) {
            iss >> trash >> trash;
            Vec3f n;
            for (int i = 0; i < 3; ++i)
                iss >> n.raw[i];
            norms_.push_back(n);
        } else if (!line.compare(0, 3, "vt ")) {
            iss >> trash >> trash;
            Vec2f uv;
            for (int i = 0; i < 2; ++i)
                iss >> uv.raw[i];
            uv_.push_back(uv);
        } else if (!line.compare(0, 2, "f ")) {
            std::vector<Vec3i> f;
            Vec3i tmp;
            iss >> trash;
            while (iss >> tmp.raw[0]
                   >> trash >> tmp.raw[1]
                   >> trash >> tmp.raw[2]) {
                for (int i = 0; i < 3; ++i)
                    tmp.raw[i]--; // in wavefront obj all indices start at 1, not zero
                f.push_back(tmp);
            }
            faces_.push_back(f);
        }
    }
    std::cerr << "# v# " << verts_.size()
              << " f# "  << faces_.size()
              << " vt# " << uv_.size()
              << " vn# " << norms_.size() << std::endl;
    load_texture(filename, "_diffuse.tga", diffuse_map_);
}

Model::~Model() { }

int Model::nverts() {
    return static_cast<int>(verts_.size());
}

int Model::nfaces() {
    return static_cast<int>(faces_.size());
}

std::vector<int> Model::face(int idx) {
    std::vector<int> face;
    for (int i = 0; i < static_cast<int>(faces_[idx].size()); ++i)
        face.push_back(faces_[idx][i].ivert);
    return face;
}

Vec3f Model::vert(int i) {
    return verts_[i];
}

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

TGAColor Model::diffuse(Vec2i uv) {
    return diffuse_map_.get(uv.x, uv.y);
}

Vec2i Model::uv(int iface, int nvert) {
    int idx = faces_[iface][nvert].iuv;
    return Vec2i(uv_[idx].x * diffuse_map_.get_width(),
                 uv_[idx].y * diffuse_map_.get_height());
}

Vec3f Model::normal(int iface, int nvert) {
    int idx = faces_[iface][nvert].inorm;
    return norms_[idx].normalize();
}