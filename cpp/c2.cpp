#include <iostream>
#include <vector>
#include <map>
#include <algorithm>
#include <cmath>
// ... other necessary includes ...

struct Point {
    int64_t x, y;

    Point(int64_t x_, int64_t y_): x(x_), y(y_) {}

    Point operator - (const Point & p) const {
        return Point(x - p.x, y - p.y);
    }

    int64_t cross (const Point & p) const {
        return x * p.y - y * p.x;
    }

    int64_t cross (const Point & p, const Point & q) const {
        return (p - *this).cross(q - *this);
    }

    int half () const {
        return int(y < 0 || (y == 0 && x < 0));
    }
};

std::vector<std::vector<size_t>> find_faces(std::vector<Point> vertices, std::vector<std::vector<size_t>> adj) {
    size_t n = vertices.size();
    std::vector<std::vector<char>> used(n);
    for (size_t i = 0; i < n; i++) {
        used[i].resize(adj[i].size());
        used[i].assign(adj[i].size(), 0);
        auto compare = [&](size_t l, size_t r) {
            Point pl = vertices[l] - vertices[i];
            Point pr = vertices[r] - vertices[i];
            if (pl.half() != pr.half())
                return pl.half() < pr.half();
            return pl.cross(pr) > 0;
        };
        std::sort(adj[i].begin(), adj[i].end(), compare);
    }
    std::vector<std::vector<size_t>> faces;
    for (size_t i = 0; i < n; i++) {
        for (size_t edge_id = 0; edge_id < adj[i].size(); edge_id++) {
            if (used[i][edge_id]) {
                continue;
            }
            std::vector<size_t> face;
            size_t v = i;
            size_t e = edge_id;
            while (!used[v][e]) {
                used[v][e] = true;
                face.push_back(v);
                size_t u = adj[v][e];
                size_t e1 = std::lower_bound(adj[u].begin(), adj[u].end(), v, [&](size_t l, size_t r) {
                    Point pl = vertices[l] - vertices[u];
                    Point pr = vertices[r] - vertices[u];
                    if (pl.half() != pr.half())
                        return pl.half() < pr.half();
                    return pl.cross(pr) > 0;
                }) - adj[u].begin() + 1;
                if (e1 == adj[u].size()) {
                    e1 = 0;
                }
                v = u;
                e = e1;
            }
            std::reverse(face.begin(), face.end());
            int sign = 0;
            for (size_t j = 0; j < face.size(); j++) {
                size_t j1 = (j + 1) % face.size();
                size_t j2 = (j + 2) % face.size();
                int64_t val = vertices[face[j]].cross(vertices[face[j1]], vertices[face[j2]]);
                if (val > 0) {
                    sign = 1;
                    break;
                } else if (val < 0) {
                    sign = -1;
                    break;
                }
            }
            if (sign <= 0) {
                faces.insert(faces.begin(), face);
            } else {
                faces.emplace_back(face);
            }
        }
    }
    return faces;
}



std::vector<std::vector<size_t>> find_faces_from_adj_matrix(const std::vector<std::vector<int>>& adj_matrix) {
    // Calculate positions for vertices (this is an example for a grid layout)
    std::vector<Point> vertices;
    size_t n = adj_matrix.size();  // Assuming it's a square matrix
    for (size_t i = 0; i < n; ++i) {
        for (size_t j = 0; j < n; ++j) {
            // Example: Lay out the vertices in a grid
            vertices.emplace_back(i, j);
        }
    }

    // Create edges based on the adjacency matrix
    std::vector<std::vector<size_t>> adj_list(n);
    for (size_t i = 0; i < n; ++i) {
        for (size_t j = 0; j < n; ++j) {
            if (adj_matrix[i][j] == 1) {
                adj_list[i].push_back(j);
            }
        }
    }

    // Now we can call find_faces with the generated vertices and adjacency list
    return find_faces(vertices, adj_list);
}


int main() {
    // Example of a simple 4x4 adjacency matrix
    std::vector<std::vector<int>> adj_matrix = {
        {0, 1, 0, 1},
        {1, 0, 1, 0},
        {0, 1, 0, 1},
        {1, 0, 1, 0}
    };

    // Call the function to find faces from the adjacency matrix
    std::cout << "Calling find_faces_from_adj_matrix..." << std::endl;
    auto faces = find_faces_from_adj_matrix(adj_matrix);
    std::cout << "Function returned." << std::endl;

    // Check if faces vector is empty
    if (faces.empty()) {
        std::cout << "No faces found." << std::endl;
    } else {
        // Print the faces
        std::cout << "Faces found:" << std::endl;
        for (const auto& face : faces) {
            std::cout << "Face: ";
            for (size_t vertex : face) {
                std::cout << vertex << " ";
            }
            std::cout << std::endl;
        }
    }

    return 0;
}
