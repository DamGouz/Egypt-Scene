#pragma once
#include "../../nclgl/OGLRenderer.h"
#include "../../nclgl/Camera.h"
#include "../../nclgl/SceneNode.h"
#include "Pyramid.h"
#include "../../nclgl/heightmap.h"
#include "Room.h"
class Renderer2 : public OGLRenderer {
public:
	Renderer2(Window & parent);
	virtual ~Renderer2(void);

	virtual void UpdateScene(float msec);
	virtual void RenderScene();
	void ToggleCircle() { camera->Circular(14500, 14500); }
	void gotoTarget(float x, float y, float z, float speed) { camera->go_to(x, y, z, speed); }
	bool there_yet() { return camera->there_yet(); }
protected:
	void DrawNode(SceneNode * n);
	bool up = false;
	SceneNode * root;
	Camera * camera;
	Shader * wallShader;
};