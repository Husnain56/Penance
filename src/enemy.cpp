#include <cmath>
#include <constants.hpp>
#include <enemy.hpp>
#include <player.hpp>

std::vector<Enemy *> Enemy::s_enemies;

void Enemy::update(const Map &map)
{
	using namespace GameConstants;

	if (current_state == STATE_DEAD || current_state == STATE_HURT)
	{
		process_state();
		return;
	}
	
	// enemy ai meachanics parameters
	const float AGGRO_RANGE = aggro_range;
	const float ATTACK_RANGE = attack_range;
	const int ENEMY_DMG = attack_damage;
	const float CHASE_SPEED = chase_speed;
	const float PATROL_SPEED = patrol_speed;

	run_anim.frame_rec.width = run_anim.frame_width;
	attack_anim.frame_rec.width = attack_anim.frame_width;
	jump_anim.frame_rec.width = jump_anim.frame_width;
	idle_anim.frame_rec.width = idle_anim.frame_width;

	float tileSize = map.get_tile_size();

	// Hitbox consistent with player
	float hitW = run_anim.frame_width * scale * 0.5f;
	float hitH = run_anim.frame_rec.height * scale * 0.9f;
	float offX = (run_anim.frame_width * scale - hitW) / 2.0f;
	float offY = (run_anim.frame_rec.height * scale - hitH);

	Player *p = target_player;
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

		// Face player when engaging in CHASE/ATTACK to ensure attack faces the player
		if (dx > 0.0f)
			is_facing_right = true;
		else if (dx < 0.0f)
			is_facing_right = false;

		// If player too far -> roam
		if (dist > AGGRO_RANGE)
		{
			ai_state = AIState::ROAM;
		}
		else if (dist > ATTACK_RANGE)
		{
			ai_state = AIState::CHASE;
		}
		else
		{
			ai_state = AIState::ATTACK;
		}
	}
	else
	{
		ai_state = AIState::ROAM;
	}

	// High-level behavior dispatch (children override on_* hooks for custom behavior)
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

	// --- Predictive horizontal movement with improved un-stuck logic ---
	float prevX = position.x;
	float desiredX = prevX + moveX;

	// Current vertical tile indices (feet)
	int curTopTile = (int)((position.y + offY) / tileSize);
	int curBottomTile = (int)((position.y + offY + hitH - 1) / tileSize);

	// Tiles at desiredX
	int newLeftTile = (int)((desiredX + offX) / tileSize);
	int newRightTile = (int)((desiredX + offX + hitW) / tileSize);

	// Horizontal blocking detection only using foot row (avoid ceilings blocking)
	bool blocked = false;
	if (moveX > 0.0f)
	{
		if (map.get_tile_id(newRightTile, curBottomTile) > 0)
			blocked = true;
	}
	else if (moveX < 0.0f)
	{
		if (map.get_tile_id(newLeftTile, curBottomTile) > 0)
			blocked = true;
	}

	// Ledge detection: if no ground in front after moving, mark willFallOff
	bool willFallOff = false;
	if (!blocked && moveX != 0.0f)
	{
		// front foot X coordinate after desired move
		float frontX = desiredX + offX + (moveX > 0.0f ? (hitW + 1.0f) : -1.0f);
		int frontTileX = (int)(frontX / tileSize);
		int tileBelowFront = curBottomTile + 1;

		// If no tile under front foot and not on world floor, will fall off
		if (map.get_tile_id(frontTileX, tileBelowFront) == 0 && !(position.y >= GROUND_Y - 1.0f))
			willFallOff = true;
	}

	// If blocked or would fall off, reverse facing and attempt a small nudge away to prevent
	// sticking.
	if (blocked || willFallOff)
	{
		// When attacking, prefer to hold ground rather than roam away
		if (!is_attacking)
		{
			is_facing_right = !is_facing_right;

			// apply a small immediate nudge in new facing direction to leave the blocking tile
			float nudge = tileSize * 0.15f;
			desiredX = prevX + (is_facing_right ? nudge : -nudge);

			// If still blocked (rare), cancel horizontal move
			int testLeft = (int)((desiredX + offX) / tileSize);
			int testRight = (int)((desiredX + offX + hitW) / tileSize);
			if ((moveX > 0.0f && map.get_tile_id(testRight, curBottomTile) > 0)
				|| (moveX < 0.0f && map.get_tile_id(testLeft, curBottomTile) > 0))
			{
				desiredX = prevX; // cancel
			}
		}
		else
		{
			// attacking: don't change position
			desiredX = prevX;
		}
	}

	// Commit horizontal position
	position.x = desiredX;

	// Recalculate tile indices after commit
	int leftTile = (int)((position.x + offX) / tileSize);
	int rightTile = (int)((position.x + offX + hitW) / tileSize);
	int topTile = (int)((position.y + offY) / tileSize);
	int bottomTile = (int)((position.y + offY + hitH - 1) / tileSize);

	// --- Inter-enemy collision: avoid stacking by nudging and flipping ---
	for (Enemy *other : s_enemies)
	{
		if (other == this || !other->is_alive())
			continue;

		Vector2 op = other->get_position();
		float oLeft = op.x + offX;
		float oRight = op.x + offX + hitW;
		float oTop = op.y + offY;
		float oBottom = op.y + offY + hitH;

		float myLeft = position.x + offX;
		float myRight = position.x + offX + hitW;
		float myTop = position.y + offY;
		float myBottom = position.y + offY + hitH;

		bool overlap = !(myRight < oLeft || myLeft > oRight || myBottom < oTop || myTop > oBottom);
		if (overlap)
		{
			// flip and nudge to avoid constant overlap
			is_facing_right = !is_facing_right;
			position.x += (is_facing_right ? (tileSize * 0.12f) : -(tileSize * 0.12f));
			break;
		}
	}

	// gravity + map collisions
	velocity_y += GRAVITY;
	position.y += velocity_y;

	// recalc tiles post-vertical move
	leftTile = (int)((position.x + offX) / tileSize);
	rightTile = (int)((position.x + offX + hitW) / tileSize);
	topTile = (int)((position.y + offY) / tileSize);
	bottomTile = (int)((position.y + offY + hitH) / tileSize);

	if (velocity_y > 0.0f) // falling
	{
		bool hitMapFloor = (map.get_tile_id(leftTile, bottomTile) > 0
							|| map.get_tile_id(rightTile, bottomTile) > 0);
		bool hitWorldFloor = (position.y >= GROUND_Y);

		if (hitMapFloor || hitWorldFloor)
		{
			if (hitMapFloor)
				position.y = (bottomTile * tileSize) - offY - hitH;
			else
				position.y = GROUND_Y;

			velocity_y = 0.0f;
			is_jumping = false;
		}
		else
		{
			is_jumping = true;
		}
	}
	else if (velocity_y < 0.0f) // moving up (head hit)
	{
		if (map.get_tile_id(leftTile, topTile) > 0 || map.get_tile_id(rightTile, topTile) > 0)
		{
			position.y = ((topTile + 1) * tileSize) - offY;
			velocity_y = 0.0f;
		}
	}

	// --- Run animation ---
	run_anim.frame_counter++;
	int anim_speed = 60 / FRAME_RATE;
	if (run_anim.frame_counter >= anim_speed)
	{
		run_anim.frame_counter = 0;
		run_anim.curr_frame = (run_anim.curr_frame + 1) % run_anim.total_frame;
		run_anim.frame_rec.x = (float)run_anim.curr_frame * run_anim.frame_width;
		run_anim.frame_rec.width = run_anim.frame_width;
	}

	if (wantToAttack)
	{
		if (!is_attacking)
		{
			is_attacking = true;
			attack_anim.curr_frame = 0;
			attack_anim.frame_counter = 0;
			attack_hit_registered = false;
		}
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
				attack_anim.frame_rec.width = attack_anim.frame_width;

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