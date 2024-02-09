#include "ke_model.hpp"

#include <vector>

#define KE_FLOAT_AVG(x, y) ((x+y)/2.0)

namespace ke {
	std::vector<KeModel::Vertex> create_triangles(KeModel::Vertex start, float height, unsigned int power)
	{
		std::vector<KeModel::Vertex> triangles;
		
		start.color = { 1.0f, 0.0f, 0.0f };

		// Find left vertex
		KeModel::Vertex left_vertex;
		left_vertex.position.x = start.position.x - (height / 2);
		left_vertex.position.y = start.position.y + height;
		left_vertex.color = { 0.0f, 1.0f, 0.0f };
		

		// Find right vertex
		KeModel::Vertex right_vertex;
		right_vertex.position.x = start.position.x + (height / 2);
		right_vertex.position.y = start.position.y + height;
		right_vertex.color = { 0.0f, 0.0f, 1.0f };
		

		// return a triangle
		if (power == 0) {
			triangles.push_back({ start });
			triangles.push_back({ right_vertex });
			triangles.push_back({ left_vertex });
			return triangles;
		}
		
		// append new triangles
		auto top_triangles = create_triangles(start, height / 2, power - 1);

		auto right_triangles = create_triangles(
			{ 
				{
					KE_FLOAT_AVG(start.position.x, right_vertex.position.x), 
					KE_FLOAT_AVG(start.position.y, right_vertex.position.y)
				},
				{
					KE_FLOAT_AVG(start.color.r, right_vertex.color.r),
					KE_FLOAT_AVG(start.color.g, right_vertex.color.g),
					KE_FLOAT_AVG(start.color.b, right_vertex.color.b)
				}
			}, height / 2, power - 1);

		auto left_triangles = create_triangles(
			{ 
				{
					KE_FLOAT_AVG(start.position.x, left_vertex.position.x), 
					KE_FLOAT_AVG(start.position.y, left_vertex.position.y)
				},
				{
					KE_FLOAT_AVG(start.color.r, left_vertex.color.r),
					KE_FLOAT_AVG(start.color.g, left_vertex.color.g),
					KE_FLOAT_AVG(start.color.b, left_vertex.color.b)
				}
			}, height / 2, power - 1);

		triangles.insert(triangles.end(), top_triangles.begin(), top_triangles.end());
		triangles.insert(triangles.end(), left_triangles.begin(), left_triangles.end());
		triangles.insert(triangles.end(), right_triangles.begin(), right_triangles.end());

		return triangles;
	}
}