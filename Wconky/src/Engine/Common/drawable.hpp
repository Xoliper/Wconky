#ifndef ENGINE_COMMON_DRAWABLE_HPP_
#define ENGINE_COMMON_DRAWABLE_HPP_

#include <GL/glew.h>
#include <vector>

class Drawable {
public:
	Drawable(){ params = NULL; };
	virtual ~Drawable(){};
	virtual void Render() = 0;

	std::vector<GLuint> * params;
};

#endif /* ENGINE_COMMON_DRAWABLE_HPP_ */
