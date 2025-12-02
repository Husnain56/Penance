#pragma once
#include <character.hpp>
#include <vector>

class Enemy; // forward

class Player : public Character
{
private:
    float velocity_y;
    float velocity_x;

    float accel;
    float decel;
    float max_speed;

    int jump_count;

    std::vector<Enemy*> attack_hit_enemies;

    // Dash state
    bool is_dashing;
    int dash_frames_remaining;
    int dash_frames_total;
    int dash_cooldown_frames;
    int dash_cooldown_timer;
    float dash_speed;

public:
    Player(Vector2 pos);

    void update(const Map& map) override;
    void init() override;
    void reset();

    void set_position(Vector2 pos) { position = pos; }
};