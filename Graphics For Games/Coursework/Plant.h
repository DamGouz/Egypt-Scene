#pragma once
#include "..\..\nclgl\scenenode.h"
#include "..\..\nclgl\OBJMesh.h"

class Plant:public SceneNode
{
public:
	Plant(int x);
	~Plant() {};
	virtual void Update(float msec);
	static void CreatePlant() {
		OBJMesh * n = new OBJMesh();
		n->LoadOBJMesh(MESHDIR"leaf.obj");
		leaf = n;
	}
	static void DeletePlant() { delete leaf; }
protected:
	static Mesh * leaf;

	int leaf_start[24];
	int leaf_angle[24];
	float growth_c;
	float target_h;
	float target_r;
};

