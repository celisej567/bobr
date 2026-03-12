#include "iostream"
#include "SDL3/SDL.h"
#include "SDL3/SDL_mouse.h"
#include "glad/glad.h"
// #include "GL/gl.h"

#include "vector"

#include "glm.hpp"
#include "gtc/matrix_transform.hpp"
#include "gtc/type_ptr.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

//#include "fstream"
//#include "sstream"

#include "CShader.h"
#include "FileSystemStuff.h"
#include "CTexture.h"


#define WND_WIDTH 800
#define WND_HEIGHT 600

//float vertices[] = {
//     0.5f,  0.5f, 0.0f,  // top right
//     0.5f, -0.5f, 0.0f,  // bottom right
//    -0.5f, -0.5f, 0.0f,  // bottom left
//    -0.5f,  0.5f, 0.0f   // top left 
//};
//
//unsigned int indices[] = {  // note that we start from 0!
//    0, 1, 3,   // first triangle
//    1, 2, 3    // second triangle
//};

//float vertices[] = {
//    // pos              color             UV   
//     0.5f,  0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,  // top right
//     0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,  // bottom right
//    -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,  // bottom left
//};

float vertices[] = {
    -0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
     0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
     0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
     0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
    -0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,

    -0.5f, -0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
     0.5f, -0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
     0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
     0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
    -0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,

    -0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
    -0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
    -0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
    -0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,

     0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
     0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
     0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,
     0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,
     0.5f, -0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
     0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,

    -0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,
     0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
     0.5f, -0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
     0.5f, -0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
    -0.5f, -0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,

    -0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,
     0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
     0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
     0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
    -0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
    -0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f
};

float vertices2[] = {
    -0.5f,  0.5f, 0.0f, 1.0f, 0.0f, 0.0f,  // top left
     0.5f,  0.5f, 0.0f, 0.0f, 1.0f, 0.0f,  // bottom right
    -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f  // bottom left
};

glm::vec3 cubePositions[] = {
    glm::vec3( 0.0f,  0.0f,  0.0f), 
    glm::vec3( 2.0f,  5.0f, -15.0f), 
    glm::vec3(-1.5f, -2.2f, -2.5f),  
    glm::vec3(-3.8f, -2.0f, -12.3f),  
    glm::vec3( 2.4f, -0.4f, -3.5f),  
    glm::vec3(-1.7f,  3.0f, -7.5f),  
    glm::vec3( 1.3f, -2.0f, -2.5f),  
    glm::vec3( 1.5f,  2.0f, -2.5f), 
    glm::vec3( 1.5f,  0.2f, -1.5f), 
    glm::vec3(-1.3f,  1.0f, -1.5f)  
};


int main()
{
    puts("bebra\n");
    
    SDL_Init(SDL_INIT_VIDEO);
    
    SDL_Window* wnd = SDL_CreateWindow("launch", WND_WIDTH, WND_HEIGHT, SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL);
    //SDL_Renderer* ren = SDL_CreateRenderer(wnd, NULL);

	SDL_SetWindowRelativeMouseMode(wnd, true);
    
    SDL_GLContext sdl_gl = SDL_GL_CreateContext(wnd);
   

    if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }
    SDL_GL_SetSwapInterval(1);

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

    CShader shader1("shaders/default.frag","shaders/default.vert");
    shader1.Use();
    shader1.SetUniformInt("texture2", 1);

//     //////////////////////////////////////////////

    unsigned int VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);  
    glGenBuffers(1, &VBO);
//     //glGenBuffers(1, &EBO);


    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    //glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3*sizeof(float)));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

	float deltaTime = 0.0f;	// Time between current frame and last frame
	float lastFrame = 0.0f; // Time of last frame


	//glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, -4.0f);
	//glm::vec3 cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);
	//glm::vec3 cameraDirection = glm::normalize(cameraPos - cameraTarget);
//
	//glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f); 
	//glm::vec3 cameraRight = glm::normalize(glm::cross(up, cameraDirection));
//
	//glm::vec3 cameraUp = glm::cross(cameraDirection, cameraRight);
	glm::vec3 cameraPos   = glm::vec3(0.0f, 0.0f,  3.0f);
	glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
	glm::vec3 cameraUp    = glm::vec3(0.0f, 1.0f,  0.0f);
	const float sensitivity = 0.1f;
	float yaw   = -90.0f;
	float pitch =  0.0f;

    SDL_Event event;
    bool quit = false;
    while(!quit)
    {
		float time = SDL_GetTicks() / 1000.0f;

		float currentFrame = time;
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		float cameraSpeed = 10.f * deltaTime;
		glm::vec3 cameraRight = glm::normalize(glm::cross(cameraFront, cameraUp));
        
        while(SDL_PollEvent(&event))
        {
			switch(event.type)
			{

				case SDL_EVENT_KEY_DOWN:
				{
					switch(event.key.scancode)
					{
						/*
						case SDL_SCANCODE_W:
						{
							cameraPos += cameraSpeed * cameraFront;
							break;
						}
						case SDL_SCANCODE_A:
						{
							cameraPos -= cameraSpeed * cameraRight;
							break;
						}
						case SDL_SCANCODE_S:
						{
							cameraPos -= cameraSpeed * cameraFront;
							break;
						}
						case SDL_SCANCODE_D:
						{
							cameraPos += cameraSpeed * cameraRight;
							break;
						}*/
						case SDL_SCANCODE_ESCAPE:
						{
							SDL_SetWindowRelativeMouseMode(wnd, !SDL_GetWindowRelativeMouseMode(wnd));
							//SDL_SetRelativeMouseMode(!SDL_GetRelativeMouseMode());
							break;
						}
					}
					
					break;
				}
				case SDL_EVENT_MOUSE_MOTION:
				{
					if(!SDL_GetWindowRelativeMouseMode(wnd))
						break;
						
					float xoffset = event.motion.xrel;
					xoffset *= sensitivity;
					float yoffset = -event.motion.yrel;
					yoffset *= sensitivity;

					
    				yaw   += xoffset;
    				pitch += yoffset;


    				if(pitch > 89.0f)
    				    pitch = 89.0f;
    				if(pitch < -89.0f)
    				    pitch = -89.0f;
								
    				glm::vec3 direction;
    				direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    				direction.y = sin(glm::radians(pitch));
    				direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    				cameraFront = glm::normalize(direction);
					break;
				}
				case SDL_EVENT_QUIT:
				{
            		quit = true;
            		break;
				}

			}
        }

		const bool *key_states = SDL_GetKeyboardState(NULL);

		if (key_states[SDL_SCANCODE_W]) 
		{
        	cameraPos += cameraSpeed * cameraFront;
    	}
		if (key_states[SDL_SCANCODE_A]) 
		{
        	cameraPos -= cameraSpeed * cameraRight;
    	} 
		if (key_states[SDL_SCANCODE_S]) 
		{
        	cameraPos -= cameraSpeed * cameraFront;
    	} 
		if (key_states[SDL_SCANCODE_D]) 
		{
        	cameraPos += cameraSpeed * cameraRight;
    	}

        glViewport(0,0,WND_WIDTH,WND_HEIGHT);
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		const float radius = 10.0f;
		float camX = sin(time) * radius;
		float camZ = cos(time) * radius;
		glm::mat4 view = glm::mat4(1);
		//view = glm::lookAt(glm::vec3(camX, 0.0, camZ), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f)); 
		view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);

        shader1.Use();
        shader1.SetUniformVec4("OffsetShit", 0,0,0,1);

        shader1.SetUniformFloat("mixAmount", sinf(time) * 0.5f + 0.5f);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture1);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture2);
        glBindVertexArray(VAO);

		for(unsigned int i = 0; i < 10; i++)
		{
		    glm::mat4 model = glm::mat4(1.0f);
		    model = glm::translate(model, cubePositions[i]);
		    float angle = 20.0f * i; 
		    model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));


			//glm::mat4 model = glm::mat4(1.0f);
			//model = glm::rotate(model, glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f));
			if( !((i+1) % 3) || i == 0 )
				model = glm::rotate(model, time * glm::radians(50.0f), glm::vec3(0.5f, 1.0f, 0.5f));

    		//view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));

    		glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)WND_WIDTH / (float)WND_HEIGHT, 0.1f, 100.0f);	

    		GLfloat* model_p = glm::value_ptr(model);
    		GLfloat* view_p = glm::value_ptr(view);
    		GLfloat* projection_p = glm::value_ptr(projection);


        	shader1.SetUniformMat4("model", model_p);
        	shader1.SetUniformMat4("view", view_p);
        	shader1.SetUniformMat4("proj", projection_p);

		
		    glDrawArrays(GL_TRIANGLES, 0, 36);
		}

        SDL_GL_SwapWindow(wnd);
        glFinish();

    }

    if(!SDL_GL_DestroyContext(sdl_gl))
    {
        std::cout << "Failed to destroy context. SDL says: \n" <<  SDL_GetError() << std::endl;
        return -1;
    }

//     //SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(wnd);

    puts("bebra2\n");
    
    return 0;
}
