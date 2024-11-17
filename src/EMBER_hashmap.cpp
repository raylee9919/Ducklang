/* ========================================================================
   $File: $
   $Date: $
   $Revision: $
   $Creator: Sung Woo Lee $
   $Notice: (C) Copyright 2024 by Sung Woo Lee. All Rights Reserved. $
   ======================================================================== */

static u32
hash_string(String key) {
    u32 result = 2166136261;
    for (int i = 0; i < key.length; i++) {
        result ^= (u8)key.data[i];
        result *= 16777619;
    }
    return result;
}

struct Hashmap_Val {
    Eval_Type type;
    s32 offset;
    u32 length;
#if 0
    union {
        s32 int_val;
        f32 f32_val;
        f64 f64_val;
    };
#endif
};

struct Hashmap_Node {
    String key;
    Hashmap_Val val;
    Hashmap_Node *next;
};

struct Hashmap_Entry {
    Hashmap_Node *first;
};

struct Hashmap {
    void init(u32 _entry_count) {
        entry_count = _entry_count;
        entries = (Hashmap_Entry *)malloc(sizeof(Hashmap_Entry) * entry_count);
        for (u32 i = 0; i < entry_count; ++i) { entries[i].first = 0; }
    }

    void push(String key, Hashmap_Val val) {
        u32 entry_idx = (hash_string(key) % entry_count);
        Hashmap_Entry *entry = (entries + entry_idx);

        Hashmap_Node *node = entry->first;
        while (node && node->next != 0) {
            if (node->key == key) return;
            node = node->next;
        }

        Hashmap_Node *new_node = (Hashmap_Node *)malloc(sizeof(Hashmap_Node));
        *new_node = Hashmap_Node{key, val, 0};
        if (node) {
            node->next = new_node;
        } else {
            entry->first = new_node;
        }
    }
    inline void push(const char *str, Hashmap_Val val) { push(_String(str), val); }

    Hashmap_Val get(String key) {
        Hashmap_Val result;

        u32 entry_idx = (hash_string(key) % entry_count);
        Hashmap_Entry *entry = (entries + entry_idx);

        Hashmap_Node *node = entry->first;
        while (node && node->key != key) node = node->next;

        if (node != 0) {
            result = node->val;
        } else {
            result.type = EVAL_ERROR;
        }

        return result;
    }
    inline Hashmap_Val get(const char *str) { return get(_String(str)); }

    void remove(String key) {
        u32 entry_idx = (hash_string(key) % entry_count);
        Hashmap_Entry *entry = (entries + entry_idx);

        Hashmap_Node *node = entry->first;
        if (node && node->key == key) {
            entry->first = node->next;
            free(node);
        } else {
            Hashmap_Node *next_node;
            for (;;) {
                next_node = node->next;
                if (!next_node || next_node->key == key) break;
                node = node->next;
            }

            node->next = next_node->next;
            free(node);
        }
    }
    inline void remove(const char *str) { remove(_String(str)); }


    u32 entry_count;
    Hashmap_Entry *entries;
};

