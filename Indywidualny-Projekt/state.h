#pragma once

#include "game.h"

// Maszyna stan�w wykonana jest w ten spos�b w celach edukacyjnych. Z pewno�ci�
// do tego zastosowania mo�a by zrobi� to pro�ciej.

class State {
public:
    virtual void handleEvents() = 0;
    virtual void update(float delta_time) = 0;
    virtual void render() = 0;
    virtual ~State() {}
};

class PlayingState : public State {
public:
    PlayingState(Game* game_) : game(game_) {}

    void handleEvents() {
        //game->handlePlaying();
    }

    void update(float delta_time) {
        game->updatePlaying();
    }

    void render() {
        game->renderPlaying();
    }

private:
    Game* game;
};