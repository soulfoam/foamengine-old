#pragma once

#include "shader.h"

void   shader_module_init(void);
void   shader_module_cleanup(void);
Shader shader_module_get_shader(ShaderType type);
void   shader_module_set_active_shader(Shader shader);
Shader shader_module_get_active_shader(void);

