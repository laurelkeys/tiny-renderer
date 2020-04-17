#include "Obj.hh"

#include <sstream>

using Types::Vec2i;
using Types::Vec3i;

using Types::Vec2f;
using Types::Vec3f;

namespace Primitives {

    ///////////////////////////////////////////////////////
    /// Face //////////////////////////////////////////////
    ///////////////////////////////////////////////////////

    Vertex &Face::operator[](unsigned int i) {
        switch (i) {
            case 0: return v0;
            case 1: return v1;
            case 2: return v2;
            default: // invalid index
                assert(i < 3);
                return v0;
        }
    }

    const Vertex &Face::operator[](unsigned int i) const {
        switch (i) {
            case 0: return v0;
            case 1: return v1;
            case 2: return v2;
            default: // invalid index
                assert(i < 3);
                return v0;
        }
    }
}