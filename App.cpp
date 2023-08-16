#include "App.h"


App::App() : blue({ 8, 61, 119, 255 }), green({ 38, 125, 57, 255 }), red({ 221, 49, 46, 255 }), millisecondsPerFrame(1000 / FPS) {
    initSDL();
    initInputs();
}


App::~App() {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}


void App::run() {
    appIsRunning = true;
    while (appIsRunning) {
        int ticks = SDL_GetTicks();

        updateInputs();

        SDL_SetRenderDrawColor(renderer, 30, 30, 30, SDL_ALPHA_OPAQUE);
        SDL_RenderClear(renderer);

        // RAYCASTER
        drawMap();

        updatePlayer();
        player.draw(renderer);
        updateEnemy();
        enemy.draw(renderer);

        if(DDA(player, enemy))
            SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        else
            SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);


        SDL_RenderDrawLine(renderer, player.x, player.y, enemy.x, enemy.y);

        // END
        SDL_RenderPresent(renderer);

        int elapsedTime = SDL_GetTicks() - ticks;
        if (elapsedTime < millisecondsPerFrame)
            SDL_Delay(millisecondsPerFrame - elapsedTime);
    }
}


// Private

void App::initSDL() {
    if (SDL_CreateWindowAndRenderer(WIDTH, HEIGHT, SDL_WINDOW_SHOWN, &window, &renderer) < 0) {
        cout << "SDL_CreateWindowAndRenderer() failed:\t" << SDL_GetError();
        exit(EXIT_FAILURE);
    }
    SDL_SetWindowTitle(window, "Triangles");
}


void App::initInputs() {
    pressedKeyArray.fill(false);
    appIsRunning = false;
}


void App::updateInputs() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            appIsRunning = false;
        }
        else if (event.type == SDL_KEYDOWN) {
            switch (event.key.keysym.sym) {
            case SDLK_RIGHT:
                pressedKeyArray[(int)PressedKey::RIGHT] = true;
                break;
            case SDLK_LEFT:
                pressedKeyArray[(int)PressedKey::LEFT] = true;
                break;
            case SDLK_UP:
                pressedKeyArray[(int)PressedKey::UP] = true;
                break;
            case SDLK_DOWN:
                pressedKeyArray[(int)PressedKey::DOWN] = true;
                break;
            case SDLK_d:
                pressedKeyArray[(int)PressedKey::D] = true;
                break;
            case SDLK_a:
                pressedKeyArray[(int)PressedKey::A] = true;
                break;
            case SDLK_w:
                pressedKeyArray[(int)PressedKey::W] = true;
                break;
            case SDLK_s:
                pressedKeyArray[(int)PressedKey::S] = true;
                break;
            case SDLK_z:
                pressedKeyArray[(int)PressedKey::Z] = true;
                break;
            case SDLK_x:
                pressedKeyArray[(int)PressedKey::X] = true;
                break;
            case SDLK_c:
                pressedKeyArray[(int)PressedKey::C] = true;
                break;
            case SDLK_LCTRL:
                pressedKeyArray[(int)PressedKey::CTRL] = true;
                break;
            default:
                break;
            }
        }
        else if (event.type == SDL_KEYUP) {
            switch (event.key.keysym.sym) {
            case SDLK_RIGHT:
                pressedKeyArray[(int)PressedKey::RIGHT] = false;
                break;
            case SDLK_LEFT:
                pressedKeyArray[(int)PressedKey::LEFT] = false;
                break;
            case SDLK_UP:
                pressedKeyArray[(int)PressedKey::UP] = false;
                break;
            case SDLK_DOWN:
                pressedKeyArray[(int)PressedKey::DOWN] = false;
                break;
            case SDLK_d:
                pressedKeyArray[(int)PressedKey::D] = false;
                break;
            case SDLK_a:
                pressedKeyArray[(int)PressedKey::A] = false;
                break;
            case SDLK_w:
                pressedKeyArray[(int)PressedKey::W] = false;
                break;
            case SDLK_s:
                pressedKeyArray[(int)PressedKey::S] = false;
                break;
            case SDLK_z:
                pressedKeyArray[(int)PressedKey::Z] = false;
                break;
            case SDLK_x:
                pressedKeyArray[(int)PressedKey::X] = false;
                break;
            case SDLK_c:
                pressedKeyArray[(int)PressedKey::C] = false;
                break;
            case SDLK_LCTRL:
                pressedKeyArray[(int)PressedKey::CTRL] = false;
                break;
            default:
                break;
            }
        }
    }
}


void App::drawMap() const {
    SDL_Rect rect;
    rect.w = 30;
    rect.h = rect.w;

    for (int i = 0; i < mapWidth; i++) {
        for (int j = 0; j < mapHeight; j++) {
            if (map[i][j] != 0)
            {
                SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
                rect.x = i * rect.w;
                rect.y = j * rect.h;
                SDL_RenderFillRect(renderer, &rect);
            }

            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
            rect.x = i * rect.w;
            rect.y = j * rect.h;
            SDL_RenderDrawRect(renderer, &rect);
        }
    }
}


void App::updatePlayer() {
    float speed{ 5.f };
    if (pressedKeyArray[(int)PressedKey::W])
    {
        player.y -= speed;
    }
    if (pressedKeyArray[(int)PressedKey::A])
    {
        player.x -= speed;
    }
    if (pressedKeyArray[(int)PressedKey::S])
    {
        player.y += speed;
    }
    if (pressedKeyArray[(int)PressedKey::D])
    {
        player.x += speed;
    }
}


void App::updateEnemy() {
    float speed{ 5.f };
    if (pressedKeyArray[(int)PressedKey::UP])
    {
        enemy.y -= speed;
    }
    if (pressedKeyArray[(int)PressedKey::LEFT])
    {
        enemy.x -= speed;
    }
    if (pressedKeyArray[(int)PressedKey::DOWN])
    {
        enemy.y += speed;
    }
    if (pressedKeyArray[(int)PressedKey::RIGHT])
    {
        enemy.x += speed;
    }
}


bool App::DDA(Object p, Object e) {
    vec2<float> rayStartPos{ p.x/30.f, p.y/30.f };  // 30 - szerokosc kratki
    vec2<float> rayEndPos{ e.x/30.f, e.y/30.f };

    vec2<float> rayDir{ e.x - p.x, e.y - p.y }; // roznica wektorow
    float vecLen{ sqrt(rayDir.x*rayDir.x + rayDir.y*rayDir.y) };
    // norma
    rayDir.x /= vecLen;
    rayDir.y /= vecLen;

    vec2<float> rayUnitStepSize = { sqrt(1 + (rayDir.y / rayDir.x) * (rayDir.y / rayDir.x)), sqrt(1 + (rayDir.x / rayDir.y) * (rayDir.x / rayDir.y)) }; // dlugosc promienia dla 1 vec
    vec2<int> mapCheck{ rayStartPos.x, rayStartPos.y }; // aktualny pkt
    vec2<float> rayLen1D; // dlugosc kolumn/wierszy
    vec2<int> step; // ?

    if (rayDir.x < 0) 
    {
        step.x = -1;
        rayLen1D.x = (rayStartPos.x - float(mapCheck.x)) * rayUnitStepSize.x;
    }
    else 
    {
        step.x = 1;
        rayLen1D.x = (float(mapCheck.x + 1) - rayStartPos.x) * rayUnitStepSize.x;
    }
    if (rayDir.y < 0) 
    {
        step.y = -1;
        rayLen1D.y = (rayStartPos.y - float(mapCheck.y)) * rayUnitStepSize.y;
    }
    else 
    {
        step.y = 1;
        rayLen1D.y = (float(mapCheck.y + 1) - rayStartPos.y) * rayUnitStepSize.y;
    }


    bool found{ false };
    while (!found)
    {
        if (rayLen1D.x < rayLen1D.y)
        {
            mapCheck.x += step.x;
            rayLen1D.x += rayUnitStepSize.x;
        }
        else
        {
            mapCheck.y += step.y;
            rayLen1D.y += rayUnitStepSize.y;
        }


        if (isPointBelongsToTile(rayEndPos, mapCheck))
        {
            return true;
        }

        if (map[mapCheck.x][mapCheck.y] != 0)
        {
            return false;
        }

    }
}


bool App::isPointBelongsToTile(vec2<float> point, vec2<int> tile)
{
    return int(point.x) == tile.x && int(point.y) == tile.y;
}
