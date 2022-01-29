#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<iostream>

//ע��ص����������ڵ������ڴ�С
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
//���뺯��
void processInput(GLFWwindow* window);

//׼��������ɫ��
const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\0";

//׼��Ƭ����ɫ��
const char* fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor; \n"
"void main(){\n"
"   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f); \n"
"}\0";

int main() {
    //1.ʵ��������
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    //2.�������ڶ���
    GLFWwindow* window = glfwCreateWindow(800, 600, "learnOpenGL", NULL, NULL);
    if (window == NULL) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    //ע��ص�����
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    //3.��ʼ��GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }


    int success;
    char infoLog[512];
    //4.����������ɫ��
    unsigned int vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);//����Ϊ������ɫ��
    //����ɫ��Դ�븽�Ӹ���ɫ������
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);//'1'ָ�ַ�������
    glCompileShader(vertexShader);//������ɫ������
    //��֤��ɫ���Ƿ����ɹ�
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    //5.����Ƭ����ɫ��
    unsigned int fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    //6.������ɫ������������ɫ������
    unsigned int shaderProgram;
    shaderProgram = glCreateProgram();
    //����ɫ�����ӵ���ɫ��������
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    //��֤������ɫ�������Ƿ�ʧ��
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::LINK::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    //��ɫ���������ӵ���������Ժ�ɾ����ɫ������
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    //7.����������������
    float vertices[] = {
        0.0f, 0.0f, 0.0f,   
        0.0f, -0.5f, 0.0f,  
        0.0f, 0.5f, 0.0f, 
        -0.5f, 0.0f, 0.0f,
        0.5f, 0.0f, 0.0f
    };

    //8.������������
    unsigned int indices[] = { // ע��������0��ʼ! 
        0, 1, 4, // ��һ��������
        0, 3, 2  // �ڶ���������
    };


    //9.�������󣬶��㻺����󣬶���������������������
    unsigned int VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    glBindVertexArray(VAO);

    //10.���Ӷ��㻺�����
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    //11.���������������
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    //12.������������
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);


    //13.��Ⱦѭ��(Render Loop)
    while (!glfwWindowShouldClose(window)) {
        //����û��Ƿ����˷��ؼ�(Esc)
        processInput(window);

        //��Ⱦָ�����glClear�����������Ļ����ɫ���壩
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        //��Ⱦ���򣬻�������
        //����������(����֮��ÿ����ɫ�����ú���Ⱦ���ö���ʹ������������)
        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        //glDrawArrays(GL_TRIANGLES, 0, 3);


        //�����û�д���ʲô�¼������´���״̬�������ö�Ӧ�Ļص�����
        glfwSwapBuffers(window);
        //������ɫ����
        glfwPollEvents();
    }

    //14.�ͷŶ���
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glDeleteProgram(shaderProgram);

    glfwTerminate();
    return 0;
}


//ע��ص����������ڵ������ڴ�С
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

//���뺯��,������escape���˳�����
void processInput(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}
