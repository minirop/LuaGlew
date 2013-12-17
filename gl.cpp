/******************************************************************************
* Copyright (C) 2013 Alexander Roper
*
* Permission is hereby granted, free of charge, to any person obtaining
* a copy of this software and associated documentation files (the
* "Software"), to deal in the Software without restriction, including
* without limitation the rights to use, copy, modify, merge, publish,
* distribute, sublicense, and/or sell copies of the Software, and to
* permit persons to whom the Software is furnished to do so, subject to
* the following conditions:
*
* The above copyright notice and this permission notice shall be
* included in all copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
* IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
* CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
* TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
* SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
******************************************************************************/

#include <GL/glew.h>
#include <map>
#include <iostream>

#include "helpers.h"

// A
GENERATE_VOID_FUNCTION_2(glAccum, GLenum, GLfloat)
GENERATE_VOID_FUNCTION_1(glActiveTexture, GLenum)
GENERATE_VOID_FUNCTION_2(glAlphaFunc, GLenum, GLclampf)
GENERATE_SIGNATURE(glAreTexturesResident)
{
	CREATE_ARRAY(GLuint, textures, 1)
	GLboolean * residences = new GLboolean[count];
	
	FILL_ARRAY(textures, 1, unsigned)
	
	glAreTexturesResident(count, textures, residences);
	
	lua_newtable(L);
	
	FILL_TABLE(residences, count, boolean)
	
	delete[] textures;
	delete[] residences;
	
	return 1;
}
GENERATE_VOID_FUNCTION_1(glArrayElement, GLint)
GENERATE_VOID_FUNCTION_2(glAttachShader, GLuint, GLuint)

// B
GENERATE_VOID_FUNCTION_1(glBegin, GLenum)
GENERATE_VOID_FUNCTION_2(glBeginQuery, GLenum, GLuint)
GENERATE_VOID_FUNCTION_3(glBindAttribLocation, GLuint, GLuint, const GLchar*)
GENERATE_VOID_FUNCTION_2(glBindBuffer, GLenum, GLuint)
GENERATE_VOID_FUNCTION_2(glBindTexture, GLenum, GLuint)
GENERATE_VOID_FUNCTION_7(glBitmap, GLsizei, GLsizei, GLfloat, GLfloat, GLfloat, GLfloat, const GLubyte *)
GENERATE_VOID_FUNCTION_4(glBlendColor, GLclampf, GLclampf, GLclampf, GLclampf)
GENERATE_VOID_FUNCTION_1(glBlendEquation, GLenum)
GENERATE_VOID_FUNCTION_2(glBlendEquationSeparate, GLenum, GLenum)
GENERATE_VOID_FUNCTION_2(glBlendFunc, GLenum, GLenum)
GENERATE_VOID_FUNCTION_4(glBlendFuncSeparate, GLenum, GLenum, GLenum, GLenum)
GENERATE_VOID_FUNCTION_4(glBufferData, GLenum, GLsizeiptr, const GLvoid *, GLenum)
GENERATE_VOID_FUNCTION_4(glBufferSubData, GLenum, GLintptr, GLsizeiptr, const GLvoid *)

// C
GENERATE_VOID_FUNCTION_1(glCallList, GLuint)
GENERATE_SIGNATURE(glCallLists)
{
	GLsizei count = luaL_len(L, 1);
	
	for(int i = 0;i < count;i++)
	{
		lua_rawgeti(L, 1, (i+1));
		glCallList(lua_tounsigned(L, -1));
		lua_pop(L, 1);
	}
	
	return 0;
}
GENERATE_VOID_FUNCTION_1(glClear, GLbitfield)
GENERATE_VOID_FUNCTION_4(glClearAccum, GLfloat, GLfloat, GLfloat, GLfloat)
GENERATE_VOID_FUNCTION_4(glClearColor, GLclampf, GLclampf, GLclampf, GLclampf)
GENERATE_VOID_FUNCTION_1(glClearDepth, GLclampd)
GENERATE_VOID_FUNCTION_1(glClearIndex, GLfloat)
GENERATE_VOID_FUNCTION_1(glClearStencil, GLint)
GENERATE_VOID_FUNCTION_1(glClientActiveTexture, GLenum)
GENERATE_SIGNATURE(glClipPlane)
{
	static std::map<int, GLdouble *> data;
	
	GLenum plane = lua_to<GLenum>(L, 1);
	if(data.count(plane))
	{
		delete[] data[plane];
		data.erase(plane);
	}
	
	if(lua_gettop(L) == 1)
	{
		return 0;
	}
	
	data[plane] = new GLdouble[4];
	
	for(int i = 0;i < 4;i++)
	{
		lua_rawgeti(L, 1, (i+1));
		data[plane][i] = lua_tonumber(L, -1);
		lua_pop(L, 1);
	}
	
	glClipPlane(plane, data[plane]);
	
	return 0;
}
GENERATE_VOID_FUNCTION_3(glColor3d, GLdouble, GLdouble, GLdouble)
GENERATE_VOID_FUNCTION_4(glColor4d, GLdouble, GLdouble, GLdouble, GLdouble)
GENERATE_VOID_FUNCTION_4(glColorMask, GLboolean, GLboolean, GLboolean, GLboolean)
GENERATE_VOID_FUNCTION_2(glColorMaterial, GLenum, GLenum)
GENERATE_SIGNATURE(glColorPointer)
{
	static GLfloat * data = NULL;
	delete[] data;
	
	if(lua_gettop(L) == 0)
	{
		// if no param, only free the memory if necessary
		return 0;
	}
	
	GLint size = lua_to<GLint>(L, 1);
	
	GLsizei count = luaL_len(L, 2);
	data = new GLfloat[count];
	FILL_ARRAY(data, 2, number)
	
	glColorPointer(size, GL_FLOAT, 0, data);
	
	return 0;
}
GENERATE_VOID_FUNCTION_6(glColorSubTable, GLenum, GLsizei, GLsizei, GLenum, GLenum, const GLvoid *)
GENERATE_VOID_FUNCTION_6(glColorTable, GLenum, GLenum, GLsizei, GLenum, GLenum, const GLvoid *)
GENERATE_SIGNATURE(glColorTableParameterfv)
{
	static GLfloat * data = NULL;
	delete[] data;
	
	if(lua_gettop(L) == 0)
	{
		// if no param, only free the memory if necessary
		return 0;
	}
	
	GLenum target = lua_to<GLenum>(L, 1);
	GLenum pname = lua_to<GLenum>(L, 2);
	
	GLsizei count = luaL_len(L, 3);
	data = new GLfloat[count];
	FILL_ARRAY(data, 3, number)
	
	glColorTableParameterfv(target, pname, data);
	
	return 0;
}
GENERATE_VOID_FUNCTION_1(glCompileShader, GLuint)
GENERATE_VOID_FUNCTION_7(glCompressedTexImage1D, GLenum, GLint, GLenum, GLsizei, GLint, GLsizei, const GLvoid *)
GENERATE_VOID_FUNCTION_8(glCompressedTexImage2D, GLenum, GLint, GLenum, GLsizei, GLsizei, GLint, GLsizei, const GLvoid *)
GENERATE_VOID_FUNCTION_9(glCompressedTexImage3D, GLenum, GLint, GLenum, GLsizei, GLsizei, GLsizei, GLint, GLsizei, const GLvoid *)
GENERATE_VOID_FUNCTION_7(glCompressedTexSubImage1D, GLenum, GLint, GLint, GLsizei, GLenum, GLsizei, const GLvoid *)
GENERATE_VOID_FUNCTION_9(glCompressedTexSubImage2D, GLenum, GLint, GLint, GLint, GLsizei, GLsizei, GLenum, GLsizei, const GLvoid *)
GENERATE_VOID_FUNCTION_11(glCompressedTexSubImage3D, GLenum, GLint, GLint, GLint, GLint, GLsizei, GLsizei, GLsizei, GLenum, GLsizei, const GLvoid *)
GENERATE_VOID_FUNCTION_6(glConvolutionFilter1D, GLenum, GLenum, GLsizei, GLenum, GLenum, const GLvoid *)
GENERATE_VOID_FUNCTION_7(glConvolutionFilter2D, GLenum, GLenum, GLsizei, GLsizei, GLenum, GLenum, const GLvoid *)
GENERATE_VOID_FUNCTION_3(glConvolutionParameterf, GLenum, GLenum, GLfloat)
GENERATE_VOID_FUNCTION_5(glCopyColorSubTable, GLenum, GLsizei, GLint, GLint, GLsizei)
GENERATE_VOID_FUNCTION_5(glCopyColorTable, GLenum, GLenum, GLint, GLint, GLsizei)
GENERATE_VOID_FUNCTION_5(glCopyConvolutionFilter1D, GLenum, GLenum, GLint, GLint, GLsizei)
GENERATE_VOID_FUNCTION_6(glCopyConvolutionFilter2D, GLenum, GLenum, GLint, GLint, GLsizei, GLsizei)
GENERATE_VOID_FUNCTION_5(glCopyPixels, GLint, GLint, GLsizei, GLsizei, GLenum)
GENERATE_VOID_FUNCTION_7(glCopyTexImage1D, GLenum, GLint, GLenum, GLint, GLint, GLsizei, GLint)
GENERATE_VOID_FUNCTION_8(glCopyTexImage2D, GLenum, GLint, GLenum, GLint, GLint, GLsizei, GLsizei, GLint)
GENERATE_VOID_FUNCTION_6(glCopyTexSubImage1D, GLenum, GLint, GLint, GLint, GLint, GLsizei)
GENERATE_VOID_FUNCTION_8(glCopyTexSubImage2D, GLenum, GLint, GLint, GLint, GLint, GLint, GLsizei, GLsizei)
GENERATE_VOID_FUNCTION_9(glCopyTexSubImage3D, GLenum, GLint, GLint, GLint, GLint, GLint, GLint, GLsizei, GLsizei)
GENERATE_RETURN_FUNCTION(glCreateProgram, GLuint)
GENERATE_RETURN_FUNCTION_1(glCreateShader, GLuint, GLenum)
GENERATE_VOID_FUNCTION_1(glCullFace, GLenum)

// D
GENERATE_SIGNATURE(glDeleteBuffers)
{
	if(lua_isnumber(L, 1))
	{
		GLuint buffer = lua_tounsigned(L, 1);
		glDeleteBuffers(1, &buffer);
	}
	else
	{
		CREATE_ARRAY(GLuint, buffers, 1)
		FILL_ARRAY(buffers, 1, unsigned)
		
		glDeleteBuffers(count, buffers);
		delete[] buffers;
	}
	
	return 0;
}
GENERATE_VOID_FUNCTION_2(glDeleteLists, GLuint, GLsizei)
GENERATE_VOID_FUNCTION_1(glDeleteProgram, GLuint)
GENERATE_SIGNATURE(glDeleteQueries)
{
	if(lua_isnumber(L, 1))
	{
		GLuint query = lua_tounsigned(L, 1);
		glDeleteQueries(1, &query);
	}
	else
	{
		CREATE_ARRAY(GLuint, queries, 1)
		FILL_ARRAY(queries, 1, unsigned)
		
		glDeleteQueries(count, queries);
		delete[] queries;
	}
	
	return 0;
}
GENERATE_VOID_FUNCTION_1(glDeleteShader, GLuint)
GENERATE_SIGNATURE(glDeleteTextures)
{
	if(lua_isnumber(L, 1))
	{
		GLuint texture = lua_tounsigned(L, 1);
		glDeleteTextures(1, &texture);
	}
	else
	{
		CREATE_ARRAY(GLuint, textures, 1)
		FILL_ARRAY(textures, 1, unsigned)
		
		glDeleteTextures(count, textures);
		delete[] textures;
	}
	
	return 0;
}
GENERATE_VOID_FUNCTION_1(glDepthFunc, GLenum)
GENERATE_VOID_FUNCTION_1(glDepthMask, GLboolean)
GENERATE_VOID_FUNCTION_2(glDepthRange, GLclampd, GLclampd)
GENERATE_VOID_FUNCTION_2(glDetachShader, GLuint, GLuint)
GENERATE_VOID_FUNCTION_1(glDisable, GLenum)
GENERATE_VOID_FUNCTION_1(glDisableClientState, GLenum)
GENERATE_VOID_FUNCTION_1(glDisableVertexAttribArray, GLuint)
GENERATE_VOID_FUNCTION_3(glDrawArrays, GLenum, GLint, GLsizei)
GENERATE_VOID_FUNCTION_1(glDrawBuffer, GLenum)
GENERATE_SIGNATURE(glDrawBuffers)
{
	if(lua_isnumber(L, 1))
	{
		GLuint buffer = lua_tounsigned(L, 1);
		glDrawBuffers(1, &buffer);
	}
	else
	{
		CREATE_ARRAY(GLuint, buffers, 1)
		FILL_ARRAY(buffers, 1, unsigned)
		
		glDrawBuffers(count, buffers);
		delete[] buffers;
	}
	
	return 0;
}
GENERATE_SIGNATURE(glDrawElements)
{
	GLenum mode = lua_to<GLenum>(L, 1);
	
	CREATE_ARRAY(GLuint, indices, 2)
	FILL_ARRAY(indices, 2, unsigned)
	
	glDrawElements(mode, count, GL_UNSIGNED_INT, indices);
	delete[] indices;
	
	return 0;
}
GENERATE_SIGNATURE(glDrawPixels)
{
	GLsizei width = lua_to<GLsizei>(L, 1);
	GLsizei height = lua_to<GLsizei>(L, 2);
	GLenum format = lua_to<GLenum>(L, 3);
	
	CREATE_ARRAY(GLuint, pixels, 4)
	FILL_ARRAY(pixels, 4, unsigned)
	
	glDrawPixels(width, height, format, GL_UNSIGNED_INT, pixels);
	delete[] pixels;
	
	return 0;
}
GENERATE_SIGNATURE(glDrawRangeElements)
{
	GLenum mode = lua_to<GLenum>(L, 1);
	GLuint start = lua_to<GLuint>(L, 2);
	GLuint end = lua_to<GLuint>(L, 3);
	
	CREATE_ARRAY(GLuint, indices, 4)
	FILL_ARRAY(indices, 4, unsigned)
	
	glDrawRangeElements(mode, start, end, count, GL_UNSIGNED_INT, indices);
	delete[] indices;
	
	return 0;
}

// E
GENERATE_VOID_FUNCTION_1(glEdgeFlag, GLboolean)
GENERATE_NOT_IMPL_FUNCTION(glEdgeFlagPointer)
GENERATE_VOID_FUNCTION_1(glEnable, GLenum)
GENERATE_VOID_FUNCTION_1(glEnableClientState, GLenum)
GENERATE_VOID_FUNCTION_1(glEnableVertexAttribArray, GLuint)
GENERATE_VOID_FUNCTION(glEnd)
GENERATE_VOID_FUNCTION(glEndList)
GENERATE_VOID_FUNCTION_1(glEndQuery, GLenum)
GENERATE_VOID_FUNCTION_1(glEvalCoord1d, GLdouble)
GENERATE_VOID_FUNCTION_2(glEvalCoord2d, GLdouble, GLdouble)
GENERATE_VOID_FUNCTION_3(glEvalMesh1, GLenum, GLint, GLint)
GENERATE_VOID_FUNCTION_5(glEvalMesh2, GLenum, GLint, GLint, GLint, GLint)
GENERATE_VOID_FUNCTION_1(glEvalPoint1, GLint)
GENERATE_VOID_FUNCTION_2(glEvalPoint2, GLint, GLint)

// F
GENERATE_SIGNATURE(glFeedbackBuffer)
{
	GLsizei size = lua_to<GLsizei>(L, 1);
	GLenum type = lua_to<GLenum>(L, 2);
	GLfloat * buffers = new GLfloat[size];
	
	glFeedbackBuffer(size, type, buffers);
	
	lua_newtable(L);
	FILL_TABLE(buffers, size, number)
	
	delete[] buffers;
	return 1;
}
GENERATE_VOID_FUNCTION(glFinish)
GENERATE_VOID_FUNCTION(glFlush)
GENERATE_VOID_FUNCTION_2(glFogf, GLenum, GLfloat)
GENERATE_VOID_FUNCTION_1(glFogCoordd, GLdouble)
GENERATE_SIGNATURE(glFogCoordPointer)
{
	static GLdouble * data = NULL;
	delete[] data;
	
	if(lua_gettop(L) == 0)
	{
		// if no param, only free the memory if necessary
		return 0;
	}
	
	GLsizei count = luaL_len(L, 1);
	data = new GLdouble[count];
	FILL_ARRAY(data, 1, number)
	
	glFogCoordPointer(GL_DOUBLE, 0, data);
	
	return 0;
}
GENERATE_VOID_FUNCTION_1(glFrontFace, GLenum)
GENERATE_VOID_FUNCTION_6(glFrustum, GLdouble, GLdouble, GLdouble, GLdouble, GLdouble, GLdouble)

// G
GENERATE_SIGNATURE(glGenBuffers)
{
	GLsizei count = lua_to<GLsizei>(L, 1);
	GLuint * buffers = new GLuint[count];
	
	glGenBuffers(count, buffers);
	
	if(count == 1)
	{
		lua_pushunsigned(L, *buffers);
	}
	else
	{
		lua_newtable(L);
		FILL_TABLE(buffers, count, unsigned)
	}
	
	delete buffers;
	
	return 1;
}
GENERATE_RETURN_FUNCTION_1(glGenLists, GLuint, GLsizei)
GENERATE_SIGNATURE(glGenQueries)
{
	GLsizei count = lua_to<GLsizei>(L, 1);
	GLuint * queries = new GLuint[count];
	
	glGenQueries(count, queries);
	
	if(count == 1)
	{
		lua_pushunsigned(L, *queries);
	}
	else
	{
		lua_newtable(L);
		FILL_TABLE(queries, count, unsigned)
	}
	
	delete queries;
	
	return 1;
}
GENERATE_SIGNATURE(glGenTextures)
{
	GLsizei count = lua_to<GLsizei>(L, 1);
	GLuint * textures = new GLuint[count];
	
	glGenTextures(count, textures);
	
	if(count == 1)
	{
		lua_pushunsigned(L, *textures);
	}
	else
	{
		lua_newtable(L);
		
		FILL_TABLE(textures, count, unsigned)
	}
	
	delete textures;
	
	return 1;
}
GENERATE_SIGNATURE(glGet)
{
	GLuint type = lua_to<GLuint>(L, 1);
	GLuint pname = lua_to<GLuint>(L, 2);
	GLint count = 1;
	
	switch(pname)
	{
		case GL_ALIASED_POINT_SIZE_RANGE:
		case GL_ALIASED_LINE_WIDTH_RANGE:
		case GL_DEPTH_RANGE:
		case GL_LINE_WIDTH_RANGE:
		case GL_MAP1_GRID_DOMAIN:
		case GL_MAP2_GRID_SEGMENTS:
		case GL_MAX_VIEWPORT_DIMS:
		case GL_POINT_SIZE_RANGE:
		case GL_POLYGON_MODE:
		// case GL_SMOOTH_LINE_WIDTH_RANGE: // same value as GL_LINE_WIDTH_RANGE
		// case GL_SMOOTH_POINT_SIZE_RANGE: // same value as GL_POINT_SIZE_RANGE
			count = 2;
			break;
		case GL_CURRENT_NORMAL:
		case GL_POINT_DISTANCE_ATTENUATION:
			count = 3;
			break;
		case GL_ACCUM_CLEAR_VALUE:
		case GL_BLEND_COLOR:
		case GL_COLOR_CLEAR_VALUE:
		case GL_COLOR_WRITEMASK:
		case GL_CURRENT_COLOR:
		case GL_CURRENT_RASTER_COLOR:
		case GL_CURRENT_RASTER_POSITION:
		case GL_CURRENT_RASTER_SECONDARY_COLOR:
		case GL_CURRENT_RASTER_TEXTURE_COORDS:
		case GL_CURRENT_SECONDARY_COLOR:
		case GL_CURRENT_TEXTURE_COORDS:
		case GL_FOG_COLOR:
		case GL_LIGHT_MODEL_AMBIENT:
		case GL_MAP2_GRID_DOMAIN:
		case GL_SCISSOR_BOX:
		case GL_VIEWPORT:
			count = 4;
			break;
		case GL_COLOR_MATRIX:
		case GL_MODELVIEW_MATRIX:
		case GL_PROJECTION_MATRIX:
		case GL_TEXTURE_MATRIX:
		case GL_TRANSPOSE_COLOR_MATRIX:
		case GL_TRANSPOSE_MODELVIEW_MATRIX:
		case GL_TRANSPOSE_PROJECTION_MATRIX:
		case GL_TRANSPOSE_TEXTURE_MATRIX:
			count = 16;
			break;
		case GL_COMPRESSED_TEXTURE_FORMATS:
			{
				glGetIntegerv(GL_NUM_COMPRESSED_TEXTURE_FORMATS, &count);
			}
			break;
		default:
			; // still 1
	}
	
	switch(type)
	{
#define SWITCH_CASE(c, type, func) \
		case c: \
			{ \
				type param; \
				glGet ## func ## v(pname, &param); \
				push_gl(L, param); \
			} \
			break;
		SWITCH_CASE(GL_BOOL, GLboolean, Boolean)
		SWITCH_CASE(GL_DOUBLE, GLdouble, Double)
		SWITCH_CASE(GL_FLOAT, GLfloat, Float)
		SWITCH_CASE(GL_INT, GLint, Integer)
#undef SWITCH_CASE
		default:
			lua_pushnil(L);
	}
	return 1;
}
GENERATE_NOT_IMPL_FUNCTION(glGetActiveAttrib)
GENERATE_NOT_IMPL_FUNCTION(glGetActiveUniform)
GENERATE_NOT_IMPL_FUNCTION(glGetAttachedShaders)
GENERATE_RETURN_FUNCTION_2(glGetAttribLocation, GLint, GLuint, const GLchar *)
GENERATE_NOT_IMPL_FUNCTION(glGetBufferParameteriv)
GENERATE_NOT_IMPL_FUNCTION(glGetBufferPointerv)
GENERATE_NOT_IMPL_FUNCTION(glGetBufferSubData)
GENERATE_NOT_IMPL_FUNCTION(glGetClipPlane)
GENERATE_NOT_IMPL_FUNCTION(glGetColorTable)
GENERATE_NOT_IMPL_FUNCTION(glGetColorTableParameter)
GENERATE_NOT_IMPL_FUNCTION(glGetCompressedTexImage)
GENERATE_NOT_IMPL_FUNCTION(glGetConvolutionFilter)
GENERATE_NOT_IMPL_FUNCTION(glGetConvolutionParameter)
GENERATE_RETURN_FUNCTION(glGetError, GLenum)
GENERATE_NOT_IMPL_FUNCTION(glGetHistogram)
GENERATE_NOT_IMPL_FUNCTION(glGetHistogramParameter)
GENERATE_NOT_IMPL_FUNCTION(glGetLight)
GENERATE_NOT_IMPL_FUNCTION(glGetMap)
GENERATE_NOT_IMPL_FUNCTION(glGetMaterial)
GENERATE_NOT_IMPL_FUNCTION(glGetMinmax)
GENERATE_NOT_IMPL_FUNCTION(glGetMinmaxParameter)
GENERATE_NOT_IMPL_FUNCTION(glGetPixelMap)
GENERATE_NOT_IMPL_FUNCTION(glGetPointerv)
GENERATE_NOT_IMPL_FUNCTION(glGetPolygonStipple)
GENERATE_NOT_IMPL_FUNCTION(glGetProgram)
GENERATE_NOT_IMPL_FUNCTION(glGetProgramInfoLog)
GENERATE_NOT_IMPL_FUNCTION(glGetQueryObject)
GENERATE_NOT_IMPL_FUNCTION(glGetQueryiv)
GENERATE_NOT_IMPL_FUNCTION(glGetSeparableFilter)
GENERATE_NOT_IMPL_FUNCTION(glGetShader)
GENERATE_NOT_IMPL_FUNCTION(glGetShaderInfoLog)
GENERATE_NOT_IMPL_FUNCTION(glGetShaderSource)
GENERATE_NOT_IMPL_FUNCTION(glGetString)
GENERATE_NOT_IMPL_FUNCTION(glGetTexEnv)
GENERATE_NOT_IMPL_FUNCTION(glGetTexGen)
GENERATE_NOT_IMPL_FUNCTION(glGetTexImage)
GENERATE_NOT_IMPL_FUNCTION(glGetTexLevelParameter)
GENERATE_NOT_IMPL_FUNCTION(glGetTexParameter)
GENERATE_NOT_IMPL_FUNCTION(glGetUniform)
GENERATE_RETURN_FUNCTION_2(glGetUniformLocation, GLint, GLuint, const GLchar *)
GENERATE_NOT_IMPL_FUNCTION(glGetVertexAttrib)
GENERATE_NOT_IMPL_FUNCTION(glGetVertexAttribPointerv)

// H
GENERATE_VOID_FUNCTION_2(glHint, GLenum, GLenum)
GENERATE_VOID_FUNCTION_4(glHistogram, GLenum, GLsizei, GLenum, GLboolean)

// I
GENERATE_VOID_FUNCTION_1(glIndexd, GLdouble)
GENERATE_VOID_FUNCTION_1(glIndexMask, GLuint)
GENERATE_NOT_IMPL_FUNCTION(glIndexPointer)
GENERATE_VOID_FUNCTION(glInitNames)
GENERATE_NOT_IMPL_FUNCTION(glInterleavedArrays)
GENERATE_RETURN_FUNCTION_1(glIsBuffer, GLboolean, GLuint)
GENERATE_RETURN_FUNCTION_1(glIsEnabled, GLboolean, GLenum)
GENERATE_RETURN_FUNCTION_1(glIsList, GLboolean, GLuint)
GENERATE_RETURN_FUNCTION_1(glIsProgram, GLboolean, GLuint)
GENERATE_RETURN_FUNCTION_1(glIsQuery, GLboolean, GLuint)
GENERATE_RETURN_FUNCTION_1(glIsShader, GLboolean, GLuint)
GENERATE_RETURN_FUNCTION_1(glIsTexture, GLboolean, GLuint)

// L
GENERATE_VOID_FUNCTION_3(glLightf, GLenum, GLenum, GLfloat)
GENERATE_VOID_FUNCTION_2(glLightModelf, GLenum, GLfloat)
GENERATE_VOID_FUNCTION_2(glLineStipple, GLenum, GLushort)
GENERATE_VOID_FUNCTION_1(glLineWidth, GLfloat)
GENERATE_VOID_FUNCTION_1(glLinkProgram, GLuint)
GENERATE_VOID_FUNCTION_1(glListBase, GLuint)
GENERATE_VOID_FUNCTION(glLoadIdentity)
GENERATE_SIGNATURE(glLoadMatrixd)
{
	// check count == 16
	CREATE_ARRAY(GLdouble, matrix, 1)
	FILL_ARRAY(matrix, 1, number)
	
	glLoadMatrixd(matrix);
	delete[] matrix;
	
	return 0;
}
GENERATE_VOID_FUNCTION_1(glLoadName, GLuint)
GENERATE_SIGNATURE(glLoadTransposeMatrixd)
{
	// check count == 16
	CREATE_ARRAY(GLdouble, matrix, 1)
	FILL_ARRAY(matrix, 1, number)
	
	glLoadTransposeMatrixd(matrix);
	delete[] matrix;
	
	return 0;
}
GENERATE_VOID_FUNCTION_1(glLogicOp, GLenum)

// M
GENERATE_NOT_IMPL_FUNCTION(glMap1)
GENERATE_NOT_IMPL_FUNCTION(glMap2)
GENERATE_NOT_IMPL_FUNCTION(glMapBuffer)
GENERATE_VOID_FUNCTION_3(glMapGrid1d, GLint, GLdouble, GLdouble)
GENERATE_VOID_FUNCTION_6(glMapGrid2d, GLint, GLdouble, GLdouble, GLint, GLdouble, GLdouble)
GENERATE_VOID_FUNCTION_3(glMaterialf, GLenum, GLenum, GLfloat)
GENERATE_VOID_FUNCTION_1(glMatrixMode, GLenum)
GENERATE_VOID_FUNCTION_3(glMinmax, GLenum, GLenum, GLboolean)
GENERATE_NOT_IMPL_FUNCTION(glMultMatrix)
GENERATE_NOT_IMPL_FUNCTION(glMultTransposeMatrix)
GENERATE_NOT_IMPL_FUNCTION(glMultiDrawArrays)
GENERATE_NOT_IMPL_FUNCTION(glMultiDrawElements)
GENERATE_VOID_FUNCTION_2(glMultiTexCoord1d, GLenum, GLdouble)
GENERATE_VOID_FUNCTION_3(glMultiTexCoord2d, GLenum, GLdouble, GLdouble)
GENERATE_VOID_FUNCTION_4(glMultiTexCoord3d, GLenum, GLdouble, GLdouble, GLdouble)
GENERATE_VOID_FUNCTION_5(glMultiTexCoord4d, GLenum, GLdouble, GLdouble, GLdouble, GLdouble)

// N
GENERATE_VOID_FUNCTION_2(glNewList, GLuint, GLenum)
GENERATE_VOID_FUNCTION_3(glNormal3d, GLdouble, GLdouble, GLdouble)
GENERATE_NOT_IMPL_FUNCTION(glNormalPointer)

// O
GENERATE_VOID_FUNCTION_6(glOrtho, GLdouble, GLdouble, GLdouble, GLdouble, GLdouble, GLdouble)

// P
GENERATE_VOID_FUNCTION_1(glPassThrough, GLfloat)
GENERATE_NOT_IMPL_FUNCTION(glPixelMap)
GENERATE_VOID_FUNCTION_2(glPixelStoref, GLenum, GLfloat)
GENERATE_VOID_FUNCTION_2(glPixelTransferf, GLenum, GLfloat)
GENERATE_VOID_FUNCTION_2(glPixelZoom, GLfloat, GLfloat)
GENERATE_VOID_FUNCTION_2(glPointParameterf, GLenum, GLfloat)
GENERATE_VOID_FUNCTION_1(glPointSize, GLfloat)
GENERATE_VOID_FUNCTION_2(glPolygonMode, GLenum, GLenum)
GENERATE_VOID_FUNCTION_2(glPolygonOffset, GLfloat, GLfloat)
GENERATE_NOT_IMPL_FUNCTION(glPolygonStipple)
GENERATE_VOID_FUNCTION(glPopAttrib)
GENERATE_VOID_FUNCTION(glPopClientAttrib)
GENERATE_VOID_FUNCTION(glPopMatrix)
GENERATE_VOID_FUNCTION(glPopName)
GENERATE_NOT_IMPL_FUNCTION(glPrioritizeTextures)
GENERATE_VOID_FUNCTION_1(glPushAttrib, GLbitfield)
GENERATE_VOID_FUNCTION_1(glPushClientAttrib, GLbitfield)
GENERATE_VOID_FUNCTION(glPushMatrix)
GENERATE_VOID_FUNCTION_1(glPushName, GLuint)

// R
GENERATE_VOID_FUNCTION_2(glRasterPos2d, GLdouble, GLdouble)
GENERATE_VOID_FUNCTION_3(glRasterPos3d, GLdouble, GLdouble, GLdouble)
GENERATE_VOID_FUNCTION_4(glRasterPos4d, GLdouble, GLdouble, GLdouble, GLdouble)
GENERATE_VOID_FUNCTION_1(glReadBuffer, GLenum)
GENERATE_NOT_IMPL_FUNCTION(glReadPixels)
GENERATE_VOID_FUNCTION_4(glRectd, GLdouble, GLdouble, GLdouble, GLdouble)
GENERATE_RETURN_FUNCTION_1(glRenderMode, GLint, GLenum)
GENERATE_VOID_FUNCTION_1(glResetHistogram, GLenum)
GENERATE_VOID_FUNCTION_1(glResetMinmax, GLenum)
GENERATE_VOID_FUNCTION_4(glRotated, GLdouble, GLdouble, GLdouble, GLdouble)

// S
GENERATE_VOID_FUNCTION_2(glSampleCoverage, GLclampf, GLboolean)
GENERATE_VOID_FUNCTION_3(glScaled, GLdouble, GLdouble, GLdouble)
GENERATE_VOID_FUNCTION_4(glScissor, GLint, GLint, GLsizei, GLsizei)
GENERATE_VOID_FUNCTION_3(glSecondaryColor3d, GLdouble, GLdouble, GLdouble)
GENERATE_NOT_IMPL_FUNCTION(glSecondaryColorPointer)
GENERATE_NOT_IMPL_FUNCTION(glSelectBuffer)
GENERATE_NOT_IMPL_FUNCTION(glSeparableFilter2D)
GENERATE_VOID_FUNCTION_1(glShadeModel, GLenum)
GENERATE_NOT_IMPL_FUNCTION(glShaderSource)
GENERATE_VOID_FUNCTION_3(glStencilFunc, GLenum, GLint, GLuint)
GENERATE_VOID_FUNCTION_4(glStencilFuncSeparate, GLenum, GLenum, GLint, GLuint)
GENERATE_VOID_FUNCTION_1(glStencilMask, GLuint)
GENERATE_VOID_FUNCTION_2(glStencilMaskSeparate, GLenum, GLuint)
GENERATE_VOID_FUNCTION_3(glStencilOp, GLenum, GLenum, GLenum)
GENERATE_VOID_FUNCTION_4(glStencilOpSeparate, GLenum, GLenum, GLenum, GLenum)

// T
GENERATE_VOID_FUNCTION_1(glTexCoord1d, GLdouble)
GENERATE_VOID_FUNCTION_2(glTexCoord2d, GLdouble, GLdouble)
GENERATE_VOID_FUNCTION_3(glTexCoord3d, GLdouble, GLdouble, GLdouble)
GENERATE_VOID_FUNCTION_4(glTexCoord4d, GLdouble, GLdouble, GLdouble, GLdouble)
GENERATE_SIGNATURE(glTexCoordPointer)
{
	static GLfloat * data = NULL;
	delete[] data;
	
	if(lua_gettop(L) == 0)
	{
		// if no param, only free the memory if necessary
		return 0;
	}
	
	GLint size = lua_to<GLint>(L, 1);
	
	GLsizei count = luaL_len(L, 2);
	data = new GLfloat[count];
	FILL_ARRAY(data, 2, number)
	
	glTexCoordPointer(size, GL_FLOAT, 0, data);
	
	return 0;
}
GENERATE_VOID_FUNCTION_3(glTexEnvf, GLenum, GLenum, GLfloat)
GENERATE_VOID_FUNCTION_3(glTexGend, GLenum, GLenum, GLdouble)
GENERATE_NOT_IMPL_FUNCTION(glTexImage1D)
GENERATE_NOT_IMPL_FUNCTION(glTexImage2D)
GENERATE_NOT_IMPL_FUNCTION(glTexImage3D)
GENERATE_VOID_FUNCTION_3(glTexParameterf, GLenum, GLenum, GLfloat)
GENERATE_NOT_IMPL_FUNCTION(glTexSubImage1D)
GENERATE_NOT_IMPL_FUNCTION(glTexSubImage2D)
GENERATE_NOT_IMPL_FUNCTION(glTexSubImage3D)
GENERATE_VOID_FUNCTION_3(glTranslated, GLdouble, GLdouble, GLdouble)

// U
GENERATE_VOID_FUNCTION_2(glUniform1f, GLint, GLfloat)
GENERATE_VOID_FUNCTION_3(glUniform2f, GLint, GLfloat, GLfloat)
GENERATE_VOID_FUNCTION_4(glUniform3f, GLint, GLfloat, GLfloat, GLfloat)
GENERATE_VOID_FUNCTION_5(glUniform4f, GLint, GLfloat, GLfloat, GLfloat, GLfloat)
GENERATE_RETURN_FUNCTION_1(glUnmapBuffer, GLboolean, GLenum)
GENERATE_VOID_FUNCTION_1(glUseProgram, GLuint)

// V
GENERATE_VOID_FUNCTION_1(glValidateProgram, GLuint)
GENERATE_VOID_FUNCTION_2(glVertex2d, GLdouble, GLdouble)
GENERATE_VOID_FUNCTION_3(glVertex3d, GLdouble, GLdouble, GLdouble)
GENERATE_VOID_FUNCTION_4(glVertex4d, GLdouble, GLdouble, GLdouble, GLdouble)
GENERATE_VOID_FUNCTION_2(glVertexAttrib1d, GLuint, GLdouble)
GENERATE_VOID_FUNCTION_3(glVertexAttrib2d, GLuint, GLdouble, GLdouble)
GENERATE_VOID_FUNCTION_4(glVertexAttrib3d, GLuint, GLdouble, GLdouble, GLdouble)
GENERATE_VOID_FUNCTION_5(glVertexAttrib4d, GLuint, GLdouble, GLdouble, GLdouble, GLdouble)
GENERATE_SIGNATURE(glVertexAttribPointer)
{
	static GLfloat * data = NULL;
	delete[] data;
	
	if(lua_gettop(L) == 0)
	{
		// if no param, only free the memory if necessary
		return 0;
	}
	
	GLuint index = lua_to<GLuint>(L, 1);
	GLint size = lua_to<GLint>(L, 2);
	GLboolean normalized = lua_to<GLboolean>(L, 3);
	
	GLsizei count = luaL_len(L, 4);
	data = new GLfloat[count];
	FILL_ARRAY(data, 4, number)
	
	glVertexAttribPointer(index, size, GL_FLOAT, normalized, 0, data);
	
	return 0;
}
GENERATE_SIGNATURE(glVertexPointer)
{
	static GLfloat * data = NULL;
	delete[] data;
	
	if(lua_gettop(L) == 0)
	{
		// if no param, only free the memory if necessary
		return 0;
	}
	
	GLint size = lua_to<GLint>(L, 1);
	
	GLsizei count = luaL_len(L, 2);
	data = new GLfloat[count];
	FILL_ARRAY(data, 2, number)
	
	glVertexPointer(size, GL_FLOAT, 0, data);
	
	return 0;
}
GENERATE_VOID_FUNCTION_4(glViewport, GLint, GLint, GLsizei, GLsizei)

// W
GENERATE_VOID_FUNCTION_2(glWindowPos2d, GLdouble, GLdouble)
GENERATE_VOID_FUNCTION_3(glWindowPos3d, GLdouble, GLdouble, GLdouble)

#ifndef GLEW_NO_GLU

// gluBeginCurve
// gluBeginPolygon
// gluBeginSurface
// gluBeginTrim
// gluBuild1DMipmapLevels
// gluBuild1DMipmaps
// gluBuild2DMipmapLevels
GENERATE_VOID_FUNCTION_7(gluBuild2DMipmaps, GLenum, GLint, GLsizei, GLsizei, GLenum, GLenum, const void *)
// gluBuild3DMipmapLevels
// gluBuild3DMipmaps
// gluCheckExtension
// gluCylinder
// gluDeleteNurbsRenderer
// gluDeleteQuadric
// gluDeleteTess
// gluDisk
// gluEndCurve
// gluEndPolygon
// gluEndSurface
// gluEndTrim
// gluErrorString
// gluGetNurbsProperty
// gluGetString
// gluGetTessProperty
// gluLoadSamplingMatrices
GENERATE_VOID_FUNCTION_9(gluLookAt, GLdouble, GLdouble, GLdouble, GLdouble, GLdouble, GLdouble, GLdouble, GLdouble, GLdouble)
// gluNewNurbsRenderer
// gluNewQuadric
// gluNewTess
// gluNextContour
// gluNurbsCallback
// gluNurbsCallbackData
// gluNurbsCallbackDataEXT
// gluNurbsCurve
// gluNurbsProperty
// gluNurbsSurface
GENERATE_VOID_FUNCTION_4(gluOrtho2D, GLdouble, GLdouble, GLdouble, GLdouble)
// gluPartialDisk
GENERATE_VOID_FUNCTION_4(gluPerspective, GLdouble, GLdouble, GLdouble, GLdouble)
// gluPickMatrix
// gluProject
// gluPwlCurve
// gluQuadricCallback
// gluQuadricDrawStyle
// gluQuadricNormals
// gluQuadricOrientation
// gluQuadricTexture
// gluScaleImage
// gluSphere
// gluTessBeginContour
// gluTessBeginPolygon
// gluTessCallback
// gluTessEndContour
// gluTessEndPolygon
// gluTessNormal
// gluTessProperty
// gluTessVertex
// gluUnProject
// gluUnProject4

#endif

#define ENUM_PARAM_2_2(lua_name, func_name) { #lua_name , lua_## func_name }
#define ENUM_PARAM_2(lua_name, func_name) ENUM_PARAM_2_2(lua_name, func_name)

static const luaL_Reg libgl[] = {
#define ENUM_PARAM(lua_name) ENUM_PARAM_2(lua_name, gl ## lua_name)
	// A
	ENUM_PARAM(Accum),
	ENUM_PARAM(ActiveTexture),
	ENUM_PARAM(AlphaFunc),
	ENUM_PARAM(AreTexturesResident),
	ENUM_PARAM(ArrayElement),
	ENUM_PARAM(AttachShader),
	
	// B
	ENUM_PARAM(Begin),
	ENUM_PARAM(BeginQuery),
	ENUM_PARAM(BindAttribLocation),
	ENUM_PARAM(BindBuffer),
	ENUM_PARAM(BindTexture),
	ENUM_PARAM(Bitmap),
	ENUM_PARAM(BlendColor),
	ENUM_PARAM(BlendEquation),
	ENUM_PARAM(BlendEquationSeparate),
	ENUM_PARAM(BlendFunc),
	ENUM_PARAM(BlendFuncSeparate),
	ENUM_PARAM(BufferData),
	ENUM_PARAM(BufferSubData),
	
	// C
	ENUM_PARAM(CallList),
	ENUM_PARAM(CallLists),
	ENUM_PARAM(Clear),
	ENUM_PARAM(ClearAccum),
	ENUM_PARAM(ClearColor),
	ENUM_PARAM(ClearDepth),
	ENUM_PARAM(ClearIndex),
	ENUM_PARAM(ClearStencil),
	ENUM_PARAM(ClientActiveTexture),
	ENUM_PARAM(ClipPlane),
	ENUM_PARAM_2(Color3, glColor3d),
	ENUM_PARAM_2(Color4, glColor4d),
	ENUM_PARAM(ColorMask),
	ENUM_PARAM(ColorMaterial),
	ENUM_PARAM(ColorPointer),
	ENUM_PARAM(ColorSubTable),
	ENUM_PARAM(ColorTable),
	ENUM_PARAM_2(ColorTableParameter, glColorTableParameterfv),
	ENUM_PARAM(CompileShader),
	ENUM_PARAM(CompressedTexImage1D),
	ENUM_PARAM(CompressedTexImage2D),
	ENUM_PARAM(CompressedTexImage3D),
	ENUM_PARAM(CompressedTexSubImage1D),
	ENUM_PARAM(CompressedTexSubImage2D),
	ENUM_PARAM(CompressedTexSubImage3D),
	ENUM_PARAM(ConvolutionFilter1D),
	ENUM_PARAM(ConvolutionFilter2D),
	ENUM_PARAM_2(ConvolutionParameter, glConvolutionParameterf),
	ENUM_PARAM(CopyColorSubTable),
	ENUM_PARAM(CopyColorTable),
	ENUM_PARAM(CopyConvolutionFilter1D),
	ENUM_PARAM(CopyConvolutionFilter2D),
	ENUM_PARAM(CopyPixels),
	ENUM_PARAM(CopyTexImage1D),
	ENUM_PARAM(CopyTexImage2D),
	ENUM_PARAM(CopyTexSubImage1D),
	ENUM_PARAM(CopyTexSubImage2D),
	ENUM_PARAM(CopyTexSubImage3D),
	ENUM_PARAM(CreateProgram),
	ENUM_PARAM(CreateShader),
	ENUM_PARAM(CullFace),

	// D
	ENUM_PARAM(DeleteBuffers),
	ENUM_PARAM(DeleteLists),
	ENUM_PARAM(DeleteProgram),
	ENUM_PARAM(DeleteQueries),
	ENUM_PARAM(DeleteShader),
	ENUM_PARAM(DeleteTextures),
	ENUM_PARAM(DepthFunc),
	ENUM_PARAM(DepthMask),
	ENUM_PARAM(DepthRange),
	ENUM_PARAM(DetachShader),
	ENUM_PARAM(Disable),
	ENUM_PARAM(DisableClientState),
	ENUM_PARAM(DisableVertexAttribArray),
	ENUM_PARAM(DrawArrays),
	ENUM_PARAM(DrawBuffer),
	ENUM_PARAM(DrawBuffers),
	ENUM_PARAM(DrawElements),
	ENUM_PARAM(DrawPixels),
	ENUM_PARAM(DrawRangeElements),

	// E
	ENUM_PARAM(EdgeFlag),
	ENUM_PARAM(EdgeFlagPointer),
	ENUM_PARAM(Enable),
	ENUM_PARAM(EnableClientState),
	ENUM_PARAM(EnableVertexAttribArray),
	ENUM_PARAM(End),
	ENUM_PARAM(EndList),
	ENUM_PARAM(EndQuery),
	ENUM_PARAM_2(EvalCoord1, glEvalCoord1d),
	ENUM_PARAM_2(EvalCoord2, glEvalCoord2d),
	ENUM_PARAM(EvalMesh1),
	ENUM_PARAM(EvalMesh2),
	ENUM_PARAM(EvalPoint1),
	ENUM_PARAM(EvalPoint2),

	// F
	ENUM_PARAM(FeedbackBuffer),
	ENUM_PARAM(Finish),
	ENUM_PARAM(Flush),
	ENUM_PARAM_2(Fog, glFogf),
	ENUM_PARAM_2(FogCoord, glFogCoordd),
	ENUM_PARAM(FogCoordPointer),
	ENUM_PARAM(FrontFace),
	ENUM_PARAM(Frustum),

	// G
	ENUM_PARAM(GenBuffers),
	ENUM_PARAM(GenLists),
	ENUM_PARAM(GenQueries),
	ENUM_PARAM(GenTextures),
	ENUM_PARAM(Get),
	ENUM_PARAM(GetActiveAttrib),
	ENUM_PARAM(GetActiveUniform),
	ENUM_PARAM(GetAttachedShaders),
	ENUM_PARAM(GetAttribLocation),
	ENUM_PARAM_2(GetBufferParameter, glGetBufferParameteriv),
	ENUM_PARAM_2(GetBufferPointer, glGetBufferPointerv),
	ENUM_PARAM(GetBufferSubData),
	ENUM_PARAM(GetClipPlane),
	ENUM_PARAM(GetColorTable),
	ENUM_PARAM(GetColorTableParameter),
	ENUM_PARAM(GetCompressedTexImage),
	ENUM_PARAM(GetConvolutionFilter),
	ENUM_PARAM(GetConvolutionParameter),
	ENUM_PARAM(GetError),
	ENUM_PARAM(GetHistogram),
	ENUM_PARAM(GetHistogramParameter),
	ENUM_PARAM(GetLight),
	ENUM_PARAM(GetMap),
	ENUM_PARAM(GetMaterial),
	ENUM_PARAM(GetMinmax),
	ENUM_PARAM(GetMinmaxParameter),
	ENUM_PARAM(GetPixelMap),
	ENUM_PARAM_2(GetPointer, glGetPointerv),
	ENUM_PARAM(GetPolygonStipple),
	ENUM_PARAM(GetProgram),
	ENUM_PARAM(GetProgramInfoLog),
	ENUM_PARAM(GetQueryObject),
	ENUM_PARAM_2(GetQuery, glGetQueryiv),
	ENUM_PARAM(GetSeparableFilter),
	ENUM_PARAM(GetShader),
	ENUM_PARAM(GetShaderInfoLog),
	ENUM_PARAM(GetShaderSource),
	ENUM_PARAM(GetString),
	ENUM_PARAM(GetTexEnv),
	ENUM_PARAM(GetTexGen),
	ENUM_PARAM(GetTexImage),
	ENUM_PARAM(GetTexLevelParameter),
	ENUM_PARAM(GetTexParameter),
	ENUM_PARAM(GetUniform),
	ENUM_PARAM(GetUniformLocation),
	ENUM_PARAM(GetVertexAttrib),
	ENUM_PARAM(GetVertexAttribPointerv),

	// H
	ENUM_PARAM(Hint),
	ENUM_PARAM(Histogram),

	// I
	ENUM_PARAM_2(Index, glIndexd),
	ENUM_PARAM(IndexMask),
	ENUM_PARAM(IndexPointer),
	ENUM_PARAM(InitNames),
	ENUM_PARAM(InterleavedArrays),
	ENUM_PARAM(IsBuffer),
	ENUM_PARAM(IsEnabled),
	ENUM_PARAM(IsList),
	ENUM_PARAM(IsProgram),
	ENUM_PARAM(IsQuery),
	ENUM_PARAM(IsShader),
	ENUM_PARAM(IsTexture),

	// L
	ENUM_PARAM_2(Light, glLightf),
	ENUM_PARAM_2(LightModel, glLightModelf),
	ENUM_PARAM(LineStipple),
	ENUM_PARAM(LineWidth),
	ENUM_PARAM(LinkProgram),
	ENUM_PARAM(ListBase),
	ENUM_PARAM(LoadIdentity),
	ENUM_PARAM_2(LoadMatrix, glLoadMatrixd),
	ENUM_PARAM(LoadName),
	ENUM_PARAM_2(LoadTransposeMatrix, glLoadTransposeMatrixd),
	ENUM_PARAM(LogicOp),

	// L
	ENUM_PARAM_2(Light, glLightf),
	ENUM_PARAM_2(LightModel, glLightModelf),
	ENUM_PARAM(LineStipple),
	ENUM_PARAM(LineWidth),
	ENUM_PARAM(LinkProgram),
	ENUM_PARAM(ListBase),
	ENUM_PARAM(LoadIdentity),
	ENUM_PARAM_2(LoadMatrix, glLoadMatrixd),
	ENUM_PARAM(LoadName),
	ENUM_PARAM_2(LoadTransposeMatrix, glLoadTransposeMatrixd),
	ENUM_PARAM(LogicOp),

	// M
	ENUM_PARAM(Map1),
	ENUM_PARAM(Map2),
	ENUM_PARAM(MapBuffer),
	ENUM_PARAM_2(MapGrid1, glMapGrid1d),
	ENUM_PARAM_2(MapGrid2, glMapGrid2d),
	ENUM_PARAM_2(Material, glMaterialf),
	ENUM_PARAM(MatrixMode),
	ENUM_PARAM(Minmax),
	ENUM_PARAM(MultMatrix),
	ENUM_PARAM(MultTransposeMatrix),
	ENUM_PARAM(MultiDrawArrays),
	ENUM_PARAM(MultiDrawElements),
	ENUM_PARAM_2(MultiTexCoord1, glMultiTexCoord1d),
	ENUM_PARAM_2(MultiTexCoord2, glMultiTexCoord2d),
	ENUM_PARAM_2(MultiTexCoord3, glMultiTexCoord3d),
	ENUM_PARAM_2(MultiTexCoord4, glMultiTexCoord4d),

	// N
	ENUM_PARAM(NewList),
	ENUM_PARAM_2(Normal3, glNormal3d),
	ENUM_PARAM(NormalPointer),

	// O
	ENUM_PARAM(Ortho),

	// P
	ENUM_PARAM(PassThrough),
	ENUM_PARAM(PixelMap),
	ENUM_PARAM_2(PixelStore, glPixelStoref),
	ENUM_PARAM_2(PixelTransfer, glPixelTransferf),
	ENUM_PARAM(PixelZoom),
	ENUM_PARAM_2(PointParameter, glPointParameterf),
	ENUM_PARAM(PointSize),
	ENUM_PARAM(PolygonMode),
	ENUM_PARAM(PolygonOffset),
	ENUM_PARAM(PolygonStipple),
	ENUM_PARAM(PopAttrib),
	ENUM_PARAM(PopClientAttrib),
	ENUM_PARAM(PopMatrix),
	ENUM_PARAM(PopName),
	ENUM_PARAM(PrioritizeTextures),
	ENUM_PARAM(PushAttrib),
	ENUM_PARAM(PushClientAttrib),
	ENUM_PARAM(PushMatrix),
	ENUM_PARAM(PushName),

	// R
	ENUM_PARAM_2(RasterPos2, glRasterPos2d),
	ENUM_PARAM_2(RasterPos3, glRasterPos3d),
	ENUM_PARAM_2(RasterPos4, glRasterPos4d),
	ENUM_PARAM(ReadBuffer),
	ENUM_PARAM(ReadPixels),
	ENUM_PARAM_2(Rect, glRectd),
	ENUM_PARAM(RenderMode),
	ENUM_PARAM(ResetHistogram),
	ENUM_PARAM(ResetMinmax),
	ENUM_PARAM_2(Rotate, glRotated),

	// S
	ENUM_PARAM(SampleCoverage),
	ENUM_PARAM_2(Scale, glScaled),
	ENUM_PARAM(Scissor),
	ENUM_PARAM_2(SecondaryColor3, glSecondaryColor3d),
	ENUM_PARAM(SecondaryColorPointer),
	ENUM_PARAM(SelectBuffer),
	ENUM_PARAM(SeparableFilter2D),
	ENUM_PARAM(ShadeModel),
	ENUM_PARAM(ShaderSource),
	ENUM_PARAM(StencilFunc),
	ENUM_PARAM(StencilFuncSeparate),
	ENUM_PARAM(StencilMask),
	ENUM_PARAM(StencilMaskSeparate),
	ENUM_PARAM(StencilOp),
	ENUM_PARAM(StencilOpSeparate),

	// T
	ENUM_PARAM_2(TexCoord1, glTexCoord1d),
	ENUM_PARAM_2(TexCoord2, glTexCoord2d),
	ENUM_PARAM_2(TexCoord3, glTexCoord3d),
	ENUM_PARAM_2(TexCoord4, glTexCoord4d),
	ENUM_PARAM(TexCoordPointer),
	ENUM_PARAM_2(TexEnv, glTexEnvf),
	ENUM_PARAM_2(TexGen, glTexGend),
	ENUM_PARAM(TexImage1D),
	ENUM_PARAM(TexImage2D),
	ENUM_PARAM(TexImage3D),
	ENUM_PARAM_2(TexParameter, glTexParameterf),
	ENUM_PARAM(TexSubImage1D),
	ENUM_PARAM(TexSubImage2D),
	ENUM_PARAM(TexSubImage3D),
	ENUM_PARAM_2(Translate, glTranslated),

	// U
	ENUM_PARAM_2(Uniform1, glUniform1f),
	ENUM_PARAM_2(Uniform2, glUniform2f),
	ENUM_PARAM_2(Uniform3, glUniform3f),
	ENUM_PARAM_2(Uniform4, glUniform4f),
	ENUM_PARAM(UnmapBuffer),
	ENUM_PARAM(UseProgram),

	// V
	ENUM_PARAM(ValidateProgram),
	ENUM_PARAM_2(Vertex2, glVertex2d),
	ENUM_PARAM_2(Vertex3, glVertex3d),
	ENUM_PARAM_2(Vertex4, glVertex4d),
	ENUM_PARAM_2(VertexAttrib1, glVertexAttrib1d),
	ENUM_PARAM_2(VertexAttrib2, glVertexAttrib2d),
	ENUM_PARAM_2(VertexAttrib3, glVertexAttrib3d),
	ENUM_PARAM_2(VertexAttrib4, glVertexAttrib4d),
	ENUM_PARAM(VertexAttribPointer),
	ENUM_PARAM(VertexPointer),
	ENUM_PARAM(Viewport),

	// W
	ENUM_PARAM_2(WindowPos2, glWindowPos2d),
	ENUM_PARAM_2(WindowPos3, glWindowPos3d),
	
#ifndef GLEW_NO_GLU
	ENUM_PARAM_2(gluBuild2DMipmaps, gluBuild2DMipmaps),
	ENUM_PARAM_2(gluLookAt, gluLookAt),
	ENUM_PARAM_2(gluOrtho2D, gluOrtho2D),
	ENUM_PARAM_2(gluPerspective, gluPerspective),
#endif /* GLEW_NO_GLU */

	{NULL, NULL}
#undef ENUM_PARAM
#undef ENUM_PARAM_2
#undef ENUM_PARAM_2_2
};

#define ADD_DEFINE(x) \
	lua_pushunsigned(L, x); \
	lua_setfield(L, -2, #x);

#define ADD_DEFINE_DOUBLE(x) \
	lua_pushnumber(L, x); \
	lua_setfield(L, -2, #x);

extern "C" __declspec(dllexport) int luaopen_luaglew(lua_State * L)
{
	luaL_newlib(L, libgl);
	
	ADD_DEFINE(GL_DEPTH_TEST)
	ADD_DEFINE(GL_LIGHTING)
	ADD_DEFINE(GL_TEXTURE_2D)
	ADD_DEFINE(GL_PROJECTION)
	ADD_DEFINE(GL_VERTEX_ARRAY)
	ADD_DEFINE(GL_COLOR_ARRAY)
	ADD_DEFINE(GL_FLOAT)
	ADD_DEFINE(GL_NORMAL_ARRAY)
	ADD_DEFINE(GL_TEXTURE_COORD_ARRAY)
	ADD_DEFINE(GL_TRIANGLES)
	ADD_DEFINE(GL_COLOR_BUFFER_BIT)
	ADD_DEFINE(GL_DEPTH_BUFFER_BIT)
	ADD_DEFINE(GL_TRUE)
	ADD_DEFINE(GL_MODELVIEW)
	ADD_DEFINE(GL_RGBA)
	ADD_DEFINE(GL_UNSIGNED_BYTE)
	ADD_DEFINE(GL_TEXTURE_MAG_FILTER)
	ADD_DEFINE(GL_TEXTURE_MIN_FILTER)
	ADD_DEFINE(GL_LINEAR)
	ADD_DEFINE(GL_LINEAR_MIPMAP_LINEAR)
	ADD_DEFINE(GL_QUADS)
	ADD_DEFINE(GL_LINES)
	ADD_DEFINE(GL_ZERO)
	ADD_DEFINE(GL_FALSE)
	ADD_DEFINE(GL_LOGIC_OP)
	ADD_DEFINE(GL_NONE)
	ADD_DEFINE(GL_TEXTURE_COMPONENTS)
	ADD_DEFINE(GL_NO_ERROR)
	ADD_DEFINE(GL_POINTS)
	ADD_DEFINE(GL_CURRENT_BIT)
	ADD_DEFINE(GL_TRUE)
	ADD_DEFINE(GL_ONE)
	ADD_DEFINE(GL_CLIENT_PIXEL_STORE_BIT)
	ADD_DEFINE(GL_LINES)
	ADD_DEFINE(GL_LINE_LOOP)
	ADD_DEFINE(GL_POINT_BIT)
	ADD_DEFINE(GL_CLIENT_VERTEX_ARRAY_BIT)
	ADD_DEFINE(GL_LINE_STRIP)
	ADD_DEFINE(GL_LINE_BIT)
	ADD_DEFINE(GL_TRIANGLES)
	ADD_DEFINE(GL_TRIANGLE_STRIP)
	ADD_DEFINE(GL_TRIANGLE_FAN)
	ADD_DEFINE(GL_QUADS)
	ADD_DEFINE(GL_QUAD_STRIP)
	ADD_DEFINE(GL_POLYGON_BIT)
	ADD_DEFINE(GL_POLYGON)
	ADD_DEFINE(GL_POLYGON_STIPPLE_BIT)
	ADD_DEFINE(GL_PIXEL_MODE_BIT)
	ADD_DEFINE(GL_LIGHTING_BIT)
	ADD_DEFINE(GL_FOG_BIT)
	ADD_DEFINE(GL_DEPTH_BUFFER_BIT)
	ADD_DEFINE(GL_ACCUM)
	ADD_DEFINE(GL_LOAD)
	ADD_DEFINE(GL_RETURN)
	ADD_DEFINE(GL_MULT)
	ADD_DEFINE(GL_ADD)
	ADD_DEFINE(GL_NEVER)
	ADD_DEFINE(GL_ACCUM_BUFFER_BIT)
	ADD_DEFINE(GL_LESS)
	ADD_DEFINE(GL_EQUAL)
	ADD_DEFINE(GL_LEQUAL)
	ADD_DEFINE(GL_GREATER)
	ADD_DEFINE(GL_NOTEQUAL)
	ADD_DEFINE(GL_GEQUAL)
	ADD_DEFINE(GL_ALWAYS)
	ADD_DEFINE(GL_SRC_COLOR)
	ADD_DEFINE(GL_ONE_MINUS_SRC_COLOR)
	ADD_DEFINE(GL_SRC_ALPHA)
	ADD_DEFINE(GL_ONE_MINUS_SRC_ALPHA)
	ADD_DEFINE(GL_DST_ALPHA)
	ADD_DEFINE(GL_ONE_MINUS_DST_ALPHA)
	ADD_DEFINE(GL_DST_COLOR)
	ADD_DEFINE(GL_ONE_MINUS_DST_COLOR)
	ADD_DEFINE(GL_SRC_ALPHA_SATURATE)
	ADD_DEFINE(GL_STENCIL_BUFFER_BIT)
	ADD_DEFINE(GL_FRONT_LEFT)
	ADD_DEFINE(GL_FRONT_RIGHT)
	ADD_DEFINE(GL_BACK_LEFT)
	ADD_DEFINE(GL_BACK_RIGHT)
	ADD_DEFINE(GL_FRONT)
	ADD_DEFINE(GL_BACK)
	ADD_DEFINE(GL_LEFT)
	ADD_DEFINE(GL_RIGHT)
	ADD_DEFINE(GL_FRONT_AND_BACK)
	ADD_DEFINE(GL_AUX0)
	ADD_DEFINE(GL_AUX1)
	ADD_DEFINE(GL_AUX2)
	ADD_DEFINE(GL_AUX3)
	ADD_DEFINE(GL_INVALID_ENUM)
	ADD_DEFINE(GL_INVALID_VALUE)
	ADD_DEFINE(GL_INVALID_OPERATION)
	ADD_DEFINE(GL_STACK_OVERFLOW)
	ADD_DEFINE(GL_STACK_UNDERFLOW)
	ADD_DEFINE(GL_OUT_OF_MEMORY)
	ADD_DEFINE(GL_2D)
	ADD_DEFINE(GL_3D)
	ADD_DEFINE(GL_3D_COLOR)
	ADD_DEFINE(GL_3D_COLOR_TEXTURE)
	ADD_DEFINE(GL_4D_COLOR_TEXTURE)
	ADD_DEFINE(GL_PASS_THROUGH_TOKEN)
	ADD_DEFINE(GL_POINT_TOKEN)
	ADD_DEFINE(GL_LINE_TOKEN)
	ADD_DEFINE(GL_POLYGON_TOKEN)
	ADD_DEFINE(GL_BITMAP_TOKEN)
	ADD_DEFINE(GL_DRAW_PIXEL_TOKEN)
	ADD_DEFINE(GL_COPY_PIXEL_TOKEN)
	ADD_DEFINE(GL_LINE_RESET_TOKEN)
	ADD_DEFINE(GL_EXP)
	ADD_DEFINE(GL_VIEWPORT_BIT)
	ADD_DEFINE(GL_EXP2)
	ADD_DEFINE(GL_CW)
	ADD_DEFINE(GL_CCW)
	ADD_DEFINE(GL_COEFF)
	ADD_DEFINE(GL_ORDER)
	ADD_DEFINE(GL_DOMAIN)
	ADD_DEFINE(GL_CURRENT_COLOR)
	ADD_DEFINE(GL_CURRENT_INDEX)
	ADD_DEFINE(GL_CURRENT_NORMAL)
	ADD_DEFINE(GL_CURRENT_TEXTURE_COORDS)
	ADD_DEFINE(GL_CURRENT_RASTER_COLOR)
	ADD_DEFINE(GL_CURRENT_RASTER_INDEX)
	ADD_DEFINE(GL_CURRENT_RASTER_TEXTURE_COORDS)
	ADD_DEFINE(GL_CURRENT_RASTER_POSITION)
	ADD_DEFINE(GL_CURRENT_RASTER_POSITION_VALID)
	ADD_DEFINE(GL_CURRENT_RASTER_DISTANCE)
	ADD_DEFINE(GL_POINT_SMOOTH)
	ADD_DEFINE(GL_POINT_SIZE)
	ADD_DEFINE(GL_POINT_SIZE_RANGE)
	ADD_DEFINE(GL_POINT_SIZE_GRANULARITY)
	ADD_DEFINE(GL_LINE_SMOOTH)
	ADD_DEFINE(GL_LINE_WIDTH)
	ADD_DEFINE(GL_LINE_WIDTH_RANGE)
	ADD_DEFINE(GL_LINE_WIDTH_GRANULARITY)
	ADD_DEFINE(GL_LINE_STIPPLE)
	ADD_DEFINE(GL_LINE_STIPPLE_PATTERN)
	ADD_DEFINE(GL_LINE_STIPPLE_REPEAT)
	ADD_DEFINE(GL_LIST_MODE)
	ADD_DEFINE(GL_MAX_LIST_NESTING)
	ADD_DEFINE(GL_LIST_BASE)
	ADD_DEFINE(GL_LIST_INDEX)
	ADD_DEFINE(GL_POLYGON_MODE)
	ADD_DEFINE(GL_POLYGON_SMOOTH)
	ADD_DEFINE(GL_POLYGON_STIPPLE)
	ADD_DEFINE(GL_EDGE_FLAG)
	ADD_DEFINE(GL_CULL_FACE)
	ADD_DEFINE(GL_CULL_FACE_MODE)
	ADD_DEFINE(GL_FRONT_FACE)
	ADD_DEFINE(GL_LIGHTING)
	ADD_DEFINE(GL_LIGHT_MODEL_LOCAL_VIEWER)
	ADD_DEFINE(GL_LIGHT_MODEL_TWO_SIDE)
	ADD_DEFINE(GL_LIGHT_MODEL_AMBIENT)
	ADD_DEFINE(GL_SHADE_MODEL)
	ADD_DEFINE(GL_COLOR_MATERIAL_FACE)
	ADD_DEFINE(GL_COLOR_MATERIAL_PARAMETER)
	ADD_DEFINE(GL_COLOR_MATERIAL)
	ADD_DEFINE(GL_FOG)
	ADD_DEFINE(GL_FOG_INDEX)
	ADD_DEFINE(GL_FOG_DENSITY)
	ADD_DEFINE(GL_FOG_START)
	ADD_DEFINE(GL_FOG_END)
	ADD_DEFINE(GL_FOG_MODE)
	ADD_DEFINE(GL_FOG_COLOR)
	ADD_DEFINE(GL_DEPTH_RANGE)
	ADD_DEFINE(GL_DEPTH_TEST)
	ADD_DEFINE(GL_DEPTH_WRITEMASK)
	ADD_DEFINE(GL_DEPTH_CLEAR_VALUE)
	ADD_DEFINE(GL_DEPTH_FUNC)
	ADD_DEFINE(GL_ACCUM_CLEAR_VALUE)
	ADD_DEFINE(GL_STENCIL_TEST)
	ADD_DEFINE(GL_STENCIL_CLEAR_VALUE)
	ADD_DEFINE(GL_STENCIL_FUNC)
	ADD_DEFINE(GL_STENCIL_VALUE_MASK)
	ADD_DEFINE(GL_STENCIL_FAIL)
	ADD_DEFINE(GL_STENCIL_PASS_DEPTH_FAIL)
	ADD_DEFINE(GL_STENCIL_PASS_DEPTH_PASS)
	ADD_DEFINE(GL_STENCIL_REF)
	ADD_DEFINE(GL_STENCIL_WRITEMASK)
	ADD_DEFINE(GL_MATRIX_MODE)
	ADD_DEFINE(GL_NORMALIZE)
	ADD_DEFINE(GL_VIEWPORT)
	ADD_DEFINE(GL_MODELVIEW_STACK_DEPTH)
	ADD_DEFINE(GL_PROJECTION_STACK_DEPTH)
	ADD_DEFINE(GL_TEXTURE_STACK_DEPTH)
	ADD_DEFINE(GL_MODELVIEW_MATRIX)
	ADD_DEFINE(GL_PROJECTION_MATRIX)
	ADD_DEFINE(GL_TEXTURE_MATRIX)
	ADD_DEFINE(GL_ATTRIB_STACK_DEPTH)
	ADD_DEFINE(GL_CLIENT_ATTRIB_STACK_DEPTH)
	ADD_DEFINE(GL_ALPHA_TEST)
	ADD_DEFINE(GL_ALPHA_TEST_FUNC)
	ADD_DEFINE(GL_ALPHA_TEST_REF)
	ADD_DEFINE(GL_DITHER)
	ADD_DEFINE(GL_BLEND_DST)
	ADD_DEFINE(GL_BLEND_SRC)
	ADD_DEFINE(GL_BLEND)
	ADD_DEFINE(GL_LOGIC_OP_MODE)
	ADD_DEFINE(GL_INDEX_LOGIC_OP)
	ADD_DEFINE(GL_COLOR_LOGIC_OP)
	ADD_DEFINE(GL_AUX_BUFFERS)
	ADD_DEFINE(GL_DRAW_BUFFER)
	ADD_DEFINE(GL_READ_BUFFER)
	ADD_DEFINE(GL_SCISSOR_BOX)
	ADD_DEFINE(GL_SCISSOR_TEST)
	ADD_DEFINE(GL_INDEX_CLEAR_VALUE)
	ADD_DEFINE(GL_INDEX_WRITEMASK)
	ADD_DEFINE(GL_COLOR_CLEAR_VALUE)
	ADD_DEFINE(GL_COLOR_WRITEMASK)
	ADD_DEFINE(GL_INDEX_MODE)
	ADD_DEFINE(GL_RGBA_MODE)
	ADD_DEFINE(GL_DOUBLEBUFFER)
	ADD_DEFINE(GL_STEREO)
	ADD_DEFINE(GL_RENDER_MODE)
	ADD_DEFINE(GL_PERSPECTIVE_CORRECTION_HINT)
	ADD_DEFINE(GL_POINT_SMOOTH_HINT)
	ADD_DEFINE(GL_LINE_SMOOTH_HINT)
	ADD_DEFINE(GL_POLYGON_SMOOTH_HINT)
	ADD_DEFINE(GL_FOG_HINT)
	ADD_DEFINE(GL_TEXTURE_GEN_S)
	ADD_DEFINE(GL_TEXTURE_GEN_T)
	ADD_DEFINE(GL_TEXTURE_GEN_R)
	ADD_DEFINE(GL_TEXTURE_GEN_Q)
	ADD_DEFINE(GL_PIXEL_MAP_I_TO_I)
	ADD_DEFINE(GL_PIXEL_MAP_S_TO_S)
	ADD_DEFINE(GL_PIXEL_MAP_I_TO_R)
	ADD_DEFINE(GL_PIXEL_MAP_I_TO_G)
	ADD_DEFINE(GL_PIXEL_MAP_I_TO_B)
	ADD_DEFINE(GL_PIXEL_MAP_I_TO_A)
	ADD_DEFINE(GL_PIXEL_MAP_R_TO_R)
	ADD_DEFINE(GL_PIXEL_MAP_G_TO_G)
	ADD_DEFINE(GL_PIXEL_MAP_B_TO_B)
	ADD_DEFINE(GL_PIXEL_MAP_A_TO_A)
	ADD_DEFINE(GL_PIXEL_MAP_I_TO_I_SIZE)
	ADD_DEFINE(GL_PIXEL_MAP_S_TO_S_SIZE)
	ADD_DEFINE(GL_PIXEL_MAP_I_TO_R_SIZE)
	ADD_DEFINE(GL_PIXEL_MAP_I_TO_G_SIZE)
	ADD_DEFINE(GL_PIXEL_MAP_I_TO_B_SIZE)
	ADD_DEFINE(GL_PIXEL_MAP_I_TO_A_SIZE)
	ADD_DEFINE(GL_PIXEL_MAP_R_TO_R_SIZE)
	ADD_DEFINE(GL_PIXEL_MAP_G_TO_G_SIZE)
	ADD_DEFINE(GL_PIXEL_MAP_B_TO_B_SIZE)
	ADD_DEFINE(GL_PIXEL_MAP_A_TO_A_SIZE)
	ADD_DEFINE(GL_UNPACK_SWAP_BYTES)
	ADD_DEFINE(GL_UNPACK_LSB_FIRST)
	ADD_DEFINE(GL_UNPACK_ROW_LENGTH)
	ADD_DEFINE(GL_UNPACK_SKIP_ROWS)
	ADD_DEFINE(GL_UNPACK_SKIP_PIXELS)
	ADD_DEFINE(GL_UNPACK_ALIGNMENT)
	ADD_DEFINE(GL_PACK_SWAP_BYTES)
	ADD_DEFINE(GL_PACK_LSB_FIRST)
	ADD_DEFINE(GL_PACK_ROW_LENGTH)
	ADD_DEFINE(GL_PACK_SKIP_ROWS)
	ADD_DEFINE(GL_PACK_SKIP_PIXELS)
	ADD_DEFINE(GL_PACK_ALIGNMENT)
	ADD_DEFINE(GL_MAP_COLOR)
	ADD_DEFINE(GL_MAP_STENCIL)
	ADD_DEFINE(GL_INDEX_SHIFT)
	ADD_DEFINE(GL_INDEX_OFFSET)
	ADD_DEFINE(GL_RED_SCALE)
	ADD_DEFINE(GL_RED_BIAS)
	ADD_DEFINE(GL_ZOOM_X)
	ADD_DEFINE(GL_ZOOM_Y)
	ADD_DEFINE(GL_GREEN_SCALE)
	ADD_DEFINE(GL_GREEN_BIAS)
	ADD_DEFINE(GL_BLUE_SCALE)
	ADD_DEFINE(GL_BLUE_BIAS)
	ADD_DEFINE(GL_ALPHA_SCALE)
	ADD_DEFINE(GL_ALPHA_BIAS)
	ADD_DEFINE(GL_DEPTH_SCALE)
	ADD_DEFINE(GL_DEPTH_BIAS)
	ADD_DEFINE(GL_MAX_EVAL_ORDER)
	ADD_DEFINE(GL_MAX_LIGHTS)
	ADD_DEFINE(GL_MAX_CLIP_PLANES)
	ADD_DEFINE(GL_MAX_TEXTURE_SIZE)
	ADD_DEFINE(GL_MAX_PIXEL_MAP_TABLE)
	ADD_DEFINE(GL_MAX_ATTRIB_STACK_DEPTH)
	ADD_DEFINE(GL_MAX_MODELVIEW_STACK_DEPTH)
	ADD_DEFINE(GL_MAX_NAME_STACK_DEPTH)
	ADD_DEFINE(GL_MAX_PROJECTION_STACK_DEPTH)
	ADD_DEFINE(GL_MAX_TEXTURE_STACK_DEPTH)
	ADD_DEFINE(GL_MAX_VIEWPORT_DIMS)
	ADD_DEFINE(GL_MAX_CLIENT_ATTRIB_STACK_DEPTH)
	ADD_DEFINE(GL_SUBPIXEL_BITS)
	ADD_DEFINE(GL_INDEX_BITS)
	ADD_DEFINE(GL_RED_BITS)
	ADD_DEFINE(GL_GREEN_BITS)
	ADD_DEFINE(GL_BLUE_BITS)
	ADD_DEFINE(GL_ALPHA_BITS)
	ADD_DEFINE(GL_DEPTH_BITS)
	ADD_DEFINE(GL_STENCIL_BITS)
	ADD_DEFINE(GL_ACCUM_RED_BITS)
	ADD_DEFINE(GL_ACCUM_GREEN_BITS)
	ADD_DEFINE(GL_ACCUM_BLUE_BITS)
	ADD_DEFINE(GL_ACCUM_ALPHA_BITS)
	ADD_DEFINE(GL_NAME_STACK_DEPTH)
	ADD_DEFINE(GL_AUTO_NORMAL)
	ADD_DEFINE(GL_MAP1_COLOR_4)
	ADD_DEFINE(GL_MAP1_INDEX)
	ADD_DEFINE(GL_MAP1_NORMAL)
	ADD_DEFINE(GL_MAP1_TEXTURE_COORD_1)
	ADD_DEFINE(GL_MAP1_TEXTURE_COORD_2)
	ADD_DEFINE(GL_MAP1_TEXTURE_COORD_3)
	ADD_DEFINE(GL_MAP1_TEXTURE_COORD_4)
	ADD_DEFINE(GL_MAP1_VERTEX_3)
	ADD_DEFINE(GL_MAP1_VERTEX_4)
	ADD_DEFINE(GL_MAP2_COLOR_4)
	ADD_DEFINE(GL_MAP2_INDEX)
	ADD_DEFINE(GL_MAP2_NORMAL)
	ADD_DEFINE(GL_MAP2_TEXTURE_COORD_1)
	ADD_DEFINE(GL_MAP2_TEXTURE_COORD_2)
	ADD_DEFINE(GL_MAP2_TEXTURE_COORD_3)
	ADD_DEFINE(GL_MAP2_TEXTURE_COORD_4)
	ADD_DEFINE(GL_MAP2_VERTEX_3)
	ADD_DEFINE(GL_MAP2_VERTEX_4)
	ADD_DEFINE(GL_MAP1_GRID_DOMAIN)
	ADD_DEFINE(GL_MAP1_GRID_SEGMENTS)
	ADD_DEFINE(GL_MAP2_GRID_DOMAIN)
	ADD_DEFINE(GL_MAP2_GRID_SEGMENTS)
	ADD_DEFINE(GL_TEXTURE_1D)
	ADD_DEFINE(GL_TEXTURE_2D)
	ADD_DEFINE(GL_FEEDBACK_BUFFER_POINTER)
	ADD_DEFINE(GL_FEEDBACK_BUFFER_SIZE)
	ADD_DEFINE(GL_FEEDBACK_BUFFER_TYPE)
	ADD_DEFINE(GL_SELECTION_BUFFER_POINTER)
	ADD_DEFINE(GL_SELECTION_BUFFER_SIZE)
	ADD_DEFINE(GL_TEXTURE_WIDTH)
	ADD_DEFINE(GL_TRANSFORM_BIT)
	ADD_DEFINE(GL_TEXTURE_HEIGHT)
	ADD_DEFINE(GL_TEXTURE_INTERNAL_FORMAT)
	ADD_DEFINE(GL_TEXTURE_BORDER_COLOR)
	ADD_DEFINE(GL_TEXTURE_BORDER)
	ADD_DEFINE(GL_DONT_CARE)
	ADD_DEFINE(GL_FASTEST)
	ADD_DEFINE(GL_NICEST)
	ADD_DEFINE(GL_AMBIENT)
	ADD_DEFINE(GL_DIFFUSE)
	ADD_DEFINE(GL_SPECULAR)
	ADD_DEFINE(GL_POSITION)
	ADD_DEFINE(GL_SPOT_DIRECTION)
	ADD_DEFINE(GL_SPOT_EXPONENT)
	ADD_DEFINE(GL_SPOT_CUTOFF)
	ADD_DEFINE(GL_CONSTANT_ATTENUATION)
	ADD_DEFINE(GL_LINEAR_ATTENUATION)
	ADD_DEFINE(GL_QUADRATIC_ATTENUATION)
	ADD_DEFINE(GL_COMPILE)
	ADD_DEFINE(GL_COMPILE_AND_EXECUTE)
	ADD_DEFINE(GL_BYTE)
	ADD_DEFINE(GL_UNSIGNED_BYTE)
	ADD_DEFINE(GL_SHORT)
	ADD_DEFINE(GL_UNSIGNED_SHORT)
	ADD_DEFINE(GL_INT)
	ADD_DEFINE(GL_UNSIGNED_INT)
	ADD_DEFINE(GL_FLOAT)
	ADD_DEFINE(GL_2_BYTES)
	ADD_DEFINE(GL_3_BYTES)
	ADD_DEFINE(GL_4_BYTES)
	ADD_DEFINE(GL_DOUBLE)
	ADD_DEFINE(GL_CLEAR)
	ADD_DEFINE(GL_AND)
	ADD_DEFINE(GL_AND_REVERSE)
	ADD_DEFINE(GL_COPY)
	ADD_DEFINE(GL_AND_INVERTED)
	ADD_DEFINE(GL_NOOP)
	ADD_DEFINE(GL_XOR)
	ADD_DEFINE(GL_OR)
	ADD_DEFINE(GL_NOR)
	ADD_DEFINE(GL_EQUIV)
	ADD_DEFINE(GL_INVERT)
	ADD_DEFINE(GL_OR_REVERSE)
	ADD_DEFINE(GL_COPY_INVERTED)
	ADD_DEFINE(GL_OR_INVERTED)
	ADD_DEFINE(GL_NAND)
	ADD_DEFINE(GL_SET)
	ADD_DEFINE(GL_EMISSION)
	ADD_DEFINE(GL_SHININESS)
	ADD_DEFINE(GL_AMBIENT_AND_DIFFUSE)
	ADD_DEFINE(GL_COLOR_INDEXES)
	ADD_DEFINE(GL_MODELVIEW)
	ADD_DEFINE(GL_PROJECTION)
	ADD_DEFINE(GL_TEXTURE)
	ADD_DEFINE(GL_COLOR)
	ADD_DEFINE(GL_DEPTH)
	ADD_DEFINE(GL_STENCIL)
	ADD_DEFINE(GL_COLOR_INDEX)
	ADD_DEFINE(GL_STENCIL_INDEX)
	ADD_DEFINE(GL_DEPTH_COMPONENT)
	ADD_DEFINE(GL_RED)
	ADD_DEFINE(GL_GREEN)
	ADD_DEFINE(GL_BLUE)
	ADD_DEFINE(GL_ALPHA)
	ADD_DEFINE(GL_RGB)
	ADD_DEFINE(GL_RGBA)
	ADD_DEFINE(GL_LUMINANCE)
	ADD_DEFINE(GL_LUMINANCE_ALPHA)
	ADD_DEFINE(GL_BITMAP)
	ADD_DEFINE(GL_POINT)
	ADD_DEFINE(GL_LINE)
	ADD_DEFINE(GL_FILL)
	ADD_DEFINE(GL_RENDER)
	ADD_DEFINE(GL_FEEDBACK)
	ADD_DEFINE(GL_SELECT)
	ADD_DEFINE(GL_FLAT)
	ADD_DEFINE(GL_SMOOTH)
	ADD_DEFINE(GL_KEEP)
	ADD_DEFINE(GL_REPLACE)
	ADD_DEFINE(GL_INCR)
	ADD_DEFINE(GL_DECR)
	ADD_DEFINE(GL_VENDOR)
	ADD_DEFINE(GL_RENDERER)
	ADD_DEFINE(GL_VERSION)
	ADD_DEFINE(GL_EXTENSIONS)
	ADD_DEFINE(GL_S)
	ADD_DEFINE(GL_ENABLE_BIT)
	ADD_DEFINE(GL_T)
	ADD_DEFINE(GL_R)
	ADD_DEFINE(GL_Q)
	ADD_DEFINE(GL_MODULATE)
	ADD_DEFINE(GL_DECAL)
	ADD_DEFINE(GL_TEXTURE_ENV_MODE)
	ADD_DEFINE(GL_TEXTURE_ENV_COLOR)
	ADD_DEFINE(GL_TEXTURE_ENV)
	ADD_DEFINE(GL_EYE_LINEAR)
	ADD_DEFINE(GL_OBJECT_LINEAR)
	ADD_DEFINE(GL_SPHERE_MAP)
	ADD_DEFINE(GL_TEXTURE_GEN_MODE)
	ADD_DEFINE(GL_OBJECT_PLANE)
	ADD_DEFINE(GL_EYE_PLANE)
	ADD_DEFINE(GL_NEAREST)
	ADD_DEFINE(GL_LINEAR)
	ADD_DEFINE(GL_NEAREST_MIPMAP_NEAREST)
	ADD_DEFINE(GL_LINEAR_MIPMAP_NEAREST)
	ADD_DEFINE(GL_NEAREST_MIPMAP_LINEAR)
	ADD_DEFINE(GL_LINEAR_MIPMAP_LINEAR)
	ADD_DEFINE(GL_TEXTURE_MAG_FILTER)
	ADD_DEFINE(GL_TEXTURE_MIN_FILTER)
	ADD_DEFINE(GL_TEXTURE_WRAP_S)
	ADD_DEFINE(GL_TEXTURE_WRAP_T)
	ADD_DEFINE(GL_CLAMP)
	ADD_DEFINE(GL_REPEAT)
	ADD_DEFINE(GL_POLYGON_OFFSET_UNITS)
	ADD_DEFINE(GL_POLYGON_OFFSET_POINT)
	ADD_DEFINE(GL_POLYGON_OFFSET_LINE)
	ADD_DEFINE(GL_R3_G3_B2)
	ADD_DEFINE(GL_V2F)
	ADD_DEFINE(GL_V3F)
	ADD_DEFINE(GL_C4UB_V2F)
	ADD_DEFINE(GL_C4UB_V3F)
	ADD_DEFINE(GL_C3F_V3F)
	ADD_DEFINE(GL_N3F_V3F)
	ADD_DEFINE(GL_C4F_N3F_V3F)
	ADD_DEFINE(GL_T2F_V3F)
	ADD_DEFINE(GL_T4F_V4F)
	ADD_DEFINE(GL_T2F_C4UB_V3F)
	ADD_DEFINE(GL_T2F_C3F_V3F)
	ADD_DEFINE(GL_T2F_N3F_V3F)
	ADD_DEFINE(GL_T2F_C4F_N3F_V3F)
	ADD_DEFINE(GL_T4F_C4F_N3F_V4F)
	ADD_DEFINE(GL_CLIP_PLANE0)
	ADD_DEFINE(GL_CLIP_PLANE1)
	ADD_DEFINE(GL_CLIP_PLANE2)
	ADD_DEFINE(GL_CLIP_PLANE3)
	ADD_DEFINE(GL_CLIP_PLANE4)
	ADD_DEFINE(GL_CLIP_PLANE5)
	ADD_DEFINE(GL_LIGHT0)
	ADD_DEFINE(GL_COLOR_BUFFER_BIT)
	ADD_DEFINE(GL_LIGHT1)
	ADD_DEFINE(GL_LIGHT2)
	ADD_DEFINE(GL_LIGHT3)
	ADD_DEFINE(GL_LIGHT4)
	ADD_DEFINE(GL_LIGHT5)
	ADD_DEFINE(GL_LIGHT6)
	ADD_DEFINE(GL_LIGHT7)
	ADD_DEFINE(GL_HINT_BIT)
	ADD_DEFINE(GL_POLYGON_OFFSET_FILL)
	ADD_DEFINE(GL_POLYGON_OFFSET_FACTOR)
	ADD_DEFINE(GL_ALPHA4)
	ADD_DEFINE(GL_ALPHA8)
	ADD_DEFINE(GL_ALPHA12)
	ADD_DEFINE(GL_ALPHA16)
	ADD_DEFINE(GL_LUMINANCE4)
	ADD_DEFINE(GL_LUMINANCE8)
	ADD_DEFINE(GL_LUMINANCE12)
	ADD_DEFINE(GL_LUMINANCE16)
	ADD_DEFINE(GL_LUMINANCE4_ALPHA4)
	ADD_DEFINE(GL_LUMINANCE6_ALPHA2)
	ADD_DEFINE(GL_LUMINANCE8_ALPHA8)
	ADD_DEFINE(GL_LUMINANCE12_ALPHA4)
	ADD_DEFINE(GL_LUMINANCE12_ALPHA12)
	ADD_DEFINE(GL_LUMINANCE16_ALPHA16)
	ADD_DEFINE(GL_INTENSITY)
	ADD_DEFINE(GL_INTENSITY4)
	ADD_DEFINE(GL_INTENSITY8)
	ADD_DEFINE(GL_INTENSITY12)
	ADD_DEFINE(GL_INTENSITY16)
	ADD_DEFINE(GL_RGB4)
	ADD_DEFINE(GL_RGB5)
	ADD_DEFINE(GL_RGB8)
	ADD_DEFINE(GL_RGB10)
	ADD_DEFINE(GL_RGB12)
	ADD_DEFINE(GL_RGB16)
	ADD_DEFINE(GL_RGBA2)
	ADD_DEFINE(GL_RGBA4)
	ADD_DEFINE(GL_RGB5_A1)
	ADD_DEFINE(GL_RGBA8)
	ADD_DEFINE(GL_RGB10_A2)
	ADD_DEFINE(GL_RGBA12)
	ADD_DEFINE(GL_RGBA16)
	ADD_DEFINE(GL_TEXTURE_RED_SIZE)
	ADD_DEFINE(GL_TEXTURE_GREEN_SIZE)
	ADD_DEFINE(GL_TEXTURE_BLUE_SIZE)
	ADD_DEFINE(GL_TEXTURE_ALPHA_SIZE)
	ADD_DEFINE(GL_TEXTURE_LUMINANCE_SIZE)
	ADD_DEFINE(GL_TEXTURE_INTENSITY_SIZE)
	ADD_DEFINE(GL_PROXY_TEXTURE_1D)
	ADD_DEFINE(GL_PROXY_TEXTURE_2D)
	ADD_DEFINE(GL_TEXTURE_PRIORITY)
	ADD_DEFINE(GL_TEXTURE_RESIDENT)
	ADD_DEFINE(GL_TEXTURE_BINDING_1D)
	ADD_DEFINE(GL_TEXTURE_BINDING_2D)
	ADD_DEFINE(GL_VERTEX_ARRAY)
	ADD_DEFINE(GL_NORMAL_ARRAY)
	ADD_DEFINE(GL_COLOR_ARRAY)
	ADD_DEFINE(GL_INDEX_ARRAY)
	ADD_DEFINE(GL_TEXTURE_COORD_ARRAY)
	ADD_DEFINE(GL_EDGE_FLAG_ARRAY)
	ADD_DEFINE(GL_VERTEX_ARRAY_SIZE)
	ADD_DEFINE(GL_VERTEX_ARRAY_TYPE)
	ADD_DEFINE(GL_VERTEX_ARRAY_STRIDE)
	ADD_DEFINE(GL_NORMAL_ARRAY_TYPE)
	ADD_DEFINE(GL_NORMAL_ARRAY_STRIDE)
	ADD_DEFINE(GL_COLOR_ARRAY_SIZE)
	ADD_DEFINE(GL_COLOR_ARRAY_TYPE)
	ADD_DEFINE(GL_COLOR_ARRAY_STRIDE)
	ADD_DEFINE(GL_INDEX_ARRAY_TYPE)
	ADD_DEFINE(GL_INDEX_ARRAY_STRIDE)
	ADD_DEFINE(GL_TEXTURE_COORD_ARRAY_SIZE)
	ADD_DEFINE(GL_TEXTURE_COORD_ARRAY_TYPE)
	ADD_DEFINE(GL_TEXTURE_COORD_ARRAY_STRIDE)
	ADD_DEFINE(GL_EDGE_FLAG_ARRAY_STRIDE)
	ADD_DEFINE(GL_VERTEX_ARRAY_POINTER)
	ADD_DEFINE(GL_NORMAL_ARRAY_POINTER)
	ADD_DEFINE(GL_COLOR_ARRAY_POINTER)
	ADD_DEFINE(GL_INDEX_ARRAY_POINTER)
	ADD_DEFINE(GL_TEXTURE_COORD_ARRAY_POINTER)
	ADD_DEFINE(GL_EDGE_FLAG_ARRAY_POINTER)
	ADD_DEFINE(GL_COLOR_INDEX1_EXT)
	ADD_DEFINE(GL_COLOR_INDEX2_EXT)
	ADD_DEFINE(GL_COLOR_INDEX4_EXT)
	ADD_DEFINE(GL_COLOR_INDEX8_EXT)
	ADD_DEFINE(GL_COLOR_INDEX12_EXT)
	ADD_DEFINE(GL_COLOR_INDEX16_EXT)
	ADD_DEFINE(GL_EVAL_BIT)
	ADD_DEFINE(GL_LIST_BIT)
	ADD_DEFINE(GL_TEXTURE_BIT)
	ADD_DEFINE(GL_SCISSOR_BIT)
	ADD_DEFINE(GL_ALL_ATTRIB_BITS)
	ADD_DEFINE(GL_CLIENT_ALL_ATTRIB_BITS)
	ADD_DEFINE(GL_SMOOTH_POINT_SIZE_RANGE)
	ADD_DEFINE(GL_SMOOTH_POINT_SIZE_GRANULARITY)
	ADD_DEFINE(GL_SMOOTH_LINE_WIDTH_RANGE)
	ADD_DEFINE(GL_SMOOTH_LINE_WIDTH_GRANULARITY)
	ADD_DEFINE(GL_UNSIGNED_BYTE_3_3_2)
	ADD_DEFINE(GL_UNSIGNED_SHORT_4_4_4_4)
	ADD_DEFINE(GL_UNSIGNED_SHORT_5_5_5_1)
	ADD_DEFINE(GL_UNSIGNED_INT_8_8_8_8)
	ADD_DEFINE(GL_UNSIGNED_INT_10_10_10_2)
	ADD_DEFINE(GL_RESCALE_NORMAL)
	ADD_DEFINE(GL_TEXTURE_BINDING_3D)
	ADD_DEFINE(GL_PACK_SKIP_IMAGES)
	ADD_DEFINE(GL_PACK_IMAGE_HEIGHT)
	ADD_DEFINE(GL_UNPACK_SKIP_IMAGES)
	ADD_DEFINE(GL_UNPACK_IMAGE_HEIGHT)
	ADD_DEFINE(GL_TEXTURE_3D)
	ADD_DEFINE(GL_PROXY_TEXTURE_3D)
	ADD_DEFINE(GL_TEXTURE_DEPTH)
	ADD_DEFINE(GL_TEXTURE_WRAP_R)
	ADD_DEFINE(GL_MAX_3D_TEXTURE_SIZE)
	ADD_DEFINE(GL_BGR)
	ADD_DEFINE(GL_BGRA)
	ADD_DEFINE(GL_MAX_ELEMENTS_VERTICES)
	ADD_DEFINE(GL_MAX_ELEMENTS_INDICES)
	ADD_DEFINE(GL_CLAMP_TO_EDGE)
	ADD_DEFINE(GL_TEXTURE_MIN_LOD)
	ADD_DEFINE(GL_TEXTURE_MAX_LOD)
	ADD_DEFINE(GL_TEXTURE_BASE_LEVEL)
	ADD_DEFINE(GL_TEXTURE_MAX_LEVEL)
	ADD_DEFINE(GL_LIGHT_MODEL_COLOR_CONTROL)
	ADD_DEFINE(GL_SINGLE_COLOR)
	ADD_DEFINE(GL_SEPARATE_SPECULAR_COLOR)
	ADD_DEFINE(GL_UNSIGNED_BYTE_2_3_3_REV)
	ADD_DEFINE(GL_UNSIGNED_SHORT_5_6_5)
	ADD_DEFINE(GL_UNSIGNED_SHORT_5_6_5_REV)
	ADD_DEFINE(GL_UNSIGNED_SHORT_4_4_4_4_REV)
	ADD_DEFINE(GL_UNSIGNED_SHORT_1_5_5_5_REV)
	ADD_DEFINE(GL_UNSIGNED_INT_8_8_8_8_REV)
	ADD_DEFINE(GL_ALIASED_POINT_SIZE_RANGE)
	ADD_DEFINE(GL_ALIASED_LINE_WIDTH_RANGE)
	ADD_DEFINE(GL_MULTISAMPLE)
	ADD_DEFINE(GL_SAMPLE_ALPHA_TO_COVERAGE)
	ADD_DEFINE(GL_SAMPLE_ALPHA_TO_ONE)
	ADD_DEFINE(GL_SAMPLE_COVERAGE)
	ADD_DEFINE(GL_SAMPLE_BUFFERS)
	ADD_DEFINE(GL_SAMPLES)
	ADD_DEFINE(GL_SAMPLE_COVERAGE_VALUE)
	ADD_DEFINE(GL_SAMPLE_COVERAGE_INVERT)
	ADD_DEFINE(GL_CLAMP_TO_BORDER)
	ADD_DEFINE(GL_TEXTURE0)
	ADD_DEFINE(GL_TEXTURE1)
	ADD_DEFINE(GL_TEXTURE2)
	ADD_DEFINE(GL_TEXTURE3)
	ADD_DEFINE(GL_TEXTURE4)
	ADD_DEFINE(GL_TEXTURE5)
	ADD_DEFINE(GL_TEXTURE6)
	ADD_DEFINE(GL_TEXTURE7)
	ADD_DEFINE(GL_TEXTURE8)
	ADD_DEFINE(GL_TEXTURE9)
	ADD_DEFINE(GL_TEXTURE10)
	ADD_DEFINE(GL_TEXTURE11)
	ADD_DEFINE(GL_TEXTURE12)
	ADD_DEFINE(GL_TEXTURE13)
	ADD_DEFINE(GL_TEXTURE14)
	ADD_DEFINE(GL_TEXTURE15)
	ADD_DEFINE(GL_TEXTURE16)
	ADD_DEFINE(GL_TEXTURE17)
	ADD_DEFINE(GL_TEXTURE18)
	ADD_DEFINE(GL_TEXTURE19)
	ADD_DEFINE(GL_TEXTURE20)
	ADD_DEFINE(GL_TEXTURE21)
	ADD_DEFINE(GL_TEXTURE22)
	ADD_DEFINE(GL_TEXTURE23)
	ADD_DEFINE(GL_TEXTURE24)
	ADD_DEFINE(GL_TEXTURE25)
	ADD_DEFINE(GL_TEXTURE26)
	ADD_DEFINE(GL_TEXTURE27)
	ADD_DEFINE(GL_TEXTURE28)
	ADD_DEFINE(GL_TEXTURE29)
	ADD_DEFINE(GL_TEXTURE30)
	ADD_DEFINE(GL_TEXTURE31)
	ADD_DEFINE(GL_ACTIVE_TEXTURE)
	ADD_DEFINE(GL_CLIENT_ACTIVE_TEXTURE)
	ADD_DEFINE(GL_MAX_TEXTURE_UNITS)
	ADD_DEFINE(GL_TRANSPOSE_MODELVIEW_MATRIX)
	ADD_DEFINE(GL_TRANSPOSE_PROJECTION_MATRIX)
	ADD_DEFINE(GL_TRANSPOSE_TEXTURE_MATRIX)
	ADD_DEFINE(GL_TRANSPOSE_COLOR_MATRIX)
	ADD_DEFINE(GL_SUBTRACT)
	ADD_DEFINE(GL_COMPRESSED_ALPHA)
	ADD_DEFINE(GL_COMPRESSED_LUMINANCE)
	ADD_DEFINE(GL_COMPRESSED_LUMINANCE_ALPHA)
	ADD_DEFINE(GL_COMPRESSED_INTENSITY)
	ADD_DEFINE(GL_COMPRESSED_RGB)
	ADD_DEFINE(GL_COMPRESSED_RGBA)
	ADD_DEFINE(GL_TEXTURE_COMPRESSION_HINT)
	ADD_DEFINE(GL_NORMAL_MAP)
	ADD_DEFINE(GL_REFLECTION_MAP)
	ADD_DEFINE(GL_TEXTURE_CUBE_MAP)
	ADD_DEFINE(GL_TEXTURE_BINDING_CUBE_MAP)
	ADD_DEFINE(GL_TEXTURE_CUBE_MAP_POSITIVE_X)
	ADD_DEFINE(GL_TEXTURE_CUBE_MAP_NEGATIVE_X)
	ADD_DEFINE(GL_TEXTURE_CUBE_MAP_POSITIVE_Y)
	ADD_DEFINE(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y)
	ADD_DEFINE(GL_TEXTURE_CUBE_MAP_POSITIVE_Z)
	ADD_DEFINE(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z)
	ADD_DEFINE(GL_PROXY_TEXTURE_CUBE_MAP)
	ADD_DEFINE(GL_MAX_CUBE_MAP_TEXTURE_SIZE)
	ADD_DEFINE(GL_COMBINE)
	ADD_DEFINE(GL_COMBINE_RGB)
	ADD_DEFINE(GL_COMBINE_ALPHA)
	ADD_DEFINE(GL_RGB_SCALE)
	ADD_DEFINE(GL_ADD_SIGNED)
	ADD_DEFINE(GL_INTERPOLATE)
	ADD_DEFINE(GL_CONSTANT)
	ADD_DEFINE(GL_PRIMARY_COLOR)
	ADD_DEFINE(GL_PREVIOUS)
	ADD_DEFINE(GL_SOURCE0_RGB)
	ADD_DEFINE(GL_SOURCE1_RGB)
	ADD_DEFINE(GL_SOURCE2_RGB)
	ADD_DEFINE(GL_SOURCE0_ALPHA)
	ADD_DEFINE(GL_SOURCE1_ALPHA)
	ADD_DEFINE(GL_SOURCE2_ALPHA)
	ADD_DEFINE(GL_OPERAND0_RGB)
	ADD_DEFINE(GL_OPERAND1_RGB)
	ADD_DEFINE(GL_OPERAND2_RGB)
	ADD_DEFINE(GL_OPERAND0_ALPHA)
	ADD_DEFINE(GL_OPERAND1_ALPHA)
	ADD_DEFINE(GL_OPERAND2_ALPHA)
	ADD_DEFINE(GL_TEXTURE_COMPRESSED_IMAGE_SIZE)
	ADD_DEFINE(GL_TEXTURE_COMPRESSED)
	ADD_DEFINE(GL_NUM_COMPRESSED_TEXTURE_FORMATS)
	ADD_DEFINE(GL_COMPRESSED_TEXTURE_FORMATS)
	ADD_DEFINE(GL_DOT3_RGB)
	ADD_DEFINE(GL_DOT3_RGBA)
	ADD_DEFINE(GL_MULTISAMPLE_BIT)
	ADD_DEFINE(GL_FOG_COORD_SRC)
	ADD_DEFINE(GL_FOG_COORD)
	ADD_DEFINE(GL_FOG_COORD_ARRAY)
	ADD_DEFINE(GL_SRC0_RGB)
	ADD_DEFINE(GL_FOG_COORD_ARRAY_POINTER)
	ADD_DEFINE(GL_FOG_COORD_ARRAY_TYPE)
	ADD_DEFINE(GL_SRC1_ALPHA)
	ADD_DEFINE(GL_CURRENT_FOG_COORD)
	ADD_DEFINE(GL_FOG_COORD_ARRAY_STRIDE)
	ADD_DEFINE(GL_SRC0_ALPHA)
	ADD_DEFINE(GL_SRC1_RGB)
	ADD_DEFINE(GL_FOG_COORD_ARRAY_BUFFER_BINDING)
	ADD_DEFINE(GL_SRC2_ALPHA)
	ADD_DEFINE(GL_SRC2_RGB)
	ADD_DEFINE(GL_BUFFER_SIZE)
	ADD_DEFINE(GL_BUFFER_USAGE)
	ADD_DEFINE(GL_QUERY_COUNTER_BITS)
	ADD_DEFINE(GL_CURRENT_QUERY)
	ADD_DEFINE(GL_QUERY_RESULT)
	ADD_DEFINE(GL_QUERY_RESULT_AVAILABLE)
	ADD_DEFINE(GL_ARRAY_BUFFER)
	ADD_DEFINE(GL_ELEMENT_ARRAY_BUFFER)
	ADD_DEFINE(GL_ARRAY_BUFFER_BINDING)
	ADD_DEFINE(GL_ELEMENT_ARRAY_BUFFER_BINDING)
	ADD_DEFINE(GL_VERTEX_ARRAY_BUFFER_BINDING)
	ADD_DEFINE(GL_NORMAL_ARRAY_BUFFER_BINDING)
	ADD_DEFINE(GL_COLOR_ARRAY_BUFFER_BINDING)
	ADD_DEFINE(GL_INDEX_ARRAY_BUFFER_BINDING)
	ADD_DEFINE(GL_TEXTURE_COORD_ARRAY_BUFFER_BINDING)
	ADD_DEFINE(GL_EDGE_FLAG_ARRAY_BUFFER_BINDING)
	ADD_DEFINE(GL_SECONDARY_COLOR_ARRAY_BUFFER_BINDING)
	ADD_DEFINE(GL_FOG_COORDINATE_ARRAY_BUFFER_BINDING)
	ADD_DEFINE(GL_WEIGHT_ARRAY_BUFFER_BINDING)
	ADD_DEFINE(GL_VERTEX_ATTRIB_ARRAY_BUFFER_BINDING)
	ADD_DEFINE(GL_READ_ONLY)
	ADD_DEFINE(GL_WRITE_ONLY)
	ADD_DEFINE(GL_READ_WRITE)
	ADD_DEFINE(GL_BUFFER_ACCESS)
	ADD_DEFINE(GL_BUFFER_MAPPED)
	ADD_DEFINE(GL_BUFFER_MAP_POINTER)
	ADD_DEFINE(GL_STREAM_DRAW)
	ADD_DEFINE(GL_STREAM_READ)
	ADD_DEFINE(GL_STREAM_COPY)
	ADD_DEFINE(GL_STATIC_DRAW)
	ADD_DEFINE(GL_STATIC_READ)
	ADD_DEFINE(GL_STATIC_COPY)
	ADD_DEFINE(GL_DYNAMIC_DRAW)
	ADD_DEFINE(GL_DYNAMIC_READ)
	ADD_DEFINE(GL_DYNAMIC_COPY)
	ADD_DEFINE(GL_SAMPLES_PASSED)
	ADD_DEFINE(GL_BLEND_EQUATION_RGB)
	ADD_DEFINE(GL_VERTEX_ATTRIB_ARRAY_ENABLED)
	ADD_DEFINE(GL_VERTEX_ATTRIB_ARRAY_SIZE)
	ADD_DEFINE(GL_VERTEX_ATTRIB_ARRAY_STRIDE)
	ADD_DEFINE(GL_VERTEX_ATTRIB_ARRAY_TYPE)
	ADD_DEFINE(GL_CURRENT_VERTEX_ATTRIB)
	ADD_DEFINE(GL_VERTEX_PROGRAM_POINT_SIZE)
	ADD_DEFINE(GL_VERTEX_PROGRAM_TWO_SIDE)
	ADD_DEFINE(GL_VERTEX_ATTRIB_ARRAY_POINTER)
	ADD_DEFINE(GL_STENCIL_BACK_FUNC)
	ADD_DEFINE(GL_STENCIL_BACK_FAIL)
	ADD_DEFINE(GL_STENCIL_BACK_PASS_DEPTH_FAIL)
	ADD_DEFINE(GL_STENCIL_BACK_PASS_DEPTH_PASS)
	ADD_DEFINE(GL_MAX_DRAW_BUFFERS)
	ADD_DEFINE(GL_DRAW_BUFFER0)
	ADD_DEFINE(GL_DRAW_BUFFER1)
	ADD_DEFINE(GL_DRAW_BUFFER2)
	ADD_DEFINE(GL_DRAW_BUFFER3)
	ADD_DEFINE(GL_DRAW_BUFFER4)
	ADD_DEFINE(GL_DRAW_BUFFER5)
	ADD_DEFINE(GL_DRAW_BUFFER6)
	ADD_DEFINE(GL_DRAW_BUFFER7)
	ADD_DEFINE(GL_DRAW_BUFFER8)
	ADD_DEFINE(GL_DRAW_BUFFER9)
	ADD_DEFINE(GL_DRAW_BUFFER10)
	ADD_DEFINE(GL_DRAW_BUFFER11)
	ADD_DEFINE(GL_DRAW_BUFFER12)
	ADD_DEFINE(GL_DRAW_BUFFER13)
	ADD_DEFINE(GL_DRAW_BUFFER14)
	ADD_DEFINE(GL_DRAW_BUFFER15)
	ADD_DEFINE(GL_BLEND_EQUATION_ALPHA)
	ADD_DEFINE(GL_POINT_SPRITE)
	ADD_DEFINE(GL_COORD_REPLACE)
	ADD_DEFINE(GL_MAX_VERTEX_ATTRIBS)
	ADD_DEFINE(GL_VERTEX_ATTRIB_ARRAY_NORMALIZED)
	ADD_DEFINE(GL_MAX_TEXTURE_COORDS)
	ADD_DEFINE(GL_MAX_TEXTURE_IMAGE_UNITS)
	ADD_DEFINE(GL_FRAGMENT_SHADER)
	ADD_DEFINE(GL_VERTEX_SHADER)
	ADD_DEFINE(GL_MAX_FRAGMENT_UNIFORM_COMPONENTS)
	ADD_DEFINE(GL_MAX_VERTEX_UNIFORM_COMPONENTS)
	ADD_DEFINE(GL_MAX_VARYING_FLOATS)
	ADD_DEFINE(GL_MAX_VERTEX_TEXTURE_IMAGE_UNITS)
	ADD_DEFINE(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS)
	ADD_DEFINE(GL_SHADER_TYPE)
	ADD_DEFINE(GL_FLOAT_VEC2)
	ADD_DEFINE(GL_FLOAT_VEC3)
	ADD_DEFINE(GL_FLOAT_VEC4)
	ADD_DEFINE(GL_INT_VEC2)
	ADD_DEFINE(GL_INT_VEC3)
	ADD_DEFINE(GL_INT_VEC4)
	ADD_DEFINE(GL_BOOL)
	ADD_DEFINE(GL_BOOL_VEC2)
	ADD_DEFINE(GL_BOOL_VEC3)
	ADD_DEFINE(GL_BOOL_VEC4)
	ADD_DEFINE(GL_FLOAT_MAT2)
	ADD_DEFINE(GL_FLOAT_MAT3)
	ADD_DEFINE(GL_FLOAT_MAT4)
	ADD_DEFINE(GL_SAMPLER_1D)
	ADD_DEFINE(GL_SAMPLER_2D)
	ADD_DEFINE(GL_SAMPLER_3D)
	ADD_DEFINE(GL_SAMPLER_CUBE)
	ADD_DEFINE(GL_SAMPLER_1D_SHADOW)
	ADD_DEFINE(GL_SAMPLER_2D_SHADOW)
	ADD_DEFINE(GL_DELETE_STATUS)
	ADD_DEFINE(GL_COMPILE_STATUS)
	ADD_DEFINE(GL_LINK_STATUS)
	ADD_DEFINE(GL_VALIDATE_STATUS)
	ADD_DEFINE(GL_INFO_LOG_LENGTH)
	ADD_DEFINE(GL_ATTACHED_SHADERS)
	ADD_DEFINE(GL_ACTIVE_UNIFORMS)
	ADD_DEFINE(GL_ACTIVE_UNIFORM_MAX_LENGTH)
	ADD_DEFINE(GL_SHADER_SOURCE_LENGTH)
	ADD_DEFINE(GL_ACTIVE_ATTRIBUTES)
	ADD_DEFINE(GL_ACTIVE_ATTRIBUTE_MAX_LENGTH)
	ADD_DEFINE(GL_FRAGMENT_SHADER_DERIVATIVE_HINT)
	ADD_DEFINE(GL_SHADING_LANGUAGE_VERSION)
	ADD_DEFINE(GL_CURRENT_PROGRAM)
	ADD_DEFINE(GL_POINT_SPRITE_COORD_ORIGIN)
	ADD_DEFINE(GL_LOWER_LEFT)
	ADD_DEFINE(GL_UPPER_LEFT)
	ADD_DEFINE(GL_STENCIL_BACK_REF)
	ADD_DEFINE(GL_STENCIL_BACK_VALUE_MASK)
	ADD_DEFINE(GL_STENCIL_BACK_WRITEMASK)
	ADD_DEFINE(GL_CURRENT_RASTER_SECONDARY_COLOR)
	ADD_DEFINE(GL_PIXEL_PACK_BUFFER)
	ADD_DEFINE(GL_PIXEL_UNPACK_BUFFER)
	ADD_DEFINE(GL_PIXEL_PACK_BUFFER_BINDING)
	ADD_DEFINE(GL_PIXEL_UNPACK_BUFFER_BINDING)
	ADD_DEFINE(GL_FLOAT_MAT2x3)
	ADD_DEFINE(GL_FLOAT_MAT2x4)
	ADD_DEFINE(GL_FLOAT_MAT3x2)
	ADD_DEFINE(GL_FLOAT_MAT3x4)
	ADD_DEFINE(GL_FLOAT_MAT4x2)
	ADD_DEFINE(GL_FLOAT_MAT4x3)
	ADD_DEFINE(GL_SRGB)
	ADD_DEFINE(GL_SRGB8)
	ADD_DEFINE(GL_SRGB_ALPHA)
	ADD_DEFINE(GL_SRGB8_ALPHA8)
	ADD_DEFINE(GL_SLUMINANCE_ALPHA)
	ADD_DEFINE(GL_SLUMINANCE8_ALPHA8)
	ADD_DEFINE(GL_SLUMINANCE)
	ADD_DEFINE(GL_SLUMINANCE8)
	ADD_DEFINE(GL_COMPRESSED_SRGB)
	ADD_DEFINE(GL_COMPRESSED_SRGB_ALPHA)
	ADD_DEFINE(GL_COMPRESSED_SLUMINANCE)
	ADD_DEFINE(GL_COMPRESSED_SLUMINANCE_ALPHA)
	
#ifndef GLEW_NO_GLU
	ADD_DEFINE(GLU_FALSE)
	ADD_DEFINE(GLU_TRUE)
	ADD_DEFINE(GLU_VERSION_1_1)
	ADD_DEFINE(GLU_VERSION_1_2)
	ADD_DEFINE(GLU_VERSION)
	ADD_DEFINE(GLU_EXTENSIONS)
	ADD_DEFINE(GLU_INVALID_ENUM)
	ADD_DEFINE(GLU_INVALID_VALUE)
	ADD_DEFINE(GLU_OUT_OF_MEMORY)
	ADD_DEFINE(GLU_INVALID_OPERATION)
	ADD_DEFINE(GLU_OUTLINE_POLYGON)
	ADD_DEFINE(GLU_OUTLINE_PATCH)
	ADD_DEFINE(GLU_NURBS_ERROR1)
	ADD_DEFINE(GLU_NURBS_ERROR2)
	ADD_DEFINE(GLU_NURBS_ERROR3)
	ADD_DEFINE(GLU_NURBS_ERROR4)
	ADD_DEFINE(GLU_NURBS_ERROR5)
	ADD_DEFINE(GLU_NURBS_ERROR6)
	ADD_DEFINE(GLU_NURBS_ERROR7)
	ADD_DEFINE(GLU_NURBS_ERROR8)
	ADD_DEFINE(GLU_NURBS_ERROR9)
	ADD_DEFINE(GLU_NURBS_ERROR10)
	ADD_DEFINE(GLU_NURBS_ERROR11)
	ADD_DEFINE(GLU_NURBS_ERROR12)
	ADD_DEFINE(GLU_NURBS_ERROR13)
	ADD_DEFINE(GLU_NURBS_ERROR14)
	ADD_DEFINE(GLU_NURBS_ERROR15)
	ADD_DEFINE(GLU_NURBS_ERROR16)
	ADD_DEFINE(GLU_NURBS_ERROR17)
	ADD_DEFINE(GLU_NURBS_ERROR18)
	ADD_DEFINE(GLU_NURBS_ERROR19)
	ADD_DEFINE(GLU_NURBS_ERROR20)
	ADD_DEFINE(GLU_NURBS_ERROR21)
	ADD_DEFINE(GLU_NURBS_ERROR22)
	ADD_DEFINE(GLU_NURBS_ERROR23)
	ADD_DEFINE(GLU_NURBS_ERROR24)
	ADD_DEFINE(GLU_NURBS_ERROR25)
	ADD_DEFINE(GLU_NURBS_ERROR26)
	ADD_DEFINE(GLU_NURBS_ERROR27)
	ADD_DEFINE(GLU_NURBS_ERROR28)
	ADD_DEFINE(GLU_NURBS_ERROR29)
	ADD_DEFINE(GLU_NURBS_ERROR30)
	ADD_DEFINE(GLU_NURBS_ERROR31)
	ADD_DEFINE(GLU_NURBS_ERROR32)
	ADD_DEFINE(GLU_NURBS_ERROR33)
	ADD_DEFINE(GLU_NURBS_ERROR34)
	ADD_DEFINE(GLU_NURBS_ERROR35)
	ADD_DEFINE(GLU_NURBS_ERROR36)
	ADD_DEFINE(GLU_NURBS_ERROR37)
	ADD_DEFINE(GLU_AUTO_LOAD_MATRIX)
	ADD_DEFINE(GLU_CULLING)
	ADD_DEFINE(GLU_SAMPLING_TOLERANCE)
	ADD_DEFINE(GLU_DISPLAY_MODE)
	ADD_DEFINE(GLU_PARAMETRIC_TOLERANCE)
	ADD_DEFINE(GLU_SAMPLING_METHOD)
	ADD_DEFINE(GLU_U_STEP)
	ADD_DEFINE(GLU_V_STEP)
	ADD_DEFINE(GLU_PATH_LENGTH)
	ADD_DEFINE(GLU_PARAMETRIC_ERROR)
	ADD_DEFINE(GLU_DOMAIN_DISTANCE)
	ADD_DEFINE(GLU_MAP1_TRIM_2)
	ADD_DEFINE(GLU_MAP1_TRIM_3)
	ADD_DEFINE(GLU_POINT)
	ADD_DEFINE(GLU_LINE)
	ADD_DEFINE(GLU_FILL)
	ADD_DEFINE(GLU_SILHOUETTE)
	ADD_DEFINE(GLU_ERROR)
	ADD_DEFINE(GLU_SMOOTH)
	ADD_DEFINE(GLU_FLAT)
	ADD_DEFINE(GLU_NONE)
	ADD_DEFINE(GLU_OUTSIDE)
	ADD_DEFINE(GLU_INSIDE)
	ADD_DEFINE(GLU_TESS_BEGIN)
	ADD_DEFINE(GLU_BEGIN)
	ADD_DEFINE(GLU_TESS_VERTEX)
	ADD_DEFINE(GLU_VERTEX)
	ADD_DEFINE(GLU_TESS_END)
	ADD_DEFINE(GLU_END)
	ADD_DEFINE(GLU_TESS_ERROR)
	ADD_DEFINE(GLU_TESS_EDGE_FLAG)
	ADD_DEFINE(GLU_EDGE_FLAG)
	ADD_DEFINE(GLU_TESS_COMBINE)
	ADD_DEFINE(GLU_TESS_BEGIN_DATA)
	ADD_DEFINE(GLU_TESS_VERTEX_DATA)
	ADD_DEFINE(GLU_TESS_END_DATA)
	ADD_DEFINE(GLU_TESS_ERROR_DATA)
	ADD_DEFINE(GLU_TESS_EDGE_FLAG_DATA)
	ADD_DEFINE(GLU_TESS_COMBINE_DATA)
	ADD_DEFINE(GLU_CW)
	ADD_DEFINE(GLU_CCW)
	ADD_DEFINE(GLU_INTERIOR)
	ADD_DEFINE(GLU_EXTERIOR)
	ADD_DEFINE(GLU_UNKNOWN)
	ADD_DEFINE(GLU_TESS_WINDING_RULE)
	ADD_DEFINE(GLU_TESS_BOUNDARY_ONLY)
	ADD_DEFINE(GLU_TESS_TOLERANCE)
	ADD_DEFINE(GLU_TESS_ERROR1)
	ADD_DEFINE(GLU_TESS_ERROR2)
	ADD_DEFINE(GLU_TESS_ERROR3)
	ADD_DEFINE(GLU_TESS_ERROR4)
	ADD_DEFINE(GLU_TESS_ERROR5)
	ADD_DEFINE(GLU_TESS_ERROR6)
	ADD_DEFINE(GLU_TESS_ERROR7)
	ADD_DEFINE(GLU_TESS_ERROR8)
	ADD_DEFINE(GLU_TESS_MISSING_BEGIN_POLYGON)
	ADD_DEFINE(GLU_TESS_MISSING_BEGIN_CONTOUR)
	ADD_DEFINE(GLU_TESS_MISSING_END_POLYGON)
	ADD_DEFINE(GLU_TESS_MISSING_END_CONTOUR)
	ADD_DEFINE(GLU_TESS_COORD_TOO_LARGE)
	ADD_DEFINE(GLU_TESS_NEED_COMBINE_CALLBACK)
	ADD_DEFINE(GLU_TESS_WINDING_ODD)
	ADD_DEFINE(GLU_TESS_WINDING_NONZERO)
	ADD_DEFINE(GLU_TESS_WINDING_POSITIVE)
	ADD_DEFINE(GLU_TESS_WINDING_NEGATIVE)
	ADD_DEFINE(GLU_TESS_WINDING_ABS_GEQ_TWO)
	ADD_DEFINE_DOUBLE(GLU_TESS_MAX_COORD)
#endif /* GLEW_NO_GLU */
	
	return 1;
}
