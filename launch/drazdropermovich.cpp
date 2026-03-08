#include "iostream"
#include "SDL3/SDL.h"
#include "glad/glad.h"
#include "GL/gl.h"

#include "vector"

//#include "fstream"
//#include "sstream"

#include "CShader.h"
#include "FileSystemStuff.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

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

float vertices[] = {
    // pos              color             UV   
     0.5f,  0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,  // top right
     0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,  // bottom right
    -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,  // bottom left
};

float vertices2[] = {
    -0.5f,  0.5f, 0.0f, 1.0f, 0.0f, 0.0f,  // top left
     0.5f,  0.5f, 0.0f, 0.0f, 1.0f, 0.0f,  // bottom right
    -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f  // bottom left
};


int main()
{
    puts("bebra\n");
 
    SDL_Init(SDL_INIT_VIDEO);

    SDL_Window* wnd = SDL_CreateWindow("launch", WND_WIDTH, WND_HEIGHT, SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL);
    SDL_Renderer* ren = SDL_CreateRenderer(wnd, NULL);

    SDL_GLContext sdl_gl = SDL_GL_CreateContext(wnd);

    if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    const char* gl_version = (const char *)glGetString(GL_VERSION);
    std::cout << gl_version << std::endl;

    glViewport(0,0,WND_WIDTH,WND_HEIGHT);

    int nrAttributes;
    glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes);
    std::cout << "Maximum nr of vertex attributes supported: " << nrAttributes << std::endl;

    stbi_set_flip_vertically_on_load(true);

    int width, height, nrChannels;
    unsigned char *textureData = stbi_load("textures/container.jpg", &width, &height, &nrChannels, 0);

    unsigned int texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture); 

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    if (textureData)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, textureData);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(textureData);


    int width2, height2, nrChannels2;
    unsigned char *textureData2 = stbi_load("textures/awesomeface.png", &width2, &height2, &nrChannels2, 0);

    unsigned int texture2;
    glGenTextures(1, &texture2);
    glBindTexture(GL_TEXTURE_2D, texture2); 

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    if (textureData2)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width2, height2, 0, GL_RGBA, GL_UNSIGNED_BYTE, textureData2);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(textureData2);

    CShader shader1 = CShader("shaders/default.frag","shaders/default.vert");
    shader1.Use();
    shader1.SetUniformInt("texture2", 1);
    //CShader shader2 = CShader("shaders/default2.frag","shaders/default.vert");

    //////////////////////////////////////////////

    unsigned int VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);  
    glGenBuffers(1, &VBO);
    //glGenBuffers(1, &EBO);


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
///////////////////////////////////////////////////////////

    //unsigned int VBO2, VAO2;
    //glGenVertexArrays(1, &VAO2);  
    //glGenBuffers(1, &VBO2);
//
    //glBindVertexArray(VAO2);
    //
    //glBindBuffer(GL_ARRAY_BUFFER, VBO2);
    //glBufferData(GL_ARRAY_BUFFER, sizeof(vertices2), vertices2, GL_STATIC_DRAW);
//
    //glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    //glEnableVertexAttribArray(0);
//
    //glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3*sizeof(float)));
    //glEnableVertexAttribArray(1);
//
    //glBindBuffer(GL_ARRAY_BUFFER, 0);
    //glBindVertexArray(0);

    SDL_Event event;
    bool quit = false;
    while(!quit)
    {
        while(SDL_PollEvent(&event))
        {
            switch(event.type)
            {
                case SDL_EVENT_QUIT:
                    quit = true;
                    break;
            }
        }

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        shader1.Use();
        shader1.SetUniformVec4("OffsetShit", 0,0,0,1);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture2);
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        //shader2.Use();
        //int vertexColorLocation = glGetUniformLocation(shader2.ID, "ourColor");
        //glUniform4f(vertexColorLocation, 1.0f, 1.0f, 0.0f, 1.0f);
        //glBindVertexArray(VAO2);
        //glDrawArrays(GL_TRIANGLES, 0, 3);
        //glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        SDL_GL_SwapWindow(wnd);

    }

    glDeleteShader(shader1.fragShaderID);
    glDeleteShader(shader1.vertShaderID);

    if(!SDL_GL_DestroyContext(sdl_gl))
    {
        std::cout << "Failed to destroy context. SDL says: \n" <<  SDL_GetError() << std::endl;
        return -1;
    }

    SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(wnd);

    puts("bebra2\n");
    return 0;
}