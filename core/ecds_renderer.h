/*****************************************************************************/
/*	ecds_renderer.h: Interface definition for ECDS geometry renderer		 */
/*																			 */
/*  The geometry renderer is abstracted to allow for implementation of it on */
/*  various platforms and to enable the transmission of rendering commands	 */
/*  over the message bus so networked rendering is supported.				 */
/*																			 */
/*  All of the rendering commands are translations of the OpenGL immediate	 */
/*	mode API so they should be familiar to programmers who have experience	 */
/*	with graphics programming.												 */
/*																			 */
/*****************************************************************************/

#ifndef _ECDS_RENDERER_H
#define _ECDS_RENDERER_H

#include <ecds.h>

#include <core/ecds_process.h>
typedef struct _ecds_render_instruction_t ecds_render_instruction_t;

struct _ecds_render_buffer_entry_t {
	unsigned int	opcode;		//!< Instruction opcode, for the definition of those constants, see below.
	unsigned int	length;		//!< Length of render buffer entry in bytes
	void * data;				//!< User data belonging to instruction
};

typedef struct _ecds_renderer_t ecds_renderer_t;
typedef struct _ecds_renderer_private_t ecds_renderer_private_t;

struct _ecds_renderer_t {
	ecds_process_t process;

	/* Renderer implementations should implement these API calls or interpret the instructions in the
	   rendering buffer on their own. */
	int (*begin_geometry)(ecds_renderer_t* renderer, unsigned int draw_mode);		//!< Analogous to glBegin()
	int (*end_geometry)(ecds_renderer_t* renderer);									//!< Analogous to glEnd()
	int (*begin_transform)(ecds_renderer_t* renderer);								//!< Or glPushMatrix()
	int (*end_transform)(ecds_renderer_t* renderer);								//!< glPopMatrix()
	int (*new_vertex)(ecds_renderer_t* renderer, double x, double y, double z);
	int (*set_colour)(ecds_renderer_t* renderer, double a, double r, double g, double b);
};

struct _ecds_renderer_private_t {
	ecds_object_t obj;

	ecds_list_t* buffers;
	unsigned int active_buffer;
};


//===========================================================================//
//	Draw mode constants														 //
//===========================================================================//
#define ECDS_DRAW_MODE_VERTICES		0
#define ECDS_DRAW_MODE_LINE			1
#define ECDS_DRAW_MODE_LINE_LOOP	2
#define ECDS_DRAW_MODE_LINE_STRIP	3
#define ECDS_DRAW_MODE_POLYGON		4
#define ECDS_DRAW_MODE_TRIANGLE		5
#define ECDS_DRAW_MODE_QUAD			6
#define ECDS_DRAW_MODE_TEXT			7

//===========================================================================//
//	Rendering instruction constants											 //
//===========================================================================//
#define ECDS_RENDER_NOP					0
#define ECDS_RENDER_GEOMETRY_BEGIN		1
#define ECDS_RENDER_GEOMETRY_END		2
#define ECDS_RENDER_TRANSFORM_BEGIN		3
#define ECDS_RENDER_TRANSFORM_END		4
#define ECDS_RENDER_PARAMETER			5
#define ECDS_RENDER_BUFFER_CREATE		252
#define ECDS_RENDER_BUFFER_SELECT		253
#define ECDS_RENDER_COMMIT				254
#define ECDS_RENDER_CLEAR				255

//===========================================================================//
//	Immediate mode interaction functions									 //
//===========================================================================//
/**
 * @brief Forcibly re-draw all geometry that exists in memory.
 */
void ecds_renderer_redraw(ecds_renderer_t * renderer);

/**
 * @brief Clear a specified render buffer in the rendering memory.
 * @param renderer The renderer to act on.
 * @param buffer_id The buffer ID to clear, using an ID of 0 will clear the entire memory (blank the screen).
 */
void ecds_renderer_clear_buffer(ecds_renderer_t * renderer, unsigned int buffer_id);

/**
 * @brief Allocate a new buffer and append it to the rendering chain.
 * @param renderer The renderer to act on.
 * @return The ID of the newly allocated buffer, or a negative value if an error occured.
 */
int ecds_renderer_new_buffer(ecds_renderer_t* renderer);

/**
 * @brief Select a specific rendering buffer to manipulate.
 * @param renderer The renderer to act on.
 * @param buffer_id The buffer ID to select.
 * @return The ID of the selected buffer if the function was succesful, or a negative value if the function failed.
 */
int ecds_renderer_select_buffer(ecds_renderer_t* renderer, unsigned int buffer_id);

/**
 * @brief Start a new geometry chain for rendering.
 * @param renderer The renderer to act on.
 * @param draw_mode One of the ECDS_DRAW_MODE constants to specify the rendering style.
 * @return The identifier of the geometry chain that has been allocated.
 */
int ecds_renderer_begin_geometry(ecds_renderer_t* renderer, unsigned int draw_mode);

/**
 * @brief Close an active geometry chain.
 * @param renderer The renderer to act on.
 * @return The identifier of the geometry chain that is now active.
 */
int ecds_renderer_end_geometry(ecds_renderer_t* renderer);

/**
 * @brief Start a new transform operation (push the transform stack).
 * @param renderer The renderer to act on.
 * @param draw_mode One of the ECDS_TRANSFORM constants to specify which transformation to use.
 * @return The identifier of the transformation that has been allocated.
 */
int ecds_renderer_begin_transform(ecds_renderer_t* renderer, unsigned int transform_mode);

/**
 * @brief Close an active transformation (pop the transform stack).
 * @param renderer The renderer to act on.
 * @return The identifier of the new active transform operation.
 */
int ecds_renderer_end_transform(ecds_renderer_t* renderer);

int ecds_renderer_copy_buffer(ecds_renderer_t* renderer, unsigned int* buffer_length, ecds_render_instruction_t* target);
/**
 * @brief Generic parameter passing function.
 * @param renderer The renderer to act on.
 * @param parameter_number The parameter identifier to manipulate.
 * @param parameter_length The size of the parameter data in bytes.
 * @param parameter_data A pointer to the data memory where the parameter data is stored. This data is copied, not dereferenced.
 * @return The number of bytes copied on success or a negative value on failure.
 */
int ecds_renderer_parameter(
			ecds_renderer_t * renderer, 
			int parameter_number, 
			int parameter_length, 
			void * parameter_data);

//===========================================================================//													
// The following functions are convenience functions which do pre-set tasks  //
//===========================================================================//
/**
 * @brief Passes a parameter of 3 double-precision floating points into the rendering buffer.
 * @param renderer The renderer to act on.
 * @param x, y, z The position values of the vertex to enter.
 * @return The number of bytes copied on success or a negative value on failure.
 */
int ecds_renderer_add_vertex(ecds_renderer_t* renderer, double x, double y, double z);

/**
 * @brief Add a rectangle to the active rendering buffer with a given width and height.
 */
int ecds_renderer_rectangle(ecds_renderer_t* renderer, double width, double height);

/**
 * @brief Add an ellipse to the active rendering buffer with a given width and height.
 */
int ecds_renderer_ellipse(ecds_renderer_t* renderer, double width, double height);

/**
 * @brief Add an elliptical arc to the active rendering buffer with a given width and height.
 */
int ecds_renderer_arc(ecds_renderer_t* renderer, double width, double height, double start_angle, double end_angle);

/**
 * @brief Add a string of text to the rendering buffer using the active font.
 */
int ecds_renderer_text(ecds_renderer_t* renderer, const char * text);

/**
 * @brief Changes the active rendering colour.
 * @param renderer The renderer to act on.
 * @param alpha, red, green, blue The colour channel values to use.
 * @return The number of bytes copied on success or a negative value on failure.
 */
int ecds_renderer_set_colour(ecds_renderer_t* renderer, double alpha, double red, double green, double blue);

#endif