/*! \file sprite.cpp
    \brief Functions for drawing sprites.
*/
#include "surface.hpp"
#include "sprite.hpp"

using namespace blit;

namespace blit {

  /**
   * Create a spritesheet from packaged data.
   *
   * \param[in] data
   * \param[in] format
   * \param[in] image
   */
  spritesheet::spritesheet(uint8_t *data, pixel_format format, const packed_image *image) : surface(data, format, image) {  
    rows = bounds.h / 8;
    cols = bounds.w / 8;
  }

  spritesheet *spritesheet::load(const uint8_t *data, uint8_t *buffer) {
    return load((packed_image *)data, buffer);
  }

  spritesheet *spritesheet::load(const packed_image *image, uint8_t *buffer) {
    if (buffer == nullptr) {
      buffer = new uint8_t[pixel_format_stride[image->format] * image->width * image->height];
    }
    
    return new spritesheet(buffer, (pixel_format)image->format, image);
  }

  /**
   * Return the bounds of a sprite by index
   * 
   * Takes an index offset to the sprite in the spritesheet
   * and returns a `rect` describing the location and size of the sprite in pixels.
   *
   * \param[in] index Index of the sprite in the sheet
   * \return `rect` sprite x/y location (always a multiple of 8) and size (always 8x8)
   */
  rect spritesheet::sprite_bounds(const uint16_t &index) {
    return rect((index % cols) * 8, (index / cols) * 8, 8, 8);    
  }

 /**
   * Return the bounds of a sprite by index
   * 
   * Takes a `point` describing the x/y offset of the sprite in terms of tiles/units
   * and returns a `rect` describing the location of the sprite in pixels.
   *
   * \param[in] p `point` describing the x/y offset of the sprite in the spritesheet
   * \return `rect` sprite x/y location (always a multiple of 8) and size (always 8x8)
   */
  rect spritesheet::sprite_bounds(const point &p) {
    return rect(p.x * 8, p.y * 8, 8, 8);
  }

 /**
   * Return the bounds of a sprite by index
   * 
   * Takes a `rect` describing the x/y offset and size of the sprite in terms of tiles/units
   * and returns a `rect` describing the location and size of the sprite in pixels.
   *
   * \param[in] r `rect` describing the x/y offset and size of the sprite in sprite tiles
   * \return `rect` sprite x/y location (always a multiple of 8) and size (always 8x8)
   */
  rect spritesheet::sprite_bounds(const rect &r) {
    return r * 8;
  }


  // unscaled sprites
  
  /**
   * Draws a sprite to the surface
   * 
   * \param[in] sprite Index of the sprite in the sheet
   * \param[in] position `point` at which to place the sprite in the target surface
   * \param[in] transform to apply
   */
  void surface::sprite(const uint16_t &sprite, const point &position, const uint8_t &transform) {
    blit_sprite(
      sprites->sprite_bounds(sprite), 
      position, 
      transform);
  }

  /**
   * Draws a sprite to the surface
   * 
   * \param[in] sprite `point` describing the x/y offset of the sprite in the spritesheet in tiles/units
   * \param[in] position `point` at which to place the sprite in the target surface
   * \param[in] transform to apply
   */
  void surface::sprite(const point &sprite, const point &position, const uint8_t &transform) {
    blit_sprite(
      sprites->sprite_bounds(sprite),
      position,
      transform);
  }

  /**
   * Draws a sprite to the surface
   * 
   * \param[in] sprite `rect` describing the x/y offset and size of the sprite in the spritesheet in tiles/units
   * \param[in] position `point` at which to place the sprite in the target surface
   * \param[in] transform to apply
   */
  void surface::sprite(const rect &sprite, const point &position, const uint8_t &transform) {        
    blit_sprite(
      sprites->sprite_bounds(sprite),
      position,
      transform);
  }

  // unscaled sprites with origin

  /**
   * Draws a sprite to the surface
   * 
   * \param[in] sprite Index of the sprite in the sheet
   * \param[in] position `point` at which to place the sprite in the target surface
   * \param[in] origin `point` around which to transform the sprite
   * \param[in] transform to apply
   */
  void surface::sprite(const uint16_t &sprite, const point &position, const point &origin, const uint8_t &transform) {
    surface::sprite(sprite, position - origin, transform);
  }

  /**
   * Draws a sprite to the surface
   * 
   * \param[in] sprite `point` describing the x/y offset of the sprite in the spritesheet in tiles/units
   * \param[in] position `point` at which to place the sprite in the target surface
   * \param[in] origin `point` around which to transform the sprite
   * \param[in] transform to apply
   */
  void surface::sprite(const point &sprite, const point &position, const point &origin, const uint8_t &transform) {
    surface::sprite(sprite, position - origin, transform);
  }

  /**
   * Draws a sprite to the surface
   * 
   * \param[in] sprite `rect` describing the x/y offset and size of the sprite in the spritesheet in tiles/units
   * \param[in] position `point` at which to place the sprite in the target surface
   * \param[in] origin `point` around which to transform the sprite
   * \param[in] transform to apply
   */
  void surface::sprite(const rect &sprite, const point &position, const point &origin, const uint8_t &transform) {
    surface::sprite(sprite, position - origin, transform);
  }

  // scaled sprites with origin

  /**
   * Draws a sprite to the surface
   * 
   * \param[in] sprite Index of the sprite in the sheet
   * \param[in] position `point` at which to place the sprite in the target surface
   * \param[in] origin `point` around which to transform & scale the sprite
   * \param[in] scale `vec2` x/y scale factor
   * \param[in] transform to apply
   */
  void surface::sprite(const uint16_t &sprite, const point &position, const point &origin, const vec2 &scale, const uint8_t &transform) {
    rect dest_rect(
      round(position.x - float(origin.x * scale.x)),
      round(position.y - float(origin.y * scale.y)),
      round(8.0f * scale.x),
      round(8.0f * scale.y)
    );

    stretch_blit_sprite(
      sprites->sprite_bounds(sprite),
      dest_rect,
      transform);
  }

  /**
   * Draws a sprite to the surface
   * 
   * \param[in] sprite `point` describing the x/y offset of the sprite in the spritesheet in tiles/units
   * \param[in] position `point` at which to place the sprite in the target surface
   * \param[in] origin `point` around which to transform & scale the sprite
   * \param[in] scale `vec2` x/y scale factor
   * \param[in] transform to apply
   */
  void surface::sprite(const point &sprite, const point &position, const point &origin, const vec2 &scale, const uint8_t &transform) {
    rect dest_rect(
      round(position.x - float(origin.x * scale.x)),
      round(position.y - float(origin.y * scale.y)),
      round(8.0f * scale.x),
      round(8.0f * scale.y)
    );

    stretch_blit_sprite(
      sprites->sprite_bounds(sprite),
      dest_rect,
      transform);
  }

  /**
   * Draws a sprite to the surface
   * 
   * \param[in] sprite `rect` describing the x/y offset and size of the sprite in the spritesheet in tiles/units
   * \param[in] position `point` at which to place the sprite in the target surface
   * \param[in] origin `point` around which to transform  & scale the sprite
   * \param[in] scale `vec2` x/y scale factor
   * \param[in] transform to apply
   */
  void surface::sprite(const rect &sprite, const point &position, const point &origin, const vec2 &scale, const uint8_t &transform) {
    rect dest_rect(
      round(position.x - float(origin.x * scale.x)),
      round(position.y - float(origin.y * scale.y)),
      round(sprite.w * 8.0f * scale.x),
      round(sprite.h * 8.0f * scale.y)
    );

    stretch_blit_sprite(
      sprites->sprite_bounds(sprite),
      dest_rect,
      transform);
  }

  /**
   * Draws a sprite to the surface
   * 
   * \param[in] sprite Index of the sprite in the sheet
   * \param[in] position `point` at which to place the sprite in the target surface
   * \param[in] origin `point` around which to transform & scale the sprite
   * \param[in] scale `float` x/y scale factor
   * \param[in] transform to apply
   */
  void surface::sprite(const uint16_t &sprite, const point &position, const point &origin, const float &scale, const uint8_t &transform) {
    surface::sprite(sprite, position, origin, vec2(scale, scale), transform);
  }

  /**
   * Draws a sprite to the surface
   * 
   * \param[in] sprite `point` describing the x/y offset of the sprite in the spritesheet in tiles/units
   * \param[in] position `point` at which to place the sprite in the target surface
   * \param[in] origin `point` around which to transform & scale the sprite
   * \param[in] scale `float` x/y scale factor
   * \param[in] transform to apply
   */
  void surface::sprite(const point &sprite, const point &position, const point &origin, const float &scale, const uint8_t &transform) {
    surface::sprite(sprite, position, origin, vec2(scale, scale), transform);
  }

  /**
   * Draws a sprite to the surface
   * 
   * \param[in] sprite `rect` describing the x/y offset and size of the sprite in the spritesheet in tiles/units
   * \param[in] position `point` at which to place the sprite in the target surface
   * \param[in] origin `point` around which to transform  & scale the sprite
   * \param[in] scale `float` x/y scale factor
   * \param[in] transform to apply
   */
  void surface::sprite(const rect &sprite, const point &position, const point &origin, const float &scale, const uint8_t &transform) {
    surface::sprite(sprite, position, origin, vec2(scale, scale), transform);
  }


  // unscaled sprites with origin and scale - optional transform (mirror/rotate)
  //void surface::sprite(const rect &source, const point &position, const point &origin, const float &scale, const uint8_t &transform = 0);
  //void surface::sprite(const point &source, const point &position, const point &origin, const float &scale, const uint8_t &transform = 0);

  // unscaled sprites with origin and scale (x/y) - optional transform (mirror/rotate)
  //void surface::sprite(const rect &source, const point &position, const point &origin, const vec2 &scale, const uint8_t &transform = 0);
  //void surface::sprite(const point &source, const point &position, const point &origin, const vec2 &scale, const uint8_t &transform = 0);

  /*
  void surface::sprite(const spritesheet &ss, const rect &sprite, const point &position, const sprite_p &properties) {

  }

  void surface::sprite(const spritesheet &ss, const point &sprite, const point &position, const sprite_p &properties) {
    if (properties.scale.x != 1.0f || properties.scale.y != 1.0f) {
      rect sr = ss.sprite_bounds(index);
      sr.w = span.w * ss.sprite_size.w;
      sr.h = span.h * ss.sprite_size.h;

      rect dr = rect(
        pos - (origin * scale),
        size(sr.w * scale, sr.h * scale)
      );

      this->stretch_blit(&ss, sr, dr);
    }
    else {
      rect sr = ss.sprite_bounds(index);
      sr.w = span.w * ss.sprite_size.w;
      sr.h = span.h * ss.sprite_size.h;

      this->blit(&ss, sr, pos - origin);

    }
    
  }*/
  /*
  void surface::sprite(spritesheet &ss, uint16_t index, point pos, size span, point origin, float scale) {
    if (scale != 1.0f) {
      rect sr = ss.sprite_bounds(index);
      sr.w = span.w * ss.sprite_size.w;
      sr.h = span.h * ss.sprite_size.h;            
    
      rect dr = rect(
        pos - (origin * scale),
        size(sr.w * scale, sr.h * scale)
      );

      this->stretch_blit(&ss, sr, dr);
    } else {
      rect sr = ss.sprite_bounds(index);
      sr.w = span.w * ss.sprite_size.w;
      sr.h = span.h * ss.sprite_size.h;

      this->blit(&ss, sr, pos - origin);

    }
  }*/

} 