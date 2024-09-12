#pragma once

#include "Resources/ResourceManager.hpp"
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Texture.hpp>

namespace BPlotter
{

class Player;

/**
 * \brief Texture IDs under which individual textures can be saved and read.
 */
enum class TextureManagerId
{
    // Empty
};

/**
 * \brief Object storing textures of the application
 */
using TextureManager = ResourceManager<sf::Texture, TextureManagerId>;

// ====== Fonts ======= //

/**
 * \brief Fonts IDs under which individual font can be saved and read.
 */
enum class FontId
{
    ArialNarrow,
};

/**
 * \brief Object storing fonts of the application
 */
using FontManager = ResourceManager<sf::Font, FontId>;

/**
 * @brief Any application assets from textures or fonts
 */
struct ApplicationResources
{
    TextureManager textureManager;
    FontManager fontManager;
};

}// namespace BPlotter