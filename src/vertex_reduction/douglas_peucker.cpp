#include "douglas_peucker.hpp"

#include <iostream>
#include <cstring>
#include <stack>
#include <cmath>

/*
 * Calculate the perpendicular distance between a line and a point.
 * @param p The point to calculate the distance for.
 * @param v1 The first point in the line.
 * @param v2 The second point in the line.
 */
double perpendicular_distance(const ptg_vec2& p, const ptg_vec2& v1, const ptg_vec2& v2) {
    if (v1.x == v2.x) {
        return fabs((double)p.x - v1.x);
    } else {
        double slope = ((double)v2.y - v1.y) / ((double)v2.x - v1.x);
        double intercept = (double)v1.y - (slope * v1.x);
        return fabs(slope * p.x - p.y + intercept) / sqrt(slope * slope + 1.0);
    }
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
    while (!lines.empty()) {
        line l = lines.top();
        lines.pop();

        // Find point with maximum perpendicular distance.
        double max_distance = 0.0;
        unsigned int index = (l.first_point + 1) % (outline.vertex_count - 1);
        unsigned int max_index = index;
        while (index != l.last_point) {
            const double distance = perpendicular_distance(outline.vertices[index], outline.vertices[l.first_point], outline.vertices[l.last_point]);
            if (distance > max_distance) {
                max_distance = distance;
                max_index = index;
            }

            index = (index + 1) % (outline.vertex_count - 1);
        }

        // TODO: Make threshold configurable.
        const double threshold = 5.0;
        if (max_distance > threshold) {
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
    double max_distance = 0.0;
    for (unsigned int first_point = 0; first_point < outline.vertex_count - 2; ++first_point) {
        for (unsigned int last_point = first_point + 1; last_point < outline.vertex_count - 1; ++last_point) {
            const ptg_vec2 p1 = outline.vertices[first_point];
            const ptg_vec2 p2 = outline.vertices[last_point];
            const double delta_x = p2.x - p1.x;
            const double delta_y = p2.y - p1.y;
            const double distance = delta_x * delta_x + delta_y * delta_y;
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
