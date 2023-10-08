#include "Object.h"

unsigned short Object::_current_id = 0;

Object::Object(const Mesh& p_mesh) :
	_id(_current_id),
	_position(),
	_rotation(),
	mesh(p_mesh),
	vao(),
	vbo(),
	ebo()
{
	++_current_id;
}
