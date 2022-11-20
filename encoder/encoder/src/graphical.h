#pragma once

#include <GL/glew.h> //init before every other opengl lib
#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>


#define TIMEPERIOD 0.05f

class Graph {
public:
    unsigned int shader;
    unsigned int vao;
    unsigned int buffer;
    float* lines;
};

class Axis {
public:
    unsigned int shader;
    unsigned int vao;
    unsigned int buffer;
};

class Position {
public:
    float x;
    float y;
};

class Color {
public:
    float r;
    float g;
    float b;
    float a;

    Color(float red, float green, float blue, float alpha) {
        r = red;
        g = green;
        b = blue;
        a = alpha;
    }
};

static unsigned int CompileShader(unsigned int type, const std::string& source) {
    unsigned int id = glCreateShader(type);
    const char* src = source.c_str();
    glShaderSource(id, 1, &src, nullptr);
    glCompileShader(id);

    int result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);

    if (result == GL_FALSE) {
        int length;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
        char* message = (char*)alloca(length * sizeof(char));
        glGetShaderInfoLog(id, length, &length, message);
        std::cout << "Failed to Compile " << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << " shader!" << std::endl;
        std::cout << message << std::endl;
        glDeleteShader(id);
        return 0;
    }

    return id;
}

static unsigned int createShader(const std::string& vertexShader, const std::string& fragmentShader) {
    unsigned int program = glCreateProgram();
    unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
    unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

    glAttachShader(program, vs);
    glAttachShader(program, fs);

    glLinkProgram(program);
    glValidateProgram(program);

    glDeleteShader(vs);
    glDeleteShader(fs);

    return program;
}

Axis* Yinit(float x) {
    Axis* yaxis = new Axis();
    unsigned int vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    yaxis->vao = vao;

    unsigned int buffer;

    float line[4] = {
        x, -1.0f,
        x,  1.0f
    };

    glGenBuffers(1, &buffer);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glBufferData(GL_ARRAY_BUFFER, 4 * sizeof(float), &line, GL_STATIC_DRAW);
    yaxis->buffer = buffer;

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_TRUE, sizeof(float) * 2, 0);

    /*Shader*/
    std::string vertexShader =
        "#version 330 core\n"
        "\n"
        "layout(location = 0) in vec4 position;\n"
        "void main()\n"
        "{\n"
        "   gl_Position = position;\n"
        "}\n";
    std::string fragmentShader =
        "#version 330 core\n"
        "\n"
        "layout(location = 0) out vec4 color;\n"
        "uniform vec4 u_Color;\n" //uniform
        "void main()\n"
        "{\n"
        "   color = u_Color;\n" //~RGBA
        "}\n";
    unsigned int shader = createShader(vertexShader, fragmentShader);
    yaxis->shader = shader;

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    return yaxis;
}

/* X axis of the signals*/
Axis* Xinit(float y) {
    Axis* xaxis = new Axis();
    unsigned int vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    xaxis->vao = vao;

    unsigned int buffer;

    float line[4] = {
        -1.0f, y,
         1.0f, y
    };

    glGenBuffers(1, &buffer);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glBufferData(GL_ARRAY_BUFFER, 4 * sizeof(float), &line, GL_STATIC_DRAW);
    xaxis->buffer = buffer;

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_TRUE, sizeof(float) * 2, 0);

    /*Shader*/
    std::string vertexShader =
        "#version 330 core\n"
        "\n"
        "layout(location = 0) in vec4 position;\n"
        "void main()\n"
        "{\n"
        "   gl_Position = position;\n"
        "}\n";
    std::string fragmentShader =
        "#version 330 core\n"
        "\n"
        "layout(location = 0) out vec4 color;\n"
        "uniform vec4 u_Color;\n" //uniform
        "void main()\n"
        "{\n"
        "   color = u_Color;\n" //~RGBA
        "}\n";
    unsigned int shader = createShader(vertexShader, fragmentShader);
    xaxis->shader = shader;

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    return xaxis;
}

void drawAxis(Axis* axis, Color* color) {
    glUseProgram(axis->shader);

    int location = glGetUniformLocation(axis->shader, "u_Color");
    glUniform4f(location, color->r, color->g, color->b, color->a);

    glBindVertexArray(axis->buffer);

    glDrawArrays(GL_LINE_STRIP, 0, 2);

}

/* All graphs except AMI; 1==>-  |  0==>0 */
Graph* graphInit(float tp, Color* color, std::vector<int>& bin_data, float sect) {
    Position p;
    Graph* graph = new Graph();

    float lines[100 * 6];
    graph->lines = lines;

    unsigned int vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    graph->vao = vao;

    unsigned int buffer;

    p.x = -1.0f;
    p.y = 0.26f + sect; // assumed 1 "change"

    float signal[6];
    for (int i = 0; i < bin_data.size(); i++) {
        if (bin_data[i] == 1) {
            signal[0] = p.x; signal[1] = p.y;
            p.y = 0.26f + sect; signal[2] = p.x; signal[3] = p.y; //|
            p.x += tp;  signal[4] = p.x; signal[5] = p.y; //-
        }
        else if (bin_data[i] == 0) {
            signal[0] = p.x; signal[1] = p.y;
            p.y = sect - 0.26f; signal[2] = p.x; signal[3] = p.y; //|
            p.x += tp;  signal[4] = p.x; signal[5] = p.y; //-
        }
        for (int j = 0; j < 6; j++) {
            lines[6 * i + j] = signal[j];
        }
    }

    glGenBuffers(1, &buffer); // Make buffer
    glBindBuffer(GL_ARRAY_BUFFER, buffer); // Select buffer
    glBufferData(GL_ARRAY_BUFFER, bin_data.size()  * 6 * sizeof(float), &lines, GL_STATIC_DRAW); // Insert data
    graph->buffer = buffer;

    /*Select attributes*/
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_TRUE, sizeof(float) * 2, 0); //changes

    /*Shader*/
    std::string vertexShader =
        "#version 330 core\n"
        "\n"
        "layout(location = 0) in vec4 position;\n"
        "void main()\n"
        "{\n"
        "   gl_Position = position;\n"
        "}\n";
    std::string fragmentShader =
        "#version 330 core\n"
        "\n"
        "layout(location = 0) out vec4 color;\n"
        "uniform vec4 u_Color;\n" //uniform
        "void main()\n"
        "{\n"
        "   color = u_Color;\n" //RGBA
        "}\n";
    unsigned int shader = createShader(vertexShader, fragmentShader);
    graph->shader = shader;

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    return graph;
}

/* 1==>+  |  -1==>-  |  0==>0 */
Graph* amiGraphInit(float tp, Color* color, std::vector<int>& bin_data, float sect) {
    Position p;
    Graph* graph = new Graph();

    float lines[100 * 6];
    graph->lines = lines;

    unsigned int vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    graph->vao = vao;

    unsigned int buffer;

    p.x = -1.0f;
    p.y = 0.26f + sect; // assumed 1 "change"

    float signal[6];
    for (int i = 0; i < bin_data.size(); i++) {
        if (bin_data[i] == 1) {
            signal[0] = p.x; signal[1] = p.y;
            p.y = 0.26f + sect; signal[2] = p.x; signal[3] = p.y; //|
            p.x += tp;  signal[4] = p.x; signal[5] = p.y; //-
        }
        else if (bin_data[i] == -1) {
            signal[0] = p.x; signal[1] = p.y;
            p.y = sect - 0.26f; signal[2] = p.x; signal[3] = p.y; //|
            p.x += tp;  signal[4] = p.x; signal[5] = p.y; //-
        }
        else if (bin_data[i] == 0) {
            signal[0] = p.x; signal[1] = p.y;
            p.y = sect; signal[2] = p.x; signal[3] = p.y; //|
            p.x += tp;  signal[4] = p.x; signal[5] = p.y; //-
        }
        for (int j = 0; j < 6; j++) {
            lines[6 * i + j] = signal[j];
        }
    }

    glGenBuffers(1, &buffer); // Make buffer
    glBindBuffer(GL_ARRAY_BUFFER, buffer); // Select buffer
    glBufferData(GL_ARRAY_BUFFER, bin_data.size() * 6 * sizeof(float), &lines, GL_STATIC_DRAW); // Insert data
    graph->buffer = buffer;

    /*Select attributes*/
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_TRUE, sizeof(float) * 2, 0); //changes

    /*Shader*/
    std::string vertexShader =
        "#version 330 core\n"
        "\n"
        "layout(location = 0) in vec4 position;\n"
        "void main()\n"
        "{\n"
        "   gl_Position = position;\n"
        "}\n";
    std::string fragmentShader =
        "#version 330 core\n"
        "\n"
        "layout(location = 0) out vec4 color;\n"
        "uniform vec4 u_Color;\n" //uniform
        "void main()\n"
        "{\n"
        "   color = u_Color;\n" //~RGBA
        "}\n";
    unsigned int shader = createShader(vertexShader, fragmentShader);
    graph->shader = shader;

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    return graph;
}

void drawGraph(Graph* graph, Color* color, std::vector<int>& bin_data) {
    glUseProgram(graph->shader);

    int location = glGetUniformLocation(graph->shader, "u_Color");
    glUniform4f(location, color->r, color->g, color->b, color->a);
    glBindVertexArray(graph->buffer);

    glDrawArrays(GL_LINE_STRIP, 0, bin_data.size() * 3);
}

/* The function called in main */
int graph(std::vector<int>& bin_data, std::vector<int>& encoded_data, std::vector<int>& decoded_data, int encoder_choice)
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(1800, 1250, "</3NC0D3R>", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    /*GLEW Initialised*/
    if (glewInit() != GLEW_OK)
        return -1;
    std::cout << glGetString(GL_VERSION) << std::endl;

    Color color_bin(1.0f, 0.0f, 0.0f, 1.0f);
    Color color_enc(0.0f, 1.0f, 0.0f, 1.0f);
    Color color_dec(0.0f, 0.0f, 1.0f, 1.0f);
    Color color_axis(1.0f, 1.0f, 1.0f, 0.02f);

    Graph* bin;
    Graph* enc = NULL; //SET to NULL: potential uninitialization fixed
    Graph* dec;
    bin = graphInit(TIMEPERIOD, &color_bin, bin_data, 0.63f);
    switch (encoder_choice) {
    case 1: //nrzl
        enc = graphInit(TIMEPERIOD, &color_enc, encoded_data, 0.0f);
        break;
    case 2: //nrzi
        enc = graphInit(TIMEPERIOD, &color_enc, encoded_data, 0.0f);
        break;
    case 3: //manchester
        enc = graphInit(TIMEPERIOD / 2.0f, &color_enc, encoded_data, 0.0f);
        break;
    case 4: //diff manchester
        enc = graphInit(TIMEPERIOD / 2.0f, &color_enc, encoded_data, 0.0f);
        break;
    case 5: //ami
        enc = amiGraphInit(TIMEPERIOD, &color_enc, encoded_data, 0.0f);
        break;
    default:
        break;
    }
    
    dec = graphInit(TIMEPERIOD, &color_dec, decoded_data, -0.63f);

    Axis* y_bin;
    Axis* y_enc;
    Axis* y_dec;
    y_bin = Xinit(0.63f);
    y_enc = Xinit(0.0f);
    y_dec = Xinit(-0.63f);

    std::vector<Axis*> verticals;
    for (float i = -1.0f; i <= 1.0f; i += TIMEPERIOD) {
        verticals.push_back(Yinit(i));
    }

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        /* Draw verticals */
        for (auto& itr : verticals)
            drawAxis(itr, &color_axis);

        /* Draw x axis */
        drawAxis(y_bin, &color_axis);
        drawAxis(y_enc, &color_axis);
        drawAxis(y_dec, &color_axis);

        /* Draw three signals */
        drawGraph(bin, &color_bin, bin_data);
        drawGraph(enc, &color_enc, encoded_data);
        drawGraph(dec, &color_dec, decoded_data);

        /* Swap front (finished frame) and back (next frame) buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events so that window responds */
        glfwPollEvents();
    }

    //glDeleteProgram(shader);

    glfwTerminate();
    return 0;
}