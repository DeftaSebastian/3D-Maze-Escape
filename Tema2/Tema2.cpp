#include "lab_m1/Tema2/Tema2.h"

#include <vector>
#include <string>
#include <iostream>

#include "lab_m1/Tema2/transform3D.h"
#include "lab_m1/Tema2/maze_cells.h"
#include "lab_m1/Tema2/functions.h"

using namespace std;
using namespace m1;


/*
 *  To find out more about `FrameStart`, `Update`, `FrameEnd`
 *  and the order in which they are called, see `world.cpp`.
 */


Tema2::Tema2()
{
}


Tema2::~Tema2()
{
}


void Tema2::Init()
{
    renderCameraTarget = false;

    camera = new implement::Camera();
    polygonMode = GL_FILL;
    projectile = new bullet::Bullet();

    {
        Mesh* mesh = new Mesh("box");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "box.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }

    {
        Mesh* mesh = new Mesh("sphere");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "sphere.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }

    // Initialize tx, ty and tz (the translation steps)
    translateX = 0;
    translateY = 0;
    translateZ = 0;

    // Initialize sx, sy and sz (the scale factors)
    scaleX = 1;
    scaleY = 1;
    scaleZ = 1;

    // Initialize angular steps
    angularStepOX = 0;
    angularStepOY = 0;
    angularStepOZ = 0;
    playerAngle = 0;
    playerAngleSet = false;

    playerSpawnX = 0;
    playerSpawnY = 0;
    playerSpawnZ = 0;

    modelMatrixHealth = glm::mat4(1);


    firedBullet = false;
    firstPerson = true;
    projectionType = true;
    fov = 40.f;
    zFar = 200.f;
    zNear = 0.01f;
    top = 10.f;
    bottom = -10.f;
    left = -10.f;
    right = 10.f;
    projectionMatrix = glm::perspective(RADIANS(fov), window->props.aspectRatio, zNear, zFar);

    healthBars = 4;
    timeRemaining = 120;

    // Create a shader program for drawing face polygon with the color of the normal
    Shader* shader = new Shader("TemaShader");
    shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Tema2", "shaders", "VertexShader.glsl"), GL_VERTEX_SHADER);
    shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Tema2", "shaders", "FragmentShader.glsl"), GL_FRAGMENT_SHADER);
    shader->CreateAndLink();
    shaders[shader->GetName()] = shader;

    Shader* floorShader = new Shader("FloorShader");
    floorShader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Tema2", "shaders", "VertexFloor.glsl"), GL_VERTEX_SHADER);
    floorShader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Tema2", "shaders", "FragmentFloor.glsl"), GL_FRAGMENT_SHADER);
    floorShader->CreateAndLink();
    shaders[floorShader->GetName()] = floorShader;

    Shader* headShader = new Shader("HeadShader");
    headShader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Tema2", "shaders", "VertexHead.glsl"), GL_VERTEX_SHADER);
    headShader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Tema2", "shaders", "FragmentHead.glsl"), GL_FRAGMENT_SHADER);
    headShader->CreateAndLink();
    shaders[headShader->GetName()] = headShader;

    Shader* torsoShader = new Shader("TorsoShader");
    torsoShader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Tema2", "shaders", "VertexTorso.glsl"), GL_VERTEX_SHADER);
    torsoShader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Tema2", "shaders", "FragmentTorso.glsl"), GL_FRAGMENT_SHADER);
    torsoShader->CreateAndLink();
    shaders[torsoShader->GetName()] = torsoShader;

    Shader* armsShader = new Shader("ArmsShader");
    armsShader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Tema2", "shaders", "VertexArms.glsl"), GL_VERTEX_SHADER);
    armsShader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Tema2", "shaders", "FragmentArms.glsl"), GL_FRAGMENT_SHADER);
    armsShader->CreateAndLink();
    shaders[armsShader->GetName()] = armsShader;

    Shader* legsShader = new Shader("LegsShader");
    legsShader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Tema2", "shaders", "VertexLegs.glsl"), GL_VERTEX_SHADER);
    legsShader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Tema2", "shaders", "FragmentLegs.glsl"), GL_FRAGMENT_SHADER);
    legsShader->CreateAndLink();
    shaders[legsShader->GetName()] = legsShader;

    Shader* bulletShader = new Shader("BulletShader");
    bulletShader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Tema2", "shaders", "VertexBullet.glsl"), GL_VERTEX_SHADER);
    bulletShader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Tema2", "shaders", "FragmentBullet.glsl"), GL_FRAGMENT_SHADER);
    bulletShader->CreateAndLink();
    shaders[bulletShader->GetName()] = bulletShader;

    Shader* enemyShader = new Shader("EnemyShader");
    enemyShader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Tema2", "shaders", "VertexEnemy.glsl"), GL_VERTEX_SHADER);
    enemyShader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Tema2", "shaders", "FragmentEnemy.glsl"), GL_FRAGMENT_SHADER);
    enemyShader->CreateAndLink();
    shaders[enemyShader->GetName()] = enemyShader;

    Shader* enemyDoubleHpShader = new Shader("EnemyDoubleHpShader");
    enemyDoubleHpShader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Tema2", "shaders", "VertexEnemyDoubleHp.glsl"), GL_VERTEX_SHADER);
    enemyDoubleHpShader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Tema2", "shaders", "FragmentEnemyDoubleHp.glsl"), GL_FRAGMENT_SHADER);
    enemyDoubleHpShader->CreateAndLink();
    shaders[enemyDoubleHpShader->GetName()] = enemyDoubleHpShader;



    // Create a simple cube
    {
        vector<VertexFormat> vertices
        {
            VertexFormat(glm::vec3(0, 0,  1), glm::vec3(0, 1, 1), glm::vec3(0.2, 0.8, 0.2)),
            VertexFormat(glm::vec3(1, 0,  1), glm::vec3(1, 0, 1), glm::vec3(0.2, 0.8, 0.2)),
            VertexFormat(glm::vec3(0,  1,  1), glm::vec3(1, 0, 0), glm::vec3(0.2, 0.8, 0.2)),
            VertexFormat(glm::vec3(1,  1,  1), glm::vec3(0, 1, 0), glm::vec3(0.2, 0.8, 0.2)),
            VertexFormat(glm::vec3(0, 0, 0), glm::vec3(1, 1, 1), glm::vec3(0.2, 0.8, 0.2)),
            VertexFormat(glm::vec3(1, 0, 0), glm::vec3(0, 1, 1), glm::vec3(0.2, 0.8, 0.2)),
            VertexFormat(glm::vec3(0,  1, 0), glm::vec3(1, 1, 0), glm::vec3(0.2, 0.8, 0.2)),
            VertexFormat(glm::vec3(1,  1, 0), glm::vec3(0, 0, 1), glm::vec3(0.2, 0.8, 0.2)),
        };

        vector<unsigned int> indices =
        {
            0, 1, 2,        1, 3, 2,
            2, 3, 7,        2, 7, 6,
            1, 7, 3,        1, 5, 7,
            6, 7, 4,        7, 5, 4,
            0, 4, 1,        1, 4, 5,
            2, 6, 4,        0, 2, 4,
        };

        CreateMesh("cube", vertices, indices);
    }

    //generate maze
    srand(time(0));
    mazeSize = 10;
    numberOfEnemies = 20;
    mazeMatrix = (cell::Cell**)malloc(mazeSize * sizeof(cell::Cell*));
    if (mazeMatrix == NULL) {
        printf("Error! memory not allocated.");
        exit(0);
    }
    for (int i = 0; i < mazeSize; i++)
    {
        mazeMatrix[i] = (cell::Cell*)calloc(mazeSize, sizeof(cell::Cell));
        if (mazeMatrix[i] == NULL) {
            printf("Error! memory not allocated.");
            exit(0);
        }
        for (int j = 0; j < mazeSize; j++)
        {
            mazeMatrix[i][j] = cell::Cell(i, j);
        }
    }
   functions::removeWalls(mazeMatrix, mazeSize);

    for (int i = 0; i < mazeSize; i++)
    {
        if (i == 0) 
        {
            for (int j = 0; j < mazeSize; j++)
            {
                if (mazeMatrix[i][j].getTopWall())
                {
                    cout << " _";
                }
                else
                {
                    cout << "  ";
                }
            }
            cout << endl;
        }
        for (int j = 0; j < mazeSize; j++)
        {
            if (j == 0)
            {
                if (mazeMatrix[i][j].getLeftWall())
                {
                    cout << "|";
                }
                else 
                {
                    cout << " ";
                }
            }
            if (mazeMatrix[i][j].getRightWall())
            {
                cout << " |";
            }
            else {
                cout << "  ";
            }
        }
        cout << endl;
        for (int j = 0; j < mazeSize; j++)
        {
            if (mazeMatrix[i][j].getBottomWall())
            {
                cout << " _";
            }
            else 
            {
                cout << "  ";
            }
        }
        cout << endl;
    }
    Tema2::SetSpawnLocationPlayer();
    Tema2::SetSpawnEnemies();
}


void Tema2::FrameStart()
{
    // Clears the color buffer (using the previously set color) and depth buffer
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Sets the screen area where to draw
    glm::ivec2 resolution = window->GetResolution();
    glViewport(0, 0, resolution.x, resolution.y);
}


void Tema2::Update(float deltaTimeSeconds)
{
    glLineWidth(3);
    glPointSize(5);
    glPolygonMode(GL_FRONT_AND_BACK, polygonMode);

    if (healthBars == 0 || timeRemaining <= 0)
    {
        cout << "Game over!";
        exit(0);
    }
    if (!firstPerson) 
    {
        playerModelMatrix = glm::mat4(1);
        RenderPlayer();
    }


    RenderMazeAndFloor();

    RenderEnemies(deltaTimeSeconds);

    if (renderCameraTarget)
    {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, camera->GetTargetPosition());
        modelMatrix = glm::scale(modelMatrix, glm::vec3(0.1f));
        RenderMesh(meshes["sphere"], shaders["VertexNormal"], modelMatrix);
    }

    if (firedBullet)
    {
        projectileModelMatrix = glm::mat4(1);
        implement::Camera *projectileCamera = new implement::Camera();
        projectileCamera->distanceToTarget = projectile->distanceToTarget;
        projectileCamera->forward = projectile->forward;
        projectileCamera->right = projectile->right;
        projectileCamera->up = projectile->up;
        projectileCamera->position = projectile->position;
        if (!Tema2::CheckProjectileCollisions(projectileCamera)) 
        {
            firedBullet = !firedBullet;
        }
        else {
            projectile->TranslateForward(deltaTimeSeconds * 2);
            float projX = projectile->position.x;
            float projY = projectile->position.y;
            float projZ = projectile->position.z;
            projectileModelMatrix *= transform3D::Translate(projX, projY, projZ);
            projectileModelMatrix *= transform3D::Scale(0.1f, 0.1f, 0.1f);
            RenderMesh(meshes["sphere"], shaders["BulletShader"], projectileModelMatrix);
        }
    }
    Tema2::RenderHealthAndTime();
}


void Tema2::FrameEnd()
{
    DrawCoordinateSystem(camera->GetViewMatrix(), projectionMatrix);
}


/*
 *  These are callback functions. To find more about callbacks and
 *  how they behave, see `input_controller.h`.
 */


void Tema2::OnInputUpdate(float deltaTime, int mods)
{
    implement::Camera* copyCamera = new implement::Camera();
    copyCamera->distanceToTarget = camera->distanceToTarget;
    copyCamera->forward = camera->forward;
    copyCamera->right = camera->right;
    copyCamera->up = camera->up;
    copyCamera->position = camera->position;
    float speed = 2;
    if (window->KeyHold(GLFW_KEY_S))
    {
        copyCamera->MoveForward(-deltaTime * speed);
        if (!firstPerson) {
            if (Tema2::CheckWallCollisions(copyCamera)) {
                camera->MoveForward(-deltaTime * speed);
            }
        }
        else {
            if (Tema2::CheckProjectileCollisions(copyCamera)) {
                camera->MoveForward(-deltaTime * speed);
            }
        }
    }
    if (window->KeyHold(GLFW_KEY_W))
    {
        copyCamera->MoveForward(deltaTime * speed);
        if (!firstPerson) {
            if (Tema2::CheckWallCollisions(copyCamera)) {
                camera->MoveForward(deltaTime * speed);
            }
        }
        else {
            if (Tema2::CheckProjectileCollisions(copyCamera)) {
                camera->MoveForward(deltaTime * speed);
            }
        }
    }
    if (window->KeyHold(GLFW_KEY_D))
    {
        copyCamera->TranslateRight(deltaTime * speed);
        if (!firstPerson) {
            if (Tema2::CheckWallCollisions(copyCamera)) {
                camera->TranslateRight(deltaTime * speed);
            }
        }
        else {
            if (Tema2::CheckProjectileCollisions(copyCamera)) {
                camera->TranslateRight(deltaTime * speed);
            }
        }
    }
    if (window->KeyHold(GLFW_KEY_A))
    {
        copyCamera->TranslateRight(-deltaTime * speed);
        if (!firstPerson) {
            if (Tema2::CheckWallCollisions(copyCamera)) {
                camera->TranslateRight(-deltaTime * speed);
            }
        }
        else {
            if (Tema2::CheckProjectileCollisions(copyCamera)) {
                camera->TranslateRight(-deltaTime * speed);
            }
        }
    }

    if (window->KeyHold(GLFW_KEY_Q))
    {
        camera->TranslateUpward(deltaTime * 10);
    }
    if (window->KeyHold(GLFW_KEY_E))
    {
        camera->TranslateUpward(-deltaTime * 10);
    }

    if (window->KeyHold(GLFW_KEY_U)) {
        // TODO(student): Translate the camera forward
        camera->TranslateForward(deltaTime * speed);
    }

    if (window->KeyHold(GLFW_KEY_H)) {
        // TODO(student): Translate the camera to the left
        camera->TranslateRight(-deltaTime * speed);
    }

    if (window->KeyHold(GLFW_KEY_J)) {
        // TODO(student): Translate the camera backward
        camera->TranslateForward(-deltaTime * speed);
    }

    if (window->KeyHold(GLFW_KEY_K)) {
        // TODO(student): Translate the camera to the right
        camera->TranslateRight(deltaTime * speed);
    }
}


void Tema2::OnKeyPress(int key, int mods)
{
    // Add key press event
    if (key == GLFW_KEY_T)
    {
        renderCameraTarget = !renderCameraTarget;
    }
    if (key == GLFW_KEY_LEFT_CONTROL)
    {
        if (!firstPerson) {
            camera->TranslateForward(3.5f);
        }
        else {
            camera->TranslateForward(-3.5f);
        }
        firstPerson = !firstPerson;
    }
    if (firstPerson && key == GLFW_KEY_SPACE) 
    {
        firedBullet = true;
        projectile->Set(camera->position, camera->forward, camera->up);
    }
}


void Tema2::OnKeyRelease(int key, int mods)
{
    // Add key release event
}


void Tema2::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
    // Add mouse move event
    if (window->MouseHold(GLFW_MOUSE_BUTTON_RIGHT))
    {
        float sensivityOX = 0.001f;
        float sensivityOY = 0.001f;

        if (firstPerson) {
            renderCameraTarget = false;
            // TODO(student): Rotate the camera in first-person mode around
            // OX and OY using `deltaX` and `deltaY`. Use the sensitivity
            // variables for setting up the rotation speed.
            camera->RotateFirstPerson_OX(sensivityOX * -deltaY);
            camera->RotateFirstPerson_OY(sensivityOY * -deltaX);
            playerAngle += sensivityOY * -deltaX;
            angularStepOX += sensivityOX * -deltaY;
            
        } else {
            renderCameraTarget = true;
            // TODO(student): Rotate the camera in third-person mode around
            // OX and OY using `deltaX` and `deltaY`. Use the sensitivity
            // variables for setting up the rotation speed.
            camera->RotateThirdPerson_OX(sensivityOX * -deltaY);
            camera->RotateThirdPerson_OY(sensivityOY * -deltaX);
            glm::vec3 c = camera->forward;
            glm::vec3 p = glm::vec3(0, 0, -1);
            float dotProduct = c.x * p.x + c.y * p.y + c.z * p.z;
            float magnitudeC = sqrt(c.x * c.x + c.y * c.y + c.z * c.z);
            float magnitudeP = sqrt(p.x * p.x + p.y * p.y + p.z * p.z);
            float magProduct = magnitudeC * magnitudeP;
            if (playerAngleSet == false) 
            {
                if(playerSpawnX < 0)
                {
                    playerAngle = -acos(dotProduct / magProduct);
                }
                else {
                    playerAngle = acos(dotProduct / magProduct);
                }
                playerAngleSet = true;
            }
            else {
                playerAngle += sensivityOY * -deltaX;
            }
            angularStepOX += sensivityOX * -deltaY;
        }
    }
}


void Tema2::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button press event
}


void Tema2::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button release event
}


void Tema2::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}


void Tema2::OnWindowResize(int width, int height)
{
}

void Tema2::RenderSimpleMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix)
{
    if (!mesh || !shader || !shader->GetProgramID())
        return;

    // Render an object using the specified shader and the specified position
    glUseProgram(shader->program);

    // TODO(student): Get shader location for uniform mat4 "Model"
    GLint modelLocation = glGetUniformLocation(shader->GetProgramID(), "Model");

    // TODO(student): Set shader uniform "Model" to modelMatrix
    glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(modelMatrix));
    // TODO(student): Get shader location for uniform mat4 "View"
    GLint viewLocation = glGetUniformLocation(shader->GetProgramID(), "View");

    // TODO(student): Set shader uniform "View" to viewMatrix
    glm::mat4 viewMatrix = GetSceneCamera()->GetViewMatrix();
    glUniformMatrix4fv(viewLocation, 1, GL_FALSE, glm::value_ptr(viewMatrix));

    // TODO(student): Get shader location for uniform mat4 "Projection"
    GLint projLocation = glGetUniformLocation(shader->GetProgramID(), "Projection");

    // TODO(student): Set shader uniform "Projection" to projectionMatrix
    glm::mat4 projectionMatrix = GetSceneCamera()->GetProjectionMatrix();
    glUniformMatrix4fv(projLocation, 1, GL_FALSE, glm::value_ptr(projectionMatrix));

    // Draw the object
    glBindVertexArray(mesh->GetBuffers()->m_VAO);
    glDrawElements(mesh->GetDrawMode(), static_cast<int>(mesh->indices.size()), GL_UNSIGNED_INT, 0);
}

void Tema2::RenderMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix)
{
    if (!mesh || !shader || !shader->program)
        return;

    // Render an object using the specified shader and the specified position
    shader->Use();
    glUniformMatrix4fv(shader->loc_view_matrix, 1, GL_FALSE, glm::value_ptr(camera->GetViewMatrix()));
    glUniformMatrix4fv(shader->loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
    glUniformMatrix4fv(shader->loc_model_matrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));

    mesh->Render();
}

Mesh* Tema2::CreateMesh(const char* name, const std::vector<VertexFormat>& vertices, const std::vector<unsigned int>& indices)
{
    unsigned int VAO = 0;
    // Create the VAO and bind it
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    // Create the VBO and bind it
    unsigned int VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    // Send vertices data into the VBO buffer
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices[0]) * vertices.size(), &vertices[0], GL_STATIC_DRAW);

    // Create the IBO and bind it
    unsigned int IBO;
    glGenBuffers(1, &IBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);

    // Send indices data into the IBO buffer
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices[0]) * indices.size(), &indices[0], GL_STATIC_DRAW);

    // ========================================================================
    // This section demonstrates how the GPU vertex shader program
    // receives data.

    // TODO(student): If you look closely in the `Init()` and `Update()`
    // functions, you will see that we have three objects which we load
    // and use in three different ways:
    // - LoadMesh   + LabShader (this lab's shader)
    // - CreateMesh + VertexNormal (this shader is already implemented)
    // - CreateMesh + LabShader (this lab's shader)
    // To get an idea about how they're different from one another, do the
    // following experiments. What happens if you switch the color pipe and
    // normal pipe in this function (but not in the shader)? Now, what happens
    // if you do the same thing in the shader (but not in this function)?
    // Finally, what happens if you do the same thing in both places? Why?

    // Set vertex position attribute
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), 0);

    // Set vertex normal attribute
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (void*)(sizeof(glm::vec3)));

    // Set texture coordinate attribute
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (void*)(2 * sizeof(glm::vec3)));

    // Set vertex color attribute
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (void*)(2 * sizeof(glm::vec3) + sizeof(glm::vec2)));
    // ========================================================================

    // Unbind the VAO
    glBindVertexArray(0);

    // Check for OpenGL errors
    CheckOpenGLError();

    // Mesh information is saved into a Mesh object
    meshes[name] = new Mesh(name);
    meshes[name]->InitFromBuffer(VAO, static_cast<unsigned int>(indices.size()));
    meshes[name]->vertices = vertices;
    meshes[name]->indices = indices;
    return meshes[name];
}

void Tema2::RenderMazeAndFloor()
{
    //a cube is of size 1

    for (int i = 0; i < mazeSize; i++)
    {
        for (int j = 0; j < mazeSize; j++) { //We start from the top-left corner
            if (mazeMatrix[i][j].getTopWall() && i == 0)
            {
                mazeModelMatrix = glm::mat4(1);
                mazeModelMatrix *= transform3D::Scale(2.f, 2.f, 1.f);
                mazeModelMatrix *= transform3D::Translate(-mazeSize/2 + 0.5 + j, 0.f, -mazeSize + 1.f);
                RenderMesh(meshes["cube"], shaders["TemaShader"], mazeModelMatrix);
            }
            if (mazeMatrix[i][j].getLeftWall() && j == 0)
            {
                mazeModelMatrix = glm::mat4(1);
                mazeModelMatrix *= transform3D::Scale(1.f, 2.f, 2.f);
                mazeModelMatrix *= transform3D::Translate(-mazeSize, 0, -mazeSize/2 + 1.f + i);
                RenderMesh(meshes["cube"], shaders["TemaShader"], mazeModelMatrix);
            }
            if (mazeMatrix[i][j].getRightWall() && j < mazeSize - 1)
            {
                mazeModelMatrix = glm::mat4(1);
                mazeModelMatrix *= transform3D::Scale(1.f, 2.f, 2.f);
                mazeModelMatrix *= transform3D::Translate(-mazeSize + 2.f + 2 * j, 0, -mazeSize / 2 + 1.f + i);
                RenderMesh(meshes["cube"], shaders["TemaShader"], mazeModelMatrix);
            }
            if (mazeMatrix[i][j].getRightWall() && j == mazeSize - 1)
            {
                mazeModelMatrix = glm::mat4(1);
                mazeModelMatrix *= transform3D::Scale(1.f, 2.f, 2.f);
                mazeModelMatrix *= transform3D::Translate(-mazeSize + 2.f + 2 * j, 0, -mazeSize / 2 + 1.f + i);
                RenderMesh(meshes["cube"], shaders["TemaShader"], mazeModelMatrix);
            }
            if (mazeMatrix[i][j].getBottomWall() && i < mazeSize - 1)
            {
                mazeModelMatrix = glm::mat4(1);
                mazeModelMatrix *= transform3D::Scale(2.f, 2.f, 1.f);
                mazeModelMatrix *= transform3D::Translate(-mazeSize / 2 + 0.5f + j, 0, -mazeSize + 3.f + 2 * i);
                RenderMesh(meshes["cube"], shaders["TemaShader"], mazeModelMatrix);
            }
            if (mazeMatrix[i][j].getBottomWall() && i == mazeSize - 1)
            {
                mazeModelMatrix = glm::mat4(1);
                mazeModelMatrix *= transform3D::Scale(2.f, 2.f, 1.f);
                mazeModelMatrix *= transform3D::Translate(-mazeSize / 2 + 0.5f + j, 0, -mazeSize + 3 + 2 * i);
                RenderMesh(meshes["cube"], shaders["TemaShader"], mazeModelMatrix);
            }
        }
    }

    for (int i = 0; i < mazeSize; i++)
    {
        for (int j = 0; j < mazeSize; j++)
        {
            mazeModelMatrix = glm::mat4(1);
            mazeModelMatrix *= transform3D::Scale(2.f, 0.1f, 2.f);
            mazeModelMatrix *= transform3D::Translate(-mazeSize / 2 + 0.5f + j, 0.5f, -mazeSize / 2 + 1.f + i);
            RenderMesh(meshes["cube"], shaders["FloorShader"], mazeModelMatrix);
        }
    }
}

void Tema2::RenderPlayer()
{
    if (!firstPerson) {
        translateX = camera->GetTargetPosition().x;
        translateZ = camera->GetTargetPosition().z;
    }
    else {
        translateX = camera->position.x;
        translateZ = camera->position.z;
    }


    //head
    playerModelMatrix *= transform3D::Translate(translateX, translateY, translateZ);
    playerModelMatrix *= transform3D::RotateOY(playerAngle);
    playerModelMatrix *= transform3D::Scale(0.25f, 0.25f, 0.25f);
    playerModelMatrix *= transform3D::Translate(-0.5f, 4.f, 0.f);
    RenderMesh(meshes["cube"], shaders["HeadShader"], playerModelMatrix);

    //torso
    playerModelMatrix *= transform3D::Scale(1.5f, 2.25f, 1.5f);
    playerModelMatrix *= transform3D::Translate(-0.15f, -1.f, -0.25f);
    RenderMesh(meshes["cube"], shaders["TorsoShader"], playerModelMatrix);

    //left hand
    playerModelMatrix *= transform3D::Scale(0.4f, 0.75f, 0.4f);
    playerModelMatrix *= transform3D::Translate(-1.15f, 0.f, 0.4f);
    RenderMesh(meshes["cube"], shaders["ArmsShader"], playerModelMatrix);

    //right hand
    playerModelMatrix *= transform3D::Scale(1.f, 1.f, 1.f);
    playerModelMatrix *= transform3D::Translate(3.8f, 0.f, 0.f);
    RenderMesh(meshes["cube"], shaders["ArmsShader"], playerModelMatrix);

    //left leg
    playerModelMatrix *= transform3D::Scale(1.f, 1.f, 1.f);
    playerModelMatrix *= transform3D::Translate(-2.5f, -1.f, 0.f);
    RenderMesh(meshes["cube"], shaders["LegsShader"], playerModelMatrix);

    //right leg
    playerModelMatrix *= transform3D::Scale(1.f, 1.f, 1.f);
    playerModelMatrix *= transform3D::Translate(1.3f, 0.f, 0.f);
    RenderMesh(meshes["cube"], shaders["LegsShader"], playerModelMatrix);

    //should I give him a butt? 
}

void Tema2::SetSpawnLocationPlayer()
{
    int length = 0;
    int heigth = 1;
    while (length % 2 == 0) {
        length = rand() % (mazeSize - 1) - rand() % (mazeSize - 2);
    }
    while (heigth % 2 == 1 || heigth % 2 == -1) {
        heigth = rand() % mazeSize - rand() % (mazeSize - 8);
    }
    playerSpawnX = length;
    playerSpawnY = 0;
    playerSpawnZ = heigth;
    camera->Set(glm::vec3(playerSpawnX + 0.5, 1, playerSpawnZ + 0.5), glm::vec3(0, 1, 0), glm::vec3(0, 1, 0));
}

bool Tema2::CheckWallCollisions(implement::Camera* copyCamera) {
    float targetX = copyCamera->GetTargetPosition().x;
    float targetZ = copyCamera->GetTargetPosition().z;
    for (int i = 0; i < mazeSize; i++)
    {
        for (int j = 0; j < mazeSize; j++)
        {
            if (mazeMatrix[i][j].getBottomWall())
            {
                if ((targetX + 0.35f > -9.0f + 2.f * j) && (targetX < -6.7f + 2.f * j)
                    && (targetZ + 0.3f > -7.f + 2.f * i) && (targetZ < -5.9f + 2.f * i))
                {
                    return false;
                }
            }
            if (mazeMatrix[i][j].getTopWall())
            {
                if ((targetX + 0.35f > -9.0f + 2.f * j) && (targetX < -7.0f + 2.f * j)
                    && (targetZ + 0.3f > -9.f + 2.f * i) && (targetZ < -7.9f + 2.f * i))
                {
                    return false;
                }
            }
            if (mazeMatrix[i][j].getLeftWall())
            {
                if ((targetX + 0.35f > -10.0f + 2.f * j) && (targetX < -8.7f + 2.f * j)
                    && (targetZ + 0.3f > -8.f + 2.f * i) && (targetZ < -5.9f + 2.f * i))
                {
                    return false;
                }
            }
            if (mazeMatrix[i][j].getRightWall())
            {
                if ((targetX + 0.35f > -8.0f + 2.f * j) && (targetX < -6.7f + 2.f * j)
                    && (targetZ  + 0.3f > -8.f + 2.f * i) && (targetZ < -5.9f + 2.f * i))
                {
                    return false;
                }
            }
        }
    }
    for (int i = 0; i < enemies.size(); i++)
    {
        if (enemies[i].alive) {
            if ((targetX + 0.725f > (enemies[i].spawnX + enemies[i].translateX)) && (targetX < (enemies[i].spawnX + enemies[i].translateX + 0.725f))
                && (targetZ + 0.65f > (enemies[i].spawnZ + enemies[i].translateZ)) && (targetZ < (enemies[i].spawnZ + enemies[i].translateZ + 0.425f)))
            {
                enemies[i].alive = false;
                healthBars--;
                return false;
            }
        }
    }
    return true;
}

bool Tema2::CheckProjectileCollisions(implement::Camera* copyCamera) {
    float targetX = copyCamera->position.x;
    float targetZ = copyCamera->position.z;
    for (int i = 0; i < mazeSize; i++)
    {
        for (int j = 0; j < mazeSize; j++)
        {
            if (mazeMatrix[i][j].getBottomWall())
            {
                if ((targetX > -9.0f + 2.f * j) && (targetX < -7.f + 2.f * j)
                    && (targetZ > -7.f + 2.f * i) && (targetZ < -6.0f + 2.f * i))
                {
                    return false;
                }
            }
            if (mazeMatrix[i][j].getTopWall())
            {
                if ((targetX > -9.0f + 2.f * j) && (targetX < -7.0f + 2.f * j)
                    && (targetZ > -9.f + 2.f * i) && (targetZ < -8.0f + 2.f * i))
                {
                    return false;
                }
            }
            if (mazeMatrix[i][j].getLeftWall())
            {
                if ((targetX > -10.0f + 2.f * j) && (targetX < -9.0f + 2.f * j)
                    && (targetZ > -8.f + 2.f * i) && (targetZ < -6.0f + 2.f * i))
                {
                    return false;
                }
            }
            if (mazeMatrix[i][j].getRightWall())
            {
                if ((targetX > -8.0f + 2.f * j) && (targetX < -7.0f + 2.f * j)
                    && (targetZ > -8.f + 2.f * i) && (targetZ < -6.0f + 2.f * i))
                {
                    return false;
                }
            }
        }
    }
    for (int i = 0; i < enemies.size(); i++)
    {
        if (enemies[i].alive) {
            if ((targetX + 0.375f > (enemies[i].spawnX + enemies[i].translateX)) && (targetX < (enemies[i].spawnX + enemies[i].translateX + 0.375f))
                && (targetZ + 0.375f > (enemies[i].spawnZ + enemies[i].translateZ)) && (targetZ < (enemies[i].spawnZ + enemies[i].translateZ + 0.375f)))
            {
                enemies[i].alive = false;
                return false;
            }
        }
    }
    return true;
}

void Tema2::SetSpawnEnemies()
{
    int length;
    int heigth;
    for (int i = 0; i < numberOfEnemies; i++) 
    {
        length = 0;
        heigth = 1;
        while (length % 2 == 0 || playerSpawnX == length) {
            length = rand() % mazeSize - rand() % mazeSize;
        }
        while (heigth % 2 == 1 || heigth % 2 == -1 || playerSpawnZ == heigth) {
            heigth = rand() % mazeSize - rand() % mazeSize;
        }
        enemies.push_back(enemy::Enemy(length + 0.375f, 1, heigth + 0.375f));
        enemies[i].length = length;
        enemies[i].heigth = heigth;
        enemies[i].modelMatrix = transform3D::Translate(enemies[i].spawnX, 0.75f, enemies[i].spawnZ);
        enemies[i].modelMatrix *= transform3D::Translate(enemies[i].translateX, enemies[i].translateY, enemies[i].translateZ);
        enemies[i].modelMatrix *= transform3D::Scale(0.75, 1.5, 0.75);
    }
}

void Tema2::RenderEnemies(float deltaTimeSeconds)
{
    for (int i = 0; i < numberOfEnemies; i++) 
    {
        enemies[i].doubleHp = false;
        if (enemies[i].alive)
        {
            Tema2::AnimateEnemies(deltaTimeSeconds);
            for (int j = 0; j < numberOfEnemies; j++)
            {
                if (i != j)
                {
                    if (enemies[j].spawnX == enemies[i].spawnX && enemies[j].spawnZ == enemies[i].spawnZ)
                    {
                        enemies[i].doubleHp = true;
                    }
                }
            }
            if (enemies[i].doubleHp) {
                RenderMesh(meshes["box"], shaders["EnemyDoubleHpShader"], enemies[i].modelMatrix);
            }
            else {
                RenderMesh(meshes["box"], shaders["EnemyShader"], enemies[i].modelMatrix);
            }
        } 
    }
}

void Tema2::AnimateEnemies(float deltaTimeSeconds)
{
    for (int i = 0; i < numberOfEnemies; i++)
    {
        if (enemies[i].move)
        {
            enemies[i].translateX += 0.05f * deltaTimeSeconds;
            enemies[i].translateZ += 0.05f * deltaTimeSeconds;
            enemies[i].modelMatrix *= transform3D::Translate(0.05f * deltaTimeSeconds, enemies[i].translateY, 0.05f * deltaTimeSeconds);
            if (enemies[i].translateX >= 0.25f)
            {
                enemies[i].move = false;
            }
        }
        else {
            enemies[i].translateX -= 0.05f * deltaTimeSeconds;
            enemies[i].translateZ -= 0.05f * deltaTimeSeconds;
            enemies[i].modelMatrix *= transform3D::Translate(-0.05f * deltaTimeSeconds, enemies[i].translateY, -0.05f * deltaTimeSeconds);
            if (enemies[i].translateX <= 0)
            {
                enemies[i].move = true;
            }
        }
    }
}

void Tema2::RenderHealthAndTime() 
{
    projectionMatrix = glm::ortho(left, right, bottom, top, zNear, zFar);

    modelMatrixHealth = glm::mat4(1);
    modelMatrixRedHealth = glm::mat4(1);
    modelMatrixTimeRemaining = glm::mat4(1);
    modelMatrixHealth *= transform3D::Translate(translateX, translateY, translateZ);
    modelMatrixHealth *= transform3D::RotateOY(playerAngle);
    modelMatrixHealth *= transform3D::RotateOX(angularStepOX);
    modelMatrixHealth *= transform3D::Translate(5, 7, 0);
    modelMatrixHealth *= transform3D::Scale(1 * healthBars, 1, 0.01f);
    RenderMesh(meshes["cube"], shaders["VertexNormal"], modelMatrixHealth);
    
    modelMatrixRedHealth *= transform3D::Translate(translateX, translateY, translateZ);
    modelMatrixRedHealth *= transform3D::RotateOY(playerAngle);
    modelMatrixRedHealth *= transform3D::RotateOX(angularStepOX);
    modelMatrixRedHealth *= transform3D::Translate(5, 7, 0);
    modelMatrixRedHealth *= transform3D::Scale(4, 1, 0);
    RenderMesh(meshes["cube"], shaders["EnemyDoubleHpShader"], modelMatrixRedHealth);

    modelMatrixTimeRemaining *= transform3D::Translate(translateX, translateY, translateZ);
    modelMatrixTimeRemaining *= transform3D::RotateOY(playerAngle);
    modelMatrixTimeRemaining *= transform3D::RotateOX(angularStepOX);
    modelMatrixTimeRemaining *= transform3D::Translate(5, 6, 0);
    modelMatrixTimeRemaining *= transform3D::Scale(4 - 4 * glfwGetTime() / timeRemaining , 1, 0);
    RenderMesh(meshes["cube"], shaders["ArmsShader"], modelMatrixTimeRemaining);

    projectionMatrix = glm::perspective(RADIANS(fov), window->props.aspectRatio, zNear, zFar);
}