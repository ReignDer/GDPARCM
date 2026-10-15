#include <iostream>
#include <thread>
#include <SFML/Graphics.hpp>
#include <mutex>
#include <atomic>
#include <chrono>
#include <cmath>
#include <memory>

/*****************************

        USES SFML 3.0
        Link to GitHub: https://github.com/ReignDer/GDPARCM
        
*****************************/

const int WIDTH = 800;
const int HEIGHT = 600;
const int NUM_PARTICLES = 30;
const float GRAVITY = 981.0f;
const float RESTITUTION = 0.82f;

class PhysicsParticle
{
private:
    float x, y;
    float vx, vy;
    float mass;
    float radius;

    //TODO [TASK 1 - Sychronizatioon Primitive]:
    //Requirement: Add std::mutex to protect particles state during read/write ops.

    std::mutex stateMutex;

public:
    PhysicsParticle(float startX, float startY, float startVX, float startVY, float m, float r)
        : x(startX), y(startY), vx(startVX), vy(startVY), mass(m), radius(r)
    {

    }

    //TODO [TASK 2 - Physics Update Logic]:
    //Requirement:
    //1.Acquire std::lock_guard over stateMutex.
    //2.Update Position using velocity and apply gravity to vy over delta time dt
    //3.Handle boundary bounces at x<= r, x>= WIDTH-r, y<= r, y >= HEIGHT-r
    void updatePhysics(float dt)
    {
        //Implement thread-safe physics step here
        std::lock_guard<std::mutex> lock(stateMutex);

        //Aply velocity to the x position
        x += vx * dt;

        //Comment out line 58 to use the Semi-Implicit Euler on line 65 which makes gravity stable
        //Explicit Euler on Gravitational Kinematics
        //Apply velocity to the y position
        y += vy * dt + (0.5 * GRAVITY * dt * dt);

        //Apply gravity onto the y velocity for next frame
        vy += GRAVITY * dt;

        //Semi-Explicit Euler on Gravitational Kinematics
        //Implement velocity y with gravity
        //y += vy * dt;


        //Bounds
        //Left Wall
        if (x <= radius)
        {
            vx = -RESTITUTION * vx;
            x = radius;
        }
        //Right wall
        else if (x >= WIDTH - radius)
        {
            vx = -RESTITUTION * vx;
            x = WIDTH - radius;
        }
        //Ceiling
        if (y <= radius)
        {
            vy = -RESTITUTION * vy;
            y = radius;
        }
        //Floor
        else if (y >= HEIGHT - radius)
        {
            vy = -RESTITUTION * vy;
            y = HEIGHT - radius;
        }
    }

    //TODO [Task 3 - Thread-Safe State Extraction]:
    struct ParticleData
    {
        float x, y, radius;
        float kineticEnergy;
    };

    ParticleData getData()
    {
        //Implement thread-safe getter returning ParticleData struct
        std::lock_guard<std::mutex> lock(stateMutex);

        //Apply Kinetic Energy
        float ke;
        ke = 0.5 * mass * (vx * vx + vy * vy);
        return { x,y,radius,ke };
    }


};

//TODO [Task 4 - Worker Thread Routine]:
void particleThreadRoutine(PhysicsParticle& particle, std::atomic<bool>& running)
{
    auto lastTime = std::chrono::high_resolution_clock::now();

    while (running)
    {
        auto currentTime = std::chrono::high_resolution_clock::now();
        float dt = std::chrono::duration<float>(currentTime - lastTime).count();
        lastTime = currentTime;

        if (dt > 0.05f) dt = 0.05f; //Clamp step size

        particle.updatePhysics(dt);

        //Sleep ~16 ms to match 60Hz update rate
        std::this_thread::sleep_for(std::chrono::milliseconds(16));
    }
}
int main() {
    //SFML 3.0 new Window creation
	 sf::RenderWindow window(sf::VideoMode({ 800, 600 }), "My window");
     window.setFramerateLimit(60);

     //Use unique ptr as mutex is non-copyable and non-movable
     std::vector<std::unique_ptr<PhysicsParticle>> particles;
     particles.reserve(NUM_PARTICLES);

     for (int i = 0; i < NUM_PARTICLES; ++i)
     {
         particles.emplace_back(
             std::make_unique<PhysicsParticle>(
             static_cast<float>(50 + (i * 20) % (WIDTH - 100)),
             static_cast<float>(50 + (i * 15) % (HEIGHT / 2)),
             static_cast<float>((i % 5 - 2) * 80),
             static_cast<float>((i % 3) * 50),
             10.0f,
             8.0f
             )
         );
     }

     std::atomic<bool> running(true);
     std::vector<std::thread> threads;

     threads.reserve(NUM_PARTICLES);

     for (int i = 0; i < NUM_PARTICLES; ++i)
     {
         threads.emplace_back(particleThreadRoutine, std::ref(*particles[i]), std::ref(running));
     }

     //SFML 3.0 implementation for sf::Font and sf::Text
     sf::Font font;

     bool fontLoaded = font.openFromFile("arial/ARIAL.TTF");
     sf::Text hudText(font);
     if (fontLoaded)
     {
         hudText.setFont(font);
         hudText.setCharacterSize(14);
         hudText.setFillColor(sf::Color::Green);
         hudText.setPosition(sf::Vector2f(20.0f, 20.0f));
     }
    // run the program as long as the window is open
     while (window.isOpen())
     {

         //SFML 3.0 new Event
         // check all the window's events that were triggered since the last iteration of the loop
         while (const std::optional event = window.pollEvent())
         {
             // "close requested" event: we close the window
             if (event->is<sf::Event::Closed>())
                 window.close();
         }

         // clear the window with black color
         window.clear(sf::Color::Black);

         float totalKineticEnergy = 0.0f;

         for (int i = 0; i < NUM_PARTICLES; ++i)
         {
             //1.Get snapshot from particles[i].getData()
             PhysicsParticle::ParticleData particle = particles[i]->getData();
             //2.Draw sf::CircleShape at (x-r, y-r)
             sf::CircleShape circle;
             circle.setRadius(particle.radius);
             circle.setPosition(sf::Vector2f(particle.x, particle.y));
             window.draw(circle);

             //3.Accumulate totalKineticEnergy
             totalKineticEnergy += particle.kineticEnergy;
         }

         if (fontLoaded)
         {
             hudText.setString("Active Threads: " + std::to_string(NUM_PARTICLES) +
                 "\nTotal Energy: " + std::to_string(static_cast<int>(totalKineticEnergy)));
             window.draw(hudText);
         }
         // draw everything here...
         // window.draw(...);

         // end the current frame
         window.display();
     }

     running = false;
     for (auto& t : threads)
     {
         if (t.joinable())
         {
             t.join();
         }
     }
}