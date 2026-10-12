#pragma once

#include "numbers.h"

#include <vector>

struct MovegenTTEntry {
    u64 hash;
    u64 num_moves;
    u16 depth;

    MovegenTTEntry() = default;

    MovegenTTEntry(const u64 hash, const u64 depth, const u64 num_moves) {
        this->hash = hash;
        this->num_moves = num_moves;
        this->depth = depth;
    };
};

class MovegenTranspositionTable {
    std::vector<MovegenTTEntry> entries;

public:
    MovegenTranspositionTable(const u64 new_size_mib = 16) {
        const u64 size = new_size_mib * 1024 * 1024 / sizeof(MovegenTTEntry);
        this->entries.resize(size);
        for (int i = 0; i < size; i++) {
            entries[i].hash = i + 1; // Make sure that there will never be a hit on an uninitialized entry
        }
    }

    u64 index(const u64 key) const {
        return static_cast<u64>((static_cast<u128>(key) * static_cast<u128>(entries.size())) >> 64);
    }

    void prefetch(const u64 key) {
        __builtin_prefetch(&this->get(key));
    }

    MovegenTTEntry& get(const u64 key) {
        return entries[index(key)];
    }
};