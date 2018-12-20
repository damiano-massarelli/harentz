#ifndef BSPTREE_H
#define BSPTREE_H
#include "Face.h"
#include "Point3.h"
#include <memory>
#include <functional>

enum class RelPos {
    FRONT, BACK, ONPLANE, SPANNING
};

struct BSPTreeNode {
    std::unique_ptr<Face> splitter;
    std::unique_ptr<BSPTreeNode> frontChild;
    std::unique_ptr<BSPTreeNode> backChild;
};


class BSPTree
{
    private:
        std::unique_ptr<BSPTreeNode> m_root;

        std::unique_ptr<BSPTreeNode> buildTree(std::vector<std::unique_ptr<Face>>& faces);

        /** \brief just a utility */
        void recursiveWalk(const BSPTreeNode* current, const Point3& cameraPos, std::function<void(const Face*)> visitor);


        bool splitFace(const Face* face, const Face* plane, std::unique_ptr<Face>& outFront, std::unique_ptr<Face>& outBack);

        /** \brief returns the intersection point between an edge and a plane
          * \param edgeStart the start of the edge
          * \param edgeEnd the end of the edge
          * \param plane the plane
          * \param outIntersection the intersection point (might be invalid)
          * \return true if an intersection was found, false otherwise. Do not use outIntersection when this function returns false */
        bool getIntersection(const Point3& edgeStart, const Point3& edgeEnd, const Face* plane, Point3& outIntersection);

        /** \brief Specify whether a point is in front of behind a plane (defined by a face)
          * \param pt the point
          * \param plane the plane
          * \return a RelPos object that specifies the position */
        RelPos classifyPoint(const Point3& pt, const Face* plane);

        RelPos classifyFace(const Face* face, const Face* plane);

    public:
        BSPTree(std::vector<std::unique_ptr<Face>>& faces);

        /** \brief goes through the tree calling the visitor function for each face
          * \param cameraPos the position of the camera used to render
          * \param visitor the function called for each face of the tree visited
          * The tree is visited from back to front for faces that are in front of the
          * camera and from front to back for those that are behind it. This prevents
          * overlaps and glitches from happening */
        void walk(const Point3& cameraPos, std::function<void(const Face*)> visitor);

        virtual ~BSPTree();

};

#endif // BSPTREE_H
