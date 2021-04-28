

#include "Geometric.h"

void Geometric::RenderRectange(const GeometricFormat& geo_size, const ColorData& color, SDL_Renderer* screen)
{
    SDL_Rect fillRect = {geo_size.left_, geo_size.top_, geo_size.width_, geo_size.height_};
    SDL_RenderFillRect(screen, &fillRect );
}

void Geometric::RenderOutline(const GeometricFormat& geo_size, const ColorData& color, SDL_Renderer* screen)
{
    SDL_Rect outlineRect = {geo_size.left_, geo_size.top_, geo_size.width_, geo_size.height_};
    SDL_RenderDrawRect(screen, &outlineRect );
}
