#pragma once

#include "components/simple_scene.h"
#include <lab_m1/Tema2/lab_camera.h>
#include <lab_m1/Tema2/maze_cells.h>
#include <lab_m1/Tema2/Bullet.h>
#include <lab_m1/Tema2/Enemy.h>


namespace m1
{
    class Tema2 : public gfxc::SimpleScene
    {
     public:
        Tema2();
        ~Tema2();

        void Init() override;

     private:
        void FrameStart() override;
        void Update(float deltaTimeSeconds) override;
        void FrameEnd() override;

        void OnInputUpdate(float deltaTime, int mods) override;
        void OnKeyPress(int key, int mods) override;
        void OnKeyRelease(int key, int mods) override;
        void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
        void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
        void OnWindowResize(int width, int height) override;
        void RenderMazeAndFloor();
        void RenderSimpleMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix);
        Mesh* CreateMesh(const char* name, const std::vector<VertexFormat>& vertices, const std::vector<unsigned int>& indices);
        void RenderPlayer();
        void RenderMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix);
        void SetSpawnLocationPlayer();
        bool CheckWallCollisions(implement::Camera* copyCamera);
        bool CheckProjectileCollisions(implement::Camera* copyCamera);
        void SetSpawnEnemies();
        void RenderEnemies(float deltaTimeSeconds);
        void Tema2::AnimateEnemies(float deltaTimeSeconds);
        void Tema2::RenderHealthAndTime();


     protected:
        glm::mat4 modelMatrix, playerModelMatrix, mazeModelMatrix, projectileModelMatrix;
        float translateX, translateY, translateZ, playerSpawnX, playerSpawnY, playerSpawnZ, playerAngle;
        float scaleX, scaleY, scaleZ;
        float angularStepOX, angularStepOY, angularStepOZ;
        GLenum polygonMode;
        int mazeSize;
        cell::Cell** mazeMatrix;
        implement::Camera* camera;
        glm::mat4 projectionMatrix;
        glm::mat4 modelMatrixHealth, modelMatrixRedHealth, modelMatrixTimeRemaining;
        bool renderCameraTarget;
        bool projectionType;
        bool firstPerson;
        bool firedBullet;
        bool playerAngleSet;
        float left, right, bottom, top, fov, zNear, zFar;
        glm::vec3 thirdPersonPosition;
        bullet::Bullet *projectile;
        std::vector<enemy::Enemy> enemies;
        int numberOfEnemies, healthBars, timeRemaining;
    };
}   // namespace m1
