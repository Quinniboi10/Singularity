#pragma once

namespace chess {
    struct Config {
        bool uci_chess960 = false;
    };

    inline Config singularity_config;
}