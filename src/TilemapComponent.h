#pragma once
#include "Component.h"

namespace gbln {
	class TilemapComponent : public Component {
	public:
		TilemapComponent(gueepo::Tilemap* tilemap);
		void Render() override;
		void Destroy() override;
	private:
		gueepo::Tilemap* m_tilemap;
	};
}