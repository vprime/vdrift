/************************************************************************/
/*                                                                      */
/* This file is part of VDrift.                                         */
/*                                                                      */
/* VDrift is free software: you can redistribute it and/or modify       */
/* it under the terms of the GNU General Public License as published by */
/* the Free Software Foundation, either version 3 of the License, or    */
/* (at your option) any later version.                                  */
/*                                                                      */
/* VDrift is distributed in the hope that it will be useful,            */
/* but WITHOUT ANY WARRANTY; without even the implied warranty of       */
/* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the        */
/* GNU General Public License for more details.                         */
/*                                                                      */
/* You should have received a copy of the GNU General Public License    */
/* along with VDrift.  If not, see <http://www.gnu.org/licenses/>.      */
/*                                                                      */
/************************************************************************/

#ifndef _TRACKMAP_H
#define _TRACKMAP_H

#include "mathvector.h"
#include "graphics/scenenode.h"
#include "graphics/texture.h"
#include "roadstrip.h"

#include <list>
#include <string>
#include <ostream>

class ContentManager;
class Texture;

class TrackMap
{
public:
	TrackMap();

	~TrackMap();

	/// w and h are the display device dimensions in pixels
	/// returns true if successful
	bool BuildMap(
		const int screen_width,
		const int screen_height,
		const std::list <RoadStrip> & roads,
		const std::string & trackname,
		const std::string & texturepath,
		ContentManager & content,
		std::ostream & error_output);

	void Unload();

	/// update the map with provided information for map visibility,
	/// as well as a list of car positions and whether or not they're the player car
	void Update(bool mapvisible, const std::list <std::pair<Vec3, bool> > & carpositions);

	SceneNode & GetNode() {return mapnode;}

	/// raterize vxy triangle into 32bit rgba color buffer, stride is in bytes
	static void RasterizeTriangle(
		const float vx[3],
		const float vy[3],
		unsigned color,
		void * color_buffer,
		int stride);

private:
	// map texture size
	const int map_width;
	const int map_height;

	// track to map scale factor
	float map_scale;

	// track aabb in world space
	Vec2 track_min;
	Vec2 track_max;

	// map aabb in screen space
	Vec2 map_min;
	Vec2 map_max;

	// size of a pixel in screen space
	Vec2 pixel_size;

	// size of the car dot in screen space
	Vec2 dot_size;

	SceneNode mapnode;
	keyed_container <Drawable>::handle mapdraw;
	VertexArray mapverts;

	std::tr1::shared_ptr<Texture> track_map;
	std::tr1::shared_ptr<Texture> cardot0;
	std::tr1::shared_ptr<Texture> cardot1;
	std::tr1::shared_ptr<Texture> cardot0_focused;
	std::tr1::shared_ptr<Texture> cardot1_focused;

	class CarDot
	{
		public:
			void Init(
				SceneNode & topnode,
				std::tr1::shared_ptr<Texture> & tex,
				const Vec2 & corner1,
				const Vec2 & corner2)
			{
				dotdraw = topnode.GetDrawlist().twodim.insert(Drawable());
				Drawable & drawref = GetDrawable(topnode);
				drawref.SetVertArray(&dotverts);
				drawref.SetCull(false, false);
				drawref.SetColor(1,1,1,0.7);
				drawref.SetDrawOrder(0.1);
				Retexture(topnode, tex);
				Reposition(corner1, corner2);
			}
			void Retexture(SceneNode & topnode, std::tr1::shared_ptr<Texture> & newtex)
			{
				assert(newtex.get());
				texture = newtex;
				GetDrawable(topnode).SetTextures(texture->GetId());
			}
			void Reposition(const Vec2 & corner1, const Vec2 & corner2)
			{
				dotverts.SetToBillboard(corner1[0], corner1[1], corner2[0], corner2[1]);
			}
			void SetVisible(SceneNode & topnode, bool visible)
			{
				GetDrawable(topnode).SetDrawEnable(visible);
			}
			void DebugPrint(SceneNode & topnode, std::ostream & out) const
			{
				const Drawable & drawref = GetDrawable(topnode);
				out << &drawref << ": enable=" << drawref.GetDrawEnable() << ", tex=" << drawref.GetTexture0() << ", verts=" << drawref.GetVertArray() << std::endl;
			}
			keyed_container <Drawable>::handle & GetDrawableHandle()
			{
				return dotdraw;
			}

		private:
			keyed_container <Drawable>::handle dotdraw;
			std::tr1::shared_ptr<Texture> texture;
			VertexArray dotverts;

			Drawable & GetDrawable(SceneNode & topnode)
			{
				return topnode.GetDrawlist().twodim.get(dotdraw);
			}

			const Drawable & GetDrawable(SceneNode & topnode) const
			{
				return topnode.GetDrawlist().twodim.get(dotdraw);
			}
	};

	std::list <CarDot> dotlist;
};

#endif
