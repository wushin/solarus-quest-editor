/*
 * Copyright (C) 2014-2015 Christopho, Solarus - http://www.solarus-games.org
 *
 * Solarus Quest Editor is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Solarus Quest Editor is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program. If not, see <http://www.gnu.org/licenses/>.
 */
#ifndef SOLARUSEDITOR_MAP_MODEL_H
#define SOLARUSEDITOR_MAP_MODEL_H

#include "entities/entity_traits.h"
#include "layer_traits.h"
#include "sprite_model.h"
#include <solarus/MapData.h>
#include <deque>
#include <memory>
#include <QPointer>

class Quest;
class QuestResources;
class TilesetModel;
class QPainter;

using EntityIndex = Solarus::EntityIndex;

/**
 * @brief Model that wraps a map.
 *
 * It makes the link between the editor and the map data of the
 * Solarus library.
 * Signals are sent when something changes in the wrapped map.
 */
class MapModel : public QObject {
  Q_OBJECT

public:

  static constexpr int NO_FLOOR = Solarus::MapData::NO_FLOOR;

  // Creation.
  MapModel(
      Quest& quest, const QString& map_id, QObject* parent = nullptr);

  const Quest& get_quest() const;
  Quest& get_quest();
  QString get_map_id() const;

  // Map properties.
  QSize get_size() const;
  void set_size(const QSize& size);
  bool has_world() const;
  QString get_world() const;
  void set_world(const QString& world);
  bool has_floor() const;
  int get_floor() const;
  void set_floor(int floor);
  QPoint get_location() const;
  void set_location(const QPoint& location);
  TilesetModel* get_tileset_model() const;
  QString get_tileset_id() const;
  void set_tileset_id(const QString& tileset_id);
  QString get_music_id() const;
  void set_music_id(const QString& music_id);

  // Entities.
  int get_num_entities() const;
  int get_num_entities(Layer layer) const;
  bool entity_exists(const EntityIndex& index) const;
  EntityType get_entity_type(const EntityIndex& index) const;
  QString get_entity_type_name(const EntityIndex& index) const;
  Layer get_entity_layer(const EntityIndex& index) const;
  QPoint get_entity_xy(const EntityIndex& index) const;
  void set_entity_xy(const EntityIndex& index, const QPoint& xy);
  void add_entity_xy(const EntityIndex& index, const QPoint& translation);
  QPoint get_entity_top_left(const EntityIndex& index) const;
  QPoint get_entity_origin(const EntityIndex& index) const;
  QSize get_entity_size(const EntityIndex& index) const;
  QRect get_entity_bounding_box(const EntityIndex& index) const;

  void draw_entity(const EntityIndex& index, QPainter& painter) const;

signals:

  void size_changed(const QSize& size);
  void world_changed(const QString& world);
  void floor_changed(int floor);
  void location_changed(const QPoint& location);
  void tileset_id_changed(const QString& tileset_id);
  void music_id_changed(const QString& music_id);

  void entity_xy_changed(const EntityIndex& index, const QPoint& xy);

public slots:

  void save() const;

private:

  class EntityModel;
  class Block;
  class Chest;
  class Crystal;
  class CrystalBlock;
  class CustomEntity;
  class Destination;
  class Destructible;
  class Door;
  class DynamicTile;
  class Enemy;
  class Jumper;
  class Npc;
  class Pickable;
  class Sensor;
  class Separator;
  class ShopTreasure;
  class Stairs;
  class Stream;
  class Switch;
  class Teletransporter;
  class Tile;
  class Wall;

  const Solarus::EntityData& get_entity(const EntityIndex& index) const;
  Solarus::EntityData& get_entity(const EntityIndex& index);

  const EntityModel& get_entity_model(const EntityIndex& index) const;
  EntityModel& get_entity_model(const EntityIndex& index);

  Quest& quest;                   /**< The quest the tileset belongs to. */
  const QString map_id;           /**< Id of the map. */
  Solarus::MapData map;           /**< Map data wrapped by this model. */
  TilesetModel* tileset_model;    /**< Tileset of this map. nullptr if not set. */
  std::array<std::deque<std::unique_ptr<EntityModel>>, Layer::LAYER_NB>
      entities;                   /**< All entities. */

};

#include "entities/entity_model.h"

#endif
