#include <SDL2/SDL.h>
#include <iostream>
#include <cmath>

// Window size constants
const int WIDTH = 800;
const int HEIGHT = 600;

class MandelbrotRenderer {
private:
    SDL_Window* window;
    SDL_Renderer* renderer;
    
public:
    MandelbrotRenderer() : window(nullptr), renderer(nullptr) {}
    
    ~MandelbrotRenderer() {
        cleanup();
    }
    
    bool initialize() {
        // Initialize SDL
        if (SDL_Init(SDL_INIT_VIDEO) < 0) {
            std::cerr << "Unable to initialize SDL: " << SDL_GetError() << std::endl;
            return false;
        }
        
        // Create window
        window = SDL_CreateWindow("Mandelbrot Set - C++", 
                                SDL_WINDOWPOS_UNDEFINED, 
                                SDL_WINDOWPOS_UNDEFINED, 
                                WIDTH, HEIGHT, 
                                SDL_WINDOW_SHOWN);
        
        if (window == nullptr) {
            std::cerr << "Unable to create window: " << SDL_GetError() << std::endl;
            SDL_Quit();
            return false;
        }
        
        // Create renderer
        renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
        if (renderer == nullptr) {
            std::cerr << "Unable to create renderer: " << SDL_GetError() << std::endl;
            SDL_DestroyWindow(window);
            SDL_Quit();
            return false;
        }
        
        return true;
    }
    
    void cleanup() {
        if (renderer) {
            SDL_DestroyRenderer(renderer);
            renderer = nullptr;
        }
        if (window) {
            SDL_DestroyWindow(window);
            window = nullptr;
        }
        SDL_Quit();
    }
    
    // Function to check if a point is in the Mandelbrot set
    int mandelbrot(double real, double imag, int maxIterations) {
        double zReal = real;
        double zImag = imag;
        int iterations = 0;
        
        while (iterations < maxIterations && (zReal * zReal + zImag * zImag <= 4.0)) {
            double newReal = zReal * zReal - zImag * zImag + real;
            double newImag = 2.0 * zReal * zImag + imag;
            
            zReal = newReal;
            zImag = newImag;
            iterations++;
        }
        
        return iterations;
    }
    
    // Function to draw the Mandelbrot set
    void drawMandelbrot(int maxIterations = 1000) {
        // Range of complex coordinates
        const double xMin = -2.0;
        const double xMax = 1.0;
        const double yMin = -1.5;
        const double yMax = 1.5;
        
        // Clear the screen
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
        
        // Iterate through each pixel on the screen
        for (int y = 0; y < HEIGHT; ++y) {
            for (int x = 0; x < WIDTH; ++x) {
                // Calculate the complex coordinates for this pixel
                double real = xMin + (xMax - xMin) * x / static_cast<double>(WIDTH);
                double imag = yMin + (yMax - yMin) * y / static_cast<double>(HEIGHT);
                
                // Calculate Mandelbrot iterations
                int iterations = mandelbrot(real, imag, maxIterations);
                
                // Create color based on iterations with better coloring scheme
                if (iterations == maxIterations) {
                    // Point is in the set - black
                    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
                } else {
                    // Point is not in the set - colorful
                    int r = (iterations * 9) % 256;
                    int g = (iterations * 15) % 256;
                    int b = (iterations * 12) % 256;
                    SDL_SetRenderDrawColor(renderer, r, g, b, 255);
                }
                
                SDL_RenderDrawPoint(renderer, x, y);
            }
        }
        
        // Present the rendered frame
        SDL_RenderPresent(renderer);
    }
    
    void run() {
        // Draw the Mandelbrot set
        drawMandelbrot(1000);
        
        // Main event loop
        SDL_Event event;
        bool quit = false;
        
        std::cout << "Mandelbrot Set rendered! Press ESC or close window to exit." << std::endl;
        std::cout << "Controls:" << std::endl;
        std::cout << "- Press R to re-render" << std::endl;
        std::cout << "- Press ESC or close window to exit" << std::endl;
        
        while (!quit) {
            while (SDL_PollEvent(&event)) {
                switch (event.type) {
                    case SDL_QUIT:
                        quit = true;
                        break;
                    case SDL_KEYDOWN:
                        if (event.key.keysym.sym == SDLK_ESCAPE) {
                            quit = true;
                        } else if (event.key.keysym.sym == SDLK_r) {
                            std::cout << "Re-rendering Mandelbrot set..." << std::endl;
                            drawMandelbrot(1000);
                        }
                        break;
                }
            }
            
            // Small delay to prevent excessive CPU usage
            SDL_Delay(16);
        }
    }
};

int main(int argc, char* argv[]) {
    MandelbrotRenderer app;
    
    if (!app.initialize()) {
        std::cerr << "Failed to initialize application!" << std::endl;
        return 1;
    }
    
    app.run();
    
    std::cout << "Application closed successfully." << std::endl;
    return 0;
}