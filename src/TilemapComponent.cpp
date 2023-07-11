#include "TilemapComponent.h"

#include "Transform.h"
#include "Entity.h"

namespace gbln {
	
	TilemapComponent::TilemapComponent(gueepo::Tilemap* tilemap) {
		m_tilemap = tilemap;
	}

	void TilemapComponent::Render() {
		float x_position = 0;
		float y_position = 0;
		float scale_x = 1;
		float scale_y = 1;

		if (owner->HasComponentOfType<Transform>()) {
			Transform* transform = owner->GetComponentOfType<Transform>();
			x_position = transform->position.x;
			y_position = transform->position.y;
			scale_x = transform->scale.x;
			scale_y = transform->scale.y;
		}

		for (int i = 0; i < m_tilemap->GetNumberOfLayers(); i++) {
			gueepo::TilemapLayer* layer = m_tilemap->GetLayer(i);

			for (int j = 0; j < layer->data.size(); j++) {
				gueepo::Renderer::Draw(
					layer->data[j]->texture,
					((layer->data[j]->x * m_tilemap->GetTileWidth()) * scale_x) + x_position,
					(layer->data[j]->y * m_tilemap->GetTileHeight()) * scale_y + y_position,
					(m_tilemap->GetTileWidth()) * scale_x,
					(m_tilemap->GetTileHeight()) * scale_y
				);
			}
		}
	}

	void TilemapComponent::Destroy() {
		if (m_tilemap != nullptr) {
			delete m_tilemap;
		}
	}
}