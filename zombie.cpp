//using Xcode MacOs

#include <GLUT/glut.h>
#include <cmath>
#include <vector>
using namespace std;
#include <iostream>

// Constants for controlling speed
float zombie1Speed = 0.05f;  // Speed of Zombie 1
float zombie2Speed = 0.05f;  // Speed of Zombie 2

// Position variables for the zombies
float zombie1X = -0.5f, zombie1Y = 0.0f;
float zombie2X = 0.5f, zombie2Y = -0.2f;

// Scaling factors for the zombies
float zombie1Scale = 1.0f;
float zombie2Scale = 1.0f;

// Projectile structure
struct Projectile {
    float x, y, dx, dy;
    float r, g, b;
    bool active;
};

// List of projectiles
vector<Projectile> projectiles;

// Function to draw a filled rectangle
void drawRectangle(float x, float y, float width, float height) {
    glBegin(GL_QUADS);
    glVertex2f(x, y);               // Bottom-left
    glVertex2f(x + width, y);       // Bottom-right
    glVertex2f(x + width, y + height); // Top-right
    glVertex2f(x, y + height);      // Top-left
    glEnd();
}

// Function to draw a circle
void drawCircle(float cx, float cy, float radius, int segments) {
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(cx, cy);  // Center of circle
    for (int i = 0; i <= segments; i++) {
        float angle = 2.0f * 3.14159f * i / segments;
        float x = radius * cos(angle);
        float y = radius * sin(angle);
        glVertex2f(cx + x, cy + y);
    }
    glEnd();
}

// Function to draw a zombie
void drawZombie(float x, float y, float scale, float r, float g, float b) {
    glPushMatrix();
    glTranslatef(x, y, 0.0f);
    glScalef(scale, scale, 1.0f);

    // Head
    glColor3f(r, g, b);
    drawRectangle(-0.2f, 0.2f, 0.4f, 0.4f);

    // Eyes
    glColor3f(1.0f, 1.0f, 0.0f);
    drawCircle(-0.1f, 0.45f, 0.05f, 20); // Left eye
    drawCircle(0.1f, 0.45f, 0.05f, 20);  // Right eye

    glColor3f(0.0f, 0.0f, 0.0f);
    drawCircle(-0.1f, 0.45f, 0.02f, 20); // Left pupil
    drawCircle(0.1f, 0.45f, 0.02f, 20);  // Right pupil

    // Mouth
    glColor3f(1.0f, 0.0f, 0.0f);
    drawRectangle(-0.1f, 0.3f, 0.2f, 0.05f);

    // Body
    glColor3f(0.2f, 0.2f, 1.0f);
    drawRectangle(-0.15f, -0.2f, 0.3f, 0.4f);

    // Arms
    glColor3f(r, g, b);
    drawRectangle(-0.35f, 0.0f, 0.2f, 0.1f); // Left arm
    drawRectangle(0.15f, 0.0f, 0.2f, 0.1f);  // Right arm

    // Legs
    glColor3f(0.3f, 0.3f, 0.3f);
    drawRectangle(-0.15f, -0.6f, 0.1f, 0.4f); // Left leg
    drawRectangle(0.05f, -0.6f, 0.1f, 0.4f);  // Right leg

    glPopMatrix();
}

// Function to draw projectiles
void drawProjectiles() {
    for (const auto& proj : projectiles) {
        if (proj.active) {
            glColor3f(proj.r, proj.g, proj.b);
            drawCircle(proj.x, proj.y, 0.05f, 10);
        }
    }
}

// Function to handle zombie movement
void moveZombie(int key) {
    if (key == GLUT_KEY_UP) zombie1Y += zombie1Speed;
    else if (key == GLUT_KEY_DOWN) zombie1Y -= zombie1Speed;
    else if (key == GLUT_KEY_LEFT) zombie1X -= zombie1Speed;
    else if (key == GLUT_KEY_RIGHT) zombie1X += zombie1Speed;
}

void moveZombie2(unsigned char key, int x, int y) {
    if (key == 'w') zombie2Y += zombie2Speed;
    else if (key == 's') zombie2Y -= zombie2Speed;
    else if (key == 'a') zombie2X -= zombie2Speed;
    else if (key == 'd') zombie2X += zombie2Speed;
}

// Function to shoot projectiles
void shootProjectile(float x, float y, float dx, float dy, float r, float g, float b) {
    Projectile p = { x, y, dx, dy, r, g, b, true };
    projectiles.push_back(p);
}

// Function to update projectiles
void updateProjectiles() {
    for (auto& proj : projectiles) {
        if (proj.active) {
            proj.x += proj.dx;
            proj.y += proj.dy;

            if (proj.x < -1.0f || proj.x > 1.0f || proj.y < -1.0f || proj.y > 1.0f)
                proj.active = false;
        }
    }
}
// Collision detection and zombie shrinking
void checkCollisions() {
    for (auto& proj : projectiles) {
        if (proj.active) {
            // Zombie 1 collision
            if (proj.x > zombie1X - 0.2f * zombie1Scale && proj.x < zombie1X + 0.2f * zombie1Scale &&
                proj.y > zombie1Y - 0.4f * zombie1Scale && proj.y < zombie1Y + 0.4f * zombie1Scale) {

                proj.active = false;   // Deactivate the projectile
                zombie1Scale *= 0.9f;  // Shrink Zombie 1

                // Apply knockback away from the projectile's direction (make it stronger)
                float knockbackMagnitude = -1.0f; // Adjust this value to control knockback intensity
                zombie1X -= knockbackMagnitude * proj.dx;
                zombie1Y -= knockbackMagnitude * proj.dy;
            }

            // Zombie 2 collision
            if (proj.x > zombie2X - 0.2f * zombie2Scale && proj.x < zombie2X + 0.2f * zombie2Scale &&
                proj.y > zombie2Y - 0.4f * zombie2Scale && proj.y < zombie2Y + 0.4f * zombie2Scale) {

                proj.active = false;   // Deactivate the projectile
                zombie2Scale *= 0.9f;  // Shrink Zombie 2

                // Apply knockback away from the projectile's direction (make it stronger)
                float knockbackMagnitude = -1.0f; // Adjust this value to control knockback intensity
                zombie2X -= knockbackMagnitude * proj.dx;
                zombie2Y -= knockbackMagnitude * proj.dy;
            }
        }
    }
}

// Function to add a thematic background (fog, gravestones, and moon)
void drawThematicBackground() {
    // Sky
    glColor3f(0.1f, 0.1f, 0.3f);  // Dark blue sky
    drawRectangle(-1.0f, -1.0f, 2.0f, 2.0f);

    // Mist / Fog
    glColor4f(0.5f, 0.5f, 0.5f, 0.4f);  // Light gray mist with some transparency
    drawRectangle(-1.0f, -0.5f, 2.0f, 1.0f);

    // Gravestones
    glColor3f(0.6f, 0.6f, 0.6f);  // Light gray gravestones
    drawRectangle(-0.9f, -0.7f, 0.1f, 0.3f);
    drawRectangle(0.7f, -0.7f, 0.1f, 0.3f);
    drawRectangle(-0.6f, -0.9f, 0.1f, 0.3f);

    // Moon
    glColor3f(1.0f, 1.0f, 1.0f);  // White for the moon
    drawCircle(0.8f, 0.8f, 0.1f, 30);
}

// Modify the display function to include the background
void display() {
    glClear(GL_COLOR_BUFFER_BIT);

    // Draw the thematic background
    drawThematicBackground();

    // Draw the zombies and projectiles
    drawZombie(zombie1X, zombie1Y, zombie1Scale, 0.0f, 0.8f, 0.0f);
    drawZombie(zombie2X, zombie2Y, zombie2Scale, 0.6f, 0.2f, 0.6f);
    drawProjectiles();

    glutSwapBuffers();
}

void keyPress(int key, int x, int y) { moveZombie(key); }

void keyRelease(unsigned char key, int x, int y) {
    moveZombie2(key, x, y);
    if (key == 'f') shootProjectile(zombie1X + 0.3f, zombie1Y, 0.05f, 0.0f, 1.0f, 0.0f, 0.0f);
    if (key == 'j') shootProjectile(zombie2X - 0.3f, zombie2Y, -0.05f, 0.0f, 0.6f, 0.2f, 0.6f);

    // Speed control for Zombie 1 and Zombie 2
    if (key == '+') {
        zombie1Speed += 0.01f;
        zombie2Speed += 0.01f;
    }
    if (key == '-') {
        zombie1Speed = (0.01f, zombie1Speed - 0.01f);
        zombie2Speed = (0.01f, zombie2Speed - 0.01f);
    }
}

void update() {
    updateProjectiles();
    checkCollisions();
    glutPostRedisplay();
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Zombie Game");
    glMatrixMode(GL_PROJECTION);
    gluOrtho2D(-1.0, 1.0, -1.0, 1.0);
    glutDisplayFunc(display);
    glutIdleFunc(update);
    glutSpecialFunc(keyPress);
    glutKeyboardFunc(keyRelease);
    glutMainLoop();
    return 0;
}
