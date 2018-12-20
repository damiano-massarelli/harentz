#include "BSPTree.h"
#include <cmath>

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
        // Used in case of spanning
        std::unique_ptr<Face> frontFace;
        std::unique_ptr<Face> backFace;

        switch (classifyFace(face.get(), current->splitter.get())) {
        case RelPos::FRONT:
            frontList.push_back(std::move(face));
            break;
        case RelPos::BACK:
            backList.push_back(std::move(face));
            break;
        case RelPos::SPANNING:
            if (splitFace(face.get(), current->splitter.get(), frontFace, backFace)) {
                frontList.push_back(std::move(frontFace));
                backList.push_back(std::move(backFace));
            } else {
                frontList.push_back(std::move(face)); // False alarm, could not split for some reason
            }
            break;
        default:
            break;
        }
    }

    // Recursive steps
    current->frontChild = buildTree(frontList);
    current->backChild = buildTree(backList);
    return current;
}


RelPos BSPTree::classifyPoint(const Point3& pt, const Face* plane)
{
    Point3 center = plane->getCenter();
    Point3 direction = normalized(pt - center);
    float result = dot(direction, normalized(plane->getNormal()));
    if (result > 0.001f) // a bit of threshold, prevents float rounding errors
        return RelPos::FRONT;
    if (result < -0.001f)
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
    // vertices that are onPlane are not taken into account
    if (numFront + numOnPlane == numOfVertices)
        return RelPos::FRONT;
    if (numBack + numOnPlane == numOfVertices)
        return RelPos::BACK;
    return RelPos::SPANNING;
}

void BSPTree::recursiveWalk(const BSPTreeNode* current, const Point3& cameraPos, std::function<void(const Face*)>visitor)
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


void BSPTree::walk(const Point3& cameraPos, std::function<void(const Face*)> visitor)
{
    recursiveWalk(m_root.get(), cameraPos, visitor);
}

bool BSPTree::splitFace(const Face* face, const Face* plane, std::unique_ptr<Face>& outFront, std::unique_ptr<Face>& outBack)
{
    outFront = std::make_unique<Face>();
    outBack = std::make_unique<Face>();
    outFront->setFillColor(face->getFillColor()); // Copies the color of the original face
    outFront->setOutlineColor(face->getOutlineColor());
    outBack->setFillColor(face->getFillColor()); // Copies the color of the original face
    outBack->setOutlineColor(face->getOutlineColor());

    const std::vector<Point3>& vertices = face->getVertices();
    int numOfVertices = vertices.size();
    for (int i = 0; i < numOfVertices; i++) {
        /* Creates the starting and ending point of the edge.
         * (i+1)%numOfVertices is used to take into consideration the last edge: last vert. -> first vert. */
        const Point3& v1 = vertices[i];
        const Point3& v2 = vertices[(i+1) % numOfVertices];
        Point3 intersection;
        RelPos v1Pos = classifyPoint(v1, plane);
        RelPos v2Pos = classifyPoint(v2, plane);

        if (v1Pos == v2Pos) {
            if (v1Pos == RelPos::FRONT) { // both in the front face
                outFront->addVertex(v1);
                outFront->addVertex(v2);
            } else { // both back
                outBack->addVertex(v1);
                outBack->addVertex(v2);
            }
        } else { // try to split
            if (getIntersection(v1, v2, plane, intersection)) {
                outFront->addVertex(intersection);
                outBack->addVertex(intersection);
            }
        }
    }
    // The split is successful only if two valid faces are created.
    return outBack->getVertices().size() == 4 && outFront->getVertices().size() == 4;
}

bool BSPTree::getIntersection(const Point3& edgeStart, const Point3& edgeEnd, const Face* plane, Point3& outIntersection)
{
    // The direction of the ray passing through edgeStart and edgeEnd
    Point3 dir = normalized(edgeEnd - edgeStart);
    Point3 center = plane->getCenter();
    Point3 normal = normalized(plane->getNormal());
    float denominator = dot(dir, normal);
    if (std::abs(denominator) < 0.0001f) return false; // denominator is too little, better to avoid computation.
    float t = dot((center - edgeStart), normal) / denominator;
    outIntersection = edgeStart + dir * t;
    /* If the intersection occurs before the beginning of the edge (t < 0) or after the end of if (dot(outIntersection - edgeEnd, dir) >= 0),
       do not split */
    if (dot(outIntersection - edgeEnd, dir) >= 0 || t < 0.0f) return false; // the intersection occurs outside the edge
    return true;
}

BSPTree::~BSPTree()
{
    //dtor
}
