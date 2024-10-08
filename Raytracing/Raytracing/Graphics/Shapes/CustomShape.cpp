#define GLM_ENABLE_EXPERIMENTAL
#include "CustomShape.h"
#include <iostream>
#include <limits>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/intersect.hpp>
#define TINYOBJLOADER_IMPLEMENTATION
#include "../../tiny_obj_loader.h"

namespace Graphics {
    namespace Shapes {
        // ------------------------------------------------------------------------
        /*! Default Constructor
         *
         *
         */ // ---------------------------------------------------------------------
        CustomShape::CustomShape(const std::string& objFilePath) {
            LoadMesh(objFilePath);
        }

        // ------------------------------------------------------------------------
        /*! Destructor
         *
         *
         */ // ---------------------------------------------------------------------
        CustomShape::~CustomShape() {

        }

        // ------------------------------------------------------------------------
        /*! LoadMesh
         *
         *   Loads the .obj file using tinyobjloader.
         */ // ---------------------------------------------------------------------
        bool CustomShape::LoadMesh(const std::string& objFilePath) {
            tinyobj::ObjReader reader;
            if (!reader.ParseFromFile(objFilePath)) {
                if (!reader.Error().empty()) {
                    std::cerr << "TinyObjReader: " << reader.Error() << std::endl;
                }
                return false;
            }

            if (!reader.Warning().empty()) {
                std::cout << "TinyObjReader: " << reader.Warning() << std::endl;
            }

            const tinyobj::attrib_t& attrib = reader.GetAttrib();
            const std::vector<tinyobj::shape_t>& shapes = reader.GetShapes();
            const std::vector<tinyobj::material_t>& materials = reader.GetMaterials();

            // Loop over shapes and get vertices and indices
            for (const auto& shape : shapes) {
                for (const auto& index : shape.mesh.indices) {
                    // Get the vertex position using the index from the OBJ file
                    glm::dvec3 vertex = glm::dvec3(
                        attrib.vertices[3 * index.vertex_index + 0],
                        attrib.vertices[3 * index.vertex_index + 1],
                        attrib.vertices[3 * index.vertex_index + 2]
                    );
                    mVertices.push_back(vertex);
                }

                // Get triangle indices (in sets of 3) from the actual indices array
                for (size_t f = 0; f < shape.mesh.indices.size(); f += 3) {
                    glm::ivec3 tri = glm::ivec3(
                        shape.mesh.indices[f].vertex_index,
                        shape.mesh.indices[f + 1].vertex_index,
                        shape.mesh.indices[f + 2].vertex_index
                    );
                    mIndices.push_back(tri);
                }
            }

            return true;
        }

        // ------------------------------------------------------------------------
        /*! Test Intersection
         *
         *   Tests whether a ray intersects with any triangle in the mesh.
         */ // ---------------------------------------------------------------------
        Trace::Hit CustomShape::TestIntersection(const Trace::Ray& ray, glm::dvec3& inpoint, glm::dvec3& innormal, glm::dvec3& outcolor) noexcept {

            // Transform the ray into the object's space.
            Trace::Ray newRay = mTransform.InverseTransformRay(ray);

            Trace::Hit hit(false);
            double closestT = std::numeric_limits<double>::max();

            // Loop over triangles and perform ray-triangle intersection test
            for (const auto& tri : mIndices) {
                glm::dvec3 v0 = mVertices[tri.x];
                glm::dvec3 v1 = mVertices[tri.y];
                glm::dvec3 v2 = mVertices[tri.z];

                glm::dvec2 baryPosition; // Holds the barycentric coordinates
                double t;

                // Use glm::intersectRayTriangle for intersection test
                if (glm::intersectRayTriangle(newRay.GetOrigin(), glm::normalize(newRay.GetEndPoint() - newRay.GetOrigin()), v0, v1, v2, baryPosition, t)) {
                    if (t < closestT && t > 0.0) {  // Check for closest intersection in front of the camera
                        closestT = t;
                        hit.hit = true;

                        // Calculate intersection point and normal
                        inpoint = mTransform.ApplyTransform(newRay.GetOrigin() + (glm::normalize(newRay.GetEndPoint() - newRay.GetOrigin()) * t));
                        innormal = glm::normalize(glm::cross(v1 - v0, v2 - v0)); // Normal is perpendicular to the triangle surface

                        // Assign the color from the material (here we use a default color, modify to get real materials)
                        outcolor = glm::vec4(1, 1, 1, 1);  // Modify this to use real material colors if needed
                    }
                }
            }

            return hit;
        }
    }
}
