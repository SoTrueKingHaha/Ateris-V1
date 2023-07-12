#pragma once
#include <string>
#include <sstream>

struct Color_t {
	unsigned char r, g, b, a;
	[[nodiscard]] std::string to_hex( ) const
	{
		std::stringstream ss;
		ss << "\x7";
		ss << std::hex << ( static_cast< int >( r ) << 16 | static_cast< int >( g ) << 8 | static_cast< int >( b ) );
		return ss.str( );
	}

	[[nodiscard]] std::string to_hex_alpha( ) const
	{
		std::stringstream ss;
		ss << "\x8";
		ss << std::hex << ( static_cast< int >( r ) << 16 | static_cast< int >( g ) << 8 | static_cast< int >( b ) );
		ss << std::hex << static_cast< int >( a );
		return ss.str( );
	}
};

struct Chams_t {
	bool	showObstructed = false;
	int		drawMaterial = 0;
	int		overlayType = 0;
	bool	chamsActive = false;
	Color_t fresnelBase = { 0,0,0,255 };
};