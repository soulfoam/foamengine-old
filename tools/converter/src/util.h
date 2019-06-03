#include <assimp/cimport.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <cglm/cglm.h>

typedef enum
{
	OBJ,
	FBX,
	DAE
} f_type;


static inline void
set_input_file_type(const char *input_path, f_type *type)
{
	if (strstr(input_path, ".obj") != NULL) *type = OBJ;
	if (strstr(input_path, ".fbx") != NULL) *type = FBX;
	if (strstr(input_path, ".dae") != NULL) *type = DAE;
}

static inline bool
f_eq(float a, float b)
{
	float dec = 1000000.0f;
	a = floor(dec*a)/dec;
	b = floor(dec*b)/dec;

	return a == b;
}

static inline void
aiMat4_to_mat4(struct aiMatrix4x4 *from, mat4 to)
{	
	to[0][0] = from->a1; to[1][0] = from->a2;
	to[2][0] = from->a3; to[3][0] = from->a4;
	to[0][1] = from->b1; to[1][1] = from->b2;
	to[2][1] = from->b3; to[3][1] = from->b4;
	to[0][2] = from->c1; to[1][2] = from->c2;
	to[2][2] = from->c3; to[3][2] = from->c4;
	to[0][3] = from->d1; to[1][3] = from->d2;
	to[2][3] = from->d3; to[3][3] = from->d4;
}

