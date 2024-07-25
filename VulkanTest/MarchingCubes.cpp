#include "MarchingCubes.h"
#include <iostream>
MarchingCubes::MarchingCubes(glm::vec3 pos, glm::vec3 size, int resolution)
{

	fnFractal->SetSource(fnSimplex);
	fnFractal->SetOctaveCount(1);
	

	float cubeValues[8];
	glm::vec3 vertlist[12];
	glm::vec3 p[8];

	
	for (int z = 0; z < size.z * resolution; z++) {
		for (int y = 0; y < size.y * resolution; y++) {
			for (int x = 0; x < size.x * resolution; x++) {

				int cubeindex = 0;
				glm::vec3 cubePos = pos + (glm::vec3(x, y, z)/ (float)resolution);
				for (int i = 0; i < 8; i++) {

					p[i] = cubePos + (offsets[i]/(float)resolution);

					cubeValues[i] = noise(p[i]);

					
					if (cubeValues[i] < isolevel) {
						cubeindex |= 1 << i;
					}
				}

				if (cubeindex == 0) {
					continue;
				}

				if (edgeTable[cubeindex] & 1)
					vertlist[0] =
					vertexInterp(isolevel, p[0], p[1], cubeValues[0], cubeValues[1]);
				if (edgeTable[cubeindex] & 2)
					vertlist[1] =
					vertexInterp(isolevel, p[1], p[2], cubeValues[1], cubeValues[2]);
				if (edgeTable[cubeindex] & 4)
					vertlist[2] =
					vertexInterp(isolevel, p[2], p[3], cubeValues[2], cubeValues[3]);
				if (edgeTable[cubeindex] & 8)
					vertlist[3] =
					vertexInterp(isolevel, p[3], p[0], cubeValues[3], cubeValues[0]);
				if (edgeTable[cubeindex] & 16)
					vertlist[4] =
					vertexInterp(isolevel, p[4], p[5], cubeValues[4], cubeValues[5]);
				if (edgeTable[cubeindex] & 32)
					vertlist[5] =
					vertexInterp(isolevel, p[5], p[6], cubeValues[5], cubeValues[6]);
				if (edgeTable[cubeindex] & 64)
					vertlist[6] =
					vertexInterp(isolevel, p[6], p[7], cubeValues[6], cubeValues[7]);
				if (edgeTable[cubeindex] & 128)
					vertlist[7] =
					vertexInterp(isolevel, p[7], p[4], cubeValues[7], cubeValues[4]);
				if (edgeTable[cubeindex] & 256)
					vertlist[8] =
					vertexInterp(isolevel, p[0], p[4], cubeValues[0], cubeValues[4]);
				if (edgeTable[cubeindex] & 512)
					vertlist[9] =
					vertexInterp(isolevel, p[1], p[5], cubeValues[1], cubeValues[5]);
				if (edgeTable[cubeindex] & 1024)
					vertlist[10] =
					vertexInterp(isolevel, p[2], p[6], cubeValues[2], cubeValues[6]);
				if (edgeTable[cubeindex] & 2048)
					vertlist[11] =
					vertexInterp(isolevel, p[3], p[7], cubeValues[3], cubeValues[7]);

				/* Create the triangle */
				for (int i = 0; triTable[cubeindex][i] != -1; i += 3) {
					Vertex p0 = {};
					Vertex p1 = {};
					Vertex p2 = {};

					p0.pos = vertlist[triTable[cubeindex][i]];;
					p1.pos = vertlist[triTable[cubeindex][i + 1]];
					p2.pos = vertlist[triTable[cubeindex][i + 2]];

					p0.color = (p0.pos / size);
					p1.color = (p1.pos / size);
					p2.color = (p2.pos / size);

					p0.normal = calcNormal(p0.pos);
					p1.normal = calcNormal(p1.pos);
					p2.normal = calcNormal(p2.pos);

					

					indices.push_back(indices.size());
					indices.push_back(indices.size());
					indices.push_back(indices.size());

					vertices.push_back(p0);
					vertices.push_back(p1);
					vertices.push_back(p2);


				}


			}
		}
	}

	

}

float MarchingCubes::noise(glm::vec3 pos)
{
	return -((fnFractal->GenSingle3D(pos.x / 64, pos.y / 64, pos.z / 64, seed) * 1));
	
}

glm::vec3 MarchingCubes::vertexInterp(float isoLevel, glm::vec3 p1, glm::vec3 p2, float v1, float v2)
{
	double mu;
	glm::vec3 p;

	if (abs(isoLevel - v1) < 0.00001)
		return(p1);
	if (abs(isoLevel - v2) < 0.00001)
		return(p2);
	if (abs(v1 - v2) < 0.00001)
		return(p1);
	mu = (isoLevel - v1) / (v2 - v1);
	p.x = p1.x + mu * (p2.x - p1.x);
	p.y = p1.y + mu * (p2.y - p1.y);
	p.z = p1.z + mu * (p2.z - p1.z);

	return(p);
}

glm::vec3 MarchingCubes::calcNormal(glm::vec3 p)
{
	float EPSILON = 1.0f;
	return glm::normalize(glm::vec3(
		noise(glm::vec3(p.x + EPSILON, p.y, p.z)) - noise(glm::vec3(p.x - EPSILON, p.y, p.z)),
		noise(glm::vec3(p.x, p.y + EPSILON, p.z)) - noise(glm::vec3(p.x, p.y - EPSILON, p.z)),
		noise(glm::vec3(p.x, p.y, p.z + EPSILON)) - noise(glm::vec3(p.x, p.y, p.z - EPSILON))
	));
}
