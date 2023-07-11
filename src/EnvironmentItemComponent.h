#pragma once
#include "Component.h"
#include "gueepo2d.h"

static const int SPEED = 200;

// this is awful.
static const int MAX_Y = 250;
static const int MIN_STARTING_Y = -250;
static const int MAX_STARTING_Y = -1000;

namespace gbln {
	class EnvironmentItemComponent : public Component {
	public:
		void BeginPlay() override {
			RandomizeInitialPosition();
		}

		void Update(float DeltaTime) override {

			if (owner->HasComponentOfType<Transform>()) {
				Transform* transform = owner->GetComponentOfType<Transform>();
				transform->Translate(gueepo::math::vec2(0.0f, SPEED * DeltaTime));

				if (transform->position.y > MAX_Y) {
					RandomizeInitialPosition();
				}
			}
		}

	private:
		void RandomizeInitialPosition() {
			float RandomValue = gueepo::rand::Float();
			float InitialPosition = MIN_STARTING_Y + (RandomValue * (MAX_STARTING_Y - MIN_STARTING_Y));

			owner->GetComponentOfType<Transform>()->position.y = InitialPosition;
		}
	};
}