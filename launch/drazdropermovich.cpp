#include "entity/ECS.h"
#include "iostream"
#include "SDL3/SDL.h"
#include "SDL3/SDL_mouse.h"
#include "glad/glad.h"
// #include "GL/gl.h"

#include "shared.h"

#include "vector"

#include "glm.hpp"
#include "gtc/matrix_transform.hpp"
#include "gtc/type_ptr.hpp"
#include <SDL3/SDL_scancode.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

//#include "fstream"
//#include "sstream"

#include "CShader.h"
#include "FileSystemStuff.h"
#include "CTexture.h"
#include "CCamera.h"
#include "CModel.h"
#include "entity/CModelEntity.h"

#include "cmd/cmd.h"

#include "iomanip"
#include "dbg.h"

#define WND_WIDTH 800
#define WND_HEIGHT 600

int main(int argc, char **argv)
{
    puts("bebra\n");
    ConMsg("new bebra %s\n", "aboba");
    CMD::Msg("new CMD bebra %s\n", "aboba");

    CMD::ProcessArguments(argc, argv);

    std::cout << CMD::GetExecutable() << std::endl;

    if( CMD::FindArg("-aboba") )
        std::cout << "DID Found -aboba" << std::endl;
    else
        std::cout << "DID NOT Found -aboba" << std::endl;

    if( CMD::FindArg("-bebra") )
        std::cout << "DID Found -bebra, value is " << CMD::GetArg("-bebra", "NONE") << std::endl;
    else
        std::cout << "DID NOT Found -bebra" << std::endl;

    {
    if( CMD::FindArg("-integer") )
        std::cout << "DID Found -integer, value stored as int = " << CMD::GetArgInt("-integer") << std::endl;
    else
        std::cout << "DID NOT Found -integer" << std::endl;
    }

    {
    if( CMD::FindArg("-bool") )
        std::cout << "DID Found -bool, value stored as bool = " << CMD::GetArgBool("-bool") << std::endl;
    else
        std::cout << "DID NOT Found -bool" << std::endl;
    }

    {
    if( CMD::FindArg("-float") )
        std::cout << "DID Found -float, value stored as float = " << std::setprecision(9) << CMD::GetArgFloat("-float") << std::endl;
    else
        std::cout << "DID NOT Found -float" << std::endl;
    }

    {
    if( CMD::FindArg("-double") )
        std::cout << "DID Found -double, value stored as double = " << std::setprecision(17) << CMD::GetArgDouble("-double") << std::endl;
    else
        std::cout << "DID NOT Found -double" << std::endl;
    }

    SDL_Init(SDL_INIT_VIDEO);
    
    wnd = SDL_CreateWindow("launch", WND_WIDTH, WND_HEIGHT, SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL);
    //SDL_Renderer* ren = SDL_CreateRenderer(wnd, NULL);

	SDL_SetWindowRelativeMouseMode(wnd, true);
    
    SDL_GLContext sdl_gl = SDL_GL_CreateContext(wnd);
   

    if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }
    //SDL_GL_SetSwapInterval(1);

    const char* gl_version = (const char *)glGetString(GL_VERSION);
    std::cout << gl_version << std::endl;

    glViewport(0,0,WND_WIDTH,WND_HEIGHT);

	glEnable(GL_DEPTH_TEST);

    int nrAttributes;
    glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes);
    std::cout << "Maximum nr of vertex attributes supported: " << nrAttributes << std::endl;

    stbi_set_flip_vertically_on_load(true);

    CTexture texture1("textures/container.jpg", GL_RGB);
    CTexture texture2("textures/awesomeface.png", GL_RGBA);

    CompileAllShaders();

    CShader shader1("shaders/default.frag","shaders/default.vert");
    shader1.Use();
    shader1.SetUniformInt("texture2", 1);

	float deltaTime = 0.0f;	// Time between current frame and last frame
	float lastFrame = 0.0f; // Time of last frame


	CCamera mainCamera = CCamera();
	g_pActiveCamera = &mainCamera;

    CModel ObjModel("models/box.obj");

    CEntity* ent = (CEntity*)CreateEntity("base_entity");
    SpawnEntity(ent);

    CEntity* gizmos = (CEntity*)CreateEntity("gizmos");
    SpawnEntity(gizmos);

    CModelEntity* ent2 = (CModelEntity*)CreateEntity("model_entity");
    ent2->SetAbsPos({0,10,0});
    ent2->SetModelName("models/box.obj");
    ent2->SetTextureName("textures/container.jpg", GL_RGB);
    SpawnEntity(ent2);

    CModelEntity* ent3 = (CModelEntity*)CreateEntity("rotating_model");
    ent3->SetAbsPos({0,20,-10});
    ent3->SetModelName("models/box.obj");
    ent3->SetTextureName("textures/awesomeface.png", GL_RGBA);
    SpawnEntity(ent3);

    //glm::mat4 projection = glm::perspective(glm::radians(fov), (float)WND_WIDTH / (float)WND_HEIGHT, 0.1f, 100.0f);	

    SDL_Event event;
    bool quit = false;
    while(!quit)
    {
		float time = SDL_GetTicks() / 1000.0f;

        curtime = time;
		float currentFrame = time;
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		//float cameraSpeed = 10.f * deltaTime;
		//glm::vec3 cameraRight = glm::normalize(glm::cross(cameraFront, cameraUp));
        
        while(SDL_PollEvent(&event))
        {
			switch(event.type)
			{

				case SDL_EVENT_KEY_DOWN:
				{
					switch(event.key.scancode)
					{
						case SDL_SCANCODE_ESCAPE:
						{
							SDL_SetWindowRelativeMouseMode(wnd, !SDL_GetWindowRelativeMouseMode(wnd));
							//SDL_SetRelativeMouseMode(!SDL_GetRelativeMouseMode());
							break;
						}
                        case SDL_SCANCODE_J:
                        {  
                            if(ent2)
                            {
                                DeleteEntity(ent2);
                                ent2 = 0;
                            }
                            else
                            {
                                ent2 = (CModelEntity*)CreateEntity("model_entity");
                                ent2->SetAbsPos({0,10,0});
                                ent2->SetModelName("models/box.obj");
                                ent2->SetTextureName("textures/container.jpg", GL_RGB);
                                SpawnEntity(ent2);
                            }
                        }
                        break;
                        default:
                        break;
					}
					
					break;
				}
				case SDL_EVENT_MOUSE_WHEEL:
				case SDL_EVENT_MOUSE_MOTION:
    			case SDL_EVENT_MOUSE_BUTTON_DOWN:
    			case SDL_EVENT_MOUSE_BUTTON_UP:
    			case SDL_EVENT_MOUSE_ADDED:
    			case SDL_EVENT_MOUSE_REMOVED:
				{
					
					if(g_pActiveCamera)
					{
						if(!SDL_GetWindowRelativeMouseMode(wnd))
	            			break;
						g_pActiveCamera->ProcessSDLMouseInput(event, deltaTime);
					}
					
					break;
				}
				case SDL_EVENT_QUIT:
				{
            		quit = true;
            		break;
				}

			}
        }

        ProcessEntitiesTick();

		const bool *key_states = SDL_GetKeyboardState(NULL);

		if(g_pActiveCamera)
			g_pActiveCamera->ProcessSDLKeyInput(key_states, deltaTime);

        glViewport(0,0,WND_WIDTH,WND_HEIGHT);
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClearDepth(1);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		const float radius = 10.0f;
		float camX = sin(time) * radius;
		float camZ = cos(time) * radius;
		view = glm::mat4(1);
		//view = glm::lookAt(glm::vec3(camX, 0.0, camZ), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f)); 
		//view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);

		if(g_pActiveCamera)
			view = g_pActiveCamera->GetViewMatrix();

        shader1.Use();
        shader1.SetUniformVec4("OffsetShit", 0,0,0,1);

        shader1.SetUniformFloat("mixAmount", sinf(time) * 0.5f + 0.5f);


		float fov = 45;
		if(g_pActiveCamera)
				fov = g_pActiveCamera->GetFov();

        projection = glm::perspective(glm::radians(fov), (float)WND_WIDTH / (float)WND_HEIGHT, 0.1f, 100.0f);

        ProcessEntitiesFrame();

        SDL_GL_SwapWindow(wnd);
        glFinish();

    }

    AssetCache::Destroy();

    if(!SDL_GL_DestroyContext(sdl_gl))
    {
        std::cout << "Failed to destroy context. SDL says: \n" <<  SDL_GetError() << std::endl;
        return -1;
    }

//     //SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(wnd);
    wnd = NULL;

    puts("bebra2\n");
    
    return 0;
}
