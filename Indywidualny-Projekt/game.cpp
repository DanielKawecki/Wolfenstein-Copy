#include <iostream>
#include <fstream>
#include <string>
#include "game.h"

bool compareDistance(const Drawable& a, const Drawable& b) {
    return a.distance > b.distance;
}

Game::Game() {
    initializeGL();
    inicializeTextures();
    clock = Clock();
    clock.start();
}

Game::~Game() {
    if (window) {
        glfwDestroyWindow(window);
        glfwTerminate();
    }
}

void Game::initializeGL() {
    /* Initialize the library */
    if (!glfwInit())
        throw std::runtime_error("Failed to initialize GLFW");

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(screen_width, screen_height, window_title, nullptr, nullptr);
    if (!window) {
        glfwTerminate();
        throw std::runtime_error("Failed to create GLFW window");
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);
    glOrtho(0, screen_width, screen_height, 0, -1, 1);
}

void Game::initilizeMap(/*map number*/) {
    map_layout.clear();

    try {
        std::ifstream file("assets/maps/map1.txt");
        if (!file.is_open()) {
            throw std::runtime_error("Error opening file");
        }
        std::string line;
        while (std::getline(file, line)) {
            map_layout.push_back(line);
        }
        file.close();
    }
    catch (const std::exception& e) {
        std::cerr << "Exception occurred: " << e.what() << std::endl;
    }
}

void Game::readFromMap() {
    bsf_tiles.resize(map_layout.size(), std::vector<Tile>(map_layout[0].size(), Tile(0.f, 0.f, false)));
    all_enemies.clear();
    all_refills.clear();
    enemy_count = 0;

    for (size_t y = 0; y < map_layout.size(); ++y) {
        for (size_t x = 0; x < map_layout[y].size(); ++x) {
            if (map_layout[y][x] == 'p') {
                player.setPosition(x * 64 + 32, y * 64 + 32);
                bsf_tiles[y][x] = Tile(x * tile_size, y * tile_size, false);
            }
                
            else if (map_layout[y][x] == 'e') {
                Enemy enemy(x * 64 + 32, y * 64 + 32, -32.f);
                all_enemies.push_back(enemy);
                bsf_tiles[y][x] = Tile(x * tile_size, y * tile_size, false);
                enemy_count += 1;
            }

            else if (map_layout[y][x] == 'a') {
                Refill ammo;
                ammo.x = x * tile_size + tile_size / 2;
                ammo.y = y * tile_size + tile_size / 2;
                ammo.type = "ammo";
                all_refills.push_back(ammo);
            }

            else if (map_layout[y][x] == 'h') {
                Refill health;
                health.x = x * tile_size + tile_size / 2;
                health.y = y * tile_size + tile_size / 2;
                health.type = "health";
                all_refills.push_back(health);
            }

            else if (stringContains(wall_chars, map_layout[y][x])) {
                bsf_tiles[y][x] = Tile(x * tile_size, y * tile_size, true);
            }
            
            else {
                bsf_tiles[y][x] = Tile(x * tile_size, y * tile_size, false);
            }

            for (int di = -1; di <= 1; ++di) {
                for (int dj = -1; dj <= 1; ++dj)
                    if (di * di + dj * dj == 1)
                    {
                        int ni = static_cast<int>(y) + di;
                        int nj = static_cast<int>(x) + dj;
                        if (ni >= 0 && ni < bsf_tiles.size() && nj >= 0 && nj < bsf_tiles[y].size() && !bsf_tiles[ni][nj].isWall()) {
                            bsf_tiles[y][x].addNeighbor(&bsf_tiles[ni][nj]);
                        }
                    }
            }
        }
    }
}

void Game::inicializeTextures() {
    textures.title_screen = loadTexture("assets/textures/title_screen.png");
    textures.highlight = loadTexture("assets/textures/wings.png");
    textures.test = loadTexture("assets/textures/test.png");
    textures.greystone = loadTexture("assets/textures/greystone.png");
    textures.eagle = loadTexture("assets/textures/eagle.png");
    textures.red_brick = loadTexture("assets/textures/redbrick.png");
    textures.guard_stationary = loadTexture("assets/sprites/guard_stationary.png");
    textures.health_pack = loadTexture("assets/sprites/health.png");
    textures.ammo_pack = loadTexture("assets/sprites/ammo.png");

    guard_textures.run0 = loadTexture("assets/sprites/guard/run0.png");
    guard_textures.run1 = loadTexture("assets/sprites/guard/run1.png");
    guard_textures.run2 = loadTexture("assets/sprites/guard/run2.png");
    guard_textures.run3 = loadTexture("assets/sprites/guard/run3.png");
    guard_textures.aim = loadTexture("assets/sprites/guard/aim.png");
    guard_textures.shoot = loadTexture("assets/sprites/guard/shoot.png");
    guard_textures.death1 = loadTexture("assets/sprites/guard/death1.png");
    guard_textures.death2 = loadTexture("assets/sprites/guard/death2.png");
    guard_textures.death3 = loadTexture("assets/sprites/guard/death3.png");
    guard_textures.death4 = loadTexture("assets/sprites/guard/death4.png");
    guard_textures.death5 = loadTexture("assets/sprites/guard/death5.png");

    texture_atlas.insert({ '#', textures.test });
    texture_atlas.insert({ '1', textures.greystone });
    texture_atlas.insert({ '2', textures.eagle });
    texture_atlas.insert({ '3', textures.red_brick });
}

GLuint Game::loadTexture(const char* texturePath) {
    GLuint textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);

    int width, height, channels;
    unsigned char* image = SOIL_load_image(texturePath, &width, &height, &channels, SOIL_LOAD_RGBA);
    if (!image) {
        std::cerr << "Failed to load texture: " << texturePath << std::endl;
        return 0;
    }

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
    SOIL_free_image_data(image);

    // Set texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    // Set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glBindTexture(GL_TEXTURE_2D, 0);

    return textureID;
}

void Game::updatePlaying() {
    //checkForDead();
    setDeltaTime();
    player.handleInput(window, delta_time);
    shoot();
    updateEnemies();
    updateRefills();

    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        pushState(new MenuState(this));
    }
    else if (player.isDead()) {
        pushState(new DeadState(this));
    }
}

void Game::renderPlaying() {
    /* Render here */
    glClear(GL_COLOR_BUFFER_BIT);

    glColor3f(0.44f, 0.44f, 0.44f);
    glBegin(GL_QUADS);
    glVertex2f(0, screen_height);
    glVertex2f(screen_width, screen_height);
    glVertex2f(screen_width, screen_height / 2);
    glVertex2f(0, screen_height / 2);
    glEnd();

    glColor3f(0.22f, 0.22f, 0.22f);
    glBegin(GL_QUADS);
    glVertex2f(0, screen_height / 2);
    glVertex2f(screen_width, screen_height / 2);
    glVertex2f(screen_width, 0);
    glVertex2f(0, 0);
    glEnd();

    all_drawables.clear();
    drawRays3d();
    drawEnemies();
    drawRefills();
    //drawMap2d();
    //drawPlayer2d();
    //drawEnemies();

    std::sort(all_drawables.begin(), all_drawables.end(), compareDistance);
    drawDrawable();

    /* Swap front and back buffers */
    glfwSwapBuffers(window);
}

void Game::updateMenu() {
    setDeltaTime();
    menuHandleInput();    
}

void Game::renderMenu() {
    /* Render here */
    glClear(GL_COLOR_BUFFER_BIT);

    glColor3f(1.f, 1.f, 1.f);

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, textures.title_screen);

    glBegin(GL_QUADS);
    glTexCoord2f(0, 0); glVertex2d(0, 0);
    glTexCoord2f(1, 0); glVertex2d(screen_width, 0);
    glTexCoord2f(1, 1); glVertex2d(screen_width, screen_height);
    glTexCoord2f(0, 1); glVertex2d(0, screen_height);
    glEnd();

    glBindTexture(GL_TEXTURE_2D, 0);
    
    // Menu highlight
    int highlight_x = 303;
    int highlight_y = 290 + highlight * 70;
    int width = 360;
    int height = 75;

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, textures.highlight);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glPointSize(16);
    glColor3f(1, 1, 1);
    glBegin(GL_QUADS);
    glTexCoord2f(0, 0); glVertex2d(highlight_x, highlight_y);
    glTexCoord2f(1, 0); glVertex2d(highlight_x + width, highlight_y);
    glTexCoord2f(1, 1); glVertex2d(highlight_x + width, highlight_y + height);
    glTexCoord2f(0, 1); glVertex2d(highlight_x, highlight_y + height);
    glEnd();

    glDisable(GL_BLEND);
    glBindTexture(GL_TEXTURE_2D, 0);

    /* Swap front and back buffers */
    glfwSwapBuffers(window);
}

void Game::setDeltaTime() {
    current_time = static_cast<float>(glfwGetTime());
    delta_time = current_time - previous_time;
    previous_time = current_time;

    if (clock.getElapsedTime() > 5) {
        std::cout << 1 / delta_time << "fps" << std::endl;
        clock.restart();
    }
}

void Game::updateDead() {
    if (isEnterPressed()) {
        switch (highlight) {
        case 0:
            initilizeMap();
            player = Player();
            player.setWalls(map_layout, wall_chars);
            readFromMap();
            pushState(new PlayingState(this));
            break;
        case 1:
            highlight = 0;
            pushState(new MenuState(this));
            break;
        case 2:
            glfwSetWindowShouldClose(window, 1);
            break;
        default:
            break;
        }
    }

    static bool is_W_pressed = false;
    static bool is_S_pressed = false;

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS && !is_W_pressed) {
        is_W_pressed = true;
        highlight -= 1;
        if (highlight < 0)
            highlight = dead_buttons_count - 1;
    }
    else if (glfwGetKey(window, GLFW_KEY_W) == GLFW_RELEASE)
        is_W_pressed = false;


    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS && !is_S_pressed) {
        is_S_pressed = true;
        highlight += 1;
        if (highlight > dead_buttons_count - 1)
            highlight = 0;
    }
    else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_RELEASE)
        is_S_pressed = false;
}

void Game::renderDead() {
    glClear(GL_COLOR_BUFFER_BIT);

    glColor3f(1.f, 0.f, 0.f);

    // Menu highlight
    int highlight_x = 303;
    int highlight_y = 290 + highlight * 70;
    int width = 360;
    int height = 75;

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, textures.highlight);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glPointSize(16);
    glColor3f(1, 1, 1);
    glBegin(GL_QUADS);
    glTexCoord2f(0, 0); glVertex2d(highlight_x, highlight_y);
    glTexCoord2f(1, 0); glVertex2d(highlight_x + width, highlight_y);
    glTexCoord2f(1, 1); glVertex2d(highlight_x + width, highlight_y + height);
    glTexCoord2f(0, 1); glVertex2d(highlight_x, highlight_y + height);
    glEnd();

    glDisable(GL_BLEND);
    glBindTexture(GL_TEXTURE_2D, 0);

    /* Swap front and back buffers */
    glfwSwapBuffers(window);
}

void Game::drawRays3d() {
    int depth_of_field;
    int map_x;
    int map_y;
    int map_position;
    float length = 0;
    float player_x = player.getX();
    float player_y = player.getY();
    float ray_angle = 0;
    float ray_x;
    float ray_y;
    float x_offset = 64;
    float y_offset = 64;
    float player_angle = player.getAngle();
    float shading;
    float hit_position = 0.f;
    float slice_offset = 0.f;
    ray_angle = player_angle - FOV / 2.f;
    GLuint texture;
    GLuint texture_horiz;
    GLuint texture_vert;

    if (ray_angle < 0)
        ray_angle += 2 * PI;
    if (ray_angle > 2 * PI)
        ray_angle -= 2 * PI;

    for (int rays = 0; rays < number_of_rays; rays++) {
        ////--- Chcecking Horizontal Lines ---
        depth_of_field = 0;
        float horizontal_distance = 1000000;
        float horizontal_x = player_x;
        float horizontal_y = player_y;
        float angle_tangent = -1 / tan(ray_angle);
        if (ray_angle > PI) {
            ray_y = (((int)player_y >> 6) << 6) - 0.0001;
            ray_x = (player_y - ray_y) * angle_tangent + player_x;
            y_offset = -tile_size;
            x_offset = -y_offset * angle_tangent;
        }
        if (ray_angle < PI) {
            ray_y = (((int)player_y >> 6) << 6) + tile_size;
            ray_x = (player_y - ray_y) * angle_tangent + player_x;
            y_offset = tile_size;
            x_offset = -y_offset * angle_tangent;
        }
        if (ray_angle == 0 || ray_angle == PI) {
            ray_x = player_x;
            ray_y = player_y;
            depth_of_field = map_width;
        }
        while (depth_of_field < map_width) {
            map_x = (int)(ray_x) >> 6;
            map_y = (int)(ray_y) >> 6;
            map_position = map_y * map_width + map_x;

            if (map_x < map_width && map_x >= 0 && stringContains(wall_chars, map_layout[map_y][map_x])) {
                horizontal_x = ray_x;
                horizontal_y = ray_y;
                horizontal_distance = getRayLength(player_x, player_y, horizontal_x, horizontal_y, ray_angle);

                texture_horiz = texture_atlas.find(map_layout[map_y][map_x])->second;
                depth_of_field = map_width; // Ending the while loop
            }
            else {
                ray_x += x_offset;
                ray_y += y_offset;
                depth_of_field += 1;
            }
        }

        //--- Chcecking Vertical Lines ---
        depth_of_field = 0;
        float vertical_distance = 1000000;
        float vertical_x = player_x;
        float vertical_y = player_y;
        float negative_tangent = -tan(ray_angle);
        if (ray_angle > (PI / 2.f) && ray_angle < (3.f * PI / 2.f)) {
            ray_x = (((int)player_x >> 6) << 6) - 0.0001;
            ray_y = (player_x - ray_x) * negative_tangent + player_y;
            x_offset = -tile_size;
            y_offset = -x_offset * negative_tangent;
        }
        if (ray_angle < (PI / 2.f) || ray_angle > (3.f * PI / 2.f)) {
            ray_x = (((int)player_x >> 6) << 6) + tile_size;
            ray_y = (player_x - ray_x) * negative_tangent + player_y;
            x_offset = tile_size;
            y_offset = -x_offset * negative_tangent;
        }
        if (ray_angle == 0 || ray_angle == PI) {
            ray_x = player_x;
            ray_y = player_y;
            depth_of_field = map_width;
        }
        while (depth_of_field < map_width) {
            map_x = (int)(ray_x) >> 6;
            map_y = (int)(ray_y) >> 6;
            map_position = map_y * map_width + map_x;

            if (map_y < map_width && map_y >= 0 && stringContains(wall_chars, map_layout[map_y][map_x])) {
                vertical_x = ray_x;
                vertical_y = ray_y;
                vertical_distance = getRayLength(player_x, player_y, vertical_x, vertical_y, ray_angle);

                texture_vert = texture_atlas.find(map_layout[map_y][map_x])->second;
                depth_of_field = map_width;
            }
            else {
                ray_x += x_offset;
                ray_y += y_offset;
                depth_of_field += 1;
            }
        }

        if (vertical_distance < horizontal_distance) {
            ray_x = vertical_x;
            ray_y = vertical_y;
            length = vertical_distance;
            hit_position = ray_y;
            shading = (64.f * 2) / length; if (shading >= 1) { shading = 1; };
            glColor3f(0.6 * shading, 0.6 * shading, 0.6 * shading);
            texture = texture_vert;

            float whole;
            slice_offset = std::modff(floor(hit_position) / 64.f, &whole);

            if (ray_angle < 270 * Degree && ray_angle > 90 * Degree)
                slice_offset = 1.f - slice_offset;
        }
        else if (vertical_distance > horizontal_distance) {
            ray_x = horizontal_x;
            ray_y = horizontal_y;
            length = horizontal_distance;
            hit_position = ray_x;
            shading = (64.f * 2) / length; if (shading >= 1) { shading = 1; };
            glColor3f(0.7 * shading, 0.7 * shading, 0.7 * shading); 
            texture = texture_horiz;

            float whole;
            slice_offset = std::modff(floor(hit_position) / 64.f, &whole);

            if (ray_angle < 180 * Degree)
                slice_offset = 1.f - slice_offset;
        }

        //--- 3D Projection ---
        float cosine_angle = player_angle - ray_angle;

        if (cosine_angle < 0)
            cosine_angle += 2 * PI;
        if (cosine_angle > 2 * PI)
            cosine_angle -= 2 * PI;

        length = length * cos(cosine_angle);
        float line_height = (tile_size * projection_distance) / length;
        float line_offset = screen_height / 2.f - line_height / 2.f;

        Drawable slice = { rays * 2, line_offset, 2, line_height, texture, length, slice_offset, "slice"};
        all_drawables.push_back(slice);

        ray_angle += delta_angle;

        if (ray_angle < 0)
            ray_angle += 2 * PI;
        if (ray_angle > 2 * PI)
            ray_angle -= 2 * PI;
    }
}

float Game::getRayLength(float a_x, float a_y, float b_x, float b_y, float angle) {
    return (sqrt((b_x - a_x) * (b_x - a_x) + (b_y - a_y) * (b_y - a_y)));
}

void Game::popState() {
    delete states.top();
    states.top();
}

void Game::changeState(State* state_) {
    if (!states.empty())
        popState();
    pushState(state_);
}

State* Game::peekState()
{
    if (states.empty())
        return nullptr;

    return states.top();
}

void Game::menuHandleInput() {
    if (isEnterPressed()) {
        switch (highlight) {
        case 0:
            initilizeMap();
            player = Player();
            player.setWalls(map_layout, wall_chars);
            readFromMap();
            pushState(new PlayingState(this));
            break;
        case 1:
            glfwSetWindowShouldClose(window, 1);
            break;
        default:
            break;
        }
    }

    static bool is_W_pressed = false;
    static bool is_S_pressed = false;

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS && !is_W_pressed) {
        is_W_pressed = true;
        highlight -= 1;
        if (highlight < 0)
            highlight = menu_buttons_count - 1;
    }
    else if (glfwGetKey(window, GLFW_KEY_W) == GLFW_RELEASE)
        is_W_pressed = false;


    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS && !is_S_pressed) {
        is_S_pressed = true;
        highlight += 1;
        if (highlight > menu_buttons_count - 1)
            highlight = 0;
    }
    else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_RELEASE)
        is_S_pressed = false;
}

void Game::updateEnemies() {
    player_tile = getTile((int)floor(player.getX() / 64.f), (int)floor(player.getY() / 64.f));

    for (auto& enemy : all_enemies) {
        if (enemy.isAlive()) {
            enemy.setVision(!visionCheck(enemy.getX(), enemy.getY()));
            enemy_tile = getTile((int)floor(enemy.getX() / 64.f), (int)floor(enemy.getY() / 64.f));

            enemy.seekPlayer(enemy_tile, player_tile, bsf_tiles);
            enemy.checkAgro(player.getX(), player.getY());
            if (enemy.shoot()) {
                player.substractHealth();
            }

            enemy.update(delta_time);
        }
        else if (enemy.isDying())
            enemy.die();
    }
}

void Game::updateRefills() {
    for (auto it = all_refills.begin(); it != all_refills.end(); ) {
        float distance = hypotf(it->x - player.getX(), it->y - player.getY());
        if (distance < 50.f) {
            if (it->type == "ammo") {
                player.addAmmo();
            }
            else if (it->type == "health") {
                player.addHealth();
            }
            it = all_refills.erase(it);
        }
        else {
            ++it;
        }
    }
}

void Game::drawPlayer2d() {

    glColor3f(0, 1, 0);
    glLineWidth(1);
    glBegin(GL_QUADS);
    glVertex2f(player.getRect().left, player.getRect().top);
    glVertex2f(player.getRect().right, player.getRect().top);
    glVertex2f(player.getRect().right, player.getRect().bottom);
    glVertex2f(player.getRect().left, player.getRect().bottom);
    glEnd();

    // Direction that player looks at
    float angleRadians = player.getAngle();

    float endX = player.getX() + 20.0f * cos(angleRadians);
    float endY = player.getY() + 20.0f * sin(angleRadians);

    glLineWidth(2);
    glBegin(GL_LINES);
    glVertex2f(player.getX(), player.getY());
    glVertex2f(endX, endY);
    glEnd();
}

void Game::drawMap2d() {
    int x0 = 0;
    int y0 = 0;
    int tile_size = 64;

    for (size_t y = 0; y < map_layout.size(); ++y) {
        for (size_t x = 0; x < map_layout[y].size(); ++x) {
            if (stringContains(wall_chars, map_layout[y][x]))
                glColor3f(1, 1, 1);
            else 
                glColor3f(0, 0, 0);
            
            x0 = x * tile_size;
            y0 = y * tile_size;
            glBegin(GL_QUADS);
            glVertex2i(x0 + 1, y0 + 1);
            glVertex2i(x0 + 1, y0 + tile_size - 1);
            glVertex2i(x0 + tile_size - 1, y0 + tile_size - 1);
            glVertex2i(x0 + tile_size - 1, y0 + 1);
            glEnd();
        }
    }

}

void Game::drawLine(float a_x, float a_y, float b_x, float b_y) {
    glLineWidth(1);
    glBegin(GL_LINES);
    glVertex2i(a_x, a_y);
    glVertex2i(b_x, b_y);
    glEnd();
}

void Game::drawPoint(float x, float y) {
    glColor3f(1, 1, 0);
    glLineWidth(3);
    glBegin(GL_POINTS);
    glVertex2i(x, y);
    glEnd();
}

void Game::drawSlice(float x, float y, float size_x, float size_y, GLuint texture, float distance, float slice_offset) {
    float shading = 0.8; // (tile_size * 2) / distance; if (shading >= 1) { shading = 1; };
    glColor3f(1.f * shading, 1.f * shading, 1.f * shading);

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glBegin(GL_QUADS);
    glTexCoord2f(slice_offset, 0.0f); glVertex2f(x, y);
    glTexCoord2f(slice_offset + 1.f / tile_size, 0.0f); glVertex2f(x + size_x, y);
    glTexCoord2f(slice_offset + 1.f / tile_size, 1.0f); glVertex2f(x + size_x, y + size_y);
    glTexCoord2f(slice_offset, 1.0f); glVertex2f(x, y + size_y);
    glEnd();

    glBindTexture(GL_TEXTURE_2D, 0);
}

void Game::drawRefills() {
    float player_angle = player.getAngle();
    GLuint texture;

    for (int i = 0; i < all_refills.size(); i++) {

        float sprite_x = all_refills[i].x - player.getX();
        float sprite_y = all_refills[i].y - player.getY();
        float sprite_z = all_refills[i].z;

        float cosine = -cos(player_angle);
        float sine = sin(player_angle);
        float a = sprite_y * cosine + sprite_x * sine;
        float b = sprite_x * cosine - sprite_y * sine;

        float theta = atan2f(sprite_y, sprite_x) - player_angle;
        float distance = hypotf(sprite_x, sprite_y) * cos(theta);

        float dot_product = sprite_x * cos(player_angle) + sprite_y * sin(player_angle);

        if (dot_product > 0) {
            sprite_x = a;
            sprite_y = b;


            float size = all_refills[i].width * (all_refills[i].width / distance);

            sprite_x = (sprite_x * (projection_distance / 8.f) / sprite_y) + ((screen_width / 8.f) / 2.f);
            sprite_y = (sprite_z * (projection_distance / 8.f) / sprite_y) + ((screen_height / 8.f) / 2.f);

            if (all_refills[i].type == "ammo")
                texture = textures.ammo_pack;
            else if (all_refills[i].type == "health")
                texture = textures.health_pack;

            Drawable refill = { sprite_x * 8.f, sprite_y * 8.f, size, size, texture, distance, 0, "sprite" };
            all_drawables.push_back(refill);
        }
    }
}

void Game::drawEnemies() {
    float player_angle = player.getAngle();

    for (int i = 0; i < all_enemies.size(); i++) {
        
        float sprite_x = all_enemies[i].getX() - player.getX();
        float sprite_y = all_enemies[i].getY() - player.getY();
        float sprite_z = all_enemies[i].getZ();

        float cosine = -cos(player_angle);
        float sine = sin(player_angle);
        float a = sprite_y * cosine + sprite_x * sine;
        float b = sprite_x * cosine - sprite_y * sine;
        
        float theta = atan2f(sprite_y, sprite_x) - player_angle;
        float distance = hypotf(sprite_x, sprite_y) * cos(theta);

        float dot_product = sprite_x * cos(player_angle) + sprite_y * sin(player_angle);

        if (dot_product > 0) {
            sprite_x = a;
            sprite_y = b;

            float size = 180.f * (256.f / distance);

            sprite_x = (sprite_x * (projection_distance / 8.f) / sprite_y) + ((screen_width / 8.f) / 2.f);
            sprite_y = (sprite_z * (projection_distance / 8.f) / sprite_y) + ((screen_height / 8.f) / 2.f);
            //getEnemyTexture(all_enemies[i].getStationary(), all_enemies[i].getShooting(), all_enemies[i].getSuccesfulShot(), all_enemies[i].getTextureRunning());
            getEnemyTexture(all_enemies[i]);

            all_enemies[i].setScreenX(sprite_x * 8.f);

            Drawable enemy = { sprite_x * 8.f, sprite_y * 8.f, size, size, current_guard_texture, distance, 0, "sprite" };
            all_drawables.push_back(enemy);

        }
        /*if (all_enemies[i].getVision())
            drawLine(player.getX(), player.getY(), all_enemies[i].getX(), all_enemies[i].getY());

        drawPoint(all_enemies[i].getX(), all_enemies[i].getY());*/
    }
}

void Game::drawDrawable() {
    for (const Drawable& it : all_drawables) {
        if (it.type == "slice")
            drawSlice(it.x, it.y, it.size_x, it.size_y, it.texture, it.distance, it.slice_offset);
        else if (it.type == "sprite") {
            glColor3f(1.f, 1.f, 1.f);
            
            glEnable(GL_TEXTURE_2D);
            glBindTexture(GL_TEXTURE_2D, it.texture);

            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

            glBegin(GL_QUADS);
            glTexCoord2f(0.f, 1.f); glVertex2f(it.x - it.size_x / 2.f, it.y);
            glTexCoord2f(1.f, 1.f); glVertex2f(it.x + it.size_x / 2.f, it.y);
            glTexCoord2f(1.f, 0.f); glVertex2f(it.x + it.size_x / 2.f, it.y - it.size_y);
            glTexCoord2f(0.f, 0.f); glVertex2f(it.x - it.size_x / 2.f, it.y - it.size_y);
            glEnd();

            glDisable(GL_BLEND);
            glBindTexture(GL_TEXTURE_2D, 0);
        }
    }
}

bool Game::stringContains(const std::string& string, char ch) {
    for (char c : string) {
        if (c == ch)
            return true;
    }
    return false;
}

Tile* Game::getTile(int row, int column) {
    return &bsf_tiles[column][row];
}

bool Game::isEnterPressed() {
    if (glfwGetKey(window, GLFW_KEY_ENTER) == GLFW_PRESS && !is_Enter_pressed) {
        is_Enter_pressed = true;
        return true;
    }
    else if (glfwGetKey(window, GLFW_KEY_ENTER) == GLFW_RELEASE) {
        is_Enter_pressed = false;
        return false;
    }

    return false;
}

void Game::pushState(State* state_) {
    states.push(state_);
}

void Game::run() {
    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
        
        if (!states.empty()) {
            states.top()->update(delta_time);
            states.top()->render();
        }
    }
}

bool Game::visionCheck(float enemy_x, float enemy_y) {
    float dx = player.getX() - (enemy_x + 10.f);
    float dy = player.getY() - (enemy_y + 10.f);

    int steps = std::max(std::abs(dx), std::abs(dy)) / (tile_size / 16.f);
    float xIncrement = dx / steps;
    float yIncrement = dy / steps;

    float x = enemy_x;
    float y = enemy_y;

    for (int i = 0; i <= steps; ++i) {

        int map_x = (int)(floor(x) / tile_size);
        int map_y = (int)(floor(y) / tile_size);

        if (stringContains(wall_chars, map_layout[map_y][map_x])) {
            return true;
        }

        x += xIncrement;
        y += yIncrement;
    }

    return false;
}

void Game::getEnemyTexture(Enemy& enemy) {//bool stationary, bool shooting, bool succesful_shot, int number) {
    bool stationary = enemy.getStationary();
    bool shooting = enemy.getShooting();
    bool succesful_shot = enemy.getSuccesfulShot();
    bool alive = enemy.isAlive();
    int number_death = enemy.getTextureDeath();
    int number = enemy.getTextureRunning();
    
    if (stationary && !shooting)
        current_guard_texture = textures.guard_stationary;

    else if (!stationary) {
        switch (number) {
        case 0:
            current_guard_texture = guard_textures.run0;
            break;
        case 1:
            current_guard_texture = guard_textures.run1;
            break;
        case 2:
            current_guard_texture = guard_textures.run2;
            break;
        case 3:
            current_guard_texture = guard_textures.run3;
            break;
        default:
            current_guard_texture = textures.guard_stationary;
            break;
        }
    }

    else if (shooting) {
        current_guard_texture = guard_textures.aim;
    }

    if (succesful_shot) {
        current_guard_texture = guard_textures.shoot;
    }

    if (enemy.isDying()) {
        switch (number_death) {
        case 0:
            current_guard_texture = guard_textures.death1;
            break;
        case 1:
            current_guard_texture = guard_textures.death2;
            break;
        case 2:
            current_guard_texture = guard_textures.death3;
            break;
        case 3:
            current_guard_texture = guard_textures.death4;
            break;
        case 4:
            current_guard_texture = guard_textures.death5;
        default:
            current_guard_texture = guard_textures.death5;
            break;
        }
    }

    else if (!alive)
        current_guard_texture = guard_textures.death5;
}

void Game::shoot() {
    static bool is_SLASH_pressed = false;

    if (glfwGetKey(window, GLFW_KEY_SLASH) == GLFW_PRESS && !is_SLASH_pressed) {
        for (int i = 0; i < all_enemies.size(); i++) {
            is_SLASH_pressed = true;
            //std::cout << all_enemies[i].getScreenX() << std::endl;
            if (all_enemies[i].getScreenX() > reticle_position - reticle_offset &&
                all_enemies[i].getScreenX() < reticle_position + reticle_offset) {
                all_enemies[i].subtractHealth();
                printf("Enemy shot!\n");
            }
        }
    } 

    else if (glfwGetKey(window, GLFW_KEY_SLASH) == GLFW_RELEASE)
        is_SLASH_pressed = false;

}

void Game::checkForDead() {
    for (int i = 0; i < all_enemies.size(); ) {
        if (!all_enemies[i].isAlive()) {
            all_enemies.erase(all_enemies.begin() + i);
        }
        else {
            i++;
        }
    }
}

