#pragma once
#include "..\..\nclgl\scenenode.h"
#include "..\..\nclgl\OBJMesh.h"
class Beam : public SceneNode {
public:
	Beam();
	~Beam(void) {};
	virtual void Update(float msec);
	static void CreateBeam() {
		OBJMesh * m = new OBJMesh();
		OBJMesh * n = new OBJMesh();
		m->LoadOBJMesh(MESHDIR"cube.obj");
		bm = m;
	}
	static void DeleteBeam() { delete bm; }
	//void SetTexture(GLuint tex) { texture = tex; }
	
	float reth() { return (10700 + height); }
protected:
	static Mesh * bm;
	SceneNode * bd;
	float height = 0;
	//GLuint texture;
};
