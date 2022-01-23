#ifndef CONTROLLABLE_H
#define CONTROLLABLE_H

class Controllable {
public:
	virtual ~Controllable() = default;
	virtual void show_controls() = 0;
	virtual void hide_controls() = 0;
	virtual void scale(float factor_x, float factor_y) = 0;
};

#endif // ORIGINCONTROL_H
