#include "../source/SiE.h"

int main() {
		

	
	WindowCreationProperties prop;
	prop.openGLVersionMajor = 3;
	prop.openGLVersionMinor = 2;
	prop.width = 1280;
	prop.height = 720;
	prop.multisamplingCount = 10;

	Window window(prop);

	//Test case preparation...
    MeshAttribute::Vertex vertices;

    vertices.positions = {
           -0.5, -0.5,  0.5,
        0.5, -0.5,  0.5,
        0.5,  0.5,  0.5,
        -0.5,  0.5,  0.5,
        // back
        -0.5, -0.5, -0.5,
        0.5, -0.5, -0.5,
        0.5,  0.5, -0.5,
        -0.5,  0.5, -0.5
    };

    vertices.indices = {
        // front
        0, 1, 2,
        2, 3, 0,
        // right
        1, 5, 6,
        6, 2, 1,
        // back
        7, 6, 5,
        5, 4, 7,
        // left
        4, 0, 3,
        3, 7, 4,
        // bottom
        4, 5, 1,
        1, 0, 4,
        // top
        3, 2, 6,
        6, 7, 3
    };



    GameObject obj = GameObject::CreateNew("Cube");
    Mesh& mesh = obj.AddComponent<Mesh>();

    mesh.SetVertices(vertices);
    mesh.SetShader("base_shader");

    
    mesh.PreDrawn().Connect([&](Mesh& mesh, Shader& shader, const glm::mat4& matrix) {
        mesh.GetMasterObject().GetAs<GameObject>().Transform().Rotate(0,15 * window.GetDeltaTime(),0);
    });
    

    obj.Transform().SetPosition(0,0,0);

	while (window.IsOpen()) {




		window.DrawFrame();
	}


};


