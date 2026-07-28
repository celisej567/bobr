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
#include <SDL3/SDL_video.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

//#include "fstream"
//#include "sstream"

#include "CShader.h"
#include "FileSystemStuff.h"
#include "CTexture.h"
#include "CCamera.h"
#include "CCameraController.h"
#include "CModel.h"
#include "entity/CModelEntity.h"

#include "cmd/cmd.h"

#include "dbg.h"
#include "window/SDL3Window.hpp"
#include "inputmanager/inputmanager.hpp"

#include "shared.h"

#include "libs.h"
#include "manyinterfaces.h"

typedef const char* (*ReturnSomeString_t)();
typedef IMyLib* (*ReturnMyLib_t)();

glm::mat4 ReverseZPerspective(float fovRadians, float aspect, float nearPlane, float farPlane)
{
    float tangent = std::tan(fovRadians / 2.0f);
    glm::mat4 Result(0.0f);
    Result[0][0] = 1.0f / (aspect * tangent);
    Result[1][1] = 1.0f / tangent;
    Result[2][3] = -1.0f;
    Result[2][2] = nearPlane / (farPlane - nearPlane);
    Result[3][2] = (nearPlane * farPlane) / (farPlane - nearPlane);
    return Result;
}


glm::mat4 ReverseZPerspectiveEndless(float fovRadians, float aspect, float nearPlane)
{
    float tangent = std::tan(fovRadians / 2.0f);
    glm::mat4 Result(0.0f);
    Result[0][0] = 1.0f / (aspect * tangent);
    Result[1][1] = 1.0f / tangent;
    Result[2][3] = -1.0f;
    Result[2][2] = 0.0f;
    Result[3][2] = nearPlane;
    return Result;
}

int main(int argc, char **argv)
{
    SDL_Init(SDL_INIT_VIDEO);

    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

    puts("bebra\n");
    ConMsg("new bebra %s\n", "aboba");
    CMD::Msg("new CMD bebra %s\n", "aboba");

    CMD::ProcessArguments(argc, argv);

    std::cout << CMD::GetExecutable() << std::endl;

    /*if( CMD::FindArg("-aboba") )
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
    }*/

    //TODO: make paths platform-independed
#ifdef PLATFORM_WINDOWS
    g_InputManager = (IInputManager*)LIB_LoadModule("./inputmanager.dll");
#elifdef PLATFORM_POSIX
    g_InputManager = (IInputManager*)LIB_LoadModule("./libinputmanager.so");
#endif

    if(!g_InputManager)
        CMD::Msg("Unable to load module IInputManager.\n");
    else
        CMD::Msg("IInputManager Loaded.\n");

    g_pMainWindow = new CSDL3Window();
    
	g_pMainWindow->SetRelativeMouse(true);
    
    if(g_InputManager)
        g_InputManager->InitializeWindow(g_pMainWindow);


    SDL_GLContext sdl_gl = SDL_GL_CreateContext((SDL_Window*)(g_pMainWindow->get()));


    if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    CMD::Msg("-vsync = %d\n", CMD::GetArgInt("-vsync", 0));
    SDL_GL_SetSwapInterval(CMD::GetArgInt("-vsync", 0));

    const char* gl_version = (const char *)glGetString(GL_VERSION);
    std::cout << gl_version << std::endl;

    glViewport(0,0,WND_WIDTH,WND_HEIGHT);

    glEnable(GL_DEPTH_TEST);
    glClipControl(GL_LOWER_LEFT, GL_ZERO_TO_ONE);
    glDepthFunc(GL_GREATER);

    int nrAttributes;
    glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes);
    std::cout << "Maximum nr of vertex attributes supported: " << nrAttributes << std::endl;

    int depthBits, stencilBits;
    SDL_GL_GetAttribute(SDL_GL_DEPTH_SIZE, &depthBits);
    SDL_GL_GetAttribute(SDL_GL_STENCIL_SIZE, &stencilBits);
    printf("Depth: %d, Stencil: %d\n", depthBits, stencilBits);

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

    CCameraController camController(&mainCamera);

    camController.SetMoveSpeed(5, 100);

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


    CModelEntity* terr = (CModelEntity*)CreateEntity("model_entity");
    terr->SetAbsPos({0,-10,0});
    terr->SetModelName("models/terr.obj");
    terr->SetTextureName("textures/container.jpg", GL_RGB);
    terr->SetScale(100,100,100);
    SpawnEntity(terr);

    //glm::mat4 projection = glm::perspective(glm::radians(fov), (float)WND_WIDTH / (float)WND_HEIGHT, 0.1f, 100.0f);	

    //temp
    struct FPSTimer
    {
        Uint64 current_time = SDL_GetTicks();
        Uint64 fps_timer = current_time;
        int frame_count = 0;
        int current_fps = 0;
        char window_title[128];
    } fps_timer_shit;


    bool quit = false;
    while(!quit)
    {
		float time = SDL_GetTicks() / 1000.0f;

        curtime = time;
		float currentFrame = time;
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

        if(g_InputManager)
        {
            g_InputManager->PollEvents();

            if (g_InputManager->GetExitFlag())
                quit = true;

            if (g_InputManager->IsKeyPressed(KeyCode::Escape))
            {
                g_pMainWindow->ToggleRelativeMouse();
            }

            if (g_InputManager->IsKeyPressed(KeyCode::J))
            {
                if(ent2 || terr)
                {
                    DeleteEntity(ent2);
                    ent2 = 0;

                    DeleteEntity(terr);
                    terr = 0;
                }
                else
                {
                    ent2 = (CModelEntity*)CreateEntity("model_entity");
                    ent2->SetAbsPos({0,10,0});
                    ent2->SetModelName("models/box.obj");
                    ent2->SetTextureName("textures/container.jpg", GL_RGB);
                    SpawnEntity(ent2);

                    terr = (CModelEntity*)CreateEntity("model_entity");
                    terr->SetAbsPos({0,-10,0});
                    terr->SetModelName("models/terr.obj");
                    terr->SetTextureName("textures/container.jpg", GL_RGB);
                    terr->SetScale(100,100,100);
                    SpawnEntity(terr);
                }
            }
        }

        ProcessEntitiesTick();

        // camera controller — keyboard always, mouse only when relative mode is on
        camController.Update(g_InputManager, deltaTime, g_pMainWindow->GetRelativeMouse());

        glViewport(0,0,WND_WIDTH,WND_HEIGHT);
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClearDepth(0.0f);
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

        //projection = ReverseZPerspective(glm::radians(fov), (float)WND_WIDTH / (float)WND_HEIGHT, 0.125, 100.0f);
        projection = ReverseZPerspectiveEndless(glm::radians(fov), (float)WND_WIDTH / (float)WND_HEIGHT, 0.125);

        ProcessEntitiesFrame();

        ///////// Fps counter stuff
        {
            fps_timer_shit.frame_count++;
            fps_timer_shit.current_time = SDL_GetTicks();


            if (fps_timer_shit.current_time - fps_timer_shit.fps_timer >= 1000) {
                fps_timer_shit.current_fps = fps_timer_shit.frame_count;
                fps_timer_shit.frame_count = 0;
                fps_timer_shit.fps_timer = fps_timer_shit.current_time;

                SDL_snprintf(fps_timer_shit.window_title, sizeof(fps_timer_shit.window_title), "| BUILD %s %s | SDL3 FPS : %d",__DATE__, __TIME__, fps_timer_shit.current_fps);
                SDL_SetWindowTitle((SDL_Window*)g_pMainWindow->get(), fps_timer_shit.window_title);
            }
        }

        //TODO move to window class
        SDL_GL_SwapWindow((SDL_Window*)g_pMainWindow->get());
        glFinish();

    }

    if(g_InputManager)
        g_InputManager->Shutdown();

    AssetCache::Destroy();

    if(!SDL_GL_DestroyContext(sdl_gl))
    {
        std::cout << "Failed to destroy context. SDL says: \n" <<  SDL_GetError() << std::endl;
        return -1;
    }

//     //SDL_DestroyRenderer(ren);
    delete g_pMainWindow;
    g_pMainWindow = NULL;

    puts("bebra2\n");
    
    return 0;
}
