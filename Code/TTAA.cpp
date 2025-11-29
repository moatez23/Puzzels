#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>

using std::vector;
using std::cout;
using std::endl;

const int MATRIX_MAX = 17;
struct Point;
vector<Point> createShifts(Point P);
bool pointInTriangle (Point pt, Point v1, Point v2, Point v3);
enum class direction {
    UP, 
    RIGHT, 
    DOWN, 
    LEFT 
};

struct Point {
    float x;
    float y;
    Point(float X, float Y) : x(X), y(Y) {};
};

struct PossibleShifts {
    Point _dimensions; 
    vector<Point> _shifts;
    PossibleShifts(Point point, vector<Point> shifts) : _dimensions(point), _shifts(shifts) {}; 
};

class Triangle {
    private:

        int _xC;
        int _yC;
        int _area;
        direction _direction;
    
    public:

        vector<PossibleShifts> _combinations;
        vector<Point> _allTriangles; 

        Triangle(int area, int X, int Y, direction dir = direction::UP) : _xC(X), _yC(Y), _area(area), _direction(dir) {
            createDimensions(_area, _combinations);
            makeCombinations(_xC,_yC,_combinations,_allTriangles);
        };

        void createDimensions(int area, vector<PossibleShifts>& combinations);
        void makeCombinations(int X, int Y, vector<PossibleShifts>& combinations, vector<Point>& allTriangles);

        vector<Point> createShifts(Point dimensions);

        int getArea() const {return _area;};
        int getXC() const {return _xC;};
        void setXC(int x) {_xC = x;};
        int getYC() const {return _yC;};
        void setYC(int y) {_yC = y;};

        void printDimensions() const;
        void printTriangles() const;
        
        vector<PossibleShifts> getCombinations() {return _combinations;};
};

void Triangle::createDimensions(int area, vector<PossibleShifts>& combinations) {
    int effectiveArea = 2 * area;

    for (int base{2}; base < effectiveArea; ++base) {
        int height;
        if ((effectiveArea) % base == 0) {
            height = effectiveArea / base;
            combinations.push_back(PossibleShifts(Point((float)base, (float)height), createShifts(Point((float)base, (float)height))));
        }
    }
    return;
}

vector<Point> Triangle::createShifts(Point dimensions) {
    
    vector<Point> results;

    int shiftY{}, shiftX{};
    

    Point q1(1,1); 

    Point v1((float)shiftX,(float)shiftY); 
    Point v2(0, dimensions.y);
    Point v3(dimensions.x, 0);

    int maxHeight = (int)dimensions.y;
    while (pointInTriangle(q1,v1,v2,v3)) {
        shiftY = 0;
        v1.x = (float)shiftX;
        v1.y = (float)shiftY;
        while (pointInTriangle(q1,v1,v2,v3)) {
            results.push_back(Point((float)shiftX, (float)shiftY));
            shiftY--;
            v2.y--;
            v3.y--;
        }
        v2.y = (float)maxHeight;
        v2.x--;
        v3.y = 0;
        v3.x--;
        shiftX--;
    }
   return results;
}

void Triangle::printDimensions() const {
    for (const auto& j : _combinations) {
        cout << j._dimensions.x << " " << j._dimensions.y << " = ";
        for (int i{}; i < j._shifts.size(); i++) {
            cout << j._shifts[i].x << ' ' << j._shifts[i].y << ' ';
        }
        cout << "\n";
    }
    return;
}

void Triangle::makeCombinations(int X, int Y, vector<PossibleShifts>& combinations, vector<Point>& allTriangles) {
    int aX, aY, bX, bY, cX, cY;
    

    for (int i{}; i < combinations.size();i++) { 
        for (int j{}; j < combinations[i]._shifts.size();j++) {
        
            aX = X + (int)combinations[i]._shifts[j].x;
            aY = Y + (int)combinations[i]._shifts[j].y;
            bX = aX + (int)combinations[i]._dimensions.x;
            bY = aY;
            cX = aX;
            cY = aY + (int)combinations[i]._dimensions.y;

            if ( aX < 0 || aX > MATRIX_MAX || bX < 0 || bX > MATRIX_MAX ||cX < 0 || cX > MATRIX_MAX || 
                 aY < 0 || aY > MATRIX_MAX || bY < 0 || bY > MATRIX_MAX ||cY < 0 || cY > MATRIX_MAX) {
                continue;
            }

            allTriangles.push_back(Point((float)aX,(float)aY));
            allTriangles.push_back(Point((float)bX,(float)bY));
            allTriangles.push_back(Point((float)cX,(float)cY));
        }
    }


    for (int i{}; i < combinations.size();i++) {
        for (int j{}; j < combinations[i]._shifts.size();j++) {
    
            aX = X + (int)combinations[i]._shifts[j].y;
            aY = Y + std::abs((int)combinations[i]._shifts[j].x) + 1;
            bX = aX;
            bY = aY - (int)combinations[i]._dimensions.x;
            cX = aX + (int)combinations[i]._dimensions.y;
            cY = aY;

            if ( aX < 0 || aX > MATRIX_MAX || bX < 0 || bX > MATRIX_MAX ||cX < 0 || cX > MATRIX_MAX ||
                 aY < 0 || aY > MATRIX_MAX || bY < 0 || bY > MATRIX_MAX ||cY < 0 || cY > MATRIX_MAX) {
                continue;
            }

            allTriangles.push_back(Point((float)aX,(float)aY));
            allTriangles.push_back(Point((float)bX,(float)bY));
            allTriangles.push_back(Point((float)cX,(float)cY));
        }
    }


    for (int i{}; i < combinations.size(); ++i) { 
        for (int j{}; j < combinations[i]._shifts.size(); ++j) {

            aX = X + std::abs((int)combinations[i]._shifts[j].x) + 1;
            aY = Y + std::abs((int)combinations[i]._shifts[j].y) + 1;
            bX = aX - (int)combinations[i]._dimensions.x;
            bY = aY;
            cX = aX;
            cY = aY - (int)combinations[i]._dimensions.y;

            if ( aX < 0 || aX > MATRIX_MAX || bX < 0 || bX > MATRIX_MAX ||cX < 0 || cX > MATRIX_MAX || 
                 aY < 0 || aY > MATRIX_MAX || bY < 0 || bY > MATRIX_MAX ||cY < 0 || cY > MATRIX_MAX) {
                continue;
            }

            allTriangles.push_back(Point((float)aX,(float)aY));
            allTriangles.push_back(Point((float)bX,(float)bY));
            allTriangles.push_back(Point((float)cX,(float)cY));
        }
    }

    for (int i{}; i < combinations.size(); i++) { 
        for (int j{}; j < combinations[i]._shifts.size(); j++) {
        
            aX = X + std::abs((int)combinations[i]._shifts[j].y) + 1;
            aY = Y + (int)combinations[i]._shifts[j].x;
            bX = aX;
            bY = aY + (int)combinations[i]._dimensions.x;
            cX = aX - (int)combinations[i]._dimensions.y;
            cY = aY;

            if ( aX < 0 || aX > MATRIX_MAX || bX < 0 || bX > MATRIX_MAX ||cX < 0 || cX > MATRIX_MAX ||
                 aY < 0 || aY > MATRIX_MAX || bY < 0 || bY > MATRIX_MAX ||cY < 0 || cY > MATRIX_MAX) {
                continue;
            }

            allTriangles.push_back(Point((float)aX,(float)aY));
            allTriangles.push_back(Point((float)bX,(float)bY));
            allTriangles.push_back(Point((float)cX,(float)cY));
        }
    }

    return;
}

void Triangle::printTriangles() const {
    for (int i{}; i < _allTriangles.size();i++) {
        cout << "( ";
           cout << _allTriangles[i].x << " " << _allTriangles[i].y << ' ';
        cout << ") |=|";
        if ((i + 1) % 3 == 0) {
            cout << "\n";
        }
    }
    cout << "\n";
    return;
}

bool onSegment(Point p, Point q, Point r) { 
    if (q.x <= std::max(p.x, r.x) && q.x >= std::min(p.x, r.x) && 
        q.y <= std::max(p.y, r.y) && q.y >= std::min(p.y, r.y)) {
            return true; 
        }
    return false; 
} 
  
int orientation(Point p, Point q, Point r) { 

    float val = (q.y - p.y) * (r.x - q.x) - 
              (q.x - p.x) * (r.y - q.y); 
    
    if (std::abs(val) < 1e-9) return 0;  
    return (val > 0) ? 1 : 2; 
} 

bool doIntersect(Point p1, Point q1, Point p2, Point q2) { 
    int o1 = orientation(p1, q1, p2); 
    int o2 = orientation(p1, q1, q2); 
    int o3 = orientation(p2, q2, p1); 
    int o4 = orientation(p2, q2, q1); 
    if (o1 == 0 || o2 == 0 || o3 == 0 || o4 == 0) return false;

    if (o1 != o2 && o3 != o4) 
        return true; 


    if (o1 == 0 && onSegment(p1, p2, q1)) return false;  
  
    if (o2 == 0 && onSegment(p1, q2, q1)) return false; 

    if (o3 == 0 && onSegment(p2, p1, q2)) return false; 

    if (o4 == 0 && onSegment(p2, q1, q2)) return false; 
  
    return false;
} 

float sign (Point p1, Point p2, Point p3) {
    return (p1.x - p3.x) * (p2.y - p3.y) - (p2.x - p3.x) * (p1.y - p3.y);
}

bool pointInTriangle (Point pt, Point v1, Point v2, Point v3) {
    float d1, d2, d3;
    bool has_neg, has_pos;

    d1 = sign(pt, v1, v2);
    d2 = sign(pt, v2, v3);
    d3 = sign(pt, v3, v1);

    has_neg = (d1 < 0) || (d2 < 0) || (d3 < 0);
    has_pos = (d1 > 0) || (d2 > 0) || (d3 > 0);

    return !(has_neg && has_pos);
}

bool triangleIsContainedInOtherTriangle(vector<Point>& e, Point p, Point q, Point r) { 
    for (int i{}; i < e.size(); i += 3) {
        if (pointInTriangle(p,e[i],e[i+1],e[i+2]) && pointInTriangle(q,e[i],e[i+1],e[i+2]) && pointInTriangle(r,e[i],e[i+1],e[i+2])) return true;
    }
    return false;
}


bool triangleIsContainingOtherTriangle(vector<Point>& containingTriangleVertices, Point p, Point q, Point r) {
    for (int i{}; i < containingTriangleVertices.size(); i+=3) {
        Point vertexOne(containingTriangleVertices[i].x,containingTriangleVertices[i].y);
        Point vertexTwo(containingTriangleVertices[i+1].x, containingTriangleVertices[i+1].y);
        Point vertexThree(containingTriangleVertices[i+2].x, containingTriangleVertices[i+2].y);
        if (pointInTriangle(vertexOne, p, q, r) && pointInTriangle(vertexTwo, p, q, r) && pointInTriangle(vertexThree, p, q, r)) return true;
    }
    return false;
}

void preProcessValidTriangles(vector<Triangle>& WIN) {
    for (int i{}; i < WIN.size(); i++) {
        for (int j{}; j < WIN[i]._allTriangles.size(); j += 3) {
            Point p(WIN[i]._allTriangles[j].x, WIN[i]._allTriangles[j].y);
            Point q(WIN[i]._allTriangles[j+1].x, WIN[i]._allTriangles[j+1].y);
            Point r(WIN[i]._allTriangles[j+2].x, WIN[i]._allTriangles[j+2].y);
            for (int k{}; k < WIN.size(); k++) {
                if (k == i) continue;
                Point a((float)WIN[k].getXC(), (float)WIN[k].getYC());
                Point b((float)WIN[k].getXC(), (float)WIN[k].getYC() + 1);
                Point c((float)WIN[k].getXC() + 1, (float)WIN[k].getYC() + 1);
                Point d((float)WIN[k].getXC() + 1, (float)WIN[k].getYC());

                if (doIntersect(p, q, a, b)) {
                    WIN[i]._allTriangles.erase(WIN[i]._allTriangles.begin() + j, WIN[i]._allTriangles.begin() + j + 3);
                    break;
                }

                if (doIntersect(p, q, a, c)) { WIN[i]._allTriangles.erase(WIN[i]._allTriangles.begin() + j, WIN[i]._allTriangles.begin() + j + 3); break; }
                if (doIntersect(p, q, a, d)) { WIN[i]._allTriangles.erase(WIN[i]._allTriangles.begin() + j, WIN[i]._allTriangles.begin() + j + 3); break; }
                if (doIntersect(p, q, b, c)) { WIN[i]._allTriangles.erase(WIN[i]._allTriangles.begin() + j, WIN[i]._allTriangles.begin() + j + 3); break; }
                if (doIntersect(p, q, b, d)) { WIN[i]._allTriangles.erase(WIN[i]._allTriangles.begin() + j, WIN[i]._allTriangles.begin() + j + 3); break; }
                if (doIntersect(p, q, c, d)) { WIN[i]._allTriangles.erase(WIN[i]._allTriangles.begin() + j, WIN[i]._allTriangles.begin() + j + 3); break; }
                if (doIntersect(p, r, a, b)) { WIN[i]._allTriangles.erase(WIN[i]._allTriangles.begin() + j, WIN[i]._allTriangles.begin() + j + 3); break; }
                if (doIntersect(p, r, a, c)) { WIN[i]._allTriangles.erase(WIN[i]._allTriangles.begin() + j, WIN[i]._allTriangles.begin() + j + 3); break; }
                if (doIntersect(p, q, a, d)) { WIN[i]._allTriangles.erase(WIN[i]._allTriangles.begin() + j, WIN[i]._allTriangles.begin() + j + 3); break; }
                if (doIntersect(p, r, b, c)) { WIN[i]._allTriangles.erase(WIN[i]._allTriangles.begin() + j, WIN[i]._allTriangles.begin() + j + 3); break; }
                if (doIntersect(p, r, b, d)) { WIN[i]._allTriangles.erase(WIN[i]._allTriangles.begin() + j, WIN[i]._allTriangles.begin() + j + 3); break; }
                if (doIntersect(p, r, c, d)) { WIN[i]._allTriangles.erase(WIN[i]._allTriangles.begin() + j, WIN[i]._allTriangles.begin() + j + 3); break; }
                if (doIntersect(q, r, a, b)) { WIN[i]._allTriangles.erase(WIN[i]._allTriangles.begin() + j, WIN[i]._allTriangles.begin() + j + 3); break; }
                if (doIntersect(q, r, a, c)) { WIN[i]._allTriangles.erase(WIN[i]._allTriangles.begin() + j, WIN[i]._allTriangles.begin() + j + 3); break; }
                if (doIntersect(q, q, a, d)) { WIN[i]._allTriangles.erase(WIN[i]._allTriangles.begin() + j, WIN[i]._allTriangles.begin() + j + 3); break; }
                if (doIntersect(q, r, b, c)) { WIN[i]._allTriangles.erase(WIN[i]._allTriangles.begin() + j, WIN[i]._allTriangles.begin() + j + 3); break; }
                if (doIntersect(q, r, b, d)) { WIN[i]._allTriangles.erase(WIN[i]._allTriangles.begin() + j, WIN[i]._allTriangles.begin() + j + 3); break; }
                if (doIntersect(q, r, c, d)) { WIN[i]._allTriangles.erase(WIN[i]._allTriangles.begin() + j, WIN[i]._allTriangles.begin() + j + 3); break; }
            }
        }
    }
}

void drawGrid(const vector<Point>& solutionPoints) {
    const int GRID_SIZE = MATRIX_MAX; 
    char grid[GRID_SIZE][GRID_SIZE];


    for(int y = 0; y < GRID_SIZE; ++y) {
        for(int x = 0; x < GRID_SIZE; ++x) {
            grid[y][x] = '.';
        }
    }

    for(size_t i = 0; i < solutionPoints.size(); i += 3) {
        Point p1 = solutionPoints[i];
        Point p2 = solutionPoints[i+1];
        Point p3 = solutionPoints[i+2];

        for(int y = 0; y < GRID_SIZE; ++y) {
            for(int x = 0; x < GRID_SIZE; ++x) {
                Point center((float)x + 0.5f, (float)y + 0.5f);
                if(pointInTriangle(center, p1, p2, p3)) {
                    grid[y][x] = '*';
                }
            }
        }
    }

    // Print Grid Header
    cout << "\nGRID VISUALIZATION:\n   ";
    for(int x = 0; x < GRID_SIZE; ++x) cout << std::setw(2) << x;
    cout << "\n";

    for(int y = 0; y < GRID_SIZE; ++y) {
        cout << std::setw(2) << y << " ";
        for(int x = 0; x < GRID_SIZE; ++x) {
            cout << " " << grid[y][x];
        }
        cout << endl;
    }
}

void printSolution(vector<Point> e) {
    for (int j{}; j < e.size(); j += 3) {
        cout << "Triangle " << (j/3)+1 << ": ";
        cout << "(" << e[j].x << "," << e[j].y <<  ") |=| (" << e[j+1].x << "," << e[j+1].y << ") |=| (" << e[j+2].x << "," << e[j+2].y << ") ";
        cout << "\n";
    }
    cout << "\n";
    drawGrid(e);
}

void mySolution(vector<Triangle>& WIN, int index, vector<Point> e) {

    for (int x{}; x < index; x++) {
        cout << "***";
    }
    cout << index << "\n";

    if (index == WIN.size()) {
        cout << "\nSuccess! Solution found.\n";
        printSolution(e);
        exit(0);

    } else {

        for (int i{}; i < WIN[index]._allTriangles.size(); i += 3) {
            Point p = WIN[index]._allTriangles[i];
            Point q = WIN[index]._allTriangles[i+1];
            Point r = WIN[index]._allTriangles[i+2];
            
            bool triangleValid = true;
            for (int j{}; j < e.size(); j += 3) {
                if (doIntersect(p,q,e[j],e[j+1])) { triangleValid = false; break; }
                if (doIntersect(p,q,e[j],e[j+2])) { triangleValid = false; break; }
                if (doIntersect(p,q,e[j+2],e[j+1])) { triangleValid = false; break; }
                if (doIntersect(p,r,e[j],e[j+1])) { triangleValid = false; break; }
                if (doIntersect(p,r,e[j],e[j+2])) { triangleValid = false; break; }
                if (doIntersect(p,r,e[j+2],e[j+1])) { triangleValid = false; break; }
                if (doIntersect(q,r,e[j],e[j+1])) { triangleValid = false; break; }
                if (doIntersect(q,r,e[j],e[j+2])) { triangleValid = false; break; }
                if (doIntersect(q,r,e[j+2],e[j+1])) { triangleValid = false; break; }
            }

            if (triangleValid) {
                if(triangleIsContainedInOtherTriangle(e, p,q,r)) {
                    triangleValid = false; 
                }
                if(triangleIsContainingOtherTriangle(e,p,q,r)) {
                    triangleValid = false;
                }
            }

            if (triangleValid) {
                    e.push_back(q);
                    e.push_back(p);
                    e.push_back(r);
                    mySolution(WIN, index + 1, e);
                    e.pop_back();
                    e.pop_back();
                    e.pop_back();
            }
        }
    }
    return;  
}

int main() {

    vector<Triangle> WIN;

    vector<Point> ExistingPoints;
    
    Triangle T1(2,3,0), T2(18,7,0), T3(12,2,1), T4(4,13,1), T5(3,4,2), T6(7,11,2),
             T7(6,16,2), T8(6,0,3), T9(9,3,4), T10(11,9,4), T11(8,14,5), T12(4,0,6),
             T13(14,5,6), T14(18,15,6), T15(20,8,8), T16(7,1,10), T17(3,11,10),
             T18(3,16,10), T19(3,2,11), T20(7,7,12), T21(10,13,12), T22(5,16,13),
             T23(4,0,14), T24(10,5,14), T25(3,12,14),  T26(12,3,15), T27(7,14,15),
             T28(8,9,16), T29(2,13,16);

    WIN.insert(WIN.end(), { T1, T2, T3, T4, T5, T6, T7, T8, T9,
                            T10, T11, T12, T13, T14, T15, T16,
                            T17, T18, T19, T20, T21, T22, T23,
                            T24, T25, T26, T27, T28, T29 } );

    preProcessValidTriangles(WIN);

    mySolution(WIN,0,ExistingPoints);

    return 0;
}
