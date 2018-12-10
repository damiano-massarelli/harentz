#include "BSPTree.h"

BSPTree::BSPTree(std::vector<std::unique_ptr<Face>>& faces)
{
    m_root = buildTree(faces);
}

std::unique_ptr<BSPTreeNode> BSPTree::buildTree(std::vector<std::unique_ptr<Face>>& faces)
{
    if (faces.size() == 0) return nullptr; // End of recursion

    std::unique_ptr<BSPTreeNode> current = std::make_unique<BSPTreeNode>();
    std::vector<std::unique_ptr<Face>> frontList;
    std::vector<std::unique_ptr<Face>> backList;

    current->splitter = std::move(faces[0]); // Select the first as splitter, pretty naive
    for (auto& face : faces) {
        if (face == nullptr) continue; // the first face is moved and therefore it is null
        switch (classifyFace(face.get(), current->splitter.get())) {
        case RelPos::FRONT:
            frontList.push_back(std::move(face));
            break;
        case RelPos::BACK:
            backList.push_back(std::move(face));
            break;
        case RelPos::SPANNING: /// TODO: should not be like this
            frontList.push_back(std::move(face));
            break;
        default:
            break;
        }
    }

    current->frontChild = buildTree(frontList);
    current->backChild = buildTree(backList);
    return current;
}


RelPos BSPTree::classifyPoint(const Point3& pt, const Face* plane)
{
    Point3 center = plane->getCenter();
    Point3 direction = pt - center;
    float result = dot(direction, plane->getNormal());
    if (result > 0.001)
        return RelPos::FRONT;
    if (result < -0.001)
        return RelPos::BACK;

    return RelPos::ONPLANE;
}

RelPos BSPTree::classifyFace(const Face* face, const Face* plane)
{
    int numFront{0};
    int numBack{0};
    int numOnPlane{0};
    int numOfVertices = face->getVertices().size();
    for (const auto& pt : face->getVertices()) {
        RelPos pos = classifyPoint(pt, plane);
        numFront += pos == RelPos::FRONT ? 1 : 0;
        numBack += pos == RelPos::BACK ? 1 : 0;
        numOnPlane += pos == RelPos::ONPLANE ? 1 : 0;
    }
    if (numFront == numOfVertices || numOnPlane == numOfVertices)
        return RelPos::FRONT;
    if (numBack == numOfVertices)
        return RelPos::BACK;
    return RelPos::SPANNING;
}

void BSPTree::recursiveWalk(const BSPTreeNode* current, const Point3& cameraPos, std::function<void(Face*)>visitor)
{
    if (current == nullptr) return;
    RelPos camPos = classifyPoint(cameraPos, current->splitter.get());
    if (camPos == RelPos::FRONT) {
        recursiveWalk(current->backChild.get(), cameraPos, visitor);
        visitor(current->splitter.get());
        recursiveWalk(current->frontChild.get(), cameraPos, visitor);
    } else {
        recursiveWalk(current->frontChild.get(), cameraPos, visitor);
        visitor(current->splitter.get());
        recursiveWalk(current->backChild.get(), cameraPos, visitor);
    }
}


void BSPTree::walk(const Point3& cameraPos, std::function<void(Face*)> visitor)
{
    recursiveWalk(m_root.get(), cameraPos, visitor);
}

BSPTree::~BSPTree()
{
    //dtor
}
