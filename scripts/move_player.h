#pragma once

#include <SDL.h>
#include <vector>

#include "../../labrat/entity/entity.h"
#include "../../labrat/scene/SceneState.h"
#include "../../labrat/scene/camera.h"
#include "../../labrat/scene/scene.h"
#include "../../labrat/reactive/reactive.h"
#include "../../labrat/reactive/source.h"

struct move_player {

	bool key_hold = false;

	move_player() {}

	void key_down(PhysObj* body, SDL_Event e, glm::vec2 grav_norm) {
		switch (e.key.keysym.sym)
		{
		case SDLK_SPACE:
			if (std::find(std::begin(body->normals_acting), std::end(body->normals_acting), grav_norm) != body->normals_acting.end())
				body->velocity += glm::vec2(0.0f, 5.0f);
			break;

		case  SDLK_a:
			if(!key_hold)
				body->velocity += glm::vec2(-2.0f, 0.0f);
			break;

		case SDLK_d:
			if (!key_hold)
				body->velocity += glm::vec2(2.0f, 0.0f);
			break;

		default: break;
		}

		key_hold = true;
	}

	void key_up(PhysObj* body, SDL_Event e) {
		switch (e.key.keysym.sym)
		{
		case  SDLK_a:
		case SDLK_d:
			body->velocity = glm::vec2(0.0f, 0.0f);
			break;

		default: break;
		}

		key_hold = false;
	}

	auto operator()(Entity* object, Reactive<std::vector<SDL_Event>>& events, glm::vec2 grav_norm) {

		return from(object->body, events)
			// Ignore this error, its a false positive
			.use([this, grav_norm](PhysObj* body, std::vector<SDL_Event> events) {

				// Negative because we are moving the world not the camera
				const float camera_speed = 0.05f;

				for (auto& e : events) {
					if (e.type == SDL_KEYDOWN) {
						key_down(body, e, grav_norm);
					}
					else {
						key_up(body, e); 
					}
				}

				return body; })
			.determine(object->body);
	}
};