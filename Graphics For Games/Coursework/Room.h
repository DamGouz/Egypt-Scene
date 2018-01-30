#pragma once
#include "..\..\nclgl\scenenode.h"
#include "..\..\nclgl\OBJMesh.h"
class Room : public SceneNode {
public:
	Room(float x, float y, float z, int draw_bin);
	~Room(void) {};
	virtual void Update(float msec);
	static void CreateRoom() {
		Mesh * m = new Mesh();
		m = Mesh::GenerateQuad();
		room = m;

		OBJMesh * n = new OBJMesh();
		n->LoadOBJMesh(MESHDIR"cylinder.obj");
		cyl = n;

		OBJMesh * o = new OBJMesh();
		o->LoadOBJMesh(MESHDIR"sphere.obj");
		f = o;
	}
	static void DeleteRoom() { delete room; delete cyl; delete f; }
	//void SetTexture(GLuint tex) { texture = tex; }

protected:
	static Mesh * room;
	static Mesh * cyl;
	static Mesh * f;
	SceneNode * wall;

	SceneNode * fire1;
	SceneNode * fire2;
	SceneNode * fire3;
	SceneNode * fire4;
	//GLuint texture;
};
