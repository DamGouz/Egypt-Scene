#pragma once

#include "OGLRenderer.h"

 enum MeshBuffer {
	 VERTEX_BUFFER, COLOUR_BUFFER, TEXTURE_BUFFER, NORMAL_BUFFER, TANGENT_BUFFER, INDEX_BUFFER, MAX_BUFFER
};
class Mesh {
public:
	void SetBumpMap(GLuint tex) { bumpTexture = tex; }
	GLuint GetBumpMap() { return bumpTexture; }
	Mesh(void);
	~Mesh(void);
	virtual void Draw();
	static Mesh * GenerateTriangle();
	void SetTexture(GLuint tex, int i) { texture[i] = tex; }
	GLuint GetTexture(int i) { return texture[i]; }
	static Mesh * GenerateQuad();
	static Mesh * GeneratePoint();
	Vector3 * normals;
protected:
	void GenerateTangents();
	Vector3 GenerateTangent(const Vector3 &a, const Vector3 &b,const Vector3 &c, const Vector2 & ta,const Vector2 & tb, const Vector2 & tc);
	Vector3 * tangents;
	GLuint bumpTexture;
	void GenerateNormals();
	void BufferData();
	
	GLuint numIndices;
	unsigned int * indices;
	GLuint texture[3];
	Vector2 * textureCoords;
	
	GLuint arrayObject;
	GLuint bufferObject[MAX_BUFFER];
	GLuint numVertices;
	GLuint type;
	Vector3 * vertices;
	Vector4 * colours;
};
