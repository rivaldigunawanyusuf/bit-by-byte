#include <SDL2/SDL.h>
#include <iostream>
#include <string>
#include <vector>
#include <random>
#include <ctime>

// Window size constants
const int WIDTH = 400;
const int HEIGHT = 600;

struct Button {
    SDL_Rect rect;
    std::string label;
    bool isPressed;
    
    Button(int x, int y, int w, int h, const std::string& text) 
        : rect{x, y, w, h}, label(text), isPressed(false) {}
    
    bool contains(int mouseX, int mouseY) const {
        return mouseX >= rect.x && mouseX <= rect.x + rect.w &&
               mouseY >= rect.y && mouseY <= rect.y + rect.h;
    }
};

class NumberGuessingGame {
private:
    SDL_Window* window;
    SDL_Renderer* renderer;
    
    // Game state
    int targetNumber;
    std::string currentInput;
    std::string gameMessage;
    bool gameWon;
    int attempts;
    
    // Random number generator
    std::mt19937 rng;
    std::uniform_int_distribution<int> dist;
    
    // UI elements
    std::vector<Button> numberButtons;
    Button enterButton;
    Button clearButton;
    
public:
    NumberGuessingGame() 
        : window(nullptr), renderer(nullptr),
          targetNumber(0), gameWon(false), attempts(0),
          rng(std::time(nullptr)), dist(1, 100),
          enterButton(280, 480, 100, 40, "ENTER"),
          clearButton(280, 530, 100, 40, "CLEAR") {
        
        // Initialize number buttons (0-9) in a grid layout
        int buttonWidth = 60;
        int buttonHeight = 60;
        int startX = 50;
        int startY = 300;
        int spacing = 10;
        
        // Create buttons 1-9 in 3x3 grid
        for (int i = 1; i <= 9; i++) {
            int row = (i - 1) / 3;
            int col = (i - 1) % 3;
            int x = startX + col * (buttonWidth + spacing);
            int y = startY + row * (buttonHeight + spacing);
            numberButtons.emplace_back(x, y, buttonWidth, buttonHeight, std::to_string(i));
        }
        
        numberButtons.emplace_back(startX + buttonWidth + spacing, 
                                 startY + 3 * (buttonHeight + spacing), 
                                 buttonWidth, buttonHeight, "0");
        
        generateNewNumber();
    }
    
    ~NumberGuessingGame() {
        cleanup();
    }
    
    bool initialize() {
        // Initialize SDL
        if (SDL_Init(SDL_INIT_VIDEO) < 0) {
            std::cerr << "Unable to initialize SDL: " << SDL_GetError() << std::endl;
            return false;
        }
        
        // Create window
        window = SDL_CreateWindow("Number Guessing Game", 
                                SDL_WINDOWPOS_CENTERED, 
                                SDL_WINDOWPOS_CENTERED, 
                                WIDTH, HEIGHT, 
                                SDL_WINDOW_SHOWN);
        
        if (window == nullptr) {
            std::cerr << "Unable to create window: " << SDL_GetError() << std::endl;
            SDL_Quit();
            return false;
        }
        
        // Create renderer
        renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
        if (renderer == nullptr) {
            std::cerr << "Unable to create renderer: " << SDL_GetError() << std::endl;
            SDL_DestroyWindow(window);
            SDL_Quit();
            return false;
        }
        
        std::cout << "SDL2 initialized successfully!" << std::endl;
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
    
    // Generate a new random number between 1 and 100
    void generateNewNumber() {
        targetNumber = dist(rng);
        currentInput = "";
        gameMessage = "Guess a number between 1 and 100!";
        gameWon = false;
        attempts = 0;
    }
    
    // Simple geometric text rendering
    void renderText(const std::string& text, int x, int y, SDL_Color color = {255, 255, 255, 255}) {
        if (text.empty()) return;
        
        const int charWidth = 12;
        const int charHeight = 16;
        const int spacing = 2;
        
        SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
        
        for (size_t i = 0; i < text.length(); i++) {
            char c = text[i];
            int charX = x + i * (charWidth + spacing);
            
            // Draw simple geometric characters
            if (c >= '0' && c <= '9') {
                drawDigit(c - '0', charX, y, charWidth, charHeight);
            } else if (c >= 'A' && c <= 'Z') {
                drawLetter(c, charX, y, charWidth, charHeight);
            } else if (c >= 'a' && c <= 'z') {
                drawLetter(c - 32, charX, y, charWidth, charHeight); // Convert to uppercase
            } else if (c == ' ') {
                // Space - do nothing
            } else if (c == ':') {
                SDL_Rect dot1 = {charX + charWidth/2 - 1, y + charHeight/3, 2, 2};
                SDL_Rect dot2 = {charX + charWidth/2 - 1, y + 2*charHeight/3, 2, 2};
                SDL_RenderFillRect(renderer, &dot1);
                SDL_RenderFillRect(renderer, &dot2);
            } else if (c == '!') {
                SDL_Rect line = {charX + charWidth/2 - 1, y, 2, charHeight * 3/4};
                SDL_Rect dot = {charX + charWidth/2 - 1, y + charHeight * 7/8, 2, 2};
                SDL_RenderFillRect(renderer, &line);
                SDL_RenderFillRect(renderer, &dot);
            }
        }
    }
    
    void drawDigit(int digit, int x, int y, int w, int h) {
        switch (digit) {
            case 0:
                drawRect(x, y, w, 2);           // top
                drawRect(x, y, 2, h);           // left
                drawRect(x + w - 2, y, 2, h);   // right
                drawRect(x, y + h - 2, w, 2);  // bottom
                break;
            case 1:
                drawRect(x + w - 2, y, 2, h);   // right line
                break;
            case 2:
                drawRect(x, y, w, 2);           // top
                drawRect(x + w - 2, y, 2, h/2); // right top
                drawRect(x, y + h/2 - 1, w, 2); // middle
                drawRect(x, y + h/2, 2, h/2);   // left bottom
                drawRect(x, y + h - 2, w, 2);   // bottom
                break;
            case 3:
                drawRect(x, y, w, 2);           // top
                drawRect(x + w - 2, y, 2, h);   // right
                drawRect(x, y + h/2 - 1, w, 2); // middle
                drawRect(x, y + h - 2, w, 2);   // bottom
                break;
            case 4:
                drawRect(x, y, 2, h/2);         // left top
                drawRect(x + w - 2, y, 2, h);   // right
                drawRect(x, y + h/2 - 1, w, 2); // middle
                break;
            case 5:
                drawRect(x, y, w, 2);           // top
                drawRect(x, y, 2, h/2);         // left top
                drawRect(x, y + h/2 - 1, w, 2); // middle
                drawRect(x + w - 2, y + h/2, 2, h/2); // right bottom
                drawRect(x, y + h - 2, w, 2);   // bottom
                break;
            case 6:
                drawRect(x, y, w, 2);           // top
                drawRect(x, y, 2, h);           // left
                drawRect(x, y + h/2 - 1, w, 2); // middle
                drawRect(x + w - 2, y + h/2, 2, h/2); // right bottom
                drawRect(x, y + h - 2, w, 2);   // bottom
                break;
            case 7:
                drawRect(x, y, w, 2);           // top
                drawRect(x + w - 2, y, 2, h);   // right
                break;
            case 8:
                drawRect(x, y, w, 2);           // top
                drawRect(x, y, 2, h);           // left
                drawRect(x + w - 2, y, 2, h);   // right
                drawRect(x, y + h/2 - 1, w, 2); // middle
                drawRect(x, y + h - 2, w, 2);   // bottom
                break;
            case 9:
                drawRect(x, y, w, 2);           // top
                drawRect(x, y, 2, h/2);         // left top
                drawRect(x + w - 2, y, 2, h);   // right
                drawRect(x, y + h/2 - 1, w, 2); // middle
                drawRect(x, y + h - 2, w, 2);   // bottom
                break;
        }
    }
    
    void drawLetter(char letter, int x, int y, int w, int h) {
        switch (letter) {
            case 'A':
                drawRect(x, y, w, 2);           // top
                drawRect(x, y, 2, h);           // left
                drawRect(x + w - 2, y, 2, h);   // right
                drawRect(x, y + h/2 - 1, w, 2); // middle
                break;
            case 'B':
                drawRect(x, y, 2, h);           // left
                drawRect(x, y, w - 2, 2);       // top
                drawRect(x, y + h/2 - 1, w - 2, 2); // middle
                drawRect(x, y + h - 2, w - 2, 2);   // bottom
                drawRect(x + w - 2, y + 2, 2, h/2 - 3); // right top
                drawRect(x + w - 2, y + h/2 + 1, 2, h/2 - 3); // right bottom
                break;
            case 'C':
                drawRect(x, y, w, 2);           // top
                drawRect(x, y, 2, h);           // left
                drawRect(x, y + h - 2, w, 2);   // bottom
                break;
            case 'E':
                drawRect(x, y, 2, h);           // left
                drawRect(x, y, w, 2);           // top
                drawRect(x, y + h/2 - 1, w - 2, 2); // middle
                drawRect(x, y + h - 2, w, 2);   // bottom
                break;
            case 'G':
                drawRect(x, y, w, 2);           // top
                drawRect(x, y, 2, h);           // left
                drawRect(x, y + h - 2, w, 2);   // bottom
                drawRect(x + w - 2, y + h/2, 2, h/2); // right bottom
                drawRect(x + w/2, y + h/2 - 1, w/2, 2); // middle
                break;
            case 'I':
                drawRect(x, y, w, 2);           // top
                drawRect(x + w/2 - 1, y, 2, h); // middle
                drawRect(x, y + h - 2, w, 2);   // bottom
                break;
            case 'L':
                drawRect(x, y, 2, h);           // left
                drawRect(x, y + h - 2, w, 2);   // bottom
                break;
            case 'M':
                drawRect(x, y, 2, h);           // left
                drawRect(x + w - 2, y, 2, h);   // right
                drawRect(x + w/4, y, 2, h/2);   // left diagonal
                drawRect(x + 3*w/4 - 2, y, 2, h/2); // right diagonal
                break;
            case 'N':
                drawRect(x, y, 2, h);           // left
                drawRect(x + w - 2, y, 2, h);   // right
                drawRect(x + w/3, y + h/3, 2, h/3); // diagonal
                break;
            case 'O':
                drawRect(x, y, w, 2);           // top
                drawRect(x, y, 2, h);           // left
                drawRect(x + w - 2, y, 2, h);   // right
                drawRect(x, y + h - 2, w, 2);   // bottom
                break;
            case 'R':
                drawRect(x, y, 2, h);           // left
                drawRect(x, y, w - 2, 2);       // top
                drawRect(x, y + h/2 - 1, w - 2, 2); // middle
                drawRect(x + w - 2, y + 2, 2, h/2 - 3); // right top
                drawRect(x + w/2, y + h/2 + 1, 2, h/2 - 1); // diagonal
                break;
            case 'S':
                drawRect(x, y, w, 2);           // top
                drawRect(x, y, 2, h/2);         // left top
                drawRect(x, y + h/2 - 1, w, 2); // middle
                drawRect(x + w - 2, y + h/2, 2, h/2); // right bottom
                drawRect(x, y + h - 2, w, 2);   // bottom
                break;
            case 'T':
                drawRect(x, y, w, 2);           // top
                drawRect(x + w/2 - 1, y, 2, h); // middle
                break;
            case 'U':
                drawRect(x, y, 2, h);           // left
                drawRect(x + w - 2, y, 2, h);   // right
                drawRect(x, y + h - 2, w, 2);   // bottom
                break;
            default:
                drawRect(x, y, w, 2);
                drawRect(x, y, 2, h);
                drawRect(x + w - 2, y, 2, h);
                drawRect(x, y + h - 2, w, 2);
                break;
        }
    }
    
    void drawRect(int x, int y, int w, int h) {
        SDL_Rect rect = {x, y, w, h};
        SDL_RenderFillRect(renderer, &rect);
    }
    
    // Draw a button with specified colors
    void drawButton(const Button& button, SDL_Color bgColor, SDL_Color textColor) {
        // Draw button background
        SDL_SetRenderDrawColor(renderer, bgColor.r, bgColor.g, bgColor.b, bgColor.a);
        SDL_RenderFillRect(renderer, &button.rect);
        
        // Draw button border (lighter color for better visibility)
        SDL_SetRenderDrawColor(renderer, 150, 150, 150, 255);
        SDL_RenderDrawRect(renderer, &button.rect);
        
        // Draw button text (centered)
        if (!button.label.empty()) {
            int textWidth = button.label.length() * 12; // Approximate width
            int textHeight = 16;
            int textX = button.rect.x + (button.rect.w - textWidth) / 2;
            int textY = button.rect.y + (button.rect.h - textHeight) / 2;
            renderText(button.label, textX, textY, textColor);
        }
    }
    
    void render() {
        // Clear screen with dark background
        SDL_SetRenderDrawColor(renderer, 30, 30, 40, 255);
        SDL_RenderClear(renderer);
        
        // Render game title
        renderText("NUMBER GUESSING GAME", 50, 30, {255, 255, 255, 255});
        
        // Render current input
        std::string inputDisplay = "INPUT: " + currentInput;
        renderText(inputDisplay, 50, 80, {255, 255, 100, 255});
        
        // Render game message
        renderText(gameMessage, 50, 120, {255, 255, 255, 255});
        
        // Render attempts counter
        std::string attemptsText = "ATTEMPTS: " + std::to_string(attempts);
        renderText(attemptsText, 50, 160, {200, 200, 200, 255});
        
        // Render number buttons
        for (const auto& button : numberButtons) {
            SDL_Color bgColor = button.isPressed ? SDL_Color{100, 150, 255, 255} : SDL_Color{70, 130, 200, 255};
            SDL_Color textColor = {255, 255, 255, 255};
            drawButton(button, bgColor, textColor);
        }
        
        // Render enter button
        SDL_Color enterBgColor = enterButton.isPressed ? SDL_Color{100, 200, 100, 255} : SDL_Color{50, 150, 50, 255};
        SDL_Color enterTextColor = {255, 255, 255, 255};
        drawButton(enterButton, enterBgColor, enterTextColor);
        
        // Render clear button
        SDL_Color clearBgColor = clearButton.isPressed ? SDL_Color{200, 100, 100, 255} : SDL_Color{150, 50, 50, 255};
        SDL_Color clearTextColor = {255, 255, 255, 255};
        drawButton(clearButton, clearBgColor, clearTextColor);
        
        SDL_RenderPresent(renderer);
    }
    
    void handleButtonClick(int mouseX, int mouseY) {
        // Check number buttons
        for (auto& button : numberButtons) {
            if (button.contains(mouseX, mouseY)) {
                button.isPressed = true;
                if (currentInput.length() < 3) {  // Limit to 3 digits
                    currentInput += button.label;
                } else {
                    // Auto-clear if more than 3 digits
                    currentInput = button.label;
                }
                return;
            }
        }
        
        // Check enter button
        if (enterButton.contains(mouseX, mouseY)) {
            enterButton.isPressed = true;
            submitGuess();
            return;
        }
        
        // Check clear button
        if (clearButton.contains(mouseX, mouseY)) {
            clearButton.isPressed = true;
            currentInput = "";
            return;
        }
    }
    
    void handleButtonRelease() {
        // Reset all button pressed states
        for (auto& button : numberButtons) {
            button.isPressed = false;
        }
        enterButton.isPressed = false;
        clearButton.isPressed = false;
    }
    
    void submitGuess() {
        if (currentInput.empty()) {
            gameMessage = "Please enter a number!";
            return;
        }
        
        int guess = std::stoi(currentInput);
        attempts++;
        
        if (guess == targetNumber) {
            gameMessage = "CORRECT! NUMBER WAS: " + std::to_string(targetNumber);
            gameWon = true;
            
            // Auto-reset after a short delay
            SDL_Delay(1000);  // 1 second delay
            generateNewNumber();
        } else if (guess < targetNumber) {
            gameMessage = "TOO SMALL!";
        } else {
            gameMessage = "TOO BIG!";
        }
        
        currentInput = "";  // Clear input after guess
    }
    
    void handleKeyInput(SDL_Keycode key) {
        if (key >= SDLK_0 && key <= SDLK_9) {
            // Number key pressed
            if (currentInput.length() < 3) {
                currentInput += char('0' + (key - SDLK_0));
            } else {
                currentInput = char('0' + (key - SDLK_0));
            }
        } else if (key >= SDLK_KP_0 && key <= SDLK_KP_9) {
            // Numpad key pressed
            if (currentInput.length() < 3) {
                currentInput += char('0' + (key - SDLK_KP_0));
            } else {
                currentInput = char('0' + (key - SDLK_KP_0));
            }
        } else if (key == SDLK_RETURN || key == SDLK_KP_ENTER) {
            // Enter key pressed
            submitGuess();
        } else if (key == SDLK_BACKSPACE) {
            // Backspace key pressed
            if (!currentInput.empty()) {
                currentInput.pop_back();
            }
        } else if (key == SDLK_ESCAPE) {
            // ESC key - could be used to quit or reset
            currentInput = "";
        }
    }
    
    void run() {
        if (!initialize()) {
            return;
        }
        
        bool running = true;
        SDL_Event event;
        
        while (running) {
            while (SDL_PollEvent(&event)) {
                switch (event.type) {
                    case SDL_QUIT:
                        running = false;
                        break;
                        
                    case SDL_MOUSEBUTTONDOWN:
                        if (event.button.button == SDL_BUTTON_LEFT) {
                            handleButtonClick(event.button.x, event.button.y);
                        }
                        break;
                        
                    case SDL_MOUSEBUTTONUP:
                        if (event.button.button == SDL_BUTTON_LEFT) {
                            handleButtonRelease();
                        }
                        break;
                        
                    case SDL_KEYDOWN:
                        handleKeyInput(event.key.keysym.sym);
                        break;
                }
            }
            
            render();
            SDL_Delay(16);  // ~60 FPS
        }
        
        cleanup();
    }
};

int main(int argc, char* argv[]) {
    NumberGuessingGame game;
    game.run();
    return 0;
}
