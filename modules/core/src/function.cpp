//
// Created by crax on 11/7/21.
//

#include "object/function.h"
#include "object/field.h"

#include <iostream>

bool callable::ensure_params_same_type(const std::vector<std::unique_ptr<bound_field>> &params) {
    auto in_param = params.begin();
    for (auto &m_parameter: m_parameters) {
        const auto &in_param_type = m_parameter->get_type();
        const auto &this_param_type = (*in_param)->get_field_type()->get_type();
        if (in_param_type != this_param_type) {
            return false;
        }
        in_param++;
    }
    return true;
}

