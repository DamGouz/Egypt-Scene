#pragma once
#include "..\..\nclgl\scenenode.h"
#include "..\..\nclgl\OBJMesh.h"
class Pyramid : public SceneNode {
public:
	Pyramid();
	~Pyramid(void) {};
	virtual void Update(float msec);
	static void CreatePyramid() {
		OBJMesh * m = new OBJMesh();
		m -> LoadOBJMesh(MESHDIR"pyramid.obj");
		pyr = m;
	}
	static void DeletePyramid() { delete pyr; }
	//void SetTexture(GLuint tex) { texture = tex; }
protected:
	static Mesh * pyr;
	SceneNode * body;
	//GLuint texture;
};
