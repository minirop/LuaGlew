/*
** See Copyright Notice in gl.cpp
*/

#ifndef LUA_GL_HELPERS_H
#define LUA_GL_HELPERS_H

#define LUA_LIB
#include <lua.hpp>

template<typename T>
T lua_to(lua_State * L, int index)
{
	return static_cast<T>(lua_tonumber(L, index));
}

#define LUA_TO_XXX(ogltype, luatype) \
template<> \
ogltype lua_to<ogltype>(lua_State * L, int index) \
{ \
	return static_cast<ogltype>(lua_to ## luatype(L, index)); \
}

LUA_TO_XXX(unsigned int, unsigned) // GLenum, GLbitfield, GLuint
LUA_TO_XXX(unsigned short, unsigned) // GLushort
LUA_TO_XXX(unsigned char, unsigned) // GLboolean, GLubyte
LUA_TO_XXX(int, integer) // GLint, GLsizei
LUA_TO_XXX(short, integer) // GLshort
LUA_TO_XXX(signed char, integer) // GLbyte
// LUA_TO_XXX(float, number) // GLfloat, GLclampf
// LUA_TO_XXX(double, number) // GLdouble, GLclampd
LUA_TO_XXX(const char *, string) // for 0-terminated strings (cf: glBindAttribLocation)
LUA_TO_XXX(const void *, userdata) // for pointer (cf: gluBuild2DMipmaps)
LUA_TO_XXX(const GLubyte *, userdata) // for pointer (cf: glBitmap)

template<typename T>
void push_gl(lua_State * L, T value)
{
	lua_pushnumber(L, value);
}

#define LUA_FROM_XXX(ogltype, luatype) \
template<> \
void push_gl(lua_State * L, ogltype value) \
{ \
	lua_push ## luatype(L, value); \
}

LUA_FROM_XXX(unsigned int, unsigned) // GLenum, GLbitfield, GLuint
LUA_FROM_XXX(unsigned short, unsigned) // GLushort
LUA_FROM_XXX(unsigned char, unsigned) // GLboolean, GLubyte
LUA_FROM_XXX(int, integer) // GLint, GLsizei
LUA_FROM_XXX(short, integer) // GLshort
LUA_FROM_XXX(signed char, integer) // GLbyte

#define CREATE_ARRAY(type, name, index) \
GLsizei count = luaL_len(L, index); \
type * name = new type[count];

#define FILL_ARRAY(name, index, converter) \
for(int i = 0;i < count;i++) \
{ \
	lua_rawgeti(L, index, (i+1)); \
	name[i] = lua_to ## converter(L, -1); \
	lua_pop(L, 1); \
}

#define FILL_TABLE(name, converter) \
for(GLsizei i = 0;i < count;i++) \
{ \
	lua_pushinteger(L, i+1); \
	lua_push ## converter(L, name[i]); \
	lua_settable(L, -3); \
}

#define GENERATE_SIGNATURE_2(func_name) \
static int lua_ ## func_name(lua_State * L)

#define GENERATE_SIGNATURE(func_name) GENERATE_SIGNATURE_2(func_name)

#define GENERATE_NOT_IMPL_FUNCTION(func_name) \
GENERATE_SIGNATURE(func_name) \
{ \
	char buffer[256]; \
	sprintf(buffer, "%s is not implemented", #func_name);\
	lua_pushstring(L, buffer); \
	lua_error(L); \
	return 0; \
}

#define GENERATE_VOID_FUNCTION(func_name) \
GENERATE_SIGNATURE(func_name) \
{ \
	(void)L; \
	func_name(); \
	return 0; \
}

#define GENERATE_VOID_FUNCTION_1(func_name, param1_type) \
GENERATE_SIGNATURE(func_name) \
{ \
	func_name(lua_to<param1_type>(L, 1)); \
	return 0; \
}

#define GENERATE_VOID_FUNCTION_2(func_name, param1_type, param2_type) \
GENERATE_SIGNATURE(func_name) \
{ \
	func_name(lua_to<param1_type>(L, 1), lua_to<param2_type>(L, 2)); \
	return 0; \
}

#define GENERATE_VOID_FUNCTION_3(func_name, param1_type, param2_type, param3_type) \
GENERATE_SIGNATURE(func_name) \
{ \
	func_name(lua_to<param1_type>(L, 1), lua_to<param2_type>(L, 2), lua_to<param3_type>(L, 3)); \
	return 0; \
}

#define GENERATE_VOID_FUNCTION_4(func_name, param1_type, param2_type, param3_type, param4_type) \
GENERATE_SIGNATURE(func_name) \
{ \
	func_name(lua_to<param1_type>(L, 1), lua_to<param2_type>(L, 2), lua_to<param3_type>(L, 3), lua_to<param4_type>(L, 4)); \
	return 0; \
}

#define GENERATE_VOID_FUNCTION_5(func_name, param1_type, param2_type, param3_type, param4_type, param5_type) \
GENERATE_SIGNATURE(func_name) \
{ \
	func_name(lua_to<param1_type>(L, 1), lua_to<param2_type>(L, 2), lua_to<param3_type>(L, 3), lua_to<param4_type>(L, 4), lua_to<param5_type>(L, 5)); \
	return 0; \
}

#define GENERATE_VOID_FUNCTION_6(func_name, param1_type, param2_type, param3_type, param4_type, param5_type, param6_type) \
GENERATE_SIGNATURE(func_name) \
{ \
	func_name(lua_to<param1_type>(L, 1), lua_to<param2_type>(L, 2), lua_to<param3_type>(L, 3), lua_to<param4_type>(L, 4), lua_to<param5_type>(L, 5), lua_to<param6_type>(L, 6)); \
	return 0; \
}

#define GENERATE_VOID_FUNCTION_7(func_name, param1_type, param2_type, param3_type, param4_type, param5_type, param6_type, param7_type) \
GENERATE_SIGNATURE(func_name) \
{ \
	func_name(lua_to<param1_type>(L, 1), lua_to<param2_type>(L, 2), lua_to<param3_type>(L, 3), lua_to<param4_type>(L, 4), lua_to<param5_type>(L, 5), lua_to<param6_type>(L, 6), lua_to<param7_type>(L, 7)); \
	return 0; \
}

#define GENERATE_VOID_FUNCTION_8(func_name, param1_type, param2_type, param3_type, param4_type, param5_type, param6_type, param7_type, param8_type) \
GENERATE_SIGNATURE(func_name) \
{ \
	func_name(lua_to<param1_type>(L, 1), lua_to<param2_type>(L, 2), lua_to<param3_type>(L, 3), lua_to<param4_type>(L, 4), lua_to<param5_type>(L, 5), lua_to<param6_type>(L, 6), lua_to<param7_type>(L, 7), lua_to<param8_type>(L, 8)); \
	return 0; \
}

#define GENERATE_VOID_FUNCTION_9(func_name, param1_type, param2_type, param3_type, param4_type, param5_type, param6_type, param7_type, param8_type, param9_type) \
GENERATE_SIGNATURE(func_name) \
{ \
	func_name(lua_to<param1_type>(L, 1), lua_to<param2_type>(L, 2), lua_to<param3_type>(L, 3), lua_to<param4_type>(L, 4), lua_to<param5_type>(L, 5), lua_to<param6_type>(L, 6), lua_to<param7_type>(L, 7), lua_to<param8_type>(L, 8), lua_to<param9_type>(L, 9)); \
	return 0; \
}

#define GENERATE_VOID_FUNCTION_11(func_name, param1_type, param2_type, param3_type, param4_type, param5_type, param6_type, param7_type, param8_type, param9_type, param10_type, param11_type) \
GENERATE_SIGNATURE(func_name) \
{ \
	func_name(lua_to<param1_type>(L, 1), lua_to<param2_type>(L, 2), lua_to<param3_type>(L, 3), lua_to<param4_type>(L, 4), lua_to<param5_type>(L, 5), lua_to<param6_type>(L, 6), lua_to<param7_type>(L, 7), lua_to<param8_type>(L, 8), lua_to<param9_type>(L, 9), lua_to<param10_type>(L, 10), lua_to<param11_type>(L, 11)); \
	return 0; \
}

#define GENERATE_RETURN_FUNCTION(func_name, return_type) \
GENERATE_SIGNATURE(func_name) \
{ \
	return_type ret = func_name(); \
	push_gl(L, ret); \
	return 1; \
}

#define GENERATE_RETURN_FUNCTION_1(func_name, return_type, param1_type) \
GENERATE_SIGNATURE(func_name) \
{ \
	return_type ret = func_name(lua_to<param1_type>(L, 1)); \
	push_gl(L, ret); \
	return 1; \
}

#define GENERATE_RETURN_FUNCTION_2(func_name, return_type, param1_type, param2_type) \
GENERATE_SIGNATURE(func_name) \
{ \
	return_type ret = func_name(lua_to<param1_type>(L, 1), lua_to<param2_type>(L, 1)); \
	push_gl(L, ret); \
	return 1; \
}

#endif
