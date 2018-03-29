#include "douglas_peucker.hpp"

#include <iostream>
#include <cstring>
#include <stack>
#include <cmath>
#include <algorithm>

// Helper class for linear algebra.
struct vec2 {
    float x;
    float y;

    vec2(float x, float y) {
        this->x = x;
        this->y = y;
    }

    vec2(const ptg_vec2& a) {
        x = a.x;
        y = a.y;
    }

    vec2 operator+(const vec2& other) const {
        return vec2(x + other.x, y + other.y);
    }

    vec2 operator-(const vec2& other) const {
        return vec2(x - other.x, y - other.y);
    }

    vec2 operator*(float t) const {
        return vec2(x * t, y * t);
    }
};

// Get the dot product between two points.
static float dot(const vec2& a, const vec2& b) {
    return a.x * b.x + a.y * b.y;
}

// Get the squared distance between two points.
static float distance_sqr(const vec2& a, const vec2& b) {
    const float x_diff = a.x - b.x;
    const float y_diff = a.y - b.y;
    return x_diff * x_diff + y_diff * y_diff;
}

/*
 * Calculate the squared shortest distance between a line segment and a point.
 * @param p The point to calculate the distance for.
 * @param v1 The first point in the line segment.
 * @param v2 The second point in the line segment.
 */
static float shortest_distance_sqr(const vec2& p, const vec2& v1, const vec2& v2) {
    // v1 must be not be same as v2.
    const float l2 = distance_sqr(v1, v2);

    // Project p onto line v1-v2.
    // Clamp between [0,1] to handle cases where point is outside line segment.
    const float t = std::max(0.0f, std::min(1.0f, dot(p - v1, v2 - v1) / l2));
    const vec2 projection = v1 + (v2 - v1) * t;

    // Get distance between projected point and point.
    return distance_sqr(p, projection);
}

struct line {
    unsigned int first_point;
    unsigned int last_point;
};

/*
 * Recursively reduce the vertex count in a line using Douglas-Peucker.
 * @param outline The outline to reduce.
 * @param lines The lines to reduce.
 * @param keep The array defining whether points should be kept.
 */
static void reduce_line(const ptg_outline& outline, std::stack<line>& lines, bool* keep) {
    // TODO: Make threshold configurable.
    const float threshold = 5.0f;
    const float threshold_sqr = threshold * threshold;

    while (!lines.empty()) {
        line l = lines.top();
        lines.pop();

        // Find point with maximum perpendicular distance.
        float max_distance = 0.0f;
        unsigned int index = (l.first_point + 1) % (outline.vertex_count - 1);
        unsigned int max_index = index;
        while (index != l.last_point) {
            const float distance = shortest_distance_sqr(outline.vertices[index], outline.vertices[l.first_point], outline.vertices[l.last_point]);
            if (distance > max_distance) {
                max_distance = distance;
                max_index = index;
            }

            index = (index + 1) % (outline.vertex_count - 1);
        }

        // Check whether distance exceeds the threshold.
        if (max_distance > threshold_sqr) {
            // Keep point and call recursively.
            lines.push({l.first_point, max_index});
            lines.push({max_index, l.last_point});
        } else {
            // Remove points.
            unsigned int index = (l.first_point + 1) % (outline.vertex_count - 1);
            while (index != l.last_point) {
                keep[index] = false;
                index = (index + 1) % (outline.vertex_count - 1);
            }
        }
    }
}

/*
 * Reduce vertex count in outline using Douglas-Peucker.
 * @param outline The outline to reduce.
 */
static void reduce_outline(ptg_outline& outline) {
    // Find the two points farthest from each other.
    unsigned int max_first_point = 0;
    unsigned int max_last_point = 0;
    float max_distance = 0.0;
    for (unsigned int first_point = 0; first_point < outline.vertex_count - 2; ++first_point) {
        for (unsigned int last_point = first_point + 1; last_point < outline.vertex_count - 1; ++last_point) {
            const ptg_vec2 p1 = outline.vertices[first_point];
            const ptg_vec2 p2 = outline.vertices[last_point];
            const float delta_x = (float)p2.x - p1.x;
            const float delta_y = (float)p2.y - p1.y;
            const float distance = delta_x * delta_x + delta_y * delta_y;
            if (distance > max_distance) {
                max_distance = distance;
                max_first_point = first_point;
                max_last_point = last_point;
            }
        }
    }

    // Allocate buffer for whether points should be kept.
    bool* keep = new bool[outline.vertex_count - 1];
    memset(keep, 1, outline.vertex_count - 1);

    // Apply Douglas-Peucker on both lines.
    std::stack<line> lines;
    lines.push({max_first_point, max_last_point});
    lines.push({max_last_point, max_first_point});
    reduce_line(outline, lines, keep);

    // Generate final outline.
    unsigned int vertex_index = 0;
    for (unsigned int i = 0; i < outline.vertex_count - 1; ++i) {
        if (keep[i])
            outline.vertices[vertex_index++] = outline.vertices[i];
    }
    outline.vertices[vertex_index++] = outline.vertices[0];
    outline.vertex_count = vertex_index;

    delete[] keep;
}

void ptgi_douglas_peucker(ptg_tracing_results* tracing_results) {
    std::cout << "ptgi_douglas_peucker has not yet been implemented." << std::endl;

    for (unsigned int layer = 0; layer < tracing_results->layer_count; ++layer) {
        for (unsigned int outline = 0; outline < tracing_results->outline_counts[layer]; ++outline) {
            reduce_outline(tracing_results->outlines[layer][outline]);
        }
    }
}
