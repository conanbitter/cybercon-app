#include <stdio.h>
#include <SDL.h>
#include <glad/gl.h>

#pragma pack(push, 1)
typedef struct Vertex {
    float x;
    float y;
    float u;
    float v;
} Vertex;
#pragma pack(pop)

SDL_Window* window;
SDL_GLContext context;

static const char* vertex_shader_code =
    "#version 150\n"
    "in vec2 vert;\n"
    "in vec2 vertUV;\n"
    "out vec2 fragUV;\n"
    "void main() {\n"
    "    gl_Position = vec4(vert.x, vert.y, 0.0, 1.0);\n"
    "    fragUV = vertUV;\n"
    "}";

static const char* fragment_shader_code =
    "#version 150\n"
    "uniform sampler2D tex;\n"
    "in vec2 fragUV;\n"
    "out vec4 outputColor;\n"
    "void main() {\n"
    "    outputColor = vec4(1.0,0.0,0.0,1.0);\n"
    //"    outputColor = texture(tex, fragUV);\n"
    "}";

static const Vertex quad[6] = {
    {-0.5f, -0.5f, 0.0f, 0.0f},
    {-0.5f, 0.5f, 0.0f, 1.0f},
    {0.5f, 0.5f, 1.0f, 1.0f},
    {-0.5f, -0.5f, 0.0f, 0.0f},
    {0.5f, 0.5f, 1.0f, 1.0f},
    {0.5f, -0.5f, 1.0f, 0.0f},
};

void show_shader_log(GLuint shader) {
    if (glIsShader(shader)) {
        int infoLogLength = 0;
        int maxLength = infoLogLength;

        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);
        char* infoLog = malloc(maxLength);

        glGetShaderInfoLog(shader, maxLength, &infoLogLength, infoLog);
        printf("Shader compile error: %s\n", infoLog);
        free(infoLog);
    } else {
        puts("Shader compile error: Wrong shader handle");
    }
}

void show_program_log(GLuint shaderProgram) {
    if (glIsProgram(shaderProgram)) {
        int infoLogLength = 0;
        int maxLength = infoLogLength;

        glGetProgramiv(shaderProgram, GL_INFO_LOG_LENGTH, &maxLength);
        char* infoLog = malloc(maxLength);

        glGetProgramInfoLog(shaderProgram, maxLength, &infoLogLength, infoLog);
        printf("Shader link error: %s\n", infoLog);
        free(infoLog);
    } else {
        puts("Shader compile error: Wrong shader  program handle");
    }
}

GLuint compile_shader(const char* source, GLenum shaderType) {
    GLuint shader = glCreateShader(shaderType);
    const GLchar* lines[] = {source};
    glShaderSource(shader, 1, lines, NULL);
    glCompileShader(shader);
    GLint isCompiled = GL_TRUE;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);
    if (isCompiled != GL_TRUE) {
        show_shader_log(shader);
        return 0;
    }
    return shader;
}

GLuint compile_program(const char* vertexShaderCode, const char* fragmentShaderCode) {
    GLuint program = glCreateProgram();

    GLuint compiledVertexShader = compile_shader(vertexShaderCode, GL_VERTEX_SHADER);
    GLuint compiledFragmentShader = compile_shader(fragmentShaderCode, GL_FRAGMENT_SHADER);

    glAttachShader(program, compiledVertexShader);
    glAttachShader(program, compiledFragmentShader);
    glLinkProgram(program);
    GLint result = GL_TRUE;
    glGetProgramiv(program, GL_LINK_STATUS, &result);
    if (result != GL_TRUE) {
        show_program_log(program);
        return 0;
    }
    glDeleteShader(compiledVertexShader);
    glDeleteShader(compiledFragmentShader);
    return program;
};

int main(int argc, char* argv[]) {
    SDL_Init(SDL_INIT_VIDEO);

    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

    window = SDL_CreateWindow("Cybercon", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1024, 768, SDL_WINDOW_OPENGL);
    context = SDL_GL_CreateContext(window);

    int version = gladLoadGL((GLADloadfunc)SDL_GL_GetProcAddress);
    printf("Using OpenGL %d.%d\n", GLAD_VERSION_MAJOR(version), GLAD_VERSION_MINOR(version));

    GLuint program = compile_program(vertex_shader_code, fragment_shader_code);
    glUseProgram(program);
    GLint tex_loc = glGetUniformLocation(program, "tex");
    GLint vert_loc = glGetAttribLocation(program, "vert");
    GLint vertUV_loc = glGetAttribLocation(program, "vertUV");

    GLuint vao;
    GLuint vbo;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glEnableVertexAttribArray(vert_loc);
    glVertexAttribPointer(vert_loc, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), NULL);
    glEnableVertexAttribArray(vertUV_loc);
    glVertexAttribPointer(vertUV_loc, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)(2 * sizeof(float)));
    glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(Vertex), &quad, GL_STATIC_DRAW);

    int working = 1;
    while (working) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_QUIT:
                    working = 0;
                    break;
            }
        }

        glUseProgram(program);
        glBindVertexArray(vao);
        glDrawArrays(GL_TRIANGLES, 0, 6);

        SDL_GL_SwapWindow(window);
        SDL_Delay(5);
    }

    if (glIsBuffer(vbo)) {
        glDeleteBuffers(1, &vbo);
    }
    if (glIsVertexArray(vao)) {
        glDeleteVertexArrays(1, &vao);
    }
    if (glIsProgram(program)) {
        glDeleteProgram(program);
    }

    SDL_GL_DeleteContext(context);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}