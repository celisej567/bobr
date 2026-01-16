#include "iostream"
#include "SDL3/SDL.h"
#include "glad/glad.h"
#include "GL/gl.h"

#include "fstream"
#include "sstream"

#define WND_WIDTH 800
#define WND_HEIGHT 600

float vertices[] = {
    -0.5f, -0.5f, 0.0f,
     0.5f, -0.5f, 0.0f,
     0.0f,  0.5f, 0.0f
};

const char* GetFileString(const char* str)
{
    std::ifstream inFile(str, std::ios_base::ate);
    if (!inFile.is_open())
    {
        std::cout << "Error opening file \"" << str << "\"." << std::endl;
        return NULL;
    }

    inFile.seekg(0);
    std::stringstream ghghgh;

    ghghgh << inFile.rdbuf();

    char *strbuf;
    strbuf = new char[(int)inFile.tellg()]();

    strcpy(strbuf, ghghgh.str().c_str());

    return strbuf;
}


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

    //////////////////////////////////////////
    const char* vertexShaderBuff = GetFileString("shaders/default.vert");
    if(vertexShaderBuff == NULL)
        return -1;

    unsigned int vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderBuff, NULL);
    glCompileShader(vertexShader);

    {
        int  success;
        char infoLog[512];
        glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

        if(!success)
        {
            glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
            std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
        }
    }

    delete vertexShaderBuff;
    /////////////////////////////////////////////
    const char* fragmentShaderBuff = GetFileString("shaders/default.frag");

    if(fragmentShaderBuff == NULL)
        return -1;

    unsigned int fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderBuff, NULL);
    glCompileShader(fragmentShader);

    {
        int  success;
        char infoLog[512];
        glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);

        if(!success)
        {
            glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
            std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
        }
    }

    delete fragmentShaderBuff;
    /////////////////////////////////////////////

    unsigned int shaderProgram;
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    {
        int  success;
        char infoLog[512];
        glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
        if(!success) {
            glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
            std::cout << "ERROR::SHADERPROGRAM::LINK::FAILED\n" << infoLog << std::endl;
        }
    }
    //////////////////////////////////////////////

    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO);  
    glGenBuffers(1, &VBO);


    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);  
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

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

        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        SDL_GL_SwapWindow(wnd);

    }


    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);  
    
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