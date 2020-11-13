#ifndef SPOUT_ink_rgb_H
#define SPOUT_ink_rgb_H

namespace SPOUT {

// light adds up to white
// ink adds up to black
// or this is completely wrong ...

struct ink_rgb
{
	float r;
	float g;
	float b;

	ink_rgb &  set_white()
	{
		r = 0.0;
		g = 0.0;
		b = 0.0;
		return * this;
	}

	ink_rgb &  set_blue()
	{
		r = 0.0;
		g = 0.0;
		b = 1.0;
		return * this;
	}

	ink_rgb &  set_green()
	{
		r = 0.0;
		g = 1.0;
		b = 0.0;
		return * this;
	}

	ink_rgb &  set_black()
	{
		r = 1.0;
		g = 1.0;
		b = 1.0;
		return * this;
	}

	ink_rgb()
	{
		set_blue();
	}

}; // class
}; // namespace

#endif
