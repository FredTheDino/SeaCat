// Included in the logic namespace, extracted out of the file to make it
// more readable.

template <typename T>
EMeta::EField generate_field_info(const char *name, u64 offset, u64 hash) {
    return {name, offset, hash};
}

template <class T, class M> M _fog_member_type(M T:: *);

#define GET_FIELD_TYPE(Base, field) decltype(Logic::_fog_member_type(&Base::field))

#define GEN_FIELD_INFO(E, f, ...)                                           \
    Logic::generate_field_info<GET_FIELD_TYPE(E, f)>(                       \
        "" #f "", offsetof(E, f), typeid(GET_FIELD_TYPE(E, f)).hash_code(), \
        ##__VA_ARGS__)

#define EXPAND_FIELDS_EXPORT(member) GEN_FIELD_INFO(self, member),

#define REGISTER_FIELDS(EnumType, SelfType, ...)                              \
    virtual const char *type_name() override { return #EnumType; }            \
    virtual Logic::EntityType type() override {                               \
        return Logic::EntityType::EnumType;                                   \
    }                                                                         \
    static constexpr Logic::EntityType st_type() {                            \
        return Logic::EntityType::EnumType;                                   \
    }                                                                         \
    static Logic::EMeta _fog_generate_meta() {                                \
        using self = SelfType;                                                \
        Logic::EMeta::EField _fog_fields[] = {                                \
            MAP(EXPAND_FIELDS_EXPORT, __VA_ARGS__)};                          \
        Logic::EMeta::EField *_fog_fields_mem =                               \
            Util::push_memory<Logic::EMeta::EField>(LEN(_fog_fields));        \
        Util::copy_bytes(&_fog_fields, _fog_fields_mem, sizeof(_fog_fields)); \
        return {Logic::EntityType::EnumType,                                  \
                typeid(SelfType).hash_code(),                                 \
                sizeof(self),                                                 \
                LEN(_fog_fields),                                             \
                _fog_fields_mem,                                              \
                true};                                                        \
    }

#define REGISTER_NO_FIELDS(EnumType, SelfType)                     \
    virtual const char *type_name() override { return #EnumType; } \
    virtual Logic::EntityType type() override {                    \
        return Logic::EntityType::EnumType;                        \
    }                                                              \
    static constexpr Logic::EntityType st_type() {                 \
        return Logic::EntityType::EnumType;                        \
    }                                                              \
    static Logic::EMeta _fog_generate_meta() {                     \
        return {Logic::EntityType::EnumType,                       \
                typeid(SelfType).hash_code(),                      \
                sizeof(SelfType),                                  \
                0,                                                 \
                nullptr,                                           \
                true};                                             \
    }

#define REGISTER_ENTITY(T)                                                 \
    do {                                                                   \
        static_assert(std::is_base_of<Logic::Entity, T>(),                        \
                      "Cannot register non-entity component");               \
        ASSERT(!Logic::_fog_global_entity_list[(u32) T::st_type()].registered, \
                   "Trying to register same entity type multiple times");  \
        Logic::_fog_global_entity_vtable[(u32) T::st_type()] = []() -> void *{ T t = {}; return *((void **) &t); }; \
        Logic::_fog_global_entity_list[(u32) T::st_type()] =               \
            T::_fog_generate_meta();                                        \
        REGISTER_TYPE(T);\
    } while (false);


#define REGISTER_TYPE(Type, ...) \
    Logic::register_type({typeid(Type).hash_code(), "" #Type "", sizeof(Type), ##__VA_ARGS__})

