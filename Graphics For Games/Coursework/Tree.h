#pragma once
#include "..\..\nclgl\scenenode.h"
#include "..\..\nclgl\OBJMesh.h"

class Tree : public SceneNode {
public:
	Tree(int x, int y);
	~Tree(void) {};
	virtual void Update(float msec);
	static void CreateTree() {
		OBJMesh * m = new OBJMesh();
		m->LoadOBJMesh(MESHDIR"cylinder.obj");
		cylinder = m;

		OBJMesh * n = new OBJMesh();
		n->LoadOBJMesh(MESHDIR"leaf2.obj");
		leaf = n;
	}
	static void DeleteTree() { delete cylinder, leaf; }
	
protected:
	static Mesh * cylinder;
	static Mesh * leaf;

	SceneNode * trunk;
	int leaf_start[24];
	int leaf_angle[24];
	float growth_c;
	float target_h;
	float target_r;

	
};
