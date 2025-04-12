#include <SDL2/SDL.h>
#include <stdio.h>
#include <math.h>

// Window size
const int WIDTH = 800;
const int HEIGHT = 600;

// Function to check if a point is in the Mandelbrot set
int mandelbrot(double real, double imag, int max_iter) {
    double z_real = real;
    double z_imag = imag;
    int n;

    for (n = 0; n < max_iter; ++n) {
        if (z_real * z_real + z_imag * z_imag > 4.0)
            break;

        double new_real = z_real * z_real - z_imag * z_imag + real;
        double new_imag = 2.0 * z_real * z_imag + imag;

        z_real = new_real;
        z_imag = new_imag;
    }

    return n;
}

// Function to draw the Mandelbrot set to the screen using SDL
void draw_mandelbrot(SDL_Renderer *renderer, int max_iter) {
    // Range of complex coordinates
    const double xmin = -2.0;
    const double xmax = 1.0;
    const double ymin = -1.5;
    const double ymax = 1.5;

    // Iterate through each pixel on the screen
    for (int y = 0; y < HEIGHT; ++y) {
        for (int x = 0; x < WIDTH; ++x) {
            // Calculate the complex coordinates for this pixel
            double real = xmin + (xmax - xmin) * x / WIDTH;
            double imag = ymin + (ymax - ymin) * y / HEIGHT;

            // Calculate Mandelbrot iterations
            int val = mandelbrot(real, imag, max_iter);

            // Determine color based on the number of iterations
            int color = (val * 255) / max_iter;
            SDL_SetRenderDrawColor(renderer, color, color, color, 255);
            SDL_RenderDrawPoint(renderer, x, y);
        }
    }
}

int main(int argc, char* argv[]) {
    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("Unable to initialize SDL: %s\n", SDL_GetError());
        return 1;
    }

    // Create window
    SDL_Window *window = SDL_CreateWindow("Mandelbrot Set", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WIDTH, HEIGHT, SDL_WINDOW_SHOWN);

    if (window == NULL) {
        printf("Unable to create window: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    // Create renderer
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == NULL) {
        printf("Unable to create renderer: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    // Draw Mandelbrot set
    draw_mandelbrot(renderer, 1000);

    // Display the result
    SDL_RenderPresent(renderer);

    // Wait until the user closes the window
    SDL_Event e;
    int quit = 0;
    while (!quit) {
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                quit = 1;
            }
        }
    }

    // Clean up and exit
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
