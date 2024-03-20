#include <iostream>
#include <vector>
#include <algorithm>

struct Point {
    int64_t x, y;

    Point(int64_t x_, int64_t y_) : x(x_), y(y_) {}

    Point operator-(const Point& p) const {
        return Point(x - p.x, y - p.y);
    }

    int64_t cross(const Point& p) const {
        return x * p.y - y * p.x;
    }

    int64_t cross(const Point& p, const Point& q) const {
        return (p - *this).cross(q - *this);
    }

    int half() const {
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

void convert_adjacency_matrix(const std::vector<std::vector<int>>& matrix,
                              std::vector<Point>& vertices_out,
                              std::vector<std::vector<size_t>>& adj_list_out) {
    size_t n = matrix.size();  // Number of vertices
    const double pi = 3.14159265358979323846;
    const double angle_increment = 2 * pi / n; // Equal angle increment for vertices on a circle

    // Place vertices evenly around a unit circle
    vertices_out.reserve(n);
    for (size_t i = 0; i < n; ++i) {
        double angle = i * angle_increment;
        vertices_out.emplace_back(cos(angle), sin(angle));
    }

    // Convert adjacency matrix to adjacency list
    adj_list_out.resize(n);
    for (size_t i = 0; i < n; ++i) {
        for (size_t j = 0; j < n; ++j) {
            if (matrix[i][j] != 0) {  // Assuming non-zero value indicates an edge
                adj_list_out[i].push_back(j);
            }
        }
    }
}



int main() {
    // Adjacency matrix for the graph
    std::vector<std::vector<int>> adj_matrix = {
        {0, 1, 1},
        {1, 0, 1},
        {1, 1, 0},
    };

    // Output variables
    std::vector<Point> vertices;
    std::vector<std::vector<size_t>> adj_list;

    // Convert the adjacency matrix to the required format
    convert_adjacency_matrix(adj_matrix, vertices, adj_list);

    // Use the find_faces function to find all faces in the graph
    std::vector<std::vector<size_t>> faces = find_faces(vertices, adj_list);

    // Print out the faces found
    for (const auto& face : faces) {
        std::cout << "Face: ";
        for (size_t v : face) {
            std::cout << v << " ";
        }
        std::cout << std::endl;
    }

    return 0;
}