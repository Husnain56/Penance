#include <cmath>
#include <constants.hpp>
#include <enemy.hpp>
#include <player.hpp>

std::vector<Enemy *> Enemy::s_enemies;

void Enemy::update(const Map& map)
{
	using namespace GameConstants;

	if (current_state == STATE_DEAD || current_state == STATE_HURT)
	{
		process_state();
		return;
	}

	const float AGGRO_RANGE = aggro_range;
	const float ATTACK_RANGE = attack_range;
	const int ENEMY_DMG = attack_damage;

	run_anim.frame_rec.width = run_anim.frame_width;
	attack_anim.frame_rec.width = attack_anim.frame_width;

	float tileSize = map.get_tile_size();
	int mapWidth = map.get_width(); // Get map width for boundary checking

	// Hitbox consistent with player
	float hitW = run_anim.frame_width * scale * 0.5f;
	float hitH = run_anim.frame_rec.height * scale * 0.9f;
	float offX = (run_anim.frame_width * scale - hitW) / 2.0f;
	float offY = (run_anim.frame_rec.height * scale - hitH);

	Player* p = target_player;
	bool havePlayer = (p != nullptr && p->is_alive());

	// Default next move
	float moveX = 0.0f;
	bool wantToAttack = false;
	current_state = STATE_RUN;

	if (havePlayer)
	{
		Vector2 ppos = p->get_position();
		float dx = ppos.x - position.x;
		float dy = ppos.y - position.y;
		float dist = std::sqrt(dx * dx + dy * dy);

		if (dist > AGGRO_RANGE)       ai_state = AIState::ROAM;
		else if (dist > ATTACK_RANGE) ai_state = AIState::CHASE;
		else                          ai_state = AIState::ATTACK;
	}
	else
	{
		ai_state = AIState::ROAM;
	}

	// High-level behavior dispatch
	switch (ai_state)
	{
	case AIState::ROAM:
		on_roam(map, moveX);
		break;
	case AIState::CHASE:
		on_chase(p, moveX);
		break;
	case AIState::ATTACK:
		on_attack(p);
		wantToAttack = true;
		moveX = 0.0f;
		current_state = STATE_ATTACK;
		break;
	}

	// --- Predictive horizontal movement ---
	float prevX = position.x;
	float desiredX = prevX + moveX;

	// Calculate Grid Positions
	int curBottomTile = (int)((position.y + offY + hitH - 1) / tileSize);
	int newLeftTile = (int)((desiredX + offX) / tileSize);
	int newRightTile = (int)((desiredX + offX + hitW) / tileSize);

	// --- NEW: Blockage Logic includes MAP BOUNDARIES ---
	bool blocked = false;

	if (moveX > 0.0f)
	{
		// Block if: 1. Hitting a wall tile OR 2. Trying to walk past the right edge of the map
		if (newRightTile >= mapWidth || map.get_tile_id(newRightTile, curBottomTile) > 0)
			blocked = true;
	}
	else if (moveX < 0.0f)
	{
		// Block if: 1. Hitting a wall tile OR 2. Trying to walk past the left edge (index < 0)
		if (newLeftTile < 0 || map.get_tile_id(newLeftTile, curBottomTile) > 0)
			blocked = true;
	}

	// Ledge detection
	bool willFallOff = false;
	if (!blocked && moveX != 0.0f)
	{
		float frontX = desiredX + offX + (moveX > 0.0f ? (hitW + 1.0f) : -1.0f);
		int frontTileX = (int)(frontX / tileSize);
		int tileBelowFront = curBottomTile + 1;

		// Check ledge, but ensure we don't check outside map bounds
		if (frontTileX >= 0 && frontTileX < mapWidth)
		{
			if (map.get_tile_id(frontTileX, tileBelowFront) == 0 && !(position.y >= GROUND_Y - 1.0f))
				willFallOff = true;
		}
		else
		{
			// If the ledge check is looking outside the map, consider it a cliff
			willFallOff = true;
		}
	}

	// If blocked or would fall off, reverse facing and attempt a small nudge
	if (blocked || willFallOff)
	{
		desiredX = prevX;
		moveX = 0.0f;

		// When attacking, prefer to hold ground. When Roaming, TURN AROUND.
		if (ai_state == AIState::ROAM)
		{
			// 1. Flip Direction
			is_facing_right = !is_facing_right;

			// 2. Nudge slightly away from the wall/boundary so we don't get stuck
			float nudge = tileSize * 0.15f;
			desiredX = prevX + (is_facing_right ? nudge : -nudge);

			// Note: Your on_roam function needs to read 'is_facing_right' 
			// in the next frame to actually change the velocity direction!
		}
	}

	// Commit horizontal position
	position.x = desiredX;

	// Recalculate tile indices after commit logic
	float myLeft = position.x + offX;
	float myRight = position.x + offX + hitW;

	// --- Inter-enemy collision ---
	for (Enemy* other : s_enemies)
	{
		if (other == this || !other->is_alive()) continue;

		Vector2 op = other->get_position();
		float oLeft = op.x + offX;
		float oRight = op.x + offX + hitW;

		if (std::abs(position.y - op.y) < 10.0f)
		{
			if (myRight > oLeft && myLeft < oRight)
			{
				float overlap = std::min(myRight - oLeft, oRight - myLeft);
				float pushDist = overlap / 2.0f + 1.0f;

				if (position.x < op.x) position.x -= pushDist;
				else position.x += pushDist;
			}
		}
	}

	// --- Visual Facing Logic ---
	if (current_state == STATE_ATTACK && havePlayer)
	{
		float dx = p->get_position().x - position.x;
		is_facing_right = (dx > 0);
	}
	else if (std::abs(moveX) > 0.1f)
	{
		is_facing_right = (moveX > 0);
	}

	// gravity + map collisions
	velocity_y += GRAVITY;
	position.y += velocity_y;

	int leftTile = (int)((position.x + offX) / tileSize);
	int rightTile = (int)((position.x + offX + hitW) / tileSize);
	int bottomTile = (int)((position.y + offY + hitH) / tileSize);
	int topTile = (int)((position.y + offY) / tileSize);

	if (velocity_y > 0.0f) { // Falling
		bool hitMapFloor = false;
		// Ensure we don't check floor tiles out of bounds
		if (leftTile >= 0 && rightTile < mapWidth) {
			hitMapFloor = (map.get_tile_id(leftTile, bottomTile) > 0 || map.get_tile_id(rightTile, bottomTile) > 0);
		}

		if (hitMapFloor || position.y >= GROUND_Y) {
			if (hitMapFloor) position.y = (bottomTile * tileSize) - offY - hitH;
			else position.y = GROUND_Y;
			velocity_y = 0.0f;
		}
	}
	else if (velocity_y < 0.0f) { // Head bump
		// Safety check for bounds
		if (leftTile >= 0 && rightTile < mapWidth) {
			if (map.get_tile_id(leftTile, topTile) > 0 || map.get_tile_id(rightTile, topTile) > 0) {
				position.y = ((topTile + 1) * tileSize) - offY;
				velocity_y = 0.0f;
			}
		}
	}

	// --- Animation ---
	run_anim.frame_counter++;
	if (run_anim.frame_counter >= (60 / FRAME_RATE))
	{
		run_anim.frame_counter = 0;
		run_anim.curr_frame = (run_anim.curr_frame + 1) % run_anim.total_frame;
		run_anim.frame_rec.x = (float)run_anim.curr_frame * run_anim.frame_width;
	}

	if (wantToAttack && !is_attacking)
	{
		is_attacking = true;
		attack_anim.curr_frame = 0;
		attack_anim.frame_counter = 0;
		attack_hit_registered = false;
	}

	if (is_attacking)
	{
		attack_anim.frame_counter++;
		if (attack_anim.frame_counter >= (60 / ENEMY_ATTACK_FRAME_RATE))
		{
			attack_anim.frame_counter = 0;
			attack_anim.curr_frame++;
			if (attack_anim.curr_frame >= attack_anim.total_frame)
			{
				is_attacking = false;
				attack_anim.curr_frame = 0;
				attack_hit_registered = false;
			}
			else
			{
				attack_anim.frame_rec.x = (float)attack_anim.curr_frame * attack_anim.frame_width;

				// Hit Logic
				int hit_frame = attack_anim.total_frame > 0 ? (attack_anim.total_frame / 2) : 0;
				if (!attack_hit_registered && attack_anim.curr_frame == hit_frame && p != nullptr)
				{
					Vector2 ppos = p->get_position();
					float dx = ppos.x - position.x;
					float dy = ppos.y - position.y;
					float dist = std::sqrt(dx * dx + dy * dy);

					if (dist <= ATTACK_RANGE)
					{
						p->take_damage(ENEMY_DMG);
					}
					attack_hit_registered = true;
				}
			}
		}
	}

	process_state();
}

void Enemy::on_hurt(float strength)
{
	Character::on_hurt(strength);
}

void LongRangeEnemy::on_hurt(float /*strength*/)
{
	Character::on_hurt(25.0f);
}

void ShortRangeEnemy::on_hurt(float /*strength*/)
{
	Character::on_hurt(25.0f);
}