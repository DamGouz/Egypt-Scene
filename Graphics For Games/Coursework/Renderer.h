#pragma once
#include "../../nclgl/OGLRenderer.h"
#include "../../nclgl/Camera.h"
#include "../../nclgl/SceneNode.h"
#include "Pyramid.h"
#include "../../nclgl/heightmap.h"
#include "Room.h"
#include "Tree.h"
#include "Plant.h"
#include <time.h>
#include "TextMesh.h"
#include "Beam.h"



class Renderer : public OGLRenderer {
public:
	Renderer(Window & parent);
	virtual ~Renderer(void);

	virtual void UpdateScene(float msec);
	virtual void RenderScene();
	void ToggleCircle(float x, float y, float z, float speed);
	float GetYaw(float x, float y, float z);
	float GetPitch(float x, float y, float z);

	bool SetYaw(float yaw, float speed);
	bool SetPitch(float pitch, float speed);

	void gotoTarget(float x, float y, float z, float speed) { camera->go_to(x, y, z, speed); }
	bool there_yet() { return camera->there_yet(); }
	void scene_fill(int s);
	int getScene() { return scene; }
	void	DrawText(const std::string &text, const Vector3 &position, const float size = 10.0f);

	void startBeams() { beam1->startAnim(); beam2->startAnim(); }
	void startTrees() { 
		for (vector < SceneNode * >::const_iterator i = root3->GetChildIteratorStart(); i != root3->GetChildIteratorEnd(); ++i) {
			(*i)->startAnim();
		}
	}
	bool finishedTrees() {
		bool res = true;
		for (vector < SceneNode * >::const_iterator i = root3->GetChildIteratorStart(); i != root3->GetChildIteratorEnd(); ++i) {
			res = res&&(*i)->fin();
		}
		return res;
	}
	float getredh() { return redh; }
	float followbeam() { return beam1->reth(); }
	bool finished() { return beam1->fin() && beam2->fin(); }
	void setfps(float msec) { fps = to_string(int(1000 / msec))+" fps"; }

	void disco();
protected:
	void DrawHeightmap(HeightMap * HM, int pass);
	void DrawWater();
	void DrawSkybox();

	void DrawNode(SceneNode * n);
	void ShadowNode(SceneNode * n);
	float redh = 1;
	bool up = false;
	void setTorchlight();
	HeightMap* oasisMap;
	SceneNode * root;
	SceneNode * root2;
	SceneNode * root3;
	
	void UpdateRedh(float msec);

	Shader * treeShader;
	Shader * wallShader;
	Shader * pyramidShader;
	Shader * terrainShader;
	Shader * oasisShader;

	int scene;

	int debug;

	Shader * lightShader;
	Shader * reflectShader;
	Shader * skyboxShader;
	Shader * textShader;
	HeightMap * heightMap;
	Mesh * quad;

	Light * light;

	vector<Light *>  torchlight;

	Camera * camera;

	GLuint cubeMap;
	float waterRotate;

	Font*	basicFont;

	Beam * beam1;
	Beam * beam2;

	string fps = "0 fps";


	void DrawShadowScene(); // New !
	void DrawCombinedScene(); // New !

	Shader * sceneShader;
	Shader * shadowShader;

	GLuint shadowTex;
	GLuint shadowFBO;
};

#define SHADOWSIZE 2048