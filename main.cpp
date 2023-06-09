#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include "ShaderProgram.h"
#include "Cube.h"

const int WINDOW_WIDTH = 1280;
const int WINDOW_HEIGHT = 720;
const float FPS_UPDATE_SECONDS = 1.0;

static void glfw_error_callback(int error, const char* description) {
    std::cerr << "GLFW Error " << error << ": " << description << std::endl;
}

int main(int argc, char *argv[]) {
    glfwSetErrorCallback(glfw_error_callback);
    if (!glfwInit()) {
        return 1;
    }

    const char* glsl_version = "#version 150";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_MAXIMIZED, GL_TRUE);

    // Create window with graphics context
    GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Post-Processing Test", NULL, NULL);
    if (window == NULL) {
        return 1;
    }
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1); // Enable vsync

    glewExperimental = GL_TRUE;
    if (GLEW_OK != glewInit()) {
        glfwTerminate();
        return -1;
    }

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

    ImGui::StyleColorsDark();

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);

    ShaderProgram colorPassthroughShader;
    colorPassthroughShader.loadShadersFromFile("colorPassthrough.vert", "colorPassthrough.frag");
    ShaderProgram grayscaleShader;
    grayscaleShader.loadShadersFromFile("grayscale.vert", "grayscale.frag");
    ShaderProgram mosaicShader;
    mosaicShader.loadShadersFromFile("mosaic.vert", "mosaic.frag");
    ShaderProgram scanlinesShader;
    scanlinesShader.loadShadersFromFile("scanlines.vert", "scanlines.frag");
    ShaderProgram colorizeShader;
    colorizeShader.loadShadersFromFile("colorize.vert", "colorize.frag");
    ShaderProgram boxBlurShader;
    boxBlurShader.loadShadersFromFile("boxBlur.vert", "boxBlur.frag");

    GLuint frameBufferId, framebufferColorTexture;
    glGenFramebuffers(1, &frameBufferId);
    glBindFramebuffer(GL_FRAMEBUFFER, frameBufferId);
    glGenTextures(1, &framebufferColorTexture);
    glBindTexture(GL_TEXTURE_2D, framebufferColorTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, WINDOW_WIDTH, WINDOW_HEIGHT, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, framebufferColorTexture, 0);
    GLuint renderBufferId;
    glGenRenderbuffers(1, &renderBufferId);
    glBindRenderbuffer(GL_RENDERBUFFER, renderBufferId);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, WINDOW_WIDTH, WINDOW_HEIGHT);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, renderBufferId);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    enum PostProcessingEffect {
        NO_EFFECT,
        GRAYSCALE,
        MOSAIC,
        SCANLINES,
        SEPIA,
        COLORIZE,
        BOX_BLUR
    };
    int selectedEffect = NO_EFFECT;

    glm::vec3 initialCubePosition = glm::vec3(0.0f, 0.0f, -2.0f);
    Cube cube(initialCubePosition, colorPassthroughShader);
    glm::vec3 initialCubePosition2 = glm::vec3(3.0f, 2.0f, -3.0f);
    Cube cube2(initialCubePosition2, colorPassthroughShader);
    glm::vec3 initialCubePosition3 = glm::vec3(1.5f, -2.0f, -4.0f);
    Cube cube3(initialCubePosition3, colorPassthroughShader);
    glm::vec3 initialCubePosition4 = glm::vec3(-4.0f, 3.0f, -5.0f);
    Cube cube4(initialCubePosition4, colorPassthroughShader);
    glm::vec3 initialCubePosition5 = glm::vec3(-3.0f, -4.0f, -5.0f);
    Cube cube5(initialCubePosition5, colorPassthroughShader);

    float screenVertexData[] = {
        -1.0f, -1.0f, 0.0f,    0.0f, 0.0f,
         1.0f, -1.0f, 0.0f,    1.0f, 0.0f,
         1.0f,  1.0f, 0.0f,    1.0f, 1.0f,
        -1.0f, -1.0f, 0.0f,    0.0f, 0.0f,
         1.0f,  1.0f, 0.0f,    1.0f, 1.0f,
        -1.0f,  1.0f, 0.0f,    0.0f, 1.0f
    };
    GLuint vao, vbo;
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(screenVertexData), screenVertexData, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (void*)0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    ShaderProgram screenShader;
    screenShader.loadShadersFromFile("screen.vert", "screen.frag");

    int mosaicBlockSize = 20;
    int scanlinesLineThickness = 3;
    glm::vec3 colorizeColor = glm::vec3(0.0f, 1.0f, 0.0f);
    int boxBlurRadius = 5;

    bool animate = true;
    bool animatePressed = false;
    double previousTime = glfwGetTime();
    double fpsTime = 0.0;
    int frames = 0;
    int displayedFps = 0;
    while (!glfwWindowShouldClose(window)) {
        double currentTime = glfwGetTime();
        double deltaTime = currentTime - previousTime;
        previousTime = currentTime;

        frames++;
        fpsTime += deltaTime;
        if (fpsTime >= FPS_UPDATE_SECONDS) {
            displayedFps = frames / fpsTime;
            frames = 0;
            fpsTime = 0.0;
        }

        // Poll and handle events (inputs, window resize, etc.)
        // You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if dear imgui wants to use your inputs.
        // - When io.WantCaptureMouse is true, do not dispatch mouse input data to your main application, or clear/overwrite your copy of the mouse data.
        // - When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to your main application, or clear/overwrite your copy of the keyboard data.
        // Generally you may always pass all inputs to dear imgui, and hide them from your application based on those two flags.
        glfwPollEvents();

        int animateButtonStatus = glfwGetKey(window, GLFW_KEY_SPACE);
        if (!animatePressed && animateButtonStatus == GLFW_PRESS) {
            animate = !animate;
            animatePressed = true;
        } else if (animatePressed && animateButtonStatus == GLFW_RELEASE) {
            animatePressed = false;
        }

        glBindFramebuffer(GL_FRAMEBUFFER, frameBufferId);
        glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glEnable(GL_DEPTH_TEST);
        double animationDeltaTime = animate ? deltaTime : 0.0;
        cube.animateAndRender(animationDeltaTime);
        cube2.animateAndRender(animationDeltaTime);
        cube3.animateAndRender(animationDeltaTime);
        cube4.animateAndRender(animationDeltaTime);
        cube5.animateAndRender(animationDeltaTime);
        glDisable(GL_DEPTH_TEST);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        int displayWidth, displayHeight;
        glfwGetFramebufferSize(window, &displayWidth, &displayHeight);
        glViewport(0, 0, displayWidth, displayHeight);
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        if (selectedEffect == GRAYSCALE) {
            grayscaleShader.use();
            glUniform1i(grayscaleShader.getUniformLocation((char *)"theTexture"), 0);
            glBindVertexArray(vao);
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, framebufferColorTexture);
            glDrawArrays(GL_TRIANGLES, 0, 6);
        } else if (selectedEffect == MOSAIC) {
            mosaicShader.use();
            glUniform1i(mosaicShader.getUniformLocation((char *)"theTexture"), 0);
            glUniform2f(mosaicShader.getUniformLocation((char *)"dimensions"), displayWidth, displayHeight);
            glUniform1i(mosaicShader.getUniformLocation((char *)"blockSize"), mosaicBlockSize);
            glBindVertexArray(vao);
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, framebufferColorTexture);
            glDrawArrays(GL_TRIANGLES, 0, 6);
        } else if (selectedEffect == SCANLINES) {
            scanlinesShader.use();
            glUniform1i(scanlinesShader.getUniformLocation((char *)"theTexture"), 0);
            glUniform1i(scanlinesShader.getUniformLocation((char *)"lineThickness"), scanlinesLineThickness);
            glBindVertexArray(vao);
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, framebufferColorTexture);
            glDrawArrays(GL_TRIANGLES, 0, 6);
        } else if (selectedEffect == SEPIA) {
            glm::vec3 sepiaColor = glm::vec3(0.439f, 0.259f, 0.0784f);
            colorizeShader.use();
            glUniform1i(colorizeShader.getUniformLocation((char *)"theTexture"), 0);
            glUniform3fv(colorizeShader.getUniformLocation((char *)"selectedColor"), 1, &sepiaColor[0]);
            glBindVertexArray(vao);
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, framebufferColorTexture);
            glDrawArrays(GL_TRIANGLES, 0, 6);
        } else if (selectedEffect == COLORIZE) {
            colorizeShader.use();
            glUniform1i(colorizeShader.getUniformLocation((char *)"theTexture"), 0);
            glUniform3fv(colorizeShader.getUniformLocation((char *)"selectedColor"), 1, &colorizeColor[0]);
            glBindVertexArray(vao);
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, framebufferColorTexture);
            glDrawArrays(GL_TRIANGLES, 0, 6);
        } else if (selectedEffect == BOX_BLUR) {
            boxBlurShader.use();
            glUniform1i(boxBlurShader.getUniformLocation((char *)"theTexture"), 0);
            glUniform2f(boxBlurShader.getUniformLocation((char *)"dimensions"), displayWidth, displayHeight);
            glUniform1i(boxBlurShader.getUniformLocation((char *)"blurRadius"), boxBlurRadius);
            glBindVertexArray(vao);
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, framebufferColorTexture);
            glDrawArrays(GL_TRIANGLES, 0, 6);
        } else {
            screenShader.use();
            glUniform1i(screenShader.getUniformLocation((char *)"theTexture"), 0);
            glBindVertexArray(vao);
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, framebufferColorTexture);
            glDrawArrays(GL_TRIANGLES, 0, 6);
        }

        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        ImGui::Begin("Post-Processing Effects");
        ImGui::Text("FPS: %d", displayedFps);
        if (ImGui::RadioButton("No Effect", selectedEffect == NO_EFFECT)) {
            selectedEffect = NO_EFFECT;
        }
        if (ImGui::RadioButton("Grayscale", selectedEffect == GRAYSCALE)) {
            selectedEffect = GRAYSCALE;
        }
        if (ImGui::RadioButton("Mosaic", selectedEffect == MOSAIC)) {
            selectedEffect = MOSAIC;
        }
        if (ImGui::RadioButton("Scanlines", selectedEffect == SCANLINES)) {
            selectedEffect = SCANLINES;
        }
        if (ImGui::RadioButton("Sepia", selectedEffect == SEPIA)) {
            selectedEffect = SEPIA;
        }
        if (ImGui::RadioButton("Colorize", selectedEffect == COLORIZE)) {
            selectedEffect = COLORIZE;
        }
        if (ImGui::RadioButton("Box Blur", selectedEffect == BOX_BLUR)) {
            selectedEffect = BOX_BLUR;
        }


        if (selectedEffect == MOSAIC) {
            ImGui::SeparatorText("Mosaic Options");
            ImGui::SliderInt("Block Size", &mosaicBlockSize, 1, 100);
        } else if (selectedEffect == SCANLINES) {
            ImGui::SeparatorText("Scanline Options");
            ImGui::SliderInt("Line Thickness", &scanlinesLineThickness, 1, 20);
        } else if (selectedEffect == COLORIZE) {
            ImGui::SeparatorText("Colorize Options");
            ImGui::ColorEdit3("Color", &colorizeColor[0]);
        } else if (selectedEffect == BOX_BLUR) {
            ImGui::SeparatorText("Box Blur Options");
            ImGui::SliderInt("Blur Radius", &boxBlurRadius, 1, 50);
        }

        ImGui::End();

        ImGui::Render();

        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
    }

    glDeleteFramebuffers(1, &frameBufferId);
    colorPassthroughShader.cleanup();
    grayscaleShader.cleanup();
    mosaicShader.cleanup();
    scanlinesShader.cleanup();
    colorizeShader.cleanup();
    boxBlurShader.cleanup();
    screenShader.cleanup();

    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}
