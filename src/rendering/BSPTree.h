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

        void recursiveWalk(const BSPTreeNode* current, const Point3& cameraPos, std::function<void(Face*)> visitor);

    public:
        BSPTree(std::vector<std::unique_ptr<Face>>& faces);

        RelPos classifyPoint(const Point3& pt, const Face* plane);

        RelPos classifyFace(const Face* face, const Face* plane);

        void walk(const Point3& cameraPos, std::function<void(Face*)> visitor);

        virtual ~BSPTree();

};

#endif // BSPTREE_H
