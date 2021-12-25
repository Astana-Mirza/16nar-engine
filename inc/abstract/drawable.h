#ifndef _16NAR_DRAWABLE_H
#define _16NAR_DRAWABLE_H

#include <set>

class Drawable : public sf::Drawable {
public:
	int layer = 0;
	bool is_visible() const;
	void set_visible(bool set);
	virtual ~Drawable() =default;

private:
	bool visible = true;
};

#endif // #ifndef _16NAR_DRAWABLE_H
