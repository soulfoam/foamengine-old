#pragma once

#include "model.h"

void model_module_init(void);
void model_module_cleanup(void);
void model_module_add_model(Model *model);
void model_module_remove_model(Model *model);
void model_module_update(void);
