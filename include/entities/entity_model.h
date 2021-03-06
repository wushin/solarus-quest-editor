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
#ifndef SOLARUSEDITOR_ENTITY_MODEL_H
#define SOLARUSEDITOR_ENTITY_MODEL_H

#include "entity_traits.h"
#include "layer_traits.h"
#include "map_model.h"
#include "sprite_model.h"
#include <solarus/MapData.h>
#include <QPointer>
#include <memory>

class MapModel;
class Quest;
class QuestResources;
class TilesetModel;

using EntityIndex = Solarus::EntityIndex;

/**
 * @brief Internal model of a map entity.
 *
 * This is an implementation detail of MapModel, it cannot be accessed from
 * outside.
 *
 * This class wraps an entity from the Solarus library and
 * adds all useful information about how to represent and modify it in the
 * editor.
 *
 * Each type of entity is a subclass of EntityModel.
 */
class MapModel::EntityModel {

public:

  static std::unique_ptr<EntityModel> create(
      MapModel& map, const EntityIndex& index);
  virtual ~EntityModel();

  const MapModel& get_map() const;
  MapModel& get_map();
  QString get_tileset_id() const;
  const TilesetModel* get_tileset() const;
  const Quest& get_quest() const;
  const QuestResources& get_resources() const;

  EntityType get_type() const;
  QString get_type_name() const;

  Layer get_layer() const;
  void set_layer(Layer layer);
  QPoint get_xy() const;
  void set_xy(const QPoint& xy);
  QPoint get_top_left() const;
  void set_top_left(const QPoint& top_left);
  QPoint get_origin() const;
  void set_origin(const QPoint& origin);
  int get_width() const;
  void set_width(int width);
  int get_height() const;
  void set_height(int height);
  QSize get_size() const;
  void set_size(const QSize& size);
  QRect get_bounding_box() const;
  bool has_direction_property() const;
  int get_direction() const;
  bool has_field(const QString& key) const;
  QVariant get_field(const QString& key) const;

  virtual void draw(QPainter& painter) const;

protected:

  /**
   * @brief Describes how to draw an entity as a sprite.
   */
  struct DrawSpriteInfo {

    bool enabled = true;  // false means not drawn as a sprite.
    QString sprite_id;    // Only used if there is no "sprite" field.
    QString animation;    // Animation for sprite_id (empty means default).
    bool tiled = false;   // Tiled or only once at origin point.
  };

  /**
   * @brief Describes how to draw an entity as a shape.
   *
   * The shape is filled with a background color or a pixmap,
   * or both if the pixmap is not tiled but centered,
   * and may have a border with two black lines and a color between them.
   * For now the shape is always rectangular.
   */
  struct DrawShapeInfo {

    bool enabled = false;  // false means not drawn as a shape.
    QColor background_color;
    QColor between_border_color;
    QPixmap pixmap;
    bool tiled_pixmap = false;  // Tiled or centered.
  };

  /**
   * @brief A rectangle region of an image file.
   */
  struct SubImage {
    QString file_name;
    QRect src_rect;  // An invalid rect means the whole image.
    mutable QPixmap pixmap;
  };

  /**
   * @brief Describes how to draw an entity as a fixed image.
   *
   * If nothing is specified, the image will be a generic icon for the entity
   * type.
   */
  struct DrawImageInfo {
    SubImage image_no_direction;
    QList<SubImage> images_by_direction;
    double scale = 1.0;  // If 2.0, the image will have a resolution twice better.
  };

  EntityModel(MapModel& map, const EntityIndex& index);

  const Solarus::EntityData& get_entity() const;
  Solarus::EntityData& get_entity();

  const DrawSpriteInfo& get_draw_sprite_info() const;
  void set_draw_sprite_info(const DrawSpriteInfo& draw_sprite_info);
  const DrawShapeInfo& get_draw_shape_info() const;
  void set_draw_shape_info(const DrawShapeInfo& draw_shape_info);
  const DrawImageInfo& get_draw_image_info() const;
  void set_draw_image_info(const DrawImageInfo& draw_shape_info);

  bool draw_as_sprite(QPainter& painter) const;
  bool draw_as_sprite(QPainter& painter, const QString& sprite_id, const QString& animation) const;
  bool draw_as_shape(QPainter& painter) const;
  bool draw_as_image(QPainter& painter) const;
  bool draw_as_image(QPainter& painter, const SubImage& image) const;
  bool draw_as_icon(QPainter& painter) const;

private:

  QPointer<MapModel> map;         /**< The map this entity belongs to
                                   * (could be a reference but we want operator=). */
  EntityIndex index;              /**< Index of this entity in the map. */
  QPoint origin;                  /**< Origin point of the entity relative to its top-left corner. */
  QSize size;                     /**< Size of the entity for the editor. */

  // Displaying.
  DrawSpriteInfo
      draw_sprite_info;           /**< How to draw the entity
                                   * when it is drawn as a sprite. */
  mutable std::unique_ptr<SpriteModel>
      sprite_model;               /**< Sprite to show when the entity is drawn
                                   * as a sprite. */
  mutable QPixmap sprite_image;   /**< Fixed image from the sprite. */
  DrawShapeInfo draw_shape_info;  /**< Shape to use when the entity is drawn as
                                   * a shape. */
  DrawImageInfo draw_image_info;  /**< Subimage to use when the entity is
                                   * drawn as a fixed image from a file. */
  mutable QPixmap icon;           /**< Icon to use when the entity is drawn as
                                   * an icon. */
};

#endif
