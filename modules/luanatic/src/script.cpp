//
// Created by crax on 11/7/21.
//

#include "luanatic/script.h"
#include "logging/logger.h"

logging::category log_script("script");

void luanatic::script::log_error() const {
    log_script(logging::severity::error) << "While executing script:\n\t" << get<std::string>(m_state);
}
